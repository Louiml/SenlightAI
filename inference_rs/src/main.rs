//! Senlight Coder AI — Phase 4: Fast inference engine (HuggingFace Candle).
//!
//! Loads the Phase-3 `safetensors` weights + the Phase-1 `tokenizer.json` and
//! runs a Llama-3.1-style decoder-only transformer to generate code.
//!
//! Architecture (must match `python/model.py` exactly):
//!   * Pre-Norm RMSNorm
//!   * Rotary Position Embeddings (theta = 500_000)
//!   * Grouped-Query Attention (12 query heads / 4 KV heads)
//!   * SwiGLU feed-forward network
//!   * Weight-tied embeddings (no separate lm_head tensor)
//!
//! Generation: temperature sampling + top-k filtering, streamed to stdout.

use anyhow::{anyhow, Context, Result};
use candle_core::{DType, Device, Tensor};
use std::path::PathBuf;
use tokenizers::Tokenizer;

// ---------------------------------------------------------------------------
// Model configuration (mirrors python/config.py)
// ---------------------------------------------------------------------------
#[derive(Clone)]
pub struct Config {
    pub vocab_size: usize,
    pub dim: usize,
    pub n_layers: usize,
    pub n_heads: usize,
    pub n_kv_heads: usize,
    pub head_dim: usize,
    pub ff_dim: usize,
    pub rope_theta: f32,
    pub max_seq_len: usize,
    pub rms_eps: f32,
}

impl Config {
    pub fn senlight() -> Self {
        Self {
            vocab_size: 32_000,
            dim: 1024,
            n_layers: 24,
            n_heads: 16,
            n_kv_heads: 8,
            head_dim: 64,
            ff_dim: 2816,
            rope_theta: 500_000.0,
            max_seq_len: 512,
            rms_eps: 1e-5,
        }
    }
}

// ---------------------------------------------------------------------------
// RMSNorm: y = x * gamma / sqrt(mean(x^2) + eps), gamma broadcast on last dim
// ---------------------------------------------------------------------------
struct RmsNorm {
    weight: Tensor, // (dim,)
    eps: f32,
}

impl RmsNorm {
    fn load(vb: &candle_nn::VarBuilder, dim: usize, eps: f32, name: &str) -> Result<Self> {
        let weight = vb.get((dim,), name)?;
        Ok(Self { weight, eps })
    }

    fn forward(&self, x: &Tensor) -> Result<Tensor> {
        // x: (..., D). rms: (..., 1).
        let eps_t = Tensor::new(self.eps, x.device())?;
        let rms = x
            .powf(2.0)?
            .mean([x.dims().len() - 1])?
            .broadcast_add(&eps_t)?
            .powf(-0.5)?
            .unsqueeze(x.dims().len() - 1)?;
        let y = x.broadcast_mul(&rms)?; // (..., D)
        y.broadcast_mul(&self.weight).map_err(anyhow::Error::from) // (..., D) * (D,)
    }
}

// ---------------------------------------------------------------------------
// RoPE helpers (mirrors python `precompute_rope_frequencies` + `rotate_half`)
// ---------------------------------------------------------------------------
fn precompute_rope(cfg: &Config, device: &Device) -> Result<(Tensor, Tensor)> {
    let half = cfg.head_dim / 2;
    let theta = cfg.rope_theta;
    let inv_freq: Vec<f32> = (0..half)
        .map(|i| theta.powf(-(2.0 * i as f32) / cfg.head_dim as f32))
        .collect();
    // full (max_len, head_dim) pre-rotated angles with duplicated halves.
    let mut angles = vec![0f32; cfg.max_seq_len * cfg.head_dim];
    for s in 0..cfg.max_seq_len {
        let sf = s as f32;
        for i in 0..cfg.head_dim {
            let f = inv_freq[i % half];
            angles[s * cfg.head_dim + i] = sf * f;
        }
    }
    let cos: Vec<f32> = angles.iter().map(|v| v.cos()).collect();
    let sin: Vec<f32> = angles.iter().map(|v| v.sin()).collect();
    let cos = Tensor::from_vec(cos, (cfg.max_seq_len, cfg.head_dim), device)?;
    let sin = Tensor::from_vec(sin, (cfg.max_seq_len, cfg.head_dim), device)?;
    Ok((cos, sin))
}

