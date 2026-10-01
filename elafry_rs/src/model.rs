//! The model: decoder-only transformer with affective bias injection on the attention logits.
//!
//! Mirrors `elafry/models/` in Python. The two must agree numerically, and `--verify-parity`
//! is how that is checked.
//!
//! Deviations from a plain Llama forward pass, each with a reason:
//!
//! * An **Affective Bias Matrix** derived from the internal state is folded into the queries
//!   before the score matmul, so it lands on the pre-softmax logits.
//! * The **KV cache stores unexpanded** keys and values at `n_kv_heads` width, and expansion
//!   happens inside the forward pass. The cache is `n_rep` times smaller as a result.
//! * The **output projection is tied** to the embedding matrix unless the config says not.

use anyhow::{bail, Context, Result};
use candle_core::{DType, Device, IndexOp, Tensor};
use candle_nn::{Embedding, Linear, Module, VarBuilder};

use crate::config::{AffectConfig, ModelConfig, WEIGHTS_FILENAME};
use crate::sampler::SamplingParams;

/// One layer's cached keys and values, stored *unexpanded* at `n_kv_heads`.
#[derive(Clone)]
pub struct KVEntry {
    pub k: Tensor,
    pub v: Tensor,
}

pub type KVCache = Vec<KVEntry>;

/// RMSNorm. Reduces in f32, because the mean of squares is a long reduction and losing
/// mantissa to it changes the normalisation.
struct RmsNorm {
    weight: Tensor,
    eps: f32,
}

impl RmsNorm {
    fn load(vb: &VarBuilder<'_>, name: &str, dim: usize, eps: f32) -> Result<Self> {
        // The exporter stores RMSNorm gains as a flat vector, not a (1, dim) row, so the shape
        // hint has to match. Candle checks the hint against the file and refuses otherwise.
        let weight = vb
            .pp(name)
            .get(dim, "weight")
            .with_context(|| format!("loading RMSNorm weight {name}"))?;
        if weight.dim(0)? != dim {
            bail!("{name} has width {}, expected {dim}", weight.dim(0)?);
        }
        Ok(Self {
            weight: weight.reshape(dim)?,
            eps,
        })
    }

    fn forward(&self, x: &Tensor) -> Result<Tensor> {
        // Sum over the *feature* axis, not the sequence axis. Reducing dim 1 instead of the
        // last one yields the same shape either way, so nothing looks wrong, but the
        // divisor then depends on sequence length: prefill divides by 18 and a decode step
        // by 1, so the two paths compute different normalisations and the KV cache diverges
        // from prefill. This is exactly what --verify-cache is for.
        let sum_squares = x.sqr()?.sum_keepdim(2)?;
        let width = x.dim(2)? as f64;
        let mean = (sum_squares / width)?;
        let scale = (mean + self.eps as f64)?;
        let normed = x.broadcast_div(&scale.sqrt()?)?;
        Ok(normed.broadcast_mul(&self.weight)?)
    }
}

/// Rotary tables, with duplicated halves to match the Python convention.
struct Rope {
    cos: Tensor,
    sin: Tensor,
}

impl Rope {
    fn new(cfg: &ModelConfig, device: &Device) -> Result<Self> {
        let half = cfg.head_dim / 2;
        let inv_freq: Vec<f64> = (0..half)
            .map(|i| 1.0 / (cfg.rope_theta as f64).powf(i as f64 / half as f64))
            .collect();

        let mut cos = Vec::with_capacity(cfg.max_seq_len * cfg.head_dim);
        let mut sin = Vec::with_capacity(cfg.max_seq_len * cfg.head_dim);
        for pos in 0..cfg.max_seq_len {
            // Two passes over inv_freq: the table is duplicated so one cos/sin pair covers
            // both halves of the head.
            for _ in 0..2 {
                for f in &inv_freq {
                    let angle = pos as f64 * f;
                    cos.push(angle.cos() as f32);
                    sin.push(angle.sin() as f32);
                }
            }
        }

        // The shape has to be a tuple. A bare usize builds a rank-1 tensor, and the failure
        // surfaces several operations later as an unhelpfully shaped broadcast.
        Ok(Self {
            cos: Tensor::from_vec(cos, (cfg.max_seq_len, cfg.head_dim), device)?,
            sin: Tensor::from_vec(sin, (cfg.max_seq_len, cfg.head_dim), device)?,
        })
    }

