/*
Write a C++ function `int lengthOfLongestUniqueSubstring(const std::string& s)` that returns the length of the longest substring without repeating characters. A substring is a contiguous sequence of characters within the string. The input string may be empty, may contain only lowercase or uppercase letters, digits, spaces, or any ASCII characters, and may have length up to 10^5. The function must handle edge cases such as all identical characters, all unique characters, and an empty string (which should return 0).
*/

#include <string>
#include <algorithm>
#include <array>

// Returns the length of the longest substring without repeating characters.
int lengthOfLongestUniqueSubstring(const std::string& s) {
    // Use an array to store the last index of each ASCII character; -1 means not seen.
    std::array<int, 128> lastIndex;
    lastIndex.fill(-1);
    
    int maxLen = 0;
    int start = -1; // Left boundary of current window, exclusive.
    
    for (int i = 0; i < static_cast<int>(s.size()); ++i) {
        unsigned char c = static_cast<unsigned char>(s[i]);
        // If this character has appeared before, move start to max(current, last occurrence).
        if (lastIndex[c] > start) {
            start = lastIndex[c];
        }
        // Update the character's latest occurrence.
        lastIndex[c] = i;
        // Current window length is i - start.
        maxLen = std::max(maxLen, i - start);
    }
    return maxLen;
}

#include <cassert>
#include <string>

int main() {
    assert(lengthOfLongestUniqueSubstring("") == 0);
    assert(lengthOfLongestUniqueSubstring("a") == 1);
    assert(lengthOfLongestUniqueSubstring("aaaa") == 1);
    assert(lengthOfLongestUniqueSubstring("abcabcbb") == 3);
    assert(lengthOfLongestUniqueSubstring("pwwkew") == 3);
    assert(lengthOfLongestUniqueSubstring("dvdf") == 3);
    assert(lengthOfLongestUniqueSubstring("abba") == 2);
    assert(lengthOfLongestUniqueSubstring("tmmzuxt") == 5);
    assert(lengthOfLongestUniqueSubstring("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ") == 52);
    assert(lengthOfLongestUniqueSubstring("  ") == 1); // space character
    return 0;
}

// The solution uses a sliding window approach with a hash map (or array) to track the most recent index of each character seen so far. We maintain two pointers: `start` (the left boundary of the current window, initialized to -1) and `i` (the right boundary, iterating through the string). For each character at index `i`, we check if it has been seen before (exists in the map). If so, we move the `start` pointer to the maximum of its current value and the last occurrence index of that character, effectively skipping over the previous duplicate. Then we update the map with the current index and compute the current window length as `i - start`. We keep the maximum length seen. Key edge cases: empty string returns 0; string with all unique characters returns the full length; string with all same characters returns 1; characters appearing multiple times require `start` to be updated correctly (not just set to the latest duplicate index, but to the max of current start and that index, because an older duplicate may be outside the current window). Time complexity is O(n) where n is the length of the string (each character processed once). Space complexity is O(min(n, alphabet size)) for the map, but since we use a fixed-size array of 128 or 256 for ASCII, it's O(1) in practice.
