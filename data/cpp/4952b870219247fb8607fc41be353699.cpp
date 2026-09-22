/*
Write a C++ function named `longestUniqueSubstring` that takes a constant reference to a `std::string` and returns an `int` representing the length of the longest substring without repeating characters. The input may be empty or contain any printable ASCII characters (including spaces, digits, punctuation, and letters). The function must handle cases where all characters are unique, all characters are identical, and strings of length 1. For example, for `"abcabcbb"` the answer is `3` (substring `"abc"`), for `"bbbbb"` the answer is `1` (any single `'b'`), and for `"pwwkew"` the answer is `3` (substring `"wke"` or `"kew"`). The function should be efficient for inputs up to \(10^5\) characters long. Do not modify the input string; treat it as read-only.
*/

#include <string>
#include <unordered_map>
#include <algorithm>

// Returns the length of the longest substring without repeating characters.
int longestUniqueSubstring(const std::string& s) {
    int n = s.size();
    if (n == 0) return 0;

    int maxLength = 0;
    int start = 0;
    std::unordered_map<char, int> lastSeen; // maps character to its most recent index

    for (int end = 0; end < n; ++end) {
        char currentChar = s[end];
        auto it = lastSeen.find(currentChar);
        if (it != lastSeen.end()) {
            // If the character was seen at or after 'start', move 'start' past it.
            start = std::max(start, it->second + 1);
        }
        maxLength = std::max(maxLength, end - start + 1);
        lastSeen[currentChar] = end; // update the most recent index
    }
    return maxLength;
}

#include <cassert>

int main() {
    // Edge cases
    assert(longestUniqueSubstring("") == 0);
    assert(longestUniqueSubstring("a") == 1);
    assert(longestUniqueSubstring("aaaa") == 1);

    // Standard examples
    assert(longestUniqueSubstring("abcabcbb") == 3);
    assert(longestUniqueSubstring("bbbbb") == 1);
    assert(longestUniqueSubstring("pwwkew") == 3);

    // All unique characters
    assert(longestUniqueSubstring("abcdef") == 6);

    // With spaces and punctuation
    assert(longestUniqueSubstring("a b c a b") == 3); // substring "a b" or "b c"

    // Longer repetitive pattern
    assert(longestUniqueSubstring("dvdf") == 3); // "vdf"

    // Case where duplicate appears after a long gap
    assert(longestUniqueSubstring("tmmzuxt") == 5); // "mzuxt"

    return 0;
}

// The problem is solved using a sliding window with two indices (`start` and `end`) and a hash map (or array) that stores the most recent index each character was seen. Initially `start = 0` and the maximum length is `0`. As we iterate the string with `end` from 0 to \(n-1\), if the current character `s[end]` has been seen before at a position `last` that is at or after `start`, then we move `start` to `last + 1` (because the substring from the old `start` to `end` would contain a duplicate). Then we update the maximum length as `max(maxLength, end - start + 1)` and record the current position of `s[end]` in the map. This works because the sliding window always maintains a substring without repeating characters. Edge cases: empty string returns 0; single character returns 1; all identical characters produce maximum length 1; all unique characters produce length equal to the string size. Time complexity is \(O(n)\) since each character is processed once, and map operations are average \(O(1)\). Space complexity is \(O(\min(n, \text{alphabet size}))\) because the map stores at most the number of distinct characters in the window, which is bounded by the alphabet size (e.g., 128 for ASCII).