    /// Apply to ``(batch, heads, seq, head_dim)``, with ``offset`` the position of the first
    /// new token in the sequence.
    fn apply(&self, x: &Tensor, offset: usize) -> Result<Tensor> {
        let seq = x.dim(2)?;
        if offset + seq > self.cos.dim(0)? {
            bail!(
                "position {} exceeds max_seq_len {}",
                offset + seq,
                self.cos.dim(0)?
            );
        }

        let (b, h, _, d) = x.dims4()?;

        let cos = self.cos.narrow(0, offset, seq)?;
        let sin = self.sin.narrow(0, offset, seq)?;
        let c = cos.unsqueeze(0)?.unsqueeze(0)?.broadcast_as((b, h, seq, d))?;
        let s = sin.unsqueeze(0)?.unsqueeze(0)?.broadcast_as((b, h, seq, d))?;

        // rotate_half: negate the back half and concatenate.
        let half = d / 2;
        let front = x.narrow(3, 0, half)?;
        let back = x.narrow(3, half, half)?;
        let rotated = Tensor::cat(&[&back.neg()?, &front], 3)?;

        let scaled = x.broadcast_mul(&c)?;
        let rotated = rotated.broadcast_mul(&s)?;
        Ok(scaled.broadcast_add(&rotated)?)
    }
}

/// Grouped-query attention with an optional affective bias.
///
/// The bias is folded into the queries:
///
/// ```text
/// scores = ((q + q @ B_vad) @ k^T) / sqrt(head_dim)
/// ```
///
/// See the Python `attention.py` docstring for why it is not a literal additive term on the
/// score matrix. In short: a bias that is a function of a 3D state cannot be `(seq, seq)`
/// without a `3 x seq x seq` parameter, so it is a per-head `(head_dim, head_dim)` matrix
/// acting in feature space. That costs `O(head_dim)` per head instead of `O(seq)`, and
/// because it does not depend on position, prefill and single-token decode compute identical
/// arithmetic with nothing extra to cache.
struct Attention {
    dim: usize,
    q: Linear,
    k: Linear,
    v: Linear,
    o: Linear,
    affect: Option<Linear>,
    n_heads: usize,
    n_kv_heads: usize,
    head_dim: usize,
    n_rep: usize,
    scale: f64,
    granularity: String,
}

impl Attention {
    fn load(
        vb: &VarBuilder<'_>,
        cfg: &ModelConfig,
        affect: &AffectConfig,
        layer: usize,
    ) -> Result<Self> {
        let p = |name: &str| format!("layers.{layer}.self_attn.{name}");

        // linear_no_bias, not linear. Every projection here is bias-free, and the default
        // helper adds a bias tensor, so the load fails on a tensor that is correctly absent.
        let q = candle_nn::linear_no_bias(cfg.dim, cfg.q_dim(), vb.pp(p("q_proj")))?;
        let k = candle_nn::linear_no_bias(cfg.dim, cfg.kv_dim(), vb.pp(p("k_proj")))?;
        let v = candle_nn::linear_no_bias(cfg.dim, cfg.kv_dim(), vb.pp(p("v_proj")))?;
        let o = candle_nn::linear_no_bias(cfg.q_dim(), cfg.dim, vb.pp(p("o_proj")))?;

        let affect_linear = if affect.enabled {
            let out_dim = affect.n_bias(cfg) * cfg.head_dim * cfg.head_dim;
            let state_dim = affect.state_feature_dim();
            let bias =
                candle_nn::linear_no_bias(state_dim, out_dim, vb.pp(p("affect.state_proj")))
                    .with_context(|| {
                        format!(
                            "loading {}. The config says affect.enabled is true, so this \
                             tensor is required; an Elafry checkpoint always has one.",
                            p("affect.state_proj")
                        )
                    })?;
            Some(bias)
        } else {
            None
        };

        Ok(Self {
            dim: cfg.dim,
            q,
            k,
            v,
            o,
            affect: affect_linear,
            n_heads: cfg.n_heads,
            n_kv_heads: cfg.n_kv_heads,
            head_dim: cfg.head_dim,
            n_rep: cfg.n_rep(),
            scale: 1.0 / (cfg.head_dim as f64).sqrt(),
            granularity: affect.bias_granularity.clone(),
        })
    }

