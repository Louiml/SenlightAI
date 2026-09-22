Write a C++ function that, given a vector of integers, returns the length of the longest strictly increasing subsequence (LIS). The subsequence does not need to be contiguous, but the elements must appear in their original order and each next element must be strictly greater than the previous one. If the input vector is empty, return 0. The function must be free of side effects and work for vectors of any size, including very large ones. The solution should use dynamic programming with an iterative approach, and the function must be declared with proper `const` correctness (accepting the vector by `const` reference) and return the result as an `int`.

#include <cassert>
#include <vector>

int longestIncreasingSubsequence(const std::vector<int>& nums); // declaration from solution

int main() {
    // Empty vector
    assert(longestIncreasingSubsequence({}) == 0);

    // Single element
    assert(longestIncreasingSubsequence({5}) == 1);

    // Strictly increasing
    assert(longestIncreasingSubsequence({1, 2, 3, 4, 5}) == 5);

    // Strictly decreasing
    assert(longestIncreasingSubsequence({5, 4, 3, 2, 1}) == 1);

    // Mixed with duplicates and not strictly increasing
    assert(longestIncreasingSubsequence({10, 9, 2, 5, 3, 7, 101, 18}) == 4); // LIS: 2,3,7,101

    // Duplicates cannot be used consecutively
    assert(longestIncreasingSubsequence({2, 2, 2, 2}) == 1);

    // Negative numbers and zeros
    assert(longestIncreasingSubsequence({-3, -1, 0, -2, 4}) == 4); // -3,-1,0,4

    // All same values
    assert(longestIncreasingSubsequence({7, 7, 7}) == 1);

    // Larger sequence with various patterns
    assert(longestIncreasingSubsequence({3, 10, 2, 1, 20, 30, 40}) == 4); // 3,10,20,30,40

    // Random-like sequence
    assert(longestIncreasingSubsequence({0, 8, 4, 12, 2, 10, 6, 14, 1, 9, 5, 13, 3, 11, 7, 15}) == 6); // e.g., 0,2,6,9,11,15

    return 0;
}

#include <vector>
#include <algorithm>

// Returns the length of the longest strictly increasing subsequence in the input vector.
// Accepts an empty vector and returns 0 in that case.
int longestIncreasingSubsequence(const std::vector<int>& nums) {
    if (nums.empty()) return 0;

    const int n = static_cast<int>(nums.size());
    std::vector<int> dp(n, 1); // dp[i] = LIS length ending at index i

    int maxLength = 1;
    for (int i = 1; i < n; ++i) {
        for (int j = 0; j < i; ++j) {
            if (nums[i] > nums[j]) {
                dp[i] = std::max(dp[i], dp[j] + 1);
            }
        }
        maxLength = std::max(maxLength, dp[i]);
    }

    return maxLength;
}

// The classic dynamic programming solution defines `dp[i]` as the length of the longest strictly increasing subsequence ending at index `i`. Initialize all `dp[i]` to 1 because a single element is always a valid subsequence of length 1. Then, for each index `i` from 1 to `n-1`, iterate over all previous indices `j` from 0 to `i-1`. If `nums[j]` is strictly less than `nums[i]`, then the subsequence ending at `j` can be extended by `nums[i]`, so we update `dp[i] = max(dp[i], dp[j] + 1)`. Finally, the answer is the maximum value in `dp`. Edge cases: an empty vector returns 0; a vector with one element returns 1; duplicates are handled naturally because we require strict inequality (`nums[i] > nums[j]`), so equal elements cannot extend each other. The time complexity is \(O(n^2)\) because of the double nested loop, and the space complexity is \(O(n)\) for the `dp` array. This approach is correct for all inputs, including negative numbers and large values.
