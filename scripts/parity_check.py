"""Cross-implementation parity harness.

Writes a small PyTorch checkpoint, runs the Rust engine against it, and compares logits.

This is the check neither repository had. `elafry_rs` is a second implementation of the model
in `elafry/models/`, and writing two implementations guarantees nothing about whether they
agree. They can differ in the GQA expansion order, in the RoPE convention, in where the
affective bias gets folded in, and every one of those differences produces a model that loads,
runs, reports plausible shapes, and generates fluent nonsense.

Two directions:

``--verify-cache``
    Rust cached decode against Rust full prefill. Isolates the cache path.

``--verify-parity``
    Rust logits against this file's dump. Isolates cross-language drift.

Both are checked against the tiny preset so they run in seconds on CPU.
"""

from __future__ import annotations

import argparse
import json
import subprocess
import sys
from pathlib import Path
from typing import Dict, List, Optional, Tuple

import torch

from elafry.config import AffectConfig, ModelConfig, PRESETS
from elafry.export import export_pretrained
from elafry.models.elafry import Elafry

CRATE_DIR = Path(__file__).resolve().parent.parent / "elafry_rs"

DEFAULT_PROMPT = "User: I feel alone and I do not know what to do.\n\nAssistant:"
TOP_K = 8


def build_tiny_checkpoint(
    out_dir: Path,
    tokenizer_path: Path,
    seed: int = 0,
    granularity: str = "head",
    live_bias: bool = True,
) -> Tuple[Elafry, ModelConfig, AffectConfig]:
    """A tiny checkpoint with a *live* residual path and affective bias.

    Both matter. At construction the model is the identity, so every logit is a copy of the
    input token and there is nothing for the two implementations to disagree about. Waking
    the residuals and the bias makes the comparison meaningful.
    """
    from tokenizers import Tokenizer

    torch.manual_seed(seed)
    preset = PRESETS["elafry-tiny"]

    # Size the vocabulary to the tokenizer. The Rust engine tokenizes the prompt itself, so
    # both sides must see the same ids; a tiny model with a 512-entry vocabulary would index
    # out of range on the first real token id.
    vocab_size = Tokenizer.from_file(str(tokenizer_path)).get_vocab_size()
    model_cfg = ModelConfig.from_dict(
        {**preset["model"].to_dict(), "vocab_size": vocab_size}
    )
    affect_cfg = AffectConfig(bias_granularity=granularity)

    model = Elafry(model_cfg, affect_cfg)

    gen = torch.Generator().manual_seed(seed + 7)
    with torch.no_grad():
        for layer in model.layers:
            layer.self_attn.o_proj.weight.normal_(0, 0.05, generator=gen)
            layer.mlp.down.weight.normal_(0, 0.05, generator=gen)
            if live_bias and layer.self_attn.affect is not None:
                layer.self_attn.affect.state_proj.weight.normal_(0, 0.3, generator=gen)

    # export_pretrained copies the tokenizer into the checkpoint directory, which is what lets
    # the Rust engine find it without a second flag and guarantees both sides tokenize from
    # the same file.
    export_pretrained(model, model_cfg, affect_cfg, out_dir, tokenizer_path)
    return model.eval(), model_cfg, affect_cfg

def write_dump(
    out_path: Path,
    model: Elafry,
    tokenizer_path: Optional[Path],
    prompt: str,
    state: Optional[Tuple[float, float, float]],
    top_k: int = TOP_K,
) -> Dict:
    """Write the top-k logits at every position, plus the prompt tokens.

    Top-k rather than all of the vocabulary, because the agreement is what matters and
    comparing every logit would report differences that are real but irrelevant.
    """
    from tokenizers import Tokenizer

    # No padding and no truncation: the prompt must encode to the same ids on both
    # sides, and a silent truncation difference would make every comparison meaningless.
    tk = Tokenizer.from_file(str(tokenizer_path))
    token_ids = tk.encode(prompt, add_special_tokens=False).ids

    state_t = None
    if state is not None:
        state_t = torch.tensor([list(state)], dtype=torch.float32)

    ids = torch.tensor([token_ids], dtype=torch.long)
    with torch.no_grad():
        logits = model(ids, state=state_t).logits[0]  # (S, V)

    topk = torch.topk(logits, top_k, dim=-1)
    rows: List[Tuple[int, int, float]] = []
    for position in range(logits.shape[0]):
        for idx, token_id in enumerate(topk.indices[position].tolist()):
            rows.append((position, token_id, float(topk.values[position][idx])))

    payload = {
        "prompt": prompt,
        "tokens": token_ids,
        "state": list(state) if state is not None else None,
        "topk": rows,
        "top_k": top_k,
    }
    out_path.write_text(json.dumps(payload), encoding="utf-8")
    return payload


