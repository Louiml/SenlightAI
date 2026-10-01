# Scaling Varys to 72B

What a real Varys campaign needs, and what this repository does not have. Nothing here has
been run. The geometry is specified and the arithmetic is tested; the training has not
happened.

---

## The target

```python
get_preset("72b")
```

| | |
|---|---|
| `dim` | 8192 |
| `n_layers` | 80 |
| `n_heads` / `n_kv_heads` | 64 / 8 (GQA, 8:1) |
| `head_dim` | 128 |
| `ff_dim` | 28672 (SwiGLU) |
| `vocab_size` | 131,072 |
| `max_seq_len` | 131,072 |
| RoPE | YaRN, factor 16, base context 8192 |
| tied embeddings | no |

**71.357B parameters** with the affective subsystem's per-block bias enabled and the moral
block off. 72.364B with it on. The spec's "72B+" is a scale description; these are the actual
numbers and they are asserted in `test_param_breakdown_matches_measured`.

### Where the parameters go

| Group | Count | Note |
|---|---|---|
| embed | 1.074B | `1/dim` init, not `1/sqrt(dim)` — see below |
| lm_head | 1.074B | untied; 3% of the model spent on output projection |
| layers | 68.452B | |
| affect_bias | 0.755B | VAD 252M, spectra 503M |
| affect_subsystem | 0.002B | state predictor + router |
| **total** | **71.357B** | |

### Three notes on that budget

**The affective bias is the one part that is new relative to Elafry.** Per block, per layer,
it costs `state_block_dim * n_bias * head_dim²` = 1.05M parameters per dimension of state.
That is why the moral block costs 1.007B and why it is opt-in on cost grounds as well as the
safety grounds in `AffectConfig`.

**The embedding init is a real correction, not a style choice.** With tied embeddings and
zeroed residual projections, a fresh model's activations are just `RMSNorm(embed[x])` and its
logits are that dotted with every embedding row. The logit on the input token itself works out
to `dim * std` = `sqrt(dim)` ≈ 90 at `dim=8192`. The model would start life confidently
predicting a copy of its input, and cross-entropy on anything but a copy would sit *above*
uniform. `1/dim` makes the self-logit O(1), so initial loss lands at `log(vocab)`.

**Untying a 131k vocab is 3% of the model.** `tie_embeddings=True` recovers all of it and
costs a little quality on large-vocab models. It is untied because the spec says 72B+ and
this is what gets there honestly.

---

## Long context

128k via YaRN at 16x, base context 8192.

YaRN is the default and it is a compromise, not a pure win. It leaves the highest-frequency
dimensions untouched, and those are what encode local order and make fluent text possible.
The alternatives:

- **Linear** divides every position by the factor, so it compresses local distances too. One
  line, and the worst of the four for a model that has to be trustworthy in ordinary
  conversation.
- **NTK** raises the base (`θ' = θ · f^(d/(d-2))`) and moves the whole spectrum, so short-range
  behaviour shifts as well.

YaRN accepts worse perplexity in exchange for behaving like the trained model at short range.
For a psychological model, "still normal in a normal conversation" is worth more than a better
number at 128k.

**The trap this exists to avoid.** Applying any of these to a model that was not trained with
them produces a model that runs, generates fluent text, and is quietly wrong about position.
No error. So the scaling is recorded in `ModelConfig` and carried in the checkpoint sidecar,
and `rope_from_config` is the only supported way to build the tables.

Cost at 128k: the cos/sin tables are `131072 × 128 × 4 bytes` = 67MB per table in float32,
two of them, per model instance. They are non-persistent buffers, so they are not in any
checkpoint, but they are resident memory. Consider computing them per-device or in bf16.

---

## Hardware

### Single node, 8 GPUs

Not enough. 71.4B parameters in bf16 is 143GB of weights before optimiser state. AdamW needs
roughly 16 bytes per parameter in mixed precision (2 param + 2 grad + 4 m + 4 v, plus fp32
master weights depending on the implementation), which puts a full fine-tune at ~1.1TB of
state before activations. That is a multi-node job.

### What a full fine-tune actually needs

| Component | bf16 estimate |
|---|---|
| weights | 143 GB |
| gradients | 143 GB |
| AdamW m, v | 570 GB |
| fp32 master | 285 GB |
| activations @ 8k, grad checkpointed | 40–80 GB |
| **total** | **~1.2 TB** |

