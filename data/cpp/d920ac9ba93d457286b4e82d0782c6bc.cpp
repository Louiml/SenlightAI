// Write a standalone C++ function `softmaxForward` that takes a 1D `std::vector<float>` representing input values and an integer `axis` (which must be 0 for a 1D vector), and returns a `std::vector<float>` containing the softmax probabilities (or log-softmax values if a third boolean parameter `logSoftmax` is true) applied along that axis. The function must handle the standard softmax computation: subtract the maximum input value for numerical stability, exponentiate, sum the exponentials, and divide each exponentiated value by the sum. When `logSoftmax` is true, return the natural logarithm of each normalized probability. Assume the input is non-empty and finite (no NaN or infinity), and the axis is valid (0 for the 1D case). The function must be `const`-correct (mark the input vector parameter as `const std::vector<float>&`), and not modify the input. Also handle the edge case where all inputs are equal (so the maximum subtraction yields all zeros, leading to exp(0)=1 for each, and sum = N, so probabilities are 1/N; for log softmax, values are log(1/N) = -log(N)).
#include <cassert>
#include <cmath>
#include <vector>

// The solution function is assumed to be declared above (or in the same file).

int main() {
    // Basic softmax with distinct values.
    std::vector<float> input1 = {1.0f, 2.0f, 3.0f};
    auto out1 = softmaxForward(input1);
    // Expected: exp(1-3)=exp(-2)=0.1353, exp(2-3)=exp(-1)=0.3679, exp(0)=1.0
    // Sum = 1.5032, probabilities: 0.0900, 0.2447, 0.6652 approximately.
    assert(std::abs(out1[0] - 0.0900f) < 1e-4f);
    assert(std::abs(out1[1] - 0.2447f) < 1e-4f);
    assert(std::abs(out1[2] - 0.6652f) < 1e-4f);
    // Sum of probabilities should be approximately 1.
    assert(std::abs((out1[0] + out1[1] + out1[2]) - 1.0f) < 1e-5f);

    // Edge case: all equal values -> uniform distribution.
    std::vector<float> input2 = {5.0f, 5.0f, 5.0f, 5.0f};
    auto out2 = softmaxForward(input2);
    // Expected: each probability = 1/4 = 0.25.
    for (size_t i = 0; i < out2.size(); ++i) {
        assert(std::abs(out2[i] - 0.25f) < 1e-6f);
    }
    // Sum of probabilities should be 1.
    float sum2 = 0.0f;
    for (float v : out2) sum2 += v;
    assert(std::abs(sum2 - 1.0f) < 1e-5f);

    // Log-softmax for the same equal values: log(0.25) = -log(4) ≈ -1.38629.
    auto logOut2 = softmaxForward(input2, 0, true);
    float expectedLog = std::log(0.25f);
    for (size_t i = 0; i < logOut2.size(); ++i) {
        assert(std::abs(logOut2[i] - expectedLog) < 1e-6f);
    }

    // Single-element vector: softmax gives 1.0; log-softmax gives 0.0.
    std::vector<float> input3 = {7.0f};
    auto out3 = softmaxForward(input3);
    assert(std::abs(out3[0] - 1.0f) < 1e-6f);
    auto logOut3 = softmaxForward(input3, 0, true);
    assert(std::abs(logOut3[0] - 0.0f) < 1e-6f);

    // Negative and large values to test stability.
    std::vector<float> input4 = {-1000.0f, -1000.0f, -999.0f};
    auto out4 = softmaxForward(input4);
    // exp(-1) = 0.3679 for the -999, and exp(0)=1 for the -1000s; sum = 2.3679
    // probabilities: 1/2.3679 ≈ 0.4223 each for -1000, and 0.3679/2.3679 ≈ 0.1554 for -999.
    assert(std::abs(out4[0] - (1.0f / (2.0f + std::exp(1.0f)))) < 1e-5f);
    assert(std::abs(out4[1] - (1.0f / (2.0f + std::exp(1.0f)))) < 1e-5f);
    assert(std::abs(out4[2] - (std::exp(1.0f) / (2.0f + std::exp(1.0f)))) < 1e-5f);

    // Verify input is not modified (const-correctness).
    std::vector<float> original = input1;
    auto out5 = softmaxForward(input1);
    assert(input1 == original);

    // Test that sum of probabilities is exactly 1 for a random-ish vector.
    std::vector<float> input5 = {0.1f, -0.2f, 0.3f, 1.0f, -0.5f};
    auto out6 = softmaxForward(input5);
    float sum6 = 0.0f;
    for (float v : out6) sum6 += v;
    assert(std::abs(sum6 - 1.0f) < 1e-5f);

    return 0;
}
#include <vector>
#include <algorithm>
#include <cmath>
#include <stdexcept>

// Compute softmax (or log-softmax if logSoftmax=true) along the given axis
// of a 1D input vector. Axis must be 0 for a 1D vector.
std::vector<float> softmaxForward(const std::vector<float>& input, int axis = 0, bool logSoftmax = false) {
    if (axis != 0) {
        throw std::invalid_argument("softmaxForward: axis must be 0 for a 1D vector");
    }
    if (input.empty()) {
        throw std::invalid_argument("softmaxForward: input must be non-empty");
    }

    // Step 1: Find the maximum value for numerical stability.
    float maxVal = *std::max_element(input.begin(), input.end());

    // Step 2: Compute exp(x_i - maxVal) into the result vector.
    std::vector<float> result(input.size());
    for (size_t i = 0; i < input.size(); ++i) {
        result[i] = std::exp(input[i] - maxVal);
    }

    // Step 3: Compute the sum of the exponentials.
    float sum = 0.0f;
    for (float val : result) {
        sum += val;
    }

    // Step 4: Normalize by dividing by the sum (or take log of normalized values).
    if (!logSoftmax) {
        for (float& val : result) {
            val /= sum;
        }
    } else {
        for (float& val : result) {
            val = std::log(val / sum);
        }
    }

    return result;
}
// The softmax operation maps a vector of real numbers into a probability distribution over the classes (positions) along a specified axis. For a 1D input vector of length `N`, the softmax at index `i` is `exp(x[i] - max(x)) / sum_j exp(x[j] - max(x))`. Subtracting the maximum prevents overflow in the exponentials (e.g., if inputs are large like 1000, `exp(1000)` overflows to infinity, but `exp(1000-1000)=exp(0)=1`). The algorithm: first find the maximum value in the vector using `std::max_element`. Then compute the exponentiated values into a temporary vector (or in place into a result vector) by subtracting the max and applying `std::exp`. Sum all these exponentiated values to get the denominator. Finally, for each index, divide the exponentiated value by the sum to get the probability; if `logSoftmax` is true, take `std::log` of the probability. Edge case: if all inputs are equal, the max equals each value, so after subtraction all are 0, exp(0)=1, sum = N, each probability = 1/N; log-softmax gives `-log(N)` for each. Time complexity: O(N) for max, O(N) for exp, O(N) for sum, O(N) for division/log, so overall O(N). Space complexity: O(N) for the result vector (and optionally a temporary exponent vector; but we can reuse the result vector for exponentiation and then modify in place). The function should not allocate extra beyond the result vector (and maybe a temporary for the exponentiated values, but we can write into the result directly and then sum from it). To be clean, we will compute into a result vector, sum that vector, then divide all entries, and if logSoftmax, take log of each entry. The function should validate that `axis == 0` for 1D, and `logSoftmax` default to `false`. Return by value to avoid lifetime issues.
