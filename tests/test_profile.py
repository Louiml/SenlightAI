"""Tests for consent and longitudinal profiles.

The properties under test are mostly about what *cannot* happen:

* nothing is written without a live grant for the right scope
* nothing survives its scope's retention window
* erasing a scope reaches the version log, not just the current view
* a profile with two observations is never certified as trustworthy

Each of those corresponds to a way this subsystem could be harmless-looking and wrong. A
profile store that works perfectly and is enforceable is a much worse artefact than one that
is slightly clumsy, so the awkward cases are the ones worth pinning.
"""

from __future__ import annotations

import json
import time

import pytest

from varys.affective.state import MORAL_SLICE, STATE_DIMS, VAD_SLICE
from varys.profile import (
    DEFAULT_TTL_DAYS,
    MIN_OBSERVATIONS,
    SCOPES,
    ConsentError,
    ConsentLedger,
    ProfileError,
    ProfileState,
    ProfileStore,
    ProfileVersion,
    pseudonymise,
)

AFFECT_STATE = [-0.6, 0.5, -0.4] + [0.0] * 6 + [0.0] * 12
CLINICAL_STATE = [0.0] * 3 + [0.7] * 6 + [0.0] * 12
MORAL_STATE = [0.0] * 3 + [0.0] * 6 + [0.9] * 12


def _store(tmp_path, retention=None, grants=("affect", "clinical", "moral", "history"), days=30):
    ledger = ConsentLedger()
    for scope in grants:
        ledger.grant(scope, days=days)
    return ProfileStore(ledger, root=tmp_path, retention_days=retention), ledger


# ------------------------------------------------------------------ consent

def test_nothing_is_live_before_anything_is_granted():
    assert ConsentLedger().live_scopes() == []


def test_require_raises_when_nothing_was_granted():
    with pytest.raises(ConsentError, match="has not agreed"):
        ConsentLedger().require("affect")


def test_require_raises_after_revocation():
    led = ConsentLedger()
    led.grant("affect")
    led.revoke("affect")
    with pytest.raises(ConsentError, match="no consent"):
        led.require("affect")


def test_require_raises_after_expiry_and_says_so():
    """The message has to distinguish "ask the user" from "fix a bug"."""
    led = ConsentLedger()
    led.grant("affect", days=1)
    with pytest.raises(ConsentError, match="expired"):
        led.require("affect", now=time.time() + 2 * 86400)


def test_a_grant_cannot_be_given_without_an_expiry():
    """A consent grant with no expiry is not a grant, it is a licence."""
    with pytest.raises(ValueError, match="not a grant, it is a licence"):
        ConsentLedger().grant("affect", days=0)
    with pytest.raises(ValueError, match="must be positive"):
        ConsentLedger().grant("affect", days=-5)


def test_unknown_scope_is_rejected_everywhere():
    led = ConsentLedger()
    with pytest.raises(ValueError, match="unknown consent scope"):
        led.grant("psychology")
    with pytest.raises(ConsentError, match="unknown consent scope"):
        led.require("psychology")


def test_scopes_are_ordered_by_sensitivity():
    """Order matters: it is the escalation path, and the defaults follow it."""
    assert list(SCOPES) == ["affect", "clinical", "moral", "history"]


def test_moral_gets_a_shorter_window_than_affect():
    """A profile of which foundations someone appeals to is a blueprint for influencing
    them, so it should not sit around as long as a mood."""
    assert DEFAULT_TTL_DAYS["moral"] < DEFAULT_TTL_DAYS["affect"]
    assert DEFAULT_TTL_DAYS["moral"] <= 7


def test_revocation_reports_what_was_actually_in_force():
    """Returning the requested scope instead would tell a caller to delete data belonging to
    a scope they never had consent for."""
    led = ConsentLedger()
    led.grant("affect")
    led.grant("moral")
    assert led.revoke("affect") == ["affect"]
    assert led.revoke("clinical") == []          # never granted
    assert led.revoke("affect") == []            # already gone


