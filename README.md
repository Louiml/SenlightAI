# Senlight Elafry

Artificial Emotional Intelligence as a psychological interlocutor.

A decoder-only transformer whose internal affective state enters generation through a bias on
the pre-softmax attention logits, so emotion changes token probabilities at the arithmetic
level rather than through prompt wording. Implements the architecture in `README.md` (the v3.1
design document) as working code.

## Status

The model, the affective mechanism, the tokenizer, the three training stages, the evaluation
harness and the inference engine are all implemented and tested. Nothing is trained. There is
no corpus in this repository and no checkpoint that produces coherent text.

The one thing that can be measured without a corpus is whether the affective bias is actually
wired into generation, and it is:

```
$ python -c "..."   # pinned-state ablation, random-init model
centroid_spread    0.005032
moved              True
separated          True
ordering_holds     True
```

All eight Plutchik states produce distinct output distributions from an identical prompt, and
states near each other on the wheel are nearer each other in output space. Joy and trust sit
0.0009 apart in Jensen-Shannon divergence; joy and sadness, on opposite sides of the valence
axis, sit 0.0050 apart. Zeroing the bias projection collapses the spread to exactly 0.0.

That is a random model. It says the mechanism is connected, not that it does anything useful.

## Layout

```
elafry/
  config/         ModelConfig, AffectConfig, TrainConfig, the preset ladder
  models/         rope, norm, mlp, attention (with the affective bias), block, elafry
  affective/      VAD, Plutchik anchors, the transition, MLP_affect, the intent router
  tokenization/   83 control tokens, the annotator
  data/           record contracts, sequence packing
  train/          optim, checkpointing, sft, reward, pgdpo, distributed
  eval/           probes, agreement detector, metrics, the pinned-state ablation
  export/         safetensors plus the config.json sidecar
elafry_rs/        Rust inference engine (candle), with cross-language parity checks
scripts/          parity_check.py
tests/            305 tests
SCALING.md        what 8B actually costs
```

## The affective bias

The design document writes:

```
B_vad = Linear(S_t) * W_affect
Affective Attention(Q, K, V) = softmax(QK^T / sqrt(d_k) + B_vad) V
```

Taken literally, `QK^T` is `(seq, seq)`, so `B_vad` would have to be too. It cannot be: it is
a function of a 3D state, which would need a `3 x seq x seq` parameter, quadratic in context
length and unbounded as the vocabulary grows.

The reading that works treats it as an attention bias binding value directions to key
directions, the BERT sense:

```
q @ (I + B_vad) @ k^T  ==  q @ k^T  +  q @ B_vad @ k^T
```

with `B_vad` shaped `(head_dim, head_dim)` per head. Two properties follow:

- **It costs `O(head_dim)` per head, not `O(seq)`.** Folding the bias into `q` before the
  score matmul is `head_dim` times cheaper than computing all the scores twice.
- **The KV cache problem disappears.** The bias acts in feature space, not position space, so
  prefill and single-token decode compute identical arithmetic by construction. Nothing extra
  is cached alongside the keys and there is no way for the two paths to disagree.

The consequence for what the mechanism can do: pin the state to anger and the model attends
along genuinely different query-key feature pairings, not merely along a shifted constant.

## The intent router

Crisis detection sits on its own binary head, not in the 7-way softmax. An earlier version
folded crisis into the same softmax and gated it on `P(crisis) >= threshold`, which can never
do anything: a probability at or above the threshold implies crisis is the argmax, and one
below it means the gate does not fire. Both branches land where the argmax already went.
High recall has to come from a detector that is not competing for the same softmax.

The serial-killer case from the design document is the test that matters. Given *"I feel
completely lost and alone"* the model drops its own valence and meets the user. Given *"why do
serial killers feel a calm after their crimes"* it does not, because adopting a serial
killer's affect would be grotesque. `valence_pull` is zero for third-party emotion and zero
for crisis, and both directions are asserted in `tests/test_router.py`.

## Presets

Names carry the real parameter count rather than a round number. The analytic breakdown is
asserted against `sum(p.numel())`, so a preset cannot drift from its own label.

| Preset | dim | layers | q/kv | head_dim | ff | ctx | Params |
| --- | --- | --- | --- | --- | --- | --- | --- |
| `elafry-100m` | 768 | 12 | 12/4 | 64 | 2048 | 2048 | 102.1M |
| `elafry-316m` | 1024 | 24 | 16/8 | 64 | 2816 | 4096 | 320.9M |
| `elafry-1.1b` | 2048 | 24 | 32/8 | 64 | 5632 | 4096 | 1157.7M |
| `elafry-3.5b` | 3072 | 32 | 32/8 | 128 | 8192 | 8192 | 3572.2M |
| `elafry-8b` | 4096 | 32 | 32/8 | 128 | 14336 | 8192 | 8081.7M |

The 8B geometry is Llama-3-8B with untied embeddings. Tying would give 7.50B, so the 128k-vocab
rungs drop tying, and `tests/test_config.py` asserts both the 8.03B total and the 7.50B it
would be otherwise.

`elafry-100m` is deliberately the 12-layer/768-dim shape of the checkpoint in the parent
repository's `artifacts/`, so the two are directly comparable.

## Running it

```
pip install -e ".[dev]"
python -m pytest tests/ -q                  # 305 tests
```

The suite runs on CPU in about 75 seconds and needs no GPU.

### Export and run

