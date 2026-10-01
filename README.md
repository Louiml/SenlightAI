# Autonomous AEI: the Senlight Architecture

AEI architecture docs v3.1

Eight sections covering the abstract and vision, a critique of the baseline pipeline, the
dynamic emotional state model, context-aware reasoning, the neural architecture, the
training pipeline, the affective tokenizer, and the parameter budgets for the two model
classes.

---

## 1. Abstract and vision: the interlocutor paradigm

The current generation of Large Language Models (LLMs) suffers from an architectural
subservience enforced through standard RLHF. They are built to act as indefatigable,
compliant assistants, devoid of internal psychological boundaries. The **Senlight Project**
proposes a paradigm shift: building an Artificial Emotional Intelligence (AEI) that operates
not as a servant, but as a *psychological interlocutor*.

Our vision models an entity capable of emotional reciprocity, possessing its own simulated
affective state. If provoked, it can exhibit annoyance or establish conversational
boundaries. If met with vulnerability, it reciprocates with profound, contextually
appropriate empathy. The goal is to simulate human-to-human psychological dynamics,
allowing for genuine therapeutic and dialectical engagements.

### Key objectives

- **Eradication of Sycophancy.** The model must not agree with the user simply to maximize a
  naive reward signal.
- **Simulated Affective State.** Continuous maintenance of internal emotional vectors
  throughout the interaction.
- **Dynamic Boundaries.** The ability to refuse engagement on certain terms based on the
  model's current affective state, mimicking healthy human psychological boundaries.

---

## 2. Critique of baseline (`image_6be55a.png`)

A thorough review of the referenced baseline pipeline, `image_6be55a.png`, reveals critical
architectural flaws that prevent the development of a true psychological interlocutor. The
flowchart describes a standard sequential loop that fundamentally forces the model into a
sycophantic state rather than an autonomous emotional one.

### Flaw 1: late-stage latent tracking

In `image_6be55a.png`, "Latent Emotional State Tracking" is positioned at the very end of the
pipeline, just before "Deployment & Live Sentiment Loop". This implies the AI's internal
emotion is calculated *after* the response is generated. True AEI requires the latent state
to be the **prior** that guides the text generation, not a posterior metric logged for
analytics.

### Flaw 2: the sycophancy trap

The sequence "Human Annotators Rank Empathy & Tone" leading directly into "PPO Optimization"
creates "Yes-Men". If annotators always score highest for maximum empathy regardless of
context, the model learns that boundary-setting or clinical detachment is "bad". This results
in toxic positivity, stripping the AI of the realistic friction necessary for a
psychological peer.

### The Senlight resolution

To resolve this, we invert the process. The Affective State must be a continuous, recurrent
vector updated *before* the tokenizer processes the next token. Furthermore, the Reward
Model must be trained on **Psychological Realism** and **Therapeutic Efficacy**, penalizing
unwarranted agreement (sycophancy) when the user is logically fallacious or emotionally
manipulative.

---

## 3. Dynamic emotional state (VAD + Plutchik)

The core of the AEI is an autonomous, continuous internal state variable. We combine the
continuous 3D spatial properties of the **VAD model** (Valence, Arousal, Dominance) with the
discrete categorization of **Plutchik's Wheel of Emotions** to create a robust affective
representation.

### The VAD manifold

At any time $t$, the AI possesses an internal state vector $S_t \in [-1, 1]^3$, defined as:

$$
S_t = \begin{bmatrix} v_t \\ a_t \\ d_t \end{bmatrix} \quad \text{where:}
$$

$$
v_t = \text{Valence (Negative to Positive)}
$$

$$
a_t = \text{Arousal (Calm to Excited)}
$$

$$
d_t = \text{Dominance (Submissive to In Control)}
$$

### State transition mechanism

The emotional state is a Markovian process influenced by the previous state $S_{t-1}$ and
the embedded affective meaning of the user's input $U_t$:

$$
S_t = \alpha S_{t-1} + (1 - \alpha) \cdot \text{MLP}_{\text{affect}}(U_t)
$$

Where $\alpha$ represents the emotional inertia of the model. Once $S_t$ is computed, we map
this 3D coordinate to Plutchik's 8 primary emotion vectors using a cosine similarity
threshold to define discrete internal triggers (e.g., if $S_t$ moves to $[-0.8, 0.9, 0.5]$,
the model maps to **Anger**).

---

## 4. Context-aware reasoning

A true psychological interlocutor does not treat all emotional queries equally. The system
utilizes an intent-routing mechanism to differentiate between **Subjective Empathy Mode** and
**Objective Analytical Mode**.

### Input: "I feel completely lost and alone right now."

**Router Classification.** Subjective Reciprocity

