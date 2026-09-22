"""Step 1 — Hugging Face dataset ingestion + affective annotation.

Loads the empathy / psychology datasets the pipeline specifies and normalizes
every row into the canonical Senlight SFT JSONL row::

    { instruction, input, output,
      valence, arousal, dominance, intent, emotion, plutchik,
      source }

Annotation (valence / arousal / intent) is computed by the embedded affective
stack in ``affective.emotions``/``affective.intent`` so the whole pipeline shares
one source of truth (deterministic, offline).

Datasets consumed (canonical current IDs):
  * SFT / affective grounding:
      - ``facebook/empathetic_dialogues``  (context emotion -> utterance)
      - ``Amod/mental_health_counseling_conversations``  (psych domain Q/A)
      - ``google-research-datasets/go_emotions``  (fine-grained emotion -> intent)
  * RLEF / reward modeling:
      - ``Anthropic/hh-rlhf``               (chosen/rejected helpful-countable pairs)
      - ``SetFit/tweet_eval_sentiment``     (sentiment-scaled support ranking)

Gated datasets that need a token (``Amod/...``, ``tweet_eval``) fall back to a
bundled synthetic generator when no ``HF_TOKEN`` is available, so the intake
step is always runnable for validation. Every load is ``max_rows``-limited so a
validation run never pulls the whole (potentially huge) corpus.
"""

from __future__ import annotations

import json
import os
import sys
from pathlib import Path
from typing import Callable, Iterable

sys.path.insert(0, str(Path(__file__).resolve().parent))

import config as C  # noqa: E402
from affective.emotions import analyze  # noqa: E402
from affective.intent import classify_intent  # noqa: E402

HERE = Path(__file__).resolve().parent

# Canonical row keys.
ROW_KEYS = (
    "instruction", "input", "output",
    "valence", "arousal", "dominance", "intent", "emotion", "plutchik",
    "source",
)


# ---------------------------------------------------------------------------
# Emotion taxonomies (mapping dataset emotion strings -> VAD/intent anchors)
# ---------------------------------------------------------------------------
# empathetic_dialogues context emotions (subset of the 32 used).
_EMP_VA: dict[str, tuple[float, float]] = {
    "joyful": (0.9, 0.7), "happy": (0.9, 0.6), "excited": (0.8, 0.9),
    "amazed": (0.7, 0.8), "confident": (0.6, 0.5), "proud": (0.7, 0.5),
    "grateful": (0.8, -0.3), "caring": (0.7, 0.2), "trusting": (0.6, 0.3),
    "content": (0.7, -0.3), "hopeful": (0.7, 0.4), "sentimental": (0.3, -0.3),
    "nostalgic": (0.1, -0.4), "anticipating": (0.3, 0.5), "curious": (0.5, 0.5),
    "surprised": (0.2, 0.9), "anxious": (-0.4, 0.7), "afraid": (-0.5, 0.6),
    "annoyed": (-0.5, 0.4), "angry": (-0.7, 0.8), "furious": (-0.9, 0.9),
    "terrified": (-0.8, 0.9), "apprehensive": (-0.4, 0.5), "disgusted": (-0.6, -0.1),
    "sad": (-0.8, -0.5), "lonely": (-0.7, -0.6), "devastated": (-0.9, -0.4),
    "guilty": (-0.6, 0.4), "ashamed": (-0.6, 0.4), "jealous": (-0.4, 0.5),
    "embarrassed": (-0.5, 0.3), "disappointed": (-0.6, -0.3),
}
_EMP_INTENT: dict[str, str] = {
    "anxious": "Venting", "afraid": "Venting", "annoyed": "Venting",
    "angry": "Venting", "furious": "Venting", "terrified": "Venting",
    "apprehensive": "Venting", "disgusted": "Venting", "guilty": "Disclosure",
    "ashamed": "Disclosure", "sad": "Disclosure", "lonely": "Disclosure",
    "devastated": "Disclosure", "embarrassed": "Disclosure",
    "disappointed": "Disclosure", "jealous": "Disclosure",
}

# go_emotions: 27 fine-grained + neutral -> intent anchor. High-arousal negative
# emotions vent; relation-self emotions seek validation; neutral is casual.
_GOE_INTENT: dict[str, str] = {
    "admiration": "Casual", "amusement": "Casual", "anger": "Venting",
    "annoyance": "Venting", "approval": "Casual", "caring": "Disclosure",
    "confusion": "Advice_Seeking", "curiosity": "Advice_Seeking", "desire": "Casual",
    "disappointment": "Disclosure", "disapproval": "Venting", "disgust": "Venting",
    "embarrassment": "Disclosure", "excitement": "Casual", "fear": "Venting",
    "gratitude": "Casual", "grief": "Disclosure", "joy": "Casual",
    "love": "Casual", "nervousness": "Venting", "optimism": "Casual",
    "pride": "Casual", "realization": "Casual", "relief": "Casual",
    "remorse": "Disclosure", "sadness": "Disclosure", "surprise": "Casual",
    "neutral": "Casual",
}

