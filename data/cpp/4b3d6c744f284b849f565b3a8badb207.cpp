// Write a C++ function that takes two strings, `s1` and `s2`, and two integers `n` and `m` representing their respective lengths (the strings may contain arbitrary ASCII characters, including spaces, but lengths are explicitly provided), and returns the length of the longest common subsequence (LCS) between them. The LCS is the longest sequence of characters that appears in the same order in both strings, but not necessarily contiguously. For example, given `s1 = "ABCBDAB"` and `s2 = "BDCABA"` with lengths 7 and 6, the LCS length is 4 (e.g., "BDAB" or "BCAB"). The function must be efficient for strings up to length 1000, handle empty strings (when `n` or `m` is 0) correctly, and not modify the input strings.
The solution uses dynamic programming. Define `dp[i][j]` as the length of the LCS of the prefixes `s1[0..i-1]` and `s2[0..j-1]`. Initialize a 2D array of size `(n+1) x (m+1)` with zeros. For each `i` from 1 to `n` and `j` from 1 to `m`, if `s1[i-1] == s2[j-1]`, then the current characters match, and we extend the LCS from the previous prefixes: `dp[i][j] = dp[i-1][j-1] + 1`. Otherwise, we take the better of skipping one character from either string: `dp[i][j] = max(dp[i-1][j], dp[i][j-1])`. The answer is `dp[n][m]`. Edge cases: if either string is empty, the LCS length is 0, which is handled by the zero-initialized base row/column. Time complexity is O(n*m) and space complexity is O(n*m) (can be optimized to O(min(n,m)) but not required for this task). The function should use `const` references to avoid copying input strings.
#include <string>
#include <vector>
#include <algorithm>

// Computes the length of the longest common subsequence of s1 and s2.
// n is the length of s1, m is the length of s2.
// Returns the LCS length as an integer.
int longestCommonSubsequence(const std::string& s1, const std::string& s2, int n, int m) {
    // dp[i][j] = LCS length of s1[0..i-1] and s2[0..j-1]
    std::vector<std::vector<int>> dp(n + 1, std::vector<int>(m + 1, 0));

    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= m; ++j) {
            if (s1[i - 1] == s2[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1] + 1;
            } else {
                dp[i][j] = std::max(dp[i - 1][j], dp[i][j - 1]);
            }
        }
    }

    return dp[n][m];
}
#include <cassert>
#include <string>

// Forward declaration for testing
int longestCommonSubsequence(const std::string& s1, const std::string& s2, int n, int m);

int main() {
    // Basic example from the analysis
    assert(longestCommonSubsequence("ABCBDAB", "BDCABA", 7, 6) == 4);

    // Empty strings
    assert(longestCommonSubsequence("", "abc", 0, 3) == 0);
    assert(longestCommonSubsequence("abc", "", 3, 0) == 0);
    assert(longestCommonSubsequence("", "", 0, 0) == 0);

    // Identical strings
    assert(longestCommonSubsequence("hello", "hello", 5, 5) == 5);

    // Completely different characters
    assert(longestCommonSubsequence("abc", "xyz", 3, 3) == 0);

    // Single character match
    assert(longestCommonSubsequence("a", "a", 1, 1) == 1);
    assert(longestCommonSubsequence("a", "b", 1, 1) == 0);

    // Case with spaces and punctuation
    std::string s1 = "a b c";
    std::string s2 = "x b y";
    assert(longestCommonSubsequence(s1, s2, 5, 5) == 1); // only 'b' matches

    // Longer case with repeated characters
    assert(longestCommonSubsequence("AAAA", "AA", 4, 2) == 2);
    assert(longestCommonSubsequence("AA", "AAAA", 2, 4) == 2);

    // Disjoint but same order subsequence
    assert(longestCommonSubsequence("abcdef", "acf", 6, 3) == 3);
    assert(longestCommonSubsequence("acf", "abcdef", 3, 6) == 3);

    return 0;
}
