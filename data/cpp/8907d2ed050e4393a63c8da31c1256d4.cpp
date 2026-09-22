Write a C++ function `bool isOneEditDistance(const std::string& s, const std::string& t)` that returns `true` if the two given strings differ by exactly one edit operation, where an edit is defined as inserting exactly one character, deleting exactly one character, or replacing exactly one character, and `false` otherwise. Empty strings are allowed (e.g., `""` and `"a"` differ by one insertion). The function must be case-sensitive and not modify the input strings. You may not use any external libraries beyond `<string>` and `<cstddef>`. The solution should be efficient and handle all edge cases, including identical strings (which should return `false`) and strings whose length difference is greater than one (which should return `false` immediately).

The core idea is to compare the two strings from left to right, exploiting the fact that if their lengths differ by more than 1, they cannot be one edit away. Also, if lengths are equal, the only possible edit is a single replacement, so we count mismatches and return true only if exactly one mismatch exists. If lengths differ by exactly 1, the shorter string must be insertable into the longer one with exactly one insertion (equivalently, the longer string must be deleteable to the shorter with exactly one deletion).  

For the equal-length case: iterate through both strings simultaneously. Count the number of positions where characters differ. If the count is exactly 1, return true; otherwise false. Important edge case: if strings are identical, count is 0 → false.  

For the length-differ-by-1 case (assume `s` is shorter, swap if not): use two indices `i` for `s` and `j` for `t`. While `i < s.size()` and `j < t.size()`, if characters match, advance both; if they differ, we simulate one insertion into `s` (or deletion from `t`). Since only one insertion is allowed, once we find a mismatch, we must skip exactly one character in `t` (i.e., `j++`) and then require that all remaining characters match exactly (by advancing `j` and `i` together without allowing further mismatches). If we never found a mismatch, then the only possible case is that `t` has exactly one extra character at the end, which is valid. The algorithm runs in O(n) time where n is the length of the longer string, and uses O(1) extra space.

Edge cases:  
- Identical strings → false.  
- Empty string vs. one-character string → true.  
- Length difference > 1 → false.  
- Longer string has an extra character in the middle → handled by the mismatch-skip logic.

#include <string>
#include <cstddef>

// Returns true if s and t differ by exactly one insertion, deletion, or replacement.
bool isOneEditDistance(const std::string& s, const std::string& t) {
    // Ensure s is the shorter (or equal-length) string for uniform processing.
    if (s.size() > t.size()) {
        return isOneEditDistance(t, s);
    }

    const std::size_t len_s = s.size();
    const std::size_t len_t = t.size();

    // Length difference cannot exceed 1.
    if (len_t - len_s > 1) {
        return false;
    }

    if (len_s == len_t) {
        // Equal length: exactly one replacement needed.
        std::size_t mismatches = 0;
        for (std::size_t i = 0; i < len_s; ++i) {
            if (s[i] != t[i]) {
                ++mismatches;
                if (mismatches > 1) {
                    return false;
                }
            }
        }
        return mismatches == 1;
    }

    // len_t == len_s + 1: exactly one insertion (or deletion) needed.
    std::size_t i = 0; // index for s (shorter)
    std::size_t j = 0; // index for t (longer)
    bool inserted = false;

    while (i < len_s && j < len_t) {
        if (s[i] == t[j]) {
            ++i;
            ++j;
        } else {
            if (inserted) {
                // More than one insertion needed.
                return false;
            }
            inserted = true;
            ++j; // skip one character in t (simulate insertion)
        }
    }

    // If we finished s, then the only way to be valid is that j == len_t
    // or we have one unused character left in t (the extra one).
    return true;
}

#include <cassert>

int main() {
    // Basic replacement
    assert(isOneEditDistance("abc", "abc") == false);
    assert(isOneEditDistance("abc", "abd") == true);
    assert(isOneEditDistance("abc", "ab") == true); // deletion from longer
    assert(isOneEditDistance("ab", "abc") == true); // insertion into shorter

    // Edge cases with empty strings
    assert(isOneEditDistance("", "") == false);
    assert(isOneEditDistance("", "a") == true);
    assert(isOneEditDistance("a", "") == true);

    // Length difference > 1
    assert(isOneEditDistance("abc", "abde") == false);
    assert(isOneEditDistance("a", "bcd") == false);

    // Insertion in middle
    assert(isOneEditDistance("abc", "aXbc") == true);
    assert(isOneEditDistance("aXbc", "abc") == true);

    // More than one edit
    assert(isOneEditDistance("abc", "axc") == false); // two mismatches in equal length
    assert(isOneEditDistance("abc", "aXbYc") == false); // two insertions

    // Identical strings of various lengths
    assert(isOneEditDistance("x", "x") == false);
    assert(isOneEditDistance("hello", "hello") == false);

    // Single character differences
    assert(isOneEditDistance("a", "b") == true);
    assert(isOneEditDistance("a", "ab") == true);
}