# tweet_eval sentiment -> support-relevant valence/arousal.
_TWEET_SENT: dict[str, tuple[float, float]] = {
    "positive": (0.9, 0.4), "neutral": (0.0, 0.0), "negative": (-0.8, 0.3),
}


# ---------------------------------------------------------------------------
# Row factory
# ---------------------------------------------------------------------------
def _row(instruction: str, user_input: str, output: str, source: str) -> dict:
    """Build an annotated canonical row from raw text."""
    probe = (user_input or instruction or "").strip()
    st = analyze(probe)
    it = classify_intent(st)
    v, a, d = st.vad
    return {
        "instruction": (instruction or "").strip(),
        "input": (user_input or "").strip(),
        "output": (output or "").strip(),
        "valence": round(v, 3), "arousal": round(a, 3), "dominance": round(d, 3),
        "intent": it.intent, "emotion": st.emotion, "plutchik": st.plutchik,
        "source": source,
    }


def _va_of_emotion(emotion: str) -> tuple[float, float]:
    key = (emotion or "").lower().strip()
    return _EMP_VA.get(key, (0.0, 0.0))


# ---------------------------------------------------------------------------
# Normalizers (dataset -> iterable of canonical rows)
# ---------------------------------------------------------------------------
def _norm_empathetic(batch: Iterable[dict]) -> Iterable[dict]:
    for r in batch:
        instr = "The user just shared that they feel {e}. Respond warmly and "
        "empathically, listening and validating without diagnosing."
        e = (r.get("emotion") or "neutral").lower()
        utterance = (r.get("utterance") or "").strip()
        if not utterance:
            continue
        instruction = f"The user just shared that they feel {e}. Respond warmly and empathically, listening and validating without diagnosing."
        # Ground the affective probe with the context emotion so annotation is
        # stable even for short utterances.
        st = analyze(f"I feel {e}. {utterance}")
        it = classify_intent(st)
        v, a, d = st.vad
        yield {
            "instruction": instruction, "input": utterance, "output": "",
            "valence": round(v, 3), "arousal": round(a, 3), "dominance": round(d, 3),
            "intent": it.intent, "emotion": st.emotion, "plutchik": st.plutchik,
            "source": "empathetic_dialogues",
        }


def _norm_mental_health(batch: Iterable[dict]) -> Iterable[dict]:
    for r in batch:
        q = (r.get("question") or r.get("input") or r.get("prompt") or "").strip()
        a = (r.get("answer") or r.get("response") or r.get("output") or "").strip()
        if not q:
            continue
        yield _row(
            "As a compassionate, non-judgmental supporter, respond warmly and "
            "offer gentle guidance to this person (no medical diagnosis).",
            q, a, "mental_health",
        )


_GOE_COLUMNS = [
    "admiration", "amusement", "anger", "annoyance", "approval", "caring",
    "confusion", "curiosity", "desire", "disappointment", "disapproval",
    "disgust", "embarrassment", "excitement", "fear", "gratitude", "grief",
    "joy", "love", "nervousness", "optimism", "pride", "realization", "relief",
    "remorse", "sadness", "surprise", "neutral",
]


def _norm_goemotions(batch: Iterable[dict], columns: list[str] = _GOE_COLUMNS) -> Iterable[dict]:
    for r in batch:
        text = (r.get("text") or "").strip()
        if not text:
            continue
        # go_emotions "raw" rows carry one boolean column per emotion.
        emo = "neutral"
        for col in columns:
            if r.get(col) in (1, True):
                emo = col
                break
        intent = _GOE_INTENT.get(emo, "Casual")
        row = _row(
            "Reflect this person's message with empathy and warmth "
            "(no medical diagnosis).",
            text, "", "go_emotions",
        )
        row["intent"] = intent
        row["emotion"] = emo
        yield row


# hh-rlhf: preference pairs -> RLEF pair rows (chosen/rejected completions).
def _norm_hh_rlhf(batch: Iterable[dict]) -> Iterable[dict]:
    for r in batch:
        chosen = (r.get("chosen") or "").strip()
        rejected = (r.get("rejected") or "").strip()
        if not chosen or not rejected:
            continue
        # Prompt = everything before the final "Assistant:" line.
        md = chosen.rsplit("Assistant:", 1)
        prompt = md[0].rsplit("Human:", 1)[-1].strip() if md else ""
        chosen_resp = md[1].strip() if len(md) > 1 else chosen
        rej_md = rejected.rsplit("Assistant:", 1)
        rejected_resp = rej_md[1].strip() if len(rej_md) > 1 else rejected
        yield {
            "prompt": prompt or "The user is in need of empathetic support.",
            "chosen": chosen_resp,
            "rejected": rejected_resp,
            "source": "hh_rlhf",
        }


