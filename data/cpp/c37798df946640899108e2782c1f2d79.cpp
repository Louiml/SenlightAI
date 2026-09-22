Write a C++ function that, given two non-empty strings `s` and `p` consisting of lowercase English letters, returns a vector of starting indices (0-based) of all anagrams of `p` in `s`. An anagram is a permutation of the same characters with the same frequencies. For example, if `s = "cbaebabacd"` and `p = "abc"`, the output should be `[0, 6]` because substrings `s[0..2] = "cba"` and `s[6..8] = "bac"` are anagrams. If no anagram exists or if `p.size() > s.size()`, return an empty vector. Assume both strings contain only lowercase English letters.
The solution uses a sliding window of fixed length equal to `p.size()`. Maintain two integer arrays of size 26 (`c1` for `p` and `c2` for the current window in `s`). First, count frequencies of characters in `p` and the first window of `s` (of length `p.size()`). Compare the two arrays; if equal, push index 0. Then slide the window one character at a time: decrement the count of the character leaving the window (at index `j`) and increment the count of the new character entering (at index `i`). After each slide, compare `c1` and `c2`; if equal, the window starting at `j+1` is an anagram, so push that index. Continue until the window reaches the end of `s`. Important edge cases: if `s.size() < p.size()`, return an empty vector immediately; handle duplicate characters naturally by frequency counting; ensure the comparison loop breaks early when a mismatch is found, and only push the index when all 26 counts match. Time complexity is O(n * 26) = O(n) where n is the length of `s`, because each character is added/removed once and each window comparison checks at most 26 entries. Space complexity is O(1) (two fixed-size arrays of 26 integers, plus the output vector which is O(m) where m is the number of anagrams found).
#include <vector>
#include <string>
#include <cstddef>

// Return starting indices of all anagrams of pattern p in text s.
std::vector<int> findAllAnagrams(const std::string& s, const std::string& p) {
    std::vector<int> result;
    if (s.size() < p.size()) {
        return result;
    }

    int patternCount[26] = {0};
    int windowCount[26] = {0};

    // Initialize counts for pattern and first window.
    const size_t patternSize = p.size();
    for (size_t i = 0; i < patternSize; ++i) {
        ++patternCount[p[i] - 'a'];
        ++windowCount[s[i] - 'a'];
    }

    // Helper lambda to compare the two frequency arrays.
    auto countsEqual = [&]() {
        for (int i = 0; i < 26; ++i) {
            if (patternCount[i] != windowCount[i]) {
                return false;
            }
        }
        return true;
    };

    if (countsEqual()) {
        result.push_back(0);
    }

    // Slide the window from left to right.
    size_t left = 0;
    for (size_t right = patternSize; right < s.size(); ++right) {
        --windowCount[s[left] - 'a'];
        ++windowCount[s[right] - 'a'];
        ++left;
        if (countsEqual()) {
            result.push_back(static_cast<int>(left));
        }
    }

    return result;
}
#include <cassert>
#include <vector>
#include <string>

int main() {
    // Basic example from description.
    assert(findAllAnagrams("cbaebabacd", "abc") == std::vector<int>({0, 6}));
    
    // No anagrams (pattern longer than text).
    assert(findAllAnagrams("ab", "abc") == std::vector<int>());
    
    // Exact match of whole string.
    assert(findAllAnagrams("abc", "abc") == std::vector<int>({0}));
    
    // Repeated pattern multiple times.
    assert(findAllAnagrams("abab", "ab") == std::vector<int>({0, 1, 2}));
    
    // Single character pattern.
    assert(findAllAnagrams("aaa", "a") == std::vector<int>({0, 1, 2}));
    
    // No anagrams (different characters).
    assert(findAllAnagrams("xyz", "abc") == std::vector<int>());
    
    // Pattern with duplicates.
    assert(findAllAnagrams("abacbabc", "abc") == std::vector<int>({1, 4, 5}));
    
    // Empty result when pattern size equals 1 but no match.
    assert(findAllAnagrams("bbb", "a") == std::vector<int>());
    
    // Overlapping anagrams.
    assert(findAllAnagrams("abaab", "ab") == std::vector<int>({0, 2, 3}));
    
    // Larger case with surrounding characters.
    assert(findAllAnagrams("abcdcba", "abc") == std::vector<int>({0, 4}));
}
