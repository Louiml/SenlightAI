"""Senlight Coder AI — tkinter chat GUI.

A desktop chat window that talks to the Rust/Candle inference engine. Because
the model is a raw code-completion LM, this GUI frames each session as: you
type/paste code or a prompt; Senlight completes it; the completion is folded
back into the running context so later turns build on prior ones.

The engine is invoked as a subprocess (one `senlight_inference` call per turn)
so the GUI can stream the reply text chat-style while staying responsive.

Run:
    python gui_chat.py
Optional overrides via environment variables:
    SENLIGHT_BIN     path to the Rust inference executable
    SENLIGHT_WEIGHTS path to the .safetensors weights
    SENLIGHT_TOKENIZER path to tokenizer.json
"""

from __future__ import annotations

import os
import subprocess
import threading
import tkinter as tk
from pathlib import Path
from tkinter import scrolledtext, ttk

import config as C

# ---------------------------------------------------------------------------
# Paths / engine resolution
# ---------------------------------------------------------------------------
PROJECT = C.ROOT

def resolve_binary() -> Path:
    """Locate the Rust inference executable (release build preferred)."""
    explicit = os.environ.get("SENLIGHT_BIN")
    if explicit and Path(explicit).exists():
        return Path(explicit)
    cands = [
        PROJECT / "inference_rs" / "target" / "release" / "senlight_inference.exe",
        PROJECT / "inference_rs" / "target" / "release" / "senlight_inference",
        PROJECT / "inference_rs" / "target" / "debug" / "senlight_inference.exe",
    ]
    for c in cands:
        if c.exists():
            return c
    raise FileNotFoundError(
        "Rust inference engine not built. Run:\n"
        "    cd inference_rs && cargo build --release"
    )

BIN = resolve_binary()
WEIGHTS = os.environ.get("SENLIGHT_WEIGHTS", str(C.ARTIFACTS_DIR / "senlight_step4000.safetensors"))
TOKENIZER = os.environ.get("SENLIGHT_TOKENIZER", str(C.TOKENIZER_PATH))

# Generation defaults.
DEFAULTS = {"max_new": 96, "temperature": 0.7, "top_k": 50}