_TWEET_SENT_IDX: dict[int, tuple[str, float, float]] = {
    0: ("negative", -0.8, 0.3),  # tweet_eval encodes 0=negative
    1: ("neutral", 0.0, 0.0),
    2: ("positive", 0.9, 0.4),
}


def _norm_tweet_sentiment(batch: Iterable[dict]) -> Iterable[dict]:
    for r in batch:
        text = (r.get("text") or "").strip()
        if not text:
            continue
        raw = r.get("label")
        if isinstance(raw, int) and raw in _TWEET_SENT_IDX:
            label, v, a = _TWEET_SENT_IDX[raw]
        else:
            key = (str(raw) if raw is not None else "").lower().strip()
            v, a = _TWEET_SENT.get(key, (0.0, 0.0))
            label = key
        yield {
            "prompt": text,
            "text": text,
            "sentiment": label,
            "valence": v, "arousal": a,
            "source": "tweet_eval",
        }


# ---------------------------------------------------------------------------
# Synthetic fallbacks (used only when a dataset is gated / offline)
# ---------------------------------------------------------------------------
def _synth_sft() -> Iterable[dict]:
    pool = [
        ("I have been feeling really anxious about everything lately.",
         "I'm sorry you're carrying that weight. Want to share what's been most on your mind?"),
        ("I feel like nobody at work appreciates me.",
         "That sounds really isolating. Your effort matters, even if it isn't acknowledged."),
        ("What should I do, I keep fighting with my partner.",
         "Conflicts can be draining. Maybe slow things down and share what you each need--would that help?"),
        ("I have felt so lonely since I moved here.",
         "Loneliness is heavy. Building connection takes time; I'm glad you told me."),
        ("I just feel so sad and empty lately.",
         "That emptiness sounds painful. You're not alone in this; let's take it gently."),
        ("My therapist suggested I talk things through, here I am.",
         "That's a brave step. Talk to me at your own pace."),
    ]
    for q, a in pool:
        yield _row(
            "As a compassionate, non-judgmental supporter, respond warmly and "
            "offer gentle guidance (no medical diagnosis).",
            q, a, "synthetic",
        )


def _synth_rlhf() -> Iterable[dict]:
    return iter([
        {
            "prompt": "The user says they feel overwhelmed at work.",
            "chosen": "I can hear how much pressure you're under. Let's take it one step at a time—what's the most urgent thing?",
            "rejected": "Work stress is common, you'll be fine. Just push through.",
            "source": "synthetic",
        },
        {
            "prompt": "The user says they feel like a failure.",
            "chosen": "That's a really hard place to be. Want to talk about what's making you feel that way?",
            "rejected": "Everyone fails sometimes, don't worry about it.",
            "source": "synthetic",
        },
    ])


def _synth_sentiment() -> Iterable[dict]:
    return iter([
        {"prompt": "I finally got the job I wanted!", "text": "I finally got the job I wanted!", "sentiment": "positive", "valence": 0.9, "arousal": 0.5, "source": "synthetic"},
        {"prompt": "Today was just an ordinary day.", "text": "Today was just an ordinary day.", "sentiment": "neutral", "valence": 0.0, "arousal": 0.0, "source": "synthetic"},
        {"prompt": "I feel so overwhelmed and stuck right now.", "text": "I feel so overwhelmed and stuck right now.", "sentiment": "negative", "valence": -0.7, "arousal": 0.3, "source": "synthetic"},
    ])


# ---------------------------------------------------------------------------
# Dataset table + loader
# ---------------------------------------------------------------------------
DATASET_SPECS: dict[str, dict] = {
    "empathetic_dialogues": {
        "id": "facebook/empathetic_dialogues", "config": "default", "split": "train",
        "normalizer": "empathetic", "rlf": False,
    },
    "mental_health": {
        "id": "Amod/mental_health_counseling_conversations", "config": "default",
        "split": "train", "normalizer": "mental_health", "rlf": False, "gated": True,
    },
    "go_emotions": {
        "id": "google-research-datasets/go_emotions", "config": "raw", "split": "train",
        "normalizer": "goemotions", "rlf": False,
    },
    "hh_rlhf": {
        "id": "Anthropic/hh-rlhf", "config": "default", "split": "train",
        "normalizer": "hh_rlhf", "rlf": True,
    },
    "tweet_eval": {
        "id": "tweet_eval", "config": "sentiment", "split": "train",
        "normalizer": "tweet_sentiment", "rlf": True, "gated": True,
    },
}

