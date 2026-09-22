// Write a C++ function `longestSubarrayWithSumAtMostK` that takes a `const std::vector<int>&` of positive integers and an integer `k` (k ≥ 0), and returns the length of the longest contiguous subarray whose sum is less than or equal to `k`. If no subarray satisfies the condition (e.g., all elements are larger than k), return 0. The function must handle empty input (return 0) and large inputs efficiently. The original snippet uses a sliding window with two pointers, so your solution must follow that algorithm.

The problem is solved using a two-pointer sliding window technique. Maintain a window `[l, r]` (inclusive) where `l` is the left index and `r` is the right index. Iterate `r` from 0 to n-1, adding `a[r]` to a running `sum`. While the current window sum exceeds `k`, shrink the window from the left by subtracting `a[l]` and incrementing `l`. After adjusting, the window `[l, r]` is valid (sum ≤ k), so update the answer as `max(answer, r - l + 1)`. This works because all elements are positive, so increasing `r` always increases sum, and moving `l` only decreases sum, guaranteeing we find the longest window ending at each `r`. Edge cases: empty vector → return 0. All elements > k → the while loop will shrink until `l > r` (sum becomes 0), then window length is 0, so answer stays 0. Single element ≤ k → answer 1. Time complexity O(n) because each element is added and removed at most once. Space complexity O(1) beyond input storage.

#include <vector>
#include <algorithm>

// Returns the length of the longest contiguous subarray whose sum ≤ k.
int longestSubarrayWithSumAtMostK(const std::vector<int>& nums, int k) {
    int n = static_cast<int>(nums.size());
    int max_length = 0;
    int current_sum = 0;
    int left = 0;

    for (int right = 0; right < n; ++right) {
        current_sum += nums[right];

        while (current_sum > k) {
            current_sum -= nums[left];
            ++left;
        }

        max_length = std::max(max_length, right - left + 1);
    }

    return max_length;
}

#include <cassert>
#include <vector>

int longestSubarrayWithSumAtMostK(const std::vector<int>& nums, int k);

int main() {
    // Basic cases
    assert(longestSubarrayWithSumAtMostK({1, 2, 3}, 3) == 2);  // [1,2] or [3]
    assert(longestSubarrayWithSumAtMostK({1, 2, 3}, 1) == 1);  // [1]
    assert(longestSubarrayWithSumAtMostK({1, 2, 3}, 0) == 0);  // all > 0
    
    // Empty
    assert(longestSubarrayWithSumAtMostK({}, 10) == 0);
    
    // All elements large
    assert(longestSubarrayWithSumAtMostK({5, 6, 7}, 4) == 0);
    
    // Whole array qualifies
    assert(longestSubarrayWithSumAtMostK({2, 3, 4}, 10) == 3);
    
    // Exact k
    assert(longestSubarrayWithSumAtMostK({3, 2, 1}, 6) == 3);
    
    // Multiple elements equal to k
    assert(longestSubarrayWithSumAtMostK({4, 4, 4}, 4) == 1);
    
    // Larger test
    assert(longestSubarrayWithSumAtMostK({1, 2, 3, 4, 5}, 9) == 3);  // [2,3,4]
    assert(longestSubarrayWithSumAtMostK({10, 1, 1, 1, 10}, 3) == 3); // [1,1,1]
    
    // Negative k is not valid per spec (k ≥ 0), but if given, all sums > k → 0
    // (We don't test this because it's out of spec.)

    return 0;
}
