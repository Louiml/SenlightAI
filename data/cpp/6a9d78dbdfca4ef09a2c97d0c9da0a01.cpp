Write a standalone C++ function that simulates a single feedforward and backpropagation update for a tiny neural-network layer, but without any existing network classes. The function should be named `applyNeuronUpdate` and take the following parameters: a `std::vector<double>` representing the input activations from the previous layer (size `n`), a `std::vector<double>` representing the current weights from a single neuron to those `n` inputs (same size), a `double` for the neuron’s bias (used directly as an additive constant before the transfer function), a `double` target output for this neuron, a `double` learning rate `eta`, and a `double` momentum factor `alpha`. The function must: (1) compute the neuron’s output as `tanh(bias + sum_i(input_i * weight_i))`; (2) compute the output error delta as `(target - output) * (1 - output^2)`; (3) compute weight updates using the rule `new_weight_i = weight_i + eta * input_i * delta + alpha * previous_weight_change_i`, but since there is no previous change stored, the momentum term should be replaced with `alpha * delta * weight_i` (a simplified decaying regularization) — clarify in comments; (4) update the weights in‑place in the provided vector; (5) return the neuron’s raw output (before or after transfer? choose: return the post‑transfer output). The function must handle an empty input vector gracefully by returning 0.0 and making no changes. No classes, no global state, no `main` in the solution.

#include <cassert>
#include <cmath>
#include <vector>

// Declaration of the function under test.
double applyNeuronUpdate(std::vector<double>& inputs, std::vector<double>& weights,
                         double bias, double target, double eta, double alpha);

int main() {
    // Test 1: basic case with two inputs, zero bias.
    {
        std::vector<double> inputs = {1.0, 2.0};
        std::vector<double> weights = {0.5, -0.5};
        double output = applyNeuronUpdate(inputs, weights, 0.0, 0.0, 0.1, 0.0);
        double expected_output = std::tanh(1.0*0.5 + 2.0*(-0.5)); // tanh(-0.5)
        assert(std::fabs(output - expected_output) < 1e-9);
        // Since target=0 and output~-0.462, delta = (0 - (-0.462))*(1 - 0.213) ~ 0.462*0.787 = 0.363
        // weight[0] = 0.5 + 0.1*1.0*0.363 = 0.5363
        // weight[1] = -0.5 + 0.1*2.0*0.363 = -0.4274
        assert(std::fabs(weights[0] - 0.5363) < 1e-3);
        assert(std::fabs(weights[1] + 0.4274) < 1e-3);
    }

    // Test 2: empty input vector returns 0.0 and leaves weights unchanged.
    {
        std::vector<double> inputs;
        std::vector<double> weights = {1.0, 2.0};
        double output = applyNeuronUpdate(inputs, weights, 0.0, 1.0, 0.5, 0.1);
        assert(output == 0.0);
        assert(weights[0] == 1.0);
        assert(weights[1] == 2.0);
    }

    // Test 3: bias shifts output.
    {
        std::vector<double> inputs = {1.0};
        std::vector<double> weights = {0.0};
        double output = applyNeuronUpdate(inputs, weights, 2.0, 0.0, 0.01, 0.0);
        assert(std::fabs(output - std::tanh(2.0)) < 1e-9);
        // delta = (0 - tanh(2))*(1 - tanh^2(2)) ~ -0.964 * 0.0707 ~ -0.0682
        // change = 0.01 * 1.0 * (-0.0682) = -0.000682 => weight ≈ -0.000682
        assert(std::fabs(weights[0] + 0.000682) < 1e-4);
    }

    // Test 4: momentum term (alpha) modifies update even with zero gradient? No, delta is zero if target==output.
    {
        std::vector<double> inputs = {0.5, -0.5};
        std::vector<double> weights = {0.0, 0.0};
        double output = applyNeuronUpdate(inputs, weights, 0.0, std::tanh(0.0), 0.1, 0.2);
        // sum=0, output=0, delta=0 => weights unchanged regardless of alpha.
        assert(output == 0.0);
        assert(weights[0] == 0.0);
        assert(weights[1] == 0.0);
    }

    // Test 5: large input, verify output bounded in (-1,1).
    {
        std::vector<double> inputs = {100.0, -100.0};
        std::vector<double> weights = {1.0, 1.0};
        double output = applyNeuronUpdate(inputs, weights, 0.0, 0.0, 0.01, 0.0);
        assert(output > -1.0 && output < 1.0);
        // output = tanh(0) = 0, so delta=0, weights remain unchanged.
        assert(weights[0] == 1.0);
        assert(weights[1] == 1.0);
    }

    return 0;
}

#include <vector>
#include <cmath>
#include <cstddef>

/**
 * Performs a single feedforward and backpropagation update for one neuron.
 * @param inputs        Activations from previous layer (size n).
 * @param weights       Current weights from this neuron to each input (size n). Updated in place.
 * @param bias          Bias term added before the transfer function.
 * @param target        Desired output for this neuron.
 * @param eta           Learning rate (0 < eta <= 1 typical).
 * @param alpha         Momentum-like factor (0 <= alpha < 1). Simplification: uses current weight for decay.
 * @return              Neuron output after tanh (post-transfer).
 */
double applyNeuronUpdate(std::vector<double>& inputs, std::vector<double>& weights,
                         double bias, double target, double eta, double alpha)
{
    // Guard: if no inputs, nothing to compute or update.
    if (inputs.empty() || weights.empty()) {
        return 0.0;
    }

    // Ensure same size (for safety, use min size).
    std::size_t n = (inputs.size() < weights.size()) ? inputs.size() : weights.size();

    // Feedforward: compute pre-activation.
    double sum = bias;
    for (std::size_t i = 0; i < n; ++i) {
        sum += inputs[i] * weights[i];
    }

    // Transfer: tanh.
    double output = std::tanh(sum);

    // Backpropagation: delta for output neuron.
    double delta = (target - output) * (1.0 - output * output);

    // Update weights in place.
    for (std::size_t i = 0; i < n; ++i) {
        // Gradient term: eta * input * delta
        // Simplified momentum: alpha * delta * weight (acts like weight decay, not true momentum)
        double change = eta * inputs[i] * delta + alpha * delta * weights[i];
        weights[i] += change;
    }

    return output;
}

// The core algorithm follows the classic single‑neuron delta rule with a tanh activation. First, compute the weighted sum plus bias. Apply `tanh` to get the output. The derivative of `tanh(x)` with respect to the pre‑activation is `1 - output^2`, so the delta for the output neuron is `(target - output) * (1 - output^2)`. For each input index, update the weight by `eta * input[i] * delta` (gradient descent) plus a momentum‑like term `alpha * delta * weight[i]` — this is not the standard momentum (which would require storing the previous update), but for a self‑contained task it is a valid simplification that encourages weight decay when `alpha` is positive. The function operates in‑place on the weight vector. Edge cases: if the input vector is empty, there is nothing to update, so return 0.0 immediately. If the weight vector has a different size than the input vector, the function should still work by iterating over the minimum length? The task specification says sizes are equal; we can assume that. For time complexity, the function is O(n) because it loops once over the inputs and weights. Space complexity is O(1) beyond the input/output vectors. The return value is the neuron’s output after `tanh`, which lies in (−1,1). The test code will provide small vectors and verify the output and updated weights using approximate equality (e.g., with a tolerance) because floating‑point operations are involved.
