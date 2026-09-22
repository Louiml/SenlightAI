Write a C++ function `bool wildcardMatch(const std::string& s, const std::string& p)` that performs wildcard pattern matching with support for two special characters: `?` matches exactly one character (any character), and `*` matches any sequence of characters (including the empty sequence). The function should return `true` if the entire string `s` matches the pattern `p`, and `false` otherwise. The comparison is case-sensitive. The function must handle empty strings and patterns consisting only of `*` characters correctly. The algorithm should use a greedy backtracking approach with pointers to track the last seen `*` and the position in the string where that star last matched, achieving linear time complexity in the worst case. No recursion or dynamic programming tables are allowed; use only O(1) auxiliary space (excluding input strings). Provide the implementation as a free function with appropriate `const` qualifiers.
The solution uses a two-pointer greedy algorithm with backtracking. Maintain indices `i` (for string `s`) and `j` (for pattern `p`). Also track `star` as the index in the pattern of the most recent `*` encountered, and `match` as the position in `s` where that star last matched (initially -1 meaning none). Loop while `i` is less than `s.length()`. In each iteration: if the current pattern character is `?` or equals `s[i]`, advance both pointers. Else if the current pattern character is `*`, record its index in `star`, increment `j`, and set `match = i` (i.e., the star matches zero characters initially). Else if a star was previously found (i.e., `star != -1`), backtrack: set `j = star + 1`, increment `match` (so the star matches one more character), and set `i = match`. If none of these apply, return `false`. After the loop, skip any trailing `*` characters in the pattern; if all are skipped and `j` reaches the pattern length, return `true`, otherwise `false`. Edge cases: empty string with empty pattern returns `true` (loop doesn’t run, trailing star loop passes). Pattern containing only `*` matches any string (including empty). A `*` that appears in the middle is handled by greedy matching as much as possible, then backtracking if needed. Complexity: O(|s| + |p|) time in worst case because each pointer advances and backtracking increments `match` at most |s| times; O(1) auxiliary space.
#include <string>

// Returns true if the string `s` matches the pattern `p`.
// Supports '?' (any single character) and '*' (any sequence including empty).
bool wildcardMatch(const std::string& s, const std::string& p) {
    size_t i = 0;          // index in s
    size_t j = 0;          // index in p
    int star = -1;         // index of last '*' in p, -1 if none
    size_t match = 0;      // position in s where the last '*' matched

    while (i < s.size()) {
        if (j < p.size() && (p[j] == '?' || p[j] == s[i])) {
            ++i;
            ++j;
        } else if (j < p.size() && p[j] == '*') {
            star = static_cast<int>(j);
            ++j;
            match = i;
        } else if (star != -1) {
            // Backtrack: let the star match one more character
            j = static_cast<size_t>(star) + 1;
            ++match;
            i = match;
        } else {
            return false;
        }
    }

    // Skip trailing '*' characters
    while (j < p.size() && p[j] == '*') {
        ++j;
    }

    return j == p.size();
}
#include <cassert>

int main() {
    // Basic cases
    assert(wildcardMatch("aa", "a") == false);
    assert(wildcardMatch("aa", "aa") == true);
    assert(wildcardMatch("aa", "*") == true);
    assert(wildcardMatch("aa", "a*") == true);
    assert(wildcardMatch("ab", "?*") == true);
    assert(wildcardMatch("abc", "a?c") == true);
    assert(wildcardMatch("abc", "a?d") == false);
    
    // Empty string and pattern
    assert(wildcardMatch("", "") == true);
    assert(wildcardMatch("", "*") == true);
    assert(wildcardMatch("a", "") == false);
    
    // Multiple stars and backtracking
    assert(wildcardMatch("abbbbbcd", "ab*cd") == true);
    assert(wildcardMatch("mississippi", "m*iss*iss*pi") == true);
    assert(wildcardMatch("mississippi", "m*iss*ip*") == true);
    assert(wildcardMatch("baaabab", "ba*a?b") == true);
    assert(wildcardMatch("baaabab", "ba*a?") == true);
    
    // Edge cases with only stars
    assert(wildcardMatch("anything", "***") == true);
    assert(wildcardMatch("x", "?") == true);
    assert(wildcardMatch("xy", "?") == false);
    
    return 0;
}
