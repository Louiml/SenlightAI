Write a C++ function `simulateFullyConnectedForward` that simulates the forward pass of a fully connected neural network layer with configurable activation functions, as inspired by the `FullyConnected::ForwardTimeStep` logic. The function should take: a vector of input values (size `ni`), a 2D vector representing the weight matrix (size `no x ni`, where `no` is the number of outputs), a bias vector (size `no`), an activation type string (one of `"linear"`, `"tanh"`, `"logistic"`, `"relu"`, `"softmax"`), and a boolean `training` flag. It must return a vector of outputs (size `no`) computed as `output = weights * input + bias`, then applying the specified activation function. For `"softmax"`, apply the softmax function across all outputs (stable version subtracting the max for numerical stability). For `"tanh"`, use `std::tanh`. For `"logistic"`, use the sigmoid function `1/(1+exp(-x))`. For `"relu"`, use `max(0, x)`. For `"linear"`, no activation. The `training` flag does not affect the computation but is provided for API compatibility; ignore its value. The function must be `const`-correct (no input modification) and handle edge cases: zero-sized inputs (return empty vector), and softmax with zero elements (return empty vector). Assume `ni > 0` and `no > 0` for valid calls, but still handle empty vectors gracefully.
// The core algorithm is straightforward matrix-vector multiplication followed by an element-wise or vector-wise activation. For each output neuron `j` in `[0, no)`, compute `sum = bias[j]`, then for each input `i` in `[0, ni)`, `sum += weights[j][i] * input[i]`. Store this in a temporary vector `pre_activation` of size `no`. Then apply the activation function:
//
// - **Linear**: Output equals `pre_activation` directly.
// - **Tanh**: Apply `std::tanh` to each element.
// - **Logistic**: Apply `1/(1+std::exp(-x))` to each element.
// - **ReLU**: Apply `std::max(0.0, x)` to each element.
// - **Softmax**: First compute `max_val = *std::max_element(pre_activation.begin(), pre_activation.end())` (if non-empty). Then for each element compute `exp(x - max_val)`, sum these, and divide each by the sum. This prevents overflow.
//
// Edge cases:
// - If `input` is empty or `weights` is empty or `no` is 0, return an empty vector.
// - If `ni` is 0 but `no > 0`, treat matrix-vector multiplication as just the bias (the loops over `i` are simply skipped).
// - Ensure the weight matrix has exactly `no` rows and each row has at least `ni` elements; if dimensions mismatch, the behavior is undefined but we can defensively iterate only up to the smaller of the input size and row size to avoid out-of-bounds access.
//
// Time complexity: O(no * ni) for the multiplication, plus O(no) for activation (softmax is O(no) after computing the sum). Space complexity: O(no) for the pre-activation vector and output vector.
//
// The solution uses `std::vector<double>` for both input and output, and `std::vector<std::vector<double>>` for weights. The function is declared as `std::vector<double> simulateFullyConnectedForward(const std::vector<double>& input, const std::vector<std::vector<double>>& weights, const std::vector<double>& bias, const std::string& activation, bool training)`. It is marked `const` by taking all inputs by const reference. The implementation includes `<vector>`, `<string>`, `<cmath>`, and `<algorithm>`.
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>

// Simulate forward pass of a fully connected layer with a given activation.
// Input: input vector (size ni), weight matrix (no x ni), bias vector (size no),
//        activation type string, and training flag (ignored).
// Returns: output vector (size no) after applying matrix multiplication and activation.
std::vector<double> simulateFullyConnectedForward(
    const std::vector<double>& input,
    const std::vector<std::vector<double>>& weights,
    const std::vector<double>& bias,
    const std::string& activation,
    bool /*training*/)
{
    const size_t no = weights.size();
    const size_t ni = input.size();

    // Handle empty or invalid dimensions gracefully.
    if (no == 0 || ni == 0) {
        return std::vector<double>();
    }

    std::vector<double> pre_activation(no, 0.0);

    // Compute weights * input + bias
    for (size_t j = 0; j < no; ++j) {
        // Ensure the weight row has at least as many entries as input; if not, use min.
        const size_t valid_count = std::min(ni, weights[j].size());
        double sum = bias[j]; // assume bias has at least no entries; if not, this is UB but typical usage is correct
        for (size_t i = 0; i < valid_count; ++i) {
            sum += weights[j][i] * input[i];
        }
        pre_activation[j] = sum;
    }

    // Apply activation function.
    std::vector<double> output(no);
    if (activation == "linear") {
        output = pre_activation;
    } else if (activation == "tanh") {
        for (size_t j = 0; j < no; ++j) {
            output[j] = std::tanh(pre_activation[j]);
        }
    } else if (activation == "logistic") {
        for (size_t j = 0; j < no; ++j) {
            output[j] = 1.0 / (1.0 + std::exp(-pre_activation[j]));
        }
    } else if (activation == "relu") {
        for (size_t j = 0; j < no; ++j) {
            output[j] = std::max(0.0, pre_activation[j]);
        }
    } else if (activation == "softmax") {
        // Stable softmax: subtract max before exponentiating.
        double max_val = *std::max_element(pre_activation.begin(), pre_activation.end());
        double sum_exp = 0.0;
        for (size_t j = 0; j < no; ++j) {
            output[j] = std::exp(pre_activation[j] - max_val);
            sum_exp += output[j];
        }
        for (size_t j = 0; j < no; ++j) {
            output[j] /= sum_exp;
        }
    } else {
        // Unknown activation: default to linear.
        output = pre_activation;
    }

    return output;
}
#include <cassert>
#include <cmath>

