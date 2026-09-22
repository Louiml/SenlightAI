# Senlight Coder AI

A small, custom Large Language Model designed to understand and generate code,
built from scratch as a Llama-3.1-style decoder-only transformer (Grouped-Query
Attention, Rotary Position Embeddings, SwiGLU, Pre-RMSNorm).

The stack is deliberately **hybrid**, exactly as specified:

| Stage | Language | Tech |
|-------|----------|------|
| Tokenization (BPE) | **Rust** | `tokenizers` |
| Dataset / DataLoader | **Python** | PyTorch |
| Model architecture | **Python** | PyTorch |
| Training loop | **Python** | PyTorch / `torch.amp` |
| Fast inference | **Rust** | HuggingFace `candle` |

---

## Repository layout

```
Senlight/
├── data/                     # raw source-code corpus (any .py/.rs/.js/...)
├── artifacts/                # generated: tokenizer.json, data.bin, weights
│   ├── tokenizer.json
│   ├── data.bin  (+ .ranges.json)
│   └── senlight_step*.safetensors
├── python/
│   ├── requirements.txt
│   ├── config.py             # single source of truth for all hyperparameters
│   ├── dataset.py            # Phase 1b: Dataset / DataLoader / collate
│   ├── model.py              # Phase 2: RMSNorm, RoPE, SwiGLU, GQA, SenlightCoder
│   ├── train.py              # Phase 3: training loop + safetensors exporter
│   └── gui_chat.py           # Bonus: tkinter chat GUI over the Rust engine
├── tokenizer_rs/             # Phase 1a: Rust BPE tokenizer trainer
│   └── src/main.rs
└── inference_rs/             # Phase 4: Candle inference engine
    └── src/main.rs
```

---

## Model specifications (Llama 3.1-style)

### 316M (current default — bilingual EN/HE)

| Parameter              | Value       |
|------------------------|-------------|
| Vocabulary size        | 32,000      |
| Hidden dim             | **1024**     |
| Layers                 | **24**       |
| Attention heads        | **16**       |
| KV heads (GQA)         | **8**        |
| Head dim               | 64          |
| SwiGLU FF dim          | **2816**     |
| RoPE base (`theta`)    | 500,000     |
| Max sequence length    | 512         |
| RMSNorm epsilon        | 1e-5        |
| Weight tying           | yes         |
| **Total parameters**   | **~316M**   |

> Training wins: the 300M model **more than triples** capacity over the original
> ~100M. It fits the 12 GB RTX 3060 at batch 2–4 (fp16). The **same byte-level
> tokenizer** is retrained on a **bilingual EN+Hebrew corpus**, so it merges
> Hebrew into whole-word tokens (lossless round-trip). **Hebrew support** is
> added via `research/hebrew.jsonl` (greetings, feelings, food, holidays,
> numbers, colors, translation, writing system) baked into the tokenizer corpus.

> Honest note: a 316M model needs *far more* tokens (50M–100M+) than this
> ~14.5M-token demo corpus to be fully fluent. The retained checkpoints are:
>   * `senlight_300m_he2` — **300M bilingual (EN+HE)** — the default.
>   * `senlight_psy_refined` — the earlier 100M psychological model (reference).

> **Architecture highlights (mirroring `meta-llama/Llama-3.1`):**
> * **GQA** — 12 query heads share 4 KV heads, shrinking the inference KV cache 3×.
> * **RoPE** with a high base frequency (500k) improves long-context generalization.
> * **SwiGLU** gated feed-forward (`SiLU(gate·x) * up·x`) instead of ReLU/GELU.
> * **Pre-RMSNorm** bias-free normalization before every sublayer.

---

## Phase 1 — Tokenization & Data Pipeline

