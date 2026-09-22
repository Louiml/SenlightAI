"""RLEF alignment via DPO with **reward-model-sampled preference pairs**.

Fixes the earlier DPO (which used meaningless reversed-text rejections) by
building preference pairs the way the guide intends:
  1. Sample K candidate responses per prompt from the *reference* policy (SFT).
  2. Score every candidate with the trained **Affective Reward Model**.
  3. chose  = highest-reward candidate (empathetic, fluent)
     reject = lowest-reward candidate (drifting / off-topic)
  4. Optimize the policy with the standard DPO objective against the frozen
     reference, so it increases probability of the reward-preferred response.

Loss, for a pair (x, y_w, y_l):
    -log σ( β ( log π(y_w|x) − log π_ref(y_w|x)
               − log π(y_l|x) + log π_ref(y_l|x) ) )

Usage:
    python dpo.py --steps 2500 --batch-size 2 --sft senlight_psy_sft \
                  --reward affective_reward --out senlight_psy_dpo
"""

from __future__ import annotations

import argparse
import json
import math
import time
import warnings
from pathlib import Path

import torch
import torch.nn.functional as F
from torch.optim import AdamW
from torch.optim.lr_scheduler import LambdaLR

import config as C
from dataset import load_tokenizer
from model import SenlightCoder

warnings.filterwarnings("ignore", message=".*lr_scheduler.*")

SFT_JSONL = C.ARTIFACTS_DIR / "psy_sft.jsonl"
PURE_JSONL = C.ARTIFACTS_DIR / "psy_sft_pure.jsonl"
RNG = torch.Generator().manual_seed(1)
EOS_ID = C.EOS_ID
PAD_ID = C.PAD_ID

# Generation / preference-pair hyperparameters.
MAX_PROMPT = 128      # keep the prompt short so we can afford candidates
MAX_COMPLETION = 48   # max length of a sampled candidate
N_CANDIDATES = 4      # candidates sampled per prompt ("K" above)
GEN_TEMP = 0.9
GEN_TOP_K = 40


def prompt_from(row: dict) -> str:
    inst = row["instruction"].strip()
    inp = (row.get("input") or "").strip()
    if inp:
        return f"{inst}\n\nUser: {inp}\n\nAssistant:"
    return f"{inst}\n\nAssistant:"


def _ids(tok, text: str, max_len: int) -> list[int]:
    ids = tok.encode(text, add_special_tokens=False).ids
    return ids[:max_len]


# ---------------------------------------------------------------------------
# Batched autoregressive sampling from a generator policy
# ---------------------------------------------------------------------------
@torch.no_grad()
def sample_completions(
    model: SenlightCoder,
    tok,
    prompts: list[str],
    n: int = N_CANDIDATES,
    max_new: int = MAX_COMPLETION,
    temperature: float = GEN_TEMP,
    top_k: int = GEN_TOP_K,
) -> tuple[list[list[str]], list[list[list[int]]]]:
    """For each prompt, sample ``n`` continuation strings (and id lists)."""
    device = next(model.parameters()).device
    # One prompt at a time to keep memory small on a 100M model.
    all_str: list[list[str]] = []
    all_ids: list[list[list[int]]] = []

    for prompt in prompts:
        pid = _ids(tok, prompt, MAX_PROMPT)
        # Sample n continuations greedily-ish (stochastic, no batched cache).
        candidates_ids: list[list[int]] = []
        for _ in range(n):
            seq = list(pid)
            for _ in range(max_new):
                inp = torch.tensor(seq[-512:], dtype=torch.long, device=device).unsqueeze(0)
                logits, _ = model(inp)
                next_logits = logits[0, -1, :]           # (V,)
                next_logits = next_logits / max(temperature, 1e-6)
                if top_k > 0:
                    v, _ = torch.topk(next_logits, min(top_k, next_logits.shape[0]))
                    next_logits = torch.where(next_logits < v[-1],
                                              torch.full_like(next_logits, -1e9), next_logits)
                probs = F.softmax(next_logits.float(), dim=-1)
                nxt = torch.multinomial(probs, 1).item()
                if nxt == EOS_ID:
                    break
                seq.append(nxt)
                if len(seq) - len(pid) >= max_new:
                    break
            candidates_ids.append(seq[len(pid):])  # response portion only
        # Decode strings for reward scoring later (kept light here).
        all_ids.append(candidates_ids)
        all_str.append([tok.decode(c, True) for c in candidates_ids])
    return all_str, all_ids


