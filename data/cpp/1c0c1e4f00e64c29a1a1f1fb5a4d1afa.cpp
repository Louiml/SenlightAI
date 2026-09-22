/*
Write a C++ function `std::string longestCommonSubsequence(const std::string& X, const std::string& Y)` that returns the longest common subsequence (LCS) of two input strings `X` and `Y` as a string. If multiple LCS exist, return any one of them. If no common subsequence exists (i.e., the LCS length is 0), return an empty string. The function must handle arbitrary UTF-8 characters? No — assume ASCII input for simplicity. The function must not modify the inputs and should be const-correct. The solution must implement the classic dynamic programming approach with backtracking, not a recursive memoization, to ensure deterministic behavior and clear complexity.
*/
#include <string>
#include <vector>
#include <algorithm>

// Returns the longest common subsequence of two input strings.
// If multiple LCS exist, any one is returned. Empty string if none.
std::string longestCommonSubsequence(const std::string& X, const std::string& Y) {
    const int m = static_cast<int>(X.size());
    const int n = static_cast<int>(Y.size());

    // dp[i][j] = length of LCS of X[0..i-1] and Y[0..j-1]
    std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1, 0));

    for (int i = 1; i <= m; ++i) {
        for (int j = 1; j <= n; ++j) {
            if (X[i - 1] == Y[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1] + 1;
            } else {
                dp[i][j] = std::max(dp[i - 1][j], dp[i][j - 1]);
            }
        }
    }

    // Reconstruct the LCS by backtracking
    int i = m;
    int j = n;
    std::string result;
    result.reserve(dp[m][n]);

    while (i > 0 && j > 0) {
        if (X[i - 1] == Y[j - 1]) {
            // This character is part of the LCS
            result.push_back(X[i - 1]);
            --i;
            --j;
        } else if (dp[i - 1][j] >= dp[i][j - 1]) {
            // Move up (tie-breaking: prefer up)
            --i;
        } else {
            // Move left
            --j;
        }
    }

    // The result was built in reverse order (from end to start), so reverse it
    std::reverse(result.begin(), result.end());
    return result;
}
#include <cassert>
#include <string>

// Declaration of the function (already defined above, just for clarity)
std::string longestCommonSubsequence(const std::string& X, const std::string& Y);

int main() {
    // Basic examples
    assert(longestCommonSubsequence("ABABCBC", "BCACB") == "BABC");
    assert(longestCommonSubsequence("ABCBDAB", "BDCABA") == "BCAB");  // one valid LCS
    assert(longestCommonSubsequence("ABCDEF", "ACDF") == "ACDF");
    assert(longestCommonSubsequence("ABCDEF", "GHIJK") == "");

    // Edge cases
    assert(longestCommonSubsequence("", "") == "");
    assert(longestCommonSubsequence("abc", "") == "");
    assert(longestCommonSubsequence("", "xyz") == "");

    // Repeated characters
    assert(longestCommonSubsequence("AAAA", "AAAA") == "AAAA");
    assert(longestCommonSubsequence("AAA", "AA") == "AA");

    // No common characters but length > 0
    assert(longestCommonSubsequence("abc", "def") == "");

    // Single character
    assert(longestCommonSubsequence("x", "x") == "x");
    assert(longestCommonSubsequence("x", "y") == "");

    // Multiple LCS: "abc" and "acb" -> LCS "ab" or "ac", both length 2, but our backtracking yields "ab"
    std::string result = longestCommonSubsequence("abc", "acb");
    assert(result == "ab" || result == "ac");

    // Long string with mixed characters (ensures no crash)
    std::string longX = "AGGTAB";
    std::string longY = "GXTXAYB";
    assert(longestCommonSubsequence(longX, longY) == "GTAB");

    return 0;
}
// The problem is solved using dynamic programming. We build a 2D table `dp` of size `(m+1) x (n+1)`, where `dp[i][j]` stores the length of the LCS of the first `i` characters of `X` and the first `j` characters of `Y`. The recurrence is:
// - If `i == 0` or `j == 0`, then `dp[i][j] = 0`.
// - If `X[i-1] == Y[j-1]`, then `dp[i][j] = dp[i-1][j-1] + 1`.
// - Otherwise, `dp[i][j] = max(dp[i-1][j], dp[i][j-1])`.
//
// After filling the table, `dp[m][n]` gives the length of the LCS. To reconstruct the actual string, we start from `i = m`, `j = n` and move backwards:
// - If the characters match (`X[i-1] == Y[j-1]`), we include that character in the result (prepend it) and move diagonally up-left (`i--`, `j--`).
// - Otherwise, we move in the direction of the larger value: if `dp[i-1][j] >= dp[i][j-1]`, move up (`i--`); else move left (`j--`). This yields a valid LCS.
//
// Edge cases: empty strings (returns empty string), no common characters (returns empty string), strings with all identical characters (returns the whole string), and cases where multiple LCS exist — our backtracking chooses one deterministically based on the tie-breaking rule (prefer moving up when equal). Time complexity is O(m*n) for filling the table and O(m+n) for backtracking, total O(m*n). Space complexity is O(m*n) for the table. For large inputs, we could optimize space to O(n) for length only, but reconstruction requires full table; the problem expects this classic approach.