fn rotate_half(x: &Tensor) -> Result<Tensor> {
    let nd = x.dims().len() - 1;
    let d: usize = x.dim(nd)?;
    let h = d / 2;
    let x1 = x.narrow(nd, 0, h)?;
    let x2 = x.narrow(nd, h, d - h)?;
    Tensor::cat(&[x2.neg()?, x1], nd).map_err(anyhow::Error::from)
}

fn causal_mask(seq: usize, device: &Device) -> Result<Tensor> {
    let mut data = vec![0f32; seq * seq];
    for s in 0..seq {
        for t in (s + 1)..seq {
            data[s * seq + t] = f32::NEG_INFINITY;
        }
    }
    Tensor::from_vec(data, (seq, seq), device).map_err(anyhow::Error::from)
}

// ---------------------------------------------------------------------------
// Sampler: temperature + top-k + categorical draw on an f32 vec
// ---------------------------------------------------------------------------
struct Sampler {
    rng: rand::rngs::ThreadRng,
}

impl Sampler {
    fn new() -> Self {
        Self { rng: rand::thread_rng() }
    }

    fn sample(&mut self, logits: &Tensor, temperature: f32, top_k: usize) -> Result<usize> {
        let mut v: Vec<f32> = logits.to_vec1()?;
        if v.is_empty() {
            anyhow::bail!("empty logits");
        }
        for x in v.iter_mut() {
            *x /= temperature.max(1e-6);
        }
        if top_k > 0 && top_k < v.len() {
            let k = top_k;
            let mut idx: Vec<usize> = (0..v.len()).collect();
            idx.sort_by(|&a, &b| v[b].partial_cmp(&v[a]).unwrap_or(std::cmp::Ordering::Equal));
            let threshold = v[idx[k]];
            for x in v.iter_mut() {
                if *x < threshold {
                    *x = f32::NEG_INFINITY;
                }
            }
        }
        let max = v.iter().cloned().fold(f32::NEG_INFINITY, f32::max);
        let mut probs: Vec<f64> = v.iter().map(|x| ((x - max) as f64).exp()).collect();
        let sum: f64 = probs.iter().sum();
        for p in probs.iter_mut() {
            *p /= sum;
        }
        use rand::Rng;
        let u: f64 = self.rng.gen_range(0.0..1.0);
        let mut acc = 0.0;
        for (i, p) in probs.iter().enumerate() {
            acc += p;
            if u < acc {
                return Ok(i);
            }
        }
        Ok(probs
            .iter()
            .enumerate()
            .max_by(|a, b| a.1.partial_cmp(b.1).unwrap_or(std::cmp::Ordering::Equal))
            .map(|(i, _)| i)
            .unwrap_or(0))
    }
}

// ---------------------------------------------------------------------------
// Weights + forward pass
// ---------------------------------------------------------------------------
struct LayerWeights {
    input_norm: RmsNorm,
    q: Tensor,
    k: Tensor,
    v: Tensor,
    o: Tensor,
    post_attn_norm: RmsNorm,
    gate: Tensor,
    up: Tensor,
    down: Tensor,
}

struct SenlightCoder {
    cfg: Config,
    embed: Tensor,
    layers: Vec<LayerWeights>,
    norm: RmsNorm,
    head_index: Vec<u32>, // group-expanded KV head indices (len == n_heads)
}