    fn split_heads(&self, x: &Tensor, n_heads: usize) -> Result<Tensor> {
        let (b, s, _) = x.dims3()?;
        let out = x.reshape((b, s, n_heads, self.head_dim))?.transpose(1, 2)?;
        Ok(out.contiguous()?)
    }

    /// Expand KV heads to query heads. The ordering is the point: each KV head's *contiguous*
    /// block of `n_rep` query heads must receive the same keys. Getting this wrong produces a
    /// model that trains and generates fluent nonsense.
    fn expand_kv(&self, x: &Tensor) -> Result<Tensor> {
        if self.n_rep == 1 {
            return Ok(x.clone());
        }
        let (b, _, s, d) = x.dims4()?;
        let out = x
            .unsqueeze(2)?
            .expand((b, self.n_kv_heads, self.n_rep, s, d))?
            .reshape((b, self.n_heads, s, d))?;
        Ok(out.contiguous()?)
    }

    fn apply_bias(&self, q: &Tensor, state: Option<&Tensor>) -> Result<Tensor> {
        let (linear, state) = match (&self.affect, state) {
            (Some(l), Some(s)) => (l, s),
            _ => return Ok(q.clone()),
        };

        let (b, _, _, _) = q.dims4()?;
        let n_bias = if self.granularity == "kv" {
            self.n_kv_heads
        } else {
            self.n_heads
        };

        // (B, state_dim) -> (B, n_bias, d, d) -> (B, n_heads, d, d)
        let raw = linear.forward(state)?;
        let bias = raw
            .reshape((b, n_bias, self.head_dim, self.head_dim))?
            .contiguous()?;

        let bias = if n_bias == self.n_heads {
            bias
        } else {
            bias.unsqueeze(2)?
                .expand((b, n_bias, self.n_rep, self.head_dim, self.head_dim))?
                .reshape((b, self.n_heads, self.head_dim, self.head_dim))?
                .contiguous()?
        };

        let folded = q.matmul(&bias)?;
        Ok(q.broadcast_add(&folded)?)
    }

    fn forward(
        &self,
        x: &Tensor,
        rope: &Rope,
        past: Option<&KVEntry>,
        state: Option<&Tensor>,
    ) -> Result<(Tensor, Option<KVEntry>)> {
        let past_len = match past {
            Some(entry) => entry.k.dim(2)?,
            None => 0,
        };

        let q = self.split_heads(&self.q.forward(x)?, self.n_heads)?;
        let k = self.split_heads(&self.k.forward(x)?, self.n_kv_heads)?;
        let v = self.split_heads(&self.v.forward(x)?, self.n_kv_heads)?;

        let q = rope.apply(&q, past_len)?;
        let k = rope.apply(&k, past_len)?;

        let present = match past {
            Some(p) => KVEntry {
                k: Tensor::cat(&[&p.k, &k], 2)?,
                v: Tensor::cat(&[&p.v, &v], 2)?,
            },
            None => KVEntry {
                k: k.clone(),
                v: v.clone(),
            },
        };

        let k = self.expand_kv(&present.k)?;
        let v = self.expand_kv(&present.v)?;

        let q = self.apply_bias(&q, state)?;
        let (b, _, seq, _) = q.dims4()?;

        let scores = q.matmul(&k.transpose(2, 3)?)?.affine(self.scale, 0.)?;
        let scores = self.mask(scores, past_len)?;

        // Softmax over the *key* axis. scores is (batch, heads, queries, keys), so that is
        // dim 3. Normalising dim 2 instead normalises across queries, which is meaningless:
        // every query row would then sum to 1 over the queries rather than over the keys it
        // attends to. It also fails silently at sequence length 1, where dim 2 has size 1 and
        // softmax is the identity, so a single-token test passes and nothing else does.
        let attn = candle_nn::ops::softmax(&scores, 3)?;
        let out = attn.matmul(&v)?;

        // Back to (batch * seq, n_heads * head_dim) for the output projection, which takes a
        // rank-2 input. Then back to rank 3, because the block's residual add needs the batch
        // axis. Neither shape is inferable from the other.
        let out = out.transpose(1, 2)?.contiguous()?;
        let out = out.reshape((b * seq, self.n_heads * self.head_dim))?;
        let projected = self.o.forward(&out)?;
        let projected = projected.reshape((b, seq, self.dim))?;

        Ok((projected, Some(present)))
    }

