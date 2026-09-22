// Write a C++ function `long long countGoodSegments(const std::string& s, const std::vector<char>& allowedChars)` that, given a string `s` of lowercase English letters and a list of allowed characters, returns the total number of contiguous substrings where every character in the substring belongs to the allowed set. For each maximal contiguous block of allowed characters, if the block has length `L`, it contributes `L*(L+1)/2` substrings (all contiguous substrings within that block). The function should handle empty strings, empty allowed lists, and strings with no allowed characters at all. The input string may contain only lowercase letters `'a'`–`'z'`, and the allowed list may contain duplicates, which should be ignored.

The core idea is to scan the string once, segmenting it into maximal runs of characters that are present in the allowed set. For each such run of length `L`, the number of valid substrings entirely inside that run is exactly the sum of the first `L` positive integers: `L*(L+1)/2`. This is because a substring of length 1 has `L` choices, length 2 has `L-1` choices, ..., length `L` has 1 choice, summing to `L + (L-1) + ... + 1 = L*(L+1)/2`. We must also ensure that no substring crosses a disallowed character, which is automatically guaranteed by splitting at disallowed positions. Edge cases include: an empty string (returns 0), an empty allowed list (no characters are allowed, so 0 substrings), a string with only disallowed characters (runs never start, so 0), and a string where all characters are allowed (one run of length `n`, giving `n*(n+1)/2`). The time complexity is `O(n + k)` where `n` is the string length and `k` is the number of allowed characters, because we build a boolean lookup set (e.g., using a `bool[26]` array) in `O(k)` and then scan the string once in `O(n)`. Space complexity is `O(1)` auxiliary (a fixed-size bool array), plus `O(1)` for variables.

#include <string>
#include <vector>
#include <cstdint>

// Count the total number of contiguous substrings of s that consist entirely
// of characters present in allowedChars. Duplicates in allowedChars are ignored.
long long countGoodSegments(const std::string& s, const std::vector<char>& allowedChars) {
    // Build a lookup table for allowed characters (lowercase 'a'-'z' only).
    bool isAllowed[26] = {false};
    for (char c : allowedChars) {
        isAllowed[c - 'a'] = true;
    }

    long long total = 0;
    long long currentRunLength = 0;

    for (char c : s) {
        if (isAllowed[c - 'a']) {
            ++currentRunLength;
        } else {
            // End of a run: add all substrings within this run.
            total += currentRunLength * (currentRunLength + 1) / 2;
            currentRunLength = 0;
        }
    }

    // Handle the final run if the string ends with allowed characters.
    total += currentRunLength * (currentRunLength + 1) / 2;

    return total;
}

#include <cassert>
#include <string>
#include <vector>

// The solution function is declared/defined above (not repeated here for brevity).
int main() {
    // Empty string
    assert(countGoodSegments("", {'a'}) == 0);

    // Empty allowed list
    assert(countGoodSegments("abc", {}) == 0);

    // No allowed characters present
    assert(countGoodSegments("xyz", {'a', 'b'}) == 0);

    // Single run of length 3 → 3*4/2=6 substrings
    assert(countGoodSegments("aaa", {'a'}) == 6);

    // Two runs separated by a disallowed character
    // "aab" has run "aa" (length 2 → 3 substrings) and "b" (length 1 → 1 substring) → total 4
    assert(countGoodSegments("aab", {'a', 'b'}) == 4);

    // Duplicate allowed characters should not affect result
    assert(countGoodSegments("abab", {'a', 'a', 'b'}) == 10); // "abab" all allowed → 4*5/2=10

    // Mixed case: "a!b" but only 'a' and 'b' allowed (no '!'), so runs "a" (1) and "b" (1) → total 2
    assert(countGoodSegments("a!b", {'a', 'b'}) == 2);

    // Longer example: "abacaba" with allowed {'a','b'} — runs: "ab" (2→3), "a" (1→1), "aba" (3→6), total 10
    assert(countGoodSegments("abacaba", {'a', 'b'}) == 10);

    // Edge case: all characters disallowed → 0
    assert(countGoodSegments("zzz", {'a'}) == 0);

    // Large run length to verify long long handling
    std::string big(100000, 'a'); // 100000 'a's
    long long expected = 100000LL * (100000LL + 1) / 2;
    assert(countGoodSegments(big, {'a'}) == expected);
}
