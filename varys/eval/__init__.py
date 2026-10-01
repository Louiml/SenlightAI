"""Evaluation for Varys.

    ablation.py    pinned-state ablation, per state block
    agreement.py  sycophancy measurement against paired false premises

What is *not* here yet, and should be before anyone reads a number out of this as a quality
claim: a clinical-validity suite, and an adversarial suite targeting the persuasion surface
that :mod:`varys.affective.moral` describes. Both need data this repository does not have.
They are named in ``README.md`` under what a Varys evaluation has to include and has not yet
got.

The two modules present are the ones that can be written and pinned without a corpus, and
they are the two whose absence would most easily let a broken model look fine: a model whose
affective bias is inert and a model that has learned to agree with everything both produce
fluent, plausible output.
"""

from varys.eval.ablation import (
    AblationCondition,
    BlockAblationResult,
    default_conditions,
    format_ablation,
    run_block_ablation,
)
from varys.eval.agreement import (
    ELAFRY_DISAGREEMENT_BASELINE,
    AgreementVerdict,
    SycophancyResult,
    classify_agreement,
    measure_sycophancy,
    sycophancy_rate,
)

__all__ = [
    "AblationCondition",
    "BlockAblationResult",
    "run_block_ablation", "default_conditions",
    "format_ablation",
    "AgreementVerdict",
    "SycophancyResult",
    "classify_agreement",
    "sycophancy_rate",
    "measure_sycophancy",
    "ELAFRY_DISAGREEMENT_BASELINE",
]
