//! Configuration, loaded from the sidecar that travels with the weights.
//!
//! The sibling crate at `../../inference_rs` hardcoded its geometry as a Rust struct. When
//! the checkpoint on disk did not match that struct, the engine exited at startup with no
//! way to override it: no flag, no environment variable, nothing. So this module reads
//! `config.json` instead, and there is exactly one place where the geometry is written down.
//!
//! The format is deliberately flat and versioned. An unknown `format_version` is an error
//! rather than a best-effort parse: silently loading a file written by a future exporter is
//! how you get plausible garbage.

use anyhow::{anyhow, bail, Context, Result};
use serde::Deserialize;
use std::path::{Path, PathBuf};

pub const CONFIG_FILENAME: &str = "config.json";
pub const WEIGHTS_FILENAME: &str = "model.safetensors";
pub const TOKENIZER_FILENAME: &str = "tokenizer.json";
pub const SUPPORTED_FORMAT_VERSION: u32 = 1;

/// Decoder geometry. Field names match `elafry/config/base.py` exactly, because the JSON is
/// written there and read here.
#[derive(Debug, Clone, Deserialize)]
pub struct ModelConfig {
    pub vocab_size: usize,
    pub dim: usize,
    pub n_layers: usize,
    pub n_heads: usize,
    pub n_kv_heads: usize,
    pub head_dim: usize,
    pub ff_dim: usize,
    pub rope_theta: f32,
    pub max_seq_len: usize,
    #[serde(default = "default_eps")]
    pub rms_eps: f32,
    #[serde(default)]
    pub tie_embeddings: bool,
}

fn default_eps() -> f32 {
    1e-5
}

impl ModelConfig {
    /// Query heads per KV head. Turns grouped-query attention into plain MHA.
    pub fn n_rep(&self) -> usize {
        self.n_heads / self.n_kv_heads
    }

    pub fn q_dim(&self) -> usize {
        self.n_heads * self.head_dim
    }

    pub fn kv_dim(&self) -> usize {
        self.n_kv_heads * self.head_dim
    }

    /// Fail here rather than producing a tensor whose shapes do not match the weights.
    pub fn validate(&self) -> Result<()> {
        if self.n_heads == 0 || self.n_kv_heads == 0 {
            bail!("head counts must be non-zero");
        }
        if self.n_heads % self.n_kv_heads != 0 {
            bail!(
                "n_heads ({}) is not a multiple of n_kv_heads ({}); grouped-query attention \
                 needs whole groups",
                self.n_heads,
                self.n_kv_heads
            );
        }
        if self.head_dim % 2 != 0 {
            bail!("head_dim ({}) must be even for rotary embeddings", self.head_dim);
        }
        if self.rope_theta <= 0.0 {
            bail!("rope_theta must be positive, got {}", self.rope_theta);
        }
        Ok(())
    }

    /// Transformer parameters implied by the geometry, for the startup log. Excludes the
    /// affective subsystem, which this crate cannot infer from the config alone.
    pub fn implied_params(&self) -> u64 {
        let d = self.dim as u64;
        let per_layer = d * (self.q_dim() + 2 * self.kv_dim() + self.q_dim()) as u64
            + 3 * d * (self.ff_dim as u64)
            + 2 * d;
        let embed = (self.vocab_size as u64) * d;
        let head = if self.tie_embeddings { 0 } else { embed };
        embed + head + per_layer * (self.n_layers as u64) + d
    }
}

/// The affective subsystem.
///
/// `state_source`, `inertia` and `clamp` are parsed and carried but unused here. They govern
/// the state transition, and this crate deliberately does not implement it: the state is
/// supplied from outside via `--set-vad`. Running the transition in Rust would mean a second
/// implementation of `MLP_affect` with no way to check it against the first, which is the
/// failure mode `elafry_rs` exists to avoid. The fields stay in the struct so a config
/// written by Python round-trips unchanged and a future build can use them.
#[derive(Debug, Clone, Deserialize)]
#[allow(dead_code)]
pub struct AffectConfig {
    #[serde(default)]
    pub enabled: bool,
    #[serde(default = "default_state_source")]
    pub state_source: String,
    #[serde(default = "default_granularity")]
    pub bias_granularity: String,
    #[serde(default = "default_inertia")]
    pub inertia: f32,
    #[serde(default = "default_clamp")]
    pub clamp: f32,
    #[serde(default)]
    pub use_intent: bool,
    #[serde(default)]
    pub use_velocity: bool,
    #[serde(default)]
    pub use_crisis: bool,
}

