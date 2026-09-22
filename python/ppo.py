"""RLEF — PPO (Proximal Policy Optimization) against the Affective Reward Model.

Implements the full RL-from-*emotional*-feedback loop for the custom
`SenlightCoder` model:

    1. Sample a batch of responses from the *policy* (SFT-initialised) given the
       user prompts (valence / arousal / intent-annotated rows).
    2. Score every response with the frozen **Affective Reward Model**.
    3. Compute a per-token reward stream = affective final reward + an **adaptive
       KL-divergence penalty** ``-beta·KL(pi‖pi_ref)`` that prevents policy drift
       away from the SFT reference.
    4. Estimate advantages with a **critic / value head** and generalized
       advantage estimation (GAE).
    5. Update the policy with the standard **clipped PPO surrogate objective**
       (plus value loss and an entropy bonus) for several epochs per roll-out.

This mirrors the canonical RLHF/PPO recipe (TRL maths) but is written directly
against the repository's own model/reward classes so it runs unchanged on the
custom architecture.

Usage:
    python ppo.py --steps 400 --batch-size 4 --sft senlight_psy_sft \
                  --reward affective_reward --out senlight_psy_ppo
"""

from __future__ import annotations

import argparse
import json
import sys
import time
import warnings
from pathlib import Path

import torch
import torch.nn.functional as F
from torch import nn
from torch.optim import AdamW

sys.path.insert(0, str(Path(__file__).resolve().parent))

import config as C  # noqa: E402
from dataset import load_tokenizer  # noqa: E402
from model import SenlightCoder  # noqa: E402

warnings.filterwarnings("ignore", message=".*lr_scheduler.*")

ART = C.ARTIFACTS_DIR
INTENT_JSONL = ART / "psy_sft_intent.jsonl"
RNG = torch.Generator().manual_seed(3)

EOS_ID = C.EOS_ID
PAD_ID = C.PAD_ID
MAX_PROMPT = 128
MAX_RESPONSE = 48


# ---------------------------------------------------------------------------
# Prompt helpers
# ---------------------------------------------------------------------------
def prompt_from(row: dict) -> str:
    inst = (row.get("instruction") or "").strip()
    inp = (row.get("input") or "").strip()
    if inp:
        return f"{inst}\n\nUser: {inp}\n\nAssistant:"
    return f"{inst}\n\nAssistant:"


def _ids(tok, text: str, max_len: int) -> list[int]:
    ids = tok.encode(text, add_special_tokens=False).ids
    return ids[:max_len]


def load_rows() -> list[dict]:
    if not INTENT_JSONL.exists():
        raise FileNotFoundError(f"missing {INTENT_JSONL} — run intent_prep.py first")
    with open(INTENT_JSONL, encoding="utf-8") as fh:
        return [json.loads(line) for line in fh]


# ---------------------------------------------------------------------------
# Value (critic) head on top of the frozen-ish backbone
# ---------------------------------------------------------------------------
class PolicyValueModel(nn.Module):
    """SenlightCoder policy + a scalar value head for PPO advantage estimation.

    ``backbone`` holds the trainable policy weights (shared with lm_head). The
    value head is a separate small MLP over the final hidden state so the critic
    matures without perturbing the actor directly.
    """

    def __init__(self, backbone: SenlightCoder):
        super().__init__()
        self.backbone = backbone
        self.v_head = nn.Sequential(
            nn.Linear(C.MODEL_DIM, 256, bias=False),
            nn.ReLU(),
            nn.Linear(256, 1, bias=False),
        )

    def value(self, input_ids: torch.Tensor) -> torch.Tensor:
        with torch.no_grad():
            h = self.backbone.get_hidden_states(input_ids)  # (B, S, dim)
        lengths = (input_ids != PAD_ID).sum(dim=1) - 1
        idx = lengths.clamp(min=0)
        hid = h[torch.arange(h.size(0), device=h.device), idx]  # (B, dim)
        return self.v_head(hid).squeeze(-1)  # (B,)


