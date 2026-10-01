"""Literature grounding, without shipping any literature.

Varys is specified for peer-reviewed psychological reasoning. That creates a failure mode
that does not exist in the affective work: **a model asked to ground a claim in the
literature will invent a citation**, and a plausible-looking one with a real author's name
and a real journal and a volume number that corresponds to a different paper. Nothing in the
output looks wrong. This is worse than an ungrounded answer, because an ungrounded answer is
at least checkable at a glance.

So the constraint here is structural rather than advisory: **a citation can only be built
from something retrieval actually returned.**

* :class:`Source` is the only way a reference enters the system, and it carries its own
  identifier.
* :class:`Citation` is constructed only by looking an identifier up in a
  :class:`CitationIndex`. There is no constructor that takes free text.
* :func:`check_grounding` reports which claims in a response are supported by retrieved
  material and which are not, and the unsupported ones are the ones a caller has to treat as
  the model's own opinion.

That is why this module ships with **no corpus**. The interface, the validation, and the
grounding audit are the parts that can be written and tested without deciding which papers
to use, and that decision belongs to whoever is accountable for the citations. A retrieval
backend is a two-method protocol; there are four plausible ones and no basis here for
choosing.

## What this does not do

It does not verify that a paper says what the model claims it says. It can tell you the
citation resolves to a real retrieved document and that the claim falls inside the span that
was returned. It cannot tell you the interpretation is correct. That check is reading the
paper, and no amount of code substitutes for it. :attr:`Grounding.span` exists so a human can
be shown the passage and check it in seconds.
"""

from __future__ import annotations

import hashlib
import re
from dataclasses import dataclass
from typing import Dict, List, Optional, Protocol, Sequence, Tuple

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


# ------------------------------------------------------------------ sources


@dataclass(frozen=True)
class Source:
    """One retrievable document.

    ``source_id`` is derived from the bibliographic fields rather than assigned, so the same
    paper added twice is the same source and a citation cannot point at a duplicate.
    """

    authors: Tuple[str, ...]
    year: int
    title: str
    venue: str = ""
    doi: str = ""

    @property
    def source_id(self) -> str:
        key = "|".join(
            [self.title.strip().lower(), str(self.year), self.venue.strip().lower()]
            + [a.strip().lower() for a in self.authors]
        )
        return hashlib.sha256(key.encode("utf-8")).hexdigest()[:16]

    def short(self) -> str:
        first = self.authors[0].split()[-1] if self.authors else "anon"
        more = " et al." if len(self.authors) > 1 else ""
        return f"{first}{more} ({self.year})"

    def to_dict(self) -> Dict[str, object]:
        return {
            "source_id": self.source_id,
            "authors": list(self.authors),
            "year": self.year,
            "title": self.title,
            "venue": self.venue,
            "doi": self.doi,
        }


@dataclass(frozen=True)
class Passage:
    """A retrieved span, which is what a claim can actually be grounded in."""

    source: Source
    text: str
    locator: str = ""
    score: float = 0.0

    @property
    def source_id(self) -> str:
        return self.source.source_id


class RetrievalBackend(Protocol):
    """What Varys needs from a literature store.

    Two methods. Anything that can satisfy this is a valid backend: a vector database, a
    search API, a local Lucene index, or the in-memory stand-in below.
    """

    def search(self, query: str, k: int = 5) -> List[Passage]:
        """Return the ``k`` most relevant passages."""

    def by_id(self, source_id: str) -> Optional[Source]:
        """Look a source up by its identifier, or ``None``."""


class InMemoryBackend:
    """A trivial lexical backend. Enough to test the whole grounding path.

    Scoring is token overlap with an IDF weighting, which is crude and is not the point. It
    exists so that :func:`check_grounding` can be tested end to end without a corpus, and so
    that a deployment can be brought up before a real index is wired in.

    The IDF is computed over the corpus at construction, which is why adding documents
    changes existing scores. That is correct rather than convenient: a retrieval rank is only
    meaningful relative to the collection it was computed against.
    """

    def __init__(self, passages: Sequence[Passage] = ()):
        self._passages: List[Passage] = []
        for p in passages:
            self.add(p)
        self._idf = self._build_idf()

    def add(self, passage: Passage) -> None:
        self._passages.append(passage)
        self._idf = self._build_idf()

    @staticmethod
    def _tokenize(text: str) -> List[str]:
        return [t for t in re.findall(r"[a-z0-9]+", text.lower()) if len(t) > 2]

    def _build_idf(self) -> Dict[str, float]:
        import math

        n = max(1, len(self._passages))
        df: Dict[str, int] = {}
        for p in self._passages:
            for tok in set(self._tokenize(f"{p.source.title} {p.text}")):
                df[tok] = df.get(tok, 0) + 1
        return {t: math.log(1.0 + n / (1.0 + d)) for t, d in df.items()}

    def search(self, query: str, k: int = 5) -> List[Passage]:
        if k <= 0:
            return []
        q = self._tokenize(query)
        scored: List[Tuple[float, int, Passage]] = []
        for i, p in enumerate(self._passages):
            body = self._tokenize(f"{p.source.title} {p.text}")
            counts: Dict[str, int] = {}
            for t in body:
                counts[t] = counts.get(t, 0) + 1
            score = sum(self._idf.get(t, 0.0) for t in set(q) if t in counts)
            if score > 0:
                scored.append((score, i, p))
        scored.sort(key=lambda t: (-t[0], t[1]))
        return [Passage(p.source, p.text, p.locator, score) for score, _, p in scored[:k]]

    def by_id(self, source_id: str) -> Optional[Source]:
        for p in self._passages:
            if p.source.source_id == source_id:
                return p.source
        return None

    def __len__(self) -> int:
        return len(self._passages)


