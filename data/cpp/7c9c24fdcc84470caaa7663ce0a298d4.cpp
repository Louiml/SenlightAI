// Write a C++ function named `shortestCommonSupersequenceLength` that takes two strings `s1` and `s2` and returns the length of the shortest string that has both `s1` and `s2` as subsequences. A supersequence is a string that contains the original strings as subsequences (not necessarily contiguous). The function should handle empty strings, strings of different lengths, and strings with repeated characters. The returned value is an integer representing the minimal length of such a supersequence. For example, for `s1 = "abac"` and `s2 = "cab"`, the shortest supersequence is `"cabac"` (length 5), and the function should return `5`.

The problem is a classic dynamic programming task. The shortest common supersequence length can be derived from the longest common subsequence (LCS) of the two strings. If we remove the common characters (the LCS) and keep the remaining characters in order, the minimal supersequence length is `M + N - LCS_length`, where `M` and `N` are the lengths of the strings. To compute LCS, we build a 2D DP table `dp[i][j]` where `dp[i][j]` is the length of the LCS of the first `i` characters of `s1` and first `j` characters of `s2`. Base case: if either `i` or `j` is 0, the LCS is 0. Transition: if the current characters match (`s1[i-1] == s2[j-1]`), then `dp[i][j] = 1 + dp[i-1][j-1]`; otherwise, `dp[i][j] = max(dp[i-1][j], dp[i][j-1])`. The answer is `n + m - dp[n][m]`. Edge cases: empty strings – if either is empty, the supersequence length is the length of the other string; strings with no common characters – the supersequence is the concatenation of both, giving length `M+N`. Time complexity is `O(M*N)` and space complexity is `O(M*N)` for the DP table, though it can be optimized to `O(min(M,N))` but the task asks for a clean, correct solution.

#include <string>
#include <vector>
#include <algorithm>

// Returns the length of the shortest common supersequence of s1 and s2.
int shortestCommonSupersequenceLength(const std::string& s1, const std::string& s2) {
    int n = static_cast<int>(s1.size());
    int m = static_cast<int>(s2.size());

    // dp[i][j] = length of LCS of s1[0..i-1] and s2[0..j-1]
    std::vector<std::vector<int>> dp(n + 1, std::vector<int>(m + 1, 0));

    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= m; ++j) {
            if (s1[i - 1] == s2[j - 1]) {
                dp[i][j] = 1 + dp[i - 1][j - 1];
            } else {
                dp[i][j] = std::max(dp[i - 1][j], dp[i][j - 1]);
            }
        }
    }

    int lcsLength = dp[n][m];
    return n + m - lcsLength;
}

#include <cassert>
#include <string>

int shortestCommonSupersequenceLength(const std::string& s1, const std::string& s2);

int main() {
    // Basic cases
    assert(shortestCommonSupersequenceLength("abac", "cab") == 5);
    assert(shortestCommonSupersequenceLength("abc", "abc") == 3);
    assert(shortestCommonSupersequenceLength("abc", "def") == 6);

    // Empty strings
    assert(shortestCommonSupersequenceLength("", "") == 0);
    assert(shortestCommonSupersequenceLength("abc", "") == 3);
    assert(shortestCommonSupersequenceLength("", "xyz") == 3);

    // Repeated characters and overlapping
    assert(shortestCommonSupersequenceLength("aaaa", "aa") == 4);
    assert(shortestCommonSupersequenceLength("abcbd", "bcb") == 5);
    assert(shortestCommonSupersequenceLength("geek", "eke") == 5);
    assert(shortestCommonSupersequenceLength("abcd", "bcd") == 4);

    // Uncommon but valid
    assert(shortestCommonSupersequenceLength("a", "b") == 2);
    assert(shortestCommonSupersequenceLength("x", "x") == 1);

    return 0;
}
