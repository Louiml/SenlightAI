// Write a C++ function `fullyConnectedForward` that simulates the forward pass of a fully connected layer for a single training sample without any dropout. The function should accept a `std::vector<float>` containing the flattened input activations (size `input_size`), a `std::vector<float>` representing the weight matrix in row-major order (shape `[input_size][output_size]`), a `std::vector<float>` representing the bias vector (size `output_size`), and an integer `input_size` and `output_size`. The function should return a `std::vector<float>` of size `output_size` containing the output activations computed as `output = weights^T * input + bias` (i.e., each output neuron `j` is the dot product of the input with column `j` of the weight matrix, plus the bias `j`). The function must be `const`-correct (i.e., not modify its input vectors) and should handle the edge cases where `input_size` or `output_size` is zero (return an empty vector) and where input size mismatches the weight matrix dimensions (in which case the function should throw a `std::invalid_argument`). The implementation must not use any external libraries beyond the standard C++ library.
#include <cassert>
#include <vector>
#include <cmath>

// The solution function is declared above (included here for completeness).
// For testing, we assume fullyConnectedForward is available.

int main() {
    // Basic 2x2 case.
    std::vector<float> input1 = {1.0f, 2.0f};
    std::vector<float> weights1 = {1.0f, 2.0f, 3.0f, 4.0f}; // [[1,2],[3,4]] row-major
    std::vector<float> bias1 = {0.5f, -0.5f};
    auto out1 = fullyConnectedForward(input1, weights1, bias1, 2, 2);
    // W^T * x = [[1,3],[2,4]] * [1,2] = [1*1+3*2, 2*1+4*2] = [7, 10]
    // plus bias -> [7.5, 9.5]
    assert(out1.size() == 2);
    assert(std::fabs(out1[0] - 7.5f) < 1e-5);
    assert(std::fabs(out1[1] - 9.5f) < 1e-5);

    // Single input, single output.
    std::vector<float> input2 = {5.0f};
    std::vector<float> weights2 = {2.0f};
    std::vector<float> bias2 = {1.0f};
    auto out2 = fullyConnectedForward(input2, weights2, bias2, 1, 1);
    assert(out2.size() == 1);
    assert(std::fabs(out2[0] - 11.0f) < 1e-5);

    // Zero output size returns empty.
    std::vector<float> input3 = {1.0f, 2.0f};
    std::vector<float> weights3 = {}; // empty
    std::vector<float> bias3 = {};    // empty
    auto out3 = fullyConnectedForward(input3, weights3, bias3, 2, 0);
    assert(out3.empty());

    // Zero input size with output size > 1: output equals bias.
    std::vector<float> input4 = {}; // empty
    std::vector<float> weights4 = {}; // empty
    std::vector<float> bias4 = {3.0f, 4.0f};
    auto out4 = fullyConnectedForward(input4, weights4, bias4, 0, 2);
    assert(out4.size() == 2);
    assert(std::fabs(out4[0] - 3.0f) < 1e-5);
    assert(std::fabs(out4[1] - 4.0f) < 1e-5);

    // Dimension mismatch should throw.
    bool threw = false;
    try {
        std::vector<float> input5 = {1.0f};
        std::vector<float> weights5 = {1.0f, 2.0f}; // wrong size for input_size=1, output_size=2
        std::vector<float> bias5 = {0.0f, 0.0f};
        auto out5 = fullyConnectedForward(input5, weights5, bias5, 1, 2);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);

    // Larger random-like case (deterministic check).
    std::vector<float> input6 = {0.1f, 0.2f, 0.3f};
    std::vector<float> weights6 = {
        1.0f, 2.0f, 3.0f,
        4.0f, 5.0f, 6.0f,
        7.0f, 8.0f, 9.0f
    }; // 3x3 row-major
    std::vector<float> bias6 = {0.1f, 0.2f, 0.3f};
    auto out6 = fullyConnectedForward(input6, weights6, bias6, 3, 3);
    // Manual: y0 = 1*0.1+4*0.2+7*0.3+0.1 = 0.1+0.8+2.1+0.1=3.1
    // y1 = 2*0.1+5*0.2+8*0.3+0.2 = 0.2+1.0+2.4+0.2=3.8
    // y2 = 3*0.1+6*0.2+9*0.3+0.3 = 0.3+1.2+2.7+0.3=4.5
    assert(out6.size() == 3);
    assert(std::fabs(out6[0] - 3.1f) < 1e-5);
    assert(std::fabs(out6[1] - 3.8f) < 1e-5);
    assert(std::fabs(out6[2] - 4.5f) < 1e-5);

    return 0;
}
#include <vector>
#include <stdexcept>
#include <cstddef>

