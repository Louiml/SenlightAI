"""Senlight Affective AI — DESKTOP application (Aurora Glass design).

Embeds the redesigned **"Aurora Glass"** frontend (`web/aurora/`) — the v3
design system from `website/DESIGN.md` (deep-space canvas, warm orange/amber
aurora, frosted glass, fixed glass sidebar rail) — in a native OS window via
pywebview. The frontend talks directly to Python through `pywebview.api`
(no HTTP server), bridging emotion detection and the Rust/Candle engine.

Run:
    python desktop_app.py
Overrides:
    SENLIGHT_BIN / SENLIGHT_WEIGHTS / SENLIGHT_TOKENIZER
"""

from __future__ import annotations

import os
import subprocess
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))

import config as C                      # noqa: E402
from affective.emotions import analyze  # noqa: E402


def resolve_binary() -> Path:
    explicit = os.environ.get("SENLIGHT_BIN")
    if explicit and Path(explicit).exists():
        return Path(explicit)
    project = Path(__file__).resolve().parent.parent
    for c in [
        project / "inference_rs" / "target" / "release" / "senlight_inference.exe",
        project / "inference_rs" / "target" / "release" / "senlight_inference",
        project / "inference_rs" / "target" / "debug" / "senlight_inference.exe",
    ]:
        if c.exists():
            return c
    raise FileNotFoundError("Rust inference engine not built (cargo build --release)")


PROJECT = Path(__file__).resolve().parent.parent
BIN = resolve_binary()
WEIGHTS = os.environ.get(
    "SENLIGHT_WEIGHTS", str(C.ARTIFACTS_DIR / "senlight_300m_he2.safetensors")
)
TOKENIZER = os.environ.get("SENLIGHT_TOKENIZER", str(C.TOKENIZER_PATH))
# Aurora Glass frontend (DESIGN.md v3)
WEB_INDEX = Path(__file__).resolve().parent / "web" / "aurora" / "index.html"

EMPATHY_PREAMBLE = (
    "You are Senlight, a warm, empathetic listener. Validate the user's "
    "feelings, avoid judgment, de-escalate, and gently offer support.\n\n"
)


class Api:
    """JS<->Python bridge exposed as ``window.pywebview.api``."""

    def detect_emotion(self, text: str) -> dict:
        st = analyze(text or "")
        return {
            "emotion": st.emotion,
            "plutchik": st.plutchik,
            "vad": list(st.vad),
            "confidence": round(st.confidence, 3),
        }

    def chat(self, prompt: str, max_tokens: int, temperature: float,
             top_k: int, context: str) -> dict:
        user = (prompt or "").strip()
        if not user:
            return {"error": "empty prompt", "reply": "", "context": context,
                    "emotion": "Neutral", "plutchik": "Neutral", "vad": [0, 0, 0],
                    "confidence": 0}
        st = analyze(user)
        hint = (
            f"[User is feeling {st.plutchik.lower()}; "
            f"VAD=({st.vad[0]:+.2f},{st.vad[1]:+.2f},{st.vad[2]:+.2f})]\n"
        )
        full = (context + " " + hint + "\n" + user).strip()
        try:
            reply = self._run_engine(full, int(max_tokens), float(temperature), int(top_k))
        except Exception as exc:  # noqa: BLE001
            reply = f"[engine error] {exc}"
        return {
            "reply": reply,
            "emotion": st.emotion,
            "plutchik": st.plutchik,
            "vad": list(st.vad),
            "confidence": round(st.confidence, 3),
            "context": full,
        }

    @staticmethod
    def _run_engine(prompt: str, max_new: int, temp: float, top_k: int) -> str:
        cmd = [str(BIN), prompt, str(max_new), str(temp), str(top_k)]
        env = dict(os.environ, SENLIGHT_WEIGHTS=WEIGHTS, SENLIGHT_TOKENIZER=TOKENIZER)
        proc = subprocess.run(
            cmd, capture_output=True, text=True, encoding="utf-8",
            errors="replace", timeout=300, env=env,
            cwd=str(PROJECT / "inference_rs"),
        )
        out = proc.stdout.strip()
        if proc.returncode != 0:
            out = (out + "\n" + proc.stderr).strip() or f"[engine exit {proc.returncode}]"
        marker = "[infer] >>> "
        if marker in out:
            out = out.split(marker, 1)[1].strip()
        return out


def main() -> None:
    import webview

    if not WEB_INDEX.exists():
        raise FileNotFoundError(f"Aurora Glass frontend not found: {WEB_INDEX}")

    window = webview.create_window(
        "Senlight — Aurora Glass",
        url=str(WEB_INDEX.resolve()),
        width=1120,
        height=760,
        min_size=(760, 540),
        js_api=Api(),
        background_color="#0c0805",  # warm near-black canvas
    )
    _ = window
    print(f"[desktop] frontend = {WEB_INDEX.name} (Aurora Glass)")
    print(f"[desktop] weights   = {WEIGHTS}")
    print(f"[desktop] engine    = {BIN}")
    webview.start()


if __name__ == "__main__":
    main()
