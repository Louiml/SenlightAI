Write a C++ function `bool isRegexMatch(const std::string& s, const std::string& p)` that implements regular expression matching with support for two special characters: `.` matches any single character, and `*` matches zero or more of the preceding element (the preceding character or `.`). The function must return `true` if the entire string `s` matches the entire pattern `p`, otherwise `false`. The matching should be exact and cover the whole input string (not partial). The pattern may contain multiple `*` operators, and a `*` always follows a valid character or `.` (the input is guaranteed to be well-formed). Handle empty strings, patterns starting with `.*`, and patterns where `*` cancels the preceding character entirely. The function should use dynamic programming for efficiency.
#include <cassert>
#include <string>

// Declaration of the function under test
bool isRegexMatch(const std::string& s, const std::string& p);

int main() {
    // Basic no wildcard matches
    assert(isRegexMatch("abc", "abc") == true);
    assert(isRegexMatch("abc", "abd") == false);
    assert(isRegexMatch("", "") == true);
    
    // '.' wildcard
    assert(isRegexMatch("a", ".") == true);
    assert(isRegexMatch("ab", "a.") == true);
    assert(isRegexMatch("ab", ".") == false);
    
    // '*' zero or more
    assert(isRegexMatch("", "a*") == true);
    assert(isRegexMatch("aa", "a*") == true);
    assert(isRegexMatch("aab", "a*b") == true);
    assert(isRegexMatch("b", "a*b") == true);
    assert(isRegexMatch("a", "a*a") == true);
    assert(isRegexMatch("aa", "a*a") == true);
    assert(isRegexMatch("aaa", "a*a") == false); // three a's, pattern can match at most two then a final a => three a's
    
    // Combination of '.' and '*'
    assert(isRegexMatch("aa", ".*") == true);
    assert(isRegexMatch("ab", ".*") == true);
    assert(isRegexMatch("", ".*") == true);
    assert(isRegexMatch("abc", "a.c") == true);
    assert(isRegexMatch("acc", "a.c") == true);
    assert(isRegexMatch("ac", "a.c") == false); // needs exactly three characters
    
    // Multiple stars and edge cases
    assert(isRegexMatch("aaa", "a*a*a") == true); // three a's can be matched
    assert(isRegexMatch("mississippi", "mis*is*p*.") == true); // classic example
    assert(isRegexMatch("ab", ".*c") == false); // pattern requires a trailing c
    assert(isRegexMatch("a", "ab*") == true); // b* matches zero b's
    
    return 0;
}
#include <string>
#include <vector>

// Returns true if the entire string s matches the pattern p,
// where '.' matches any single character and '*' matches zero or more
// of the preceding element.
bool isRegexMatch(const std::string& s, const std::string& p) {
    const std::size_t n = s.size();
    const std::size_t m = p.size();
    
    // dp[i][j] = true if s[0..i-1] matches p[0..j-1]
    std::vector<std::vector<bool>> dp(n + 1, std::vector<bool>(m + 1, false));
    
    // Base case: empty string matches empty pattern
    dp[0][0] = true;
    
    // Base case: pattern can match empty string only if it consists of x* pairs
    for (std::size_t j = 2; j <= m; ++j) {
        if (p[j - 1] == '*') {
            dp[0][j] = dp[0][j - 2];
        }
    }
    
    for (std::size_t i = 1; i <= n; ++i) {
        for (std::size_t j = 1; j <= m; ++j) {
            if (p[j - 1] != '*') {
                // Direct character match or '.' wildcard
                dp[i][j] = (s[i - 1] == p[j - 1] || p[j - 1] == '.') && dp[i - 1][j - 1];
            } else {
                // '*' can match zero occurrences of the preceding character
                dp[i][j] = dp[i][j - 2];
                
                // Or it can match one or more if the preceding character matches s[i-1]
                if (s[i - 1] == p[j - 2] || p[j - 2] == '.') {
                    dp[i][j] = dp[i][j] || dp[i - 1][j];
                }
            }
        }
    }
    
    return dp[n][m];
}
// This is the classic "Regular Expression Matching" problem. We use a 2D dynamic programming table `dp[i][j]` where `i` is the length of the prefix of `s` considered (first `i` characters) and `j` is the length of the prefix of `p` considered (first `j` characters). `dp[i][j]` is `true` if `s[0..i-1]` matches `p[0..j-1]`.
//
// Base case: `dp[0][0] = true` (empty string matches empty pattern). For `j > 0`, `dp[0][j]` is `true` only if the pattern prefix can match an empty string, which happens when the pattern consists of pairs like `x*`, `x*y*`, etc. So we set `dp[0][j] = dp[0][j-2]` when `p[j-1] == '*'`.
//
// Recurrence: For `i >= 1` and `j >= 1`:
// - If `p[j-1] != '*'`: then `dp[i][j] = (s[i-1] == p[j-1] or p[j-1] == '.') and dp[i-1][j-1]`. The current character must match and the previous prefixes must match.
// - If `p[j-1] == '*'`: the `*` can match zero occurrences of `p[j-2]`, so `dp[i][j] = dp[i][j-2]` (ignore `p[j-2]` and `*`). Alternatively, if `p[j-2]` matches `s[i-1]` (i.e., `s[i-1] == p[j-2] or p[j-2] == '.'`), then we can consume one character from `s` and keep the same pattern: `dp[i][j] = dp[i][j] or dp[i-1][j]`.
//
// Edge cases: The pattern `.*` matches any string because `.` matches any character and `*` repeats it. Empty string with pattern like `a*` matches because `*` can take zero occurrences. Patterns like `a*a` work because the `*` can match multiple `a`s and then the final `a` matches.
//
// Time complexity: `O(n*m)` where `n` is the length of `s` and `m` is the length of `p`. Space complexity: `O(n*m)` for the DP table, which can be optimized to `O(m)` using two rows, but the standard solution uses the full table.
