// Write a C++ function `int longestCommonSubsequenceLength(const std::string& first, const std::string& second)` that computes the length of the longest common subsequence (LCS) between two input strings. A subsequence is a sequence derived by deleting some or no characters from the original string without changing the order of the remaining characters. The function must handle empty strings, strings with repeated characters, and arbitrary length inputs. Return the integer length of the LCS. Use dynamic programming with a 2D table where `dp[i][j]` represents the LCS length of the prefixes `first[0..i-1]` and `second[0..j-1]`. The solution should be iterative (bottom-up) and avoid recursion to eliminate stack overflow risks for large inputs.
The problem is solved using classic dynamic programming for LCS. We create a `(n+1) x (m+1)` table where `n = first.size()` and `m = second.size()`. The first row and column are all zeros because an empty string has LCS length 0 with any prefix. For each pair `(i, j)` from 1 to n and 1 to m, if the characters `first[i-1]` and `second[j-1]` match, then `dp[i][j] = dp[i-1][j-1] + 1` (extending the LCS of the previous prefixes). Otherwise, `dp[i][j] = max(dp[i-1][j], dp[i][j-1])` (taking the best LCS by skipping a character from either string). The answer is at `dp[n][m]`. Important edge cases: both strings empty (returns 0), one string empty (returns 0), strings with all identical characters (returns min length), and strings with no common characters (returns 0). Time complexity is `O(n*m)` and space complexity is `O(n*m)` for the table. The iterative approach avoids recursion depth issues and is straightforward to implement with `const` references to avoid copies.
#include <string>
#include <vector>
#include <algorithm>

// Compute the length of the longest common subsequence between two strings.
// Uses bottom-up dynamic programming with a 2D table.
int longestCommonSubsequenceLength(const std::string& first, const std::string& second) {
    const int n = static_cast<int>(first.size());
    const int m = static_cast<int>(second.size());
    
    // dp[i][j] holds LCS length for first[0..i-1] and second[0..j-1]
    std::vector<std::vector<int>> dp(n + 1, std::vector<int>(m + 1, 0));
    
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= m; ++j) {
            if (first[i - 1] == second[j - 1]) {
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

// Function declaration (link to solution)
int longestCommonSubsequenceLength(const std::string& first, const std::string& second);

int main() {
    // Basic examples
    assert(longestCommonSubsequenceLength("abcde", "ace") == 3);
    assert(longestCommonSubsequenceLength("abc", "abc") == 3);
    assert(longestCommonSubsequenceLength("abc", "def") == 0);
    
    // Empty strings
    assert(longestCommonSubsequenceLength("", "") == 0);
    assert(longestCommonSubsequenceLength("abc", "") == 0);
    assert(longestCommonSubsequenceLength("", "xyz") == 0);
    
    // Repeated characters and different lengths
    assert(longestCommonSubsequenceLength("aaaa", "aa") == 2);
    assert(longestCommonSubsequenceLength("ababab", "baba") == 4);
    
    // Order matters (not a substring)
    assert(longestCommonSubsequenceLength("a1b2c3", "abc123") == 3);
    assert(longestCommonSubsequenceLength("12345", "54321") == 1);
    
    // Larger strings with interleaving
    assert(longestCommonSubsequenceLength("AGGTAB", "GXTXAYB") == 4);
    
    // Strings with spaces and punctuation
    assert(longestCommonSubsequenceLength("hello world", "hero") == 3);
    
    return 0;
}
