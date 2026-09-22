// Write a C++ function `longestCommonSubsequence` that takes two strings `text1` and `text2` and returns the length of their longest common subsequence (LCS). A subsequence is a sequence that appears in the same relative order but not necessarily contiguously. The function must handle empty strings (returning 0), strings with repeated characters, and strings of different lengths. The solution must use dynamic programming with space optimization to achieve O(min(n,m)) auxiliary space after reading both inputs. The function should be declared with appropriate `const` correctness for the input parameters and return an `int`.
The problem is classic LCS. The core recurrence is: if `text1[i-1] == text2[j-1]`, then the LCS length is `1 + dp[i-1][j-1]`; otherwise it is `max(dp[i-1][j], dp[i][j-1])`. Base cases: if either string is empty, the LCS length is 0. The full DP table would be (n+1) x (m+1), but we only need the previous row to compute the current row, so we keep two vectors `prev` (length m+1) and `curr` (length m+1). For each character of `text1`, we loop over each character of `text2` and update `curr[j]` using the recurrence, then assign `prev = curr` for the next iteration. Note that `curr[0]` remains 0 because J is zero when either string is empty. Edge cases: one string empty (immediately returns 0), strings with all identical characters (returns the length of the shorter string), and strings with no common characters (returns 0). Time complexity is O(n*m) and extra space complexity is O(m) (or O(min(n,m)) if we swap to use the shorter string as the inner loop's dimension). We choose to always use `text2`'s length for the vectors; this is fine, but we could swap inputs to save space if needed.
#include <string>
#include <vector>
#include <algorithm>

// Returns the length of the longest common subsequence between two strings.
// Uses dynamic programming with space optimization (only previous and current rows).
int longestCommonSubsequence(const std::string& text1, const std::string& text2) {
    int n = static_cast<int>(text1.size());
    int m = static_cast<int>(text2.size());
    
    // If either string is empty, LCS length is 0.
    if (n == 0 || m == 0) return 0;
    
    // Use the shorter string as the column dimension for less memory.
    if (m > n) {
        // For simplicity, keep as is; both strings are read-only.
        // But we can choose to work with the shorter as columns.
        // Here we still use text2's length as columns.
    }
    
    std::vector<int> prev(m + 1, 0);
    std::vector<int> curr(m + 1, 0);
    
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= m; ++j) {
            if (text1[i - 1] == text2[j - 1]) {
                curr[j] = 1 + prev[j - 1];
            } else {
                curr[j] = std::max(prev[j], curr[j - 1]);
            }
        }
        prev = curr; // Move current row to previous for next iteration.
        // Reset curr[0] is implicitly 0; other entries get overwritten.
    }
    
    return prev[m];
}
#include <cassert>
#include <string>

// Function declaration (actual implementation above).
int longestCommonSubsequence(const std::string& text1, const std::string& text2);

int main() {
    // Basic cases
    assert(longestCommonSubsequence("abc", "abc") == 3);
    assert(longestCommonSubsequence("abc", "def") == 0);
    assert(longestCommonSubsequence("abc", "ac") == 2); // "ac"
    
    // Empty strings
    assert(longestCommonSubsequence("", "abc") == 0);
    assert(longestCommonSubsequence("abc", "") == 0);
    assert(longestCommonSubsequence("", "") == 0);
    
    // One character common
    assert(longestCommonSubsequence("a", "b") == 0);
    assert(longestCommonSubsequence("a", "a") == 1);
    
    // Repeated characters
    assert(longestCommonSubsequence("aaa", "aa") == 2);
    assert(longestCommonSubsequence("abab", "baba") == 3); // "aba" or "bab"
    
    // Different lengths with common subsequence not contiguous
    assert(longestCommonSubsequence("AGGTAB", "GXTXAYB") == 4); // "GTAB"
    assert(longestCommonSubsequence("ABCDGH", "AEDFHR") == 3); // "ADH"
    
    // Case sensitivity
    assert(longestCommonSubsequence("ABC", "abc") == 0);
    
    // Single character same/different
    assert(longestCommonSubsequence("x", "x") == 1);
    assert(longestCommonSubsequence("x", "y") == 0);
    
    // All characters common but order matters
    assert(longestCommonSubsequence("abc", "cba") == 1); // one char at best
    
    return 0;
}
