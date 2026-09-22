/*
Write a C++ function `longestWord` that takes a single `std::string` as input (which may contain multiple spaces, leading/trailing whitespace, and no punctuation) and returns the longest contiguous sequence of non-space characters. If there are multiple words with the same maximum length, the function should return the first such word encountered when scanning from left to right. If the input string is empty or contains only whitespace, the function should return an empty string. The input string must not be modified, and the function must work correctly for strings of any length, including very long ones without spaces.
*/
#include <string>

// Returns the first longest whitespace-separated word from the input string.
// Returns an empty string if the input is empty or contains only whitespace.
std::string longestWord(const std::string& s) {
    const std::size_t len = s.size();
    std::size_t maxStart = 0;
    std::size_t maxLen = 0;
    std::size_t start = 0;
    std::size_t j = 0;

    while (j <= len) {
        // If we are inside the string and not a space, advance j.
        if (j < len && s[j] != ' ') {
            ++j;
        } else {
            // End of a word (or empty segment due to consecutive spaces).
            const std::size_t currLen = j - start;
            if (currLen > maxLen) {
                maxLen = currLen;
                maxStart = start;
            }
            // Skip the space (or one past the end) and set new start.
            ++j;
            start = j;
        }
    }

    return s.substr(maxStart, maxLen);
}
#include <cassert>
#include <string>

// Declaration of the function under test.
std::string longestWord(const std::string& s);

int main() {
    // Basic cases
    assert(longestWord("the quick brown fox") == "quick");
    assert(longestWord("a") == "a");
    assert(longestWord("hello world") == "hello");

    // Ties: first longest word is returned
    assert(longestWord("a bb ccc dddd") == "dddd");
    assert(longestWord("aa bb cc") == "aa");

    // Multiple spaces and leading/trailing whitespace
    assert(longestWord("  one   two three  ") == "three");
    assert(longestWord("   x   ") == "x");

    // Empty or all-whitespace input
    assert(longestWord("") == "");
    assert(longestWord("   ") == "");

    // Long single word
    assert(longestWord("supercalifragilisticexpialidocious") == "supercalifragilisticexpialidocious");

    // Mixed spacing with no spaces (all one word)
    assert(longestWord("abcdef") == "abcdef");

    return 0;
}
// The solution scans the input string character by character, tracking the start index of the current word and the end of each word when a space is encountered or the end of the string is reached. Whenever a word is completed, compare its length (`j - i`) to the current maximum; if it is strictly greater, update the maximum length and the start position of the best word. After the loop, extract the substring from the best start position with the maximum length. Edge cases include: empty input (loop never triggers, returns empty substring), a string made entirely of spaces (no words completed, so `maxLen` stays 0, and `substr(0,0)` returns empty), multiple consecutive spaces (resets `i = j` after each space, skipping empty “words”), and ties (only update when strictly greater, preserving the first maximum). Time complexity is \(O(n)\) for one pass plus \(O(n)\) for the final substring extraction, and space complexity is \(O(1)\) auxiliary besides the returned substring.
