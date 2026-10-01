"""Versioned, consent-gated, retention-limited profile storage.

## What a Varys profile is, and what it deliberately is not

A profile is a **state trajectory**, not a log of what someone said. It holds running
aggregates over the 21-dim state with a count, a confidence, and timestamps. It does not hold
transcripts, quotes, message text, or anything the person said.

That is the single most important design decision in this module, and it is not a storage
optimisation. A store of raw text is a store of things a person said in a moment of distress
and would never choose to have read back to them. A store of aggregates can be wrong, can be
incomplete, and can be deleted, and nobody has to read their own worst night back to
themselves. The cost is that the profile cannot quote, and that is the right cost.

## Four properties, and why each is enforced where it is

**Consent is checked at the write and the read, not by the caller.** Every method that touches
persisted data calls :meth:`ConsentLedger.require` itself. A caller cannot forget, because
forgetting is not in the code path.

**Retention is enforced on read, not only on write.** Expiring only at write time means a
profile that is never written to again is read forever, which defeats the entire mechanism.
:meth:`ProfileStore.get` checks expiry and returns nothing.

**Erasure reaches the version log.** A redaction that only clears the current view while
older versions sit in the log is theatre, and it is the usual way these systems leak. Erasure
overwrites the payload of every version for that scope and leaves a tombstone, so the log
still shows that something was held and when it went, but the data itself is gone. That
sacrifice is real: you cannot have both a complete audit trail and true erasure. This picks
erasure and keeps the metadata, and the metadata is enough to answer "what did you hold and
when did you stop".

**Confidence travels with the aggregate.** A profile built from two observations is not the
same thing as one built from two hundred, and a consumer that cannot tell the difference will
act on a two-observation profile as though it were a settled fact. Every state carries
``n_observations`` and :meth:`ProfileState.is_trustworthy` refuses to certify anything below
a minimum. This is the same discipline as the evidence gate in
:mod:`varys.affective.state`: repetition, not a single reading, is what establishes something.

## The moral block

Moral data is separated out rather than lumped in with the rest, because it needs a shorter
retention window, a separate consent scope, and a manifest warning when it is written. The
reasoning is in :mod:`varys.affective.moral` and in :mod:`varys.profile.consent`: a profile of
which foundations someone appeals to is a blueprint for influencing them.

The code enforces the separation. It cannot enforce that whoever wrote the persuasion layer
downstream respects it, and nothing here pretends otherwise.

## Storage

JSON on disk, one file per profile, keyed by a pseudonymous id. No database, because a
profile store that is a SQLite file is a profile store somebody will add a query to at 2am
and forget to put behind the same consent check. A flat file is auditable by reading it.
"""

from __future__ import annotations

import hashlib
import json
import os
import tempfile
import time
from dataclasses import asdict, dataclass
from pathlib import Path
from typing import Dict, List, Optional, Sequence

from varys.affective.state import (
    MORAL_SLICE,
    SPECTRA_SLICE,
    STATE_DIMS,
    VAD_SLICE,
)
from varys.profile.consent import SCOPES, ConsentError, ConsentLedger

__all__ = [
    "ProfileId",
    "pseudonymise",
    "ProfileState",
    "ProfileVersion",
    "ProfileStore",
    "ProfileError",
]

#: Below this many observations an aggregate is not treated as established. Chosen to match
#: the evidence-gate philosophy elsewhere in Varys: one reading is a moment, not a person.
MIN_OBSERVATIONS = 3

#: Which state block each consent scope covers. The scope names are about *what was agreed*;
#: the block names are about *where it lives in the 21-dim state*, and they are not the same
#: words. Declared once because getting it wrong in one of several places is how a
#: "clinical" write ends up stored in the affect block, or an erase misses a block entirely.
SCOPE_BLOCK: Dict[str, str] = {
    "affect": "vad",
    "clinical": "spectra",
    "moral": "moral",
}


class ProfileError(RuntimeError):
    """Something about a profile or the store is wrong. Distinct from a consent failure."""


def _slice_for(block: str) -> slice:
    """The slice a named block occupies in the 21-dim state."""
    return {"vad": VAD_SLICE, "spectra": SPECTRA_SLICE, "moral": MORAL_SLICE}[block]


