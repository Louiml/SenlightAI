"""Affective AI — modern web GUI backend.

A zero-dependency stdlib HTTP server that serves the modern HTML/CSS/JS
frontend and bridges chat + affective state detection to the Rust/Candle
inference engine and the embedded VAD/Plutchik emotion model.

Endpoints:
    GET  /                       modern GUI (index.html)
    GET  /static/*               css / js assets
    POST /api/chat               {prompt, max_tokens, temperature, top_k, context}
                                 -> {reply, emotion, plutchik, vad:[V,A,D]}
    GET  /api/emotion?text=...   live emotion detection -> {emotion, plutchik, vad}

Run:
    python web_gui.py             # http://localhost:8787
"""

from __future__ import annotations

import json
import os
import subprocess
import threading
from http.server import BaseHTTPRequestHandler, ThreadingHTTPServer
from pathlib import Path

import sys

sys.path.insert(0, str(Path(__file__).resolve().parent))
import config as C  # noqa: E402
from affective.emotions import analyze  # noqa: E402

# ---------------------------------------------------------------------------
# Config
# ---------------------------------------------------------------------------
ROOT = Path(__file__).resolve().parent          # python/
WEB_DIR = ROOT / "web" / "aurora"                # python/web/aurora (Aurora Glass design)
PROJECT = ROOT.parent


def resolve_binary() -> Path:
    explicit = os.environ.get("SENLIGHT_BIN")
    if explicit and Path(explicit).exists():
        return Path(explicit)
    for c in [
        PROJECT / "inference_rs" / "target" / "release" / "senlight_inference.exe",
        PROJECT / "inference_rs" / "target" / "release" / "senlight_inference",
        PROJECT / "inference_rs" / "target" / "debug" / "senlight_inference.exe",
    ]:
        if c.exists():
            return c
    raise FileNotFoundError("Rust inference engine not built (cargo build --release)")


BIN = resolve_binary()
WEIGHTS = os.environ.get(
    "SENLIGHT_WEIGHTS", str(C.ARTIFACTS_DIR / "senlight_300m_he2.safetensors")
)
TOKENIZER = os.environ.get("SENLIGHT_TOKENIZER", str(C.TOKENIZER_PATH))

HOST = os.environ.get("SENLIGHT_HOST", "127.0.0.1")
PORT = int(os.environ.get("SENLIGHT_PORT", "8787"))

EMPATHY_PREAMBLE = (
    "You are Senlight, a warm, empathetic listener. Validate the user's "
    "feelings, avoid judgment, de-escalate, and gently offer support.\n\n"
)

# ---------------------------------------------------------------------------
# Affective helpers
# ---------------------------------------------------------------------------
def emotional_context(user_text: str) -> dict:
    st = analyze(user_text)
    hint = (
        f"[User is feeling {st.plutchik.lower()}; "
        f"VAD=({st.vad[0]:+.2f},{st.vad[1]:+.2f},{st.vad[2]:+.2f})]\n"
    )
    return {
        "hint": hint,
        "emotion": st.emotion,
        "plutchik": st.plutchik,
        "vad": list(st.vad),
        "confidence": round(st.confidence, 3),
    }


def run_engine(prompt: str, max_new: int, temp: float, top_k: int) -> str:
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


# ---------------------------------------------------------------------------
# HTTP server
# ---------------------------------------------------------------------------
class Handler(BaseHTTPRequestHandler):
    def log_message(self, *_):
        pass  # quiet

    # -- static / index ------------------------------------------------
    def do_GET(self):
        path = self.path.split("?")[0]
        if path in ("/", "/index.html"):
            self._send_file(WEB_DIR / "index.html", "text/html")
        elif path.startswith("/static/"):
            name = path[len("/static/"):]
            if not name or ".." in name:
                self._send(400, "bad path")
                return
            f = (WEB_DIR / "static" / name).resolve()
            if not f.is_file():
                self._send(404, "not found")
                return
            ctype = {
                ".css": "text/css", ".js": "text/javascript", ".html": "text/html",
                ".svg": "image/svg+xml", ".png": "image/png", ".woff2":
                "font/woff2",
            }.get(f.suffix.lower(), "application/octet-stream")
            self._send_file(f, ctype)
        elif path == "/api/emotion":
            text = self._q("text", "")
            self._send_json(emotional_context(text))
        else:
            self._send(404, "not found")

    # -- chat -----------------------------------------------------------
    def do_POST(self):
        if self.path != "/api/chat":
            self._send(404, "not found")
            return
        try:
            n = int(self.headers.get("Content-Length", 0))
            body = json.loads(self.rfile.read(n) or b"{}")
        except Exception:
            self._send_json({"error": "bad JSON"})
            return

        user = (body.get("prompt") or "").strip()
        max_new = int(body.get("max_tokens", 90))
        temp = float(body.get("temperature", 0.75))
        top_k = int(body.get("top_k", 40))
        prior = body.get("context") or ""

        if not user:
            self._send_json({"error": "empty prompt"})
            return

        aff = emotional_context(user)
        context = (prior + " " + aff["hint"] + "\n" + user).strip()
        train_hint = "" if prior else ""  # preamble handled client-side
        _ = train_hint

        try:
            reply = run_engine(context, max_new, temp, top_k)
        except Exception as exc:  # noqa: BLE001
            reply = f"[engine error] {exc}"

        self._send_json({
            "reply": reply,
            "emotion": aff["emotion"],
            "plutchik": aff["plutchik"],
            "vad": aff["vad"],
            "confidence": aff["confidence"],
            "context": context,
        })

    # -- helpers --------------------------------------------------------
    def _q(self, key, default):
        q = self.path.split("?", 1)[1] if "?" in self.path else ""
        for part in q.split("&"):
            if part.startswith(key + "="):
                return part[len(key) + 1:].replace("+", " ")
        return default

    def _send(self, code, text):
        self.send_response(code)
        self.send_header("Content-Type", "text/plain; charset=utf-8")
        self.send_header("Content-Length", str(len(text.encode())))
        self.end_headers()
        self.wfile.write(text.encode())

    def _send_json(self, obj):
        data = json.dumps(obj).encode("utf-8")
        self.send_response(200)
        self.send_header("Content-Type", "application/json; charset=utf-8")
        self.send_header("Content-Length", str(len(data)))
        self.send_header("Cache-Control", "no-store")
        self.end_headers()
        self.wfile.write(data)

    def _send_file(self, path: Path, ctype: str):
        try:
            data = path.read_bytes()
        except OSError:
            self._send(404, "not found")
            return
        self.send_response(200)
        self.send_header("Content-Type", ctype)
        self.send_header("Content-Length", str(len(data)))
        self.send_header("Cache-Control", "no-store")
        self.end_headers()
        self.wfile.write(data)


def main() -> None:
    srv = ThreadingHTTPServer((HOST, PORT), Handler)
    print(f"[affective-web] serving GUI at http://{HOST}:{PORT}")
    print(f"[affective-web] weights = {WEIGHTS}")
    srv.serve_forever()


if __name__ == "__main__":
    main()