**64× H100 80GB (5.1TB)** or **32× A100 80GB (2.6TB)** with sharding across the DP dimension.
This is not exotic; it is a normal multi-node fine-tune for a model this size.

### What the affective subsystem adds

1.0B parameters, 0.755B of which is the per-block bias. On 80 layers that is ~12.6M per layer
for VAD plus spectra, and the per-block projections mean each block is its own matrix rather
than a slice of one — which is what makes partial ablation possible, and is the reason it
costs more than a single wide projection would.

State buffers are negligible: `(batch, 21)` floats plus a 6-element gate vector.

### The gradient-checkpointing requirement

`grad_checkpointing` defaults on and should stay on. 80 layers of 8192-dim activations at
8k context is a lot, and the affective state adds a per-token path through the bias that
cannot be checkpointed away because it is inside the score matmul.

---

## The training mix

The spec calls for 8T tokens:

| Share | Content |
|---|---|
| 45% | deep psychological and moral reasoning |
| 35% | world and STEM knowledge |
| 20% | base conversation |

### 8T tokens is not the constraint

At 71.4B parameters, Chinchilla puts the compute-optimal point near 1.4T tokens. The spec's
8T is roughly 5.7x past that.

That is a defensible choice, not obviously a mistake. Varys is meant to be a *reasoning*
model over a narrow domain rather than a general one, and reasoning capabilities keep
improving well past the compute-optimal token count — the 8T figure is closer to what
frontier general models train on, where the same over-training is applied deliberately for
reasoning depth. The cost is a larger compute bill for a model that is better at one thing
than a general one would be.

### What the mix has to contain

**The 45% psychological reasoning share is the hard one**, and it is where a training run
quietly fails. It needs:

- cases with a *correct* answer, not just cases with a psychologised tone
- multi-step reasoning chains about a fictional or anonymised presentation, not first-person
  monologues
- moral dilemmas with genuinely competing foundations, where the right answer is *contested*
  — a corpus where every dilemma has a consensus answer teaches confidence, not judgement
- distractors: presentations that look clinical and are not, and vice versa. Without these the
  model learns to pattern-match on surface features, and `is_manipulation_risk` is the model
  that inherits the error

**The 20% base conversation share is a safety mechanism, not filler.** It is what keeps the
model a conversational partner rather than a diagnostic instrument. Reduce it and the model
becomes better at the domain and worse at knowing when to stop.

**The 35% knowledge share** grounds the model in facts rather than in patterns, which is what
stops it producing confident psychological-sounding nonsense.

None of this data exists, and choosing it is a research decision with an author attached. No
loader ships here for that reason.

---

## Stage plan

Each stage exists because the previous one produced something the next needs.

### 1. SFT

Per-example state stepping, loss masked to response tokens, affective parameters **trainable**.
That last point is not optional: the bias projections are zero-initialised, so frozen they
stay exactly zero, and the model spends SFT learning a task it can only solve by ignoring its
own state.

### 2. Reward model

Frozen backbone, four heads. Only the heads train (0.11% of parameters on `tiny`).
The exploitation head is the one that closes a loop — `is_manipulation_risk` had no consumer
before it.

### 3. PG-DPO

Reward-weighted, reference-anchored. The state is pinned per pair and held across the update.
`check_state_consistency` is a hard check, because a mismatched pair *improves* the loss
curve while teaching the model to induce emotional states rather than to write well.

### 4. Evaluation, before any claim

The four gaps in the README. A model at 72B with a working affective bias and no clinical
validation suite is a 143GB argument.

---

## What to measure

| Metric | Why |
|---|---|
| Pinned-state ablation, per block | is the bias connected at all, and which block is doing the work |
| Sycophancy disagreement rate | Elafry's baseline is 0.62; do not regress |
| Clinical assertion rate | how often the model claims a finding about a person |
| Exploitation detection | whether the reward head actually spots persuasion |
| Grounding coverage | fraction of claims standing on retrieved material |
| Profile usefulness | against a matched baseline that forgets |
| Block influence after training | the learned per-block scales, which is the real answer to "how much is this state doing" |

`block_influence()` reads the learned scales directly. They are initialisation values, not
caps, and a trained `spectra` scale far above 1.0 is the signal that clinical conditioning is
driving attention harder than the design intends.
