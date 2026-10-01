"""The Varys decoder.

A Llama-style decoder-only transformer with one substantive difference from Elafry: the
state it carries is 21 dimensions in three blocks with different ranges, different decay
rates and different safety properties, and the attention bias reads those blocks separately.

## The causal-order problem, and how it is resolved

The state both *depends on* the model (it is predicted from the model's own representation
of the input) and *influences* the model (it biases attention). Reading the state's input
from the model's final hidden states would be circular: ``S -> h -> S``.

The fix is to read the state from the token embeddings, before any layer runs. Those depend
only on the input ids, so the graph is acyclic. "Internal" here means *the model derives its
own state from its own input representation*, not *an external encoder hands it a vector*.

## Why the state updates per turn, not per token

Inside a training window the state is computed once and held. A per-token recurrence over
4096 steps is a different model and a much harder optimisation problem than the one the
architecture describes; the gradient has to traverse 4096 state updates, and the clinical
block's confidence gate needs several consecutive observations before it will move at all.
During autoregressive decode the state is recomputed per token, which is where the evolution
is actually observable. See ``state_per_token``.

## What is *not* here

No dataset, no tokenizer, no training loop. Those are separate modules and Varys ships
without any of them populated. See ``README.md``.
"""

from __future__ import annotations

import math
from dataclasses import dataclass
from typing import Dict, List, Optional, Tuple

import torch
from torch import Tensor, nn

from varys.affective.encoders import AffectEncoder, InternalStatePredictor, MultiAxisVelocity
from varys.affective.state import MORAL_SLICE, SPECTRA_SLICE, STATE_DIMS, VAD_SLICE
from varys.config.base import AffectConfig, ModelConfig
from varys.models.block import VarysBlock
from varys.models.norm import RMSNorm
from varys.models.rope import rope_from_config
from varys.models.router import IntentDecision, IntentRouter
from varys.models.state_machine import GatedStateMachine, zero_state

__all__ = ["Varys", "VarysOutput", "sample_from_logits", "param_breakdown"]


@dataclass
class VarysOutput:
    """What a forward pass returns.

    Carrying the state out is not optional: the caller has to feed it into the next turn,
    and dropping it silently resets the model to neutral, which is also what makes a
    long conversation start sounding like a first one.
    """

    logits: Tensor
    state: Tensor
    present_kv: Optional[List[Tuple[Tensor, Tensor]]] = None
    drive: Optional[Tensor] = None

    @property
    def next_token_logits(self) -> Tensor:
        """``(B, vocab)`` logits for the position after the sequence."""
        return self.logits[:, -1, :]