def test_revoke_all_clears_everything():
    led = ConsentLedger()
    for s in SCOPES:
        led.grant(s)
    assert set(led.revoke()) == set(SCOPES)
    assert led.live_scopes() == []


def test_expiring_within_warns_before_it_lapses():
    led = ConsentLedger()
    led.grant("affect", days=10)
    led.grant("moral", days=30)
    assert "affect" in led.expiring_within(15)
    assert "moral" not in led.expiring_within(15)


def test_audit_records_uses_and_never_profile_content():
    led = ConsentLedger()
    led.grant("affect")
    led.require("affect", purpose="profile.observe")
    led.revoke("affect")
    log = led.audit()
    assert [e["action"] for e in log] == ["grant", "use", "revoke"]
    assert log[1]["purpose"] == "profile.observe"
    assert not any("values" in e or "state" in e for e in log)


def test_consent_is_revocable_even_when_never_expires_on_its_own():
    led = ConsentLedger()
    led.grant("moral", days=365)
    assert led.has("moral")
    led.revoke()
    assert not led.has("moral")


# ---------------------------------------------------------------- pseudonym

def test_pseudonym_is_stable_and_not_reversible():
    a = pseudonymise("user_12345", "pepper")
    b = pseudonymise("user_12345", "pepper")
    c = pseudonymise("user_12345", "different-pepper")
    d = pseudonymise("user_99999", "pepper")
    assert a == b
    assert a != c and a != d
    assert "user_12345" not in a
    assert len(a) == 32


def test_pseudonym_differs_by_salt():
    """Otherwise the store is a list of user ids with extra steps."""
    assert pseudonymise("x", "salt-a") != pseudonymise("x", "salt-b")


# ------------------------------------------------------------------- state

def test_profile_state_running_mean():
    st = ProfileState(values=[0.0] * len(STATE_DIMS))
    st.observe([-1.0, 0.0, 0.0] + [0.0] * 18)
    st.observe([1.0, 0.0, 0.0] + [0.0] * 18)
    assert st.values[VAD_SLICE.start] == pytest.approx(0.0)
    assert st.n_observations == 2


def test_profile_state_rejects_a_wrong_width():
    with pytest.raises(ProfileError, match="needs 21"):
        ProfileState(values=[0.0] * 3)
    with pytest.raises(ProfileError, match="needs 21"):
        ProfileState(values=[0.0] * 21).observe([0.0] * 5)


def test_a_thin_profile_is_not_trustworthy():
    """'The user said something sad once' is not the same as 'the user has been low for a
    month', and only one of those should change behaviour."""
    st = ProfileState(values=[0.0] * len(STATE_DIMS))
    assert not st.is_trustworthy()
    for _ in range(MIN_OBSERVATIONS - 1):
        st.observe(AFFECT_STATE)
    assert not st.is_trustworthy()
    st.observe(AFFECT_STATE)
    assert st.is_trustworthy()


def test_block_accessor_returns_the_right_slice():
    st = ProfileState(values=MORAL_STATE)
    assert st.block("moral") == MORAL_STATE[MORAL_SLICE]
    assert st.block("vad") == MORAL_STATE[VAD_SLICE]
    with pytest.raises(KeyError):
        st.block("nope")


# ------------------------------------------------------------------- store

def test_writing_without_consent_is_refused(tmp_path):
    """Enforced in the store, not by the caller. A caller who forgets to check is exactly
    the case this exists for."""
    store = ProfileStore(ConsentLedger(), root=tmp_path)
    with pytest.raises(ConsentError):
        store.observe("p1", AFFECT_STATE, scope="affect")


def test_reading_without_consent_returns_nothing(tmp_path):
    store, _ = _store(tmp_path)
    store.observe("p1", AFFECT_STATE, scope="affect")
    bare = ProfileStore(ConsentLedger(), root=tmp_path)
    assert bare.get("p1", "affect") is None