**Rust BPE tokenizer** (`tokenizer_rs`)
* Recursively scans `data/` for source files.
* Trains a byte-level BPE vocab (32k) using HF `tokenizers`.
* **Whitespace/indentation-safe**: a `ByteLevel` pretokenizer maps every byte
  (tabs, multi-space indents, newlines) to a private-use code point so
  indentation survives tokenization losslessly.
* Verifies 4 indented snippets round-trip (`encode → decode == input`) —
  including the *reloaded on-disk* `tokenizer.json`.
* Emits `artifacts/tokenizer.json`.

**Python DataLoader** (`python/dataset.py`)
* `tokenize_corpus()` → flat `int32` token stream in `artifacts/data.bin`
  (with per-file ranges persisted for reuse).
* `SenlightCodeDataset` slides overlapping context windows
  (`block_size=512`, `stride=256`) that never straddle file boundaries.
* Yields `input_ids` / shifted `labels` for next-token prediction.
* `make_dataloader()` with a padding-aware `collate_fn`.

```bash
cd tokenizer_rs && cargo run --release     # train + verify tokenizer
cd ../python    && python dataset.py       # build dataset, print batch stats
```

---

## Phase 2 — Core Model Architecture

`python/model.py` implements, as clean standalone modules:

* `RMSNorm` — bias-free Pre-Norm.
* RoPE — `precompute_rope_frequencies` + `apply_rotary_pos_emb`(+`rotate_half`).
* `SwiGLU` — gated feed-forward.
* `GroupedQueryAttention` — GQA with KV expansion.
* `SenlightCoderBlock` — Pre-Norm attention + Pre-Norm SwiGLU with residuals.
* `SenlightCoder` — full stack + `compute_loss` + LLaMA-style weight init
  (residual projections zero-initialized so each block starts as identity).

Verified: 100.1M params, forward/backward on CPU & RTX 3060, and
KV-cache consistency (`incremental == prefill`).

```bash
cd python && python -c "
import torch; from model import SenlightCoder
m = SenlightCoder()
print(sum(p.numel() for p in m.parameters()))   # 100092672
"
```

---

## Phase 3 — Training Loop

`python/train.py` provides:
* `AdamW` (weight decay 0.1, betas `(0.9, 0.95)`) + decoupled decay.
* Cosine-annealed LR with linear warmup (`LambdaLR`).
* Global gradient clipping (`max_grad_norm=1.0`).
* Mixed precision via `torch.amp` (`autocast(fp16)` + `GradScaler`) with a
  graceful CPU fallback (`--no-amp`).
* `export_to_safetensors()` → HF/LLaMA-style keys that Candle loads directly:
  `model.embed_tokens.weight`, `model.layers.N.*`, `model.norm.weight`
  (RoPE buffers excluded; tied lm_head collapsed into the embedding).

```bash
cd python
python train.py --steps 20000 --save-every 1000   # full run (GPU, fp16)
python train.py --steps 200  --no-amp             # CPU smoke test
```

Checkpoints (`checkpoint_stepN.pt`) are resumable; periodic `safetensors`
snapshots are emitted for the Rust engine.

---

## Phase 4 — Fast Inference Engine (Rust / Candle)

`inference_rs` loads the exported `safetensors` + `tokenizer.json` and
re-implements the Llama forward pass on `candle-core` + `candle-nn`:

* Pre-Norm `RMSNorm`, RoPE, GQA attention, SwiGLU FFN — all hand-written.
* Tied embeddings → single matmul against `model.embed_tokens.weight`.
* Temperature + top-k categorical sampling, streamed to the terminal.
* Optional **KV cache** for fast incremental (prefill + decode) generation.

```bash
cd inference_rs
cargo run --release -- "def fibonacci(n):" 64 0.8 50
# args: <prompt> <max_new_tokens> <temperature> <top_k>
# override paths:
SENLIGHT_WEIGHTS=... SENLIGHT_TOKENIZER=... cargo run --release

# Interactive chat REPL (keeps running context across turns):
cargo run --release -- --chat 96 0.7 50
#     args: --chat <max_new_tokens> <temperature> <top_k>    (type /quit to exit)

# Validate the KV cache vs full-context inference:
cargo run --release -- --verify-cache
```