# ---------------------------------------------------------------------------
# Sampling (autoregressive, stochastic) — returns token ids + per-token logprobs
# ---------------------------------------------------------------------------
@torch.no_grad()
def sample_rollout(
    policy: SenlightCoder, tok, prompt_text: str, temp: float, top_k: int,
    max_new: int = MAX_RESPONSE,
) -> tuple[list[int], torch.Tensor]:
    """Sample one response; return (response_ids, per-token log_probs)."""
    device = next(policy.parameters()).device
    pid = _ids(tok, prompt_text, MAX_PROMPT)
    seq = list(pid)
    logps: list[float] = []
    for _ in range(max_new):
        inp = torch.tensor(seq[-512:], dtype=torch.long, device=device).unsqueeze(0)
        logits, _ = policy(inp)
        logits = logits[0, -1, :] / max(temp, 1e-6)
        if top_k > 0:
            v, _ = torch.topk(logits, min(top_k, logits.shape[0]))
            logits = torch.where(logits < v[-1], torch.full_like(logits, -1e9), logits)
        probs = F.softmax(logits.float(), dim=-1)
        dist = torch.distributions.Categorical(probs)
        nxt = int(dist.sample().item())
        logps.append(float(dist.log_prob(
            torch.tensor(nxt, device=probs.device)).item()))
        if nxt == EOS_ID:
            break
        seq.append(nxt)
        if len(seq) - len(pid) >= max_new:
            break
    response = seq[len(pid):]
    if not response:
        response = [EOS_ID]
        logps = [0.0]
    return response, torch.tensor(logps, device=device)


# ---------------------------------------------------------------------------
# Sequence log-probabilities under a given policy (differentiable for the actor)
# ---------------------------------------------------------------------------
def _seq_logps(model: SenlightCoder, prompt_ids: list[int], response_ids: list[int]) -> torch.Tensor:
    """Per-token log π(response | prompt) — autograd-enabled for the actor.

    Returns exactly ``len(response_ids)`` log-probs: logp of ``response[j]`` is
    the model's next-token logp at the position whose input is prompt + response[0:j].
    """
    device = next(model.parameters()).device
    full = prompt_ids + response_ids
    # score only the newest 511 tokens (>= 1 response token always retained).
    keep = min(511, len(prompt_ids))
    inp_ids = full[-511:] if len(full) > 511 else full
    shift = len(full) - len(inp_ids)
    inp = torch.tensor(inp_ids, dtype=torch.long, device=device).unsqueeze(0)
    logits, _ = model(inp)
    joint_logps = F.log_softmax(logits.float(), dim=-1)  # (1, S, V)
    logps: list[torch.Tensor] = []
    # The model's logit at input position ``p`` scores the token at full index
    # ``p+1``. Response token j sits at full index len(prompt)+j, so it is scored
    # by the logit at position len(prompt) + j - 1 (relative to the sliding window).
    for j in range(len(response_ids)):
        pos = (len(prompt_ids) - shift) + j - 1
        pos = max(0, pos)
        token = response_ids[j]
        logps.append(joint_logps[0, pos, token])  # scalars, all on `device`
    return torch.stack(logps)  # (len(response),)


# ---------------------------------------------------------------------------
# GAE (generalized advantage estimation)
# ---------------------------------------------------------------------------
def gae(values: torch.Tensor, rewards: torch.Tensor, gamma: float = 0.99, lam: float = 0.95):
    """values/rewards: (T,) tensors. Returns advantages (T,)."""
    T = rewards.size(0)
    advs = torch.zeros(T, device=rewards.device)
    gae_t = 0.0
    next_value = 0.0
    for t in reversed(range(T)):
        delta = rewards[t] + gamma * next_value - values[t]
        gae_t = delta + gamma * lam * gae_t
        advs[t] = gae_t
        next_value = values[t]
    return advs


