# Scaling to 8B

The target in `README.md` section 8 is 8.03B parameters and 2 trillion tokens. This file is
about what that actually costs, because the numbers in the design document are aspirations and
this is the part where they meet arithmetic.

## What fits on a single 3060

Measured on this machine: 12GB VRAM, compute capability 8.6, bf16 supported.

| Preset | Params | Weights + fp32 AdamW | Verdict |
| --- | --- | --- | --- |
| `elafry-100m` | 102.1M | 1.6 GB | Full fine-tune, comfortable |
| `elafry-316m` | 320.9M | 5.1 GB | Full fine-tune, comfortable |
| `elafry-1.1b` | 1157.7M | 18.5 GB | Does not fit. LoRA or an 8-bit optimizer |
| `elafry-3.5b` | 3572.2M | 57.2 GB | No |
| `elafry-8b` | 8081.7M | 129.3 GB | No |

The memory column is weights (2 bytes, bf16) plus fp32 AdamW moments (8 bytes) plus fp32
gradients, which is about 16 bytes per parameter. Gradient checkpointing trades compute for
activation memory and does not move this number, because the optimizer states dominate.

So the ceiling on this box is somewhere between `elafry-1.1b` with an 8-bit optimizer and
`elafry-316m` with full AdamW. That is the honest upper bound for anything that trains here.

## What 8B actually requires

`elafry-8b` needs about 129 GB just for the optimizer state, before activations. Realistic
configurations:

| Hardware | Memory | Fits 8B with |
| --- | --- | --- |
| 1x H100 80GB | 80 GB | No. Inference only |
| 2x H100 80GB | 160 GB | FSDP over 2 ranks, activations are then the constraint |
| 8x H100 80GB | 640 GB | FSDP over 8 ranks, comfortable |
| 8x H100 with ZeRO-3 and offload | any | Fits, at a large throughput cost |

FSDP scaffolding is in `elafry/train/distributed.py`. It is written so the single-device path
is a genuine no-op, because distributed code that only runs under `torchrun` cannot be tested
on a laptop and distributed bugs hide behind plausible loss curves.

```python
from elafry.train import Linear4DConfig, setup_distributed, wrap_fsdp

setup_distributed()                       # reads RANK / WORLD_SIZE / LOCAL_RANK
model = wrap_fsdp(model, Linear4DConfig(dp=8))
```

Activation checkpointing is available via
`elafry.train.distributed.enable_activation_checkpointing`. It checkpoints whole blocks,
attention and affective bias included, which is the correct boundary: the bias depends on the
state, and recomputing it against a different state would be a silent correctness bug.

## Token budget

Chinchilla-optimal for 8B is about 160 billion tokens. The design document's 2T is 12.5x
that, which is a deliberate choice rather than an oversight: heavily over-trained models
generally do better, and 2T is the budget that leaves room for it. The cost is roughly 620
GPU-days of H100 compute at a realistic 1.8 TFLOP/s effective throughput:

```
2e12 tokens x 8.03e9 params x 6 FLOP/param/token / 1.8e12 FLOP/s / 86400 = ~620 days
```

That is not a figure to put in a plan and forget. It is four to eight months of dedicated
cluster time, and it is the single largest risk in this project. A smaller model trained on
more of the data available would be a better use of the same compute.

## The data problem is upstream of all of this

No corpus ships with this package. The two fetchers in the parent repository
(`python/download_dataset.py`, `python/fetch_ultrachat.py`) can pull from UltraData-Code
(22.6M rows available, 8,400 downloaded) and UltraChat. Neither comes close to 2T tokens.

At the current corpus size, the parent repository's own 100M checkpoint produces pure
gibberish: generating from it gives `"ruffples, learnve color theongve color. This Tocks f
difficult my d C es.idse industve be"`. That model saw 14.5M tokens, roughly 140x under the
Chinchilla figure for its size, and the result is not a partially-working model but a
non-functional one.

So the ordering that makes sense is:

1. Get a corpus past ~50B tokens. That is the prerequisite for anything above 300M producing
   coherent text.
2. Train `elafry-100m` and `elafry-316m` end to end through all three PG-RL stages. These are
   cheap and they exercise every code path.
3. Only then scale up. The architecture is not the bottleneck and the corpus is.

## What is verified and what is not

Verified on this machine, by the test suite and the parity harness:

* The model forward and backward pass at every preset's depth and head geometry
* The KV cache agrees with full prefill to 2.0e-7, in both Python and Rust
* The Rust and Python implementations agree to 3.6e-7 on the same prompt and pinned state
* Pinning the affective state changes the output distribution, and the change follows VAD
  geometry rather than adding noise
* A tiny model overfits a single batch to loss below 0.005, which is the standard proof that a
  from-scratch transformer is wired correctly
* Checkpoints carry their own geometry in a `config.json` sidecar, and the Rust engine
  refuses to load a file whose shape disagrees with it

Not verified, and not possible to verify on this hardware:

* That any preset above 316M trains. The code path is type-checked and shape-tested at every
  preset's depth, but no run has happened.
* That the model produces coherent text. Nothing has been trained on a real corpus.
* That the sycophancy harness measures what it claims on a trained model. Its calibration set
  is the same set the marker lists were written against, so the reported 0.90 accuracy is
  optimistic and is a floor rather than an estimate.
* That DPO reduces the sycophancy rate. The margin is unit-tested for its effect on gradients;
  its effect on a trained model is untested.