/**
 * Simulates the forward pass of a fully connected (dense) layer.
 * Computes output = weights^T * input + bias, where weights is stored row-major
 * with shape [input_size][output_size].
 *
 * @param input         Flattened input activations of size input_size.
 * @param weights       Weight matrix in row-major order, size input_size * output_size.
 * @param bias          Bias vector of size output_size.
 * @param input_size    Number of input features.
 * @param output_size   Number of output neurons.
 * @return              Output vector of size output_size.
 * @throws std::invalid_argument if weights/bias size mismatches dimensions or if input_size is zero while output_size is positive.
 */
std::vector<float> fullyConnectedForward(
    const std::vector<float>& input,
    const std::vector<float>& weights,
    const std::vector<float>& bias,
    std::size_t input_size,
    std::size_t output_size)
{
    // Handle zero-dimension cases early.
    if (input_size == 0 || output_size == 0) {
        // If output_size is zero, return empty vector.
        if (output_size == 0) {
            return {};
        }
        // If input_size is zero but output_size > 0, weights must be empty and bias must match.
        if (weights.size() != 0 || bias.size() != output_size) {
            throw std::invalid_argument("fullyConnectedForward: dimension mismatch for zero input size");
        }
        // With zero input, output is just the bias.
        return bias; // bias.size() == output_size, but we want a vector copy.
    }

    // Validate dimensions.
    if (weights.size() != input_size * output_size) {
        throw std::invalid_argument("fullyConnectedForward: weight matrix size does not match input_size * output_size");
    }
    if (bias.size() != output_size) {
        throw std::invalid_argument("fullyConnectedForward: bias size does not match output_size");
    }
    if (input.size() != input_size) {
        throw std::invalid_argument("fullyConnectedForward: input size does not match input_size");
    }

    // Compute output.
    std::vector<float> output(output_size, 0.0f);
    for (std::size_t j = 0; j < output_size; ++j) {
        float sum = bias[j];
        for (std::size_t i = 0; i < input_size; ++i) {
            sum += weights[i * output_size + j] * input[i];
        }
        output[j] = sum;
    }
    return output;
}
// The task is a straightforward matrix-vector multiplication with bias addition. Given an input vector `x` of length `input_size`, a weight matrix `W` stored row-major with dimensions `[input_size][output_size]`, and bias `b` of length `output_size`, the result `y` is computed as `y[j] = sum_{i=0}^{input_size-1} W[i * output_size + j] * x[i] + b[j]` for each `j`. The main algorithm iterates over each output neuron `j`, computes the dot product of the input vector with the `j`-th column of the weight matrix, and adds the bias. Time complexity is `O(input_size * output_size)` because each of the `output_size` outputs requires a dot product of length `input_size`. Space complexity is `O(output_size)` for the result vector, plus `O(1)` auxiliary space for loop counters. Edge cases: (a) If `input_size == 0` or `output_size == 0`, the function returns an empty vector (assuming the weight matrix is also empty; otherwise throw if dimensions are inconsistent). (b) If the weight matrix size does not equal `input_size * output_size`, throw `std::invalid_argument` to prevent out-of-bounds access. (c) If the bias size does not equal `output_size`, also throw `std::invalid_argument`. No modifications to input vectors are made, and all parameters are passed by `const` reference for efficiency. The solution is self-contained and uses only `<vector>`, `<stdexcept>`, and `<cstddef>`.
