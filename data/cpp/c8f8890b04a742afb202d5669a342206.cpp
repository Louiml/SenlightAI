// Write a standalone C++ function named `longestCommonSubsequence` that takes two `std::string` arguments and returns an `int` representing the length of the longest common subsequence (LCS) between the two strings. A subsequence is a sequence that appears in the same relative order but not necessarily contiguously. The function must handle empty strings (returning 0), differing lengths, and cases where characters repeat. The implementation should use dynamic programming with a 2D table for clarity and efficiency. The function should not modify the input strings and should be `const`-correct where applicable. Include all necessary standard library headers. The solution must be self-contained and not rely on any external libraries beyond the C++ standard library.
The solution uses classic dynamic programming. Let `m = s1.length()` and `n = s2.length()`. We create a 2D vector `dp` of size `(m+1) x (n+1)`, where `dp[i][j]` represents the length of the LCS of the prefixes `s1[0..i-1]` and `s2[0..j-1]`. The base cases are `dp[0][j] = 0` and `dp[i][0] = 0` because an empty string has no common subsequence with anything. For each `i` from 1 to `m` and `j` from 1 to `n`, if `s1[i-1] == s2[j-1]`, then `dp[i][j] = dp[i-1][j-1] + 1`; otherwise, `dp[i][j] = max(dp[i-1][j], dp[i][j-1])`. This recurrence correctly captures the optimal substructure: when characters match, we extend the LCS of the previous prefixes; when they don't, we take the better of skipping the current character from either string. The answer is `dp[m][n]`. Time complexity is `O(m*n)` and space complexity is also `O(m*n)`. Edge cases include empty strings (where the table is all zeros and the result is 0), strings with no common characters (result 0), and strings that are identical (result equals the length). The algorithm handles repeated characters correctly because it always considers all possible subsequence alignments.
#include <string>
#include <vector>
#include <algorithm>

// Returns the length of the longest common subsequence between two strings.
int longestCommonSubsequence(const std::string& s1, const std::string& s2) {
    int m = static_cast<int>(s1.length());
    int n = static_cast<int>(s2.length());

    // dp[i][j] = LCS length of s1[0..i-1] and s2[0..j-1]
    std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1, 0));

    for (int i = 1; i <= m; ++i) {
        for (int j = 1; j <= n; ++j) {
            if (s1[i - 1] == s2[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1] + 1;
            } else {
                dp[i][j] = std::max(dp[i - 1][j], dp[i][j - 1]);
            }
        }
    }

    return dp[m][n];
}
#include <cassert>

int main() {
    // Basic cases
    assert(longestCommonSubsequence("abcde", "ace") == 3);  // "ace"
    assert(longestCommonSubsequence("abc", "abc") == 3);    // identical
    assert(longestCommonSubsequence("abc", "def") == 0);    // no common

    // Empty strings
    assert(longestCommonSubsequence("", "") == 0);
    assert(longestCommonSubsequence("abc", "") == 0);
    assert(longestCommonSubsequence("", "xyz") == 0);

    // Different lengths and repeated characters
    assert(longestCommonSubsequence("aaaa", "aa") == 2);
    assert(longestCommonSubsequence("aggtab", "gxtxayb") == 4); // "gtab"
    assert(longestCommonSubsequence("abcdgh", "aedfhr") == 3);  // "adh"
    assert(longestCommonSubsequence("ABCBDAB", "BDCABA") == 4); // "BDAB" or "BCAB"

    // Single character cases
    assert(longestCommonSubsequence("a", "a") == 1);
    assert(longestCommonSubsequence("a", "b") == 0);

    // Case sensitivity
    assert(longestCommonSubsequence("ABC", "abc") == 0);
    assert(longestCommonSubsequence("AbC", "AC") == 2); // "AC"

    return 0;
}
