Write a C++ function `int longestUniqueSubstringLength(const std::string& s)` that returns the length of the longest substring without repeating characters. The input string may be empty or contain any printable ASCII characters (including spaces, digits, punctuation). The function must correctly handle edge cases such as all identical characters, a single character, and substrings that end at the end of the string. Do not use any external libraries beyond the standard C++ library. The solution should avoid the use of `map` if a more efficient data structure can be used, and must not rely on modifying the input string.

The classic sliding window approach using two pointers is optimal. Maintain a left index `start` marking the beginning of the current window and a right index `i` that iterates through the string. Use an array (or `unordered_map`) to store the last seen position of each character (initialized to -1). For each character `s[i]`, if it was last seen at a position `lastPos` that is >= `start`, then the window contains a duplicate, so we move `start` to `lastPos + 1`. Update the last seen position of `s[i]` to `i`, and compute the current window length as `i - start + 1`, updating the global maximum. This works because any substring with all unique characters must have its left boundary move only forward when a duplicate is found. Edge cases: empty string returns 0; a string of length 1 returns 1; all identical characters like "aaaa" yields 1; a string like "abca" correctly yields 3. Time complexity is O(n) because each character is visited at most twice (once by `i`, once by moving `start`). Space complexity is O(1) for the fixed-size array (if using an array of 128 or 256 for ASCII), or O(k) where k is the character set size if using a hash map.

#include <string>
#include <algorithm>

// Returns the length of the longest substring without repeating characters.
int longestUniqueSubstringLength(const std::string& s) {
    // For ASCII characters, we can track last positions in a fixed array.
    // Initialize all to -1 (meaning "not seen yet").
    int lastPos[256];
    std::fill(lastPos, lastPos + 256, -1);

    int maxLen = 0;
    int start = 0;  // Left boundary of the current window.

    for (int i = 0; i < static_cast<int>(s.size()); ++i) {
        unsigned char c = static_cast<unsigned char>(s[i]);
        // If the character was seen at or after the current start,
        // we must move start past that occurrence.
        if (lastPos[c] >= start) {
            start = lastPos[c] + 1;
        }
        lastPos[c] = i;
        maxLen = std::max(maxLen, i - start + 1);
    }
    return maxLen;
}

#include <cassert>
#include <string>

// Function declaration from solution.
int longestUniqueSubstringLength(const std::string& s);

int main() {
    // Empty string
    assert(longestUniqueSubstringLength("") == 0);
    // Single character
    assert(longestUniqueSubstringLength("a") == 1);
    // All duplicates
    assert(longestUniqueSubstringLength("aaaa") == 1);
    // Simple case
    assert(longestUniqueSubstringLength("abcabcbb") == 3);
    // Repeating pattern with a unique tail
    assert(longestUniqueSubstringLength("pwwkew") == 3);
    // All unique
    assert(longestUniqueSubstringLength("abcdef") == 6);
    // Duplicate at the beginning
    assert(longestUniqueSubstringLength("abca") == 3);
    // Spaces and punctuation
    assert(longestUniqueSubstringLength("a b c") == 3);
    // Numeric characters
    assert(longestUniqueSubstringLength("12321") == 3);
    // Case sensitivity
    assert(longestUniqueSubstringLength("Aa") == 2);
    return 0;
}