impl SenlightCoder {
    fn load(vb: &candle_nn::VarBuilder, cfg: &Config) -> Result<Self> {
        let embed = vb.get((cfg.vocab_size, cfg.dim), "model.embed_tokens.weight")?;
        let norm = RmsNorm::load(vb, cfg.dim, cfg.rms_eps, "model.norm.weight")?;

        let mut layers = Vec::with_capacity(cfg.n_layers);
        for l in 0..cfg.n_layers {
            let p = format!("model.layers.{l}.");
            let input_norm = RmsNorm::load(vb, cfg.dim, cfg.rms_eps, &format!("{p}input_layernorm.weight"))?;
            let q = vb.get((cfg.n_heads * cfg.head_dim, cfg.dim), &format!("{p}self_attn.q_proj.weight"))?;
            let k = vb.get((cfg.n_kv_heads * cfg.head_dim, cfg.dim), &format!("{p}self_attn.k_proj.weight"))?;
            let v = vb.get((cfg.n_kv_heads * cfg.head_dim, cfg.dim), &format!("{p}self_attn.v_proj.weight"))?;
            let o = vb.get((cfg.dim, cfg.n_heads * cfg.head_dim), &format!("{p}self_attn.o_proj.weight"))?;
            let post_attn_norm = RmsNorm::load(vb, cfg.dim, cfg.rms_eps, &format!("{p}post_attention_layernorm.weight"))?;
            let gate = vb.get((cfg.ff_dim, cfg.dim), &format!("{p}mlp.gate_proj.weight"))?;
            let up = vb.get((cfg.ff_dim, cfg.dim), &format!("{p}mlp.up_proj.weight"))?;
            let down = vb.get((cfg.dim, cfg.ff_dim), &format!("{p}mlp.down_proj.weight"))?;
            layers.push(LayerWeights {
                input_norm,
                q,
                k,
                v,
                o,
                post_attn_norm,
                gate,
                up,
                down,
            });
        }

        // GQA group expansion: for n_kv=4, n_rep=3 -> [0,0,0,1,1,1,2,2,2,3,3,3].
        let n_rep = cfg.n_heads / cfg.n_kv_heads;
        let mut head_index = Vec::with_capacity(cfg.n_heads);
        for kv in 0..cfg.n_kv_heads {
            for _ in 0..n_rep {
                head_index.push(kv as u32);
            }
        }

        Ok(Self {
            cfg: cfg.clone(),
            embed,
            layers,
            norm,
            head_index,
        })
    }

    /// Prefill forward: run the whole `ids` (1, S) sequence, storing the
    /// per-layer KV tensors into `cache`, and return logits (1, S, vocab).
    fn forward_prefill(
        &self,
        ids: &Tensor,
        cos: &Tensor,
        sin: &Tensor,
        device: &Device,
        cache: &mut Vec<Option<(Tensor, Tensor)>>,
    ) -> Result<Tensor> {
        let seq = ids.dims()[1];
        let ids_flat = ids.flatten_all()?;
        let mut h = self.embed.index_select(&ids_flat, 0)?; // (seq, dim)

        let positions_data: Vec<u32> = (0..seq as u32).collect();
        let positions = Tensor::from_vec(positions_data, seq, device)?;
        let mask = causal_mask(seq, device)?;
        let k_idx = Tensor::from_vec(self.head_index.clone(), self.cfg.n_heads, device)?;
        let v_idx = Tensor::from_vec(self.head_index.clone(), self.cfg.n_heads, device)?;

        for (li, layer) in self.layers.iter().enumerate() {
            let h_norm = layer.input_norm.forward(&h)?;
            let q = h_norm.matmul(&layer.q.transpose(0, 1)?)?;
            let k = h_norm.matmul(&layer.k.transpose(0, 1)?)?;
            let v = h_norm.matmul(&layer.v.transpose(0, 1)?)?;

            let q = q.reshape((seq, self.cfg.n_heads, self.cfg.head_dim))?.transpose(0, 1)?;
            let k = k.reshape((seq, self.cfg.n_kv_heads, self.cfg.head_dim))?.transpose(0, 1)?;
            let v = v.reshape((seq, self.cfg.n_kv_heads, self.cfg.head_dim))?.transpose(0, 1)?;

            let cos_s = cos.index_select(&positions, 0)?.unsqueeze(0)?; // (1, seq, D)
            let sin_s = sin.index_select(&positions, 0)?.unsqueeze(0)?;
            let q = q.broadcast_mul(&cos_s)?.broadcast_add(&rotate_half(&q)?.broadcast_mul(&sin_s)?)?;
            let k = k.broadcast_mul(&cos_s)?.broadcast_add(&rotate_half(&k)?.broadcast_mul(&sin_s)?)?;

            // Store raw (KV-head) tensors into the cache (no GQA expansion yet).
            let k_store = k.contiguous()?;
            let v_store = v.contiguous()?;
            cache[li] = Some((k_store.clone(), v_store.clone()));

            // Do not retain the full Q; but for prefill we expand KV now.
            let k = k.contiguous()?.index_select(&k_idx, 0)?;
            let v = v.contiguous()?.index_select(&v_idx, 0)?;

            let scale = self.cfg.head_dim as f32;
            let scale_t = Tensor::new(scale.sqrt().recip(), device)?;
            let scores = q.matmul(&k.transpose(1, 2)?)?;
            let attn = scores.broadcast_mul(&scale_t)?;
            let attn = attn.broadcast_add(&mask)?;
            let attn = candle_nn::ops::softmax(&attn, 2)?;
            let attn_out = attn.matmul(&v)?;
            let attn_out = attn_out.transpose(0, 1)?.reshape((seq, self.cfg.n_heads * self.cfg.head_dim))?;
            let attn_proj = attn_out.matmul(&layer.o.transpose(0, 1)?)?;
            h = h.add(&attn_proj)?;

            // FullyConnected/MoE style FFN (SwiGLU).
            let h_norm = layer.post_attn_norm.forward(&h)?;
            let gate = candle_nn::ops::silu(&h_norm.matmul(&layer.gate.transpose(0, 1)?)?)?;
            let up = h_norm.matmul(&layer.up.transpose(0, 1)?)?;
            let mlp_out = gate.broadcast_mul(&up)?.matmul(&layer.down.transpose(0, 1)?)?;
            h = h.add(&mlp_out)?;
        }

        let h = self.norm.forward(&h)?;
        let logits = h.matmul(&self.embed.transpose(0, 1)?)?;
        logits.unsqueeze(0).map_err(anyhow::Error::from)
    }