def pseudonymise(subject: str, salt: str) -> str:
    """A stable, non-reversible profile id from an identifying string.

    Profiles are keyed by a hash, not by a username or a Roblox user id. A file called
    ``user_12345.json`` is a list of everyone's ids; a file called
    ``a3f9...json`` is not, and the mapping never touches disk. That is the whole reason
    this function exists, and it is why the salt is supplied by the caller rather than
    generated: the salt has to live wherever the mapping is kept, and if it is in the same
    directory as the profiles then the pseudonym bought nothing.

    The id is truncated to 32 hex characters. There is no security claim beyond collision
    resistance; this is about not writing identifiers to disk, not about resisting an
    attacker who already has the store.
    """
    return hashlib.sha256(f"{salt}:{subject}".encode("utf-8")).hexdigest()[:32]


#: Re-exported so callers do not have to reach into the consent module for a scope name.
ProfileId = str


@dataclass
class ProfileState:
    """Running aggregates over one scope's block, plus provenance.

    Attributes:
        values: the 21-dim aggregate, with the blocks this scope does not cover left at zero.
        n_observations: how many turns contributed. The confidence is inseparable from this.
        updated_at: epoch seconds of the last contributing observation.
        scope: which block this state covers.
    """

    values: List[float]
    n_observations: int = 0
    updated_at: float = 0.0
    scope: str = "affect"

    def __post_init__(self) -> None:
        if len(self.values) != len(STATE_DIMS):
            raise ProfileError(
                f"profile state needs {len(STATE_DIMS)} dims, got {len(self.values)}"
            )
        self.values = [float(v) for v in self.values]

    def observe(self, state: Sequence[float], now: Optional[float] = None) -> "ProfileState":
        """Fold one turn's state into the running mean, in place.

        A running mean rather than a last-value store, because a profile that overwrites is a
        profile with no memory, and a profile with no memory is not longitudinal. The count
        increments on every call whether or not the value moved, because "how many times have
        we looked" is itself the evidence.

        VAD is averaged in its signed cube. The clinical and moral blocks are averaged in
        ``[0, 1]``, so their running means are intensities and not signed coordinates.
        """
        t = time.time() if now is None else now
        v = [float(x) for x in state]
        if len(v) != len(STATE_DIMS):
            raise ProfileError(f"observation needs {len(STATE_DIMS)} dims, got {len(v)}")

        n = self.n_observations
        for i, x in enumerate(v):
            if n == 0:
                self.values[i] = x
            else:
                self.values[i] += (x - self.values[i]) / (n + 1)
        self.n_observations = n + 1
        self.updated_at = t
        return self

    def is_trustworthy(self, minimum: int = MIN_OBSERVATIONS) -> bool:
        """Whether this aggregate has enough behind it to act on.

        Refusing to certify a thin profile is the difference between "the user has been low
        for a month" and "the user said something sad once", and only one of those is worth
        changing behaviour over.
        """
        return self.n_observations >= max(1, minimum)

    def block(self, name: str) -> List[float]:
        sl = {"vad": VAD_SLICE, "spectra": SPECTRA_SLICE, "moral": MORAL_SLICE}[name]
        return self.values[sl]

    def to_dict(self) -> Dict[str, object]:
        return asdict(self)

    @classmethod
    def from_dict(cls, d: Dict[str, object]) -> "ProfileState":
        return cls(
            values=list(d["values"]),  # type: ignore[arg-type]
            n_observations=int(d.get("n_observations", 0)),  # type: ignore[arg-type]
            updated_at=float(d.get("updated_at", 0.0)),  # type: ignore[arg-type]
            scope=str(d.get("scope", "affect")),
        )


@dataclass
class ProfileVersion:
    """One point in a profile's history.

    ``erased`` marks a tombstone: the payload was overwritten with zeros and only the
    metadata survives. Keeping the tombstone rather than dropping the version is deliberate,
    and the reason is in the module docstring: erasure and a complete audit trail cannot both
    be had, and this picks erasure while keeping enough to answer "what did you hold".
    """

    index: int
    at: float
    state: ProfileState
    note: str = ""
    erased: bool = False

    def to_dict(self) -> Dict[str, object]:
        d = asdict(self)
        d["state"] = self.state.to_dict()
        return d

    @classmethod
    def from_dict(cls, d: Dict[str, object]) -> "ProfileVersion":
        return cls(
            index=int(d["index"]),  # type: ignore[arg-type]
            at=float(d["at"]),  # type: ignore[arg-type]
            state=ProfileState.from_dict(d["state"]),  # type: ignore[arg-type]
            note=str(d.get("note", "")),
            erased=bool(d.get("erased", False)),
        )


