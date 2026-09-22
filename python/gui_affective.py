"""Senlight — Affective AI chat GUI (RLEF deployment loop).

A desktop chat window wired to the psychological Senlight model (SFT + reward
model + DPO aligned). It also performs **live latent emotional state tracking**:
each user message is analyzed into a Valence–Arousal–Dominance vector and a
Plutchik emotion, shown in a side panel, and that affective state is injected
into the model's prompt so the reply is conditioned to be empathetic and
de-escalating — the "deployment & live sentiment loop" from the training guide.

The text generation engine is the Rust/Candle `senlight_inference` binary.

Run:
    python gui_affective.py
Overrides: SENLIGHT_BIN / SENLIGHT_WEIGHTS / SENLIGHT_TOKENIZER
"""

from __future__ import annotations

import os
import subprocess
import threading
import tkinter as tk
from pathlib import Path
from tkinter import scrolledtext, ttk

import config as C
from affective.emotions import analyze, PLUTCHIK_VAD

DEFAULTS = {"max_new": 90, "temperature": 0.75, "top_k": 40}

PROJECT = C.ROOT


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
    "SENLIGHT_WEIGHTS", str(C.ARTIFACTS_DIR / "senlight_psy_dpo.safetensors")
)
TOKENIZER = os.environ.get("SENLIGHT_TOKENIZER", str(C.TOKENIZER_PATH))

# Affective de-escalation prefix: helps the model respond supportively.
EMPATHY_PREAMBLE = (
    "You are Senlight, a warm, empathetic listener. Validate the user's "
    "feelings, avoid judgment, de-escalate, and gently offer support.\n\n"
)


def emotional_context(user_text: str) -> tuple[str, dict]:
    """Turn the user's newest message into an affective prompt context."""
    st = analyze(user_text)
    hint = (
        f"[User is feeling {st.plutchik.lower()}; "
        f"VAD=({st.vad[0]:+.2f},{st.vad[1]:+.2f},{st.vad[2]:+.2f})]\n"
    )
    return hint, st.to_dict()


