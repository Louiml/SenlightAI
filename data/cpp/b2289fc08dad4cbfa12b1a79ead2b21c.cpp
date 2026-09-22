// Write a C++ function that takes two strings as input and returns the length of their longest common subsequence (LCS). The strings may contain uppercase and lowercase letters, digits, spaces, and punctuation, and may be empty. The function should handle strings of length up to 1000 characters efficiently. For example, given `"abc"` and `"ac"`, the LCS length is 2 (`"ac"`), and given `"ABC"` and `"abc"`, it is 0 because the comparison is case-sensitive. The return type should be an integer.
The problem is solved using dynamic programming. Define `dp[i][j]` as the length of the LCS of the first `i` characters of string `a` and the first `j` characters of string `b`. The base cases are `dp[0][j] = 0` and `dp[i][0] = 0` since an empty string has no common subsequence with anything. For `i` and `j` from 1 upward, if the characters `a[i-1]` and `b[j-1]` match, then `dp[i][j] = dp[i-1][j-1] + 1` (extending the LCS of the prefixes without these characters). Otherwise, we take the better of skipping the last character of `a` (`dp[i-1][j]`) or skipping the last character of `b` (`dp[i][j-1]`), i.e., `dp[i][j] = max(dp[i-1][j], dp[i][j-1])`. The answer is `dp[a.size()][b.size()]`. Edge cases include empty strings (returns 0) and strings with no common characters (returns 0). The time complexity is O(n*m) where n and m are the lengths of the inputs, and the space complexity is O(n*m) if we store the full table, but it can be optimized to O(min(n,m)) using two rows. Here we use the full table for clarity, which is acceptable for n,m ≤ 1000 (1e6 cells).
#include <string>
#include <vector>
#include <algorithm>

// Return the length of the longest common subsequence of a and b.
int longestCommonSubsequenceLength(const std::string& a, const std::string& b) {
    const auto n = a.size();
    const auto m = b.size();
    // dp[i][j] = LCS length of a[0..i-1] and b[0..j-1].
    std::vector<std::vector<int>> dp(n + 1, std::vector<int>(m + 1, 0));
    for (std::size_t i = 1; i <= n; ++i) {
        for (std::size_t j = 1; j <= m; ++j) {
            if (a[i - 1] == b[j - 1]) {
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

// Include the solution function's declaration (or paste the definition here).
// For brevity, the function is assumed to be declared above.

int main() {
    assert(longestCommonSubsequenceLength("abc", "ac") == 2);
    assert(longestCommonSubsequenceLength("ABC", "abc") == 0);
    assert(longestCommonSubsequenceLength("", "anything") == 0);
    assert(longestCommonSubsequenceLength("abc", "") == 0);
    assert(longestCommonSubsequenceLength("abcdef", "ace") == 3);
    assert(longestCommonSubsequenceLength("AGGTAB", "GXTXAYB") == 4);
    assert(longestCommonSubsequenceLength("abc", "abc") == 3);
    assert(longestCommonSubsequenceLength("ab", "ba") == 1);
    assert(longestCommonSubsequenceLength("hello world", "world hello") == 5); // "world" or "hello"
    assert(longestCommonSubsequenceLength("a b c", "abc") == 3); // spaces are characters too
}
