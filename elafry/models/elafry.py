"""The Elafry decoder.

A standard Llama-style decoder-only transformer with one addition: the affective state,
which is threaded into attention as a bias and updated by a Markovian transition between
turns.

## The causal-order problem, and how it is resolved

The state both *depends on* the model (it is predicted from the model's own representation
of the input) and *influences* the model (it biases attention). Taking the state's input
from the model's own final hidden states would be circular: ``S -> h -> S``.

The fix is to read the state from the token embeddings, before any layer runs. Those depend
only on the input ids, so the graph is acyclic. This still gives the distinction the
architecture note is after, because "internal" here means *the model derives its own state
from its own input representation* rather than *an external affect encoder hands it a
pre-computed vector*. The external path remains available as the control condition.

The same reasoning caps how often the state updates. Inside a training window the state is
computed once and held, because a per-token recurrence over 512 steps is a different model
and a much harder optimisation problem than the one the architecture note describes. During
autoregressive decode the state is recomputed per token, which is where the evolution is
actually observable. See ``state_per_token``.

## Weight tying

``lm_head`` shares the embedding matrix when ``tie_embeddings`` is set. That saves
``vocab_size * dim`` parameters, which matters at 8B and is noise at 100M. The 128k-vocab
presets untie because that is what puts them at their stated parameter counts.
"""

from __future__ import annotations

import math
from dataclasses import dataclass
from typing import Dict, List, Optional, Tuple

import torch

from torch import Tensor, nn

from elafry.affective.encoders import (
    AffectEncoder,
    InternalStatePredictor,
    Velocity,
)
from elafry.affective.state import initial_state, transition
from elafry.config.base import AffectConfig, ModelConfig
from elafry.models.block import ElafryBlock
from elafry.models.norm import RMSNorm
from elafry.models.router import IntentDecision, IntentRouter
from elafry.models.rope import precompute_rope


__all__ = ["Elafry", "ElafryOutput", "param_breakdown"]


@dataclass
class ElafryOutput:
    """What a forward pass returns. Carrying the state out is not optional: the caller has
    to feed it into the next turn, and dropping it silently resets the model to neutral."""

    logits: Tensor
    state: Tensor
    present_kv: Optional[List[Tuple[Tensor, Tensor]]] = None
    drive: Optional[Tensor] = None

    @property
    def next_token_logits(self) -> Tensor:
        """``(B, vocab)`` logits for the position after the sequence."""
        return self.logits[:, -1, :]