    /// Causal mask over absolute positions.
    ///
    /// Only applied when a single forward attends to more than one new position. A lone token
    /// against a full cache is already causal, and masking it every step costs a `seq x total`
    /// allocation per token for nothing.
    fn mask(&self, scores: Tensor, past_len: usize) -> Result<Tensor> {
        let seq = scores.dim(2)?;
        if seq <= 1 {
            return Ok(scores);
        }

        let _total = scores.dim(3)?;
        let total = scores.dim(3)?;
        // A large *finite* floor rather than f32::NEG_INFINITY. Adding -inf to the logits and
        // then softmaxing depends on how the softmax treats non-finite inputs, and here it
        // does not produce the exact zeros it should: an unmasked single-token forward matches
        // the cached path to the bit, while the masked one drifts by about 0.08 in the logit.
        // exp of a very negative finite value underflows to exactly 0.0, which is what we want
        // and is guaranteed.
        const MASK_FLOOR: f32 = -1e30;
        let mut mask = vec![MASK_FLOOR; seq * total];
        for i in 0..seq {
            let q_pos = past_len + i;
            for j in 0..total {
                if j <= q_pos {
                    mask[i * total + j] = 0.0;
                }
            }
        }
        let mask = Tensor::from_vec(mask, (seq, total), scores.device())?;
        Ok(scores.broadcast_add(&mask)?)
    }
}

struct Block {
    input_layernorm: RmsNorm,
    attention: Attention,
    post_attention_layernorm: RmsNorm,
    gate: Linear,
    up: Linear,
    down: Linear,
}

impl Block {
    fn load(
        vb: &VarBuilder<'_>,
        cfg: &ModelConfig,
        affect: &AffectConfig,
        layer: usize,
    ) -> Result<Self> {
        // The three SwiGLU matrices. The on-disk names carry the _proj suffix the exporter
        // adds, matching every Llama-derived checkpoint.
        let mlp = |name: &str| -> Result<Linear> {
            let (in_dim, out_dim) = if name == "down_proj" {
                (cfg.ff_dim, cfg.dim)
            } else {
                (cfg.dim, cfg.ff_dim)
            };
            Ok(candle_nn::linear_no_bias(
                in_dim,
                out_dim,
                vb.pp(format!("layers.{layer}.mlp.{name}")),
            )?)
        };

        Ok(Self {
            input_layernorm: RmsNorm::load(
                vb,
                &format!("layers.{layer}.input_layernorm"),
                cfg.dim,
                cfg.rms_eps,
            )?,
            attention: Attention::load(vb, cfg, affect, layer)?,
            post_attention_layernorm: RmsNorm::load(
                vb,
                &format!("layers.{layer}.post_attention_layernorm"),
                cfg.dim,
                cfg.rms_eps,
            )?,
            gate: mlp("gate_proj")?,
            up: mlp("up_proj")?,
            down: mlp("down_proj")?,
        })
    }

    fn forward(
        &self,
        x: &Tensor,
        rope: &Rope,
        past: Option<&KVEntry>,
        state: Option<&Tensor>,
    ) -> Result<(Tensor, Option<KVEntry>)> {
        let residual = x.clone();
        let h = self.input_layernorm.forward(x)?;
        let (h, present) = self.attention.forward(&h, rope, past, state)?;
        let x = residual.add(&h)?;

        let residual = x.clone();
        let h = self.post_attention_layernorm.forward(&x)?;
        let gated = candle_nn::ops::silu(&self.gate.forward(&h)?)?;
        let up = self.up.forward(&h)?;
        let h = self.down.forward(&gated.mul(&up)?)?;
        let x = residual.add(&h)?;

        Ok((x, present))
    }
}

