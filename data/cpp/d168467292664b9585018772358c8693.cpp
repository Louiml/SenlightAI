// Write a C++ function that takes two strings, `s` and `t`, and returns `true` if `t` is an anagram of `s` (i.e., they contain the same characters with the same frequencies, ignoring order), and `false` otherwise. The strings may contain any printable ASCII characters (characters with integer values 0–255), including spaces, digits, punctuation, uppercase and lowercase letters (treat 'A' and 'a' as different). The function should handle empty strings correctly (two empty strings are anagrams). You may not use any standard library sorting or counting functions; you must implement the frequency counting yourself using a fixed-size array. Provide a descriptive function name and ensure the function is `const`-correct (i.e., it should not modify its input parameters).

#include <cassert>

int main() {
    // Basic anagram
    assert(isAnagram("listen", "silent") == true);
    // Different lengths
    assert(isAnagram("abc", "ab") == false);
    // Non-anagram with same characters but different counts
    assert(isAnagram("aab", "abb") == false);
    // Empty strings
    assert(isAnagram("", "") == true);
    // One empty, one not
    assert(isAnagram("", "a") == false);
    // Case sensitivity
    assert(isAnagram("A", "a") == false);
    // Spaces and punctuation
    assert(isAnagram("a b c", "c b a") == true);
    // Repeat characters
    assert(isAnagram("anagram", "nagaram") == true);
    // High ASCII characters (e.g., 'ÿ' has code 255)
    assert(isAnagram(std::string(1, static_cast<char>(255)) + "x", "x" + std::string(1, static_cast<char>(255))) == true);
    // Same string
    assert(isAnagram("same", "same") == true);
    return 0;
}

#include <array>
#include <cstddef>
#include <string>

// Returns true if t is an anagram of s (same characters with same frequencies).
bool isAnagram(const std::string& s, const std::string& t) {
    std::array<int, 256> charCount{};
    
    for (char c : s) {
        charCount[static_cast<unsigned char>(c)]++;
    }
    for (char c : t) {
        charCount[static_cast<unsigned char>(c)]--;
    }
    for (int count : charCount) {
        if (count != 0) {
            return false;
        }
    }
    return true;
}

// The solution uses a fixed-size integer array of 256 elements (one per possible character value in the assumed ASCII/byte range) to count character frequencies. First, initialize all entries to zero. Then iterate over the first string, incrementing the count for each character’s ASCII value. Next, iterate over the second string, decrementing the count for each character. After processing both strings, if all entries in the array are zero, the strings have identical character frequencies and are anagrams; otherwise, they are not. Edge cases: empty strings (array stays all zero, returns `true`), strings of different lengths (the count array will contain non-zero values because the total increments and decrements won’t balance), and characters with high ASCII values (like 255) are handled because the array size is 256. Time complexity is O(n + m) where n and m are the lengths of the two strings, and space complexity is O(1) since the array size is constant.
