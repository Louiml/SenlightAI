/*
Given an integer `n` followed by a sequence of `n` integers, write a standalone C++ function `longestPositiveSubarraySum` that returns the maximum possible sum of a contiguous subarray that contains at least one positive integer. If no positive integer exists in the entire array, return 0. The function should handle negative numbers, zeros, and all values within the 32-bit signed integer range, and the returned sum may exceed the 32-bit range if many large positives are consecutive, so the return type must be `long long`. The function must be efficient for `n` up to 10^6, and must not modify the input vector.
*/

#include <vector>
#include <algorithm>
#include <cstdint>

// Returns the maximum sum of a contiguous subarray that contains at least one positive integer.
// If no positive integer exists in the array, returns 0.
long long longestPositiveSubarraySum(const std::vector<int>& arr) {
    long long current_sum = 0;
    long long best_sum = 0;
    bool has_positive = false;

    for (const int value : arr) {
        if (value > 0) {
            has_positive = true;
        }

        // If adding the current value makes the running sum negative,
        // reset the running sum (standard Kadane reset).
        if (current_sum + value < 0) {
            current_sum = 0;
            has_positive = false; // the subarray restarts, so we lose the positive flag
        } else {
            current_sum += value;
        }

        // Only update best_sum if the current subarray contains a positive number.
        if (has_positive) {
            best_sum = std::max(best_sum, current_sum);
        }
    }

    return best_sum;
}

#include <cassert>
#include <vector>

// Declaration of the function under test (assumed to be in the same translation unit)
long long longestPositiveSubarraySum(const std::vector<int>& arr);

int main() {
    // Basic case with positives and negatives
    assert(longestPositiveSubarraySum({1, -2, 3, 4, -1, 2}) == 8); // subarray [3,4,-1,2] sum=8, contains positives

    // All negative numbers, no positive -> return 0
    assert(longestPositiveSubarraySum({-1, -2, -3}) == 0);

    // All zeros, no positive -> return 0
    assert(longestPositiveSubarraySum({0, 0, 0}) == 0);

    // Single positive element
    assert(longestPositiveSubarraySum({5}) == 5);

    // Single negative element
    assert(longestPositiveSubarraySum({-5}) == 0);

    // Mixed with zeros and positives, best subarray is just the largest positive
    assert(longestPositiveSubarraySum({0, -1, 0, 10, 0}) == 10);

    // Large consecutive positives that sum beyond int range
    std::vector<int> big(200000, 100000); // 200k * 100k = 2e10 > 2^31-1
    assert(longestPositiveSubarraySum(big) == 200000LL * 100000LL);

    // Array that forces reset after a negative dip, but a later positive gives a better sum
    assert(longestPositiveSubarraySum({-100, 1, 2, 3}) == 6); // subarray [1,2,3]

    // Negative numbers after a positive, where the best subarray starts at the positive and extends
    assert(longestPositiveSubarraySum({4, -1, -2}) == 4); // subarray [4] alone, because adding negatives reduces sum

    // Multiple positives separated by negatives, best sum may include partial negatives if beneficial
    assert(longestPositiveSubarraySum({2, -1, 2}) == 3); // subarray [2,-1,2] sum=3

    // Edge case: zeros followed by a positive, then negatives
    assert(longestPositiveSubarraySum({0, 0, 3, -1}) == 3); // subarray [3] alone

    return 0;
}

// This problem is a variant of Kadane's algorithm with an extra constraint: the subarray must contain at least one positive integer. The classic Kadane's algorithm finds the maximum subarray sum, but it may produce a subarray consisting entirely of non-positive numbers (e.g., if all numbers are negative, the best subarray could be a single negative number). Here we need to force inclusion of a positive element.  
// Approach: Iterate through the array once while maintaining two running sums:  
// - `current_sum`: the maximum sum of any subarray ending at the current position that could be extended (this follows standard Kadane logic: if `current_sum + current_value` is negative, we reset to 0, because any further extension would be worse than starting fresh).  
// - `best_pos_sum`: the maximum sum of any subarray found so far that contains at least one positive number.  
// To ensure the constraint, we track whether the current subarray (that yields `current_sum`) has seen a positive number. If the current value is positive, the current subarray automatically satisfies the constraint. If we reset the running sum (because it becomes negative), we also reset the "has positive" flag to false. When we update `best_pos_sum`, we only consider subarrays where the flag is true.  
// Edge cases:  
// - If all numbers are non-positive, `current_sum` will reset often and `best_pos_sum` stays 0, which is correct because no positive-containing subarray exists.  
// - If the array has a single positive number, the answer is that number.  
// - If there are zeros and negatives but at least one positive, the answer is the maximum positive-containing subarray sum, which is at least the maximum positive value alone.  
// Time complexity: O(n) single pass. Space complexity: O(1) auxiliary, not counting input storage.
