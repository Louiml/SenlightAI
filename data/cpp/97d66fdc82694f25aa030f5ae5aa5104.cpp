// Write a standalone C++ function `int longestUniqueSubstringLength(const std::string& s)` that returns the length of the longest contiguous substring containing all distinct characters. The input string may contain any printable ASCII characters (letters, digits, symbols, spaces) and can be empty. The function must handle strings of length up to 50,000 efficiently. You are not allowed to use any external libraries beyond the standard C++ headers. The task is purely algorithmic; no file I/O, networking, or classes are required.

#include <cassert>
#include <string>

// Declaration of the function under test.
int longestUniqueSubstringLength(const std::string& s);

int main() {
    // Basic examples from the original problem.
    assert(longestUniqueSubstringLength("abcabcbb") == 3);
    assert(longestUniqueSubstringLength("bbbbb") == 1);
    assert(longestUniqueSubstringLength("pwwkew") == 3);

    // Edge cases.
    assert(longestUniqueSubstringLength("") == 0);
    assert(longestUniqueSubstringLength("a") == 1);
    assert(longestUniqueSubstringLength("abcd") == 4);          // all unique
    assert(longestUniqueSubstringLength("abca") == 3);          // duplicate at the end
    assert(longestUniqueSubstringLength("aab") == 2);           // duplicate at the beginning
    assert(longestUniqueSubstringLength("dvdf") == 3);          // overlapping duplicates
    assert(longestUniqueSubstringLength(" ") == 1);             // space is a valid character
    assert(longestUniqueSubstringLength("abccdefg") == 5);      // "cdefg" length 5

    // Stress test with repeated pattern.
    std::string stress(10000, 'a');
    assert(longestUniqueSubstringLength(stress) == 1);

    // Mixed characters.
    assert(longestUniqueSubstringLength("a1! a1!") == 4);       // "a1! " length 4

    // Long random-like string with all unique characters.
    std::string uniqueChars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    assert(longestUniqueSubstringLength(uniqueChars) == 52);

    return 0;
}

#include <string>
#include <algorithm>

// Returns the length of the longest substring without repeating characters.
int longestUniqueSubstringLength(const std::string& s) {
    // Using an array to store last occurrence index (default -1).
    // Covers all standard ASCII characters (0-127) but we use 256 for safety.
    int lastIndex[256];
    std::fill(std::begin(lastIndex), std::end(lastIndex), -1);

    int left = 0;          // start of current window
    int maxLength = 0;

    for (int right = 0; right < static_cast<int>(s.size()); ++right) {
        unsigned char c = static_cast<unsigned char>(s[right]);

        // If this character was seen and its last occurrence is within the current window,
        // shift the left boundary to just after that occurrence.
        if (lastIndex[c] >= left) {
            left = lastIndex[c] + 1;
        }

        // Update the last occurrence of this character.
        lastIndex[c] = right;

        // Update maxLength with current window size.
        maxLength = std::max(maxLength, right - left + 1);
    }

    return maxLength;
}

// The problem is a classic sliding-window/ two-pointer technique. We maintain a window defined by two indices: a `left` boundary (start of the current valid substring) and a `right` index that expands as we iterate. We also keep a hash map (or fixed-size array for ASCII) storing the **most recent index** of each character seen. As we move `right` through the string, if the current character has appeared before at a position `lastPos` that is **at or after** `left`, then we have a duplicate inside our window. To maintain uniqueness, we must move `left` to `lastPos + 1` (or simply update `left` to the next index after the previous occurrence). The length of the current valid window is `right - left + 1`, and we track the maximum over all windows.  
//
// **Edge cases**:  
// - Empty string → return 0.  
// - Single character → return 1.  
// - All identical characters (e.g., "aaaa") → the longest substring is just one character, so return 1.  
// - Characters that are spaces, symbols, or non-letters are handled the same way; we treat them as normal characters.  
// - When a duplicate is found, we must ensure `left` moves **forward** even if the previous occurrence is before `left` (then no update is needed).  
//
// **Complexity**: O(n) time, since each character is visited at most twice (once by `right`, once by potential `left` movement). O(1) auxiliary space if we use a fixed-size array of size 128 (for standard ASCII) or O(256) to be safe. Alternatively, using a `std::unordered_map` gives O(1) average but with more overhead; the array approach is deterministic and faster.
