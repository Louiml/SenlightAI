Write a C++ function `int maxSubarraySum(const std::vector<int>& nums)` that returns the maximum sum of any contiguous subarray (a non-empty sequence of consecutive elements) within the given vector. The function must handle vectors with both positive and negative integers, including all-negative inputs (in which case the maximum is the largest single element), and must work for vectors of size 1. The brute-force triple-loop approach shown in the snippet is inefficient; your solution should use a more efficient algorithm, preferably Kadane's algorithm. The function must not modify the input, should use appropriate `const` references, and must be self-contained (no reliance on global variables). The time complexity should be O(n) and auxiliary space O(1).
// The classic solution is Kadane's algorithm, which scans the array once while tracking two variables: `current_sum` (the maximum sum of a subarray ending at the current position) and `best_sum` (the overall maximum found so far). Initialize both with the first element. For each subsequent element `x`, update `current_sum = max(x, current_sum + x)` — this decides whether to start a new subarray at `x` or extend the previous one. Then update `best_sum = max(best_sum, current_sum)`. This works because the maximum subarray ending at position i is either the element itself or that element added to the maximum subarray ending at i-1. Edge cases: all negative numbers — the algorithm will pick the least negative (largest) element as `best_sum` because every `current_sum` resets to that element when `x` is larger than `current_sum + x`. Single-element vector: both variables start at that element, correct. Empty input is not expected per the task specification (assume non-empty). Time complexity O(n) because we single pass, space O(1) beyond the input storage.
#include <vector>
#include <algorithm>

// Returns the maximum sum of any contiguous non-empty subarray.
// Uses Kadane's algorithm: O(n) time, O(1) extra space.
int maxSubarraySum(const std::vector<int>& nums) {
    int current_sum = nums[0];
    int best_sum = nums[0];

    for (std::size_t i = 1; i < nums.size(); ++i) {
        current_sum = std::max(nums[i], current_sum + nums[i]);
        best_sum = std::max(best_sum, current_sum);
    }

    return best_sum;
}
#include <cassert>
#include <vector>

// Declaration of the function under test (copy from solution)
int maxSubarraySum(const std::vector<int>& nums);

int main() {
    // Example from the snippet: array {-2,1,-3,4,-1,2,1,-5,4} -> max subarray sum = 6 (subarray {4,-1,2,1})
    assert(maxSubarraySum({-2, 1, -3, 4, -1, 2, 1, -5, 4}) == 6);

    // All negative numbers: maximum subarray is the single largest element
    assert(maxSubarraySum({-3, -1, -2}) == -1);

    // All positive numbers: sum of entire array
    assert(maxSubarraySum({1, 2, 3}) == 6);

    // Single element
    assert(maxSubarraySum({7}) == 7);

    // Mixed with leading/trailing negatives
    assert(maxSubarraySum({-2, -3, 4, -1, -2, 1, 5, -3}) == 7); // {4,-1,-2,1,5} sum=7

    // Array with zero and negatives
    assert(maxSubarraySum({-1, 0, -2}) == 0);

    // Larger positive block after negatives
    assert(maxSubarraySum({-5, -1, 2, 3, -1, 4}) == 8); // {2,3,-1,4} sum=8

    // Two largest contiguous positives separated by small negative
    assert(maxSubarraySum({1, 2, -1, 3}) == 5); // entire array sum=5

    // Duplicate large values
    assert(maxSubarraySum({5, 5, -10, 5, 5}) == 10);

    // All zeros
    assert(maxSubarraySum({0, 0, 0}) == 0);

    return 0;
}
