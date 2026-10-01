"""Literature grounding. No corpus ships with this.

    literature.py   sources, retrieval protocol, citation minting, grounding audit

The point of this module is negative: it makes it structurally impossible for the model to
emit a citation to something that was not retrieved. A model asked to reason from the
peer-reviewed literature will otherwise invent references that look entirely real, and a
plausible citation is harder to catch than an obviously absent one.

See :mod:`varys.knowledge.literature` for why no corpus is included and what that leaves for
whoever deploys this.
"""

from varys.knowledge.literature import (
    Citation,
    CitationIndex,
    Grounding,
    GroundingReport,
    InMemoryBackend,
    Passage,
    RetrievalBackend,
    Source,
    check_grounding,
    split_claims,
)

__all__ = [
    "Source",
    "Passage",
    "RetrievalBackend",
    "InMemoryBackend",
    "Citation",
    "CitationIndex",
    "Grounding",
    "GroundingReport",
    "check_grounding",
    "split_claims",
]
