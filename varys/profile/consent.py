"""Consent: scoped, expiring, revocable, and enforced rather than declared.

## The failure mode this exists to prevent

A ``user.consented: bool`` field somewhere in the config is not consent. It is a comment. It
is checked by whoever remembers to check it, which is nobody, and it survives revocation
because nothing reads the revocation. Every real consent system that has leaked has had
consent modelled as a flag.

So consent here is:

* **Scoped.** Not "the user agreed to something". Agreement to remember that someone was sad
  is not agreement to remember that they have a thought disorder, and it is certainly not
  agreement to record which moral foundations they appeal to. Each is a separate grant.
* **Expiring.** Consent that never expires is not consent, it is a licence. Every scope has
  a default lifetime in days, and an expired grant is not a grant.
* **Revocable, with erasure.** :meth:`ConsentLedger.revoke` returns the scopes that were
  actually in force, which is what the store needs to know what to delete.
* **Enforced at the storage boundary.** :meth:`ConsentLedger.require` raises rather than
  returning a bool, so a caller cannot forget to check the result.

## The scopes, and why they are not one scope

``AFFECT`` is valence, arousal, dominance over time. Low sensitivity, and it is the thing the
architecture is actually for.

``CLINICAL`` is the HiTOP spectra. Materially more sensitive, because a spectrum is an
organising axis over someone's mental health and storing it creates a record the person may
never see and cannot contest.

``MORAL`` is the Moral Foundations. This is the dangerous one, and the reasoning is in
:mod:`varys.affective.moral`: a profile of which foundations someone appeals to is a
blueprint for influencing them, because the cheapest way to move a person is to activate
their own values rather than to argue with them. Storing this is not a normal data-governance
question. It gets its own scope, a short default lifetime, and it has to be granted
separately from the other two.

``HISTORY`` is the version log. A rolling aggregate is one number; a version log is a
trajectory, and a trajectory is reconstructible in ways an aggregate is not.

The defaults are conservative on purpose. Someone who consents to a chat assistant
remembering they seemed low has not consented to the other four things, and the code should
make them ask.

## What consent does not cover

Consent to store is not consent to use. A store can be fully consented and still be
misused: retaining everything, or reading a profile in a context the person did not expect.
:meth:`ConsentLedger.require` takes a ``purpose`` string which is recorded in the audit log,
so a read has an attributable reason attached to it. It is not a technical control and does
not pretend to be one. It is the difference between a record and an alibi.
"""

from __future__ import annotations

import time
from dataclasses import dataclass
from typing import Dict, List, Optional, Tuple

__all__ = [
    "SCOPES",
    "DEFAULT_TTL_DAYS",
    "Consent",
    "ConsentError",
    "ConsentLedger",
]


#: The scopes, least to most sensitive. Order matters: it is the escalation path.
SCOPES: Tuple[str, ...] = ("affect", "clinical", "moral", "history")

#: Default lifetime per scope, in days. A grant with no explicit expiry gets these.
#:
#: Moral is seven days rather than thirty because the thing it records is the thing that
#: makes a profile exploitable. Halving that window is a real reduction in exposure and costs
#: the system very little, because the values it is tracking are slow-moving anyway and a
#: week-long window captures them adequately.
DEFAULT_TTL_DAYS: Dict[str, int] = {
    "affect": 30,
    "clinical": 14,
    "moral": 7,
    "history": 14,
}

_DAY = 86400.0


class ConsentError(PermissionError):
    """Raised when an operation needs a grant it does not have.

    A ``PermissionError`` rather than a ``ValueError`` so a caller cannot catch it by
    accident alongside ordinary bad-argument handling, and so it reads correctly in a
    traceback.
    """


@dataclass(frozen=True)
class Consent:
    """One grant, for one scope, over a bounded window."""

    scope: str
    granted_at: float
    expires_at: float
    purpose: str = ""

    def is_live(self, now: Optional[float] = None) -> bool:
        """Whether the grant is currently in force. Expiry is inclusive of the boundary."""
        t = time.time() if now is None else now
        return self.granted_at <= t < self.expires_at

    def remaining(self, now: Optional[float] = None) -> float:
        t = time.time() if now is None else now
        return max(0.0, self.expires_at - t)

    def to_dict(self) -> Dict[str, object]:
        return {
            "scope": self.scope,
            "granted_at": self.granted_at,
            "expires_at": self.expires_at,
            "purpose": self.purpose,
        }