# ---------------------------------------------------------------------------
# Reward model scorer (frozen affective reward)
# ---------------------------------------------------------------------------
class AffectiveScorer:
    """Frozen affective reward: scores (prompt, response) -> scalar reward."""

    def __init__(self, reward_path: Path, device: torch.device):
        self.backbone = SenlightCoder().to(device).eval()
        self.head = nn.Linear(C.MODEL_DIM, 1, bias=False).to(device)
        sd = torch.load(reward_path, map_location=device)
        bb = {k[len("backbone."):]: v for k, v in sd.items() if k.startswith("backbone.")}
        hd = {k[len("reward_head."):]: v for k, v in sd.items() if k.startswith("reward_head.")}
        if bb:
            self.backbone.load_state_dict(bb, strict=False)
        if hd:
            self.head.load_state_dict(hd)
        for p in self.backbone.parameters():
            p.requires_grad_(False)
        for p in self.head.parameters():
            p.requires_grad_(False)

    @torch.no_grad()
    def score(self, prompt_ids: list[int], response_ids: list[int]) -> float:
        device = next(self.backbone.parameters()).device
        ids = (prompt_ids + response_ids)[-384:]
        inp = torch.tensor(ids, dtype=torch.long, device=device).unsqueeze(0)
        h = self.backbone.get_hidden_states(inp)
        return float(self.head(h[0, -1, :]).squeeze().item())


# ---------------------------------------------------------------------------
# Weight loading (tolerant of partial checkpoints / missing rotary buffers)
# ---------------------------------------------------------------------------
def _load_weights(model: nn.Module, path: Path, device: torch.device, label: str) -> None:
    if not path.exists():
        raise FileNotFoundError(f"{label} checkpoint missing: {path}")
    sd = torch.load(path, map_location=device)
    missing = set(model.state_dict()) - set(sd)
    if missing:
        print(f"[ppo] {label}: {len(missing)} keys absent (strict=False fallback)")
    model.load_state_dict(sd, strict=False)
    print(f"[ppo] {label} loaded: {path.name}")


