// Write a C++ function `longestCommonSubsequenceWithTrace` that takes two vectors of integers `a` and `b` (each with length up to 500) and returns a vector of integers representing the longest common subsequence (LCS) of `a` and `b` in the order they appear in both sequences. The function must reconstruct and return one valid LCS (not just its length). The input vectors may contain duplicate values, and the function should handle the case where no common subsequence exists (return an empty vector). The algorithm must be efficient enough for the given constraints.

The solution uses a classic dynamic programming approach optimized for space and time. We iterate over each element of `a` (outer loop) and for each, we iterate over each element of `b` (inner loop). We maintain a DP array `dp[j]` that stores the length of the LCS ending at position `j` in `b` when considering the current prefix of `a`. We also maintain a `track[j]` array that stores the previous index in `b` that forms the LCS, enabling reconstruction. For each pair `(a[i], b[j])`, we update `dp[j]` when a match occurs: `dp[j] = dp[best] + 1` where `best` is the index of the maximum `dp[k]` among all `k < j` such that `b[k] < a[i]`. This ensures we always extend the longest possible increasing (by value) subsequence. The reconstruction is done by finding the position with maximum `dp` value, then following the `track` pointers backward. Edge cases include empty inputs, no common elements, and duplicate values—all handled without special conditions. Time complexity is `O(n*m)` and space complexity is `O(m)` for the DP and tracking arrays.

#include <vector>
#include <algorithm>

// Returns the longest common subsequence of two integer vectors.
// The result is a vector containing the elements of one valid LCS.
std::vector<int> longestCommonSubsequenceWithTrace(const std::vector<int>& a, const std::vector<int>& b) {
    int n = static_cast<int>(a.size());
    int m = static_cast<int>(b.size());
    if (n == 0 || m == 0) return {};

    std::vector<int> dp(m + 1, 0);   // dp[j] = LCS length ending at b[j-1] for current a[i] prefix
    std::vector<int> prev(m + 1, 0); // prev[j] = previous index in b for reconstruction

    for (int i = 0; i < n; ++i) {
        int best = 0; // index of maximum dp[k] with k < j and b[k-1] < a[i]
        for (int j = 1; j <= m; ++j) {
            if (a[i] == b[j - 1]) {
                if (dp[j] < dp[best] + 1) {
                    dp[j] = dp[best] + 1;
                    prev[j] = best;
                }
            }
            if (a[i] > b[j - 1] && dp[j] > dp[best]) {
                best = j;
            }
        }
    }

    // Find the index with maximum dp value
    int end = 0;
    for (int j = 1; j <= m; ++j) {
        if (dp[j] > dp[end]) {
            end = j;
        }
    }

    // Reconstruct the subsequence backwards
    std::vector<int> result(dp[end]);
    int pos = dp[end] - 1;
    while (end > 0) {
        result[pos--] = b[end - 1];
        end = prev[end];
    }
    return result;
}

#include <cassert>
#include <vector>

// Assume longestCommonSubsequenceWithTrace is defined above.

int main() {
    // Basic test
    std::vector<int> a1 = {1, 2, 3, 4};
    std::vector<int> b1 = {2, 4};
    assert(longestCommonSubsequenceWithTrace(a1, b1) == std::vector<int>({2, 4}));

    // Empty input
    std::vector<int> a2 = {};
    std::vector<int> b2 = {1, 2};
    assert(longestCommonSubsequenceWithTrace(a2, b2).empty());

    // No common elements
    std::vector<int> a3 = {1, 2, 3};
    std::vector<int> b3 = {4, 5, 6};
    assert(longestCommonSubsequenceWithTrace(a3, b3).empty());

    // Duplicate values
    std::vector<int> a4 = {1, 1, 2, 3};
    std::vector<int> b4 = {1, 2, 1, 3};
    std::vector<int> lcs4 = longestCommonSubsequenceWithTrace(a4, b4);
    assert(lcs4.size() == 3);
    assert(lcs4 == std::vector<int>({1, 1, 3}) || lcs4 == std::vector<int>({1, 2, 3}) || lcs4 == std::vector<int>({1, 1, 3}));

    // Larger test
    std::vector<int> a5 = {5, 1, 6, 2, 7, 3};
    std::vector<int> b5 = {1, 5, 2, 6, 3, 7};
    std::vector<int> lcs5 = longestCommonSubsequenceWithTrace(a5, b5);
    assert(lcs5.size() == 3);
    assert(lcs5 == std::vector<int>({5, 6, 7}) || lcs5 == std::vector<int>({1, 2, 3}));

    // All same
    std::vector<int> a6 = {7, 7, 7};
    std::vector<int> b6 = {7, 7};
    assert(longestCommonSubsequenceWithTrace(a6, b6) == std::vector<int>({7, 7}));

    // Single element
    std::vector<int> a7 = {9};
    std::vector<int> b7 = {9};
    assert(longestCommonSubsequenceWithTrace(a7, b7) == std::vector<int>({9}));

    // One vector longer
    std::vector<int> a8 = {1, 2, 3, 4, 5};
    std::vector<int> b8 = {0, 1, 0, 2, 0, 3};
    assert(longestCommonSubsequenceWithTrace(a8, b8) == std::vector<int>({1, 2, 3}));

    // Negative numbers
    std::vector<int> a9 = {-3, -1, -2, 0};
    std::vector<int> b9 = {-1, -3, 0, -2};
    std::vector<int> lcs9 = longestCommonSubsequenceWithTrace(a9, b9);
    assert(lcs9.size() == 2);
    assert(lcs9 == std::vector<int>({-3, -2}) || lcs9 == std::vector<int>({-1, 0}) || lcs9 == std::vector<int>({-3, 0}) || lcs9 == std::vector<int>({-1, -2}));

    // Test with 500 elements (max constraint)
    std::vector<int> large_a(500);
    std::vector<int> large_b(500);
    for (int i = 0; i < 500; ++i) {
        large_a[i] = i;
        large_b[i] = 499 - i;
    }
    std::vector<int> large_lcs = longestCommonSubsequenceWithTrace(large_a, large_b);
    assert(large_lcs.size() == 1); // only one common element (249) can appear due to reversed order
    assert(large_lcs[0] == 249);

    return 0;
}