int main() {
    // Test linear activation with basic matrix.
    {
        std::vector<double> input = {1.0, 2.0};
        std::vector<std::vector<double>> weights = {{1.0, 0.5}, {2.0, -1.0}};
        std::vector<double> bias = {0.5, -0.5};
        auto out = simulateFullyConnectedForward(input, weights, bias, "linear", true);
        assert(out.size() == 2);
        assert(std::fabs(out[0] - (1.0*1.0 + 0.5*2.0 + 0.5)) < 1e-9);
        assert(std::fabs(out[1] - (2.0*1.0 + (-1.0)*2.0 + (-0.5))) < 1e-9);
    }

    // Test tanh activation.
    {
        std::vector<double> input = {0.0, 0.0};
        std::vector<std::vector<double>> weights = {{1.0, 1.0}};
        std::vector<double> bias = {0.0};
        auto out = simulateFullyConnectedForward(input, weights, bias, "tanh", false);
        assert(std::fabs(out[0] - std::tanh(0.0)) < 1e-9);
    }

    // Test relu activation with negative pre-activation.
    {
        std::vector<double> input = {1.0};
        std::vector<std::vector<double>> weights = {{-2.0}};
        std::vector<double> bias = {-1.0};
        auto out = simulateFullyConnectedForward(input, weights, bias, "relu", true);
        assert(out[0] == 0.0); // pre-activation = -3, relu gives 0
    }

    // Test logistic activation.
    {
        std::vector<double> input = {0.0};
        std::vector<std::vector<double>> weights = {{1.0}};
        std::vector<double> bias = {0.0};
        auto out = simulateFullyConnectedForward(input, weights, bias, "logistic", false);
        assert(std::fabs(out[0] - 0.5) < 1e-9); // sigmoid(0) = 0.5
    }

    // Test softmax: values should sum to 1 and be positive.
    {
        std::vector<double> input = {1.0, 2.0, 3.0};
        std::vector<std::vector<double>> weights = {{1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}};
        std::vector<double> bias = {0.0, 0.0, 0.0};
        auto out = simulateFullyConnectedForward(input, weights, bias, "softmax", true);
        double sum = 0.0;
        for (double v : out) {
            assert(v > 0.0);
            sum += v;
        }
        assert(std::fabs(sum - 1.0) < 1e-9);
        // The largest input should have the largest probability.
        assert(out[2] > out[1] && out[1] > out[0]);
    }

    // Test softmax with large values to ensure numerical stability.
    {
        std::vector<double> input = {1000.0, 1000.0, 1001.0};
        std::vector<std::vector<double>> weights = {{1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}};
        std::vector<double> bias = {0.0, 0.0, 0.0};
        auto out = simulateFullyConnectedForward(input, weights, bias, "softmax", false);
        double sum = 0.0;
        for (double v : out) {
            assert(std::isfinite(v));
            sum += v;
        }
        assert(std::fabs(sum - 1.0) < 1e-9);
    }

    // Test empty input returning empty output.
    {
        std::vector<double> input = {};
        std::vector<std::vector<double>> weights = {{1.0}};
        std::vector<double> bias = {0.0};
        auto out = simulateFullyConnectedForward(input, weights, bias, "linear", true);
        assert(out.empty());
    }

    // Test empty weights returning empty output.
    {
        std::vector<double> input = {1.0};
        std::vector<std::vector<double>> weights = {};
        std::vector<double> bias = {};
        auto out = simulateFullyConnectedForward(input, weights, bias, "tanh", true);
        assert(out.empty());
    }
}
