// Write a C++ function `std::string largestUniqueSubstring(const std::string& str)` that takes a non-empty string (which may contain any printable ASCII characters, including spaces, digits, and punctuation) and returns the longest substring that contains no repeating characters. If multiple substrings have the same maximum length, return the one that appears earliest in the original string. The function should work correctly for strings of length 1 up to very large inputs, handle all characters consistently (case-sensitive), and must not modify the input string. The returned substring must preserve the original characters and order. For example, for input `"abcabcbb"`, the longest unique substrings are `"abc"` and `"bca"` (both length 3), and the function should return `"abc"` because it starts at index 0, earlier than `"bca"` which starts at index 1.
The solution uses a sliding window technique with a hash map (or array) storing the most recent index of each character encountered. Maintain two pointers: `start` (left boundary of the current candidate window) and `end` (right boundary, iterating through the string). For each character at position `end`, check if it has appeared before at a position `prev` that is `>= start`. If so, the current window is invalid because it contains a duplicate, so before moving `start` past the previous occurrence, record the length of the current valid window and update the best answer if it’s longer (or equal but earlier starting position). Then set `start = prev + 1` to start a new window that excludes the duplicate. Always update the character’s latest index to `end`. At the end, handle the last window after the loop. Edge cases: empty string (though task says non-empty, handle gracefully), single character, all characters unique, all characters same, duplicates with intervening characters. The algorithm runs in O(n) time (each character processed a constant number of times) and O(1) auxiliary space if using a fixed-size array for 256 ASCII characters, or O(k) for a hash map where k is the character set size. Since the task involves a C-style function converting a char array to a string, but the free function takes a `std::string`, it is clean and reusable.
#include <string>
#include <unordered_map>
#include <algorithm>

// Returns the longest substring with all unique characters.
// If there is a tie, returns the leftmost occurrence.
std::string largestUniqueSubstring(const std::string& str) {
    if (str.empty()) {
        return "";
    }

    std::unordered_map<char, int> lastIndex; // character -> its last seen index
    int start = 0;          // left boundary of current window
    int bestStart = 0;      // start index of best window
    int maxLength = 0;      // length of best window

    for (int end = 0; end < static_cast<int>(str.size()); ++end) {
        char c = str[end];
        // If character seen before and within current window, move window start
        if (lastIndex.find(c) != lastIndex.end() && lastIndex[c] >= start) {
            // Current window is [start, end-1] and is valid
            int currentLength = end - start;
            if (currentLength > maxLength) {
                maxLength = currentLength;
                bestStart = start;
            }
            // Skip past previous occurrence to avoid duplicate
            start = lastIndex[c] + 1;
        }
        // Update latest occurrence
        lastIndex[c] = end;
    }

    // Check the final window after the loop
    int finalLength = static_cast<int>(str.size()) - start;
    if (finalLength > maxLength) {
        maxLength = finalLength;
        bestStart = start;
    }

    return str.substr(bestStart, maxLength);
}
#include <cassert>
#include <string>

// Function under test is declared in the solution, but we redeclare for test.
std::string largestUniqueSubstring(const std::string& str);

int main() {
    // Basic unique substring
    assert(largestUniqueSubstring("abcabcbb") == "abc");
    assert(largestUniqueSubstring("bbbbb") == "b");
    assert(largestUniqueSubstring("pwwkew") == "wke"); // "wke" appears before "kew"
    // Single character
    assert(largestUniqueSubstring("a") == "a");
    // All unique
    assert(largestUniqueSubstring("abcdef") == "abcdef");
    // Repeating pattern with longer unique after
    assert(largestUniqueSubstring("abcaabcd") == "abcd");
    // Spaces and punctuation, case-sensitive
    assert(largestUniqueSubstring("ab AB") == "ab A"); // Note: space is unique, lower and upper are distinct
    // Large string with repeated characters at ends
    assert(largestUniqueSubstring("abccdefgh") == "cdefgh");
    // Empty string (though spec says non-empty, handle gracefully)
    assert(largestUniqueSubstring("") == "");
    // Tie-breaking leftmost
    assert(largestUniqueSubstring("abcabc") == "abc"); // first occurrence at index 0
    // Digits and symbols
    assert(largestUniqueSubstring("12!345!6789") == "!345!6789" ? false : true); // Actually check correct
    // Correct for "12!345!6789": longest unique is "!345!6789"? Let's compute: substrings without repeating '!' start at index 0: "12!345" length 6 (includes ! once, then next ! at index 6? Actually chars: 1,2,!,3,4,5,!,6,7,8,9 -> first segment until second ! is "12!345" length 6, then second segment "!6789" length 5? Wait "!6789" length 5. So "12!345" length 6 is longest. So assert should be "12!345"
    assert(largestUniqueSubstring("12!345!6789") == "12!345");

    return 0;
}
