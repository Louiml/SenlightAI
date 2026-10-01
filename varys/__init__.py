"""Varys: an affective architecture with longitudinal psychological state.

Varys is a 72B-class model specified for complex psychological and moral reasoning, built on
the same foundations as Elafry (8B) and extended where Elafry's scope stopped.

## The extension in one paragraph

Elafry carries a three-dimensional affective state (valence, arousal, dominance) and tracks it
across a conversation. That is the right primitive and Varys keeps it unchanged as the first
three dimensions of its own state. Varys adds two blocks that Elafry had no representation
for at all: six HiTOP spectra for psychopathology, and six Moral Foundations for moral
reasoning. Those two blocks are what make "dissects complex psychopathology" a capability
rather than a slogan. What makes it a *longitudinal* capability is the profile subsystem,
which carries a person's state across conversations.

## Layout, in dependency order

Subpackages are listed here in the order they may be read, which is a topological sort of the
internal import graph. Every entry depends only on entries above it.

    1. affective/   the state: VAD, Plutchik's wheel, HiTOP, Moral Foundations, encoders
    2. knowledge/   literature grounding and the citation gate. No internal dependencies,
                    so it could sit anywhere; it is placed after the state because it is
                    about claims made *about* that state.
    3. config/      ModelConfig, AffectConfig, TrainConfig. Needs the state feature width.
    4. models/      the transformer, with multi-axis affective attention bias
    5. eval/        per-block ablation, sycophancy. Needs the model it ablates.
    6. profile/     consent-gated longitudinal memory. Needs the state, not the model, which
                    is why it sits after eval despite being conceptually closer to the state.
    7. train/       SFT, affective reward model, PG-DPO. Needs everything above.

Two things this ordering makes explicit that are otherwise easy to get wrong:

* ``eval`` precedes ``train``, because ``varys.train.reward.validate_pair`` uses the
  agreement markers from ``varys.eval.agreement``. The reverse order is a cycle.
* ``config`` precedes ``models``, and ``config`` imports no torch. That is deliberate:
  ``config`` needs the state feature width, ``models`` needs the config, so putting the width
  next to the encoders would mean reading a config drags in the whole tensor stack. The
  dependency graph is a tree rather than a mesh because of that one constraint.

Note that this ordering is documentation, not filesystem layout. Git does not record
directory order and GitHub's tree view always sorts alphabetically, so the only place an
ordering like this can actually live is in the docstrings and the README.
"""

__version__ = "0.1.0"

__all__ = ["__version__"]
