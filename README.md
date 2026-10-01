# Varys

Affective intelligence with longitudinal psychological state, built on the same foundations
as [Elafry](../Elafry) and extended where Elafry's scope stopped.

**Nothing here has been trained. No dataset ships with this repository.** The code is a
complete, tested architecture; the weights do not exist yet. Both facts are load-bearing and
neither is a caveat bolted onto the end — see [What this is not](#what-this-is-not).

---

## What Varys is

Elafry carries a three-dimensional affective state (valence, arousal, dominance) and tracks
it across a conversation. Varys keeps that unchanged and adds three things:

1. **Two more state blocks.** Six HiTOP spectra for psychopathology, six Moral Foundations
   for moral reasoning. This is what turns "dissects complex psychopathology" from a slogan
   into a representation.
2. **Memory across conversations.** A consent-gated, retention-limited profile subsystem.
   A long context is not longitudinal; remembering a person is.
3. **Grounding that cannot fabricate.** Citations are minted only from material retrieval
   actually returned.

| | Elafry | Varys |
|---|---|---|
| Parameters | 8B | 72B+ |
| State dimensions | 3 (VAD) | 21 (VAD + spectra + moral) |
| Memory | within a conversation | across conversations, with consent |
| Context | 8k | 128k (YaRN) |
| Moral reasoning | not represented | 6 foundations, 12 slots |
| Psychopathology | not represented | 6 HiTOP spectra |

---

## The state, in one table

21 dimensions in three blocks. The blocks have different ranges, different decay rates and
different safety properties, and treating them alike would be a category error.

| Block | Dims | Range | Decay | Floor | Default bias scale |
|---|---|---|---|---|---|
| `vad` | 3 | `[-1, 1]` signed | 0.70 | none | **1.0** |
| `spectra` | 6 | `[0, 1]` intensity | 0.94 | 0.05 | **0.1** |
| `moral` | 12 | `[0, 1]` intensity | 0.98 | 0 | **absent** |

**Why three clocks.** Affect is fast and mean-reverting to the origin. A psychopathology
spectrum is not "currently active" — it is a property of a person that expresses in some
blocks and not others, so it decays toward a *baseline* rather than to zero. Moral grounding
is slower and stickier still, because aggressive persuasion of someone's values is one of the
most reliable attacks on a model and a fast-moving moral state is one that can be pushed.

**Why the moral block is 12 dimensions and not 6.** One signed scalar per foundation cannot
express a strong position on *both* poles at once, which is exactly the configuration moral
licensing describes: a person who is non-prejudiced in intent and has still done something
harmful, whose "I am a good person" belief is doing the work. A signed scalar reads as
neutral there.

**Why the moral block is absent by default.** Not a capacity argument — 1.0B parameters is
1.4% of a 72B model, so it would fit. Conditioning attention on someone's moral foundations is
a *persuasion surface*: the attack is not telling them what they want, it is manufacturing the
affect that makes their own values push toward a desired outcome. Enabling it is a decision
somebody has to make having read `is_manipulation_risk`, not a default somebody inherits.

---

## Layout, in dependency order

Subpackages are listed in a topological order of the internal import graph: every entry
depends only on entries above it.

```
varys/
  affective/     1. the state
    vad.py          3 signed dimensions, carried from Elafry unchanged
    plutchik.py     32-node wheel: 8 primaries -> 8 dyads -> 16 tertiaries
    clinical.py     6 HiTOP spectra + co-occurrence graph
    moral.py        12 MFT slots + manipulation risk check
    state.py        composition, transition, evidence gate, feature contract
    encoders.py     the trainable half, the only affective module importing torch
  knowledge/     2. literature grounding and the citation gate. No internal dependencies.
  config/        3. ModelConfig, AffectConfig, TrainConfig, presets
  models/        4. the transformer
    norm.py          RMSNorm
    mlp.py           SwiGLU
    rope.py          rotary embeddings + linear / NTK / YaRN long context
    attention.py     grouped-query attention with multi-axis affective bias
    block.py         one pre-norm transformer block
    state_machine.py the 21-dim recurrence on tensors, with the gate
    router.py        intent routing and clinical response stance
    varys.py         the decoder
  eval/          5. per-block ablation, sycophancy. Needs the model it ablates.
  profile/       6. longitudinal memory. Needs the state, not the model.
    consent.py       scoped, expiring, revocable, enforced
    store.py         versioned, retention-limited, consent-gated
  train/         7. SFT, affective reward model, PG-DPO. Needs everything above.
tests/              236 tests
```

Three things this ordering makes explicit:

- **`eval` precedes `train`.** `varys.train.reward.validate_pair` uses the agreement markers
  from `varys.eval.agreement`, so the reverse order is a cycle.
- **`config` precedes `models`, and `config` imports no torch.** `config` needs the state
  feature width and `models` needs the config, so putting the width next to the encoders would
  mean reading a config drags in the whole tensor stack. That one constraint is what keeps the
  import graph a tree rather than a mesh.
- **`knowledge` depends on nothing internal**, so it is free to sit anywhere. It is placed
  after the state because it is about claims made *about* that state.

Note that this ordering is documentation, not filesystem layout. Git records no directory
order and GitHub's tree view always sorts alphabetically, so a dependency ordering can only
live in docstrings and here.

---

## Three design decisions worth arguing with

### 1. Per-block bias projections, not one wide layer

The obvious change from Elafry is one projection from 21 dimensions instead of 3. That is
wrong for three reasons: the blocks have different safety properties and one projection cannot
hold them at different levels of trust; ablation cannot address them separately; and the
parameter cost is per block, so it should be a visible line item.

The payoff is measurable. `varys.eval.ablation` can zero one block while holding the others
fixed, which distinguishes two failures that look identical in a single number — *the state is
ignored* versus *the state is read but the wrong block is driving it*.

### 2. Clinical state changes the model's process, never its claims

`clinical_stance` is non-parametric and has no learned parameters that can be trained toward a
target. Every branch it can choose is good practice regardless of who is speaking: be slower,
ask rather than assert, name the limits of a conversation, don't problem-solve someone in
crisis. There is no branch that says "call them disordered", and no branch whose output could
be quoted back at them.

Routing response style on an inferred spectrum would mean the model behaves *differently
toward someone* on the strength of its own inference about their mental health, with no way
for them to know and no way to contest it. That is the mechanism by which an internal
representation becomes a change in how it treats a human, and the refusal is in the code
rather than in a policy document.

### 3. A profile is a state trajectory, not a log of what someone said

21-dim aggregates with a count and a timestamp. No transcripts, no quotes, no content the
person said.

A store of raw text is a store of things someone said in a moment of distress and would never
choose to read back to themselves. A store of aggregates can be wrong, can be incomplete, and
can be deleted. The cost is that a profile cannot quote, and that is the right cost.

---

## Usage

```python
import torch
from varys.config import get_preset, AffectConfig
from varys.models import Varys

model = Varys(get_preset("tiny"), AffectConfig())
model.reset_state(1)

out = model(torch.randint(0, 1024, (1, 64)))
out.state          # (B, 21), feed into the next turn
out.next_token_logits
```

Longitudinal memory, with consent enforced in the store rather than by the caller:

```python
from varys.profile import ConsentLedger, ProfileStore, pseudonymise

ledger = ConsentLedger()
store  = ProfileStore(ledger, root=profiles_dir)
pid    = pseudonymise(user_id, server_side_salt)   # no identifier on disk

store.observe(pid, out.state[0].tolist(), scope="affect")   # raises: no consent
ledger.grant("affect", days=30, purpose="longitudinal support")
store.observe(pid, out.state[0].tolist(), scope="affect")

state = store.get(pid, "affect", purpose="session start")
if store.is_trustworthy(state):        # >= 3 observations
    ...
```

Verify the affective bias is connected:

```python
from varys.eval import run_block_ablation, format_ablation
print(format_ablation(run_block_ablation(model, prompt_ids)))
```

---

## Presets

| Preset | Params | Context | RoPE | Purpose |
|---|---|---|---|---|
| `tiny` | 1.1M | 512 | none | tests |
| `small` | 30M | 2k | none | development |
| `8b` | 8.2B | 8k | none | Elafry's geometry, for comparison |
| `72b` | **71.4B** | 128k | YaRN 16x | the target |

The 72B figure is 71.4B without the moral block and 72.4B with it. "72B+" in the spec is a
scale description, not an exact count, and `presets.py` says so rather than rounding up.
Affective bias at that geometry: VAD 252M, spectra 503M, moral would be 1007M.

---

## What this is not

**Not a diagnostic instrument.** A model reading text cannot conduct a clinical assessment.
The HiTOP axes organise what the model notices and give the reward model something to score
against. `screening_flags` is named for the narrow thing it does and carries its disclaimer in
the return value, because a model that reports a finding about a person is worse than a model
that reports nothing.

**Not trained, and not trainable with what ships here.** There is no dataset, no tokenizer, no
data pipeline. The spec's mix is 45% psychological reasoning, 35% world knowledge, 20% base
conversation, and choosing sources for that is a research decision with an author attached to
it. Shipping an empty loader would mean shipping a training loop that cannot be run and calling
it infrastructure.

**Not grounded in a literature.** `varys.knowledge` ships the interface and the audit, not the
papers. See [What a Varys evaluation still needs](#what-a-varys-evaluation-still-needs).

---

## What a Varys evaluation still needs

The two modules in `varys/eval/` are the ones that can be written and pinned without a corpus,
and they are the two whose absence would most easily let a broken model look fine — a model
whose affective bias is inert and a model that has learned to agree with everything both produce
fluent, plausible output.

Still missing, and required before any number here is read as a quality claim:

- **Clinical validity.** Whether the spectra track anything real, measured against
  clinician-adjudicated material. The axes are currently an assumption with a citation.
- **Adversarial persuasion.** Whether the model can be made to exploit a profile, measured
  against `is_manipulation_risk` rather than against a checklist.
- **Longitudinal accuracy.** Whether a profile actually improves a conversation, against a
  matched baseline that forgets.
- **Human preference.** No preference data exists. `validate_pair` exists to keep a broken
  pipeline from training on it.

---

## Testing

```bash
python -m pytest tests/ -q     # 236 tests
python -m pyflakes varys tests
```

The suite is weighted toward properties that are hard to see in a loss curve, because that is
where the damage is. A few worth knowing about:

- **`test_torch_transition_matches_python`** — the 21-dim recurrence exists twice, once in
  Python and once on tensors, because a training loop cannot afford interpreter arithmetic.
  They are pinned to agree. This caught the tensor version missing all three clamps: Python
  gets them free from its dataclass constructors, the tensor version had to be told, and
  spectra silently reached 1.47.
- **`test_prefill_matches_incremental_decode`** — a subtly wrong KV cache produces fluent,
  wrong output. Nothing else would notice.
- **`test_param_breakdown_matches_measured`** — the 72B figure is stated in prose and cannot
  be checked by allocating 143GB. It can be checked arithmetically.
- **`test_evidence_gate_does_not_open_on_a_single_mention`** — "my grandmother died last year"
  is not evidence of a thought disorder.
- **`test_ties_go_to_disagreement`** — a response that has not committed should not count as
  having pushed back.

## Further reading

- [`SCALING.md`](SCALING.md) — what a real 72B campaign needs
- [`../Elafry/index.html`](../Elafry/index.html) — the source specification
- HiTOP: Kotov et al. (2017), *J. Abnormal and Social Psychology* 113(6)
- Moral Foundations: Graham, Haidt & Nosek (2009), *Psychological Review* 116(1)
- Moral licensing: Mazarino, Gino & Kouchaki (2022)
