// Write a C++ function named `findFirstPatternMatch` that takes two constant string references: a text string and a pattern string. The function should perform a naive (brute-force) substring search to find the **first** occurrence of the pattern in the text. If the pattern is found, return the starting index (0-based) of that first occurrence. If the pattern is not found, return `-1`. Assume both strings are non-empty, and the pattern length is less than or equal to the text length. The function must handle edge cases such as the pattern being exactly the entire text, the pattern appearing at the very beginning or end, and overlapping pattern occurrences (e.g., text = "aaaa", pattern = "aa" should return 0). Do not use any standard library functions like `std::string::find`; implement the search explicitly using loops and character comparisons.

The solution uses the classic naive pattern matching algorithm. The idea is to slide the pattern over the text from left to right, and at each position `i` (from 0 to `text.size() - pattern.size()`), compare the pattern character-by-character with the corresponding text characters starting at `i`. If all characters match, we have found the first occurrence and return `i`. If a mismatch occurs, we break out of the inner comparison loop and advance `i` by one. This continues until either a match is found or we exhaust all possible starting positions. Important edge cases include: (1) pattern length equals text length, then only `i=0` is checked; (2) pattern appears at the end, so the loop bound `i <= text.size() - pattern.size()` must be used (or `i < text.size() - pattern.size() + 1`); (3) overlapping patterns are handled naturally because we always increment `i` by 1 after a mismatch, so we do not miss later overlapping matches. Time complexity is O(n*m) in the worst case (e.g., text = "aaaa...a", pattern = "aaa...ab"), where n is text length and m is pattern length. Space complexity is O(1) because we only use a few integer variables.

#include <string>

// Find the first occurrence of pattern in text using naive search.
// Returns the starting index (0-based) of the first match, or -1 if not found.
int findFirstPatternMatch(const std::string& text, const std::string& pattern) {
    const int textLen = static_cast<int>(text.size());
    const int patLen = static_cast<int>(pattern.size());

    // If pattern is longer than text, it cannot be found.
    if (patLen > textLen) {
        return -1;
    }

    // Slide the pattern over the text.
    for (int i = 0; i <= textLen - patLen; ++i) {
        int j = 0;
        // Compare pattern with text starting at i.
        while (j < patLen && text[i + j] == pattern[j]) {
            ++j;
        }
        // If all characters matched, return the starting index.
        if (j == patLen) {
            return i;
        }
    }

    // No match found.
    return -1;
}

#include <cassert>
#include <string>

// Declaration of the solution function (assume it is included above).
int findFirstPatternMatch(const std::string& text, const std::string& pattern);

int main() {
    // Basic case: pattern in the middle.
    assert(findFirstPatternMatch("THIS IS A TEST TEXT", "TEST") == 10);

    // Pattern at the beginning.
    assert(findFirstPatternMatch("hello world", "hello") == 0);

    // Pattern at the end.
    assert(findFirstPatternMatch("hello world", "world") == 6);

    // Pattern equals the entire text.
    assert(findFirstPatternMatch("abcde", "abcde") == 0);

    // Pattern not found.
    assert(findFirstPatternMatch("abcde", "xyz") == -1);

    // Overlapping pattern: first occurrence is at index 0.
    assert(findFirstPatternMatch("aaaa", "aa") == 0);

    // Pattern longer than text.
    assert(findFirstPatternMatch("abc", "abcd") == -1);

    // Single-character pattern and text.
    assert(findFirstPatternMatch("a", "a") == 0);

    // Pattern appears multiple times, first is correct.
    assert(findFirstPatternMatch("ababab", "aba") == 0);

    // Pattern appears only once at the end after mismatches.
    assert(findFirstPatternMatch("xxxyyyzzz", "zzz") == 6);

    return 0;
}