class ProfileStore:
    """Consent-gated, versioned, retention-limited profile storage.

    Args:
        ledger: the consent authority. Required, and there is no default: a store without one
            is a store that can be written to unconditionally, and the whole point of
            passing it in is that this class, not the caller, decides what is permitted.
        root: directory for the profile files. Created if absent.
        retention_days: per-scope retention, overriding the consent defaults. A value of 0
            means "do not persist this scope", which is a supported setting and is how a
            deployment that does not want moral profiles on disk at all configures itself.
        min_observations: the trustworthiness threshold.
    """

    def __init__(
        self,
        ledger: ConsentLedger,
        root: Optional[Path] = None,
        retention_days: Optional[Dict[str, int]] = None,
        min_observations: int = MIN_OBSERVATIONS,
    ):
        self.ledger = ledger
        self.root = Path(root) if root is not None else None
        self.min_observations = min_observations
        self.retention_days: Dict[str, int] = {
            "affect": 30, "clinical": 14, "moral": 7, "history": 14
        }
        if retention_days:
            for scope, days in retention_days.items():
                if scope not in SCOPES:
                    raise ValueError(f"unknown scope {scope!r}; expected one of {SCOPES}")
                if days < 0:
                    raise ValueError(f"retention for {scope} must be >= 0, got {days}")
                self.retention_days[scope] = days

        if self.root is not None:
            self.root.mkdir(parents=True, exist_ok=True)
        self._mem: Dict[str, List[ProfileVersion]] = {}

    # ------------------------------------------------------------- internals

    def _path(self, pid: ProfileId) -> Optional[Path]:
        return None if self.root is None else self.root / f"{pid}.json"

    def _load(self, pid: ProfileId) -> List[ProfileVersion]:
        if pid in self._mem:
            return self._mem[pid]
        p = self._path(pid)
        if p is not None and p.exists():
            raw = json.loads(p.read_text(encoding="utf-8"))
            versions = [ProfileVersion.from_dict(v) for v in raw.get("versions", [])]
            self._mem[pid] = versions
            return versions
        self._mem[pid] = []
        return self._mem[pid]

    def _flush(self, pid: ProfileId) -> None:
        """Write atomically.

        A profile store that can be truncated by a crash mid-write is worse than useless: it
        looks like data loss but it is actually a consent record that no longer reflects what
        was held. The temp-file-then-rename sequence is the standard way to make that
        impossible on POSIX and Windows alike.
        """
        p = self._path(pid)
        if p is None:
            return
        payload = {"versions": [v.to_dict() for v in self._mem[pid]]}
        fd, tmp = tempfile.mkstemp(dir=str(p.parent), suffix=".tmp")
        try:
            with os.fdopen(fd, "w", encoding="utf-8") as fh:
                json.dump(payload, fh, indent=2)
            os.replace(tmp, p)
        except BaseException:
            if os.path.exists(tmp):
                os.unlink(tmp)
            raise

    def _expired(self, at: float, now: float, scope: str = "affect") -> bool:
        """Whether a record written at ``at`` has outlived *its own scope's* retention.

        The scope argument is not decoration. This used to look up the ``affect`` window
        unconditionally, so a moral profile was retained for 30 days instead of the 7 its
        scope specifies. That is the exact failure the per-scope windows exist to prevent:
        the most exploitable data outliving its own short window, silently, while the
        configuration file said otherwise.
        """
        window = self.retention_days.get(scope, self.retention_days.get("affect", 30))
        if window <= 0:
            return True
        return (now - at) > window * 86400.0

    # ---------------------------------------------------------------- writing

    def observe(
        self,
        pid: ProfileId,
        state: Sequence[float],
        scope: str = "affect",
        note: str = "",
        now: Optional[float] = None,
    ) -> ProfileVersion:
        """Fold one turn into a profile and cut a new version.

        Args:
            pid: a pseudonymous id, from :func:`pseudonymise`.
            state: the 21-dim turn state.
            scope: which block is being recorded. The other blocks are zeroed, so a profile
                written under ``moral`` consent is not a back door to recording affect.
            note: free text. Keep it factual. It is stored alongside the aggregate, which is
                why this is not for content the person said.

        Raises:
            ConsentError: if the scope is not currently granted. Enforced here, not by the
                caller.
        """
        if scope not in SCOPES:
            raise ValueError(f"unknown scope {scope!r}; expected one of {SCOPES}")
        if scope == "history":
            raise ValueError(
                "the history scope is not directly writable; it is a retention window on "
                "the version log, not a block of state"
            )
        t = time.time() if now is None else now
        self.ledger.require(scope, purpose="profile.observe", now=t)

        if self.retention_days.get(scope, 0) <= 0:
            # Retention of zero means "do not persist this". The observation is still
            # accepted and folded, so the caller is not left with a hole in its own state
            # machine, it just does not reach the disk.
            versions = self._load(pid)
            return ProfileVersion(
                index=len(versions),
                at=t,
                state=self._scope_only_state(state, scope, t),
                note=note + " [not retained]",
            )

        versions = self._load(pid)
        previous = self._current_state(pid, scope, t)
        folded = (
            previous.observe(state, now=t)
            if previous is not None
            else ProfileState(values=[0.0] * len(STATE_DIMS), scope=scope).observe(state, now=t)
        )
        # Keep only the scope's block; the rest is not consented and does not belong here.
        folded.values = self._mask_to_scope(folded.values, scope)

        version = ProfileVersion(index=len(versions), at=t, state=folded, note=note)
        versions.append(version)
        self._flush(pid)
        return version

    def _scope_only_state(self, state: Sequence[float], scope: str, t: float) -> ProfileState:
        return ProfileState(
            values=self._mask_to_scope([float(x) for x in state], scope),
            n_observations=1,
            updated_at=t,
            scope=scope,
        )

    def _mask_to_scope(self, values: Sequence[float], scope: str) -> List[float]:
        """Zero every block this scope does not cover."""
        out = [0.0] * len(STATE_DIMS)
        sl = _slice_for(SCOPE_BLOCK[scope])
        out[sl] = list(values[sl])
        return out

    def _current_state(
        self, pid: ProfileId, scope: str, now: Optional[float] = None
    ) -> Optional[ProfileState]:
        """The live aggregate for a scope, or ``None``.

        Skips erased versions and versions whose retention has lapsed, and matches on scope.

        It does *not* skip a version whose block is all zero. It used to, on the theory that
        an all-zero block means the scope was never really written. That conflated two
        different things: "nothing was recorded" and "we looked, and the answer was nothing".
        For the clinical block an all-zero reading genuinely means no spectrum is marked, and
        for the affect block an all-zero reading is a real observation of neutral affect.
        Collapsing them means a caller cannot tell "this person seems fine" from "we have no
        profile for them", which is the one distinction that matters when deciding whether to
        change behaviour.

        The zero-block check was also redundant. Writes are already masked to their own
        scope's block, and a clinical write is not visible to an affect query because the
        scope names differ. So dropping it loses no isolation and stops losing real
        observations.

        ``now`` is threaded through rather than read from the clock. It used to call
        ``time.time()`` internally, which meant a caller simulating a future moment still got
        a retention check against the present: a one-day window read as live two days in.
        """
        now = time.time() if now is None else now
        for v in reversed(self._load(pid)):
            if v.erased:
                continue
            if self._expired(v.at, now, v.state.scope):
                continue
            if v.state.scope != scope:
                continue
            return v.state
        return None

    # ---------------------------------------------------------------- reading

    def get(
        self,
        pid: ProfileId,
        scope: str = "affect",
        purpose: str = "",
        now: Optional[float] = None,
    ) -> Optional[ProfileState]:
        """The live aggregate, or ``None`` if absent, expired, or unconsented.

        Retention is checked here and not only at write time. A profile that is never
        written to again would otherwise be readable forever, which defeats the mechanism
        entirely.
        """
        t = time.time() if now is None else now
        try:
            self.ledger.require(scope, purpose=purpose or "profile.get", now=t)
        except ConsentError:
            return None
        # Returned even when thin. A caller asking for a profile has a reason, and silently
        # returning None for a two-observation profile would be indistinguishable from "no
        # profile exists". The count comes with it and `is_trustworthy` is one call away.
        return self._current_state(pid, scope, t)

    def is_trustworthy(self, state: Optional[ProfileState]) -> bool:
        """Whether an aggregate has enough observations behind it to act on."""
        return state is not None and state.is_trustworthy(self.min_observations)

    def history(
        self,
        pid: ProfileId,
        purpose: str = "",
        now: Optional[float] = None,
    ) -> List[ProfileVersion]:
        """The version log, oldest first, tombstones included.

        Requires the ``history`` scope. A rolling aggregate is one number; a trajectory is
        reconstructible in ways an aggregate is not, so it gets its own consent.
        """
        t = time.time() if now is None else now
        self.ledger.require("history", purpose=purpose or "profile.history", now=t)
        return [v for v in self._load(pid) if not self._expired(v.at, t, v.state.scope)]

    # ---------------------------------------------------------------- erasure

    def erase_scope(self, pid: ProfileId, scope: str, now: Optional[float] = None) -> int:
        """Overwrite every version's payload for a scope. Returns how many were erased.

        The payload is zeroed rather than the version dropped, so the log still records that
        something was held. Dropping the version outright would be a cleaner story and a
        worse one: it makes the erasure invisible, and an invisible erasure is not something
        an auditor can check.
        """
        if scope not in SCOPES:
            raise ValueError(f"unknown scope {scope!r}; expected one of {SCOPES}")
        count = 0
        for v in self._load(pid):
            if v.erased or v.state.scope != scope:
                continue
            v.state.values = [0.0] * len(STATE_DIMS)
            v.state.n_observations = 0
            v.erased = True
            v.note = (v.note + " [erased]").strip()
            count += 1
        if count:
            self._flush(pid)
        return count

    def erase_all(self, pid: ProfileId, now: Optional[float] = None) -> int:
        """Erase every version for a profile. The exit a person actually means."""
        count = 0
        for scope in ("affect", "clinical", "moral"):
            count += self.erase_scope(pid, scope, now=now)
        for v in self._load(pid):
            if not v.erased:
                v.state.values = [0.0] * len(STATE_DIMS)
                v.state.n_observations = 0
                v.erased = True
                count += 1
        if count:
            self._flush(pid)
        return count

    def delete(self, pid: ProfileId) -> None:
        """Remove the profile file entirely. Only after full revocation."""
        self._mem.pop(pid, None)
        p = self._path(pid)
        if p is not None and p.exists():
            p.unlink()

    def revoke_and_erase(self, pid: ProfileId, scope: Optional[str] = None) -> Dict[str, object]:
        """The operation a person means by "forget this": revoke, then erase what was held.

        Order matters. Erasing first and revoking second leaves a window where the data is
        gone but the permission to recreate it is still live.
        """
        revoked = self.ledger.revoke(scope)
        targets = revoked or [s for s in SCOPES if s != "history"]
        erased = 0
        for s in targets:
            if s == "history":
                continue
            erased += self.erase_scope(pid, s)
        for v in self._load(pid):
            if not v.erased:
                v.state.values = [0.0] * len(STATE_DIMS)
                v.state.n_observations = 0
                v.erased = True
                erased += 1
        self._flush(pid)
        return {"revoked": revoked, "erased": erased}

    # --------------------------------------------------------------- reporting

    def describe(self, pid: ProfileId, now: Optional[float] = None) -> Dict[str, object]:
        """A loggable summary. Contains no content and no scope that is not currently
        granted, so it is safe to put in a monitoring pipeline."""
        t = time.time() if now is None else now
        out: Dict[str, object] = {
            "id": pid[:8],
            "versions": len(self._load(pid)),
            "consent": self.ledger.live_scopes(t),
        }
        for scope in ("affect", "clinical", "moral"):
            if not self.ledger.has(scope, t):
                out[scope] = None
                continue
            state = self._current_state(pid, scope)
            out[scope] = {
                "n": state.n_observations if state else 0,
                "trustworthy": self.is_trustworthy(state),
                "updated_at": state.updated_at if state else 0.0,
            }
        return out

    def purge_expired(self, pid: ProfileId, now: Optional[float] = None) -> int:
        """Drop versions past their scope's retention. Returns how many went.

        Tombstones are kept regardless of age. The record that something was held and then
        destroyed outlives the data, which is the point of keeping it.
        """
        t = time.time() if now is None else now
        before = len(self._load(pid))
        kept = [v for v in self._load(pid) if v.erased or not self._expired(v.at, t, v.state.scope)]
        removed = before - len(kept)
        if removed:
            self._mem[pid] = kept
            self._flush(pid)
        return removed