    /// Incremental decode: given the single new token id at absolute `pos`,
    /// append its K/V to the per-layer cache and return logits for that token.
    fn forward_next(
        &self,
        token_id: u32,
        pos: usize,
        cos: &Tensor,
        sin: &Tensor,
        device: &Device,
        cache: &mut Vec<Option<(Tensor, Tensor)>>,
    ) -> Result<Tensor> {
        // h: (1, dim) — the single current token's embedding.
        let ids = Tensor::new(&[token_id], device)?;
        let mut h = self.embed.index_select(&ids, 0)?;

        let positions = Tensor::new(&[pos as u32], device)?;
        let k_idx = Tensor::from_vec(self.head_index.clone(), self.cfg.n_heads, device)?;
        let v_idx = Tensor::from_vec(self.head_index.clone(), self.cfg.n_heads, device)?;
        let scale = self.cfg.head_dim as f32;
        let scale_t = Tensor::new(scale.sqrt().recip(), device)?;

        for (li, layer) in self.layers.iter().enumerate() {
            let h_norm = layer.input_norm.forward(&h)?;
            let q = h_norm.matmul(&layer.q.transpose(0, 1)?)?; // (1, n_heads*D)
            let k = h_norm.matmul(&layer.k.transpose(0, 1)?)?; // (1, n_kv*D)
            let v = h_norm.matmul(&layer.v.transpose(0, 1)?)?; // (1, n_kv*D)

            let q = q.reshape((1, self.cfg.n_heads, self.cfg.head_dim))?.transpose(0, 1)?; // (n_heads,1,D)
            let k = k.reshape((1, self.cfg.n_kv_heads, self.cfg.head_dim))?.transpose(0, 1)?; // (n_kv,1,D)
            let v = v.reshape((1, self.cfg.n_kv_heads, self.cfg.head_dim))?.transpose(0, 1)?; // (n_kv,1,D)

            // RoPE at the absolute position for the new token.
            let cos_s = cos.index_select(&positions, 0)?.reshape((1, self.cfg.head_dim))?.unsqueeze(0)?; // (1,1,D)
            let sin_s = sin.index_select(&positions, 0)?.reshape((1, self.cfg.head_dim))?.unsqueeze(0)?;
            let q = q.broadcast_mul(&cos_s)?.broadcast_add(&rotate_half(&q)?.broadcast_mul(&sin_s)?)?;
            let k = k.broadcast_mul(&cos_s)?.broadcast_add(&rotate_half(&k)?.broadcast_mul(&sin_s)?)?;

            // Append new K/V to the cache for this layer.
            let (cached_k, cached_v) = cache[li].as_ref().unwrap();
            let k = Tensor::cat(&[cached_k.clone(), k], 1)?; // (n_kv, cur_len, D)
            let v = Tensor::cat(&[cached_v.clone(), v], 1)?; // (n_kv, cur_len, D)
            cache[li] = Some((k.clone(), v.clone()));

            // GQA-expand KV then attend over the whole cached sequence.
            let k = k.contiguous()?.index_select(&k_idx, 0)?; // (n_heads, cur_len, D)
            let v = v.contiguous()?.index_select(&v_idx, 0)?;

            let scores = q.matmul(&k.transpose(1, 2)?)?; // (n_heads, 1, cur_len)
            let attn = scores.broadcast_mul(&scale_t)?;
            let attn = candle_nn::ops::softmax(&attn, 2)?;
            let attn_out = attn.matmul(&v)?; // (n_heads, 1, D)
            let attn_out = attn_out.transpose(0, 1)?.reshape((1, self.cfg.n_heads * self.cfg.head_dim))?;
            let attn_proj = attn_out.matmul(&layer.o.transpose(0, 1)?)?;
            h = h.add(&attn_proj)?;

            // SwiGLU FFN.
            let h_norm = layer.post_attn_norm.forward(&h)?;
            let gate = candle_nn::ops::silu(&h_norm.matmul(&layer.gate.transpose(0, 1)?)?)?;
            let up = h_norm.matmul(&layer.up.transpose(0, 1)?)?;
            let mlp_out = gate.broadcast_mul(&up)?.matmul(&layer.down.transpose(0, 1)?)?;
            h = h.add(&mlp_out)?;
        }

        let h = self.norm.forward(&h)?;
        let logits = h.matmul(&self.embed.transpose(0, 1)?)?; // (1, vocab)
        logits.reshape(self.cfg.vocab_size).map_err(anyhow::Error::from)
    }
}

