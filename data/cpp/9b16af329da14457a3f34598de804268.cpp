/*
Write a C++ function named `AreAnagrams` that takes two `std::string` parameters (by const reference) and returns a `bool` indicating whether the two strings are anagrams of each other. For this task, an anagram is defined as two strings that contain the same multiset of letters, ignoring case, spaces, punctuation, and all non-alphabetic characters. For example, `"Listen!!"` and `"Silent..."` are anagrams, but `"Hello"` and `"World"` are not. The function must handle empty strings, strings with only non-letters, and duplicate letters correctly. Use only the C++ standard library, and ensure the function is `const`-correct (i.e., it must not modify its inputs).
*/

#include <string>
#include <algorithm>
#include <cctype>

/**
 * Determines whether two strings are anagrams, ignoring non-alphabetic characters
 * and case. Returns true if both strings contain the same multiset of letters.
 *
 * @param s1 First string to compare.
 * @param s2 Second string to compare.
 * @return true if the strings are anagrams; false otherwise.
 */
bool AreAnagrams(const std::string& s1, const std::string& s2) {
    // Helper to normalize: keep only alphabetic chars, lowercase, and sort.
    auto normalize = [](const std::string& str) {
        std::string result;
        for (char ch : str) {
            if (std::isalpha(static_cast<unsigned char>(ch))) {
                result.push_back(std::tolower(static_cast<unsigned char>(ch)));
            }
        }
        std::sort(result.begin(), result.end());
        return result;
    };

    return normalize(s1) == normalize(s2);
}

#include <cassert>
#include <string>

// Declaration of the function under test (assumed to be in scope).
bool AreAnagrams(const std::string& s1, const std::string& s2);

int main() {
    // Basic cases
    assert(AreAnagrams("listen", "silent") == true);
    assert(AreAnagrams("hello", "world") == false);
    
    // Case and punctuation insensitivity
    assert(AreAnagrams("Listen!!", "Silent...") == true);
    assert(AreAnagrams("Dormitory", "Dirty room") == true);  // spaces ignored
    assert(AreAnagrams("A man, a plan, a canal: Panama", "amanaplanacanalpanama") == true);
    
    // Duplicate letters
    assert(AreAnagrams("aabb", "abab") == true);
    assert(AreAnagrams("aaa", "aa") == false);
    
    // Empty and non-alphabetic-only inputs
    assert(AreAnagrams("", "") == true);
    assert(AreAnagrams("123", "!@#") == true);  // both normalize to empty
    assert(AreAnagrams("123abc", "abc") == true);
    
    // Different lengths after normalization
    assert(AreAnagrams("cat", "cats") == false);
    
    return 0;
}

// The core approach is to normalize both strings into a canonical form that retains only alphabetic characters, converts them to lowercase, and sorts them alphabetically. Once both strings are normalized in this way, they are anagrams if and only if their normalized forms are identical. The normalization process works by iterating through each character of the input string, checking `std::isalpha(ch)` (which returns true only for A-Z and a-z, ignoring locale-specific extended characters in most default locales), and pushing the lowercase version of that character (`std::tolower(ch)`) into a result string. After building the filtered, lowercased string, `std::sort` rearranges its characters lexicographically. This sorting step makes the comparison independent of character order. Edge cases include: (1) two empty strings are trivially anagrams because both normalize to empty strings; (2) strings with only non-alphabetic characters (e.g., "123" and "!@#") also normalize to empty strings and should be considered anagrams; (3) case differences and punctuation are ignored by design; (4) duplicate letters are preserved and handled correctly because sorting groups identical letters together and the comparison is character-by-character. The time complexity is \(O(n_1 \log n_1 + n_2 \log n_2)\) where \(n_1\) and \(n_2\) are the lengths of the two input strings, dominated by the sorting steps. The auxiliary space complexity is \(O(n_1 + n_2)\) due to the temporary normalized strings.