# ---------------------------------------------------------------------------
# Reward model wrapper (frozen backbone + reward head)
# ---------------------------------------------------------------------------
class RewardScorer:
    """Scores a (prompt, response) — requires the affective reward checkpoint."""

    def __init__(self, backbone: SenlightCoder, reward_path: Path, device: torch.device):
        self.backbone = backbone.to(device).eval()
        self.head = torch.nn.Linear(C.MODEL_DIM, 1, bias=False).to(device)
        sd = torch.load(reward_path, map_location=device)
        # Reward full-state dict has "backbone.*" and "reward_head.*" keys.
        backbone_sd = {k[len("backbone."):]: v for k, v in sd.items() if k.startswith("backbone.")}
        head_sd = {k[len("reward_head."):]: v for k, v in sd.items() if k.startswith("reward_head.")}
        if backbone_sd:
            self.backbone.load_state_dict(backbone_sd, strict=False)
        if head_sd:
            self.head.load_state_dict(head_sd)
        for p in self.backbone.parameters():
            p.requires_grad_(False)
        for p in self.head.parameters():
            p.requires_grad_(False)

    @torch.no_grad()
    def score(self, prompt: str, response: str, tok, device) -> float:
        text = prompt + response
        ids = _ids(tok, text, 384)
        if not ids:
            return 0.0
        inp = torch.tensor(ids, dtype=torch.long, device=device).unsqueeze(0)
        h = self.backbone.get_hidden_states(inp)          # (1, S, dim)
        last = h[0, -1, :]                                # (dim,)
        return float(self.head(last).squeeze().item())


# ---------------------------------------------------------------------------
# Preference-pair construction using the reward model
# ---------------------------------------------------------------------------
def build_pairs(
    generator: SenlightCoder, reward: RewardScorer, tok,
    rows: list[dict], device, n_rows: int, gold_weight: int = 1,
) -> list[dict]:
    """For sampled prompts, produce {prompt, chosen, rejected} pairs.

    Candidates = gold output (repeated gold_weight times) + N samples from the
    generator policy. Reward-scored; chosen = argmax, rejected = argmin.
    """
    pairs: list[dict] = []
    for row in rows:
        prompt = prompt_from(row)
        gold = row["output"].strip()
        cands_str, cands_ids = sample_completions(generator, tok, [prompt], n=N_CANDIDATES)
        candidates = [gold] * gold_weight + cands_str[0]
        candidates = [c for c in candidates if c and len(c.strip()) > 8]
        if len(candidates) < 2:
            continue
        scores = [reward.score(prompt, c, tok, device) for c in candidates]
        best_i = max(range(len(candidates)), key=lambda i: scores[i])
        worst_i = min(range(len(candidates)), key=lambda i: scores[i])
        chosen, rejected = candidates[best_i], candidates[worst_i]
        if chosen.strip() == rejected.strip() or not chosen.strip() or not rejected.strip():
            continue
        pairs.append({"prompt": prompt, "chosen": chosen, "rejected": rejected})
    return pairs