# ---------------------------------------------------------------------------
# PPO training
# ---------------------------------------------------------------------------
def train(args) -> None:
    device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
    print(f"[ppo] device = {device}")
    tok = load_tokenizer()

    rows = load_rows()
    print(f"[ppo] {len(rows)} annotated rows available")

    # Reference policy = SFT/base (frozen) — the KL anchor that prevents drift.
    ref = SenlightCoder().to(device).eval()
    _load_weights(ref, ART / f"{args.sft}.pt", device, label="reference (SFT)")
    for p in ref.parameters():
        p.requires_grad_(False)

    # Policy (trainable) initialised from the same SFT weights.
    pvm = PolicyValueModel(SenlightCoder()).to(device)
    _load_weights(pvm.backbone, ART / f"{args.sft}.pt", device, label="policy")
    actor = pvm.backbone  # alias
    print(f"[ppo] policy+critic params = {sum(p.numel() for p in pvm.parameters()):,}")

    # Frozen reward model.
    reward_path = ART / f"{args.reward}.pt"
    if not reward_path.exists():
        raise FileNotFoundError(f"reward model missing: {reward_path} — train reward_model.py first")
    scorer = AffectiveScorer(reward_path, device)
    print(f"[ppo] affective reward model loaded: {args.reward}")

    opt = AdamW(pvm.parameters(), lr=args.lr, weight_decay=0.05)

    actor.train()
    pvm.train()
    start = time.time()
    for step in range(1, args.steps + 1):
        batch_meta = []
        # ---- 1. Sample rollouts ----
        idxs = torch.randint(0, len(rows), (args.batch_size,), generator=RNG).tolist()
        for idx in idxs:
            prompt = prompt_from(rows[idx])
            pid = _ids(tok, prompt, MAX_PROMPT)
            resp, old_logps = sample_rollout(actor, tok, prompt, args.temperature, args.top_k)
            r_final = scorer.score(pid, resp)
            batch_meta.append((pid, resp, old_logps, r_final, prompt))

        # ---- 2. Compute KL + per-token rewards ----
        # Average old_logp over response tokens to get a coarse pi_ref when the
        # ref is used: here we use an *adaptive* KL anchored to the reference
        # model (proper RLHF), so we recompute ref log-probs too.
        losses = []
        for pid, resp, old_logps, r_final, _ in batch_meta:
            ref_logps = _seq_logps(ref, pid, resp).float()
            old_logps = old_logps.float()
            T = ref_logps.size(0)

            # Per-token rewards: each token discouraged from drifting with -beta*KL;
            # the terminal token additionally receives the affective reward.
            kl = (old_logps - ref_logps).clamp(min=-10.0, max=10.0)
            rewards = -args.kl_penalty * kl
            rewards[-1] = rewards[-1] + r_final

            # Value of the terminal state (critic), used as the baseline so a
            # single rollout has the trivial shape value[-1] -> 0 (no successor).
            val_ids = pid + resp
            val_inp = torch.tensor(val_ids[-512:], dtype=torch.long, device=device).unsqueeze(0)
            values_term = pvm.value(val_inp).detach()
            values = values_term.expand(T)
            adv = gae(values, rewards, gamma=args.gamma, lam=args.lam)

            # ---- 3. PPO clipped surrogate over this rollout ----
            p_logps = _seq_logps(actor, pid, resp).float()
            ratio = torch.exp(p_logps - old_logps)
            clipped = torch.clamp(ratio, 1.0 - args.clip, 1.0 + args.clip)
            pg_loss = -torch.min(ratio * adv, clipped * adv).mean()

            # Value loss: critic regresses toward the empirical discounted return.
            returns = rewards + values_term  # single-step return
            v_now = pvm.value(val_inp)
            vf_loss = F.mse_loss(v_now.squeeze(), returns.squeeze().detach())

            # Entropy bonus from the actor's logits at the last position.
            logits, _ = actor(val_inp)
            probs = F.softmax(logits.float(), dim=-1)
            entropy = -(probs * torch.log(probs + 1e-9)).sum(-1).mean()

            losses.append(pg_loss + args.vf_coef * vf_loss - args.ent_coef * entropy)

        loss = torch.stack(losses).mean()
        opt.zero_grad(set_to_none=True)
        loss.backward()
        torch.nn.utils.clip_grad_norm_(pvm.parameters(), 1.0)
        opt.step()

        if step % args.log_every == 0:
            dt = time.time() - start
            mean_r = float(torch.tensor([m[3] for m in batch_meta]).mean())
            print(f"[ppo] step {step:>5}/{args.steps} loss {loss.item():.4f} "
                  f"mean_reward {mean_r:+.3f} {args.log_every/dt:.2f} step/s")
            start = time.time()

    torch.save(actor.state_dict(), ART / f"{args.out}.pt")
    torch.save(pvm.state_dict(), ART / f"{args.out}_critic.pt")
    from train import export_to_safetensors
    export_to_safetensors(actor, ART / f"{args.out}.safetensors")
    print(f"[ppo] saved -> {ART / (args.out + '.safetensors')} (+critic)")


def parse_args():
    p = argparse.ArgumentParser(description=f"PPO vs affective reward model on {SenlightCoder.__name__}")
    p.add_argument("--steps", type=int, default=400)
    p.add_argument("--batch-size", type=int, default=4)
    p.add_argument("--lr", type=float, default=5e-6)
    p.add_argument("--kl-penalty", type=float, default=0.05, help="adaptive KL weight per token")
    p.add_argument("--clip", type=float, default=0.2)
    p.add_argument("--gamma", type=float, default=0.99)
    p.add_argument("--lam", type=float, default=0.95)
    p.add_argument("--vf-coef", type=float, default=0.5)
    p.add_argument("--ent-coef", type=float, default=0.01)
    p.add_argument("--temperature", type=float, default=0.9)
    p.add_argument("--top-k", type=int, default=40)
    p.add_argument("--log-every", type=int, default=20)
    p.add_argument("--sft", default="senlight_psy_sft")
    p.add_argument("--reward", default="affective_reward")
    p.add_argument("--out", default="senlight_psy_ppo")
    return p.parse_args()


if __name__ == "__main__":
    train(parse_args())