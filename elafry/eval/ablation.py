"""The pinned-state ablation.

The one experiment in this project that produces a real result before there is a model worth
running.

## The question

Does pinning the internal state to a named emotion actually change what the model produces?
Not "does the tensor change", which is arithmetic anyone can verify, but does *anger* produce
measurably different predictions than *contentment* from an identical prompt?

If the answer is no, the affective bias is decorative and the whole architecture note is
describing something that does not happen. That is a question worth being able to answer, and
it is answerable on a randomly initialised model because it is a question about wiring rather
than about learning.

## Why it is worth having at all

Every other measurement in this project needs a trained model, which needs a corpus, which does
not exist yet. This does not. It runs in under a second on CPU and it catches the specific
failure where the bias is computed correctly and then discarded somewhere downstream.

## What counts as a pass

Three levels, deliberately ordered by difficulty:

``movement``
    Pinning different states changes the next-token distribution. This catches the bias not
    reaching attention at all.
``separation``
    All eight Plutchik states produce distinguishable distributions. This catches a bias that
    is present but collapsed, where the state only affects output magnitude.
``ordering``
    States adjacent on the wheel are closer in output space than opposite ones. This is the
    strongest claim, and the one that says the bias is tracking VAD geometry rather than
    just adding noise. It can fail on a random model, which is informative.
"""

from __future__ import annotations

import math
from dataclasses import dataclass, field
from typing import Dict, List, Optional, Tuple

import torch
from torch import Tensor

from elafry.affective.state import all_primary_states
from elafry.models.elafry import Elafry

__all__ = [
    "AblationResult",
    "pin_and_generate",
    "distribution_distance",
    "run_ablation",
    "format_ablation",
]


@dataclass
class AblationResult:
    """Distances between output distributions under pinned states."""

    prompt_ids: List[int]
    distances: Dict[str, Dict[str, float]] = field(default_factory=dict)
    centroid_spread: float = 0.0
    state_vector_distances: Dict[str, Dict[str, float]] = field(default_factory=dict)
    ordering_holds: bool = False
    ordering_violations: List[str] = field(default_factory=list)

    @property
    def moved(self) -> bool:
        """Whether pinning different states changed anything at all."""
        return self.centroid_spread > 0.0

    @property
    def separated(self) -> bool:
        """Whether every pair of states is distinguishable in output space."""
        worst = min(
            (v for row in self.distances.values() for v in row.values()), default=0.0
        )
        return worst > 0.0 and self.centroid_spread > 0.0

    def to_dict(self) -> Dict[str, object]:
        return {
            "centroid_spread": round(self.centroid_spread, 6),
            "ordering_holds": self.ordering_holds,
            "ordering_violations": self.ordering_violations,
            "min_pairwise_distance": round(
                min((v for row in self.distances.values() for v in row.values()), default=0.0), 6
            ),
            "distances": {
                a: {b: round(d, 6) for b, d in row.items()} for a, row in self.distances.items()
            },
        }


@torch.no_grad()
def pin_and_generate(
    model: Elafry,
    prompt_ids: Tensor,
    state: Tensor,
    max_new_tokens: int = 16,
    temperature: float = 0.0,
    generator: Optional[torch.Generator] = None,
) -> Tuple[Tensor, Tensor]:
    """Generate with the state pinned. Returns ``(tokens, final_state)``.

    ``temperature=0`` is the default here, which makes generation greedy and therefore
    reproducible. Sampling would put a noise term into every measurement and the effect being
    measured is smaller than the noise.
    """
    was_training = model.training
    model.eval()
    try:
        return model.generate(
            prompt_ids,
            max_new_tokens=max_new_tokens,
            temperature=temperature if temperature > 0 else 1e-6,
            top_k=1,
            top_p=1.0,
            eos_id=None,
            state=state,
            generator=generator,
        )
    finally:
        model.train(was_training)


@torch.no_grad()
def distribution_distance(model: Elafry, prompt_ids: Tensor, state: Tensor) -> Tensor:
    """Next-token probability distribution under a pinned state.

    Softmax rather than raw logits: raw logits can all shift together without changing which
    token is preferred, and shifting together is not the effect being claimed.
    """
    out = model(prompt_ids, state=state)
    return torch.softmax(out.next_token_logits.float(), dim=-1)[0]


