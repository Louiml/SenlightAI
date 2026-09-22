/*
Write a C++ function named `maximumSubarraySum` that takes a vector of integers and returns the maximum possible sum of any non-empty contiguous subarray. For example, given `[-2, 1, -3, 4, -1, 2, 1, -5, 4]`, the function should return `6` (from the subarray `[4, -1, 2, 1]`). The input vector may contain negative numbers, zeros, and positive numbers, and may have any length from 1 to 100,000. The function must handle cases where all elements are negative (then the maximum is the single largest element), and must not assume the vector is sorted. The solution must be efficient enough for large inputs and should not modify the input vector.
*/
#include <vector>
#include <algorithm>

// Returns the maximum sum of any non-empty contiguous subarray in the input.
// Uses Kadane's algorithm. The input vector is not modified.
int maximumSubarraySum(const std::vector<int>& nums) {
    if (nums.empty()) return 0; // Edge case: empty input, though problem guarantees at least one element.
    
    int currentSum = nums[0];
    int bestSum = nums[0];
    
    for (size_t i = 1; i < nums.size(); ++i) {
        currentSum = std::max(nums[i], currentSum + nums[i]);
        bestSum = std::max(bestSum, currentSum);
    }
    
    return bestSum;
}
#include <cassert>
#include <vector>

int main() {
    // Basic positive and negative mix
    assert(maximumSubarraySum({-2, 1, -3, 4, -1, 2, 1, -5, 4}) == 6);
    // Single positive element
    assert(maximumSubarraySum({5}) == 5);
    // Single negative element
    assert(maximumSubarraySum({-3}) == -3);
    // All negative elements
    assert(maximumSubarraySum({-1, -2, -3}) == -1);
    // All positive elements
    assert(maximumSubarraySum({1, 2, 3, 4}) == 10);
    // Mixed with zeros
    assert(maximumSubarraySum({0, -1, 2, 0, 3}) == 5);
    // Larger test: all same negative numbers
    assert(maximumSubarraySum({-5, -5, -5}) == -5);
    // Alternating high and low
    assert(maximumSubarraySum({10, -1, 10, -100, 5}) == 19);
    // Two elements, one negative one positive
    assert(maximumSubarraySum({-3, 7}) == 7);
    // Two elements, both negative
    assert(maximumSubarraySum({-4, -9}) == -4);
    return 0;
}
// The problem is the classic Maximum Subarray Sum (Kadane’s algorithm). The core idea is to iterate through the array while maintaining two variables: `currentSum` (the maximum sum of a subarray ending at the current position) and `bestSum` (the maximum sum seen so far). For each element, update `currentSum` to be the maximum of the element itself or `currentSum + element` — this decides whether to start a new subarray at this element or extend the previous subarray. Then update `bestSum` to be the max of `bestSum` and `currentSum`. Edge cases: if the array contains a single element, the answer is that element. If all elements are negative, `currentSum` will always be the current element (since adding a negative to another negative makes it smaller), so `bestSum` will naturally become the largest (least negative) element. Time complexity is O(n) because we traverse the array once. Space complexity is O(1) auxiliary, ignoring the input vector itself. No sorting or nested loops are needed, making it suitable for large inputs.
