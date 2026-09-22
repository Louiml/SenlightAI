Write a C++ function `long long maximumSubarraySum(const std::vector<int>& nums)` that returns the maximum sum of any non-empty contiguous subarray of the given integer vector. The function should handle both negative and positive integers, including cases where all elements are negative (in which case the maximum subarray is the single largest element). The input vector will not be empty, but it may contain large values that could overflow 32-bit integers, so the return type must be `long long`. You must implement the Kadane's algorithm efficiently without sorting or using nested loops.
The problem is the classic "Maximum Subarray" problem. The main algorithm is Kadane's algorithm, which solves it in a single pass. We maintain a running sum `current_sum` that represents the maximum sum of a subarray ending at the current position. At each element, we decide whether to extend the previous subarray (by adding the current element to `current_sum`) or start a new subarray (by setting `current_sum` to the current element alone), whichever yields a larger sum: `current_sum = max(current_sum + nums[i], nums[i])`. We also keep track of the global maximum `best_sum` initialized to the first element (since the array is non-empty). For each position, we update `best_sum = max(best_sum, current_sum)`. This works because at each step, the optimal subarray ending at index `i` is either the previous optimal subarray extended, or just the single element itself. The edge case of all negative numbers is handled automatically because when we compare `max(current_sum + nums[i], nums[i])`, if all elements are negative, the better choice at each step is the element itself, and `best_sum` will be the maximum (least negative) element. The algorithm runs in O(n) time and O(1) auxiliary space, making it optimal.
#include <vector>
#include <algorithm>

// Returns the maximum sum of a non-empty contiguous subarray.
long long maximumSubarraySum(const std::vector<int>& nums) {
    if (nums.empty()) {
        return 0;  // Not expected per the task, but safe for completeness.
    }

    long long current_sum = nums[0];
    long long best_sum = nums[0];

    for (std::size_t i = 1; i < nums.size(); ++i) {
        current_sum = std::max(current_sum + nums[i], static_cast<long long>(nums[i]));
        best_sum = std::max(best_sum, current_sum);
    }

    return best_sum;
}
#include <cassert>
#include <vector>

// The solution function is defined above (included for the test environment).
// Assume maximumSubarraySum is available.

int main() {
    // Basic mixed positive and negative
    assert(maximumSubarraySum({1, -2, 3, 5, -1, 2}) == 9);  // subarray {3,5,-1,2} or {3,5}? Actually {3,5,-1,2}=9, {3,5}=8, {3,5,-1,2}=9
    // All negative
    assert(maximumSubarraySum({-3, -1, -2}) == -1);
    // Single element
    assert(maximumSubarraySum({42}) == 42);
    // All positive
    assert(maximumSubarraySum({1, 2, 3, 4}) == 10);
    // Large numbers to check long long overflow
    assert(maximumSubarraySum({2147483647, 2147483647}) == 4294967294LL);
    // Negative and positive mix with zero
    assert(maximumSubarraySum({-2, 1, -3, 4, -1, 2, 1, -5, 4}) == 6);
    // Negative leading/trailing
    assert(maximumSubarraySum({-5, -4, -3, 0, -1, -2}) == 0);
    // Zero only
    assert(maximumSubarraySum({0, 0, 0}) == 0);
    // One large negative and small positive
    assert(maximumSubarraySum({-100, 2, -1}) == 2);
    return 0;
}