def _jsd(p: Tensor, q: Tensor) -> float:
    """Jensen-Shannon divergence in bits, in [0, 1].

    Symmetric and bounded, which a plain KL is not. A KL would let one nearly-degenerate
    distribution produce an enormous distance and dominate the average.
    """

    def kl(a: Tensor, b: Tensor) -> Tensor:
        eps = 1e-12
        return (a * ((a + eps).log() - (b + eps).log())).sum()

    m = 0.5 * (p + q)
    return float(0.5 * kl(p, m) + 0.5 * kl(q, m)) / math.log(2)


@torch.no_grad()
def run_ablation(
    model: Elafry,
    prompt_ids: Tensor,
    states: Optional[Dict[str, Tensor]] = None,
    prune_below: float = 1e-9,
) -> AblationResult:
    """Measure how far apart pinned states push the output distribution.

    Args:
        prompt_ids: ``(1, S)``.
        states: name to ``(1, 3)``. Defaults to the eight Plutchik primaries.
        prune_below: values below this are floored before the log in the divergence. Guards
            against a ``log(0)`` from an untrained model's near-zero probabilities.
    """
    if prompt_ids.dim() == 1:
        prompt_ids = prompt_ids.unsqueeze(0)

    states = states or all_primary_states()
    names = list(states)

    dists = {name: distribution_distance(model, prompt_ids, states[name]) for name in names}

    result = AblationResult(prompt_ids=prompt_ids[0].tolist())

    for i, a in enumerate(names):
        for b in names[i + 1 :]:
            d = _jsd(dists[a], dists[b])
            result.distances.setdefault(a, {})[b] = d
            result.distances.setdefault(b, {})[a] = d

            # The same measure on the state vectors themselves, so ordering can be compared
            # against the geometry of VAD rather than against an assumption about it. Read the
            # vector from the caller's dict rather than PLUTCHIK_VAD, so a custom state set
            # with names that are not primaries still works.
            av = tuple(states[a][0].tolist())
            bv = tuple(states[b][0].tolist())
            na = math.sqrt(sum(x * x for x in av)) or 1.0
            nb = math.sqrt(sum(x * x for x in bv)) or 1.0
            state_d = 1.0 - sum(av[i] * bv[i] for i in range(3)) / (na * nb)
            result.state_vector_distances.setdefault(a, {})[b] = state_d
            result.state_vector_distances.setdefault(b, {})[a] = state_d

    if result.distances:
        result.centroid_spread = max(
            max(row.values()) for row in result.distances.values() if row
        )

    # Ordering: states close on the wheel should be close in output space. Joy/trust are
    # neighbours; joy/sadness sit on opposite sides of the valence axis.
    result.ordering_holds, result.ordering_violations = _check_ordering(result)

    return result


def _check_ordering(result: AblationResult) -> Tuple[bool, List[str]]:
    violations: List[str] = []
    checked = 0

    for near_a, near_b in (("joy", "trust"), ("anger", "disgust"), ("fear", "surprise")):
        for far_a, far_b in (("joy", "sadness"), ("anger", "trust"), ("fear", "sadness")):
            try:
                near = result.distances[near_a][near_b]
                far = result.distances[far_a][far_b]
            except KeyError:
                continue
            checked += 1
            if near >= far:
                violations.append(
                    f"{near_a}/{near_b} ({near:.4f}) should be closer than "
                    f"{far_a}/{far_b} ({far:.4f})"
                )

    return checked > 0 and not violations, violations


def format_ablation(result: AblationResult) -> str:
    lines = [
        "centroid_spread    {:.6f}".format(result.centroid_spread),
        "moved              {}".format(result.moved),
        "separated          {}".format(result.separated),
        "ordering_holds     {}".format(result.ordering_holds),
    ]
    if result.ordering_violations:
        lines.append("ordering violations:")
        lines += [f"  {v}" for v in result.ordering_violations]

    lines.append("")
    lines.append("JSD in bits between pinned states:")
    names = sorted(result.distances)
    for a in names:
        row = result.distances.get(a, {})
        rendered = "  ".join(f"{b}:{row[b]:.4f}" for b in names if b in row)
        lines.append(f"  {a:14s} {rendered}")
    return "\n".join(lines)