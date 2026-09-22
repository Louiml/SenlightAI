Write a C++ function `int findAssociationSubstring(const std::string& str1, const std::string& str2)` that determines whether any permutation of `str1` appears as a contiguous substring of `str2`. If such a permutation exists, return the starting index (0-based) of the first occurrence in `str2`; otherwise return -1. Assume both strings contain only lowercase or uppercase English letters (ASCII characters 65–122) and that `str1` is non-empty. If `str1` is longer than `str2`, the function should return -1 immediately.

// The problem is a classic sliding-window substring-anagram check. We maintain a frequency count of characters in `str1` and use a window of length equal to `str1` that slides over `str2`. While sliding, we update the count for the character leaving the window (increment) and the character entering (decrement). A variable `remaining` tracks how many characters in the current window are still unmatched (i.e., how many characters of `str1` have not yet been satisfied). Initially, we set `remaining` to the length of `str1` and process the first window. For each window, when we add a character, if its frequency in the count array is still positive (meaning we need more of that character), we decrement `remaining`. When we remove a character, if its count in the array is non-negative (meaning we had an excess before removal), we increment `remaining`. Whenever `remaining` becomes 0, the window is a valid permutation, and we return the starting index. Edge cases: if `str1` is longer than `str2`, return -1 immediately; if the first window matches, return 0; if no window matches, return -1. Time complexity is O(n) where n is length of `str2`, and space complexity is O(1) because the count array has fixed size (128).

#include <string>
#include <vector>
#include <cstdint>

// Returns the starting index of the first window in str2 that is a permutation of str1,
// or -1 if no such window exists.
int findAssociationSubstring(const std::string& str1, const std::string& str2) {
    const std::size_t n1 = str1.size();
    const std::size_t n2 = str2.size();
    if (n1 == 0 || n1 > n2) {
        return -1;
    }

    // Frequency count for ASCII characters (0-127)
    std::vector<int> count(128, 0);
    for (char c : str1) {
        ++count[static_cast<unsigned char>(c)];
    }

    int remaining = static_cast<int>(n1);

    // Initialize the first window
    for (std::size_t i = 0; i < n1; ++i) {
        char add = str2[i];
        if (count[static_cast<unsigned char>(add)]-- > 0) {
            --remaining;
        }
    }

    if (remaining == 0) {
        return 0;
    }

    // Slide the window
    for (std::size_t i = n1; i < n2; ++i) {
        char removeChar = str2[i - n1];
        char addChar = str2[i];

        // Remove the leftmost character
        if (count[static_cast<unsigned char>(removeChar)]++ >= 0) {
            ++remaining;
        }

        // Add the new character
        if (count[static_cast<unsigned char>(addChar)]-- > 0) {
            --remaining;
        }

        if (remaining == 0) {
            return static_cast<int>(i - n1 + 1);
        }
    }

    return -1;
}

#include <cassert>
#include <string>

int findAssociationSubstring(const std::string& str1, const std::string& str2);

int main() {
    // Basic match at start
    assert(findAssociationSubstring("abc", "abcdef") == 0);
    // Match later
    assert(findAssociationSubstring("abc", "defabc") == 3);
    // Match in middle
    assert(findAssociationSubstring("ab", "xxba") == 2);
    // No match
    assert(findAssociationSubstring("abc", "defgh") == -1);
    // str1 longer than str2
    assert(findAssociationSubstring("hello", "hi") == -1);
    // Repeated characters in str1
    assert(findAssociationSubstring("aab", "baa") == 0);
    // Case sensitivity
    assert(findAssociationSubstring("abc", "ABC") == -1);
    // Large gap before match
    assert(findAssociationSubstring("zxy", "qwertyxyzz") == 6);
    // Single character
    assert(findAssociationSubstring("a", "banana") == 1);
    return 0;
}