pub struct Elafry {
    pub cfg: ModelConfig,
    /// Kept for callers that want to report or override the affective settings. The forward
    /// pass reads them at construction time, into the per-layer bias modules.
    #[allow(dead_code)]
    pub affect: AffectConfig,
    embed: Embedding,
    layers: Vec<Block>,
    norm: RmsNorm,
    lm_head: Option<Linear>,
    rope: Rope,
    device: Device,
}

impl Elafry {
    pub fn load(
        vb: VarBuilder<'_>,
        cfg: ModelConfig,
        affect: AffectConfig,
        device: Device,
    ) -> Result<Self> {
        cfg.validate()?;

        let embed = candle_nn::embedding(cfg.vocab_size, cfg.dim, vb.pp("embed_tokens"))?;

        let mut layers = Vec::with_capacity(cfg.n_layers);
        for i in 0..cfg.n_layers {
            layers.push(
                Block::load(&vb, &cfg, &affect, i).with_context(|| format!("loading layer {i}"))?,
            );
        }

        let norm = RmsNorm::load(&vb, "norm", cfg.dim, cfg.rms_eps)?;
        let rope = Rope::new(&cfg, &device)?;

        // Tied by default: the embedding matrix doubles as the output projection, saving
        // vocab_size * dim parameters. The 128k-vocab presets untie.
        let lm_head = if cfg.tie_embeddings {
            None
        } else {
            Some(candle_nn::linear_no_bias(
                cfg.dim,
                cfg.vocab_size,
                vb.pp("lm_head"),
            )?)
        };

        Ok(Self {
            cfg,
            affect,
            embed,
            layers,
            norm,
            lm_head,
            rope,
            device,
        })
    }

    /// Load straight from a checkpoint directory. Fails early and legibly on a mismatch.
    pub fn from_dir(dir: &std::path::Path, device: Device) -> Result<Self> {
        let cfg = crate::config::Config::load_from_dir(dir)?;
        cfg.check_weights(dir)?;

        let vb = unsafe {
            VarBuilder::from_mmaped_safetensors(&[dir.join(WEIGHTS_FILENAME)], DType::F32, &device)?
        };

        Self::load(vb, cfg.model, cfg.affect, device)
    }

    fn head(&self, h: &Tensor) -> Result<Tensor> {
        // Linear takes a rank-2 input, so the batch and sequence axes fold together. The
        // result goes back to rank 3, and the last axis is the *vocabulary*, not the hidden
        // size the input arrived with.
        let (b, s, _) = h.dims3()?;
        let flat = h.reshape((b * s, self.cfg.dim))?;
        let out = match &self.lm_head {
            Some(head) => head.forward(&flat)?,
            // Tied: the output projection is the embedding matrix.
            None => flat.matmul(&self.embed.embeddings().t()?)?,
        };
        Ok(out.reshape((b, s, self.cfg.vocab_size))?)
    }

    /// Forward pass over a full sequence. Returns logits ``(1, seq, vocab)``.
    pub fn forward(&self, input_ids: &Tensor, state: Option<&Tensor>) -> Result<Tensor> {
        let (b, _seq) = input_ids.dims2()?;
        if b != 1 {
            bail!("forward expects batch size 1, got {b}");
        }

        let mut x = self.embed.forward(input_ids)?;
        for layer in &self.layers {
            let (out, _) = layer.forward(&x, &self.rope, None, state)?;
            x = out;
        }
        let x = self.norm.forward(&x)?;
        self.head(&x)
    }

    /// Prefill: run the prompt, keep the cache, return the logits at the final position.
    pub fn prefill(&self, input_ids: &Tensor, state: Option<&Tensor>) -> Result<(Tensor, KVCache)> {
        let (_, seq) = input_ids.dims2()?;
        if seq == 0 {
            bail!("cannot prefill an empty prompt");
        }

        let mut x = self.embed.forward(input_ids)?;
        let mut cache: KVCache = Vec::with_capacity(self.layers.len());
        for layer in &self.layers {
            let (out, present) = layer.forward(&x, &self.rope, None, state)?;
            x = out;
            match present {
                Some(kv) => cache.push(kv),
                None => bail!("prefill produced no cache entry"),
            }
        }

        // narrow, not i(): indexing with a scalar drops the axis and hands RMSNorm a rank-2
        // tensor, which then cannot reduce over its feature axis.
        let last = self.norm.forward(&x.narrow(1, seq - 1, 1)?)?;
        let last = last.reshape((1, 1, self.cfg.dim))?;
        Ok((self.head(&last)?, cache))
    }