// ---------------------------------------------------------------------------
// Generation
// ---------------------------------------------------------------------------
/// Greedy/autoregressive generation with a **KV cache**:
///   1. `forward_prefill` runs the whole prompt once, caching K/V per layer.
///   2. Each subsequent token runs `forward_next` (single-position attention
///      over the full cached sequence) instead of re-running the whole context,
///      turning a per-token O(seq²) cost into O(seq).
fn generate(
    model: &SenlightCoder,
    tokenizer: &Tokenizer,
    prompt: &str,
    device: &Device,
    max_new_tokens: usize,
    temperature: f32,
    top_k: usize,
    eos_id: u32,
) -> Result<String> {
    let cfg = &model.cfg;
    let (cos, sin) = precompute_rope(cfg, device)?;

    let encoding = tokenizer
        .encode(prompt, false)
        .map_err(|e| anyhow!("tokenizing prompt: {e}"))?;
    let mut ids: Vec<u32> = encoding.get_ids().to_vec();
    if ids.is_empty() {
        ids.push(0);
    }
    if ids.len() > cfg.max_seq_len {
        ids.truncate(cfg.max_seq_len);
    }

    // Per-layer KV cache, initially empty.
    let mut cache: Vec<Option<(Tensor, Tensor)>> = vec![None; cfg.n_layers];

    // Prefill: embed + run the prompt, caching K/V. Grab logits at the last
    // position to predict the first generated token.
    let pre_input = Tensor::from_vec(ids.clone(), (1, ids.len()), device)?;
    let pre_logits = model.forward_prefill(&pre_input, &cos, &sin, device, &mut cache)?;
    let first_logits = pre_logits
        .narrow(1, ids.len() - 1, 1)?
        .reshape(cfg.vocab_size)?;

    let mut sampler = Sampler::new();
    let mut out: Vec<u32> = Vec::new();
    let mut next_logits = first_logits;

    for _ in 0..max_new_tokens {
        let tok = sampler.sample(&next_logits, temperature, top_k)? as u32;
        if tok == eos_id || ids.len() >= cfg.max_seq_len {
            if tok != eos_id {
                out.push(tok); // token fits but we hit max length
            }
            break;
        }
        ids.push(tok);
        out.push(tok);

        // Absolute position of the token just appended = len(ids)-1.
        let pos = ids.len() - 1;
        next_logits = model.forward_next(tok, pos, &cos, &sin, device, &mut cache)?;
    }

    tokenizer.decode(&out, true).map_err(|e| anyhow!("decoding output: {e}"))
}

