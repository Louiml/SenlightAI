"""Shared configuration for the Senlight Coder AI data pipeline.

Phase 1 (tokenization + dataset) uses a subset of these constants. Phase 2/3
extend this file with the model architecture and training hyperparameters so
that every stage reads from one source of truth.
"""

from pathlib import Path

# ---------------------------------------------------------------------------
# Filesystem layout
# ---------------------------------------------------------------------------
# The Python package lives at <repo>/python/. .. = repo root.
ROOT = Path(__file__).resolve().parent.parent
DATA_DIR = ROOT / "data"                 # raw corpus (ingest any .py/.rs/...)
ARTIFACTS_DIR = ROOT / "artifacts"       # tokenizer.json + data.bin
TOKENIZER_PATH = ARTIFACTS_DIR / "tokenizer.json"
DATA_BIN = ARTIFACTS_DIR / "data.bin"    # flat token-id stream (int32)

# ---------------------------------------------------------------------------
# Tokenizer hyperparameters (must match tokenizer_rs/src/main.rs)
# ---------------------------------------------------------------------------
VOCAB_SIZE = 32_000

# Special-token ids produced by the Rust trainer. The Python pipeline wraps each
# logical file/sequence with BOS and EOS.
PAD_ID = 3
BOS_ID = 1   # <|begin_of_text|>
EOS_ID = 2   # <|end_of_text|>

# ---------------------------------------------------------------------------
# Dataset (sliding context windows)
# ---------------------------------------------------------------------------
BLOCK_SIZE = 512     # context length per sample (input_ids / labels length)
STRIDE = 256         # window stride; overlap gives extra next-token targets
BATCH_SIZE = 8
SHUFFLE = True
SEED = 42
NUM_WORKERS = 0      # 0 = main process (simplest + most portable on Windows)

# File extensions treated as source code when scanning DATA_DIR.
SOURCE_EXTENSIONS = {
    ".py", ".rs", ".js", ".ts", ".jsx", ".tsx", ".java", ".c", ".h",
    ".cpp", ".hpp", ".go", ".rb", ".sh", ".toml", ".json", ".md", ".txt",
}

# ---------------------------------------------------------------------------
# Model architecture (Llama 3.1-style decoder-only transformer, ~303M params)
# ---------------------------------------------------------------------------
MODEL_DIM = 1024                # hidden size (embedding / transformer dim)
N_LAYERS = 24                   # transformer block count
N_HEADS = 16                    # attention query heads
N_KV_HEADS = 8                  # GQA: key/value heads (group size = 16/8 = 2)
HEAD_DIM = 64                   # per-head dim  (MODEL_DIM // N_HEADS)
FF_DIM = 2816                   # SwiGLU expanded intermediate dim
ROPE_THETA = 500_000.0          # RoPE base frequency (high, per Llama 3.x)
ROPE_MAX_LEN = 512              # precomputed rotary buffer length
RMS_EPS = 1e-5                  # RMSNorm epsilon
TIE_EMBEDDINGS = True           # share input embedding == output lm_head (saves ~46M)
USE_BIAS = False                # Llama uses bias-free linear layers
DROPOUT = 0.0                   # no dropout in the core transformer

# ---------------------------------------------------------------------------
# Training (used by Phase 3)
# ---------------------------------------------------------------------------
LR = 3e-4                       # peak AdamW learning rate
MIN_LR = 3e-5                   # cosine floor
WEIGHT_DECAY = 0.1
BETAS = (0.9, 0.95)
GRAD_CLIP_NORM = 1.0
WARMUP_STEPS = 200
MAX_STEPS = 20_000              # cosine schedule length
LOG_EVERY = 50
SAVE_EVERY = 1_000
MODEL_OUT = ARTIFACTS_DIR / "senlight.safetensors"   # Phase 3 export target