    /// One decode step against a cache. Returns the logits at the new position.
    pub fn step(
        &self,
        input_ids: &Tensor,
        cache: &mut KVCache,
        state: Option<&Tensor>,
    ) -> Result<Tensor> {
        let (_, seq) = input_ids.dims2()?;
        // An empty cache is accepted and treated as a single-token prefill, so a caller can
        // step from the very first token without special-casing the start of a sequence.
        if !cache.is_empty() && cache.len() != self.layers.len() {
            bail!("cache has {} layers, model has {}", cache.len(), self.layers.len());
        }

        let mut x = self.embed.forward(input_ids)?;
        for (i, layer) in self.layers.iter().enumerate() {
            // An empty cache means the first token, so there is nothing to attend back to.
            let past = cache.get(i).cloned();
            let (out, present) = layer.forward(&x, &self.rope, past.as_ref(), state)?;
            x = out;
            if let Some(kv) = present {
                if i < cache.len() {
                    cache[i] = kv;
                } else {
                    cache.push(kv);
                }
            }
        }

        // narrow, not i(): indexing with a scalar drops the axis and hands RMSNorm a rank-2
        // tensor, which then cannot reduce over its feature axis.
        let last = self.norm.forward(&x.narrow(1, seq - 1, 1)?)?;
        let last = last.reshape((1, 1, self.cfg.dim))?;
        self.head(&last)
    }

    /// Sample ``max_new_tokens`` continuations.
    pub fn generate(
        &self,
        prompt: &[u32],
        params: &SamplingParams,
        state: Option<&[f32; 3]>,
    ) -> Result<(Vec<u32>, [f32; 3])> {
        let budget = self.cfg.max_seq_len.saturating_sub(params.max_new_tokens);
        let start = prompt.len().saturating_sub(budget);
        let prompt = &prompt[start..];
        if prompt.is_empty() {
            bail!("empty prompt after context truncation");
        }

        let state_t = match state {
            Some(s) => Some(Tensor::from_vec(s.to_vec(), (1, 3), &self.device)?),
            None => None,
        };
        let ids = Tensor::from_vec(prompt.to_vec(), (1, prompt.len()), &self.device)?;

        let (mut logits, mut cache) = self.prefill(&ids, state_t.as_ref())?;
        let mut cur_state = state.copied().unwrap_or([0.0, 0.0, 0.0]);
        let mut out: Vec<u32> = Vec::with_capacity(params.max_new_tokens);

        for step in 0..params.max_new_tokens {
            let mut sampler = params.sampler(step);
            let next = sampler.sample(&logits, params)?;
            out.push(next);

            if params.eos_id == Some(next) || step + 1 == params.max_new_tokens {
                break;
            }

            // `--set-vad` pins the state. Without a pin this crate does not run the internal
            // state predictor, so the state stays where prefill put it.
            if let Some(s) = state {
                cur_state = *s;
            }

            let state_arg = Tensor::from_vec(cur_state.to_vec(), (1, 3), &self.device)?;
            let next_ids = Tensor::from_vec(vec![next], (1, 1), &self.device)?;
            logits = self.step(&next_ids, &mut cache, Some(&state_arg))?;
        }

        Ok((out, cur_state))
    }

    pub fn device(&self) -> &Device {
        &self.device
    }
}

