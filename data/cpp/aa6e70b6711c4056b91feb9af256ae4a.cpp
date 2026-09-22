// Write a C++ function that takes two strings `s1` and `s2` and returns the length of their **longest common subsequence** (LCS). The function must handle arbitrary non-empty strings that may include uppercase/lowercase letters, digits, spaces, and punctuation. The LCS is defined as the longest sequence of characters that appears in the same relative order in both strings, but not necessarily contiguously. The function should be case-sensitive (e.g., `'A'` and `'a'` are different). For example, for `s1 = "abcde"` and `s2 = "ace"`, the LCS is `"ace"` of length 3. For `s1 = "AGGTAB"` and `s2 = "GXTXAYB"`, the LCS is `"GTAB"` of length 4. The function must be efficient for strings up to length 1000.

The problem is a classic dynamic programming (DP) task. Define a 2D table `dp[i][j]` where `i` ranges from 0 to `len1` and `j` from 0 to `len2`, representing the length of the LCS of the first `i` characters of `s1` and the first `j` characters of `s2`. Initialize all `dp[i][0] = 0` and `dp[0][j] = 0` because an empty string has no common subsequence with anything. Then fill the table iteratively: if `s1[i-1] == s2[j-1]`, then `dp[i][j] = dp[i-1][j-1] + 1` (match the last characters and extend the LCS); otherwise, `dp[i][j] = max(dp[i-1][j], dp[i][j-1])` (skip one character from either string and take the best). The result is `dp[len1][len2]`. Edge cases: one string empty (returns 0), identical strings (returns length), strings with only common characters but in different order (returns the length of the longest common subsequence, not substring). Time complexity is `O(len1 * len2)` and space complexity is `O(len1 * len2)` for the table; with the constraint of length up to 1000, this is at most 1,000,000 cells, which is acceptable.

#include <string>
#include <vector>
#include <algorithm>

// Returns the length of the longest common subsequence of s1 and s2.
// Uses dynamic programming with a (len1+1) x (len2+1) table.
int longestCommonSubsequenceLength(const std::string& s1, const std::string& s2) {
    const int len1 = static_cast<int>(s1.size());
    const int len2 = static_cast<int>(s2.size());

    // Initialize DP table with zeros. dp[i][j] = LCS length of s1[0..i-1] and s2[0..j-1].
    std::vector<std::vector<int>> dp(len1 + 1, std::vector<int>(len2 + 1, 0));

    for (int i = 1; i <= len1; ++i) {
        for (int j = 1; j <= len2; ++j) {
            if (s1[i - 1] == s2[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1] + 1;
            } else {
                dp[i][j] = std::max(dp[i - 1][j], dp[i][j - 1]);
            }
        }
    }

    return dp[len1][len2];
}

#include <cassert>
#include <string>

// Declaration of the function under test (assume it is in the same file above).
int longestCommonSubsequenceLength(const std::string& s1, const std::string& s2);

int main() {
    // Basic examples
    assert(longestCommonSubsequenceLength("abcde", "ace") == 3);
    assert(longestCommonSubsequenceLength("AGGTAB", "GXTXAYB") == 4);

    // Empty string cases
    assert(longestCommonSubsequenceLength("", "") == 0);
    assert(longestCommonSubsequenceLength("abc", "") == 0);
    assert(longestCommonSubsequenceLength("", "xyz") == 0);

    // Identical strings
    assert(longestCommonSubsequenceLength("hello", "hello") == 5);

    // No common characters
    assert(longestCommonSubsequenceLength("abc", "def") == 0);

    // Case sensitivity
    assert(longestCommonSubsequenceLength("Abc", "abc") == 2); // "bc" or "ac"

    // Reversed order (still subsequence, not substring)
    assert(longestCommonSubsequenceLength("abc", "cba") == 1); // e.g., 'a', 'b', or 'c'

    // All same characters, different lengths
    assert(longestCommonSubsequenceLength("aaaa", "aa") == 2);

    // Longer test with spaces and punctuation
    assert(longestCommonSubsequenceLength("hello world", "h e l l o w o r l d") == 10); // "helloworld" but with extra spaces removed for LCS? Actually check: common subsequence is "helloworld" length 10, but the second string has spaces. Let's compute: "hello world" has letters h,e,l,l,o,w,o,r,l,d (10 letters) and spaces. The second string "h e l l o w o r l d" has same letters in order, so LCS length is 10.

    return 0;
}