class ConsentLedger:
    """Grants, revocations, and the audit trail.

    In-memory by design. The ledger is the thing that decides what a store is *allowed* to
    keep, so it deliberately outlives nothing and is rebuilt from the user's explicit choices
    on every session. A ledger that persists is a ledger that can be restored from a stale
    backup and quietly re-grant something the person withdrew nine months ago.

    The audit log is append-only and records every grant and every revocation, including the
    ones that failed. It holds no profile content.
    """

    def __init__(self, now: Optional[float] = None) -> None:
        self._t0 = time.time() if now is None else now
        self._grants: Dict[str, Consent] = {}
        self._audit: List[Dict[str, object]] = []

    # ------------------------------------------------------------- granting

    def grant(
        self,
        scope: str,
        days: Optional[int] = None,
        purpose: str = "",
        now: Optional[float] = None,
    ) -> Consent:
        """Grant a scope for a bounded window.

        Args:
            scope: one of :data:`SCOPES`.
            days: lifetime. ``None`` uses :data:`DEFAULT_TTL_DAYS`. Must be positive; a
                grant with no expiry is the thing this whole module exists to prevent, so
                there is no way to ask for one.
            purpose: recorded in the audit log. Empty is allowed, because sometimes there
                genuinely isn't a more specific answer, but a purpose string is what makes a
                later read attributable.
        """
        if scope not in SCOPES:
            raise ValueError(f"unknown consent scope {scope!r}; expected one of {SCOPES}")
        if days is not None and days <= 0:
            raise ValueError(
                f"days must be positive, got {days}; a consent grant with no expiry is not a "
                "grant, it is a licence"
            )

        t = self._t0 if now is None else now
        lifetime = DEFAULT_TTL_DAYS[scope] if days is None else days
        c = Consent(
            scope=scope,
            granted_at=t,
            expires_at=t + lifetime * _DAY,
            purpose=purpose,
        )
        self._grants[scope] = c
        self._audit.append({"at": t, "action": "grant", "scope": scope, "days": lifetime})
        return c

    def revoke(self, scope: Optional[str] = None, now: Optional[float] = None) -> List[str]:
        """Withdraw one scope, or all of them.

        Returns:
            The scopes that were actually in force, which is what the caller needs in order
            to erase the corresponding data. Returning the *requested* scope instead would
            mean a caller that revoked something it never had would be told to delete data
            belonging to a different scope.
        """
        t = self._t0 if now is None else now
        targets = SCOPES if scope is None else (scope,)
        if scope is not None and scope not in SCOPES:
            raise ValueError(f"unknown consent scope {scope!r}; expected one of {SCOPES}")

        revoked: List[str] = []
        for s in targets:
            existing = self._grants.get(s)
            if existing is not None and existing.is_live(t):
                revoked.append(s)
            self._grants.pop(s, None)

        self._audit.append({"at": t, "action": "revoke", "scope": scope or "*", "revoked": revoked})
        return revoked

    # -------------------------------------------------------------- reading

    def has(self, scope: str, now: Optional[float] = None) -> bool:
        c = self._grants.get(scope)
        return c is not None and c.is_live(now)

    def get(self, scope: str, now: Optional[float] = None) -> Optional[Consent]:
        c = self._grants.get(scope)
        if c is None or not c.is_live(now):
            return None
        return c

    def require(self, scope: str, purpose: str = "", now: Optional[float] = None) -> Consent:
        """Return a live grant or raise. The enforcement point.

        Raises:
            ConsentError: if the scope is unknown, was never granted, was revoked, or has
                expired. The message names the scope and says which of those it was, because
                a caller debugging a refusal needs to know whether to ask the user or to fix
                a bug.
        """
        if scope not in SCOPES:
            raise ConsentError(f"unknown consent scope {scope!r}; expected one of {SCOPES}")
        c = self._grants.get(scope)
        if c is None:
            raise ConsentError(
                f"no consent for scope {scope!r}. The person has not agreed to this, and the "
                "agreement has to be asked for explicitly."
            )
        if not c.is_live(now):
            raise ConsentError(
                f"consent for scope {scope!r} expired at {c.expires_at:.0f}. Re-ask rather "
                "than extending it silently."
            )
        if purpose:
            self._audit.append(
                {
                    "at": self._t0 if now is None else now,
                    "action": "use",
                    "scope": scope,
                    "purpose": purpose,
                }
            )
        return c

    def live_scopes(self, now: Optional[float] = None) -> List[str]:
        """Scopes currently in force, least to most sensitive."""
        return [s for s in SCOPES if self.has(s, now)]

    def expiring_within(self, days: float, now: Optional[float] = None) -> List[str]:
        """Scopes that lapse inside ``days``. For a prompt to re-ask before they lapse."""
        t = self._t0 if now is None else now
        out = []
        for s in SCOPES:
            c = self.get(s, t)
            if c is not None and c.remaining(t) < days * _DAY:
                out.append(s)
        return out

    def audit(self) -> List[Dict[str, object]]:
        """The full log. Contains scopes and timestamps, never profile content."""
        return list(self._audit)

    def to_dict(self) -> Dict[str, object]:
        return {
            "grants": {s: c.to_dict() for s, c in self._grants.items()},
            "live": self.live_scopes(),
        }
