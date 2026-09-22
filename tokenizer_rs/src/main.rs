//! Senlight Coder AI — Phase 1a: Rust BPE tokenizer trainer.
//!
//! This binary walks a source-code corpus (`data/`), trains a
//! Byte-Pair-Encoding tokenizer using Hugging Face's `tokenizers` crate, and
//! stores the result as a portable `tokenizer.json`.
//!
//! Whitespace and indentation handling:
//!   - A `ByteLevel` normalizer + pretokenizer map every raw byte (including
//!     `\t` and runs of spaces that form indentation) onto a private-use
//!     Unicode code point. Because each byte becomes exactly one code point,
//!     indentation survives tokenization and decodes back 1:1 (losslessly).
//!   - `byte_fallback` is enabled on the BPE model so any unknown byte falls
//!     back to byte-level merges and never emits `<unk>` for code bytes.
//!   - The trainer is seeded with `ByteLevel::alphabet()` so every byte can be
//!     represented from the start of training.
//!
//! Note on model typing: to run `train_from_files` with the concrete `BPE`
//! trainer we use a concrete `TokenizerImpl<BPE, ...>` (the generic `Tokenizer`
//! enum model requires its own wrapper trainer). All APIs mirror the standard
//! high-level `Tokenizer`.
//!
//! Verification: we round-trip several indented source snippets
//! (encode -> decode) and assert the decoded text exactly equals the input,
//! confirming whitespace/tabs survived.

use anyhow::{anyhow, Context, Result};
use std::fs;
use std::path::{Path, PathBuf};
use tokenizers::decoders::byte_level::ByteLevel as ByteLevelDecoder;
use tokenizers::models::bpe::{BPE, BpeTrainerBuilder};
use tokenizers::normalizers::byte_level::ByteLevel as ByteLevelNormalizer;
use tokenizers::pre_tokenizers::byte_level::ByteLevel as ByteLevelPreTokenizer;
use tokenizers::processors::PostProcessorWrapper;
use tokenizers::{AddedToken, Tokenizer};

/// Concrete, fully-parameterized tokenizer type: BPE model + ByteLevel
/// normalizer/pretokenizer/decoder. The generic `PostProcessorWrapper` is a
/// no-op until one is set.
type SenlightTokenizer = tokenizers::TokenizerImpl<
    BPE,
    ByteLevelNormalizer,
    ByteLevelPreTokenizer,
    PostProcessorWrapper,
    ByteLevelDecoder,
>;

// ---------------------------------------------------------------------------
// Configuration constants
// ---------------------------------------------------------------------------

/// Vocabulary size target. 32k is typical for small code models.
const VOCAB_SIZE: usize = 32_000;
/// Only keep merges observed at least this many times during training.
const MIN_FREQUENCY: u64 = 2;
/// File extensions we treat as source code, WITHOUT the leading dot
/// (Rust's `Path::extension()` returns the suffix without a dot).
const SOURCE_EXTENSIONS: &[&str] = &[
    "py", "rs", "js", "ts", "jsx", "tsx", "java", "c", "h", //
    "cpp", "hpp", "go", "rb", "sh", "toml", "json", "md", "txt",
];

/// Special tokens injected into every sequence during the data pipeline.
const SPECIAL_TOKENS: &[&str] = &["<unk>", "<|begin_of_text|>", "<|end_of_text|>", "<pad>"];

/// Source snippets we round-trip to prove whitespace/indentation handling.
const ROUNDTRIP_SNIPPETS: &[&str] = &[
    "def train(model):\n    optimizer.zero_grad()\n        loss.backward()\n    return loss\n",
    "fn compute(x: i64) -> i64 {\n\tif x % 2 == 0 {\n\t\treturn x * 2;\n\t} else {\n\t\treturn x + 1;\n\t}\n}\n",
    "class Foo:\n\tdef __init__(self, a, b):\n\t\tself.a = a\n\t\tself.b = b\n",
    "for i in range(10):\n        print(i)\n",
];

/// Corner-stone helper: resolve a path relative to the crate root.
///
/// Cargo places the binary at `<crate_root>/target/{debug,release}/<bin>` for
/// both `cargo run` and direct execution, so climbing two parents from the
/// executable reliably lands on the crate root regardless of the process CWD.
fn crate_root() -> Result<PathBuf> {
    let exe = std::env::current_exe().context("resolving current executable path")?;
    let root = exe
        .parent() // target/{debug,release}
        .and_then(Path::parent) // target
        .and_then(Path::parent) // crate root
        .ok_or_else(|| anyhow!("unexpected binary layout: {}", exe.display()))?;
    Ok(root.to_path_buf())
}

// ---------------------------------------------------------------------------
// File discovery
// ---------------------------------------------------------------------------

