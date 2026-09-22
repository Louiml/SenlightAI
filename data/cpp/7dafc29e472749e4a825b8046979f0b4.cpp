// Write a C++ function named `countVowelsAndConsonants` that takes a non-empty string containing only lowercase English letters and returns a `std::pair<int, int>`, where the first element is the number of vowels (`a`, `e`, `i`, `o`, `u`) and the second element is the number of consonants. The function must be case-sensitive (input is guaranteed lowercase) and must not modify the input. You may use standard library facilities like `std::unordered_set` for vowel lookup, but avoid using `std::count_if` or similar one-liners that hide the loop logic. The solution must be self-contained, with only necessary includes (`<string>`, `<utility>`, `<unordered_set>`), and no global variables.

#include <cassert>
#include <utility>

// Function declaration (definition above) for testing
std::pair<int, int> countVowelsAndConsonants(const std::string& input);

int main() {
    // Single vowel
    assert(countVowelsAndConsonants("a") == std::make_pair(1, 0));
    // Single consonant
    assert(countVowelsAndConsonants("b") == std::make_pair(0, 1));
    // All vowels
    assert(countVowelsAndConsonants("aeiou") == std::make_pair(5, 0));
    // All consonants
    assert(countVowelsAndConsonants("bcdfg") == std::make_pair(0, 5));
    // Mixed simple word
    assert(countVowelsAndConsonants("hello") == std::make_pair(2, 3));
    // Longer mixed word
    assert(countVowelsAndConsonants("programming") == std::make_pair(3, 8));
    // Repeats
    assert(countVowelsAndConsonants("bookkeeper") == std::make_pair(5, 5));
    // Single letter repeated
    assert(countVowelsAndConsonants("zzz") == std::make_pair(0, 3));
    // All 26 letters
    assert(countVowelsAndConsonants("abcdefghijklmnopqrstuvwxyz") == std::make_pair(5, 21));
    // Empty string (not in spec, but test defensive behavior)
    // Uncomment if you want to test an edge case; the function returns (0,0) for empty.
    // assert(countVowelsAndConsonants("") == std::make_pair(0, 0));
}

#include <string>
#include <utility>
#include <unordered_set>

// Count vowels (a, e, i, o, u) and consonants in a string of lowercase letters.
// Returns a pair: first = count of vowels, second = count of consonants.
std::pair<int, int> countVowelsAndConsonants(const std::string& input) {
    static const std::unordered_set<char> vowels = {'a', 'e', 'i', 'o', 'u'};
    int vowelCount = 0;
    int consonantCount = 0;
    
    for (char ch : input) {
        if (vowels.count(ch)) {
            ++vowelCount;
        } else {
            ++consonantCount;
        }
    }
    
    return {vowelCount, consonantCount};
}

// The approach is to iterate through each character of the input string once. For each character, check if it exists in a pre-defined set of vowels. If yes, increment the vowel counter; otherwise, increment the consonant counter (since the input is guaranteed to contain only lowercase letters, any non-vowel is a consonant). The vowel set can be implemented as a `std::unordered_set<char>` initialized with the five vowels for O(1) average lookup. Edge cases: a single-character string, all vowels, all consonants, or a mix — all are handled naturally. Time complexity is O(n), where n is the length of the string, and space complexity is O(1) because the vowel set has a constant size (5 elements) and we only store two counters.
