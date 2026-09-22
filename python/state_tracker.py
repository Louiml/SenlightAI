"""Step 4 — Latent Emotional State Tracking + live sentiment feedback loop.

Maintains a compact, online emotional representation of a multi-turn dialogue so
a response policy can adapt to the user's evolving state in real time.

Design
------
Per conversation we keep a small ring buffer of per-turn affective signals (VAD
+ intent). From it we derive a **latent state**:

    * ``state``        — exponential moving average (EMA) of valence/arousal/
                         dominance across turns (tracks the "current" emotion)
    * ``velocity``     — EMA of the per-turn delta (trend: worsening / improving)
    * ``volatility``   — EMA of the absolute step size (arousal instability)
    * ``intent_count`` — running tally of intents (e.g. how often the user is in
                         crisis / seeking validation) for long-horizon tracking

The EMA weighting ``alpha`` controls how many turns carry weight (smaller = more
memory). Everything is deterministic and offline (built on ``affective.*``).

The ``LiveFeedbackLoop`` is the deployment-facing wrapper: you feed each new user
turn, it updates the latent state and returns a dict of **real-time sentiment
metrics** plus a suggested *response mode* (``de-escalate``, ``comfort``,
``validate``, ``guide``, ``casual``) that the model could use to condition its
next reply.
"""

from __future__ import annotations

from dataclasses import dataclass, field
from typing import Optional

from affective.emotions import analyze, AffectiveState
from affective.intent import classify_intent

V = 0
A = 1
D = 2


@dataclass
class LatentEmotionalState:
    """The evolving emotional representation of one conversation."""

    valence: float = 0.0
    arousal: float = 0.0
    dominance: float = 0.0
    v_velocity: float = 0.0        # trend of valence over turns
    a_velocity: float = 0.0        # trend of arousal over turns
    volatility: float = 0.0        # avg magnitude of per-turn affective shift
    turns: int = 0
    crisis_hits: int = 0
    validation_hits: int = 0
    venting_hits: int = 0
    advice_hits: int = 0
    disclosure_hits: int = 0
    intent_counts: dict[str, int] = field(default_factory=dict)

    def to_dict(self) -> dict:
        return {
            "valence": round(self.valence, 3),
            "arousal": round(self.arousal, 3),
            "dominance": round(self.dominance, 3),
            "v_velocity": round(self.v_velocity, 4),
            "a_velocity": round(self.a_velocity, 4),
            "volatility": round(self.volatility, 4),
            "turns": self.turns,
            "crisis_hits": self.crisis_hits,
            "validation_hits": self.validation_hits,
            "venting_hits": self.venting_hits,
            "advice_hits": self.advice_hits,
            "disclosure_hits": self.disclosure_hits,
            "intent_counts": dict(self.intent_counts),
        }