**Active frequencies / device:** CPU backend by default (portable, no CUDA
toolkit required). CUDA is possible but needs a matching toolkit build.

---

## Affective AI (psychological model + RLEF)

Beyond the code-coder, Senlight has a **psychological / affective** training
track that follows the `research/` guide (AffectiveAI Systems): emotions live in
a 3D **Valence–Arousal–Dominance** space with **Plutchik's Wheel** categories,
and the model is aligned via Reinforcement Learning from Emotional Feedback.

### Pipeline (all stages trainable on the RTX 3060)

| Stage | Script | Output |
|-------|--------|--------|
| 0. Emotion engine | `python/affective/emotions.py` | VAD + Plutchik classifier (lexicon-based, offline) |
| 1. Data prep | `python/prep_psy.py` | combines `dataset.csv` + `Psychology-10K.json` + `counsel-chat` + `alpaca` → `psy_sft.jsonl` (with affective annotations) |
| 2. Tokenizer (psy) | `tokenizer_rs` (retrain on `data/psy/`) | conversational 32k vocab |
| 3. **SFT empathetic baseline** | `python/train_psy.py` | `senlight_psy_sft.safetensors` |
| 4. **Affective Reward Model** | `python/reward_model.py` | `affective_reward.pt` (Bradley–Terry + VAD/empathy MSE) |
| 5. **DPO/RL alignment** | `python/dpo.py` | `senlight_psy_dpo.safetensors` |
| 6. Deployment (live sentiment loop) | `python/gui_affective.py` | chat GUI with live emotion tracking |
| 6b. **Modern Web GUI** | `python/web_gui.py` | HTML/CSS/JS chat UI (http://127.0.0.1:8787) |
| 6c. **Desktop App** | `python/desktop_app.py` | native window (pywebview) running the same UI |

### Usage
```bash
# 1. prepare data (emotion-annotate dataset.csv + Psychology-10K)
cd python && python prep_psy.py

# 2. retrain tokenizer on conversational text
$env:SENLIGHT_DATA_DIR="D:/Senlight/data/psy"
(cd ../tokenizer_rs && cargo run --release)

# 3. SFT empathetic baseline
$env:SENLIGHT_DATA_DIR="D:/Senlight/data/psy"
python train_psy.py --steps 12000 --out senlight_psy_sft     # ~6M tokens / 65k rows
# optional: re-focus on psychology only (continuing the fluent base)
python train_psy.py --steps 3000 --pure --continue-from senlight_psy_sft --out senlight_psy_refined

# 4. affective reward model
python reward_model.py --steps 1500 --sft senlight_psy_sft --out affective_reward

# 5. DPO alignment
python dpo.py --steps 1500 --sft senlight_psy_sft --out senlight_psy_dpo

# 6. chat GUI with latent emotional state tracking
python gui_affective.py
```

**Modern web GUI** (zero-dependency, served by the stdlib HTTP server):

```bash
cd python
python web_gui.py            # open http://127.0.0.1:8787 in your browser
# optional: SENLIGHT_HOST=0.0.0.0 SENLIGHT_PORT=8787 python web_gui.py
```

The web UI mirrors the research visual: a dark, aurora-animated layout with a
**live Valence–Arousal–Dominance meter** and **Plutchik emotion badge** that
re-tint the page to the user's detected emotion, plus a chat composer that
streams empathetic replies from the DPO-aligned model. Frontend lives in
`python/web/` (index.html / static / styles.css / app.js).

**Desktop app** — the same modern UI in a native window (no browser, no HTTP
server): the frontend talks to Python directly through pywebview's
`window.pywebview.api` bridge (with a fetch fallback so the same UI also runs
as the web version).

```bash
pip install pywebview
cd python
python desktop_app.py            # native desktop window
```

The GUI detects the user's Plutchik emotion + VAD vector live, injects an
affective prompt context, and the DPO-aligned model responds empathetically
(validating, de-escalating, collaborative).

> **Training scale (expanded v2):** the corpus now merges **six** sources —
> `dataset.csv` (800), `Psychology-10K.json` (~9.8k), `nbertagnolli/counsel-chat`
> (~2.6k), `tatsu-lab/alpaca` (52k), `HuggingFaceH4/ultrachat_200k` (25k) and a
> curated **basic-greetings** set (36: hello / how-are-you / thanks / goodbye) —
> **90k rows / ~14.4M tokens**. The model is SFT-pretrained for 16k steps on the
> full corpus (fluent English base), then **refined on the 13k pure psychological
> rows** to restore empathetic focus. **`senlight_psy_refined` is the deployed
> model** — it produces the most fluent, on-topic supportive responses. Greeting
> data is included in the corpus and a greeting-warm variant
> (`senlight_psy_greetf`, `--only-source greetings`) is producible, but the
> tiny-greeting signal triggers repetition in a 100M model, so `refined` remains
> the best all-round checkpoint. Override with `SENLIGHT_WEIGHTS`.
>
> **DPO is now reward-sampled (fixed):** preference pairs are built by sampling
> candidate responses, scoring with the Affective Reward Model, and pairing
> highest- vs lowest-reward (replacing broken reversed-text rejections). Ran
> with 150–250 pairs; the mechanism converges, but this ~100M model
> **over-optimizes to gibberish** under DPO without stronger KL/early-stop
> control, so the SFT `refined` model remains the deployable. The pipeline is
> ready to reuse — see `dpo.py` + `dpo_pairs_large.jsonl`.

---

A tkinter chat window that talks to the Rust engine. Each turn spawns the
`senlight_inference` binary (subprocess) with the running conversation context
and streams the completion back into the transcript.

---

## Desktop chat GUI (`python/gui_chat.py`)

A tkinter chat window for the code model (dark theme, per-turn engine calls).

```bash
cd python
python gui_chat.py
```

Features:
* Dark, code-focused chat transcript (user / AI / system colored lines).
* Per-turn generation params: `max tokens`, `temperature`, `top-k`.
* **New session** button resets the conversation context.
* Press `Ctrl+Enter` to send a message.

Optional overrides:
```bash
SENLIGHT_BIN=... SENLIGHT_WEIGHTS=... SENLIGHT_TOKENIZER=... python gui_chat.py
```

> The GUI feeds the **full conversation context** to the code-completion model
> each turn. Train a larger / instruction-tuned model for genuinely useful
> conversational output.

---

## End-to-end recipe

```bash
# 1. Drop source files into data/ (any code you want the model to learn).

# 2. Train the tokenizer, build the dataset.
cd tokenizer_rs && cargo run --release
cd ../python    && python dataset.py

# 3. Train (RTX 3060, fp16) and export weights.
python train.py --steps 20000 --save-every 5000

# 4. Generate with the Rust engine.
cd ../inference_rs && cargo run --release -- "def fibonacci(n):" 128 0.8 50

# 5. Or chat with a GUI.
cd ../python && python gui_chat.py
```

> **Quality note:** the included 2-file sample corpus is tiny, so the trained
> model heavily overfits and recites that code. Drop in a large real code
> corpus and run more steps for genuinely novel generation.

---

## Requirements

* Rust 1.75+ / Cargo (`tokenizers`, `candle-core`, `candle-nn`, `rand`, `anyhow`)
* Python 3.9+
* PyTorch **with CUDA** for GPU training:
  ```bash
  pip install torch --index-url https://download.pytorch.org/whl/cu121
  pip install -r python/requirements.txt
  ```
* NVIDIA GPU recommended (verified on RTX 3060, 12 GB)