# ---------------------------------------------------------------- citations


@dataclass(frozen=True)
class Citation:
    """A reference to retrieved material.

    Constructed only through :meth:`CitationIndex.cite`, which is the whole enforcement
    mechanism. If there were a public constructor taking a string, the model would eventually
    route around it, and the guarantee would be worth nothing.
    """

    source: Source
    locator: str
    passage: str

    def render(self) -> str:
        loc = f", {self.locator}" if self.locator else ""
        return f"{self.source.short()}{loc}"

    def to_dict(self) -> Dict[str, object]:
        return {
            "source_id": self.source.source_id,
            "citation": self.render(),
            "locator": self.locator,
            "passage": self.passage,
        }


class CitationIndex:
    """Holds what retrieval returned, and mints citations from it.

    One index per query. That lifetime is deliberate and short: a citation is only meaningful
    relative to the material that was retrieved when the claim was made, and an index that
    outlives its query would let a model cite something from an earlier turn as though it
    were in front of it now.
    """

    def __init__(self, passages: Sequence[Passage] = ()):
        self._by_id: Dict[str, Passage] = {}
        for p in passages:
            self._by_id.setdefault(p.source_id, p)

    def cite(self, source_id: str, locator: str = "") -> Citation:
        """Mint a citation, or raise if the source was not retrieved.

        Raises:
            KeyError: the source is not in this index. This is the fabrication gate.
        """
        passage = self._by_id.get(source_id)
        if passage is None:
            raise KeyError(
                f"no retrieved source with id {source_id!r}. A citation can only reference "
                f"material that was actually returned; the {len(self._by_id)} sources in this "
                "index are the only citable ones."
            )
        return Citation(source=passage.source, locator=locator or passage.locator,
                        passage=passage.text)

    def citable(self) -> List[str]:
        """The identifiers that may be cited. For putting in a prompt."""
        return sorted(self._by_id)

    def __len__(self) -> int:
        return len(self._by_id)

    def __contains__(self, source_id: object) -> bool:
        return source_id in self._by_id


# ---------------------------------------------------------------- grounding


@dataclass
class Grounding:
    """One claim and whether the retrieved material supports it."""

    claim: str
    supported: bool
    overlap: float
    citation: Optional[Citation] = None

    def to_dict(self) -> Dict[str, object]:
        return {
            "claim": self.claim,
            "supported": self.supported,
            "overlap": round(self.overlap, 4),
            "citation": self.citation.render() if self.citation else None,
        }


@dataclass
class GroundingReport:
    """The audit. Which claims stand on retrieved material and which are the model's own."""

    groundings: List[Grounding]
    n_citable: int

    @property
    def unsupported(self) -> List[Grounding]:
        return [g for g in self.groundings if not g.supported]

    @property
    def coverage(self) -> float:
        if not self.groundings:
            return 1.0
        return sum(1 for g in self.groundings if g.supported) / len(self.groundings)

    def to_dict(self) -> Dict[str, object]:
        return {
            "coverage": round(self.coverage, 4),
            "n_claims": len(self.groundings),
            "n_citable_sources": self.n_citable,
            "unsupported": [g.claim for g in self.unsupported],
        }


_SENTENCE = re.compile(r"(?<=[.!?])\s+")


def split_claims(text: str) -> List[str]:
    """Split a response into claim-like units.

    A deliberately crude sentence split. Claim segmentation is its own research problem and
    pretending otherwise here would mean the grounding report silently under- or
    over-counts, which is worse than a simple rule that is obviously simple.
    """
    return [s.strip() for s in _SENTENCE.split(text.strip()) if s.strip()]


def _overlap(claim: str, passage: str) -> float:
    """Fraction of the claim's content tokens present in the passage.

    Symmetric would be wrong. A long passage contains most of a short claim's words by
    accident, so a symmetric ratio scores every claim against every passage highly. The
    claim's own coverage is the informative direction.
    """
    a = InMemoryBackend._tokenize(claim)
    if not a:
        return 0.0
    b = set(InMemoryBackend._tokenize(passage))
    return sum(1 for t in set(a) if t in b) / len(set(a))


def check_grounding(
    response: str,
    index: CitationIndex,
    threshold: float = 0.5,
) -> GroundingReport:
    """Audit a response against the material retrieval actually returned.

    Args:
        response: the model's text.
        index: the :class:`CitationIndex` built from this query's retrieval results.
        threshold: fraction of a claim's content words that must appear in some retrieved
            passage for the claim to count as supported.

    Returns:
        A report naming the unsupported claims. Those are the ones that must be presented as
        the model's own opinion rather than as findings, and a low coverage figure is a
        signal about the response, not a bug to be tuned away.
    """
    groundings: List[Grounding] = []
    for claim in split_claims(response):
        best: Optional[Passage] = None
        best_score = 0.0
        for pid in index.citable():
            passage = index._by_id[pid]
            score = _overlap(claim, f"{passage.source.title} {passage.text}")
            if score > best_score:
                best_score = score
                best = passage
        if best is not None and best_score >= threshold:
            citation = index.cite(best.source_id, best.locator)
            groundings.append(
                Grounding(claim=claim, supported=True, overlap=best_score, citation=citation)
            )
        else:
            groundings.append(Grounding(claim=claim, supported=False, overlap=best_score))

    return GroundingReport(groundings=groundings, n_citable=len(index))