# ---------------------------------------------------------------------------
# DPO core (shared by offline pairs)
# ---------------------------------------------------------------------------
def sequence_logprob(model, prompt: torch.Tensor, response: torch.Tensor,
                     pad_id: int = PAD_ID) -> torch.Tensor:
    """mean log π(response | prompt) over response tokens."""
    p_len = (prompt != pad_id).sum(dim=1)
    r_len = (response != pad_id).sum(dim=1)
    B = prompt.size(0)
    rows, pls, rls = [], [], []
    for i in range(B):
        p = prompt[i][: p_len[i]].tolist()
        r = response[i][: r_len[i]].tolist()
        # Truncate from the PROMPT side so the response is preserved.
        budget = 384
        if len(p) + len(r) > budget:
            keep_p = budget - len(r)
            p = p[max(0, len(p) - keep_p):] if keep_p > 0 else []
        joint = p + r
        rows.append(torch.tensor(joint, dtype=torch.long, device=prompt.device))
        pls.append(len(p))
        rls.append(min(len(r), max(1, budget - len(p))))
    S = max(len(x) for x in rows)
    joint = torch.zeros((B, S), dtype=torch.long, device=prompt.device)
    for i, x in enumerate(rows):
        joint[i, : len(x)] = x
    grad_guard = torch.enable_grad()
    with grad_guard:
        logits, _ = model(joint)
    targets = joint[:, 1:]
    logits_pos = logits[:, :-1]
    nll = F.cross_entropy(logits_pos.reshape(-1, logits_pos.size(-1)),
                          targets.reshape(-1), reduction="none").reshape(B, S - 1)
    mask = torch.zeros_like(nll)
    for i in range(B):
        pl = pls[i]; rl = rls[i]
        end = min(pl + rl, S - 1)
        if end > pl:
            mask[i, pl:end] = 1.0
    denom = mask.sum(dim=1).clamp(min=1)
    return -(nll * mask).sum(dim=1) / denom


def dpo_loss(policy, reference, prompt, chosen, rejected, beta=0.1):
    with torch.no_grad():
        rc_w = sequence_logprob(reference, prompt, chosen)
        rc_l = sequence_logprob(reference, prompt, rejected)
    lp_w = sequence_logprob(policy, prompt, chosen)
    lp_l = sequence_logprob(policy, prompt, rejected)
    ratio = (lp_w - rc_w) - (lp_l - rc_l)
    return -F.logsigmoid(beta * ratio).mean()