class EmotionalStateTracker:
    """Online tracker: buffer + EMA latent state per conversation."""

    def __init__(self, alpha: float = 0.4, max_buffer: int = 32,
                 crisis_hold: int = 4):
        if not (0.0 < alpha <= 1.0):
            raise ValueError("alpha must be in (0, 1]")
        self.alpha = alpha
        self.max_buffer = max_buffer
        self.crisis_hold = crisis_hold  # turns to keep de-escalation after last crisis
        self._since_crisis = 10**6       # turns since the last crisis signal
        self.buffer: list[dict] = []          # per-turn annotated signals
        self.state = LatentEmotionalState()

    # -- internal ----------------------------------------------------------
    def _prev(self) -> AffectiveState | None:
        if not self.buffer:
            return None
        last = self.buffer[-1]
        return AffectiveState(
            text=last["text"], vad=(last["valence"], last["arousal"], last["dominance"]),
            emotion=last["emotion"], plutchik=last["plutchik"],
        )

    @staticmethod
    def _exp(prev: float, new: float, alpha: float) -> float:
        return alpha * new + (1.0 - alpha) * prev

    # -- API ----------------------------------------------------------------
    def observe(self, user_text: str) -> dict:
        """Feed a user turn; update the latent state; return live metrics."""
        spoken = user_text or ""
        aff = analyze(spoken)
        intent_rec = classify_intent(aff)
        v, a, d = aff.vad

        prev = self._prev()
        if prev is not None and prev.vad != (0.0, 0.0, 0.0):
            pv, pa, pd = prev.vad
            self.state.v_velocity = self._exp(self.state.v_velocity, v - pv, self.alpha)
            self.state.a_velocity = self._exp(self.state.a_velocity, a - pa, self.alpha)
            self.state.volatility = self._exp(
                self.state.volatility,
                (abs(v - pv) + abs(a - pa)) / 2.0,
                self.alpha,
            )
        else:
            self.state.v_velocity = self._exp(self.state.v_velocity, 0.0, self.alpha)
            self.state.a_velocity = self._exp(self.state.a_velocity, 0.0, self.alpha)

        # EMA of the absolute state.
        self.state.valence = self._exp(self.state.valence, v, self.alpha)
        self.state.arousal = self._exp(self.state.arousal, a, self.alpha)
        self.state.dominance = self._exp(self.state.dominance, d, self.alpha)
        self.state.turns += 1

        # intent tallies
        it = intent_rec.intent
        self.state.intent_counts[it] = self.state.intent_counts.get(it, 0) + 1
        if it == "Crisis":
            self.state.crisis_hits += 1
        elif it == "Seeking_Validation":
            self.state.validation_hits += 1
        elif it == "Venting":
            self.state.venting_hits += 1
        elif it == "Advice_Seeking":
            self.state.advice_hits += 1
        elif it == "Disclosure":
            self.state.disclosure_hits += 1

        # Ring buffer.
        self.buffer.append({
            "text": spoken, "valence": v, "arousal": a, "dominance": d,
            "intent": it, "emotion": aff.emotion, "plutchik": aff.plutchik,
        })
        # Track how many consecutive turns have passed with no crisis signal.
        if it == "Crisis":
            self._since_crisis = 0
        else:
            self._since_crisis += 1
        if len(self.buffer) > self.max_buffer:
            self.buffer.pop(0)

        return self.metrics()

    def metrics(self) -> dict:
        """Real-time sentiment metrics for response conditioning."""
        s = self.state
        # Recency: crisis stays active for `crisis_hold` turns after the signal,
        # then decays so the loop responds to recovery in real time.
        recent_crisis = self._since_crisis <= self.crisis_hold
        recent_validation = any(b.get("intent") == "Seeking_Validation" for b in self.buffer)

        # Derive a suggested response mode from the latent state + trend.
        if recent_crisis:
            mode = "de-escalate"
        elif s.valence < -0.3 and s.v_velocity < -0.02:
            mode = "comfort"            # worsening / low -> comfort, gentle support
        elif s.valence < -0.2 and recent_validation:
            mode = "validate"           # low self-worth -> affirmation
        elif s.intent_counts.get("Advice_Seeking", 0) > 0 and s.valence <= -0.1:
            mode = "guide"              # low + asking what to do -> offer options
        elif s.intent_counts.get("Venting", 0) > 0 and not recent_crisis:
            mode = "comfort"            # releasing emotion -> reflect, de-escalate
        else:
            mode = "casual"

        # Risk decays as the recent state stabilises toward neutral/positive.
        if recent_crisis:
            risk = "high"
        elif s.valence < -0.5 or s.volatility > 0.4:
            risk = "elevated"
        elif s.valence < -0.2:
            risk = "moderate"
        else:
            risk = "low"

        out = self.state.to_dict()
        out["response_mode"] = mode
        out["trend"] = (
            "improving" if s.v_velocity > 0.03 else
            ("worsening" if s.v_velocity < -0.03 else "stable")
        )
        out["risk"] = risk
        return out

    def reset(self) -> None:
        self.buffer.clear()
        self._since_crisis = 10**6
        self.state = LatentEmotionalState()


class LiveFeedbackLoop:
    """Deployment-facing sentiment feedback loop over a tracked conversation."""

    def __init__(self, alpha: float = 0.4):
        self.tracker = EmotionalStateTracker(alpha=alpha)

    def feed(self, user_text: str) -> dict:
        """Typical deployment hook: receive user message, return live metrics."""
        return self.tracker.observe(user_text)

    def conversation_state(self) -> dict:
        return self.tracker.metrics()

    def response_hint(self) -> str:
        """Human-readable guidance derived from the current latent state."""
        m = self.tracker.metrics()
        if m["risk"] == "high":
            return ("This is a crisis-level state: respond with immediate, "
                    "non-judgmental support and encourage reaching a crisis "
                    "helpline; do not diagnose or minimize.")
        if m["response_mode"] == "validate":
            return "Low self-worth / seeking validation: affirm the user's worth and agency explicitly."
        if m["response_mode"] == "guide":
            return "User wants options: offer gentle, concrete next steps, inviting choice."
        if m["response_mode"] == "comfort":
            return "User is venting or low: reflect the feeling, de-escalate arousal, stay calm."
        return "Neutral state: respond warmly and naturally."


# ---------------------------------------------------------------------------
# Self-test
# ---------------------------------------------------------------------------
if __name__ == "__main__":
    turns = [
        "I'm feeling a bit off today, not sure why.",
        "Actually I feel anxious, my heart is racing and I can't focus.",
        "I keep thinking everyone hates me, I feel like such a failure.",
        "I honestly don't know what to do, it's all too much.",
        "I've been thinking about ending it, I just want the pain to stop.",
        "I called a helpline and I'm a little calmer now.",
        "Thanks for listening, I feel a bit more understood.",
        "I made a plan with my therapist and I feel safer now.",
        "I had a good day today, I even went for a walk.",
        "I'm feeling a lot better, thank you for being here.",
    ]
    loop = LiveFeedbackLoop(alpha=0.4)
    for i, t in enumerate(turns, 1):
        m = loop.feed(t)
        print(f"turn {i}: mode={m['response_mode']:<14} val={m['valence']:+.2f} "
              f"arou={m['arousal']:+.2f} trend={m['trend']:<9} risk={m['risk']}")
    print("\nresponse hint:", loop.response_hint())