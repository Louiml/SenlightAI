Write a standalone C++ function that, given two strings `text` and `pattern` (both non-empty, containing only printable ASCII characters), returns the zero-based index of the first occurrence of `pattern` as a contiguous substring within `text`. If `pattern` does not occur in `text`, return `-1`. The function must perform a direct character-by-character comparison without using any standard string search functions (e.g., `find`, `strstr`, regex). The solution should handle cases where `pattern` is longer than `text`, where they are identical, and where there are overlapping or multiple occurrences (always return the earliest index).

The problem is a classic naive substring search. The main algorithm iterates over each possible starting position `i` in `text` from 0 to `text.length() - pattern.length()`. For each starting position, it checks whether every character of `pattern` matches the corresponding character in `text` at `i + offset`. If the entire pattern matches, return `i` immediately. If no starting position yields a match, return `-1`. Important edge cases: (1) If `pattern` is longer than `text`, then no starting position can possibly work, so we can return `-1` immediately. (2) If `pattern` is empty (though the spec says non-empty, we could defensively treat it as matching at index 0), but we follow the given spec and assume non-empty. (3) Matching is case-sensitive; "A" and "a" are different characters. (4) Overlapping occurrences, e.g., text "aaa" pattern "aa", should return index 0, not 1. Time complexity is O(n*m) in the worst case (where n = text length, m = pattern length) because for each of the O(n) positions we may compare up to O(m) characters; average case is often better. Space complexity is O(1) auxiliary, since we only use integer indices and no extra data structures.

#include <string>

// Returns the index of the first occurrence of 'pattern' in 'text',
// or -1 if 'pattern' is not a substring of 'text'.
// Performs a naive character-by-character search.
int firstSubstringIndex(const std::string& text, const std::string& pattern) {
    const int textLength = static_cast<int>(text.length());
    const int patternLength = static_cast<int>(pattern.length());

    // If pattern is longer than text, it cannot be a substring.
    if (patternLength > textLength) {
        return -1;
    }

    // Try every possible starting position in text.
    for (int start = 0; start <= textLength - patternLength; ++start) {
        bool match = true;
        for (int offset = 0; offset < patternLength; ++offset) {
            if (text[start + offset] != pattern[offset]) {
                match = false;
                break;
            }
        }
        if (match) {
            return start;
        }
    }

    return -1;
}

#include <cassert>
#include <string>

// The solution function is declared above; here we test it.
int main() {
    // Basic match at the beginning
    assert(firstSubstringIndex("hello world", "hello") == 0);
    // Match in the middle
    assert(firstSubstringIndex("hello world", "lo wo") == 3);
    // Match at the end
    assert(firstSubstringIndex("hello world", "world") == 6);
    // Pattern longer than text
    assert(firstSubstringIndex("abc", "abcd") == -1);
    // No match exists
    assert(firstSubstringIndex("abc", "xyz") == -1);
    // Identical strings
    assert(firstSubstringIndex("same", "same") == 0);
    // Overlapping pattern; earliest index should be returned
    assert(firstSubstringIndex("aaaa", "aa") == 0);
    // Single character pattern
    assert(firstSubstringIndex("programming", "g") == 3);
    // Pattern appears multiple times; returns first
    assert(firstSubstringIndex("mississippi", "iss") == 1);
    // Case sensitivity
    assert(firstSubstringIndex("Case", "case") == -1);
    // Special characters
    assert(firstSubstringIndex("abc!@#def", "!@#") == 3);
    return 0;
}