def test_scopes_do_not_leak_into_each_other(tmp_path):
    """A profile written under moral consent must not become a back door to recording
    affect, and a clinical write must not fill the affect block."""
    store, _ = _store(tmp_path)
    store.observe("p1", CLINICAL_STATE, scope="clinical")
    got = store.get("p1", "affect")
    assert got is None or all(abs(x) < 1e-9 for x in got.block("vad"))
    assert all(abs(x) < 1e-9 for x in store.get("p1", "clinical").block("moral"))


def test_a_neutral_reading_is_not_the_same_as_no_profile(tmp_path):
    """"We looked and it was neutral" and "we have no profile" have to be distinguishable.
    A store that collapses them cannot tell a caller whether changing behaviour is
    warranted, which is the only thing the profile is for."""
    store, _ = _store(tmp_path)
    neutral = [0.0] * 21
    store.observe("p1", neutral, scope="affect")
    got = store.get("p1", "affect")
    assert got is not None
    assert got.n_observations == 1
    assert all(abs(x) < 1e-12 for x in got.block("vad"))
    assert store.get("p2", "affect") is None      # genuinely no profile


def test_reading_a_scope_you_did_not_write_returns_none(tmp_path):
    store, _ = _store(tmp_path)
    store.observe("p1", CLINICAL_STATE, scope="clinical")
    assert store.get("p1", "moral") is None


def test_history_requires_its_own_scope(tmp_path):
    """A rolling aggregate is one number; a trajectory is reconstructible in ways an
    aggregate is not."""
    store, _ = _store(tmp_path, grants=("affect",))
    store.observe("p1", AFFECT_STATE, scope="affect")
    with pytest.raises(ConsentError):
        store.history("p1")


def test_history_is_ordered_and_cuts_a_version_per_write(tmp_path):
    store, _ = _store(tmp_path)
    for _ in range(3):
        store.observe("p1", AFFECT_STATE, scope="affect")
    h = store.history("p1")
    assert [v.index for v in h] == [0, 1, 2]
    assert h[-1].at >= h[0].at


def test_history_scope_is_not_directly_writable(tmp_path):
    store, _ = _store(tmp_path)
    with pytest.raises(ValueError, match="retention window"):
        store.observe("p1", AFFECT_STATE, scope="history")


def test_retention_is_per_scope_and_enforced_on_read(tmp_path):
    """This is the fix for a real bug: the expiry check used the affect window for every
    scope, so a moral profile lived 30 days instead of the 7 its scope specifies. That is
    the exact failure the per-scope windows exist to prevent, and it happened silently while
    the configuration file said otherwise."""
    ledger = ConsentLedger()
    ledger.grant("affect", days=30)
    ledger.grant("moral", days=30)
    store = ProfileStore(ledger, root=tmp_path, retention_days={"affect": 1, "moral": 7})
    t0 = time.time()
    store.observe("p1", AFFECT_STATE, scope="affect", now=t0)
    store.observe("p1", MORAL_STATE, scope="moral", now=t0)

    assert store.get("p1", "affect", now=t0) is not None
    assert store.get("p1", "moral", now=t0) is not None

    t2 = t0 + 2 * 86400
    assert store.get("p1", "affect", now=t2) is None      # window was 1 day
    assert store.get("p1", "moral", now=t2) is not None   # window is 7 days

    t8 = t0 + 8 * 86400
    assert store.get("p1", "moral", now=t8) is None


def test_retention_is_checked_on_read_not_only_on_write(tmp_path):
    """Expiring at write time only means a profile nobody writes to again is read forever,
    which defeats the mechanism."""
    ledger = ConsentLedger()
    ledger.grant("affect", days=30)
    store = ProfileStore(ledger, root=tmp_path, retention_days={"affect": 1})
    t0 = time.time()
    store.observe("p1", AFFECT_STATE, scope="affect", now=t0)
    assert store.get("p1", "affect", now=t0 + 2 * 86400) is None


