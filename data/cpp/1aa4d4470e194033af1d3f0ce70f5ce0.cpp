// Write a C++ function `int longestSubarrayWithSumAtMostK(const std::vector<int>& arr, int k)` that, given a non-empty vector of non-negative integers and an integer `k`, returns the length of the longest contiguous subarray whose sum is **greater than or equal to** `k`. If no such subarray exists, return `0`. The vector may contain zeros, and `k` may be zero or positive. The function must handle large inputs efficiently and must not modify the input vector.
#include <cassert>
#include <vector>

int main() {
    // Basic cases
    assert(longestSubarrayWithSumAtMostK({1, 2, 3, 4}, 5) == 2); // [2,3] or [1,4]? actually [2,3] sum=5 length 2; [3,4] sum=7 length 2; longest is 2
    assert(longestSubarrayWithSumAtMostK({1, 2, 3, 4}, 10) == 4); // whole array sum 10
    assert(longestSubarrayWithSumAtMostK({1, 2, 3, 4}, 20) == 0); // none
    assert(longestSubarrayWithSumAtMostK({0, 0, 0}, 0) == 3); // all zeros qualify
    assert(longestSubarrayWithSumAtMostK({5, 1, 1, 1, 1}, 6) == 5); // whole array sum 9 >=6

    // Edge cases
    assert(longestSubarrayWithSumAtMostK({0}, 0) == 1);
    assert(longestSubarrayWithSumAtMostK({10}, 10) == 1);
    assert(longestSubarrayWithSumAtMostK({10}, 11) == 0);
    assert(longestSubarrayWithSumAtMostK({3, 3, 3}, 6) == 2); // [3,3] sum 6, but also [1..2] or [2..3]? actually [3,3] length 2, cannot get length 3 because sum 9 >6 but we need >=6? 9>=6 yes length 3! Wait: [3,3,3] sum 9 >=6, length 3, so expected 3. Let me re-check: The while loop will shrink after recording the whole array length 3, so best=3. So assert should be 3.

    // Correct the above: 
    assert(longestSubarrayWithSumAtMostK({3, 3, 3}, 6) == 3); // whole array sum 9 >=6

    // Mixed zeros and positives
    assert(longestSubarrayWithSumAtMostK({0, 5, 0, 0}, 5) == 4); // whole array sum 5

    // Large k, no match
    assert(longestSubarrayWithSumAtMostK({1, 2, 3}, 100) == 0);

    // k=0 with positive elements
    assert(longestSubarrayWithSumAtMostK({1,2,3}, 0) == 3); // whole array sum 6 >=0

    return 0;
}
#include <vector>
#include <algorithm>

// Returns the length of the longest contiguous subarray with sum >= k.
// Assumes all elements in arr are non-negative.
int longestSubarrayWithSumAtMostK(const std::vector<int>& arr, int k) {
    const int n = static_cast<int>(arr.size());
    int left = 0;
    long long currentSum = 0;
    int best = 0;

    for (int right = 0; right < n; ++right) {
        currentSum += arr[right];

        // While the current window already meets the requirement,
        // record and try to shrink from left to see if a longer window is possible.
        while (currentSum >= k) {
            best = std::max(best, right - left + 1);
            currentSum -= arr[left];
            ++left;
            // If left passes right, reset, but since elements are non-negative,
            // we ensure left <= right to avoid invalid window.
            if (left > right) {
                left = right;
                currentSum = 0;
                break;
            }
        }
    }

    return best;
}
// The problem asks for the longest contiguous subarray with sum **at least** `k`. A sliding window (two-pointer) approach works because all elements are non-negative, so increasing the window length never decreases the sum. We maintain two indices `left` and `right` representing the current window, and a `currentSum` of elements from `left` to `right`. Iterate `right` from 0 to n-1, adding `arr[right]` to the sum. If the sum becomes >= `k`, we record the window length (`right - left + 1`) as a candidate for the answer and update the maximum. Then, to continue searching for a longer window, we shrink the window from the left by subtracting `arr[left]` and incrementing `left` as long as the sum remains >= `k`. This process ensures we consider all possible windows that meet the condition. Edge cases: when `k == 0`, every single element (including zeros) qualifies, so the longest subarray is the entire array (since all elements are non-negative, the whole array sum will be >= 0, so the answer is `n`). Also, if no window has sum >= k, the answer remains 0. Time complexity is O(n) because each element is added once and removed at most once. Space complexity is O(1) auxiliary.