# ---------------------------------------------------------------------------
# Chat application
# ---------------------------------------------------------------------------
class ChatGUI:
    def __init__(self, root: tk.Tk) -> None:
        self.root = root
        self.root.title("Senlight Coder AI")
        self.root.geometry("900x680")
        self.root.minsize(640, 480)

        # Conversation context kept in Python and sent to the engine each turn.
        self.context = ""
        self.busy = False

        self._build_ui()
        self._append("system", "Senlight Coder AI — ready.")
        self._append(
            "system",
            "Model: raw code LM. Type code/a prompt; it completes it. "
            "Use the params box to tune generation.",
        )

    # ---------------- UI construction ----------------
    def _build_ui(self) -> None:
        # Top bar: params.
        top = ttk.Frame(self.root, padding=6)
        top.pack(fill="x")

        ttk.Label(top, text="Max tokens:").pack(side="left")
        self.var_max = tk.StringVar(value=str(DEFAULTS["max_new"]))
        ttk.Entry(top, textvariable=self.var_max, width=6).pack(side="left", padx=(2, 10))

        ttk.Label(top, text="Temperature:").pack(side="left")
        self.var_temp = tk.StringVar(value=str(DEFAULTS["temperature"]))
        ttk.Entry(top, textvariable=self.var_temp, width=6).pack(side="left", padx=(2, 10))

        ttk.Label(top, text="Top-K:").pack(side="left")
        self.var_topk = tk.StringVar(value=str(DEFAULTS["top_k"]))
        ttk.Entry(top, textvariable=self.var_topk, width=6).pack(side="left", padx=(2, 10))

        self.btn_clear = ttk.Button(top, text="New session", command=self._clear)
        self.btn_clear.pack(side="right")

        # Conversation transcript.
        frame = ttk.Frame(self.root)
        frame.pack(fill="both", expand=True, padx=6, pady=(0, 6))

        self.chat = scrolledtext.ScrolledText(
            frame, wrap="word", font=("Consolas", 11), state="disabled",
            bg="#111", fg="#eee", insertbackground="#eee",
        )
        self.chat.pack(fill="both", expand=True)

        # Input row.
        input_frame = ttk.Frame(self.root, padding=(6, 0, 6, 8))
        input_frame.pack(fill="x")

        self.entry = tk.Text(
            input_frame, height=4, font=("Consolas", 11),
            bg="#1e1e1e", fg="#eee", insertbackground="#eee",
        )
        self.entry.pack(side="left", fill="both", expand=True, padx=(0, 6))
        self.entry.bind("<Control-Return>", lambda _e: self._send())

        self.btn_send = ttk.Button(input_frame, text="Send", command=self._send)
        self.btn_send.pack(side="right")

        self.entry.focus_set()

    # ---------------- rendering ----------------
    def _append(self, tag: str, text: str) -> None:
        colors = {"system": "#8fa0b0", "user": "#6ec6ff", "ai": "#8be28b", "err": "#ff6b6b"}
        self.chat.configure(state="normal")
        self.chat.insert("end", text + "\n", tag)
        self.chat.tag_configure(tag, foreground=colors.get(tag, "#eee"))
        self.chat.configure(state="disabled")
        self.chat.see("end")

    def _set_busy(self, busy: bool) -> None:
        self.busy = busy
        self.btn_send.configure(state="disabled" if busy else "normal")
        self.btn_clear.configure(state="disabled" if busy else "normal")

    # ---------------- actions ----------------
    def _clear(self) -> None:
        self.context = ""
        self.chat.configure(state="normal")
        self.chat.delete("1.0", "end")
        self.chat.configure(state="disabled")
        self._append("system", "New session started.")

    def _send(self) -> None:
        if self.busy:
            return
        user = self.entry.get("1.0", "end").strip()
        if not user:
            return
        self.entry.delete("1.0", "end")
        self._append("user", f"You: {user}")

        # Fold into running context (keep under engine's max_seq).
        self.context += user + "\n"
        if len(self.context) > 4000:
            self.context = self.context[-4000:]

        try:
            max_new = max(1, int(self.var_max.get()))
            temp = max(0.05, float(self.var_temp.get()))
            top_k = max(1, int(self.var_topk.get()))
        except ValueError:
            self._append("err", "[params invalid, using defaults]")
            max_new, temp, top_k = DEFAULTS["max_new"], DEFAULTS["temperature"], DEFAULTS["top_k"]

        self._set_busy(True)
        threading.Thread(
            target=self._generate, args=(user, max_new, temp, top_k), daemon=True
        ).start()

    def _generate(self, user: str, max_new: int, temp: float, top_k: int) -> None:
        """Run the Rust engine on the running context; post the reply back."""
        try:
            cmd = [
                str(BIN), self.context, str(max_new), str(temp), str(top_k),
            ]
            env = dict(os.environ)
            env["SENLIGHT_WEIGHTS"] = WEIGHTS
            env["SENLIGHT_TOKENIZER"] = TOKENIZER
            proc = subprocess.run(
                cmd, capture_output=True, text=True, timeout=300, env=env,
                cwd=str(PROJECT / "inference_rs"),
            )
            out = proc.stdout.strip()
            if proc.returncode != 0:
                out = (out + "\n" + proc.stderr).strip() or f"[engine exit {proc.returncode}]"
            # Pull everything after the prompt echo marker.
            marker = "[infer] >>> "
            if marker in out:
                out = out.split(marker, 1)[1].strip()
        except Exception as exc:  # noqa: BLE001
            out = f"[engine error] {exc}"

        self.root.after(0, self._finish_turn, out)

    def _finish_turn(self, reply: str) -> None:
        self._append("ai", f"Senlight: {reply}")
        self.context += reply + "\n"
        self._set_busy(False)
        self.entry.focus_set()


def main() -> None:
    root = tk.Tk()
    ChatGUI(root)
    root.mainloop()


if __name__ == "__main__":
    main()