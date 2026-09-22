Write a C++ function `longestCommonSubsequenceLength` that takes two strings and returns the length of their longest common subsequence (LCS) as an `int`. The function must not print anything; it must return the computed length. The input strings may be empty, may contain any printable ASCII characters (including spaces and punctuation), and may have lengths up to 1000. The function must handle cases where one or both strings are empty (in which case the LCS length is 0). Ensure the solution uses a standard dynamic programming table approach and does not modify the input strings.

// The problem is a classic dynamic programming application. Define `dp[i][j]` as the length of the LCS of the first `i` characters of string `a` and the first `j` characters of string `b` (using 1-based indexing). Initialize `dp[0][j] = 0` and `dp[i][0] = 0` for all valid `i, j` because an empty prefix has no common subsequence. Transition: if `a[i-1] == b[j-1]`, then `dp[i][j] = dp[i-1][j-1] + 1` (the current characters match, so we extend the LCS of the previous prefixes); otherwise `dp[i][j] = max(dp[i-1][j], dp[i][j-1])` (we skip one character from either string). The answer is `dp[a.length()][b.length()]`. Since the lengths can be up to 1000, a 2D table of size `(n+1) x (m+1)` is feasible (about 1 million entries), but to be memory-efficient we can use a rolling array of size `m+1` because each row only depends on the previous row. Edge cases: empty inputs, identical strings, completely different strings, and cases where characters repeat. Time complexity is O(n*m) for the two nested loops; space complexity is O(m) with the rolling array optimization (or O(n*m) with the full table). The solution must be `const`‑correct (the parameters are passed by `const std::string&`).

#include <string>
#include <vector>
#include <algorithm>

// Returns the length of the longest common subsequence of two strings.
int longestCommonSubsequenceLength(const std::string& a, const std::string& b) {
    int n = a.size();
    int m = b.size();
    
    // Use a rolling array of size m+1 to store the DP row.
    std::vector<int> dp(m + 1, 0);
    std::vector<int> prev(m + 1, 0);
    
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= m; ++j) {
            if (a[i - 1] == b[j - 1]) {
                dp[j] = prev[j - 1] + 1;
            } else {
                dp[j] = std::max(prev[j], dp[j - 1]);
            }
        }
        std::swap(dp, prev);
    }
    
    // After the last iteration, 'prev' contains the final row.
    return prev[m];
}

#include <cassert>
#include <string>

// Declaration for the function under test (already provided in solution).
int longestCommonSubsequenceLength(const std::string& a, const std::string& b);

int main() {
    // Basic cases
    assert(longestCommonSubsequenceLength("abc", "abc") == 3);
    assert(longestCommonSubsequenceLength("abc", "def") == 0);
    assert(longestCommonSubsequenceLength("", "abc") == 0);
    assert(longestCommonSubsequenceLength("abc", "") == 0);
    assert(longestCommonSubsequenceLength("", "") == 0);
    
    // Classic LCS cases
    assert(longestCommonSubsequenceLength("AGGTAB", "GXTXAYB") == 4); // "GTAB"
    assert(longestCommonSubsequenceLength("ABCDGH", "AEDFHR") == 3);   // "ADH"
    assert(longestCommonSubsequenceLength("ABCBDAB", "BDCABA") == 4);  // "BCAB" or "BDAB"
    
    // Repeated characters
    assert(longestCommonSubsequenceLength("AAA", "AA") == 2);
    assert(longestCommonSubsequenceLength("AAB", "ABA") == 2);
    
    // Strings with spaces and punctuation
    assert(longestCommonSubsequenceLength("a b c", "abc") == 3);
    assert(longestCommonSubsequenceLength("hello world", "world hello") == 5); // "world" or "hello"
    
    return 0;
}