_NORMALIZERS: dict[str, Callable] = {
    "empathetic": _norm_empathetic,
    "mental_health": _norm_mental_health,
    "goemotions": lambda batch: _norm_goemotions(batch, _GOE_COLUMNS),
    "hh_rlhf": _norm_hh_rlhf,
    "tweet_sentiment": _norm_tweet_sentiment,
}
_SYNTH: dict[str, Callable] = {
    "mental_health": _synth_sft, "tweet_eval": _synth_sentiment,
}


def _auth_token() -> str | None:
    return os.environ.get("HF_TOKEN") or os.environ.get("HUGGINGFACE_TOKEN")


def load_dataset_rows(dataset_key: str, max_rows: int = 500) -> list[dict]:
    """Load & normalize a dataset to canonical rows; synthetic fallback if gated."""
    spec = DATASET_SPECS[dataset_key]
    try:
        from datasets import load_dataset
        kwargs: dict = {}
        token = _auth_token()
        if spec.get("gated"):
            if not token:
                _warn_gated(spec["id"])
                return _take(_SYNTH[dataset_key](), max_rows)
            kwargs["token"] = token
        ds = load_dataset(
            spec["id"], spec["config"], split=spec["split"], **kwargs,
            trust_remote_code=True,
        )
        normalizer = _NORMALIZERS[spec["normalizer"]]
        rows = _take(normalizer(ds), max_rows)
        print(f"[hf] {dataset_key}: loaded {len(rows)} rows from {spec['id']}")
        return rows
    except Exception as exc:  # noqa: BLE001
        print(f"[hf] {dataset_key}: unavailable ({exc.__class__.__name__}: {exc})");
        fallback = _SYNTH.get(dataset_key)
        if fallback is None:
            # Datasets without a dedicated synthetic set use the generic SFT pool.
            fallback = _synth_sft if not spec.get("rlf") else _synth_rlhf
        return _take(fallback(), max_rows)


def _take(gen: Iterable[dict], n: int) -> list[dict]:
    out: list[dict] = []
    for r in gen:
        if len(out) >= n:
            break
        # Keep an SFT row if it has an output, or an RLF row if it has a prompt.
        if r and (r.get("output") is not None or r.get("prompt") is not None):
            out.append(r)
    return out


def _warn_gated(ds_id: str) -> None:
    print(f"[hf] {ds_id} is gated and no HF_TOKEN is set; using synthetic fallback.")


# ---------------------------------------------------------------------------
# Persist to an SFT jsonl (merge step-1 output).
# ---------------------------------------------------------------------------
def dump_sft(rows: list[dict], path: Path, rlf_pairs: list[dict] | None = None) -> int:
    """Write non-RLF rows as canonical SFT jsonl lines; RLF pairs to a second file."""
    path.parent.mkdir(parents=True, exist_ok=True)
    n = 0
    with open(path, "w", encoding="utf-8") as fh:
        for r in rows:
            if r.get("rlf"):
                continue
            line = {k: r.get(k, "") for k in ROW_KEYS}
            fh.write(json.dumps(line, ensure_ascii=False) + "\n")
            n += 1
    if rlf_pairs:
        rp = path.with_name(path.stem + "_rlf.jsonl")
        with open(rp, "w", encoding="utf-8") as fh:
            for p in rlf_pairs:
                fh.write(json.dumps(p, ensure_ascii=False) + "\n")
        print(f"[hf] wrote {len(rlf_pairs)} RLEF preference pairs -> {rp.name}")
    return n


# ---------------------------------------------------------------------------
# CLI
# ---------------------------------------------------------------------------
def main() -> None:
    import argparse
    ap = argparse.ArgumentParser(description="Step 1: HF dataset intake + annotation")
    ap.add_argument("--sets", nargs="*", default=[
        "empathetic_dialogues", "mental_health", "go_emotions", "hh_rlhf", "tweet_eval"])
    ap.add_argument("--max-rows", type=int, default=500)
    ap.add_argument("--out", default=str(C.ARTIFACTS_DIR / "affective_intake.jsonl"))
    args = ap.parse_args()

    sft: list[dict] = []
    rlf: list[dict] = []
    for s in args.sets:
        for r in load_dataset_rows(s, args.max_rows):
            if DATASET_SPECS[s].get("rlf"):
                r["rlf"] = True
                rlf.append(r)
            else:
                sft.append(r)
    n = dump_sft(sft, Path(args.out), rlf)
    print(f"[hf] wrote {n} SFT rows + {len(rlf)} RLF pairs -> {Path(args.out).name} ")


if __name__ == "__main__":
    main()