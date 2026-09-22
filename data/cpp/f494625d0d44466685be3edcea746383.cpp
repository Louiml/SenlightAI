// Write a C++ function `bool wildcardMatch(const std::string& s, const std::string& p)` that implements wildcard pattern matching with the following rules: a `?` in the pattern matches exactly one character in the string, a `*` in the pattern matches any sequence of characters (including the empty sequence), and all other characters must match exactly. The function should return `true` if the entire string matches the pattern, and `false` otherwise. The input strings may be empty, contain lowercase letters only (for simplicity), and the pattern may contain multiple `*` characters consecutively or at any position. The function must handle cases where the pattern is empty, the string is empty, or both are empty, and must efficiently handle patterns with many `*` characters without exponential time.
#include <cassert>
#include <string>

// Known function declaration (provided above)
bool wildcardMatch(const std::string& s, const std::string& p);

int main() {
    // Basic matching
    assert(wildcardMatch("abc", "abc") == true);
    assert(wildcardMatch("abc", "a?c") == true);
    assert(wildcardMatch("abc", "a*c") == true);
    assert(wildcardMatch("abc", "*") == true);
    assert(wildcardMatch("abc", "abc*") == true);
    assert(wildcardMatch("abc", "*abc") == true);
    
    // Empty cases
    assert(wildcardMatch("", "") == true);
    assert(wildcardMatch("", "*") == true);
    assert(wildcardMatch("", "?") == false);
    assert(wildcardMatch("a", "") == false);
    
    // Non-matches
    assert(wildcardMatch("abc", "abd") == false);
    assert(wildcardMatch("abc", "a?d") == false);
    assert(wildcardMatch("ab", "a*b*c") == false);
    
    // Multiple stars and tricky patterns
    assert(wildcardMatch("aaab", "a*b") == true);
    assert(wildcardMatch("aaab", "a*ab") == true);
    assert(wildcardMatch("abc", "a**c") == true);
    assert(wildcardMatch("abc", "*?*") == true);
    assert(wildcardMatch("abc", "?b?") == true);
    assert(wildcardMatch("abcd", "a*d?") == true);
    
    // Longer string with pattern that can't match
    assert(wildcardMatch("abcdef", "a*f") == true);
    assert(wildcardMatch("abcdef", "a*g") == false);
    assert(wildcardMatch("hello", "h*o*") == true);
    assert(wildcardMatch("hello", "h*o?") == false);
    
    return 0;
}
#include <string>
#include <vector>

// Memoized recursive helper for wildcard matching.
// Checks if s[0..sIndex] matches p[0..pIndex].
bool solveWildcard(int sIndex, int pIndex, const std::string& s, const std::string& p, std::vector<std::vector<int>>& dp) {
    // Base cases
    if (sIndex < 0 && pIndex < 0) return true;
    if (sIndex < 0) {
        // String exhausted; pattern must have only '*' remaining
        while (pIndex >= 0) {
            if (p[pIndex] != '*') return false;
            pIndex--;
        }
        return true;
    }
    if (pIndex < 0) return false; // Pattern exhausted but string not

    if (dp[sIndex][pIndex] != -1) return dp[sIndex][pIndex];

    if (s[sIndex] == p[pIndex] || p[pIndex] == '?') {
        dp[sIndex][pIndex] = solveWildcard(sIndex - 1, pIndex - 1, s, p, dp) ? 1 : 0;
    } else if (p[pIndex] == '*') {
        bool includeStar = solveWildcard(sIndex - 1, pIndex, s, p, dp); // '*' consumes one char
        bool excludeStar = solveWildcard(sIndex, pIndex - 1, s, p, dp);  // '*' matches empty
        dp[sIndex][pIndex] = (includeStar || excludeStar) ? 1 : 0;
    } else {
        dp[sIndex][pIndex] = 0;
    }
    return dp[sIndex][pIndex];
}

// Public function: returns true if the entire string s matches pattern p
bool wildcardMatch(const std::string& s, const std::string& p) {
    if (s.empty() && p.empty()) return true;
    if (p.empty()) return s.empty();
    if (s.empty()) {
        for (char c : p) if (c != '*') return false;
        return true;
    }
    std::vector<std::vector<int>> dp(s.size(), std::vector<int>(p.size(), -1));
    return solveWildcard(static_cast<int>(s.size()) - 1, static_cast<int>(p.size()) - 1, s, p, dp);
}
// The solution uses recursion with memoization (dynamic programming). We define a helper function `solve(sIndex, pIndex, s, p, dp)` that checks whether the substring `s[0..sIndex]` matches the pattern substring `p[0..pIndex]`, where indices go from 0 to length-1. Base conditions: if both indices are negative, return `true`; if the string is exhausted but the pattern is not (sIndex < 0, pIndex >= 0), the only way to match is if all remaining pattern characters are `*` (each `*` can match empty); if the pattern is exhausted but the string is not (sIndex >= 0, pIndex < 0), return `false`. For the recursive case, if the current characters match (either equal or pattern has `?`), we recurse on both indices decremented. If the pattern character is `*`, we have two choices: either `*` consumes one string character (recurse with sIndex-1, same pIndex) or `*` matches nothing (recurse with same sIndex, pIndex-1). The result is the OR of both. If characters don't match, return `false`. Memoization stores results in a 2D vector initialized to -1. This avoids recomputation, giving O(m*n) time and O(m*n) space, where m = s.length() and n = p.length(). Edge cases include empty strings, patterns that are only `*`, and patterns with trailing `*` after the string is exhausted. The implementation uses `const` references to avoid copying and recursion depth is at most m+n.
