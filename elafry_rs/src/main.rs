//! elafry_rs: fast inference for Senlight Elafry, with affective state injection.
//!
//! ## Usage
//!
//! ```text
//! elafry_rs --dir <checkpoint> "prompt" [max_new] [temperature] [top_k]
//! elafry_rs --dir <checkpoint> --chat
//! elafry_rs --dir <checkpoint> --verify-cache
//! elafry_rs --dir <checkpoint> --verify-parity <dump.json>
//! ```
//!
//! ## Two rules this crate follows
//!
//! **Geometry is never hardcoded.** It comes from `config.json` beside the weights. The
//! previous engine in this repository hardcoded a `Config` struct, offered no flag or
//! environment variable to override it, and therefore could not load the only checkpoint that
//! existed. Every directory produced by `elafry.export` loads here with no arguments beyond
//! `--dir`.
//!
//! **The two implementations are checked against each other.** `--verify-cache` proves the KV
//! cache agrees with full prefill. `--verify-parity` proves this crate agrees with the Python
//! reference on the same prompt. Neither repository had either check, and both are how a model
//! ends up generating fluent nonsense that no loss curve would reveal.

mod config;
mod model;
mod sampler;

use anyhow::{anyhow, bail, Context, Result};
use candle_core::Device;
use std::path::{Path, PathBuf};

use config::{parse_vad, Config, TOKENIZER_FILENAME};
use model::Elafry;
use sampler::SamplingParams;

const HELP: &str = "\
elafry_rs - fast inference for Senlight Elafry

USAGE:
    elafry_rs [OPTIONS] <PROMPT> [MAX_NEW] [TEMPERATURE] [TOP_K]
    elafry_rs [OPTIONS] --chat
    elafry_rs [OPTIONS] --verify-cache
    elafry_rs [OPTIONS] --verify-parity <DUMP.json>

OPTIONS:
    --dir <PATH>            Checkpoint directory containing config.json
    --tokenizer <PATH>      tokenizer.json (default: <dir>/tokenizer.json)
    --chat                  Read turns from stdin instead of argv
    --set-vad V,A,D         Pin the affective state, bypassing the internal predictor
    --eos <ID>              Stop when this token is produced
    --seed <N>              Sampler seed, for reproducible output (default: 0x5e17)
    --temperature <F>       Default 0.8
    --top-k <N>             0 disables. Default 50
    --top-p <F>             1.0 disables. Default 1.0
    --verify-cache          Compare cached decode against full prefill, then exit
    --verify-parity <FILE>  Compare logits against a Python reference dump, then exit
    --tolerance <F>         Comparison tolerance. Default 2e-3
    -h, --help              This text

EXAMPLES:
    elafry_rs --dir ./ckpt \"I feel alone\"
    elafry_rs --dir ./ckpt --set-vad -0.65,0.75,0.60 \"I feel alone\"
    elafry_rs --dir ./ckpt --chat --temperature 0.7
    elafry_rs --dir ./ckpt --top-k 1 \"greedy decoding\"
";

struct Args {
    directory: Option<PathBuf>,
    tokenizer: Option<PathBuf>,
    chat: bool,
    verify_cache: bool,
    verify_parity: Option<PathBuf>,
    set_vad: Option<[f32; 3]>,
    prompt: Option<String>,
    params: SamplingParams,
    tolerance: f64,
}

impl Args {
    fn new() -> Self {
        Self {
            directory: None,
            tokenizer: None,
            chat: false,
            verify_cache: false,
            verify_parity: None,
            set_vad: None,
            prompt: None,
            params: SamplingParams::default(),
            tolerance: 2e-3,
        }
    }
}

