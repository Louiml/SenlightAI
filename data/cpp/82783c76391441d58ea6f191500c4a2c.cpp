/*
Write a standalone C++ function `tanhForwardBackward` that takes a vector of doubles as input, applies the hyperbolic tangent activation function element-wise to compute the forward pass, then computes the backward pass gradients assuming an upstream gradient vector of all ones (i.e., the derivative of the loss with respect to each output is 1). The function should return a pair of vectors: the first vector contains the forward pass outputs (tanh of each input), and the second vector contains the backward pass gradients with respect to the inputs. The backward gradient for each element is `1 - (tanh(x))^2` because the upstream gradient is 1. The input vector must be non-empty; you may assume valid input. The function should not modify the input vector. Use `std::tanh` from `<cmath>` and follow const correctness.
*/

#include <vector>
#include <cmath>
#include <utility>

/**
 * Applies tanh activation forward and backward with unit upstream gradients.
 *
 * @param input A non-empty vector of double values.
 * @return A pair of vectors: first = forward outputs (tanh(input)), 
 *         second = backward gradients (1 - tanh^2(input)).
 */
std::pair<std::vector<double>, std::vector<double>> tanhForwardBackward(
    const std::vector<double>& input) {
    const size_t n = input.size();
    std::vector<double> forward(n);
    std::vector<double> backward(n);

    for (size_t i = 0; i < n; ++i) {
        double tanhx = std::tanh(input[i]);
        forward[i] = tanhx;
        backward[i] = 1.0 - tanhx * tanhx;
    }

    return {forward, backward};
}

#include <cassert>
#include <cmath>
#include <vector>

int main() {
    // Test with a simple positive/negative mix
    {
        std::vector<double> input = {0.0, 1.0, -1.0, 2.0, -2.0};
        auto result = tanhForwardBackward(input);
        assert(result.first.size() == 5);
        assert(result.second.size() == 5);
        for (size_t i = 0; i < input.size(); ++i) {
            double expected_fwd = std::tanh(input[i]);
            assert(std::fabs(result.first[i] - expected_fwd) < 1e-12);
            double expected_bwd = 1.0 - expected_fwd * expected_fwd;
            assert(std::fabs(result.second[i] - expected_bwd) < 1e-12);
        }
    }

    // Test single element
    {
        std::vector<double> input = {0.5};
        auto result = tanhForwardBackward(input);
        assert(result.first.size() == 1);
        assert(result.second.size() == 1);
        double expected_fwd = std::tanh(0.5);
        assert(std::fabs(result.first[0] - expected_fwd) < 1e-12);
        assert(std::fabs(result.second[0] - (1.0 - expected_fwd * expected_fwd)) < 1e-12);
    }

    // Test zero input -> gradient should be 1
    {
        std::vector<double> input = {0.0};
        auto result = tanhForwardBackward(input);
        assert(std::fabs(result.first[0] - 0.0) < 1e-12);
        assert(std::fabs(result.second[0] - 1.0) < 1e-12);
    }

    // Test large magnitude input -> gradient approaches 0
    {
        std::vector<double> input = {100.0, -100.0};
        auto result = tanhForwardBackward(input);
        assert(std::fabs(result.first[0] - 1.0) < 1e-12);
        assert(std::fabs(result.first[1] + 1.0) < 1e-12);
        assert(std::fabs(result.second[0]) < 1e-12);
        assert(std::fabs(result.second[1]) < 1e-12);
    }

    // Test vector with many elements and duplicates
    {
        std::vector<double> input = {0.2, 0.2, -0.3, 0.0, 1.5};
        auto result = tanhForwardBackward(input);
        for (size_t i = 0; i < input.size(); ++i) {
            double expected_fwd = std::tanh(input[i]);
            assert(std::fabs(result.first[i] - expected_fwd) < 1e-12);
            double expected_bwd = 1.0 - expected_fwd * expected_fwd;
            assert(std::fabs(result.second[i] - expected_bwd) < 1e-12);
        }
    }

    return 0;
}

// The solution is straightforward: iterate over the input vector once to compute the forward pass and store each `tanh` output. Then, since the upstream gradient is all ones, we can compute the backward pass in the same loop or a second loop by using the stored forward output: for each `tanhx`, the local gradient is `1 - tanhx * tanhx`. This is valid because derivative of `tanh(x)` is `1 - tanh^2(x)`. Edge cases: empty input is not allowed per spec, but if it were, the function should handle gracefully by returning empty vectors. Very large positive or negative inputs yield `tanh` near ±1, and the gradient becomes near 0, which is correct. The algorithm processes each element exactly twice (once for forward, once for backward) if done sequentially, so time complexity is O(n) where n is the number of elements. Space complexity is O(n) because we store two new vectors of the same size.