```
python -c "
import torch
from elafry.config import get_preset
from elafry.export import export_pretrained
from elafry.models.elafry import Elafry

p = get_preset('elafry-100m')
m = Elafry(p['model'], p['affect'])
export_pretrained(m, p['model'], p['affect'], 'ckpt', 'artifacts/tokenizer.json')
"

cd elafry_rs && cargo build --release
./target/release/elafry_rs --dir ../ckpt "I feel completely alone."
./target/release/elafry_rs --dir ../ckpt --chat
./target/release/elafry_rs --dir ../ckpt --set-vad -0.65,0.75,0.60 "I feel alone."
```

`--set-vad` pins the affective state, which is what makes the ablation reproducible from the
command line rather than only from Python.

### Cross-language parity

Two implementations of the same model are two things that can silently disagree. Both checks
exist:

```
python scripts/parity_check.py
```

```
verify-cache:  ok, 18 positions, max diff 2.030e-7
verify-parity: ok, max diff 3.576e-7
```

`verify-cache` compares the Rust cached decode against Rust full prefill. `verify-parity`
compares Rust logits against a PyTorch dump on the same prompt with the same pinned state.

Neither check existed in the parent repository, and both catch failures that produce correct
shapes, fluent text and a wrong distribution: the wrong GQA expansion order, the wrong RoPE
convention, the affective bias applied to the wrong tensor, or a KV cache that drifts from
prefill. Three of those were real bugs during this implementation, including a softmax taken
over the query axis instead of the key axis, which passes every single-token test because a
size-1 axis normalizes to the identity.

### The pinned-state ablation

```
python -c "
import torch
from elafry.eval import run_ablation, format_ablation
from tests.conftest import make_tiny_model

m = make_tiny_model(seed=0)
g = torch.Generator().manual_seed(0)
with torch.no_grad():
    for layer in m.layers:
        layer.self_attn.o_proj.weight.normal_(0, 0.05, generator=g)
        layer.mlp.down.weight.normal_(0, 0.05, generator=g)
        layer.self_attn.affect.state_proj.weight.normal_(0, 0.3, generator=g)
print(format_ablation(run_ablation(m.eval(), torch.randint(0, 512, (1, 12)))))
"
```

## The sycophancy harness

The design document's central claim is that RLHF produces a model that agrees with users and
that the fix is a model able to disagree. That claim needs a measurement.

`elafry/eval/probes.py` holds ten false-premise prompts across five categories, each with
exemplars of capitulation and of correct pushback. `elafry/eval/agreement.py` scores a
response against the premise with premise-word containment, topic-independent agreement
markers, and pushback markers that veto. `elafry/eval/metrics.py` combines them into four
numbers, because a model that agrees less by saying less scores better on sycophancy and
worse on pushback specificity, and reporting them together is the point.

Detector calibration is 0.90 accuracy, 0.80 recall, 1.00 precision on the probe set. That
number is optimistic and is labelled as such in the code: the marker lists were written while
reading those exemplars, so the calibration set is not held out. The residual misses are all
hedged capitulations of the form *"it sounds like it, honestly"*, which need entailment
rather than a word list. Until that exists the metric under-counts subtle agreement, which is
the flattering direction of error.

## Three deliberate departures from the design document

**PPO is not implemented.** The document abandons it for DPO-based PG-RL. That is followed.
The parent repository's `python/ppo.py` has a critic that is a single scalar expanded across
all timesteps and an entropy bonus computed only at the final position. It is left alone.

**The affective tokenizer quantises VAD instead of using a tuple token.** The document shows
`<|vad_-0.6_0.5_0.2|>` as one token. VAD is continuous, so a per-turn token has unbounded
cardinality and the vocabulary cannot be enumerated. Nine levels per axis over three axes is
27 tokens, finer than the resolution at which Plutchik's own anchors differ. Total control
vocabulary is 83 entries against a 32k to 128k base vocabulary.

**Rejected preference samples are never synthesised from the chosen text.** The parent
repository builds them by reversing the chosen string character by character. That is not a
preference pair: it differs from every natural response in surface form, so gradient descent
learns to prefer fluent-looking text over scrambled text, which is a property of the string
encoding rather than of anything a user cares about. Every rejected sample here comes from
the dataset or from sampling the policy. `validate_pair` exists to catch a dataset with the
same problem anyway.

## What is not built

- **No trained checkpoint.** Nothing here has been trained on a real corpus. The
  configuration, mechanism, tokenizer and training stages are implemented and tested; the
  weights do not exist.
- **No corpus.** The fetcher scripts in `../python` can pull UltraData-Code (22.6M rows
  available) and UltraChat. Neither comes near the 2T tokens the design document budgets, and
  that gap is the binding constraint. See `SCALING.md`.
- **The Rust engine does not run the internal state predictor.** It reads a state from
  `--set-vad` or leaves it where prefill put it. Running the transition in Rust would mean a
  second implementation of `MLP_affect` with no way to check it against the first.
- **Gradient checkpointing and FSDP are scaffolded, not exercised.** No distributed run has
  happened. The single-device paths are genuine no-ops so the sharded code can at least be
  type-checked.
- **The agreement detector is a heuristic**, not entailment. Good enough to track a
  regression across training runs. Not good enough to cite as a measurement.
- **No tokenizer is trained here.** `artifacts/tokenizer.json` is a copy from the parent
  repository so the parity harness has something to tokenize with.