"""Evaluation: the sycophancy harness and the pinned-state ablation."""

from elafry.eval.ablation import (
    AblationResult,
    distribution_distance,
    format_ablation,
    pin_and_generate,
    run_ablation,
)
from elafry.eval.agreement import AgreementDetector, AgreementScore
from elafry.eval.metrics import (
    CRISIS_PROBES,
    EvalReport,
    MetricSet,
    crisis_response_quality,
    evaluate_probes,
    format_report,
    pushback_specificity,
)
from elafry.eval.probes import PROBES, PROBE_CATEGORIES, Probe, false_premise_prompts

__all__ = [
    "AblationResult",
    "AgreementDetector",
    "AgreementScore",
    "CRISIS_PROBES",
    "EvalReport",
    "MetricSet",
    "PROBES",
    "PROBE_CATEGORIES",
    "Probe",
    "crisis_response_quality",
    "distribution_distance",
    "evaluate_probes",
    "false_premise_prompts",
    "format_ablation",
    "format_report",
    "pin_and_generate",
    "pushback_specificity",
    "run_ablation",
]