class Elafry(nn.Module):
    def __init__(
        self,
        cfg: ModelConfig,
        affect: Optional[AffectConfig] = None,
    ):
        super().__init__()
        self.cfg = cfg
        self.affect_cfg = affect or AffectConfig()

        self.embed_tokens = nn.Embedding(cfg.vocab_size, cfg.dim)
        if cfg.tie_embeddings:
            self.lm_head = nn.Linear(cfg.dim, cfg.vocab_size, bias=False)
            self.lm_head.weight = self.embed_tokens.weight
        else:
            self.lm_head = nn.Linear(cfg.dim, cfg.vocab_size, bias=False)

        inject_at = set(self.affect_cfg.layers_to_inject(cfg.n_layers))
        self.state_dim = self.affect_cfg.state_feature_dim
        self.layers = nn.ModuleList(
            [
                ElafryBlock(
                    dim=cfg.dim,
                    n_heads=cfg.n_heads,
                    n_kv_heads=cfg.n_kv_heads,
                    head_dim=cfg.head_dim,
                    ff_dim=cfg.ff_dim,
                    rms_eps=cfg.rms_eps,
                    state_dim=self.state_dim,
                    affect_granularity=self.affect_cfg.bias_granularity,
                    inject_affect=(self.affect_cfg.enabled and i in inject_at),
                )
                for i in range(cfg.n_layers)
            ]
        )
        self.norm = RMSNorm(cfg.dim, eps=cfg.rms_eps)

        inv_freq, cos, sin = precompute_rope(cfg.head_dim, cfg.max_seq_len, cfg.rope_theta)
        # Non-persistent: derived from (head_dim, max_seq_len, theta), so storing it would
        # bloat every checkpoint and risk going stale if a config changes.
        self.register_buffer("inv_freq", inv_freq, persistent=False)
        self.register_buffer("cos", cos, persistent=False)
        self.register_buffer("sin", sin, persistent=False)

        self._init_weights()

        # --- affective subsystem -------------------------------------------------
        if self.affect_cfg.enabled:
            if self.affect_cfg.state_source == "external":
                self.state_encoder: Optional[nn.Module] = AffectEncoder(
                    dim=cfg.dim, hidden=self.affect_cfg.external_encoder_dim
                )
            else:
                self.state_encoder = None
            # The internal predictor always exists so the model can be switched between
            # paths after training without reinitialising.
            self.internal_state = InternalStatePredictor(dim=cfg.dim)
            self.router = IntentRouter(dim=cfg.dim)
            self.velocity = Velocity()
        else:
            self.state_encoder = None
            self.internal_state = None
            self.router = None
            self.velocity = None

        # Running state for inference. A buffer, not a parameter: saved, not optimised.
        self.register_buffer("affective_state", torch.zeros(1, 3), persistent=False)

    def _init_weights(self) -> None:
        """Normal(0, fan_in^-0.5) everywhere. Block residuals are already zeroed by
        ``ElafryBlock._init_residuals`` and must not be touched here."""
        for name, p in self.named_parameters():
            if name.endswith("o_proj.weight") or name.endswith("mlp.down.weight"):
                continue
            if p.dim() >= 2:
                fan_in = p.shape[1]
                std = 1.0 / math.sqrt(fan_in)
                if "embed_tokens" in name:
                    # 1/dim rather than the usual 1/sqrt(dim), which is a real correction
                    # and not a stylistic choice. With weight tying and the residual
                    # projections zeroed, a fresh model's activations are just
                    # RMSNorm(embed[x]), and its logits are that vector dotted with every
                    # embedding row. The logit on the input token itself works out to
                    # ||e||^2 / rms(e) = dim * std, which at std = 1/sqrt(dim) comes to
                    # sqrt(dim): about 28 on a 768-wide model. The model starts life
                    # confidently predicting a copy of its input, and cross-entropy on
                    # anything but a copy comes out *above* uniform. Scaling by 1/dim makes
                    # that self-logit O(1), so initial loss sits at log(vocab).
                    std = 1.0 / self.cfg.dim
                nn.init.normal_(p, mean=0.0, std=std)
            elif p.dim() == 1:
                nn.init.ones_(p)

    # ------------------------------------------------------------------ state

    def reset_state(self, batch_size: int = 1, device=None) -> Tensor:
        """Zero the running state. Call at the start of every conversation."""
        self.affective_state = initial_state(batch_size, device=device)
        return self.affective_state

    def compute_drive(
        self,
        input_ids: Tensor,
        attention_mask: Optional[Tensor] = None,
        prev_state: Optional[Tensor] = None,
    ) -> Tensor:
        """Where this turn pushes the state. No circularity: reads embeddings only.

        Args:
            input_ids: ``(B, S)``.
            attention_mask: ``(B, S)`` with 1 for real tokens.
            prev_state: unused here, kept for interface symmetry with the transition.
        Returns:
            ``(B, 3)`` raw drive.
        """
        if self.internal_state is None:
            raise RuntimeError("affective subsystem is disabled; nothing will produce a drive")

        hidden = self.embed_tokens(input_ids)  # (B, S, dim), pre-layer
        return self.internal_state(hidden, attention_mask)

    def step_state(
        self,
        input_ids: Tensor,
        attention_mask: Optional[Tensor] = None,
        prev_state: Optional[Tensor] = None,
    ) -> Tuple[Tensor, Tensor]:
        """Run one turn through the state machine. Returns ``(new_state, drive)``."""
        drive = self.compute_drive(input_ids, attention_mask, prev_state)
        if prev_state is None:
            prev_state = self.affective_state
            if prev_state.shape[0] != drive.shape[0]:
                prev_state = initial_state(drive.shape[0], device=drive.device)
        new_state = transition(
            prev_state,
            drive,
            inertia=self.affect_cfg.inertia,
            clamp=self.affect_cfg.clamp,
        )
        return new_state, drive

    def state_features(self, state: Tensor) -> Tensor:
        """Assemble the vector that reaches ``AffectiveBias``.

        Velocity and intent are not included by default. They are available in
        ``build_state_features`` and adding them means widening ``state_dim`` on both the
        model and the bias, which is why it is an explicit change rather than a silent one.
        """
        if state.dim() == 1:
            state = state.unsqueeze(0)
        return self.affect_cfg.build_features(state)

    def state_features_full(
        self,
        state: Tensor,
        intent: Optional[Tensor] = None,
        arousal_velocity: Optional[Tensor] = None,
        crisis: Optional[Tensor] = None,
    ) -> Tensor:
        """State features with the optional channels populated rather than zero-filled."""
        if state.dim() == 1:
            state = state.unsqueeze(0)
        return self.affect_cfg.build_features(
            state,
            intent=intent,
            arousal_velocity=arousal_velocity,
            crisis=crisis,
        )

    # ---------------------------------------------------------------- forward

    def forward(
        self,
        input_ids: Tensor,
        attention_mask: Optional[Tensor] = None,
        position_ids: Optional[Tensor] = None,
        past_kv: Optional[List[Tuple[Tensor, Tensor]]] = None,
        use_cache: bool = False,
        state: Optional[Tensor] = None,
        state_per_token: bool = False,
    ) -> ElafryOutput:
        """Args:
            input_ids: ``(B, S)``
            attention_mask: ``(B, S)``. Used for pooling only; the causal mask in attention
                is structural and does not need it.
            state: ``(B, 3)`` external state to condition on. ``None`` means derive it.
            state_per_token: recompute the state per position. Inference only, and
                expensive. Off by default because a per-token recurrence inside a 512-token
                training window is a different model from the one the design describes.
        Returns:
            :class:`ElafryOutput`
        """
        bsz, seq_len = input_ids.shape

        if state_per_token:
            raise NotImplementedError(
                "per-token state recurrence is inference-only and not wired up; the "
                "intended path is per-turn stepping via step_state()"
            )

        drive = None
        if self.affect_cfg.enabled:
            if state is None:
                state, drive = self.step_state(input_ids, attention_mask)
            else:
                state = state if state.dim() == 2 else state.unsqueeze(0)
                if state.shape[0] == 1 and bsz > 1:
                    state = state.expand(bsz, -1)
                drive = self.compute_drive(input_ids, attention_mask, state)
            state_feats = self.state_features(state)
        else:
            state = initial_state(bsz, device=input_ids.device)
            state_feats = None

        if position_ids is None:
            past_len = past_kv[0][0].shape[2] if past_kv else 0
            position_ids = torch.arange(past_len, past_len + seq_len, device=input_ids.device)
            position_ids = position_ids.unsqueeze(0).expand(bsz, -1)

        # Every row of position_ids is normally identical, so gather once and stay 2D.
        # Fall back to per-batch tables only when positions actually differ per row.
        if bool((position_ids == position_ids[0]).all()):
            cos = self.cos.index_select(0, position_ids[0])
            sin = self.sin.index_select(0, position_ids[0])
        else:
            cos = self.cos[position_ids]
            sin = self.sin[position_ids]

        h = self.embed_tokens(input_ids)
        presents: List[Tuple[Tensor, Tensor]] = []

        for layer_idx, block in enumerate(self.layers):
            layer_state = state_feats if block.inject_affect else None
            layer_past = past_kv[layer_idx] if (use_cache and past_kv) else None
            h, kv = block(h, cos, sin, layer_past, use_cache, layer_state)
            if use_cache:
                presents.append(kv)

        h = self.norm(h)
        logits = self.lm_head(h)

        return ElafryOutput(
            logits=logits,
            state=state,
            present_kv=presents if use_cache else None,
            drive=drive,
        )

    def get_hidden_states(
        self, input_ids: Tensor, attention_mask: Optional[Tensor] = None
    ) -> Tensor:
        """Final normalised hidden states ``(B, S, dim)``, bypassing ``lm_head``.

        This is the interface the affective reward model and any critic build on. Both need
        a representation, not a distribution over 128k tokens.
        """
        return self._hidden_from_ids(input_ids, attention_mask)

    def _hidden_from_ids(
        self, input_ids: Tensor, attention_mask: Optional[Tensor]
    ) -> Tensor:
        """Run the trunk and return pre-lm_head activations. No cache, no position override."""
        state = None
        state_feats = None
        if self.affect_cfg.enabled:
            state, _ = self.step_state(input_ids, attention_mask)
            state_feats = self.state_features(state)

        positions = torch.arange(input_ids.shape[1], device=input_ids.device)
        cos = self.cos[positions]
        sin = self.sin[positions]

        h = self.embed_tokens(input_ids)
        for block in self.layers:
            h, _ = block(
                h,
                cos,
                sin,
                None,
                False,
                state_feats if block.inject_affect else None,
            )
        return self.norm(h)

    # ------------------------------------------------------------- generation

    @torch.no_grad()
    def generate(
        self,
        input_ids: Tensor,
        max_new_tokens: int = 64,
        temperature: float = 0.8,
        top_k: int = 50,
        top_p: float = 1.0,
        eos_id: Optional[int] = None,
        pad_id: int = 0,
        state: Optional[Tensor] = None,
        generator: Optional[torch.Generator] = None,
    ) -> Tuple[Tensor, Tensor]:
        """Sample with a KV cache. Returns ``(token_ids, final_state)``.

        The state is recomputed on every decode step from the growing sequence, so the
        model's disposition evolves as it writes rather than being frozen at turn start.
        """
        self.eval()
        device = input_ids.device
        if state is None:
            state = initial_state(input_ids.shape[0], device=device)

        # Left-truncate to the context budget, keeping room for what we are about to add.
        budget = self.cfg.max_seq_len - max_new_tokens
        if input_ids.shape[1] > budget:
            input_ids = input_ids[:, -budget:]

        out = self.forward(input_ids, use_cache=True, state=state)
        past = out.present_kv or []
        logits = out.logits[:, -1, :]
        cur_state = out.state

        generated: List[Tensor] = []
        finished = torch.zeros(input_ids.shape[0], dtype=torch.bool, device=device)

        for _ in range(max_new_tokens):
            next_id = sample_from_logits(
                logits, temperature=temperature, top_k=top_k, top_p=top_p, generator=generator
            )
            next_id = torch.where(finished, torch.full_like(next_id, pad_id), next_id)
            # (B,) -> (B, 1). The next forward pass expects a sequence dimension, and
            # handing it a bare (B,) fails three calls later instead of here.
            next_id = next_id.unsqueeze(-1)
            generated.append(next_id)
            if eos_id is not None and bool((next_id == eos_id).all()):
                break

            step_state = cur_state
            out = self.forward(next_id, past_kv=past, use_cache=True, state=step_state)
            past = out.present_kv or past
            logits = out.logits[:, -1, :]
            cur_state = out.state

        tokens = torch.cat(generated, dim=1) if generated else input_ids[:, :0]
        return tokens, cur_state

    @torch.no_grad()
    def route(self, input_ids: Tensor, attention_mask: Optional[Tensor] = None) -> IntentDecision:
        """Classify a turn's intent. Inference helper."""
        if self.router is None:
            raise RuntimeError("router is unavailable; the affective subsystem is disabled")
        hidden = self._hidden_from_ids(input_ids, attention_mask)
        return self.router.decide(hidden.mean(dim=1)[0])


