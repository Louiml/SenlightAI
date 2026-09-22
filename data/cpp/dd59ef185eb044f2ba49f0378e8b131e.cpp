/*
Write a C++ function named `longestCommonSubsequenceLength` that takes two strings, `s` and `t`, and returns the length of their longest common subsequence (LCS). A subsequence is a sequence that appears in the same relative order but not necessarily contiguously. The function must handle empty strings and strings containing any printable ASCII characters, including spaces and punctuation. It should be implemented using dynamic programming with a bottom-up (iterative) approach, not recursion. The function must be `const`-correct, meaning it should accept the input strings by `const` reference and not modify them. The function should return an `int` representing the length of the LCS.
*/
#include <string>
#include <vector>
#include <algorithm>

// Returns the length of the longest common subsequence of s and t.
int longestCommonSubsequenceLength(const std::string& s, const std::string& t) {
    int n = static_cast<int>(s.size());
    int m = static_cast<int>(t.size());
    
    // DP table where dp[i][j] = LCS length for s[0..i-1] and t[0..j-1]
    std::vector<std::vector<int>> dp(n + 1, std::vector<int>(m + 1, 0));
    
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= m; ++j) {
            if (s[i - 1] == t[j - 1]) {
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

// Assume the solution function is declared above.

int main() {
    // Basic cases
    assert(longestCommonSubsequenceLength("ABCBDAB", "BDCAB") == 4); // LCS: "BCAB" or "BDAB"
    assert(longestCommonSubsequenceLength("", "abc") == 0);
    assert(longestCommonSubsequenceLength("abc", "") == 0);
    assert(longestCommonSubsequenceLength("", "") == 0);
    
    // Single character strings
    assert(longestCommonSubsequenceLength("a", "a") == 1);
    assert(longestCommonSubsequenceLength("a", "b") == 0);
    
    // Identical strings
    assert(longestCommonSubsequenceLength("hello", "hello") == 5);
    assert(longestCommonSubsequenceLength("abc", "abc") == 3);
    
    // Strings with spaces and punctuation
    assert(longestCommonSubsequenceLength("a b c", "a b d") == 3); // LCS: "a b"
    assert(longestCommonSubsequenceLength("!@#", "@#!") == 2); // LCS: "@#"
    
    // Repeated characters and different lengths
    assert(longestCommonSubsequenceLength("AAAA", "AA") == 2);
    assert(longestCommonSubsequenceLength("ABC", "DEF") == 0);
    assert(longestCommonSubsequenceLength("AGGTAB", "GXTXAYB") == 4); // LCS: "GTAB"
    
    return 0;
}
// The solution uses a classic bottom-up dynamic programming approach. We define a 2D DP table `dp[i][j]` where `i` ranges from 0 to `s.length()` and `j` ranges from 0 to `t.length()`. The entry `dp[i][j]` represents the length of the LCS of the prefixes `s[0..i-1]` and `t[0..j-1]`. The base case is that if either string is empty (`i==0` or `j==0`), the LCS length is 0. For each pair `(i,j)` starting from 1, we compare `s[i-1]` and `t[j-1]`: if they are equal, we add 1 to the diagonal value `dp[i-1][j-1]`; otherwise, we take the maximum of the value from the cell above (`dp[i-1][j]`) and the cell to the left (`dp[i][j-1]`). This correctly captures the optimal substructure. Edge cases include empty strings (returns 0), strings of different lengths, repeated characters, and strings with no common characters. Time complexity is `O(n*m)` where `n` and `m` are the lengths of the strings, and space complexity is also `O(n*m)` for the DP table. The approach is efficient for typical string lengths and avoids recursion overhead and stack overflow risks.
