/*
Write a C++ function `int longestIncreasingSubsequence(const std::vector<int>& nums)` that computes and returns the length of the longest strictly increasing subsequence (LIS) of a given sequence of integers. The function must handle empty input (return 0), negative numbers, and duplicate values (duplicates cannot be both included consecutively because the subsequence must be strictly increasing). You must implement a bottom-up dynamic programming solution with `O(n^2)` time and `O(n^2)` space complexity. Do not use any standard library algorithms like `std::lower_bound` or binary search for the LIS problem; use only the DP approach with a 2D table as described in the reference snippet.
*/
#include <vector>
#include <algorithm>

// Returns the length of the longest strictly increasing subsequence.
int longestIncreasingSubsequence(const std::vector<int>& nums) {
    const int n = static_cast<int>(nums.size());
    if (n == 0) return 0;

    // dp[curr][prev+1] = LIS length starting at index curr with previous index prev (or -1).
    std::vector<std::vector<int>> dp(n + 1, std::vector<int>(n + 1, 0));

    // Process from end to beginning.
    for (int curr = n - 1; curr >= 0; --curr) {
        for (int prev = curr - 1; prev >= -1; --prev) {
            // Option 1: include current element if it extends the increasing sequence.
            int include = 0;
            if (prev == -1 || nums[curr] > nums[prev]) {
                include = 1 + dp[curr + 1][curr + 1];
            }
            // Option 2: exclude current element.
            int exclude = dp[curr + 1][prev + 1];
            dp[curr][prev + 1] = std::max(include, exclude);
        }
    }
    return dp[0][0];
}
#include <cassert>
#include <vector>

// main function for testing only
int main() {
    assert(longestIncreasingSubsequence({}) == 0);
    assert(longestIncreasingSubsequence({5}) == 1);
    assert(longestIncreasingSubsequence({10, 9, 2, 5, 3, 7, 101, 18}) == 4);
    assert(longestIncreasingSubsequence({0, 1, 0, 3, 2, 3}) == 4);
    assert(longestIncreasingSubsequence({7, 7, 7, 7}) == 1);
    assert(longestIncreasingSubsequence({-2, -1, 0, 1, 2}) == 5);
    assert(longestIncreasingSubsequence({3, 10, 2, 1, 20}) == 3);
    assert(longestIncreasingSubsequence({1, 2, 3, 4, 5}) == 5);
    assert(longestIncreasingSubsequence({5, 4, 3, 2, 1}) == 1);
    assert(longestIncreasingSubsequence({1, 3, 2, 4}) == 3);
    return 0;
}
// The problem is the classic Longest Increasing Subsequence. The DP approach simulates choosing elements while tracking the previous chosen index to enforce strict ordering. We define `dp[curr][prev+1]` as the length of the longest increasing subsequence starting from index `curr` (inclusive) given that the last selected element was at index `prev` (using `prev = -1` when no element has been selected yet). The table is filled from the end of the array toward the beginning. For each `curr` from `n-1` down to `0` and each `prev` from `curr-1` down to `-1`, we consider two choices: (1) include `nums[curr]` if it is greater than `nums[prev]` (or `prev == -1`), giving `1 + dp[curr+1][curr+1]`; (2) exclude it, giving `dp[curr+1][prev+1]`. The result is the maximum of the two. The answer is stored in `dp[0][0]` after filling the table. Edge cases include an empty vector (return 0) and a vector of size 1 (return 1, since a single element is a valid increasing subsequence). The algorithm runs in `O(n^2)` time and uses `O(n^2)` space. The `prev+1` indexing avoids negative indices in the DP table.
