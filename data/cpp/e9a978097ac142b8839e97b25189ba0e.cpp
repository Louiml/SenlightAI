// Write a C++ function `std::string smallestWindowContainingAllChars(const std::string& text, const std::string& pattern)` that, given two non-empty strings `text` and `pattern`, returns the smallest contiguous substring of `text` that contains every character from `pattern` (including duplicates, i.e., each character in `pattern` must appear at least as many times as it does in `pattern`). If no such window exists, return the string `"-1"`. The function must be case‑sensitive, and if multiple windows of equal minimal length exist, return the one that starts earliest in `text`. The input strings may contain any printable ASCII characters, and the length of each string is at most 100,000. The function should be efficient, handling large inputs within linear time.

// The problem is a classic “minimum window substring” problem, solved using the sliding‑window / two‑pointer technique on an array of character frequencies (size 256 for ASCII). First, if `text` is shorter than `pattern`, return `"-1"` immediately. Build two frequency arrays: `patternFreq` (counts of each character in `pattern`) and `windowFreq` (counts within the current window). Maintain a counter `matched` that equals the number of characters in the current window that are “fully matched” relative to `pattern` (i.e., we increment `matched` when a character’s count in the window does not exceed its required count). Expand the right pointer `right` from 0 to `text.length()` – 1, adding one character at a time and updating counters. Once `matched` equals `pattern.length()`, the window contains all required characters, and we try to shrink the window from the left pointer `left`. While the window remains valid (i.e., the left character can be removed without losing a required character), we decrement its frequency and move `left` forward. After each shrink, compute the window length `right - left + 1` and update the minimum length and its start index if smaller (or if equal but earlier). Edge cases: when `pattern` is empty (though the problem says non‑empty, handle it gracefully by returning `""`); when characters not present in `pattern` appear in the window, they are always removable; when there are duplicates in `pattern`, the `matched` counter ensures we track exactly the needed counts. Time complexity is O(len(text) + len(pattern)) because each character is added and removed at most once; space complexity is O(1) since the frequency arrays have a fixed size of 256. The solution returns the substring using `substr` with the stored minimal start and length.

#include <string>
#include <vector>
#include <climits>

// Returns the smallest substring of text that contains all characters of pattern
// (including duplicates). If no such window exists, returns "-1".
std::string smallestWindowContainingAllChars(const std::string& text, const std::string& pattern) {
    const int NO_OF_CHARS = 256;
    int lenText = static_cast<int>(text.length());
    int lenPattern = static_cast<int>(pattern.length());

    if (lenText < lenPattern) {
        return "-1";
    }

    // Frequency counters for pattern and the current window
    std::vector<int> patternFreq(NO_OF_CHARS, 0);
    std::vector<int> windowFreq(NO_OF_CHARS, 0);
    for (char c : pattern) {
        patternFreq[static_cast<unsigned char>(c)]++;
    }

    int left = 0;
    int matched = 0;
    int minLen = INT_MAX;
    int startIndex = -1;

    for (int right = 0; right < lenText; ++right) {
        unsigned char currentChar = static_cast<unsigned char>(text[right]);
        windowFreq[currentChar]++;

        // Increase matched only if the need for this character hasn't been exceeded
        if (windowFreq[currentChar] <= patternFreq[currentChar]) {
            matched++;
        }

        // When all pattern characters are present in the window, try to shrink
        while (matched == lenPattern) {
            int windowLen = right - left + 1;
            if (windowLen < minLen) {
                minLen = windowLen;
                startIndex = left;
            }

            // Move left pointer: remove character at left
            unsigned char leftChar = static_cast<unsigned char>(text[left]);
            windowFreq[leftChar]--;
            // If removal breaks a necessary character, decrement matched
            if (windowFreq[leftChar] < patternFreq[leftChar]) {
                matched--;
            }
            left++;
        }
    }

    if (startIndex == -1) {
        return "-1";
    }
    return text.substr(startIndex, minLen);
}

#include <cassert>
#include <string>

// Assume the solution function is declared above

int main() {
    // Basic cases
    assert(smallestWindowContainingAllChars("ADOBECODEBANC", "ABC") == "BANC");
    assert(smallestWindowContainingAllChars("a", "a") == "a");
    assert(smallestWindowContainingAllChars("a", "aa") == "-1");
    assert(smallestWindowContainingAllChars("aa", "aa") == "aa");

    // Duplicate characters in pattern
    assert(smallestWindowContainingAllChars("abca", "aa") == "abca");
    assert(smallestWindowContainingAllChars("aab", "ab") == "ab");
    assert(smallestWindowContainingAllChars("bba", "ab") == "ba");

    // Characters not in pattern should be outside minimal window
    assert(smallestWindowContainingAllChars("xyzyx", "xyz") == "zyx");
    assert(smallestWindowContainingAllChars("aabb", "ab") == "ab");

    // Pattern longer than text
    assert(smallestWindowContainingAllChars("short", "longer") == "-1");

    // Large text, pattern at the end
    std::string big(10000, 'a');
    big += "z";
    std::string pat = "az";
    assert(smallestWindowContainingAllChars(big, pat) == "az");

    // All characters already in order
    assert(smallestWindowContainingAllChars("abc", "abc") == "abc");
    assert(smallestWindowContainingAllChars("abcabc", "abc") == "abc");

    // Single character pattern
    assert(smallestWindowContainingAllChars("hello", "l") == "l");

    return 0;
}
