/*
Write a C++ function `bool regexMatch(const std::string& s, const std::string& p)` that determines whether the entire string `s` matches the pattern `p`, where `p` supports two special characters: `.` matches any single character, and `*` matches zero or more of the preceding element. The pattern is assumed to be valid (i.e., a `*` always follows a non-`*` character, and `*` never appears at the start). The function must evaluate the full match (not a partial substring match), and must handle empty strings and patterns correctly. Implement a dynamic programming solution using a 2D boolean table, where `dp[i][j]` indicates whether the first `i` characters of `s` match the first `j` characters of `p`. Ensure the function is `const`-correct and free of any side effects.
*/
#include <string>
#include <vector>

// Determine if the entire string s matches pattern p where '.' matches any char and '*' matches zero or more of preceding char.
bool regexMatch(const std::string& s, const std::string& p) {
    int m = s.length();
    int n = p.length();
    
    std::vector<std::vector<bool>> dp(m + 1, std::vector<bool>(n + 1, false));
    
    dp[0][0] = true; // empty string matches empty pattern
    
    // For empty string, only patterns like a*, a*b*, etc. can match
    for (int j = 1; j <= n; ++j) {
        if (p[j - 1] == '*') {
            dp[0][j] = (j >= 2) && dp[0][j - 2];
        }
    }
    
    for (int i = 1; i <= m; ++i) {
        for (int j = 1; j <= n; ++j) {
            if (p[j - 1] == '*') {
                // Either skip the pattern pair or use '*' to match one more char
                dp[i][j] = dp[i][j - 2] || 
                           ((s[i - 1] == p[j - 2] || p[j - 2] == '.') && dp[i - 1][j]);
            } else {
                // Direct character match (literal or '.')
                dp[i][j] = (p[j - 1] == '.' || s[i - 1] == p[j - 1]) && dp[i - 1][j - 1];
            }
        }
    }
    
    return dp[m][n];
}
#include <cassert>

int main() {
    // Basic matches
    assert(regexMatch("aa", "a") == false);
    assert(regexMatch("aa", "a*") == true);
    assert(regexMatch("ab", ".*") == true);
    
    // Empty string cases
    assert(regexMatch("", "") == true);
    assert(regexMatch("", "a*") == true);
    assert(regexMatch("", "a*b*c*") == true);
    assert(regexMatch("", "a") == false);
    
    // Examples from the problem statement
    assert(regexMatch("aab", "c*a*b") == true);
    assert(regexMatch("mississippi", "mis*is*p*.") == false);
    
    // Literal and dot combinations
    assert(regexMatch("abc", "a.c") == true);
    assert(regexMatch("abc", "a.c.") == false);
    assert(regexMatch("aaa", "a*") == true);
    assert(regexMatch("aaa", "aa*") == true);
    assert(regexMatch("abcd", ".*") == true);
    assert(regexMatch("abc", "a.*c") == true);
    
    // Edge cases with leading/trailing star patterns
    assert(regexMatch("bbbba", ".*a*a") == true);
    assert(regexMatch("ab", ".*c") == false);
    assert(regexMatch("a", "ab*") == true);
    assert(regexMatch("b", "ab*") == false);
    
    return 0;
}
// We use a bottom-up dynamic programming approach. Define `dp[i][j]` as true if the substring `s[0..i-1]` matches the pattern prefix `p[0..j-1]`. Initialize `dp[0][0] = true` (empty matches empty). For `i > 0`, `dp[i][0] = false` because a non-empty string cannot match an empty pattern. For `j > 0`, `dp[0][j]` is true only if `p[j-1] == '*'` and `dp[0][j-2]` is true (i.e., the `*` matches zero occurrences of the preceding character). Then, for each `i` from 1 to `m` and `j` from 1 to `n`, we have two cases:
// - If `p[j-1] == '*'`: the pattern can match either by skipping the preceding character and `*` (i.e., `dp[i][j-2]`), or by using the `*` to match one or more occurrences of the preceding character. The latter is valid if `s[i-1]` matches `p[j-2]` (where `p[j-2]` could be `.` or a literal) and `dp[i-1][j]` is true (i.e., the rest of `s` up to `i-1` already matched the same pattern). Thus `dp[i][j] = dp[i][j-2] || ((s[i-1]==p[j-2] || p[j-2]=='.') && dp[i-1][j])`.
// - Otherwise (not `*`): `dp[i][j]` is true only if `p[j-1]` matches `s[i-1]` (either `p[j-1]=='.'` or literal equality) and `dp[i-1][j-1]` is true.
//
// Edge cases include empty input string (with pattern like `a*` matching empty), patterns with multiple `*` sequences, and patterns that are entirely `*`-driven. The DP table size is `(m+1) x (n+1)`. Time complexity is `O(m * n)` and space complexity is also `O(m * n)`.