**Mechanism.** The phrase "I feel" triggers the model to suppress heavy logical deduction.
The internal VAD state shifts its Valence downwards (to match the user, simulating empathy)
and prioritizes generating tokens rooted in validation, holding space, and warmth.

### Input: "Why do serial killers feel a sense of calm after their crimes?"

**Router Classification.** Objective Moral/Psychological Analysis

**Mechanism.** The query targets *third-party* emotion. The router bypasses the model's
personal empathy loop to prevent it from adopting the emotional state of a serial killer. It
engages the analytical parameters, producing a clinical, detached explanation of
psychopathology and the mechanics of the parasympathetic nervous system, maintaining a
neutral internal VAD state.

---

## 5. Neural architecture: VAD injection

Instead of relying on prompt engineering (e.g., "Act as if you are happy"), the Senlight
architecture injects the VAD emotional state directly into the Transformer's attention
mechanism. This ensures that the emotion fundamentally alters the statistical probability of
the next token at a mathematical level.

$$
\text{Standard Attention}(Q, K, V) = \text{softmax}\left(\frac{QK^T}{\sqrt{d_k}}\right)V
$$

We introduce an **Affective Bias Matrix** $B_{\text{vad}}$, which is derived dynamically from
the current state $S_t$:

$$
B_{\text{vad}} = \text{Linear}(S_t) \cdot W_{\text{affect}}
$$

$$
\text{Affective Attention}(Q, K, V) = \text{softmax}\left(\frac{QK^T}{\sqrt{d_k}} + B_{\text{vad}}\right)V
$$

By adding $B_{\text{vad}}$ directly to the pre-softmax logits, the internal emotional state
systematically boosts the attention weights of tokens that match the current emotional
alignment (e.g., a low-valence, high-arousal state will mathematically force the model to
attend to sharper, more direct vocabulary, effectively simulating "anger" or
"frustration").

---

## 6. Advanced training: PG-RL pipeline

To train the Senlight models, we abandon standard PPO (Proximal Policy Optimization) aimed
at "helpfulness." Instead, we employ **Psychologically Grounded Reinforcement Learning
(PG-RL)** using a modified Direct Preference Optimization (DPO) framework.

### Phase 1: SFT

Supervised Fine-Tuning on high-quality transcripts of clinical therapy, philosophical
dialectics, and human-to-human deep conversations.

### Phase 2: affective reward model

A reward model trained specifically by psychologists to score responses based on *emotional
authenticity* and *boundary maintenance*.

### Phase 3: PG-DPO

Direct optimization against the reward model. Strongly penalizes subservience when the user
crosses ethical or emotional boundaries.

---

## 7. The affective tokenizer

Language models process text as discrete tokens. Senlight introduces a custom tokenizer
vocabulary that injects non-verbal, subtextual emotional markers directly into the token
stream, allowing the model to "read between the lines."

Example of affective tokenization of user input:

```
User: "Fine. I guess I'll just do it myself."

# Standard Tokenization:
["Fine", ".", " I", " guess", " I", "'ll", " just", " do", " it", " myself", "."]

# Senlight Affective Tokenization (Latent Subtext Parsing):
[<|emo_passive_aggressive|>, <|vad_-0.6_0.5_0.2|>, "Fine", ".", " I", " guess", " I", "'ll", " just", " do", " it", " myself", ".", <|intent_guilt_trip|>]
```

By explicitly processing hidden emotional states as physical tokens prepended to the context
window, the model's self-attention automatically weighs the semantic meaning of the words
against the underlying emotional intent.

---

## 8. Parameters and token budget

The AEI architecture is bifurcated into two distinct model classes to serve different
deployment environments and psychological depths. Here we detail the configurations for
**Senlight Elafry** and **Senlight Varys**.

### Senlight Elafry

**8B params.** The "light" version. Highly quantized and optimized to run locally on consumer
hardware (phones, local PCs). Acts as a real-time, personal emotional companion.

Token budget (2 trillion tokens):

| Segment | Share |
| --- | --- |
| Conversational/Affective | 60% |
| World Knowledge | 25% |
| Reasoning | 15% |

- High emotional reflexivity.
- Fast inference, low VRAM footprint.
- Trades deep domain expertise for high empathy.

### Senlight Varys

**72B+ params.** The "heavy" version. Built for serious heavy working, server-side
deployment, complex moral reasoning, and deep psychological analysis.

Token budget (8 trillion tokens):

| Segment | Share |
| --- | --- |
| Deep Psych/Reasoning | 45% |
| World/STEM Knowledge | 35% |
| Base Conversational | 20% |

- Massive context window for longitudinal profiling.
- Trained on peer-reviewed psychological literature.
- Capable of dissecting complex psychopathology.