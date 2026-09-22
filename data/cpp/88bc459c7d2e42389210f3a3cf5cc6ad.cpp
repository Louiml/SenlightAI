Write a C++ function named `areAnagrams` that takes two strings `a` and `b` consisting only of lowercase English letters and returns a `bool` indicating whether the two strings are anagrams of each other. Two strings are anagrams if they contain exactly the same characters with the same frequencies, regardless of the order of characters. For example, `"listen"` and `"silent"` are anagrams, while `"hello"` and `"bello"` are not. The function must handle unequal-length strings efficiently by returning `false` immediately without further processing. Your implementation must avoid sorting and instead use a hash map (or frequency array) to count character occurrences, guaranteeing linear time complexity. The function must be `const`-correct, take the input strings by `const std::string&`, and be self-contained (include all necessary headers). Do not provide a `main` function in the solution; only the function itself.

#include <cassert>

int main() {
    // Basic anagram pairs.
    assert(areAnagrams("listen", "silent") == true);
    assert(areAnagrams("hello", "olleh") == true);
    assert(areAnagrams("act", "tac") == true);

    // Non-anagram pairs.
    assert(areAnagrams("hello", "bello") == false);
    assert(areAnagrams("abc", "abd") == false);
    assert(areAnagrams("xyz", "xy") == false); // different lengths

    // Edge cases.
    assert(areAnagrams("", "") == true);
    assert(areAnagrams("a", "a") == true);
    assert(areAnagrams("a", "b") == false);
    assert(areAnagrams("aabbcc", "abcabc") == true);
    assert(areAnagrams("aaa", "aab") == false);

    return 0;
}

#include <string>
#include <unordered_map>

// Check whether two strings are anagrams using a hash map to count character frequencies.
bool areAnagrams(const std::string& a, const std::string& b) {
    // Different lengths cannot be anagrams.
    if (a.size() != b.size()) {
        return false;
    }

    // Count frequency of each character in the first string.
    std::unordered_map<char, int> frequency;
    for (char ch : a) {
        ++frequency[ch];
    }

    // Subtract frequency of each character in the second string.
    for (char ch : b) {
        --frequency[ch];
    }

    // If all frequencies are zero, the strings are anagrams.
    for (const auto& pair : frequency) {
        if (pair.second != 0) {
            return false;
        }
    }

    return true;
}

// The problem requires checking whether two strings are permutations of each other. The most efficient approach is to count the frequency of each character in both strings and compare the frequency distributions. Since the input is restricted to lowercase English letters (26 characters), we can use a fixed-size integer array of size 26 for counting, which provides O(1) auxiliary space. However, for generality and clarity, we can also use an `unordered_map<char, int>`. The algorithm proceeds as follows: (1) If the lengths differ, return `false` immediately because anagrams must have equal length. (2) Increment counts for characters in the first string. (3) Decrement counts for characters in the second string. (4) After processing both strings, if any count in the map or array is non-zero, the strings are not anagrams; otherwise, they are. Alternatively, one can count both separately and compare. Edge cases include empty strings (both empty are anagrams), identical strings, strings with repeated characters, and strings where shifting characters could cause overflow if using signed types—use `int` for counts (safe because max length is limited by memory, and `int` is sufficient for typical inputs). Time complexity is O(n) where n is the length of the strings, and space complexity is O(1) if using a fixed-size array or O(k) in the general case for a hash map where k is the number of distinct characters (at most 26).