def test_zero_retention_means_do_not_persist(tmp_path):
    """A deployment that does not want moral profiles on disk configures itself out of
    having them, rather than relying on everyone remembering."""
    ledger = ConsentLedger()
    ledger.grant("moral", days=10)
    store = ProfileStore(ledger, root=tmp_path, retention_days={"moral": 0})
    v = store.observe("p1", MORAL_STATE, scope="moral")
    assert "not retained" in v.note
    assert not list(tmp_path.glob("*.json"))
    assert store.get("p1", "moral") is None


def test_erase_reaches_the_version_log(tmp_path):
    """A redaction that only clears the current view while older versions sit in the log is
    theatre, and it is the usual way these systems leak."""
    store, _ = _store(tmp_path)
    for _ in range(3):
        store.observe("p1", CLINICAL_STATE, scope="clinical")
    assert store.erase_scope("p1", "clinical") == 3

    raw = json.loads((tmp_path / "p1.json").read_text(encoding="utf-8"))
    assert len(raw["versions"]) == 3          # the log still shows what was held
    for v in raw["versions"]:
        assert v["erased"] is True
        assert all(abs(x) < 1e-12 for x in v["state"]["values"])
        assert v["state"]["n_observations"] == 0
    assert store.get("p1", "clinical") is None


def test_erase_leaves_other_scopes_alone(tmp_path):
    store, _ = _store(tmp_path)
    store.observe("p1", CLINICAL_STATE, scope="clinical")
    for _ in range(3):
        store.observe("p1", AFFECT_STATE, scope="affect")
    store.erase_scope("p1", "clinical")
    assert store.get("p1", "affect") is not None
    assert store.get("p1", "clinical") is None


def test_erased_versions_are_not_returned_by_history(tmp_path):
    store, _ = _store(tmp_path)
    store.observe("p1", CLINICAL_STATE, scope="clinical")
    store.erase_scope("p1", "clinical")
    h = store.history("p1")
    assert all(v.erased for v in h)


def test_revoke_and_erase_is_the_operation_a_person_means(tmp_path):
    store, ledger = _store(tmp_path)
    store.observe("p1", CLINICAL_STATE, scope="clinical")
    store.observe("p1", MORAL_STATE, scope="moral")
    result = store.revoke_and_erase("p1", scope="moral")

    assert "moral" in result["revoked"]
    assert result["erased"] >= 1
    assert not ledger.has("moral")
    assert store.get("p1", "moral") is None
    assert ledger.has("clinical")          # only moral was revoked


def test_revoke_and_erase_clears_everything_when_no_scope_is_named(tmp_path):
    store, ledger = _store(tmp_path)
    store.observe("p1", CLINICAL_STATE, scope="clinical")
    store.observe("p1", MORAL_STATE, scope="moral")
    result = store.revoke_and_erase("p1")
    assert set(result["revoked"]) == set(SCOPES)
    assert ledger.live_scopes() == []
    assert store.get("p1", "clinical") is None
    assert store.get("p1", "moral") is None


def test_erase_all_leaves_no_readable_block(tmp_path):
    store, _ = _store(tmp_path)
    store.observe("p1", CLINICAL_STATE, scope="clinical")
    store.observe("p1", MORAL_STATE, scope="moral")
    store.erase_all("p1")
    for scope in ("affect", "clinical", "moral"):
        assert store.get("p1", scope) is None


def test_delete_removes_the_file(tmp_path):
    store, _ = _store(tmp_path)
    store.observe("p1", AFFECT_STATE, scope="affect")
    assert (tmp_path / "p1.json").exists()
    store.delete("p1")
    assert not (tmp_path / "p1.json").exists()


def test_purge_expired_drops_versions_but_keeps_tombstones(tmp_path):
    """The record that something was held and then destroyed outlives the data."""
    store, _ = _store(tmp_path, retention={"affect": 1})
    t0 = time.time()
    store.observe("p1", AFFECT_STATE, scope="affect", now=t0)
    store.observe("p1", CLINICAL_STATE, scope="clinical", now=t0)
    store.erase_scope("p1", "clinical")

    assert store.purge_expired("p1", now=t0 + 5 * 86400) == 1
    remaining = store.history("p1", now=t0 + 5 * 86400)
    assert all(v.erased for v in remaining)


