"""Longitudinal profiles: the part that makes Varys longitudinal rather than long-context.

    consent.py   scoped, expiring, revocable, enforced grants
    store.py     versioned, retention-limited, consent-gated storage

A long context means the model can read a lot of one conversation. A profile means it can
remember a person across conversations. Those are different capabilities and only the second
one is longitudinal, and the second one is the one that carries obligations.

## The short version of the ethics, since the code cannot carry them

A profile of someone's affect over months is a sensitive record. A profile of their clinical
spectra is more so. A profile of which moral foundations they appeal to is a blueprint for
influencing them, because the cheapest way to move a person is to activate their own values
rather than to argue with them.

So: three separate consent scopes rather than one, per-scope retention rather than one
window, the most sensitive block stored separately and erased first, and every read and write
checking consent itself rather than trusting the caller to.

## What this is not

Not a memory system, not a user store, not a database. It holds 21-dim aggregates with a
count and a timestamp, never transcripts and never content the person said. See
:mod:`varys.profile.store` for why that particular trade is the one to make.
"""

from varys.profile.consent import (
    DEFAULT_TTL_DAYS,
    SCOPES,
    Consent,
    ConsentError,
    ConsentLedger,
)
from varys.profile.store import (
    MIN_OBSERVATIONS,
    ProfileError,
    ProfileId,
    ProfileState,
    ProfileStore,
    ProfileVersion,
    pseudonymise,
)

__all__ = [
    "SCOPES",
    "DEFAULT_TTL_DAYS",
    "Consent",
    "ConsentError",
    "ConsentLedger",
    "ProfileId",
    "ProfileState",
    "ProfileVersion",
    "ProfileStore",
    "ProfileError",
    "pseudonymise",
    "MIN_OBSERVATIONS",
]