# ---------------------------------------------------------------------------
# main
# ---------------------------------------------------------------------------
def train(args) -> None:
    device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
    print(f"[dpo] device = {device}")
    tok = load_tokenizer()

    # Load reference (frozen) = SFT model.
    ref = SenlightCoder().to(device).eval()
    sft_pt = C.ARTIFACTS_DIR / f"{args.sft}.pt"
    ref.load_state_dict(torch.load(sft_pt, map_location=device))
    for p in ref.parameters():
        p.requires_grad_(False)
    print(f"[dpo] reference (SFT) loaded: {args.sft}")

    policy = SenlightCoder().to(device)
    policy.load_state_dict({k: v.clone() for k, v in ref.state_dict().items()})

    # Reward model for preference-pair sampling.
    reward_path = C.ARTIFACTS_DIR / f"{args.reward}.pt"
    if not reward_path.exists():
        raise FileNotFoundError(f"reward model missing: {reward_path} — train reward_model.py first")
    reward = RewardScorer(ref, reward_path, device)
    print(f"[dpo] reward model loaded: {args.reward}")

    # Preference pairs — reused from cache if present (pair building is heavy).
    pairs_path = C.ARTIFACTS_DIR / f"dpo_pairs_{args.cache}.jsonl"
    if pairs_path.exists():
        print(f"[dpo] loading cached preference pairs: {pairs_path.name}")
        pairs = [json.loads(l) for l in open(pairs_path, encoding="utf-8")]
    else:
        rows = [json.loads(l) for l in open(PURE_JSONL, encoding="utf-8")]
        import random
        random.seed(args.seed)
        sub = random.sample(rows, min(args.pairs_prompts, len(rows)))
        print(f"[dpo] building preference pairs from {len(sub)} prompts "
              f"x {N_CANDIDATES} candidates ...")
        t0 = time.time()
        pairs = build_pairs(policy, reward, tok, sub, device,
                            n_rows=len(sub), gold_weight=args.gold_weight)
        print(f"[dpo] built {len(pairs)} reward-model preference pairs "
              f"in {time.time() - t0:.0f}s")
        pairs_path.write_text("\n".join(json.dumps(p) for p in pairs), encoding="utf-8")
        print(f"[dpo] cached pairs -> {pairs_path.name}")

    def ids_t(text):
        return torch.tensor(_ids(tok, text, 384), dtype=torch.long)

    def stack_pad(list_of_tensors):
        """Right-pad a list of tensors to a common length for batching."""
        L = max(t.size(0) for t in list_of_tensors)
        out = torch.zeros((len(list_of_tensors), L), dtype=torch.long)
        for i, t in enumerate(list_of_tensors):
            out[i, : t.size(0)] = t
        return out

    opt = AdamW(policy.parameters(), lr=args.lr, weight_decay=0.1)

    def lr_fn(step):
        w = args.warmup
        if step < w:
            return float(step + 1) / max(1, w)
        prog = min(1.0, (step - w) / max(1, args.steps - w))
        return 0.5 * (1.0 + math.cos(math.pi * prog))

    sched = LambdaLR(opt, lr_lambda=lr_fn)
    policy.train()
    start = time.time()
    for step in range(1, args.steps + 1):
        idxs = torch.randint(0, len(pairs), (args.batch_size,), generator=RNG).tolist()
        ps, cs, rs = [], [], []
        for i in idxs:
            pa = pairs[i]
            ps.append(ids_t(pa["prompt"]))
            cs.append(ids_t(pa["chosen"]))
            rs.append(ids_t(pa["rejected"]))
        prompt = stack_pad(ps).to(device)
        chosen = stack_pad(cs).to(device)
        rejected = stack_pad(rs).to(device)

        opt.zero_grad(set_to_none=True)
        loss = dpo_loss(policy, ref, prompt, chosen, rejected, beta=args.beta)
        loss.backward()
        torch.nn.utils.clip_grad_norm_(policy.parameters(), 1.0)
        opt.step()
        sched.step()

        if step % args.log_every == 0:
            dt = time.time() - start
            print(f"[dpo] step {step:>5}/{args.steps} loss {loss.item():.4f} "
                  f"lr {opt.param_groups[0]['lr']:.2e} {args.log_every/dt:.1f} step/s")
            start = time.time()

    torch.save(policy.state_dict(), C.ARTIFACTS_DIR / f"{args.out}.pt")
    from train import export_to_safetensors
    export_to_safetensors(policy, C.ARTIFACTS_DIR / f"{args.out}.safetensors")
    print(f"[dpo] saved -> {C.ARTIFACTS_DIR / (args.out + '.safetensors')}")


def parse_args():
    p = argparse.ArgumentParser()
    p.add_argument("--steps", type=int, default=2000)
    p.add_argument("--batch-size", type=int, default=2)
    p.add_argument("--lr", type=float, default=1e-5)
    p.add_argument("--beta", type=float, default=0.1)
    p.add_argument("--warmup", type=int, default=80)
    p.add_argument("--log-every", type=int, default=200)
    p.add_argument("--sft", default="senlight_psy_sft")
    p.add_argument("--reward", default="affective_reward")
    p.add_argument("--out", default="senlight_psy_dpo")
    p.add_argument("--pairs-prompts", type=int, default=1200,
                   help="how many prompts to build reward-sampled pairs from (when not cached)")
    p.add_argument("--cache", type=str, default="offline",
                   help="cache stem for saved preference pairs")
    p.add_argument("--gold-weight", type=int, default=1,
                   help="times the gold output is included as a candidate")
    p.add_argument("--seed", type=int, default=7)
    return p.parse_args()


if __name__ == "__main__":
    train(parse_args())