# ------------------------------------------------------------------ on disk

def test_profile_survives_a_reopen(tmp_path):
    store, ledger = _store(tmp_path)
    for _ in range(4):
        store.observe("p1", AFFECT_STATE, scope="affect")

    reopened = ProfileStore(ledger, root=tmp_path)
    got = reopened.get("p1", "affect")
    assert got is not None
    assert got.n_observations == 4
    assert got.values[VAD_SLICE.start] == pytest.approx(-0.6)


def test_the_file_contains_no_identifying_string(tmp_path):
    pid = pseudonymise("roblox_user_5551234", "salt")
    store, _ = _store(tmp_path)
    for _ in range(3):
        store.observe(pid, AFFECT_STATE, scope="affect")
    text = (tmp_path / f"{pid}.json").read_text(encoding="utf-8")
    assert "roblox" not in text and "5551234" not in text


def test_write_is_atomic(tmp_path):
    """A profile store truncated by a crash mid-write is worse than useless: it looks like
    data loss but it is actually a consent record that no longer reflects what was held."""
    store, _ = _store(tmp_path)
    for _ in range(3):
        store.observe("p1", AFFECT_STATE, scope="affect")
    assert not list(tmp_path.glob("*.tmp"))


# ---------------------------------------------------------------- describe

def test_describe_is_safe_to_log(tmp_path):
    """No content, and nothing for a scope that is not currently granted."""
    store, ledger = _store(tmp_path, grants=("affect", "clinical"))
    for _ in range(3):
        store.observe("p1", CLINICAL_STATE, scope="clinical")
    d = store.describe("p1")
    assert d["moral"] is None                      # not granted
    assert d["clinical"]["trustworthy"] is True
    assert d["clinical"]["n"] == 3
    assert "values" not in json.dumps(d)


def test_describe_reports_a_thin_profile_as_untrustworthy(tmp_path):
    store, _ = _store(tmp_path)
    store.observe("p1", CLINICAL_STATE, scope="clinical")
    d = store.describe("p1")
    assert d["clinical"]["n"] == 1
    assert d["clinical"]["trustworthy"] is False


def test_describe_shortens_the_id(tmp_path):
    store, _ = _store(tmp_path)
    d = store.describe("0123456789abcdef0123456789abcdef")
    assert d["id"] == "01234567"


# ----------------------------------------------------------------- in memory

def test_store_works_without_a_directory():
    store = ProfileStore(ConsentLedger())
    store.ledger.grant("affect", days=5)
    for _ in range(3):
        store.observe("p1", AFFECT_STATE, scope="affect")
    assert store.get("p1", "affect") is not None
    assert store.erase_scope("p1", "affect") == 3
    assert store.get("p1", "affect") is None


def test_store_rejects_a_negative_retention(tmp_path):
    with pytest.raises(ValueError, match="must be >= 0"):
        ProfileStore(ConsentLedger(), root=tmp_path, retention_days={"moral": -1})


def test_store_rejects_an_unknown_retention_scope(tmp_path):
    with pytest.raises(ValueError, match="unknown scope"):
        ProfileStore(ConsentLedger(), root=tmp_path, retention_days={"psychology": 5})


def test_version_roundtrips_through_dict():
    st = ProfileState(values=AFFECT_STATE, n_observations=2, updated_at=1.0, scope="affect")
    v = ProfileVersion(index=0, at=1.0, state=st, note="hello")
    back = ProfileVersion.from_dict(json.loads(json.dumps(v.to_dict())))
    assert back.index == 0 and back.state.scope == "affect"
    assert back.state.n_observations == 2
    assert back.state.values == pytest.approx(AFFECT_STATE)
    assert back.note == "hello"
