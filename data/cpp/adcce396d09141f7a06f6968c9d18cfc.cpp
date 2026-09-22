// Given two strings `haystack` and `needle`, write a C++ function `findFirstOccurrence` that returns the index of the first occurrence of `needle` in `haystack`, or `-1` if `needle` is not part of `haystack`. If `needle` is an empty string, return `0` (consistent with common string-matching conventions). The function must handle cases where `needle` is longer than `haystack`, where both strings are equal, and where the substring appears multiple times (returning only the first index). The behavior should be exactly as shown in the classic `strStr` problem, but you may not use the standard library's `find` method or any other string search function—you must implement the search manually using character comparisons or `substr` (the latter is acceptable for this exercise). The function should be `const`-correct and take inputs by `const std::string&`.
The solution approach is straightforward: iterate through all possible starting positions `i` in `haystack` where `needle` could fit (i.e., from `0` to `haystack.size() - needle.size()` inclusive). At each position, check if the substring of `haystack` of length `needle.size()` starting at `i` equals `needle`. If a match is found, return `i`. If no match is found after the loop, return `-1`. Special cases to handle: if `needle` is empty, return `0` immediately; if `needle.size() > haystack.size()`, return `-1` because it cannot fit. Also, if both strings are equal, the loop will find a match at index `0` naturally, but an early return for equality is not necessary. Time complexity is O((n - m + 1) * m) in the worst case, where `n` is the length of `haystack` and `m` is the length of `needle`, because each substring comparison can take up to `m` character comparisons. Space complexity is O(1) auxiliary, excluding the temporary strings created by `substr` (which are O(m) each, but they are immediately discarded). More efficient algorithms like KMP exist, but for this task the brute-force approach is acceptable and matches the given code snippet's logic.
#include <string>

// Returns the index of the first occurrence of needle in haystack,
// or -1 if needle is not present. Empty needle returns 0.
int findFirstOccurrence(const std::string& haystack, const std::string& needle) {
    // Empty needle is always found at position 0
    if (needle.empty()) {
        return 0;
    }
    // If needle is longer than haystack, it cannot be found
    if (needle.size() > haystack.size()) {
        return -1;
    }
    // Iterate over all possible starting positions
    for (std::size_t i = 0; i <= haystack.size() - needle.size(); ++i) {
        // Check substring equality
        if (haystack.substr(i, needle.size()) == needle) {
            return static_cast<int>(i);
        }
    }
    // Not found
    return -1;
}
#include <cassert>
#include <string>

// The solution function is declared above (included here for completeness)
int findFirstOccurrence(const std::string& haystack, const std::string& needle);

int main() {
    // Basic finds
    assert(findFirstOccurrence("hello", "ll") == 2);
    assert(findFirstOccurrence("aaaaa", "bba") == -1);
    assert(findFirstOccurrence("sadbutsad", "sad") == 0);
    
    // Empty needle
    assert(findFirstOccurrence("anything", "") == 0);
    
    // Needle longer than haystack
    assert(findFirstOccurrence("short", "longer") == -1);
    
    // Equal strings
    assert(findFirstOccurrence("abc", "abc") == 0);
    
    // Duplicate matches, first index only
    assert(findFirstOccurrence("mississippi", "issi") == 1);
    
    // Single character needle
    assert(findFirstOccurrence("test", "t") == 0);
    
    // Single character haystack, match and no-match
    assert(findFirstOccurrence("x", "x") == 0);
    assert(findFirstOccurrence("x", "y") == -1);
    
    // Needle appears at the end
    assert(findFirstOccurrence("abcdef", "def") == 3);
}