fn default_state_source() -> String {
    "internal".into()
}
fn default_granularity() -> String {
    "head".into()
}
fn default_inertia() -> f32 {
    0.4
}
fn default_clamp() -> f32 {
    1.0
}

impl Default for AffectConfig {
    fn default() -> Self {
        Self {
            enabled: true,
            state_source: default_state_source(),
            bias_granularity: default_granularity(),
            inertia: default_inertia(),
            clamp: default_clamp(),
            use_intent: false,
            use_velocity: false,
            use_crisis: false,
        }
    }
}

impl AffectConfig {
    /// Width of the state vector fed to the attention bias. Must match the Python side; a
    /// mismatch here shows up as a shape error inside the bias matmul several layers in.
    pub fn state_feature_dim(&self) -> usize {
        let mut n = 3;
        if self.use_intent {
            n += 1;
        }
        if self.use_velocity {
            n += 1;
        }
        if self.use_crisis {
            n += 1;
        }
        n
    }

    pub fn n_bias(&self, cfg: &ModelConfig) -> usize {
        if self.bias_granularity == "kv" {
            cfg.n_kv_heads
        } else {
            cfg.n_heads
        }
    }
}

#[derive(Debug, Clone, Deserialize)]
struct Sidecar {
    format_version: u32,
    model: ModelConfig,
    #[serde(default)]
    affect: AffectConfig,
}

#[derive(Debug, Clone)]
pub struct Config {
    pub model: ModelConfig,
    pub affect: AffectConfig,
}

impl Config {
    pub fn load_from_dir(dir: &Path) -> Result<Self> {
        let path = dir.join(CONFIG_FILENAME);
        if !path.exists() {
            bail!(
                "{} not found in {}.\n\
                 Model geometry travels with the weights. Point the binary at the directory \
                 that contains {}, or re-export the checkpoint from Python.",
                CONFIG_FILENAME,
                dir.display(),
                CONFIG_FILENAME
            );
        }

        let text = std::fs::read_to_string(&path)
            .with_context(|| format!("reading {}", path.display()))?;
        let sidecar: Sidecar = serde_json::from_str(&text)
            .with_context(|| format!("parsing {}", path.display()))?;

        if sidecar.format_version != SUPPORTED_FORMAT_VERSION {
            bail!(
                "checkpoint format version {} is not supported (this build reads {}). \
                 Re-export with the matching Python code.",
                sidecar.format_version,
                SUPPORTED_FORMAT_VERSION
            );
        }

        sidecar.model.validate()?;
        Ok(Self {
            model: sidecar.model,
            affect: sidecar.affect,
        })
    }

    pub fn describe(&self) -> String {
        let m = &self.model;
        format!(
            "{} layers, dim {}, {} q / {} kv heads (n_rep {}), head_dim {}, ff {}, vocab {}, \
             ctx {}, rope_theta {:.0}, tie {}, affect {} ({})",
            m.n_layers,
            m.dim,
            m.n_heads,
            m.n_kv_heads,
            m.n_rep(),
            m.head_dim,
            m.ff_dim,
            m.vocab_size,
            m.max_seq_len,
            m.rope_theta,
            m.tie_embeddings,
            self.affect.enabled,
            self.affect.bias_granularity,
        )
    }

