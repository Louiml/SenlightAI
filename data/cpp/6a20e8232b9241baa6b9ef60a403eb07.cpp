/*
Write a C++ function `bool containsPermutation(const std::string& s1, const std::string& s2)` that determines whether any permutation of `s1` appears as a contiguous substring of `s2`. The function should return `true` if such a substring exists, and `false` otherwise. The input strings consist only of lowercase English letters (`a`–`z`), and the function must handle cases where `s1` is longer than `s2`, empty strings, and duplicate characters. For example, `containsPermutation("ab", "eidbaooo")` should return `true` because `"ba"` is a permutation of `"ab"` and appears contiguously; `containsPermutation("ab", "eidboaoo")` should return `false`.
*/

#include <string>
#include <vector>

// Check if any permutation of s1 is a contiguous substring of s2.
// s1 and s2 contain only lowercase English letters.
bool containsPermutation(const std::string& s1, const std::string& s2) {
    if (s2.length() < s1.length()) return false;
    if (s1.empty()) return true;

    std::vector<int> target(26, 0);
    for (char c : s1) {
        target[c - 'a']++;
    }

    std::vector<int> window(26, 0);
    int left = 0;
    for (int right = 0; right < static_cast<int>(s2.length()); ++right) {
        window[s2[right] - 'a']++;
        if (right - left + 1 > static_cast<int>(s1.length())) {
            window[s2[left] - 'a']--;
            left++;
        }
        if (right - left + 1 == static_cast<int>(s1.length())) {
            if (window == target) return true;
        }
    }
    return false;
}

#include <cassert>
#include <string>

// Declare the solution function (from above)
bool containsPermutation(const std::string& s1, const std::string& s2);

int main() {
    // Basic cases
    assert(containsPermutation("ab", "eidbaooo") == true);   // "ba" is permutation
    assert(containsPermutation("ab", "eidboaoo") == false);   // no contiguous match
    assert(containsPermutation("abc", "cab") == true);         // exact
    assert(containsPermutation("abc", "cba") == true);         // reverse

    // Edge cases: s1 longer than s2
    assert(containsPermutation("abcd", "abc") == false);
    // Empty s1 should be true (empty substring exists)
    assert(containsPermutation("", "anything") == true);
    // s2 empty with non-empty s1
    assert(containsPermutation("a", "") == false);
    // Duplicate characters
    assert(containsPermutation("aa", "baa") == true);
    assert(containsPermutation("aa", "aba") == false);
    // Single character
    assert(containsPermutation("z", "zzz") == true);
    assert(containsPermutation("z", "abc") == false);
    // Larger window near the end
    assert(containsPermutation("abc", "defabc") == true);
    assert(containsPermutation("abc", "defab") == false);
    return 0;
}

// The solution uses a sliding window with fixed size equal to the length of `s1`. We first build a frequency array of size 26 for `s1` counting each character’s occurrences. Then we initialize a second frequency array for the window in `s2`. We slide the window from the leftmost position to the right: when the window size equals `len(s1)`, compare the two frequency arrays—if identical, a permutation exists. Otherwise, remove the leftmost character’s frequency and shift the window right by one. The key edge cases: if `s2` is shorter than `s1`, return `false` immediately; empty `s1` is a permutation of itself (though commonly we treat it as `true` because an empty substring exists); duplicate characters are handled naturally by frequency counts. Time complexity is O(n) where n = length of `s2`, because each character is added and removed at most once, and each comparison is O(26) ≈ constant. Space complexity is O(1) for the two fixed-size arrays.
