/*
Write a C++ function named `isAnagram` that takes two strings as input and returns a boolean value indicating whether they are anagrams of each other. The comparison must be case-insensitive (e.g., "Listen" and "Silent" are anagrams) and ignore spaces and punctuation, considering only alphabetic characters. The function should return `true` if both strings contain the same multiset of letters (ignoring case and non-alphabetic characters), and `false` otherwise. You may assume the input strings are non-null and can be of any length, including empty strings or strings containing only spaces and punctuation — such inputs should be treated as anagrams if they have no alphabetic characters at all.
*/

#include <string>
#include <cctype>
#include <array>

// Returns true if two strings are anagrams ignoring case, spaces, and punctuation.
bool isAnagram(const std::string& s1, const std::string& s2) {
    std::array<int, 26> count = {0};

    for (char ch : s1) {
        if (std::isalpha(ch)) {
            count[std::tolower(ch) - 'a']++;
        }
    }

    for (char ch : s2) {
        if (std::isalpha(ch)) {
            count[std::tolower(ch) - 'a']--;
        }
    }

    for (int c : count) {
        if (c != 0) {
            return false;
        }
    }

    return true;
}

#include <cassert>

int main() {
    // Basic case-insensitive anagrams
    assert(isAnagram("Listen", "Silent") == true);
    assert(isAnagram("Hello", "Olleh") == true);

    // Different lengths -> not anagrams
    assert(isAnagram("abc", "ab") == false);

    // Completely different letters
    assert(isAnagram("abc", "def") == false);

    // Strings with spaces, punctuation, and mixed case
    assert(isAnagram("A gentleman", "Elegant man") == true);
    assert(isAnagram("Tom Marvolo Riddle", "I am Lord Voldemort") == true);

    // Empty strings or only punctuation/spaces -> true (no letters)
    assert(isAnagram("", "") == true);
    assert(isAnagram("  !!!  ", "   ...  ") == true);

    // One has letters, the other has none
    assert(isAnagram("a", " ") == false);

    // Repeated letters with frequent occurrences
    assert(isAnagram("aaabbb", "aababb") == true);
    assert(isAnagram("aaabbb", "aabb") == false);

    // Case and symbol mixing
    assert(isAnagram("Dormitory", "Dirty room") == true);
    assert(isAnagram("The Morse Code", "Here come dots") == true);
}

// The solution uses a frequency counting approach. First, we normalize both strings by extracting only alphabetic characters and converting them to lowercase (or uppercase). We can do this by iterating over each character in the input strings, and if `std::isalpha(ch)` is true, increment a count in an array indexed by the character (e.g., `count[ch - 'a']` after converting to lowercase). We can either build two separate count arrays and compare them, or use a single array by incrementing for the first string and decrementing for the second, then checking that all entries are zero. The latter is more space-efficient. Edge cases include empty strings or strings with no alphabetic characters — both should be considered anagrams if both have zero alphabetic characters. The time complexity is O(n + m) where n and m are the lengths of the two strings, and space complexity is O(1) (fixed array of size 26 for lowercase letters).