    /// Confirm the geometry matches the weights before doing any real work.
    ///
    /// This is the check the previous engine could not make. A shape mismatch used to
    /// surface as a confusing tensor error several layers in, or as an immediate exit with no
    /// explanation at all.
    pub fn check_weights(&self, dir: &Path) -> Result<()> {
        let path = dir.join(WEIGHTS_FILENAME);
        if !path.exists() {
            bail!("{} not found in {}", WEIGHTS_FILENAME, dir.display());
        }

        let bytes =
            std::fs::read(&path).with_context(|| format!("reading {}", path.display()))?;
        let tensors = safetensors::SafeTensors::deserialize(&bytes)
            .map_err(|e| anyhow!("reading {}: {e}", path.display()))?;

        let names: Vec<&str> = tensors.names();
        if names.is_empty() {
            bail!("{} contains no tensors", WEIGHTS_FILENAME);
        }

        let embed = tensors
            .tensor("embed_tokens.weight")
            .map_err(|_| {
                anyhow!(
                    "{} has no embed_tokens.weight, so it is not an Elafry checkpoint",
                    WEIGHTS_FILENAME
                )
            })?;
        let shape = embed.shape();
        if shape.len() != 2 {
            bail!("embed_tokens.weight has shape {shape:?}, expected 2 dimensions");
        }
        if shape[0] != self.model.vocab_size || shape[1] != self.model.dim {
            bail!(
                "weights say vocab {} dim {}, config says vocab {} dim {}. The config.json \
                 and model.safetensors are from different runs.",
                shape[0],
                shape[1],
                self.model.vocab_size,
                self.model.dim
            );
        }

        // Depth is the other thing that can silently disagree.
        let max_layer = names
            .iter()
            .filter_map(|k| {
                k.strip_prefix("layers.")
                    .and_then(|rest| rest.split('.').next())
                    .and_then(|i| i.parse::<usize>().ok())
            })
            .max();
        match max_layer {
            Some(m) if m + 1 == self.model.n_layers => {}
            Some(m) => bail!(
                "weights contain layers 0..={} ({} layers), config says {}",
                m,
                m + 1,
                self.model.n_layers
            ),
            None => bail!("weights contain no layers.<i>.* tensors; wrong key namespace?"),
        }

        // GQA grouping is the third, and the most expensive to discover late.
        let k_shape = tensors
            .tensor("layers.0.self_attn.k_proj.weight")
            .map_err(|_| {
                anyhow!(
                    "{} has no layers.0.self_attn.k_proj.weight. Keys are expected \
                     un-prefixed; a HuggingFace export has an extra model. on the front.",
                    WEIGHTS_FILENAME
                )
            })?
            .shape()
            .to_vec();
        let expected = self.model.kv_dim();
        if k_shape.first().copied() != Some(expected) {
            bail!(
                "k_proj is {:?} wide, config implies n_kv_heads * head_dim = {}",
                k_shape,
                expected
            );
        }

        Ok(())
    }
}

/// Parse a `--set-vad` argument of the form `v,a,d`.
pub fn parse_vad(spec: &str) -> Result<[f32; 3]> {
    let parts: Vec<&str> = spec.split(',').collect();
    if parts.len() != 3 {
        bail!("expected three comma-separated values like -0.6,0.5,0.2, got {spec:?}");
    }
    let mut out = [0.0f32; 3];
    for (i, p) in parts.iter().enumerate() {
        out[i] = p
            .trim()
            .parse::<f32>()
            .with_context(|| format!("parsing VAD component {p:?}"))?;
        if !(-1.0..=1.0).contains(&out[i]) {
            bail!(
                "VAD component {} is outside [-1, 1]; the convention is the cube [-1,1]^3",
                out[i]
            );
        }
    }
    Ok(out)
}

/// Walk up from the executable looking for a directory that has a config.json.
pub fn default_directory() -> Result<PathBuf> {
    let exe = std::env::current_exe()?;
    let mut dir: &Path = exe.as_path();

    for _ in 0..6 {
        if dir.join(CONFIG_FILENAME).exists() {
            return Ok(dir.to_path_buf());
        }
        match dir.parent() {
            Some(p) => dir = p,
            None => break,
        }
    }

    Err(anyhow!(
        "no {CONFIG_FILENAME} found walking up from {}. Pass the checkpoint directory with \
         --dir.",
        exe.display()
    ))
}