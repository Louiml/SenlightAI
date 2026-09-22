Given a non-empty vector of positive integers and a target sum, write a C++ function that returns the largest sum that is less than or equal to the target and can be formed by selecting any element of the vector any number of times (unbounded repetition). If no sum can be formed other than 0 (i.e., all elements are larger than the target), return 0. The vector may contain duplicates, and elements are not necessarily sorted. The function must handle targets up to 1000 and vector sizes up to 100, with all integers in the vector between 1 and 1000.

This is an unbounded knapsack / coin change variant where we maximize the total sum without exceeding the target. The natural approach is dynamic programming (DP): let `dp[t]` be the maximum sum ≤ `t` achievable using any elements. Initialize `dp[0] = 0` and for each target `j` from 1 to `t`, for each element `value` in the array, if `value ≤ j`, we can update `dp[j] = max(dp[j], dp[j - value] + value)`. Since we allow unlimited repetitions, we iterate `j` from 1 upward (not backward) so that the same element can be reused. The answer is `dp[t]`. Alternatively, a recursive memoization approach similar to the provided snippet computes the minimum "loss" (`t - achievable_sum`) and returns `t - min_loss`. Edge cases include a target smaller than every element (answer 0), target equal to an element (answer that element), and large targets with small elements (answer may equal target). Time complexity is O(n * t) where n is vector size and t is the target, and space is O(t) for the 1D DP array.

#include <vector>
#include <algorithm>
#include <cstddef>

// Returns the largest sum ≤ target that can be formed by summing any elements
// from values (each can be used any number of times).
int maxSumNotExceeding(int target, const std::vector<int>& values) {
    // dp[j] = maximum achievable sum ≤ j
    std::vector<int> dp(target + 1, 0);
    
    for (int j = 1; j <= target; ++j) {
        for (int v : values) {
            if (v <= j) {
                dp[j] = std::max(dp[j], dp[j - v] + v);
            }
        }
    }
    return dp[target];
}

#include <cassert>
#include <vector>

int main() {
    // Basic case: can reach target exactly
    assert(maxSumNotExceeding(10, {1, 2, 5}) == 10);
    // Cannot reach target, nearest below
    assert(maxSumNotExceeding(9, {4, 6}) == 8); // 6+? -> 8? Actually 4+4=8, 6+? no, 4+6=10 >9, so 8
    // All elements larger than target
    assert(maxSumNotExceeding(3, {5, 7}) == 0);
    // Target smaller than smallest
    assert(maxSumNotExceeding(2, {3, 4}) == 0);
    // Only one element that repeats
    assert(maxSumNotExceeding(7, {3}) == 6); // 3+3
    // Duplicate elements are fine
    assert(maxSumNotExceeding(11, {5, 5, 2}) == 11); // 5+2+2+2? Actually 5+5+2=12 >11, so 5+2+2+2=11
    // Large target with small element
    assert(maxSumNotExceeding(100, {1}) == 100);
    // Target equals an element
    assert(maxSumNotExceeding(5, {2, 5}) == 5);
    // Combination of elements
    assert(maxSumNotExceeding(16, {3, 7, 11}) == 16); // 3+? Actually 11+? 11+3=14, 7+7+3=17 >16, so 3+? 3*5=15, 3+3+3+3+? 15, 7+3+3=13, 11+? 11+? 14, so 16 not possible? 3+3+3+3+? 15, 7+3+3+3=16 yes
    assert(maxSumNotExceeding(16, {3, 7, 11}) == 16);
    // Minimal target 0
    assert(maxSumNotExceeding(0, {1, 2}) == 0);
    
    return 0;
}
