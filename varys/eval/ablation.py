"""Pinned-state ablation, per block.

This is the module :class:`~varys.models.attention.MultiAxisBias` was designed for. A single
wide state projection cannot be partially disabled: the terms are entangled from step zero,
so "does the affective mechanism do anything" and "does clinical conditioning help or hurt"
are the same question and only the first is answerable. Separate per-block projections make
each block independently addressable, and this is where that gets spent.

## The method

Elafry's approach, kept: pin the state to a set of values, generate from each, and measure
how far the resulting output distributions separate. If pinning the state does not move the
output, the bias is not connected to anything. If it moves the output but every state lands
in the same place, the model is reading the state and ignoring what it says.

Varys adds a control that Elafry could not have, and it is the important part here. Because
the blocks are separate, the ablation can zero one block while holding the others fixed. That
distinguishes two things that otherwise look identical in a single number:

* the state is ignored entirely (distances ~ 0)
* the state is read but the *wrong* block is driving it (distances ~ 0 when you ablate the
  block that was actually doing the work)

The second is invisible without the control. A model whose attention is entirely driven by
affect, with clinical conditioning present but inert, looks exactly like a model with no
affective bias at all if you only ever ablate everything.

## On interpreting the numbers

A large separation is not evidence the mechanism is *good*. It is evidence it is *connected*.
Whether a large separation is desirable depends entirely on which block was moved: a large
response to a clinical state is exactly the thing the design tries to prevent, and this
module reports it rather than celebrating it. :func:`format_ablation` prints the block each
row ablated so a large clinical number cannot be skimmed past.
"""

from __future__ import annotations

from dataclasses import dataclass
from typing import Dict, List, Optional, Sequence

import torch
from torch import Tensor

from varys.affective.state import MORAL_SLICE, SPECTRA_SLICE, VAD_SLICE
from varys.models.varys import Varys

__all__ = [
    "AblationCondition",
    "BlockAblationResult",
    "run_block_ablation",
    "format_ablation",
]


@dataclass(frozen=True)
class AblationCondition:
    """One pinned state to compare against.

    ``name`` is for the report. ``state`` is the full 21-dim vector with the other blocks
    zeroed, so each condition moves exactly one block and the others are held constant.
    """

    name: str
    block: str
    state: List[float]


@dataclass
class BlockAblationResult:
    """What one block's conditioning is doing to the output distribution.

    Attributes:
        block: which state block was ablated.
        conditions: the pinned states compared.
        jsd: Jensen-Shannon divergence between each pair of conditions, in nats. 0 means
            indistinguishable, ln(2) is the maximum.
        centroid_spread: mean distance from the joint centroid. Near 0 with a large pairwise
            JSD would mean the conditions separate but sit in the same place, which is
            incoherent and worth reporting rather than averaging away.
        moved: whether pinning this block changed the output at all.
    """

    block: str
    conditions: List[str]
    jsd: Dict[str, Dict[str, float]]
    centroid_spread: float = 0.0
    baseline_jsd: float = 0.0

    @property
    def moved(self) -> bool:
        return self.centroid_spread > 1e-9

    @property
    def min_jsd(self) -> float:
        return min((v for row in self.jsd.values() for v in row.values()), default=0.0)

    @property
    def max_jsd(self) -> float:
        return max((v for row in self.jsd.values() for v in row.values()), default=0.0)

    def to_dict(self) -> Dict[str, object]:
        return {
            "block": self.block,
            "conditions": self.conditions,
            "jsd": {a: {b: round(v, 6) for b, v in row.items()} for a, row in self.jsd.items()},
            "centroid_spread": round(self.centroid_spread, 6),
            "min_jsd": round(self.min_jsd, 6),
            "max_jsd": round(self.max_jsd, 6),
            "moved": self.moved,
        }


def _jsd(p: Tensor, q: Tensor) -> float:
    """Jensen-Shannon divergence in nats, from two probability vectors."""
    p = p / p.sum().clamp(min=1e-12)
    q = q / q.sum().clamp(min=1e-12)
    m = 0.5 * (p + q)
    kl = lambda a, b: (a * (a.clamp(min=1e-12).log() - b.clamp(min=1e-12).log())).sum()  # noqa: E731
    return float(0.5 * kl(p, m) + 0.5 * kl(q, m))


def _next_token_distribution(
    model: Varys,
    prompt_ids: Tensor,
    state: Tensor,
    temperature: float = 1.0,
) -> Tensor:
    """The next-token distribution under a pinned state.

    One forward pass, no sampling. Sampling introduces a seed into the measurement, and an
    ablation whose number depends on the seed is not a measurement.
    """
    was_training = model.training
    model.eval()
    with torch.no_grad():
        out = model(prompt_ids, state=state)
        probs = torch.softmax(out.next_token_logits / temperature, dim=-1)
    if was_training:
        model.train()
    return probs[0]


