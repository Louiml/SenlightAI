// Write a C++ function `double maxProductSubsequence(const std::vector<double>& nums)` that takes a non-empty vector of real numbers (which may include negative values, zero, and values with up to 3 decimal places) and returns the maximum product of any contiguous subsequence (i.e., a subarray) of the given list. The product is computed using the exact input values, and you must handle the fact that multiplying by a negative number can turn a very small (negative) product into a very large positive product, while zero resets the running product. For example, for the input `{-2.0, 0.5, -3.0}`, the best contiguous subarray is the entire array with product `3.0`. Return the result as a `double` (use `long double` internally for precision). The function must not modify the input vector.
The solution uses a dynamic programming approach that tracks the maximum and minimum product ending at each position. The key insight is that a negative number can make a previously minimum (most negative) product become the new maximum if multiplied by a negative, and a previously maximum product can become the minimum if multiplied by a negative. For each element `x` at index `i`, we compute:
- `curMax = max(x, x * prevMax, x * prevMin)`
- `curMin = min(x, x * prevMax, x * prevMin)`
where `prevMax` and `prevMin` are the maximum and minimum products of any subarray ending at index `i-1`. We also maintain a global answer initialized to the first element, updated with `curMax` at each step. The case where the product becomes zero is handled naturally because multiplying by zero yields zero, and then the next element restarts the sequence. Edge cases include: a single-element array (answer is that element), all negative numbers (e.g., `{-1, -2, -3}`: best is `6` from the whole array), and zeros mixed with negatives (e.g., `{0, -2, -3}`: best is `6` from `[-2,-3]`). Time complexity is O(n) where n is the number of elements, and space complexity is O(1) extra space beyond the input.
#include <vector>
#include <algorithm>
#include <cmath>

// Returns the maximum product of any contiguous subarray.
// Uses long double internally for precision, returns double.
double maxProductSubsequence(const std::vector<double>& nums) {
    if (nums.empty()) return 0.0;

    long double globalMax = nums[0];
    long double prevMax = nums[0];
    long double prevMin = nums[0];

    for (std::size_t i = 1; i < nums.size(); ++i) {
        long double x = static_cast<long double>(nums[i]);
        long double candidate1 = x * prevMax;
        long double candidate2 = x * prevMin;

        long double curMax = std::max({x, candidate1, candidate2});
        long double curMin = std::min({x, candidate1, candidate2});

        globalMax = std::max(globalMax, curMax);

        prevMax = curMax;
        prevMin = curMin;
    }

    return static_cast<double>(globalMax);
}
#include <cassert>
#include <cmath>
#include <vector>

// The solution function is declared here (or included from header).
double maxProductSubsequence(const std::vector<double>& nums);

int main() {
    // Basic cases
    assert(std::abs(maxProductSubsequence({2.0, 3.0, -2.0, 4.0}) - 6.0) < 1e-9);
    assert(std::abs(maxProductSubsequence({-2.0, 0.5, -3.0}) - 3.0) < 1e-9);
    assert(std::abs(maxProductSubsequence({-2.0}) - (-2.0)) < 1e-9);
    assert(std::abs(maxProductSubsequence({0.0, 2.0, -3.0, -4.0}) - 12.0) < 1e-9);
    assert(std::abs(maxProductSubsequence({-1.0, -2.0, -3.0}) - 6.0) < 1e-9);
    assert(std::abs(maxProductSubsequence({0.0, 0.0, 0.0}) - 0.0) < 1e-9);
    assert(std::abs(maxProductSubsequence({0.5, 2.0, 0.0, 3.0}) - 3.0) < 1e-9);
    assert(std::abs(maxProductSubsequence({-0.1, -10.0, 0.0, 5.0}) - 10.0) < 1e-9);
    assert(std::abs(maxProductSubsequence({1.0, -2.0, -3.0, 0.0, 2.0}) - 6.0) < 1e-9);
    assert(std::abs(maxProductSubsequence({-3.0, 0.0, -2.0}) - 0.0) < 1e-9);
    return 0;
}
