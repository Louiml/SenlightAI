/*
Write a C++ function `longestUniqueSubstring` that takes a non-empty string `s` (containing only printable ASCII characters, but the algorithm should conceptually support any extended ASCII character up to value 255) and returns the length of the longest contiguous substring that contains all unique characters. For example, for `"abcabcbb"` the answer is 3 (`"abc"`), for `"bbbbb"` it is 1, and for `"pwwkew"` it is 3 (`"wke"`). The function should handle empty strings gracefully by returning 0.
*/

#include <string>
#include <cstring>

// Returns the length of the longest substring without repeating characters.
// For an empty string, returns 0. Handles extended ASCII characters (0-255).
int longestUniqueSubstring(const std::string& s) {
    const int MAX_CHAR = 256;
    int lastIndex[MAX_CHAR];
    std::memset(lastIndex, -1, sizeof(lastIndex));

    int maxLength = 0;
    int windowStart = -1;  // Points to the last repeated character's index (exclusive boundary)

    for (int i = 0; i < static_cast<int>(s.size()); ++i) {
        unsigned char ch = static_cast<unsigned char>(s[i]);

        // If this character appeared before and its previous occurrence is inside the current window
        if (lastIndex[ch] > windowStart) {
            windowStart = lastIndex[ch];
        }

        int currentLength = i - windowStart;
        if (currentLength > maxLength) {
            maxLength = currentLength;
        }

        lastIndex[ch] = i;
    }

    return maxLength;
}

#include <cassert>

int main() {
    // Basic cases
    assert(longestUniqueSubstring("abcabcbb") == 3); // "abc"
    assert(longestUniqueSubstring("bbbbb") == 1);    // "b"
    assert(longestUniqueSubstring("pwwkew") == 3);   // "wke"
    // Empty and single character
    assert(longestUniqueSubstring("") == 0);
    assert(longestUniqueSubstring("a") == 1);
    // All unique characters
    assert(longestUniqueSubstring("abcdef") == 6);
    // Repeating character at edges
    assert(longestUniqueSubstring("aab") == 2);      // "ab"
    assert(longestUniqueSubstring("abba") == 2);     // "ab" or "ba"
    // Mixed with special characters and spaces
    assert(longestUniqueSubstring("ab cd ab") == 5); // "ab cd" (note space counts)
    // Extended ASCII character
    assert(longestUniqueSubstring("a\x80\x80b") == 3); // "a\x80b"
    return 0;
}

// The solution uses a sliding window technique with a hash table (implemented as a fixed-size array of 256 integers since the maximum character value is 255) that stores the most recent index of each seen character. We maintain two pointers: the current character index `i` and a variable `last_repeat_index` that marks the start boundary of the current window minus one. For each character at index `i`, if that character has been seen before AND its previous occurrence index is greater than the current `last_repeat_index`, we move the window’s left boundary to that previous occurrence, meaning we must reset `last_repeat_index` to that index so the window excludes the duplicated character. The length of the current valid substring is `i - last_repeat_index`, and we update the maximum found so far. Finally, we update the stored index for the current character to `i`. Edge cases: empty string → return 0; string with all identical characters → each step resets the window, but length remains 1; string with all unique characters → `last_repeat_index` stays -1 and the maximum becomes the string length. Time complexity is O(n) because each character is processed once and array lookups are O(1). Space complexity is O(1) because the array size is constant (256).