/// Recursively collect every file under `root` whose extension is a source
/// extension. Returns canonical paths so duplicate discovery is impossible.
fn collect_source_files(root: &Path) -> Result<Vec<PathBuf>> {
    let mut files = Vec::new();
    if !root.exists() {
        anyhow::bail!("corpus directory '{}' does not exist", root.display());
    }

    visit_dir(root, &mut files)?;
    files.sort();
    Ok(files)
}

/// Depth-first traversal; transparently handles subdirectories.
fn visit_dir(dir: &Path, out: &mut Vec<PathBuf>) -> Result<()> {
    for entry in fs::read_dir(dir).with_context(|| format!("reading dir {}", dir.display()))? {
        let entry = entry?;
        let path = entry.path();
        if path.is_dir() {
            visit_dir(&path, out)?;
        } else if SOURCE_EXTENSIONS
            .iter()
            .any(|ext| path.extension().is_some_and(|e| e.to_string_lossy() == *ext))
        {
            out.push(path);
        }
    }
    Ok(())
}

// ---------------------------------------------------------------------------
// Tokenizer construction
// ---------------------------------------------------------------------------

/// Build the pre-configured concrete tokenizer (BPE + ByteLevel). The model
/// starts empty and is populated in place by `train_from_files`.
fn new_byte_level_bpe() -> SenlightTokenizer {
    // GPT-2/LLaMA-style byte-level BPE. `byte_fallback: true` lets any token
    // not present in the vocab fall back to byte-level merges, matching how
    // HuggingFace ships LLaMA/GPT2 tokenizers (this round-trips losslessly).
    let bpe = BPE::builder()
        .byte_fallback(true)
        .unk_token("<unk>".to_string())
        .build()
        .expect("building BPE model config should not fail");

    let mut tokenizer = SenlightTokenizer::new(bpe);

    // Pretokenizer: the ByteLevel pretokenizer performs the byte->codepoint
    // mapping and the GPT-2 regex split (`use_regex=true`). We intentionally
    // do NOT add a separate ByteLevel *normalizer* on top: doing so applies
    // `bytes_char()` twice and double-encodes whitespace bytes (space became
    // the two-char `Äł` instead of `Ġ`), which breaks lossless decoding.
    tokenizer.with_pre_tokenizer(Some(ByteLevelPreTokenizer::new(false, false, true)));

    // Decoder: ByteLevel reverses the byte->codepoint mapping on decode.
    tokenizer.with_decoder(Some(ByteLevelDecoder::new(false, false, false)));

    tokenizer
}

// ---------------------------------------------------------------------------
// Verification helpers
// ---------------------------------------------------------------------------

/// Round-trip a raw string through the tokenizer and check lossless decode.
/// Prints a clear PASS/FAIL for each snippet. Indentation and tabs are the
/// specific things we are guarding against dropping.
fn verify_roundtrips(tokenizer: &SenlightTokenizer, saved_path: &str) -> bool {
    println!("\n=== Round-trip verification (whitespace/indentation) ===");
    let mut all_pass = true;

    // The deliverable artifact is the saved `tokenizer.json`, which is what the
    // Python data pipeline will load. Round-trip through the standard enum
    // `Tokenizer` loaded from disk to mirror that exact behavior.
    match Tokenizer::from_file(saved_path) {
        Ok(loaded) => {
            let enc = loaded.encode("def foo():\n\tpass", false).unwrap();
            let ids = enc.get_ids().to_vec();
            let rt = loaded.decode(&ids, false).unwrap_or_default();
            let ok = rt == "def foo():\n\tpass";
            all_pass &= ok;
            eprintln!(
                "[debug] RELOADED round-trip 'def foo():\\n\\tpass' -> {:?} => {}",
                rt,
                if ok { "OK" } else { "FAIL" }
            );
        }
        Err(e) => {
            eprintln!("[debug] RELOAD from saved json FAILED: {e}");
            all_pass = false;
        }
    }

    for &snippet in ROUNDTRIP_SNIPPETS {
        // Round-trip through the in-memory, fully-configured tokenizer first:
        let encoding = match tokenizer.encode(snippet, false) {
            Ok(enc) => enc,
            Err(e) => {
                println!("[FAIL] encode error: {e}");
                all_pass = false;
                continue;
            }
        };
        let ids = encoding.get_ids().to_vec();
        let decoded = tokenizer.decode(&ids, false).unwrap_or_default();

        // And also through the RELOADED (enum) tokenizer, to prove the saved
        // artifact that Python consumes behaves identically.
        let reloaded_decoded = Tokenizer::from_file(saved_path)
            .ok()
            .map(|t| t.decode(&ids, false).unwrap_or_default());

        let ok = snippet == decoded
            && reloaded_decoded
                .as_deref()
                .is_none_or(|rd| rd == snippet);
        all_pass &= ok;
        println!(
            "[{}] tokens={:>4}  ids={:?}",
            if ok { "PASS" } else { "FAIL" },
            ids.len(),
            &ids[..ids.len().min(12)]
        );
        println!("    original: {snippet:?}");
        println!("    decoded : {decoded:?}");
        if !ok {
            println!(
                "    MISMATCH  original_len={} decoded_len={}",
                snippet.len(),
                decoded.len()
            );
        }
        println!();
    }
    all_pass
}

