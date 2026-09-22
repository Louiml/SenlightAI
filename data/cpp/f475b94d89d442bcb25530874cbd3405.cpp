// Given a vector of integers `nums` and an integer `k`, write a C++ function `longestSubarraySumK` that returns the length of the longest contiguous subarray whose sum equals `k`. If no such subarray exists, return `0`. The function should handle empty input, negative numbers, and large positive/negative values within the standard `int` range. The order of elements must be preserved, and the subarray must be contiguous in the original vector. For example, for `nums = {1, -1, 5, -2, 3}` and `k = 3`, the longest subarray with sum 3 is `{1, -1, 5, -2}` or `{3}` (the last element), so the answer is `4`.

The solution uses a prefix-sum technique with a hash map. We iterate through the array, maintaining a running `current_sum`. For each index `i`, we check:
- If `current_sum == k`, then the subarray from index 0 to `i` has sum `k`, so the length is `i+1`.
- If `current_sum - k` exists in the hash map at some earlier index `j`, then the subarray from `j+1` to `i` sums to `k`; its length is `i - j`.
We store only the *earliest* index for each prefix sum to maximize length when the same sum reappears. This is done by checking if the sum is already in the map before inserting; if it is, we do not overwrite it. Edge cases: an empty vector returns `0`; if the entire array sums to `k`, the longest length is `n`; if no subarray sums to `k`, the result is `0`. Time complexity is O(n) because we scan the array once and each map operation is O(1) on average. Space complexity is O(n) for the hash map storing up to `n` distinct prefix sums.

#include <vector>
#include <unordered_map>
#include <algorithm>

// Returns the length of the longest contiguous subarray with sum equal to k.
// If no such subarray exists, returns 0.
int longestSubarraySumK(const std::vector<int>& nums, int k) {
    std::unordered_map<int, int> prefix_sum_to_index; // earliest index for each prefix sum
    int current_sum = 0;
    int max_length = 0;

    for (int i = 0; i < static_cast<int>(nums.size()); ++i) {
        current_sum += nums[i];

        if (current_sum == k) {
            max_length = i + 1;
        } else if (prefix_sum_to_index.find(current_sum - k) != prefix_sum_to_index.end()) {
            max_length = std::max(max_length, i - prefix_sum_to_index[current_sum - k]);
        }

        // Only store the first (earliest) occurrence of this prefix sum.
        if (prefix_sum_to_index.find(current_sum) == prefix_sum_to_index.end()) {
            prefix_sum_to_index[current_sum] = i;
        }
    }

    return max_length;
}

#include <cassert>
#include <vector>

int main() {
    // Basic case
    std::vector<int> nums1 = {1, -1, 5, -2, 3};
    assert(longestSubarraySumK(nums1, 3) == 4);

    // Entire array sums to k
    std::vector<int> nums2 = {2, 3, 1, 4};
    assert(longestSubarraySumK(nums2, 10) == 4);

    // Subarray in the middle
    std::vector<int> nums3 = {5, -3, 2, 7, -1};
    assert(longestSubarraySumK(nums3, 6) == 3); // {2,7,-1} or {5,-3,2,7,-1}? Actually 5-3+2=4? Let's check: sum from index 2 to 4 = 2+7-1=8; index 1 to 3 = -3+2+7=6 => length 3.

    // No valid subarray
    std::vector<int> nums4 = {1, 2, 3};
    assert(longestSubarraySumK(nums4, 10) == 0);

    // Single element equals k
    std::vector<int> nums5 = {7};
    assert(longestSubarraySumK(nums5, 7) == 1);

    // Single element not equals k
    std::vector<int> nums6 = {7};
    assert(longestSubarraySumK(nums6, 5) == 0);

    // Empty vector
    std::vector<int> nums7 = {};
    assert(longestSubarraySumK(nums7, 0) == 0);

    // All zeros with k=0 -> whole array
    std::vector<int> nums8 = {0, 0, 0};
    assert(longestSubarraySumK(nums8, 0) == 3);

    // Negative k
    std::vector<int> nums9 = {-1, -2, -3};
    assert(longestSubarraySumK(nums9, -5) == 2); // {-2,-3} from index1 to2

    // Prefix sum repeats with desired difference
    std::vector<int> nums10 = {1, -1, 1, -1, 1};
    assert(longestSubarraySumK(nums10, 0) == 4); // subarray from 1 to 4? Actually whole array sum=1; from index0 to3 sum=0 length4.

    return 0;
}