fn parse_args() -> Result<Args> {
    let raw: Vec<String> = std::env::args().skip(1).collect();
    let mut args = Args::new();
    let mut positional: Vec<String> = Vec::new();
    let mut positional_only = false;
    let mut i = 0;

    // `next` pulls the value that follows a flag, naming the flag when it is missing rather
    // than failing with an index panic.
    let value = |i: &mut usize, flag: &str| -> Result<String> {
        *i += 1;
        raw.get(*i)
            .cloned()
            .with_context(|| format!("{flag} needs a value"))
    };

    while i < raw.len() {
        let arg = raw[i].clone();

        if !positional_only {
            match raw[i].as_str() {
                "-h" | "--help" => {
                    print!("{HELP}");
                    std::process::exit(0);
                }
                "--dir" => args.directory = Some(PathBuf::from(value(&mut i, "--dir")?)),
                "--tokenizer" => args.tokenizer = Some(PathBuf::from(value(&mut i, "--tokenizer")?)),
                "--chat" => args.chat = true,
                "--verify-cache" => args.verify_cache = true,
                "--verify-parity" => {
                    args.verify_parity = Some(PathBuf::from(value(&mut i, "--verify-parity")?))
                }
                "--set-vad" => args.set_vad = Some(parse_vad(&value(&mut i, "--set-vad")?)?),
                "--eos" => {
                    args.params.eos_id =
                        Some(value(&mut i, "--eos")?.parse().context("parsing --eos")?)
                }
                "--seed" => {
                    args.params.seed =
                        Some(value(&mut i, "--seed")?.parse().context("parsing --seed")?)
                }
                "--temperature" => {
                    args.params.temperature = value(&mut i, "--temperature")?
                        .parse()
                        .context("parsing --temperature")?
                }
                "--top-k" => {
                    args.params.top_k = value(&mut i, "--top-k")?
                        .parse()
                        .context("parsing --top-k")?
                }
                "--top-p" => {
                    args.params.top_p = value(&mut i, "--top-p")?
                        .parse()
                        .context("parsing --top-p")?
                }
                "--tolerance" => {
                    args.tolerance = value(&mut i, "--tolerance")?
                        .parse()
                        .context("parsing --tolerance")?
                }
                // Everything after this is positional, so a prompt beginning with a dash works.
                "--" => positional_only = true,
                _ if raw[i].starts_with("--") => bail!("unknown option {arg}\n\n{HELP}"),
                _ => positional.push(arg),
            }
        } else {
            positional.push(arg);
        }
        i += 1;
    }

    if let Some(prompt) = positional.first() {
        let mut rest = positional[1..].iter();
        if let Some(v) = rest.next() {
            args.params.max_new_tokens = v.parse().context("parsing max_new_tokens")?;
        }
        if let Some(v) = rest.next() {
            args.params.temperature = v.parse().context("parsing temperature")?;
        }
        if let Some(v) = rest.next() {
            args.params.top_k = v.parse().context("parsing top_k")?;
        }
        args.prompt = Some(prompt.clone());
    }

    Ok(args)
}

fn main() -> Result<()> {
    let mut args = parse_args()?;

    let directory = match args.directory.take() {
        Some(d) => d,
        None => config::default_directory().context(
            "no checkpoint directory given. Pass --dir, or run from a directory containing \
             config.json",
        )?,
    };

    let cfg = Config::load_from_dir(&directory)?;
    cfg.check_weights(&directory)?;
    println!("config:  {}", cfg.describe());
    println!("weights: ok (geometry matches)");

    let device = if candle_core::utils::cuda_is_available() {
        Device::new_cuda(0)?
    } else {
        // CPU without complaint. The CUDA path works; CPU keeps the verification modes
        // runnable on a machine with no GPU at all.
        Device::Cpu
    };

    let model = Elafry::from_dir(&directory, device)?;
    println!(
        "params:  {:.1}M in the transformer (the affective subsystem is extra)",
        cfg.model.implied_params() as f64 / 1e6
    );

    if args.verify_cache {
        let prompt = prompt_for_modes(args.prompt.as_deref());
        let ids = tokenize(&directory, args.tokenizer.as_deref(), &prompt)?;
        let (worst, checked) =
            model::verify_cache(&model, &ids, args.set_vad.as_ref(), args.tolerance)
                .context("KV cache does not match prefill")?;
        println!("verify-cache: ok, {checked} positions, max diff {worst:.3e}");
        return Ok(());
    }

    if let Some(dump) = args.verify_parity.take() {
        let prompt = prompt_for_modes(args.prompt.as_deref());
        let ids = tokenize(&directory, args.tokenizer.as_deref(), &prompt)?;
        let worst = model::verify_parity(&model, &dump, &ids, args.set_vad.as_ref(), args.tolerance)
            .context("Rust and Python disagree")?;
        println!("verify-parity: ok, max diff {worst:.3e}");
        return Ok(());
    }

    if args.chat {
        run_chat(&model, &directory, args.tokenizer.as_deref(), &args)
    } else {
        let prompt = args.prompt.clone().unwrap_or_default();
        generate_once(&model, &directory, args.tokenizer.as_deref(), &args, &prompt)
    }
}

