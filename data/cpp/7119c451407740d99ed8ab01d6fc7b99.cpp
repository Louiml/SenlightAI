/*
Write a C++ function `bool areAnagrams(const std::string& a, const std::string& b)` that returns `true` if the two input strings are anagrams of each other (i.e., they contain the same characters with the same frequencies, ignoring case and considering only alphabetic characters, while ignoring all non‑alphabetic characters such as digits, punctuation, and spaces). The function must treat uppercase and lowercase letters as equivalent (e.g., `"Aabc"` and `"aBc a"` are anagrams). If the strings contain no alphabetic characters at all, the function should return `true` (both are empty after filtering). Use an unordered map (or array of size 26) to count letter frequencies. You must not modify the input strings; use `const` references and apply appropriate `const` correctness. Provide a self‑contained implementation with necessary headers and comments, but no `main` function in the solution code.
*/

#include <string>
#include <cctype>
#include <unordered_map>

// Returns true if the two input strings are anagrams of each other,
// considering only alphabetic characters and ignoring case.
bool areAnagrams(const std::string& a, const std::string& b) {
    // Count frequencies of letters in each string.
    std::unordered_map<char, int> freqA, freqB;

    // Process both strings simultaneously, but we need to handle different lengths.
    // We'll process the longer length and skip non‑alphabetic characters.
    const std::size_t lenA = a.size();
    const std::size_t lenB = b.size();
    const std::size_t maxLen = (lenA > lenB) ? lenA : lenB;

    for (std::size_t i = 0; i < maxLen; ++i) {
        if (i < lenA) {
            char c = std::tolower(static_cast<unsigned char>(a[i]));
            if (std::isalpha(static_cast<unsigned char>(a[i]))) {
                ++freqA[c];
            }
        }
        if (i < lenB) {
            char c = std::tolower(static_cast<unsigned char>(b[i]));
            if (std::isalpha(static_cast<unsigned char>(b[i]))) {
                ++freqB[c];
            }
        }
    }

    // Compare frequency maps.
    if (freqA.size() != freqB.size()) {
        return false;
    }
    for (const auto& entry : freqA) {
        auto it = freqB.find(entry.first);
        if (it == freqB.end() || it->second != entry.second) {
            return false;
        }
    }
    return true;
}

#include <cassert>
#include <string>

// The function declaration is assumed to be available from the solution above.
bool areAnagrams(const std::string& a, const std::string& b);

int main() {
    // Basic anagrams
    assert(areAnagrams("listen", "silent") == true);
    assert(areAnagrams("triangle", "integral") == true);

    // Different lengths → not anagrams
    assert(areAnagrams("abc", "ab") == false);

    // Case‑insensitive
    assert(areAnagrams("Aabc", "aBc a") == true);

    // Non‑alphabetic characters are ignored
    assert(areAnagrams("hello!!", "olleh?") == true);
    assert(areAnagrams("12#", "xyz") == true); // both have no letters → empty

    // Real difference in letter counts
    assert(areAnagrams("aabb", "abab") == true);
    assert(areAnagrams("aabb", "abba") == false);

    // Empty strings
    assert(areAnagrams("", "") == true);
    assert(areAnagrams("", "a") == false);

    // Mixed case and punctuation
    assert(areAnagrams("Debit Card", "Bad Credit") == true);
    assert(areAnagrams("School", "sChOoL") == true);
    return 0;
}

// The core idea is to normalize the strings by converting to lowercase (or uppercase) and filtering out non‑alphabetic characters, then compare the frequency of each letter. A simple approach uses two `std::unordered_map<char, int>` (or an array of 26 ints) to count occurrences of every letter. First, check if after filtering the effective lengths are equal; if not, return `false` immediately (though this is optional because a frequency comparison will catch it, it saves work). Iterate over each character of both strings simultaneously: for each pair of characters, if both are alphabetic, increment the corresponding counters. Alternatively, build two maps from the filtered lowercase versions. Finally, compare the maps: if every key in one map has the same count in the other, return `true`. Edge cases: empty strings or strings with only non‑alphabetic characters → both maps remain empty → they are anagrams. Strings with mixed case must be normalized with `std::tolower`. Time complexity is O(n + m) where n and m are the lengths of the input strings (we traverse each once). Space complexity is O(k) where k is the number of distinct letters (at most 26) — O(1) in practice.