/// Compare a cached decode against full prefill, position by position.
///
/// This catches the failures that are otherwise invisible: correct shapes, fluent text, wrong
/// distribution. The Python suite runs the same check in `tests/test_kv_cache.py`; running it
/// here proves the two implementations agree.
pub fn verify_cache(
    model: &Elafry,
    prompt: &[u32],
    state: Option<&[f32; 3]>,
    tolerance: f64,
) -> Result<(f64, usize)> {
    let seq = prompt.len();
    if seq == 0 {
        bail!("--verify-cache needs a non-empty prompt");
    }

    let state_t = match state {
        Some(s) => Some(Tensor::from_vec(s.to_vec(), (1, 3), model.device())?),
        None => None,
    };
    let ids = Tensor::from_vec(prompt.to_vec(), (1, seq), model.device())?;
    let full = model.forward(&ids, state_t.as_ref())?;

    let _cache = KVCache::new();
    let mut cache = KVCache::new();
    let mut worst = 0.0f64;
    let mut worst_at = 0usize;

    for i in 0..seq {
        let step_ids = Tensor::from_vec(vec![prompt[i]], (1, 1), model.device())?;
        let logits = model.step(&step_ids, &mut cache, state_t.as_ref())?;
        let got = logits.i((0, 0))?.to_vec1::<f32>()?[0];
        let want = full.i((0, i))?.to_vec1::<f32>()?[0];
        let _diff = (got - want).abs() as f64;
        let diff = (got - want).abs() as f64;
        // Per-position trace, off unless asked for. The worst-case number alone does not say
        // whether the error appears at one token or accumulates, and those have different
        // causes.
        if std::env::var("ELAFRY_TRACE").is_ok() {
            println!("  pos {i:>3}: got {got:+.6} want {want:+.6} diff {diff:.3e}");
        }
        if diff > worst {
            worst = diff;
            worst_at = i;
        }
    }

    if worst > tolerance {
        bail!(
            "cached decode diverged from prefill by {worst:.3e} (tolerance {tolerance:.1e}).\n  \
             Worst at position {worst_at} of {seq}.\n  \
             A mismatch at position 0 means the cache is wrong from the first token; a later \
             one means the cache or the mask drifts."
        );
    }
    Ok((worst, seq))
}

/// Compare this engine's logits against a reference dump written by Python.
///
/// The two implementations exist to be interchangeable and nothing about writing both
/// guarantees they are. This is the check.
pub fn verify_parity(
    model: &Elafry,
    reference: &std::path::Path,
    prompt: &[u32],
    state: Option<&[f32; 3]>,
    tolerance: f64,
) -> Result<f64> {
    #[derive(serde::Deserialize)]
    struct Dump {
        tokens: Vec<u32>,
        #[serde(default)]
        state: Option<Vec<f32>>,
        /// (position, token_id, logit)
        topk: Vec<(usize, usize, f32)>,
    }

    let text = std::fs::read_to_string(reference)
        .with_context(|| format!("reading {}", reference.display()))?;
    let dump: Dump = serde_json::from_str(&text).context("parsing the reference dump")?;

    if dump.tokens != prompt {
        bail!(
            "the reference dump was produced for a different prompt ({} tokens vs {} given)",
            dump.tokens.len(),
            prompt.len()
        );
    }
    if dump.topk.is_empty() {
        bail!("the reference dump contains no logits to compare");
    }

    let use_state: Option<[f32; 3]> = match &dump.state {
        Some(v) if v.len() == 3 => Some([v[0], v[1], v[2]]),
        Some(_) => bail!("the reference dump has a state that is not three components"),
        None => state.copied(),
    };
    let state_t = match use_state {
        Some(s) => Some(Tensor::from_vec(s.to_vec(), (1, 3), model.device())?),
        None => None,
    };

    let ids = Tensor::from_vec(prompt.to_vec(), (1, prompt.len()), model.device())?;
    let logits = model.forward(&ids, state_t.as_ref())?;

    let mut worst = 0.0f64;
    for (position, token_id, expected) in &dump.topk {
        if *position >= prompt.len() {
            bail!(
                "the dump references position {} but the prompt has {} tokens",
                position,
                prompt.len()
            );
        }
        if logits.rank() != 3 {
            bail!(
                "forward returned rank {} (shape {:?}); the dump expects (1, seq, vocab)",
                logits.rank(),
                logits.dims()
            );
        }
        // A three-index read yields a rank-0 scalar, and to_vec1 requires rank 1.
        let actual = logits.i((0, *position, *token_id))?.to_scalar::<f32>()?;
        worst = worst.max((actual - expected).abs() as f64);
    }

    if worst > tolerance {
        bail!(
            "Rust and Python disagree by {worst:.3e} (tolerance {tolerance:.1e}) over {} \
             checked logits. The two implementations are not interchangeable.",
            dump.topk.len()
        );
    }
    Ok(worst)
}