/// Correctness check: for every position i, the logits that `forward_next`
/// produces when decoding token[i] (using the incremental cache) must match the
/// logits `forward_prefill` yields at position i for the full sequence. This
/// proves the KV cache reproduces full-context inference exactly.
fn verify_cache(model: &SenlightCoder, tokenizer: &Tokenizer, device: &Device) -> Result<()> {
    let cfg = &model.cfg;
    let (cos, sin) = precompute_rope(cfg, device)?;
    let prompt = "def foo():\n    return 42\n";

    let encoding = tokenizer.encode(prompt, false).map_err(|e| anyhow!(e.to_string()))?;
    let mut ids = encoding.get_ids().to_vec();
    if ids.len() > 64 {
        ids.truncate(64);
    }

    // Reference: full-context prefill over the whole sequence.
    let mut cache: Vec<Option<(Tensor, Tensor)>> = vec![None; cfg.n_layers];
    let full = Tensor::from_vec(ids.clone(), (1, ids.len()), device)?;
    let ref_logits = model.forward_prefill(&full, &cos, &sin, device, &mut cache)?; // (1, S, vocab)

    // Incremental: rebuild the cache prefix-by-prefix and compare each step.
    let mut max_err: f64 = 0.0;
    for end in 1..ids.len() {
        let mut icache: Vec<Option<(Tensor, Tensor)>> = vec![None; cfg.n_layers];
        let prefix = Tensor::from_vec(ids[..end].to_vec(), (1, end), device)?;
        let _ = model.forward_prefill(&prefix, &cos, &sin, device, &mut icache)?;
        // Decode token at position `end` with the cache and compare to prefill
        // logits at position (end-1) which predict token `end`.
        let step_logits = model.forward_next(ids[end], end, &cos, &sin, device, &mut icache)?;
        let ref_slice = ref_logits.narrow(1, end, 1)?.reshape(cfg.vocab_size)?;
        let a: Vec<f32> = step_logits.to_vec1()?;
        let b: Vec<f32> = ref_slice.to_vec1()?;
        for (x, y) in a.iter().zip(b.iter()) {
            let e = (x - y).abs() as f64;
            if e > max_err {
                max_err = e;
            }
        }
    }
    println!("[verify] max |incremental - prefill| logit diff = {max_err:.3e}");
    if max_err < 1e-3 {
        println!("[verify] KV CACHE IS CORRECT (matches full-context forward).");
        Ok(())
    } else {
        anyhow::bail!("KV cache verification FAILED (max diff {max_err:e})");
    }
}

/// Interactive chat REPL. Each user turn is appended to a running context, the
/// LM generates a continuation with temperature/top-k (streamed text via the
/// KV-cache engine), and the reply is echoed back and folded into context so
/// later turns "remember" the conversation.
///
/// Because Senlight is a code-completion LM (not instruction-tuned), the model
/// will naturally continue code. Type `/quit` (or `exit`) to leave.
fn chat(
    model: &SenlightCoder,
    tokenizer: &Tokenizer,
    device: &Device,
    temperature: f32,
    top_k: usize,
    max_new: usize,
) -> Result<()> {
    use std::io::{BufRead, Write};

    println!("[chat] Senlight Coder AI interactive session.");
    println!("[chat] Model is a raw code LM — it completes from your prompt.");
    println!("[chat] Type your input & press Enter. Type /quit to exit.\n");
    print!("You  > ");
    std::io::stdout().flush()?;

    let stdin = std::io::stdin();
    let mut context = String::new();

    for line in stdin.lock().lines() {
        let line = line?;
        let trimmed = line.trim();
        if trimmed.eq_ignore_ascii_case("/quit") || trimmed.eq_ignore_ascii_case("exit") {
            println!("[chat] Bye!");
            break;
        }
        if trimmed.is_empty() {
            print!("You  > ");
            std::io::stdout().flush()?;
            continue;
        }

        // Append / slightly lighten context so it stays under max_seq_len.
        context.push_str(trimmed);
        context.push('\n');
        if context.len() > 2000 {
            let cut = context.len() - 2000;
            context = context.split_off(cut);
        }

        let reply = generate(model, tokenizer, &context, device, max_new, temperature, top_k, 2)?;
        print!("Senlight > ");
        std::io::stdout().flush()?;
        println!("{reply}");
        // Fold the reply back into the running context.
        context.push_str(&reply);
        context.push('\n');

        print!("\nYou  > ");
        std::io::stdout().flush()?;
    }
    Ok(())
}

