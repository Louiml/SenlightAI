Given two non-empty lowercase strings `a` and `b`, write a C++ function `string wildcardMatchTemplate(const string& a, const string& b)` that returns the lexicographically smallest possible wildcard pattern (a string containing lowercase letters and the asterisk character `*`) that matches both strings under standard wildcard matching rules: `*` can match any sequence of characters (including an empty sequence), and literal letters match exactly one character. The pattern must match the entire strings. If no such pattern exists, return `"No"`. Assume both strings have length at least 1. If multiple valid patterns exist, prefer (in order): a pattern of the form `X*` (where X is the first character), then `*X` (where X is the last character), then `*XY*` (where XY is the first occurrence of a common two-letter substring in both strings). If none of these work, the answer is `"No"`. For example, for `a="hello"` and `b="help"`, a valid pattern is `"*he*"` because both contain the substring `"he"`. For `a="abc"` and `b="def"`, the answer is `"No"`. The function must be robust and not modify the input strings.

The key insight is that if two strings can be matched by a wildcard pattern, that pattern must either fix a common prefix, fix a common suffix, or fix a common contiguous two-character substring that appears in both. 
- If the first characters are equal, the pattern `a[0]*` matches both (since `*` swallows the rest of each string). 
- Else if the last characters are equal, the pattern `*a[last]` matches both. 
- Else, we must have a pattern like `*XY*` where `XY` is a common substring of length 2 that appears consecutively in both strings. If such a pair exists, the pattern `*XY*` matches both. 
- If none of these conditions hold, then any wildcard pattern would either need to match a prefix, suffix, or have at least one literal letter after a `*` and before another `*`; but without a common length-1 prefix, length-1 suffix, or common length-2 substring, it’s impossible. For example, consider `a="ab"` and `b="ba"`. They share no first or last character, and their length-2 substrings are `"ab"` and `"ba"`, which are different, so the answer is `"No"`. The algorithm iterates over all pairs of positions where `a[i]==b[j]` and `a[i+1]==b[j+1]` to find any common two-character substring. Time complexity is O(|a|*|b|) for the nested loop, and space complexity is O(1) auxiliary, ignoring the returned string. Edge cases include strings of length 1 (the loops for inner substrings must not run out of bounds, but if first/last check already fails, then for length 1 those checks fail because indices are equal and we compare a[0] and b[0], which we already know are not equal, so we output "No"). Also, note that the order of checks matters: we must check prefix before suffix before inner substring, as specified.

#include <string>
#include <iostream>

// Return a wildcard pattern that matches both strings, or "No" if none exists.
// The pattern is chosen in the priority order: prefix, suffix, inner common 2-char substring.
std::string wildcardMatchTemplate(const std::string& a, const std::string& b) {
    // Check for common first character
    if (a[0] == b[0]) {
        return std::string(1, a[0]) + "*";
    }

    // Check for common last character
    if (a.back() == b.back()) {
        return "*" + std::string(1, a.back());
    }

    // Check for any common two-character substring
    for (std::size_t i = 0; i + 1 < a.size(); ++i) {
        for (std::size_t j = 0; j + 1 < b.size(); ++j) {
            if (a[i] == b[j] && a[i + 1] == b[j + 1]) {
                return "*" + std::string(1, a[i]) + std::string(1, a[i + 1]) + "*";
            }
        }
    }

    return "No";
}

#include <cassert>
#include <string>

// The function under test is declared here (or included from header in real usage)
std::string wildcardMatchTemplate(const std::string& a, const std::string& b);

int main() {
    // Common first character
    assert(wildcardMatchTemplate("hello", "help") == "h*");
    assert(wildcardMatchTemplate("abc", "a") == "a*");

    // Common last character
    assert(wildcardMatchTemplate("test", "guest") == "*t");
    assert(wildcardMatchTemplate("x", "yx") == "*x");

    // Common inner two-letter substring
    assert(wildcardMatchTemplate("hello", "help") == "h*"); // priority: prefix first
    assert(wildcardMatchTemplate("abcd", "xbcx") == "*bc*");
    assert(wildcardMatchTemplate("hallo", "hallo") == "h*"); // prefix takes precedence

    // No valid pattern
    assert(wildcardMatchTemplate("ab", "ba") == "No");
    assert(wildcardMatchTemplate("abc", "def") == "No");
    assert(wildcardMatchTemplate("a", "b") == "No");

    // Edge case with length 1 but shared letter (covered by prefix)
    assert(wildcardMatchTemplate("a", "a") == "a*");

    // Edge case with length 2 and common inner substring it is also prefix/suffix
    assert(wildcardMatchTemplate("ab", "ab") == "a*"); // prefix priority
    assert(wildcardMatchTemplate("cb", "ab") == "*b"); // common last

    return 0;
}