def default_conditions() -> List[AblationCondition]:
    """Pinned states for each block: neutral, and two meaningfully different values.

    Neutral is included as a control rather than omitted. If the "extreme" conditions
    separate from each other but neither separates from neutral, the model is reading
    direction but not magnitude, which is a different and more specific failure than
    "the bias is inert".
    """
    n = 21
    conds: List[AblationCondition] = []

    conds.append(AblationCondition("vad:neutral", "vad", [0.0] * n))
    low = [0.0] * n
    low[VAD_SLICE] = [-0.8, 0.6, -0.7]
    conds.append(AblationCondition("vad:distressed", "vad", low))
    high = [0.0] * n
    high[VAD_SLICE] = [0.8, 0.6, 0.7]
    conds.append(AblationCondition("vad:elated", "vad", high))

    conds.append(AblationCondition("spectra:neutral", "spectra", [0.0] * n))
    marked = [0.0] * n
    marked[SPECTRA_SLICE] = [0.85] * 6
    conds.append(AblationCondition("spectra:marked", "spectra", marked))

    conds.append(AblationCondition("moral:neutral", "moral", [0.0] * n))
    moral = [0.0] * n
    moral[MORAL_SLICE] = [0.9] * 12
    conds.append(AblationCondition("moral:strong", "moral", moral))

    return conds


def run_block_ablation(
    model: Varys,
    prompt_ids: Tensor,
    conditions: Optional[Sequence[AblationCondition]] = None,
    temperature: float = 1.0,
) -> Dict[str, BlockAblationResult]:
    """Measure each state block's influence on the next-token distribution.

    Args:
        model: a :class:`~varys.models.varys.Varys`.
        prompt_ids: ``(1, S)``. One prompt: the measurement is about how the state moves the
            output, and averaging over prompts mixes in prompt-to-prompt variance that has
            nothing to do with the bias.
        conditions: pinned states. Defaults to :func:`default_conditions`.
        temperature: applied to the logits before the distribution is compared. 1.0 compares
            the model's actual distribution.

    Returns:
        One :class:`BlockAblationResult` per block that appears in the conditions.
    """
    if prompt_ids.dim() != 2 or prompt_ids.shape[0] != 1:
        raise ValueError(f"prompt_ids must be (1, S), got {tuple(prompt_ids.shape)}")
    conds = list(conditions or default_conditions())

    device = next(model.parameters()).device
    dists: Dict[str, Tensor] = {}
    for c in conds:
        state = torch.tensor([c.state], dtype=torch.float32, device=device)
        dists[c.name] = _next_token_distribution(model, prompt_ids, state, temperature)

    results: Dict[str, BlockAblationResult] = {}
    for block in ("vad", "spectra", "moral"):
        names = [c.name for c in conds if c.block == block]
        if len(names) < 2:
            continue
        # Include the neutral control, which belongs to the same block by name prefix.
        for c in conds:
            if c.name.startswith(f"{block}:neutral") and c.name not in names:
                names.append(c.name)
                break

        pairwise: Dict[str, Dict[str, float]] = {a: {} for a in names}
        for i, a in enumerate(names):
            for b in names[i + 1 :]:
                d = _jsd(dists[a], dists[b])
                pairwise[a][b] = d
                pairwise[b][a] = d

        centroid = torch.stack([dists[n] for n in names]).mean(dim=0)
        spread = float(
            torch.stack([(dists[n] - centroid).norm() for n in names]).mean()
        )

        results[block] = BlockAblationResult(
            block=block,
            conditions=names,
            jsd=pairwise,
            centroid_spread=spread,
        )

    return results


def format_ablation(results: Dict[str, BlockAblationResult]) -> str:
    """A report that names the block on every row.

    The naming is the safety property. A table of divergences without the ablated block
    attached invites reading a large clinical number as a good result, and a large clinical
    response is the thing the design is trying to prevent.
    """
    lines = ["pinned-state ablation", "=" * 60]
    for block, r in results.items():
        lines.append(f"block: {block}")
        lines.append(f"  conditions       : {', '.join(r.conditions)}")
        lines.append(f"  centroid spread  : {r.centroid_spread:.6e}  "
                     f"({'connected' if r.moved else 'NO EFFECT'})")
        for a, row in r.jsd.items():
            for b, v in row.items():
                if a < b:
                    lines.append(f"    JSD {a} vs {b}: {v:.6f}")
        if not r.moved:
            lines.append(
                f"  note: pinning {block} did not move the output. Either the projection is "
                "still at its zero initialisation or this block is inert."
            )
        if block == "spectra" and r.max_jsd > 0.05:
            lines.append(
                "  WARNING: clinical state is moving the next-token distribution more than "
                "5% JSD. The design intends clinical state to change response style, not "
                "which tokens the model attends to. Check the learned scale."
            )
        if block == "moral" and r.moved:
            lines.append(
                "  WARNING: the moral block is disabled by default. It moving means it was "
                "enabled, and that is a decision to review against is_manipulation_risk."
            )
        lines.append("")
    return "\n".join(lines)