class AffectiveGUI:
    def __init__(self, root: tk.Tk) -> None:
        self.root = root
        self.root.title("Senlight — Affective AI")
        self.root.geometry("1120x700")
        self.root.minsize(760, 520)

        self.context = EMPATHY_PREAMBLE
        self.busy = False
        self._build_ui()
        self._append("system", "Senlight (Affective AI) — ready. "
                               "Your emotions are tracked in the right panel.")
        self._set_emotion("", "Neutral")

    # ------------------------------------------------------------------ UI
    def _build_ui(self) -> None:
        top = ttk.Frame(self.root, padding=6)
        top.pack(fill="x")
        ttk.Label(top, text="Max:").pack(side="left")
        self.var_max = tk.StringVar(value=str(DEFAULTS["max_new"]))
        ttk.Entry(top, textvariable=self.var_max, width=6).pack(side="left", padx=2)
        ttk.Label(top, text="Temp:").pack(side="left", padx=(8, 0))
        self.var_temp = tk.StringVar(value=str(DEFAULTS["temperature"]))
        ttk.Entry(top, textvariable=self.var_temp, width=6).pack(side="left", padx=2)
        ttk.Label(top, text="TopK:").pack(side="left", padx=(8, 0))
        self.var_topk = tk.StringVar(value=str(DEFAULTS["top_k"]))
        ttk.Entry(top, textvariable=self.var_topk, width=6).pack(side="left", padx=2)
        ttk.Button(top, text="New session", command=self._clear).pack(side="right")

        body = ttk.Frame(self.root)
        body.pack(fill="both", expand=True, padx=6)
        body.grid_columnconfigure(0, weight=1)

        # Chat transcript
        chat_frame = ttk.Frame(body)
        chat_frame.grid(row=0, column=0, sticky="nsew", pady=(0, 6))
        body.grid_rowconfigure(0, weight=1)
        self.chat = scrolledtext.ScrolledText(
            chat_frame, wrap="word", font=("Consolas", 11), state="disabled",
            bg="#111", fg="#eee", insertbackground="#eee",
        )
        self.chat.pack(fill="both", expand=True)

        # Emotional state panel
        side = ttk.Frame(body)
        side.grid(row=0, column=1, sticky="n", padx=(6, 0))
        ttk.Label(side, text="— Latent Emotional State —", font=("Inter", 10, "bold")).pack()
        self.lbl_emotion = tk.Label(side, text="Neutral", font=("Inter", 20, "bold"),
                                    fg="#64748b", bg="#fafafa")
        self.lbl_emotion.pack(pady=6)
        self.lbl_vad = tk.Label(side, text="V=+0.00 A=+0.00 D=+0.00",
                                font=("Consolas", 10), bg="#fafafa")
        self.lbl_vad.pack()
        self.lbl_conf = tk.Label(side, text="", font=("Consolas", 9), fg="#888", bg="#fafafa")
        self.lbl_conf.pack()
        ttk.Label(side, text="VAD meter", font=("Inter", 9, "bold")).pack(pady=(10, 2))
        self.bar_v = ttk.Progressbar(side, length=120, mode="determinate")
        self.bar_v.pack()
        tk.Label(side, text="Valence", font=("Inter", 8), fg="#888", bg="#fafafa").pack()
        self.bar_a = ttk.Progressbar(side, length=120, mode="determinate")
        self.bar_a.pack()
        tk.Label(side, text="Arousal", font=("Inter", 8), fg="#888", bg="#fafafa").pack()
        self.bar_d = ttk.Progressbar(side, length=120, mode="determinate")
        self.bar_d.pack()
        tk.Label(side, text="Dominance", font=("Inter", 8), fg="#888", bg="#fafafa").pack()

        # Input row
        row = ttk.Frame(self.root)
        row.pack(fill="x", padx=6, pady=(0, 8))
        self.entry = tk.Text(row, height=4, font=("Consolas", 11),
                             bg="#1e1e1e", fg="#eee", insertbackground="#eee")
        self.entry.pack(side="left", fill="both", expand=True, padx=(0, 6))
        self.entry.bind("<Control-Return>", lambda _e: self._send())
        self.btn_send = ttk.Button(row, text="Send", command=self._send)
        self.btn_send.pack(side="right")
        self.entry.focus_set()

    # ------------------------------------------------------------- rendering
    _COLORS = {"system": "#8fa0b0", "user": "#6ec6ff", "ai": "#8be28b",
               "err": "#ff6b6b"}

    def _append(self, tag: str, text: str) -> None:
        self.chat.configure(state="normal")
        self.chat.insert("end", text + "\n", tag)
        self.chat.tag_configure(tag, foreground=self._COLORS.get(tag, "#eee"))
        self.chat.configure(state="disabled")
        self.chat.see("end")

    _EMO_COLOR = {
        "Joy": "#22c55e", "Trust": "#14b8a6", "Anticipation": "#f59e0b",
        "Surprise": "#f59e0b", "Sadness": "#3b82f6", "Fear": "#8b5cf6",
        "Disgust": "#64748b", "Anger": "#ef4444", "Neutral": "#64748b",
    }

    def _set_emotion(self, text: str, emotion: str, vad=(0.0, 0.0, 0.0),
                     conf=0.0) -> None:
        self.lbl_emotion.config(text=emotion, fg=self._EMO_COLOR.get(emotion, "#888"))
        if text:
            self.lbl_vad.config(text=f"V={vad[0]:+.2f} A={vad[1]:+.2f} D={vad[2]:+.2f}")
        else:
            self.lbl_vad.config(text="V=+0.00 A=+0.00 D=+0.00")
        self.lbl_conf.config(text=f"signal {conf:.0%}")
        self.bar_v["value"] = int(50 + 50 * vad[0])
        self.bar_a["value"] = int(50 + 50 * vad[1])
        self.bar_d["value"] = int(50 + 50 * vad[2])

    def _set_busy(self, busy: bool) -> None:
        self.busy = busy
        self.btn_send.config(state="disabled" if busy else "normal")

    # ---------------------------------------------------------------- actions
    def _clear(self) -> None:
        self.context = EMPATHY_PREAMBLE
        self.chat.configure(state="normal")
        self.chat.delete("1.0", "end")
        self.chat.configure(state="disabled")
        self._set_emotion("", "Neutral")
        self._append("system", "New session started.")

    def _send(self) -> None:
        if self.busy:
            return
        user = self.entry.get("1.0", "end").strip()
        if not user:
            return
        self.entry.delete("1.0", "end")
        self._append("user", f"You: {user}")

        # Latent emotional state tracking.
        hint, st = emotional_context(user)
        self._set_emotion(user[:40], st["emotion"], tuple(st["vad"]), st["confidence"])
        self._append("system", f"[detected: {st['plutchik']}  VAD=({st['vad'][0]:+.2f},"
                                f"{st['vad'][1]:+.2f},{st['vad'][2]:+.2f})]")

        # Inject affective conditioning into the running context.
        self.context += hint + user + "\n"
        if len(self.context) > 6000:
            self.context = self.context[-6000:]

        try:
            max_new = max(1, int(self.var_max.get()))
            temp = max(0.05, float(self.var_temp.get()))
            top_k = max(1, int(self.var_topk.get()))
        except ValueError:
            max_new, temp, top_k = DEFAULTS.values()

        self._set_busy(True)
        threading.Thread(target=self._generate, args=(max_new, temp, top_k),
                         daemon=True).start()

    def _generate(self, max_new: int, temp: float, top_k: int) -> None:
        try:
            cmd = [str(BIN), self.context, str(max_new), str(temp), str(top_k)]
            env = dict(os.environ)
            env["SENLIGHT_WEIGHTS"] = WEIGHTS
            env["SENLIGHT_TOKENIZER"] = TOKENIZER
            proc = subprocess.run(cmd, capture_output=True, text=True, timeout=300,
                                  env=env, cwd=str(PROJECT / "inference_rs"))
            out = proc.stdout.strip()
            if proc.returncode != 0:
                out = (out + "\n" + proc.stderr).strip() or f"[engine exit {proc.returncode}]"
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
    AffectiveGUI(root)
    root.mainloop()


if __name__ == "__main__":
    main()