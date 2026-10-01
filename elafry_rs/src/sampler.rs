//! Sampling: temperature, then top-k, then top-p.
//!
//! The order matters. Top-k first shrinks the candidate set, so the nucleus computation runs
//! over k entries instead of the whole vocabulary. At the 8B preset's 128k vocabulary, doing
//! it the other way round means sorting 128k floats per generated token.
//!
//! The sibling engine supported only temperature and top-k, over an unseeded thread-local RNG,
//! so the same prompt gave different text on every run. That makes an evaluation
//! irreproducible, so this takes a seed.

use anyhow::{bail, Result};
use candle_core::Tensor;
use rand::rngs::StdRng;
use rand::{Rng, SeedableRng};

#[derive(Debug, Clone)]
pub struct SamplingParams {
    pub max_new_tokens: usize,
    pub temperature: f64,
    pub top_k: usize,
    pub top_p: f64,
    pub eos_id: Option<u32>,
    pub seed: Option<u64>,
}

impl Default for SamplingParams {
    fn default() -> Self {
        Self {
            max_new_tokens: 64,
            temperature: 0.8,
            top_k: 50,
            top_p: 1.0,
            eos_id: None,
            seed: None,
        }
    }
}

impl SamplingParams {
    /// A fresh sampler per step, seeded deterministically from the step index. Reseeding each
    /// step is what stops a single slow start from shaping the whole continuation.
    pub fn sampler(&self, step: usize) -> Sampler {
        Sampler::new(self.seed.map(|s| s.wrapping_add(step as u64)))
    }
}

pub struct Sampler {
    rng: StdRng,
}

impl Sampler {
    pub fn new(seed: Option<u64>) -> Self {
        // A fixed default rather than OS entropy, so an unseeded run is still reproducible.
        // Pass --seed to vary it deliberately.
        Self {
            rng: StdRng::seed_from_u64(seed.unwrap_or(0x5e17)),
        }
    }

    /// Draw one token id from logits `(1, vocab)`.
    pub fn sample(&mut self, logits: &Tensor, params: &SamplingParams) -> Result<u32> {
        if params.temperature <= 0.0 {
            bail!(
                "temperature must be positive, got {}; use --top-k 1 for greedy decoding",
                params.temperature
            );
        }

        // flatten_all, not i((0, ..)): that leaves a rank-2 (1, vocab) and to_vec1 requires
        // rank 1.
        let row = logits.flatten_all()?;
        let scaled = row.affine(1.0 / params.temperature, 0.)?;
        let (ids, probs) = filter(&scaled, params)?;

        // Sample from the renormalised distribution. Renormalising matters: filtering sets
        // excluded logits to -inf, so softmaxing without rescaling leaves the survivors
        // summing to less than one.
        let total: f32 = probs.iter().sum();
        if !(total > 0.0) || !total.is_finite() {
            bail!("post-filter probabilities are degenerate (sum {total})");
        }

        let mut pick = self.rng.gen::<f32>() * total;
        for (i, p) in probs.iter().enumerate() {
            pick -= *p;
            if pick <= 0.0 {
                return Ok(ids[i]);
            }
        }
        Ok(ids[ids.len() - 1])
    }
}

/// Apply top-k then top-p, returning the surviving ``(ids, probabilities)``.
///
/// Only survivors are materialised. Keeping the full vocabulary and masking is simpler but
/// allocates 128k floats per token, which dominates the cost of the sampler at the 8B preset.
fn filter(logits: &Tensor, params: &SamplingParams) -> Result<(Vec<u32>, Vec<f32>)> {
    let vocab = logits.dim(0)?;
    let mut vals: Vec<f32> = logits.to_vec1::<f32>()?;

    // ---- top-k: keep the k highest
    let k = if params.top_k == 0 { vocab } else { params.top_k.min(vocab) };
    if k < vocab {
        let mut indexed: Vec<(usize, f32)> = vals.iter().copied().enumerate().collect();
        indexed.sort_by(|a, b| b.1.partial_cmp(&a.1).unwrap_or(std::cmp::Ordering::Equal));
        let threshold = indexed[k - 1].1;
        for v in vals.iter_mut() {
            if *v < threshold {
                *v = f32::NEG_INFINITY;
            }
        }
    }

    // ---- top-p: keep the shortest prefix whose mass reaches p
    if params.top_p > 0.0 && params.top_p < 1.0 {
        let mut indexed: Vec<(usize, f32)> = vals
            .iter()
            .copied()
            .enumerate()
            .filter(|(_, v)| v.is_finite())
            .collect();
        if indexed.is_empty() {
            bail!("every logit was filtered out");
        }
        indexed.sort_by(|a, b| b.1.partial_cmp(&a.1).unwrap_or(std::cmp::Ordering::Equal));

        let max = indexed[0].1;
        let mut probs: Vec<f32> = indexed.iter().map(|(_, v)| (v - max).exp()).collect();
        let sum: f32 = probs.iter().sum();
        if !(sum > 0.0) {
            bail!("nucleus probabilities are degenerate");
        }
        for p in probs.iter_mut() {
            *p /= sum;
        }

        let mut keep = probs.len();
        let mut cumulative = 0.0f32;
        for (i, p) in probs.iter().enumerate() {
            cumulative += *p;
            if cumulative >= params.top_p as f32 {
                // Keep the token that crosses the threshold and stop. Dropping it is the
                // classic off-by-one, and it silently truncates the nucleus.
                keep = i + 1;
                break;
            }
        }

        let surviving: Vec<usize> = indexed[..keep].iter().map(|(i, _)| *i).collect();
        for (i, v) in vals.iter_mut().enumerate() {
            if !surviving.contains(&i) {
                *v = f32::NEG_INFINITY;
            }
        }
    }

    // ---- softmax over what survived
    let max = vals
        .iter()
        .copied()
        .filter(|v| v.is_finite())
        .fold(f32::NEG_INFINITY, f32::max);
    if !max.is_finite() {
        bail!("no finite logits after filtering");
    }

    let mut ids = Vec::new();
    let mut probs = Vec::new();
    let mut sum = 0.0f32;
    for (i, v) in vals.iter().enumerate() {
        if v.is_finite() {
            let p = (*v - max).exp();
            ids.push(i as u32);
            probs.push(p);
            sum += p;
        }
    }
    if !(sum > 0.0) || !sum.is_finite() {
        bail!("filtered softmax is degenerate");
    }
    for p in probs.iter_mut() {
        *p /= sum;
    }

    Ok((ids, probs))
}