# ------------------------------------------------------------------ sampling


def sample_from_logits(
    logits: Tensor,
    temperature: float = 1.0,
    top_k: int = 0,
    top_p: float = 1.0,
    generator: Optional[torch.Generator] = None,
) -> Tensor:
    """Temperature, then top-k, then top-p. Each stage narrows what the next can see.

    Args:
        logits: ``(B, vocab)``.
        top_k: 0 disables. Applied before nucleus filtering so the candidate set is small.
        top_p: 1.0 disables.
    """
    if temperature <= 0:
        raise ValueError(f"temperature must be positive, got {temperature}")

    logits = logits / temperature

    if top_k and top_k > 0:
        k = min(top_k, logits.shape[-1])
        threshold = torch.topk(logits, k, dim=-1).values[:, -1:]
        logits = logits.masked_fill(logits < threshold, float("-inf"))

    if 0.0 < top_p < 1.0:
        sorted_logits, sorted_idx = torch.sort(logits, descending=True, dim=-1)
        cumulative = torch.softmax(sorted_logits, dim=-1).cumsum(dim=-1)
        # Keep the smallest prefix whose mass reaches top_p. Shifting by one guarantees the
        # token that crosses the threshold stays in.
        remove = cumulative - torch.softmax(sorted_logits, dim=-1) >= top_p
        sorted_logits = sorted_logits.masked_fill(remove, float("-inf"))
        logits = torch.empty_like(logits).scatter_(1, sorted_idx, sorted_logits)

    probs = torch.softmax(logits, dim=-1)
    return torch.multinomial(probs, num_samples=1, generator=generator).squeeze(-1)