class Varys(nn.Module):
    def __init__(self, cfg: ModelConfig, affect: Optional[AffectConfig] = None):
        super().__init__()
        self.cfg = cfg
        self.affect_cfg = affect or AffectConfig()

        self.embed_tokens = nn.Embedding(cfg.vocab_size, cfg.dim)
        self.lm_head = nn.Linear(cfg.dim, cfg.vocab_size, bias=False)
        if cfg.tie_embeddings:
            self.lm_head.weight = self.embed_tokens.weight

        # Which state blocks reach attention. The moral block is absent by default; see
        # AffectConfig for why that is a decision and not a capacity constraint.
        self.bias_blocks: Dict[str, int] = {"vad": VAD_SLICE.stop - VAD_SLICE.start}
        self._block_scales: Dict[str, float] = {"vad": self.affect_cfg.vad_bias_scale}
        if self.affect_cfg.use_spectra:
            self.bias_blocks["spectra"] = SPECTRA_SLICE.stop - SPECTRA_SLICE.start
            self._block_scales["spectra"] = self.affect_cfg.spectra_bias_scale
        if self.affect_cfg.use_moral:
            self.bias_blocks["moral"] = MORAL_SLICE.stop - MORAL_SLICE.start
            self._block_scales["moral"] = self.affect_cfg.moral_bias_scale

        inject_at = set(self.affect_cfg.layers_to_inject(cfg.n_layers))
        self.bias_width = sum(self.bias_blocks.values())
        self.layers = nn.ModuleList(
            [
                VarysBlock(
                    dim=cfg.dim,
                    n_heads=cfg.n_heads,
                    n_kv_heads=cfg.n_kv_heads,
                    head_dim=cfg.head_dim,
                    ff_dim=cfg.ff_dim,
                    rms_eps=cfg.rms_eps,
                    block_widths=self.bias_blocks,
                    block_scales=self._block_scales,
                    affect_granularity=self.affect_cfg.bias_granularity,
                    inject_affect=(self.affect_cfg.enabled and i in inject_at),
                )
                for i in range(cfg.n_layers)
            ]
        )
        self.norm = RMSNorm(cfg.dim, eps=cfg.rms_eps)

        # Built through the config, never from bare arguments: the scaling fields are what
        # stop a checkpoint from being loaded with geometry it was not trained under.
        inv_freq, cos, sin = rope_from_config(cfg)
        # Non-persistent: derived from (head_dim, max_seq_len, theta, scaling), so storing it
        # would bloat every checkpoint and risk going stale if a config changes.
        self.register_buffer("inv_freq", inv_freq, persistent=False)
        self.register_buffer("cos", cos, persistent=False)
        self.register_buffer("sin", sin, persistent=False)

        self._init_weights()

        if self.affect_cfg.enabled:
            self.state_encoder: Optional[nn.Module] = (
                AffectEncoder(dim=cfg.dim, hidden=self.affect_cfg.external_encoder_dim)
                if self.affect_cfg.state_source == "external"
                else None
            )
            # The internal predictor always exists so the model can be switched between paths
            # after training without reinitialising.
            self.internal_state = InternalStatePredictor(dim=cfg.dim)
            self.router = IntentRouter(dim=cfg.dim)
            self.velocity = MultiAxisVelocity()
            self.machine = GatedStateMachine(
                gate_clinical=self.affect_cfg.gate_clinical,
                gate_min_confidence=self.affect_cfg.gate_min_confidence,
                gate_decay=self.affect_cfg.gate_decay,
            )
        else:
            self.state_encoder = None
            self.internal_state = None
            self.router = None
            self.velocity = None
            self.machine = None

    def _init_weights(self) -> None:
        """Normal(0, fan_in^-0.5) everywhere. Block residuals are already zeroed by
        ``VarysBlock._init_residuals`` and must not be touched here.

        The embedding exception is a real correction rather than a stylistic choice. With
        weight tying and the residual projections zeroed, a fresh model's activations are
        just ``RMSNorm(embed[x])``, and its logits are that vector dotted with every
        embedding row. The logit on the input token itself works out to
        ``||e||^2 / rms(e) = dim * std``, which at ``std = 1/sqrt(dim)`` comes to
        ``sqrt(dim)``: about 90 on an 8192-wide model. The model would start life
        confidently predicting a copy of its input, and cross-entropy on anything but a copy
        would come out above uniform. Scaling by ``1/dim`` makes the self-logit O(1).
        """
        for name, p in self.named_parameters():
            if name.endswith("o_proj.weight") or name.endswith("mlp.down.weight"):
                continue
            if p.dim() >= 2:
                fan_in = p.shape[1]
                std = 1.0 / math.sqrt(fan_in)
                if "embed_tokens" in name:
                    std = 1.0 / self.cfg.dim
                nn.init.normal_(p, mean=0.0, std=std)
            elif p.dim() == 1:
                nn.init.ones_(p)

    # ------------------------------------------------------------------ state

    def reset_state(self, batch_size: int = 1) -> Tensor:
        """Zero the running state. Call at the start of every conversation."""
        if self.machine is not None:
            return self.machine.reset(batch_size)
        return zero_state(batch_size)

    def compute_drive(
        self,
        input_ids: Tensor,
        attention_mask: Optional[Tensor] = None,
    ) -> Tensor:
        """Where this turn pushes the state. Reads embeddings only, so no circularity.

        Args:
            input_ids: ``(B, S)``.
            attention_mask: ``(B, S)`` with 1 for real tokens.
        Returns:
            ``(B, 21)`` raw drive.
        """
        if self.internal_state is None:
            raise RuntimeError("affective subsystem is disabled; nothing will produce a drive")
        hidden = self.embed_tokens(input_ids)  # (B, S, dim), pre-layer
        return self.internal_state(hidden, attention_mask)

    def state_features(self, state: Tensor) -> Tensor:
        """Assemble the vector that reaches ``MultiAxisBias``.

        Width is exactly ``sum(self.bias_blocks.values())``, and the block order matches the
        order the bias was constructed with. Both come from the same dict, so they cannot
        drift.
        """
        if state.dim() == 1:
            state = state.unsqueeze(0)
        return self.affect_cfg.build_features(state)

    def block_influence(self) -> Dict[str, float]:
        """Each state block's learned bias scale, read from the first injecting layer.

        Every layer has its own, and they will differ after training. Reporting the first is
        the cheap summary; the ablation is the honest one.
        """
        for block in self.layers:
            if block.inject_affect and block.self_attn.affect is not None:
                return block.self_attn.affect.block_influence()
        return {}

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
    ) -> VarysOutput:
        """Args:
            input_ids: ``(B, S)``
            attention_mask: ``(B, S)``. Used for pooling only; the causal mask in attention is
                structural and does not need it.
            state: ``(B, 21)`` external state to condition on. ``None`` means derive it.
            state_per_token: inference only, and not implemented. See the module docstring.
        Returns:
            :class:`VarysOutput`
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
                drive = self.compute_drive(input_ids, attention_mask)
            state_feats = self.state_features(state)
        else:
            state = zero_state(bsz, device=input_ids.device)
            state_feats = None

        if position_ids is None:
            past_len = past_kv[0][0].shape[2] if past_kv else 0
            if past_len + seq_len > self.cfg.max_seq_len:
                raise ValueError(
                    f"sequence of {past_len + seq_len} exceeds max_seq_len "
                    f"{self.cfg.max_seq_len}. The rope table was built for "
                    f"{self.cfg.max_seq_len} positions and indexing past it silently "
                    "wraps, which corrupts position rather than erroring."
                )
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

        return VarysOutput(
            logits=logits,
            state=state,
            present_kv=presents if use_cache else None,
            drive=drive,
        )

    def step_state(
        self,
        input_ids: Tensor,
        attention_mask: Optional[Tensor] = None,
        prev_state: Optional[Tensor] = None,
    ) -> Tuple[Tensor, Tensor]:
        """Run one turn through the state machine. Returns ``(new_state, drive)``.

        The gate is *not* applied here. It needs a per-spectrum observation strength from the
        caller, and this function has no way to know one. :meth:`GatedStateMachine.step` is
        the path that gates, and a caller with no clinical detector should pass
        ``clinical_hits=None`` and let the block relax rather than pretend the gate opened.
        """
        drive = self.compute_drive(input_ids, attention_mask)
        if prev_state is None:
            prev_state = self.machine.state if self.machine is not None else None
        if prev_state is None or prev_state.shape[0] != drive.shape[0]:
            prev_state = zero_state(drive.shape[0], device=drive.device)
        new_state = self.machine.step(
            drive, clinical_hits=None, cfg=self.affect_cfg, state=prev_state
        )
        return new_state, drive

    def get_hidden_states(
        self, input_ids: Tensor, attention_mask: Optional[Tensor] = None
    ) -> Tensor:
        """Final normalised hidden states ``(B, S, dim)``, bypassing ``lm_head``.

        This is the interface the affective reward model and any critic build on. Both need a
        representation, not a distribution over 131k tokens.
        """
        return self._hidden_from_ids(input_ids, attention_mask)

    def _hidden_from_ids(
        self, input_ids: Tensor, attention_mask: Optional[Tensor]
    ) -> Tensor:
        state = None
        state_feats = None
        if self.affect_cfg.enabled:
            state, _ = self.step_state(input_ids, attention_mask)
            state_feats = self.state_features(state)

        positions = torch.arange(input_ids.shape[1], device=input_ids.device)
        if input_ids.shape[1] > self.cfg.max_seq_len:
            raise ValueError(
                f"sequence of {input_ids.shape[1]} exceeds max_seq_len {self.cfg.max_seq_len}"
            )
        cos = self.cos[positions]
        sin = self.sin[positions]

        h = self.embed_tokens(input_ids)
        for block in self.layers:
            h, _ = block(
                h, cos, sin, None, False,
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
            state = zero_state(input_ids.shape[0], device=device)

        # Left-truncate to the context budget, keeping room for what we are about to add.
        budget = self.cfg.max_seq_len - max_new_tokens
        if budget <= 0:
            raise ValueError(
                f"max_new_tokens {max_new_tokens} leaves no room inside "
                f"max_seq_len {self.cfg.max_seq_len}"
            )
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
            # (B,) -> (B, 1). The next forward pass expects a sequence dimension, and handing
            # it a bare (B,) fails three calls later instead of here.
            next_id = next_id.unsqueeze(-1)
            generated.append(next_id)
            if eos_id is not None and bool((next_id == eos_id).all()):
                break

            out = self.forward(next_id, past_kv=past, use_cache=True, state=cur_state)
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


# --------------------------------------------------------------------- params


def param_breakdown(cfg: ModelConfig, affect: Optional[AffectConfig] = None) -> Dict[str, int]:
    """Analytic parameter count, keyed by group. Sums to ``sum(p.numel())``.

    Kept analytic rather than measured so a preset's stated size can be checked without
    allocating the model, which is the only way to state a 72B figure honestly on a 12GB
    card. ``tests/test_models.py`` asserts the two agree on the presets that fit.

    The affective bias is broken out per block because that is the line item a reader needs
    to see. At the 72B geometry each dimension of state costs 1.05M parameters per layer, so
    the VAD block alone is 251M and the moral block would be 1.0B.
    """
    affect_cfg = affect or AffectConfig()

    embed = cfg.vocab_size * cfg.dim
    lm_head = 0 if cfg.tie_embeddings else cfg.vocab_size * cfg.dim

    per_layer_attn = cfg.dim * (cfg.q_dim + 2 * cfg.kv_dim + cfg.q_dim)
    per_layer_mlp = 3 * cfg.dim * cfg.ff_dim
    per_layer_norms = 2 * cfg.dim

    n_bias = cfg.n_heads if affect_cfg.bias_granularity == "head" else cfg.n_kv_heads
    bias_per_dim = n_bias * cfg.head_dim * cfg.head_dim

    bias_blocks: Dict[str, int] = {}
    if affect_cfg.enabled:
        bias_blocks["vad"] = affect_cfg.block_widths()["vad"] * bias_per_dim
        if affect_cfg.use_spectra:
            bias_blocks["spectra"] = affect_cfg.block_widths()["spectra"] * bias_per_dim
        if affect_cfg.use_moral:
            bias_blocks["moral"] = affect_cfg.block_widths()["moral"] * bias_per_dim
    per_layer_affect = sum(bias_blocks.values())

    # The bias is held *out* of `layers` rather than folded into it, so that the returned
    # groups partition the parameter set and sum to the measured total. Folding it in and
    # then also reporting it separately double-counts the whole affective bias, which at the
    # 72B geometry is 755M parameters of overstatement on a model that cannot be allocated
    # to check the arithmetic.
    per_layer_base = per_layer_attn + per_layer_mlp + per_layer_norms
    layers = cfg.n_layers * per_layer_base
    final_norm = cfg.dim

    # Learnable per-block scales, one scalar each, in every injecting layer.
    n_inject = len(affect_cfg.layers_to_inject(cfg.n_layers)) if affect_cfg.enabled else 0
    scales = n_inject * len(bias_blocks)

    affect_subsystem = 0
    if affect_cfg.enabled:
        h = affect_cfg.external_encoder_dim
        # InternalStatePredictor: dim -> h -> 21
        affect_subsystem += cfg.dim * h + h + h * len(STATE_DIMS) + len(STATE_DIMS)
        if affect_cfg.state_source == "external":
            # AffectEncoder only exists on the external path.
            affect_subsystem += cfg.dim * h + h + h * h + h + h * len(STATE_DIMS) + len(STATE_DIMS)
        # IntentRouter: 6 competing intents plus the binary crisis head.
        affect_subsystem += cfg.dim * 6 + 6 + cfg.dim + 1

    return {
        "embed": embed,
        "lm_head": lm_head,
        "layers": layers,
        "final_norm": final_norm,
        "affect_bias": per_layer_affect * cfg.n_layers,
        "affect_scales": scales,
        "affect_subsystem": affect_subsystem,
        "affect_bias_by_block": {
            name: per_dim * cfg.n_layers for name, per_dim in bias_blocks.items()
        },
    }
