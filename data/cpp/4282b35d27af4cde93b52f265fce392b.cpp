Write a C++ function that takes two strings, `needle` and `haystack`, both consisting only of lowercase English letters, and returns a boolean value indicating whether `needle` can be found as a permutation (anagram) of any contiguous substring of `haystack` of the same length as `needle`. For example, if `needle = "abc"` and `haystack = "cbabc"`, the function should return `true` because the substring `"bca"` (or `"cab"` or `"abc"`) appears as a contiguous block that contains exactly the same letters. The function must handle cases where `needle` is longer than `haystack` (return `false`), empty strings (assume empty needle returns `true` if haystack is also empty or contains an empty substring? Define: if `needle` is empty, return `true` only if `haystack` is also empty; otherwise return `false`), and duplicate characters (the permutation must match the multiset of characters exactly, not just set equality). The solution should be efficient for large inputs, avoiding generating all permutations.

The core problem is to check whether any contiguous substring of `haystack` of length `m = needle.size()` has the same character frequency counts as `needle`. A straightforward approach would generate all such substrings, sort them, and compare, but that is \(O(n \cdot m \log m)\) in the worst case. A more efficient method uses a sliding window with frequency arrays. First, compute the frequency counts of `needle` (size 26 for lowercase letters). Then slide a window of length `m` over `haystack`. For each window, maintain a running frequency array of the window's characters. At each step, update the window by removing the leftmost character and adding the new rightmost character. To avoid comparing all 26 counts every step, keep a counter `matches` that tracks how many letters have equal counts between the window and `needle`. Initialize `matches` by comparing the first window; for each shift, update only the two affected letters. If `matches` reaches 26, return `true`. Edge cases: if `m == 0`, return `true` only if `n == 0`. If `m > n`, return `false` immediately. The time complexity is \(O(n + m)\) because each character is added and removed once, and each update is constant time. Space complexity is \(O(1)\) since we use two fixed-size arrays of length 26.

#include <string>
#include <array>

// Returns true if 'needle' is a permutation of some contiguous substring of 'haystack'.
bool isAnagramSubstring(const std::string& needle, const std::string& haystack) {
    const int m = static_cast<int>(needle.size());
    const int n = static_cast<int>(haystack.size());

    // Edge case: empty needle is only a permutation of an empty substring.
    if (m == 0) {
        return n == 0;
    }
    // If needle is longer than haystack, no substring can match.
    if (m > n) {
        return false;
    }

    std::array<int, 26> needleCount{};
    std::array<int, 26> windowCount{};

    // Count characters in needle.
    for (char c : needle) {
        ++needleCount[c - 'a'];
    }

    // Initialize first window of length m.
    for (int i = 0; i < m; ++i) {
        ++windowCount[haystack[i] - 'a'];
    }

    // Count how many characters have matching counts initially.
    int matches = 0;
    for (int i = 0; i < 26; ++i) {
        if (needleCount[i] == windowCount[i]) {
            ++matches;
        }
    }

    // Initial window check.
    if (matches == 26) {
        return true;
    }

    // Slide the window.
    for (int i = m; i < n; ++i) {
        // Remove leftmost character.
        int leftIdx = haystack[i - m] - 'a';
        --windowCount[leftIdx];
        // Update matches for the left character.
        if (windowCount[leftIdx] == needleCount[leftIdx]) {
            ++matches;
        } else if (windowCount[leftIdx] + 1 == needleCount[leftIdx]) {
            // Before decrement, they were equal; now they differ.
            --matches;
        }

        // Add new rightmost character.
        int rightIdx = haystack[i] - 'a';
        ++windowCount[rightIdx];
        // Update matches for the right character.
        if (windowCount[rightIdx] == needleCount[rightIdx]) {
            ++matches;
        } else if (windowCount[rightIdx] - 1 == needleCount[rightIdx]) {
            // Before increment, they were equal; now they differ.
            --matches;
        }

        if (matches == 26) {
            return true;
        }
    }

    return false;
}

#include <cassert>

int main() {
    // Basic positive case.
    assert(isAnagramSubstring("abc", "cbabc") == true);
    // Negative case: permutation not contiguous.
    assert(isAnagramSubstring("abc", "acb") == true); // "acb" is a permutation of "abc".
    assert(isAnagramSubstring("abc", "acbx") == false); // "acbx" has extra letter.
    // Duplicate characters.
    assert(isAnagramSubstring("aab", "ababa") == true); // substring "aba" is a permutation.
    assert(isAnagramSubstring("aab", "ab") == false); // needle longer than haystack.
    // Empty needle: true only if haystack empty.
    assert(isAnagramSubstring("", "") == true);
    assert(isAnagramSubstring("", "a") == false);
    // Edge length equal.
    assert(isAnagramSubstring("xyz", "zyx") == true);
    assert(isAnagramSubstring("xyz", "xzy") == true);
    // Large distinct letters.
    assert(isAnagramSubstring("abcd", "dcba") == true);
    // No match when counts differ.
    assert(isAnagramSubstring("abc", "aab") == false);
    // Sliding window with multiple matches.
    assert(isAnagramSubstring("ab", "xabx") == true);
}
