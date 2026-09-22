// Write a C++ function named `longestRepeatingCharacterReplacement` that takes a string `s` consisting only of uppercase English letters and a non-negative integer `k`, and returns the length of the longest substring that can be made to contain only one distinct character by changing at most `k` characters anywhere in the substring. For example, given `s = "AABABBA"` and `k = 1`, the answer is `4` because we can change one `B` to `A` in the substring `"AABAB"` (or `"ABBA"` with one change) to get a run of four `A`'s or four `B`'s, but not more. The function must handle empty strings (return `0`), strings with all identical characters (the whole string length), cases where `k` is larger than the string length, and substrings where the most frequent character changes as the window slides. The algorithm must use a sliding window with a character frequency map, and it should not allocate more than O(1) extra space beyond the map.
// The optimal solution uses a sliding window with two pointers (`left` and `right`). We maintain a frequency map of characters in the current window and track `maxFreq`, the highest frequency of any single character in the window. For a valid window where at most `k` changes are allowed, the window is valid if `(windowLength - maxFreq) <= k`, because we can change all other characters to match the most frequent one. As we expand `right` to include a new character, we update its frequency and `maxFreq`. If the window becomes invalid (`windowLength - maxFreq > k`), we shrink from the left by decrementing the frequency of the left character (and removing it if it hits zero) and moving `left` forward. We then update the maximum valid window length. Note that `maxFreq` is not recalculated after shrinking—it remains the historical maximum—which is correct because decreasing the window size cannot increase the required changes, and the historical maximum is an upper bound that keeps the condition conservative but correct. Edge cases: empty string returns `0`; if `k >= n`, the answer is `n` since we can change all characters to a single one; if all characters are the same, `maxFreq` equals window length and the loop never shrinks. Time complexity is O(n) because each character is added once and removed at most once. Space complexity is O(1) in practice since the alphabet is limited to 26 letters, but using an unordered_map still gives O(1) on average; with a fixed alphabet it is strictly O(1).
#include <string>
#include <unordered_map>
#include <algorithm>

// Returns the length of the longest substring that can be transformed
// into a string of a single distinct character by changing at most k characters.
int longestRepeatingCharacterReplacement(const std::string& s, int k) {
    const int n = static_cast<int>(s.length());
    if (n == 0) {
        return 0;
    }

    int left = 0;
    int maxLength = 0;
    int maxFreq = 0;
    std::unordered_map<char, int> freq;

    for (int right = 0; right < n; ++right) {
        const char c = s[right];
        ++freq[c];
        maxFreq = std::max(maxFreq, freq[c]);

        // Current window is valid if we can change all other characters to
        // match the most frequent one within k changes.
        const int windowLength = right - left + 1;
        if (windowLength - maxFreq > k) {
            // Shrink window from left.
            --freq[s[left]];
            if (freq[s[left]] == 0) {
                freq.erase(s[left]);
            }
            ++left;
        }

        maxLength = std::max(maxLength, right - left + 1);
    }

    return maxLength;
}
#include <cassert>

int main() {
    // Basic cases from the problem statement style.
    assert(longestRepeatingCharacterReplacement("ABAB", 2) == 4);
    assert(longestRepeatingCharacterReplacement("AABABBA", 1) == 4);
    assert(longestRepeatingCharacterReplacement("AABABBA", 2) == 5);
    assert(longestRepeatingCharacterReplacement("AABA", 0) == 2);

    // Edge cases.
    assert(longestRepeatingCharacterReplacement("", 0) == 0);
    assert(longestRepeatingCharacterReplacement("", 5) == 0);
    assert(longestRepeatingCharacterReplacement("A", 0) == 1);
    assert(longestRepeatingCharacterReplacement("A", 10) == 1);
    assert(longestRepeatingCharacterReplacement("AAAA", 3) == 4);
    assert(longestRepeatingCharacterReplacement("BAAAB", 2) == 5);

    // All distinct characters with k large enough to make all same.
    assert(longestRepeatingCharacterReplacement("ABCDE", 4) == 5);
    // All distinct characters with k = 1.
    assert(longestRepeatingCharacterReplacement("ABCDE", 1) == 2);

    return 0;
}
