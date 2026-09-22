Given a non-empty vector of integers, write a C++ function `int maximumSubarraySum(const std::vector<int>& nums)` that returns the maximum possible sum of any contiguous subarray (Kadane’s algorithm). The vector may contain negative numbers, all negative numbers, a single element, or a mix of positive and negative values. If all numbers are negative, the function should return the largest (i.e., closest to zero) negative number.

The solution uses Kadane’s algorithm with dynamic programming. We maintain a running `current_sum` that represents the maximum sum of a subarray ending at the current index. At each position `i`, we decide whether to extend the previous subarray by adding `nums[i]` or start a new subarray at `nums[i]`. This decision is made by taking `max(current_sum + nums[i], nums[i])`. We also keep a `best_sum` that stores the maximum `current_sum` seen so far. The algorithm handles all edge cases naturally: for an all‑negative array, every `current_sum` will be equal to the current element itself (since adding a previous negative only makes it worse), so `best_sum` will end up as the maximum (least negative) element. For a single element array, the loop runs only once, returning that element. The time complexity is O(n) for a vector of size n, and space complexity is O(1) since we only use two integer variables.

#include <vector>
#include <algorithm>

// Returns the maximum sum of any contiguous subarray using Kadane's algorithm.
// Handles vectors with all negative numbers and single-element vectors correctly.
int maximumSubarraySum(const std::vector<int>& nums) {
    if (nums.empty()) {
        return 0; // Not expected per task but safe handling.
    }
    int current_sum = nums[0];
    int best_sum = nums[0];
    for (size_t i = 1; i < nums.size(); ++i) {
        current_sum = std::max(current_sum + nums[i], nums[i]);
        best_sum = std::max(best_sum, current_sum);
    }
    return best_sum;
}

#include <cassert>
#include <vector>

int main() {
    // Mixed positive and negative
    assert(maximumSubarraySum({-2, 1, -3, 4, -1, 2, 1, -5, 4}) == 6);
    // All negative
    assert(maximumSubarraySum({-3, -1, -2}) == -1);
    // Single element positive
    assert(maximumSubarraySum({5}) == 5);
    // Single element negative
    assert(maximumSubarraySum({-7}) == -7);
    // All positive
    assert(maximumSubarraySum({1, 2, 3, 4}) == 10);
    // Zero included
    assert(maximumSubarraySum({-1, 0, -2}) == 0);
    // Decreasing sequence
    assert(maximumSubarraySum({5, 4, 3, 2}) == 14);
    // Empty vector (safety check)
    assert(maximumSubarraySum({}) == 0);
    // Large negative and small positive
    assert(maximumSubarraySum({-100, 1, -50, 3}) == 3);
    return 0;
}