def rust_binary(release: bool = True) -> Path:
    name = "elafry_rs.exe" if sys.platform == "win32" else "elafry_rs"
    profile = "release" if release else "debug"
    return CRATE_DIR / "target" / profile / name


def run_rust(args: List[str], release: bool = True) -> subprocess.CompletedProcess:
    binary = rust_binary(release)
    if not binary.exists():
        raise FileNotFoundError(
            f"{binary} not found. Build it with: cargo build --release (in {CRATE_DIR})"
        )
    return subprocess.run(
        [str(binary)] + args, capture_output=True, text=True, timeout=600
    )


def check_cache(ckpt_dir: Path, prompt: str, release: bool = True) -> bool:
    print("\n--- verify-cache (Rust cached decode vs Rust prefill) ---")
    result = run_rust(["--dir", str(ckpt_dir), "--verify-cache", prompt], release)
    print(result.stdout.strip())
    if result.returncode != 0:
        print(result.stderr.strip())
        return False
    return "ok" in result.stdout


def check_parity(ckpt_dir: Path, dump: Path, prompt: str, release: bool = True) -> bool:
    print("\n--- verify-parity (Rust vs Python) ---")
    result = run_rust(
        ["--dir", str(ckpt_dir), "--verify-parity", str(dump), prompt], release
    )
    print(result.stdout.strip())
    if result.returncode != 0:
        print(result.stderr.strip())
        return False
    return "ok" in result.stdout


def main(argv: Optional[List[str]] = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--work-dir", type=Path, default=None)
    parser.add_argument("--prompt", default=DEFAULT_PROMPT)
    parser.add_argument(
        "--state",
        default="-0.65,0.75,0.60",
        help="v,a,d to pin the state on both sides. Pinned by default because the Rust"
             " engine does not run the internal state predictor, so an unpinned Python"
             " forward would condition on a state Rust cannot reproduce.",
    )
    parser.add_argument("--debug", action="store_true", help="use the debug profile")
    args = parser.parse_args(argv)

    import tempfile

    work = args.work_dir or Path(tempfile.mkdtemp(prefix="elafry-parity-"))
    work.mkdir(parents=True, exist_ok=True)

    # A real tokenizer so the prompt encodes identically on both sides. Falls back to byte
    # ids when none is available, in which case parity is not run (the two sides would tokenize
    # differently and every comparison would be meaningless).
    tokenizer_path = Path(__file__).resolve().parent.parent / "artifacts" / "tokenizer.json"
    if not tokenizer_path.exists():
        tokenizer_path = Path("..") / "artifacts" / "tokenizer.json"
    if not tokenizer_path.exists():
        print(
            "No tokenizer.json found. The Rust engine needs one to encode the prompt, and "
            "parity is meaningless if the two sides tokenize differently.\n"
            "Point this at a directory with artifacts/tokenizer.json, or copy one there."
        )
        return 2
    tokenizer_path = tokenizer_path.resolve()

    state = None
    if args.state:
        parts = [float(x) for x in args.state.split(",")]
        if len(parts) != 3:
            parser.error("--state needs three comma-separated values")
        state = tuple(parts)  # type: ignore[assignment]

    print(f"work dir: {work}")
    print(f"tokenizer: {tokenizer_path}")

    ckpt_dir = work / "ckpt"
    print("building a tiny checkpoint with a live residual path and affective bias...")
    model, model_cfg, _ = build_tiny_checkpoint(ckpt_dir, tokenizer_path)
    print(f"  {model_cfg.n_layers} layers, dim {model_cfg.dim}, "
          f"{model_cfg.n_heads}/{model_cfg.n_kv_heads} heads")

    dump = work / "reference.json"
    write_dump(dump, model, tokenizer_path, args.prompt, state)
    print(f"wrote {len(json.loads(dump.read_text())['topk'])} reference logits")

    ok_cache = check_cache(ckpt_dir, args.prompt, not args.debug)
    ok_parity = check_parity(ckpt_dir, dump, args.prompt, not args.debug)

    print("\n--- summary ---")
    print(f"verify-cache: {'PASS' if ok_cache else 'FAIL'}")
    print(f"verify-parity: {'PASS' if ok_parity else 'FAIL'}")

    return 0 if (ok_cache and ok_parity) else 1


if __name__ == "__main__":
    raise SystemExit(main())
