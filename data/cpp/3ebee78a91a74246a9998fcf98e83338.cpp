// Implement a C++ function named `maxAbsoluteSubarraySum` that takes a non-empty vector of integers `nums` and returns the maximum possible absolute sum of any contiguous subarray. The absolute sum of a subarray is computed as the absolute value of the sum of its elements. For example, given `nums = {1, -3, 2, 3, -4}`, the subarray `{2, 3}` has sum `5` and absolute sum `5`, while `{1, -3}` has sum `-2` and absolute sum `2`; the maximum absolute sum is `5`. The input may contain mixed positive and negative numbers, all negatives, all positives, or zeros, and the function must correctly handle subarrays of length 1. The function should be efficient for large inputs and must not modify the input vector.

The problem reduces to finding both the maximum subarray sum and the minimum subarray sum over all contiguous subarrays, because the maximum absolute sum is the larger of the absolute maximum sum and the absolute minimum sum. This is solved using Kadane's algorithm adapted to track both extremes simultaneously. For the maximum sum, we maintain `currentMax` as the maximum sum of a subarray ending at the current position, updating it as `max(currentMax + nums[i], nums[i])`. Similarly, `currentMin` tracks the minimum sum ending at the current position via `min(currentMin + nums[i], nums[i])`. We track global `bestMax` and `bestMin` as we iterate. The answer is `max(bestMax, abs(bestMin))`. Edge cases include: a vector with all negative numbers where the best subarray is a single smallest-magnitude negative (handled because `currentMax` resets at each negative), and a vector with all zeros producing 0. Complexity is O(n) time and O(1) auxiliary space.

#include <vector>
#include <algorithm>
#include <cstdlib> // for std::abs

// Returns the maximum absolute sum of any contiguous subarray.
int maxAbsoluteSubarraySum(const std::vector<int>& nums) {
    if (nums.empty()) return 0;
    
    int currentMax = nums[0];
    int currentMin = nums[0];
    int bestMax = nums[0];
    int bestMin = nums[0];
    
    for (size_t i = 1; i < nums.size(); ++i) {
        currentMax = std::max(currentMax + nums[i], nums[i]);
        bestMax = std::max(bestMax, currentMax);
        
        currentMin = std::min(currentMin + nums[i], nums[i]);
        bestMin = std::min(bestMin, currentMin);
    }
    
    return std::max(bestMax, std::abs(bestMin));
}

#include <cassert>
#include <vector>

int main() {
    // Single element
    assert(maxAbsoluteSubarraySum({5}) == 5);
    assert(maxAbsoluteSubarraySum({-7}) == 7);
    assert(maxAbsoluteSubarraySum({0}) == 0);

    // Mixed positive and negative
    assert(maxAbsoluteSubarraySum({1, -3, 2, 3, -4}) == 5);
    assert(maxAbsoluteSubarraySum({2, -1, 2}) == 3); // max sum = 3, min sum = -1
    assert(maxAbsoluteSubarraySum({-2, 1, -3, 4, -1, 2, 1, -5, 4}) == 6); // classic max sum = 6

    // All negative
    assert(maxAbsoluteSubarraySum({-1, -2, -3}) == 3); // subarray {-3}
    assert(maxAbsoluteSubarraySum({-5, -1, -2}) == 5); // subarray {-5}

    // All positive
    assert(maxAbsoluteSubarraySum({1, 2, 3}) == 6);

    // Contains zeros
    assert(maxAbsoluteSubarraySum({0, 0, 0}) == 0);
    assert(maxAbsoluteSubarraySum({-1, 0, 2}) == 2);

    // Larger test
    assert(maxAbsoluteSubarraySum({1, -4, 3, -2, 5, -1}) == 6); // subarray {3, -2, 5} sum=6

    return 0;
}