/// Print a sample of the learned vocab (by lowest token id) so we can eyeball
/// whether space/indent tokens and compound code identifiers look sane.
fn print_vocab_sample(tokenizer: &SenlightTokenizer) -> Result<()> {
    let vocab = tokenizer.get_vocab(false);
    let mut entries: Vec<(&String, &u32)> = vocab.iter().collect();
    // Sort by id ascending: lower ids = higher-frequency merges in BPE output.
    entries.sort_by_key(|(_, id)| *id);

    println!("=== Sample of learned tokens (by lowest token id) ===");
    for (token, id) in entries.iter().take(24) {
        println!("  id={:>5}  {:?}", id, token);
    }
    if entries.len() > 24 {
        println!("  ... and {} more tokens total.", entries.len());
    }
    Ok(())
}

// ---------------------------------------------------------------------------
// main
// ---------------------------------------------------------------------------

fn main() -> Result<()> {
    // Resolve corpus/output paths relative to the crate root (not the CWD) so
    // this binary behaves identically under `cargo run` or direct invocation.
    let root = crate_root()?;
    // Canonicalize to strip any `..` segments and normalize Windows paths.
    // Optionally override via SENLIGHT_DATA_DIR to retrain on a specific corpus
    // (e.g. a conversational/psychological text directory).
    let data_dir = match std::env::var("SENLIGHT_DATA_DIR") {
        Ok(dir) => PathBuf::from(dir).canonicalize().context("canonicalizing SENLIGHT_DATA_DIR")?,
        Err(_) => root
            .join("..")
            .join("data")
            .canonicalize()
            .context("canonicalizing the data corpus directory")?,
    };
    let tokenizer_out = root
        .join("..")
        .join("artifacts")
        .join("tokenizer.json");

    // 1. Discover corpus files.
    let files = collect_source_files(&data_dir)?;
    println!(
        "Found {} source files under '{}':",
        files.len(),
        data_dir.display()
    );
    for f in &files {
        println!("  - {}", f.display());
    }
    if files.is_empty() {
        anyhow::bail!(
            "no source files found — drop .py/.rs/.js/etc files into '{}'",
            data_dir.display()
        );
    }

    // 2. Build the trainer. Seed the alphabet with the ByteLevel byte code
    //    points so every source byte is a first-class merge unit from the
    //    very first training pass (prevents spurious `<unk>`).
    let initial_alphabet: std::collections::HashSet<char> = ByteLevelPreTokenizer::alphabet()
        .into_iter()
        .collect();
    let special_tokens: Vec<AddedToken> = SPECIAL_TOKENS
        .iter()
        .map(|s| AddedToken::from(s.to_string(), true))
        .collect();

    let mut trainer = BpeTrainerBuilder::new()
        .vocab_size(VOCAB_SIZE)
        .min_frequency(MIN_FREQUENCY)
        .special_tokens(special_tokens)
        .initial_alphabet(initial_alphabet)
        .show_progress(true)
        .build();

    // 3. Train directly from the discovered paths (mutates the model in place).
    println!("Training BPE tokenizer (vocab={VOCAB_SIZE})...");
    let mut tokenizer = new_byte_level_bpe();
    let path_strings: Vec<String> = files
        .iter()
        .map(|p| p.to_string_lossy().to_string())
        .collect();
    tokenizer
        .train_from_files(&mut trainer, path_strings)
        .map_err(|e| anyhow!("BPE training failed: {e}"))?;

    // 4. Add special tokens to the vocabulary (so encode/decode knows them).
    let _added = tokenizer.add_special_tokens(
        &SPECIAL_TOKENS
            .iter()
            .map(|s| AddedToken::from(s.to_string(), true))
            .collect::<Vec<_>>(),
    );

    // 5. Persist. `save` exposes the standard `Tokenizer` serialization.
    if let Some(parent) = tokenizer_out.parent() {
        fs::create_dir_all(parent)?;
    }
    tokenizer
        .save(tokenizer_out.to_str().context("tokenizer path must be UTF-8")?, false)
        .map_err(|e| anyhow!("saving tokenizer to {}: {e}", tokenizer_out.display()))?;
    println!("Saved tokenizer -> {}", tokenizer_out.display());

    // 6. Verify whitespace/indentation round-trip.
    let tokenizer_out_str = tokenizer_out
        .to_str()
        .context("tokenizer path must be UTF-8")?
        .to_string();
    let roundtrip_ok = verify_roundtrips(&tokenizer, &tokenizer_out_str);
    print_vocab_sample(&tokenizer)?;

    if !roundtrip_ok {
        anyhow::bail!("round-trip verification FAILED — aborting pipeline");
    }
    println!("\nTokenizer OK. Ready for the Python data pipeline.");
    Ok(())
}