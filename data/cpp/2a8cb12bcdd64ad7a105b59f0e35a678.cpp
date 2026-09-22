// Write a standalone C++ function `cumulativeProductSum` that mimics the behavior of the cumulative product and cumulative sum operations from the provided PyTorch reduction kernels, but operates on a plain `std::vector<double>`. The function must accept a vector of double-precision floating-point numbers (which may be empty), an integer `dim` (where `dim` is always 0 since only 1D vectors are supported), and a boolean `isProduct` flag. If `isProduct` is `true`, the function should return a vector where each element at index `i` is the product of all elements from index `0` through `i` (inclusive), with the initial accumulator value set to `1`. If `isProduct` is `false`, it should return a vector where each element at index `i` is the sum of all elements from index `0` through `i`, with the initial accumulator value set to `0`. The function must handle edge cases: an empty input vector should return an empty result vector; a single-element input should return a vector with that element unchanged (for sum) or identical (for product). Use a simple iterative loop, no external libraries beyond `<vector>` and `<cstddef>`.

The solution directly replicates the core logic of `cumsum_cpu_kernel` and `cumprod_cpu_kernel` from the snippet, but simplified for 1D `std::vector<double>`. The provided PyTorch kernels use a generic `cpu_cum_base_kernel` that iterates over the reduction dimension (`dim`) with strides, but since our input is a flat 1D vector, the stride is always 1 and the dimension is always 0. The algorithm initializes an accumulator (`acc`) to `1` for product or `0` for sum, then walks through the input vector from left to right. For each element, it updates the accumulator (`acc *= value` or `acc += value`) and writes the accumulator into the output vector at the corresponding position. This produces the required cumulative behavior. Edge cases: an empty input returns an empty output (matching the `numel() == 0` early return). A single element works naturally because the loop runs once, and the accumulator becomes the product/sum of one value, which is the value itself. Time complexity is O(n) where n is the input size; space complexity is O(n) for the output vector, plus O(1) auxiliary space for the accumulator and loop index. No special handling for NaN or infinity is required because double arithmetic naturally propagates them.

#include <vector>
#include <cstddef>

// Computes cumulative sum or cumulative product along the first (and only) dimension.
// If isProduct is true, output[i] = self[0] * self[1] * ... * self[i].
// Otherwise, output[i] = self[0] + self[1] + ... + self[i].
// For an empty input vector, returns an empty vector.
std::vector<double> cumulativeProductSum(const std::vector<double>& self, bool isProduct) {
    std::vector<double> result;
    if (self.empty()) {
        return result;
    }

    result.resize(self.size());
    double acc = isProduct ? 1.0 : 0.0;   // init_val: 1 for product, 0 for sum

    for (std::size_t i = 0; i < self.size(); ++i) {
        if (isProduct) {
            acc *= self[i];
        } else {
            acc += self[i];
        }
        result[i] = acc;
    }

    return result;
}

#include <cassert>
#include <vector>

// The solution function is declared above; include it or paste it here.
// For brevity, assume the function is already defined before this main.

int main() {
    // Basic sum
    std::vector<double> in1 = {1.0, 2.0, 3.0, 4.0};
    std::vector<double> sum1 = cumulativeProductSum(in1, false);
    assert(sum1.size() == 4);
    assert(sum1[0] == 1.0);
    assert(sum1[1] == 3.0);
    assert(sum1[2] == 6.0);
    assert(sum1[3] == 10.0);

    // Basic product
    std::vector<double> prod1 = cumulativeProductSum(in1, true);
    assert(prod1.size() == 4);
    assert(prod1[0] == 1.0);
    assert(prod1[1] == 2.0);
    assert(prod1[2] == 6.0);
    assert(prod1[3] == 24.0);

    // Empty input
    std::vector<double> empty;
    assert(cumulativeProductSum(empty, false).empty());
    assert(cumulativeProductSum(empty, true).empty());

    // Single element
    std::vector<double> single = {5.5};
    auto sumSingle = cumulativeProductSum(single, false);
    assert(sumSingle.size() == 1);
    assert(sumSingle[0] == 5.5);
    auto prodSingle = cumulativeProductSum(single, true);
    assert(prodSingle.size() == 1);
    assert(prodSingle[0] == 5.5);

    // With zero and negative numbers
    std::vector<double> mixed = {2.0, -3.0, 0.0, 4.0};
    auto sumMixed = cumulativeProductSum(mixed, false);
    assert(sumMixed[0] == 2.0);
    assert(sumMixed[1] == -1.0);
    assert(sumMixed[2] == -1.0);
    assert(sumMixed[3] == 3.0);
    auto prodMixed = cumulativeProductSum(mixed, true);
    assert(prodMixed[0] == 2.0);
    assert(prodMixed[1] == -6.0);
    assert(prodMixed[2] == 0.0);
    assert(prodMixed[3] == 0.0);

    // all ones (product stays 1, sum increments)
    std::vector<double> ones(5, 1.0);
    auto prodOnes = cumulativeProductSum(ones, true);
    for (std::size_t i = 0; i < 5; ++i) {
        assert(prodOnes[i] == 1.0);
    }
    auto sumOnes = cumulativeProductSum(ones, false);
    for (std::size_t i = 0; i < 5; ++i) {
        assert(sumOnes[i] == static_cast<double>(i + 1));
    }

    return 0;
}