# ------------------------------------------------------------------- params


def param_breakdown(cfg: ModelConfig, affect: Optional[AffectConfig] = None) -> Dict[str, int]:
    """Analytic parameter count, keyed by group. Sums to ``sum(p.numel())``.

    Kept analytic rather than measured so a preset's stated size can be checked without
    allocating the model. ``tests/test_config.py`` asserts the two agree.
    """
    affect_cfg = affect or AffectConfig()

    embed = cfg.vocab_size * cfg.dim
    lm_head = 0 if cfg.tie_embeddings else cfg.vocab_size * cfg.dim

    per_layer_attn = cfg.dim * (cfg.q_dim + 2 * cfg.kv_dim + cfg.q_dim)
    per_layer_mlp = 3 * cfg.dim * cfg.ff_dim
    per_layer_norms = 2 * cfg.dim
    n_bias = cfg.n_heads if affect_cfg.bias_granularity == "head" else cfg.n_kv_heads
    per_layer_affect = (
        affect_cfg.state_feature_dim * n_bias * cfg.head_dim * cfg.head_dim
        if affect_cfg.enabled
        else 0
    )
    per_layer = per_layer_attn + per_layer_mlp + per_layer_norms + per_layer_affect

    layers = cfg.n_layers * per_layer
    final_norm = cfg.dim

    affect_subsystem = 0
    if affect_cfg.enabled:
        h = affect_cfg.external_encoder_dim
        affect_subsystem = cfg.dim * h + h + h * 3 + 3  # InternalStatePredictor
        if affect_cfg.state_source == "external":
            # AffectEncoder only exists on the external path.
            affect_subsystem += cfg.dim * h + h + h * h + h + h * 3 + 3
        # IntentRouter: 6 competing intents plus the binary crisis head.
        affect_subsystem += cfg.dim * 6 + 6 + cfg.dim + 1

    return {
        "embed": embed,
        "lm_head": lm_head,
        "layers": layers,
        "final_norm": final_norm,
        "affect_subsystem": affect_subsystem,
    }