// ---------------------------------------------------------------------------
// main
// ---------------------------------------------------------------------------
fn main() -> Result<()> {
    let exe = std::env::current_exe().context("locating executable")?;
    let root = exe
        .parent()
        .and_then(|p| p.parent())
        .and_then(|p| p.parent())
        .context("unexpected binary layout")?;

    let default_weights = root.join("..").join("artifacts").join("senlight_300m_he2.safetensors");
    let weights: PathBuf = std::env::var("SENLIGHT_WEIGHTS").map(PathBuf::from).unwrap_or(default_weights);
    let default_tok = root.join("..").join("artifacts").join("tokenizer.json");
    let tok_path: PathBuf = std::env::var("SENLIGHT_TOKENIZER").map(PathBuf::from).unwrap_or(default_tok);

    if !weights.exists() {
        anyhow::bail!(
            "weights not found at {}. Train first (phase 3) or set SENLIGHT_WEIGHTS.",
            weights.display()
        );
    }

    let device = Device::Cpu;
    let tokenizer = Tokenizer::from_file(tok_path).map_err(|e| anyhow!("loading tokenizer: {e}"))?;
    let cfg = Config::senlight();

    let vb = unsafe {
        candle_nn::VarBuilder::from_mmaped_safetensors(&[weights], DType::F32, &device)?
    };
    let model = SenlightCoder::load(&vb, &cfg)?;
    println!(
        "[infer] loaded {} layers (dim={}, heads={}, kv={}, ff={}) on CPU.",
        cfg.n_layers, cfg.dim, cfg.n_heads, cfg.n_kv_heads, cfg.ff_dim
    );

    let prompt = std::env::args().nth(1).unwrap_or_else(|| "def fibonacci(n):\n".to_string());
    let max_new = std::env::args().nth(2).and_then(|s| s.parse::<usize>().ok()).unwrap_or(64);
    let temperature = std::env::args().nth(3).and_then(|s| s.parse::<f32>().ok()).unwrap_or(0.8);
    let top_k = std::env::args().nth(4).and_then(|s| s.parse::<usize>().ok()).unwrap_or(50);

    // `--verify-cache` (as the first arg) validates the KV cache against the
    // full-context forward path instead of running generation.
    if prompt.trim() == "--verify-cache" {
        verify_cache(&model, &tokenizer, &device)?;
        return Ok(());
    }

    // `--chat` starts an interactive REPL (args 2..4 = max_new, temperature, top_k).
    if prompt.trim() == "--chat" {
        let cmax = std::env::args().nth(2).and_then(|s| s.parse::<usize>().ok()).unwrap_or(96);
        let ctemp = std::env::args().nth(3).and_then(|s| s.parse::<f32>().ok()).unwrap_or(0.7);
        let ctopk = std::env::args().nth(4).and_then(|s| s.parse::<usize>().ok()).unwrap_or(50);
        return chat(&model, &tokenizer, &device, ctemp, ctopk, cmax);
    }

    println!("[infer] prompt: {prompt:?}");
    let generated = generate(&model, &tokenizer, &prompt, &device, max_new, temperature, top_k, 2)?;
    println!("[infer] >>> {generated}");
    println!();
    Ok(())
}
