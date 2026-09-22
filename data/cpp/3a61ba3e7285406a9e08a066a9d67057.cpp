Write a C++ function `bool isSubsequence(const std::string& s, const std::string& t)` that determines whether string `s` is a subsequence of string `t`. A subsequence is a sequence that appears in the same relative order but not necessarily contiguously. The function must return `true` if every character of `s` appears in `t` in the same order, and `false` otherwise. The function should handle empty strings (an empty `s` is always a subsequence), and must work with any ASCII characters, including digits and punctuation. The solution should avoid using `std::map` for character counting, as that approach is insufficient — it only checks character presence, not order. Instead, use a single pass over `t` with a pointer into `s`. Edge cases include `s` longer than `t` (return `false` immediately) and duplicate characters in `s`.

#include <cassert>
#include <string>

// Forward declaration of the solution function.
bool isSubsequence(const std::string& s, const std::string& t);

int main() {
    // Basic cases from the original snippet.
    assert(isSubsequence("abc", "ahbgdc") == true);
    assert(isSubsequence("axc", "ahbgdc") == false);
    
    // Empty and single-character cases.
    assert(isSubsequence("", "anything") == true);
    assert(isSubsequence("", "") == true);
    assert(isSubsequence("a", "a") == true);
    assert(isSubsequence("a", "b") == false);
    
    // Duplicate characters and order sensitivity.
    assert(isSubsequence("aaa", "aa") == false);
    assert(isSubsequence("aaa", "aaaa") == true);
    assert(isSubsequence("ab", "ba") == false);
    
    // Non-alpha characters and longer strings.
    assert(isSubsequence("123", "1a2b3c") == true);
    assert(isSubsequence("!? ", "a!b?c d") == true);
    assert(isSubsequence("abc", "ab") == false);
    assert(isSubsequence("a", "zzzazzz") == true);
    
    // Case sensitivity.
    assert(isSubsequence("AbC", "aAbBcC") == true);
    assert(isSubsequence("abc", "aAbBcC") == true);
    assert(isSubsequence("ABC", "abc") == false);
}

#include <string>

// Returns true if s is a subsequence of t (characters appear in same order).
bool isSubsequence(const std::string& s, const std::string& t) {
    size_t sIndex = 0;               // Current position in s
    const size_t sLength = s.size();
    
    // If s is longer than t, it's impossible to be a subsequence.
    if (sLength > t.size()) {
        return false;
    }
    
    // Scan through t once, advancing sIndex when a match is found.
    for (const char c : t) {
        if (sIndex < sLength && c == s[sIndex]) {
            ++sIndex;
        }
    }
    
    // A complete match means all of s has been consumed.
    return sIndex == sLength;
}

// The correct approach is a greedy two-pointer scan. Maintain an index `i = 0` into string `s`. Iterate through each character `c` in `t`. If `c` equals `s[i]`, increment `i`. After processing all characters of `t`, if `i == s.length()`, all characters of `s` have been matched in order, so return `true`; otherwise return `false`. This works because scanning `t` left-to-right and matching whenever possible yields the earliest possible positions for each character of `s`, which is always optimal for subsequence checking. Edge cases: (1) if `s` is empty, the loop never runs and `i` stays 0, which equals `s.length()` (0), so return `true`. (2) If `s.length() > t.length()`, we can early-return `false` because it's impossible to fit more characters than available. (3) If `s` has characters not in `t`, the pointer will never reach the end. Time complexity is O(n) where n = `t.length()`, since we scan `t` exactly once; even if we early-check lengths, it's still O(n + m) for reading both strings, but effectively O(n). Auxiliary space is O(1) as we only use a couple of integers.
