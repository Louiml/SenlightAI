Write a C++ function that computes the length of the Longest Common Subsequence (LCS) of two given strings. The function should take two `std::string` parameters and return an `int`. It must handle empty strings gracefully (returning 0) and be case-sensitive. The solution should avoid recursion to be safe for large inputs; instead, use dynamic programming with a 2D table to compute the result iteratively. The function must be named `longestCommonSubsequenceLength` and be declared with proper `const` correctness (use `const std::string&` for parameters). The function should not print anything; it should only return the computed length.
The LCS problem is solved using dynamic programming. We define a 2D DP table `dp[i][j]` representing the length of the LCS of the first `i` characters of string `A` and the first `j` characters of string `B`. The recurrence is:
- If `A[i-1] == B[j-1]`, then `dp[i][j] = dp[i-1][j-1] + 1`
- Else, `dp[i][j] = max(dp[i-1][j], dp[i][j-1])`
- Base case: `dp[0][j] = 0` and `dp[i][0] = 0` for all `i, j`.

We fill the table row by row, iterating over all characters. Edge case: if either string is empty, the LCS length is 0. Also, if strings have different lengths, the table handles it naturally. The time complexity is O(m*n), where m and n are the lengths of the two strings, and the space complexity is O(m*n) if we keep the full table. However, we can optimize space to O(min(m,n)) by using two rolling vectors, but for clarity and correctness we'll keep the full table in the solution. The iterative approach avoids stack overflow that recursive solutions can cause for long strings.
#include <string>
#include <vector>
#include <algorithm>

// Compute the length of the longest common subsequence of two strings.
// Uses iterative dynamic programming with a 2D table.
// Returns 0 if either string is empty.
int longestCommonSubsequenceLength(const std::string& a, const std::string& b) {
    size_t m = a.size();
    size_t n = b.size();
    
    // dp[i][j] = LCS length for a[0..i-1] and b[0..j-1]
    std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1, 0));
    
    for (size_t i = 1; i <= m; ++i) {
        for (size_t j = 1; j <= n; ++j) {
            if (a[i - 1] == b[j - 1]) {
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
    assert(longestCommonSubsequenceLength("ABC", "ABC") == 3);
    assert(longestCommonSubsequenceLength("ABC", "DEF") == 0);
    assert(longestCommonSubsequenceLength("AGGTAB", "GXTXAYB") == 4); // "GTAB"
    assert(longestCommonSubsequenceLength("ABCDGH", "AEDFHR") == 3);  // "ADH"
    
    // Empty strings
    assert(longestCommonSubsequenceLength("", "ABC") == 0);
    assert(longestCommonSubsequenceLength("ABC", "") == 0);
    assert(longestCommonSubsequenceLength("", "") == 0);
    
    // Single character matches
    assert(longestCommonSubsequenceLength("a", "a") == 1);
    assert(longestCommonSubsequenceLength("a", "b") == 0);
    
    // Case sensitivity
    assert(longestCommonSubsequenceLength("abc", "ABC") == 0);
    
    // Longer strings with repeated characters
    assert(longestCommonSubsequenceLength("AAAA", "AA") == 2);
    assert(longestCommonSubsequenceLength("ACCGGTCGAGTGCGCGGAAGCCGGCCGAA", "GTCGTTCGGAATGCCGTTGCTCTGTAAA") == 20);
    
    // Strings with same content in different order
    assert(longestCommonSubsequenceLength("ABC", "CBA") == 1); // could be 'A', 'B', or 'C'
    
    return 0;
}
