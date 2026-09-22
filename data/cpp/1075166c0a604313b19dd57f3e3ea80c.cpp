Write a C++ function named `substringCount` that takes two strings, `text` and `pattern`, and returns the number of non-overlapping occurrences of `pattern` as a substring of `text`. The function should be case-sensitive and handle empty strings appropriately: if `pattern` is empty, return 0; if `text` is empty and `pattern` is non-empty, return 0. Non-overlapping means that once a match is found, the search for the next match must start after the end of the current match. For example, in `"aaaa"` with pattern `"aa"`, there should be 2 non-overlapping occurrences (positions 0-1 and 2-3), not 3 overlapping ones.
// The solution follows a straightforward greedy approach: we scan `text` from left to right, and whenever we find `pattern` starting at the current index, we increment the count and move the search position forward by the length of `pattern` to avoid overlapping matches. If no match is found at the current position, we move forward by one character. This is implemented using `std::string::find`, which returns the first occurrence starting at or after a given position. We use a loop with a `pos` variable that tracks where to start searching. After each match, we set `pos = matchPos + pattern.size()`. If `find` returns `std::string::npos`, we break. Edge cases: empty pattern → return 0 (since empty string matches everywhere, we define it as 0 to avoid infinite loops); empty text → return 0. Complexity: each `find` call takes O(n*m) in the worst case, and we may call it up to n/m times, leading to O(n^2) worst-case if pattern is long, but typically O(n*m) for standard library implementations (e.g., using KMP). For this problem, we assume standard library efficiency, so overall time is O(n*m) where n = text length, m = pattern length, and space is O(1) auxiliary.
#include <string>

// Count non-overlapping occurrences of pattern in text.
// Returns 0 if pattern is empty or text is empty.
int substringCount(const std::string& text, const std::string& pattern) {
    if (pattern.empty() || text.empty()) {
        return 0;
    }

    int count = 0;
    std::string::size_type pos = 0;

    while (true) {
        std::string::size_type found = text.find(pattern, pos);
        if (found == std::string::npos) {
            break;
        }
        ++count;
        pos = found + pattern.size();
    }

    return count;
}
#include <cassert>

int main() {
    // Basic case
    assert(substringCount("hello world", "l") == 3);
    // Overlapping pattern should be non-overlapping
    assert(substringCount("aaaa", "aa") == 2);
    // Pattern longer than text
    assert(substringCount("abc", "abcd") == 0);
    // Empty pattern
    assert(substringCount("anything", "") == 0);
    // Empty text
    assert(substringCount("", "xyz") == 0);
    // Pattern at the very end
    assert(substringCount("xyzabc", "abc") == 1);
    // Multiple non-overlapping occurrences with gaps
    assert(substringCount("ababab", "ab") == 3);
    // Pattern at the beginning
    assert(substringCount("testtest", "test") == 2);
    // Single character pattern repeated
    assert(substringCount("abcabc", "c") == 2);
    // No match
    assert(substringCount("abc", "z") == 0);
    // Case sensitivity
    assert(substringCount("AaAa", "A") == 2);
    return 0;
}