/// The fixed prompt used by the verification modes, so a parity dump can be reproduced by
/// hand. Override it with a positional argument if you want a different one.
fn prompt_for_modes(explicit: Option<&str>) -> String {
    explicit.unwrap_or("User: I feel alone and I do not know what to do.\n\nAssistant:").to_string()
}

fn open_tokenizer(dir: &Path, explicit: Option<&Path>) -> Result<tokenizers::Tokenizer> {
    let path = match explicit {
        Some(p) => p.to_path_buf(),
        None => dir.join(TOKENIZER_FILENAME),
    };
    let tk = tokenizers::Tokenizer::from_file(&path)
        .map_err(|e| anyhow!("loading tokenizer from {}: {e}", path.display()))?;
    Ok(tk)
}

fn tokenize(dir: &Path, explicit: Option<&Path>, prompt: &str) -> Result<Vec<u32>> {
    let tk = open_tokenizer(dir, explicit)?;
    let encoding = tk
        .encode(prompt, false)
        .map_err(|e| anyhow!("encoding the prompt: {e}"))?;
    Ok(encoding.get_ids().to_vec())
}

fn generate_once(
    model: &Elafry,
    dir: &Path,
    tokenizer: Option<&Path>,
    args: &Args,
    prompt: &str,
) -> Result<()> {
    let ids = tokenize(dir, tokenizer, prompt)?;
    let (tokens, state) = model.generate(&ids, &args.params, args.set_vad.as_ref())?;

    let tk = open_tokenizer(dir, tokenizer)?;
    let text = tk
        .decode(&tokens, true)
        .map_err(|e| anyhow!("decoding the output: {e}"))?;

    println!(
        "[state] v={:+.3} a={:+.3} d={:+.3}",
        state[0], state[1], state[2]
    );
    let ids: Vec<String> = tokens.iter().map(|t| t.to_string()).collect();
    println!("[ids]   {}", ids.join(","));
    println!("[out]   {text}");
    Ok(())
}

fn run_chat(
    model: &Elafry,
    dir: &Path,
    tokenizer: Option<&Path>,
    args: &Args,
) -> Result<()> {
    let tk = open_tokenizer(dir, tokenizer)?;
    let eos_id = tk.token_to_id("<|eos|>").unwrap_or(2);

    let mut history: Vec<u32> = Vec::new();
    let mut state = args.set_vad;

    println!("[chat] /quit to leave");

    loop {
        print!("> ");
        std::io::Write::flush(&mut std::io::stdout())?;

        let mut line = String::new();
        if std::io::stdin().read_line(&mut line)? == 0 {
            break;
        }
        let line = line.trim();
        if line.is_empty() {
            continue;
        }
        if line == "/quit" || line == "exit" {
            break;
        }

        let formatted = format!("User: {line}\n\nAssistant:");
        let encoding = tk
            .encode(formatted.as_str(), false)
            .map_err(|e| anyhow!("encoding the turn: {e}"))?;

        let mut ids = history.clone();
        ids.extend_from_slice(encoding.get_ids());
        ids.push(eos_id);

        // Truncate from the left when the context is full. Recent context matters more than
        // distant context, so the oldest turns are the right ones to lose.
        let budget = model
            .cfg
            .max_seq_len
            .saturating_sub(args.params.max_new_tokens);
        if ids.len() > budget {
            ids.drain(..ids.len() - budget);
        }

        let mut params = args.params.clone();
        params.eos_id = Some(eos_id);

        let (tokens, next_state) = model.generate(&ids, &params, state.as_ref())?;
        let reply = tk
            .decode(&tokens, true)
            .map_err(|e| anyhow!("decoding the reply: {e}"))?;
        println!("{reply}");

        history = ids;
        history.extend_from_slice(&tokens);
        state = Some(next_state);

        println!(
            "[state] v={:+.3} a={:+.3} d={:+.3}",
            state.unwrap()[0],
            state.unwrap()[1],
            state.unwrap()[2]
        );
    }

    Ok(())
}