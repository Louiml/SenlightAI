Write a C++ function `extractVowelsAndConsonants` that takes a single C-string (null-terminated array of characters) as input and returns a `std::pair<std::string, std::string>` where the first string contains all vowel characters (both uppercase and lowercase, in their original case) in the order they appear, and the second string contains all consonant characters (also preserving original case) in the order they appear. Non-alphabetic characters (digits, spaces, punctuation, symbols) must be ignored. The function should handle empty strings, strings with only non-alphabetic characters, and strings containing both uppercase and lowercase letters. The returned strings should be exactly the accumulated vowels and consonants, with no extra characters or separators. Implement the function with proper `const` correctness—the input parameter must be `const char*`—and ensure the algorithm processes the input character by character in a single pass.

#include <cassert>
#include <string>
#include <utility>

// Forward declaration for the solution function (assumed to be in the same translation unit)
std::pair<std::string, std::string> extractVowelsAndConsonants(const char* str);

int main() {
    // Basic mixed-case example
    auto result1 = extractVowelsAndConsonants("Hello World");
    assert(result1.first == "eo o");
    assert(result1.second == "HllWrld");
    
    // Only vowels, both cases
    auto result2 = extractVowelsAndConsonants("AEIOUaeiou");
    assert(result2.first == "AEIOUaeiou");
    assert(result2.second == "");
    
    // Only consonants, both cases
    auto result3 = extractVowelsAndConsonants("BCDFGbcdfg");
    assert(result3.first == "");
    assert(result3.second == "BCDFGbcdfg");
    
    // Empty string
    auto result4 = extractVowelsAndConsonants("");
    assert(result4.first == "");
    assert(result4.second == "");
    
    // Only non-alphabetic characters
    auto result5 = extractVowelsAndConsonants("123 456 !@#$");
    assert(result5.first == "");
    assert(result5.second == "");
    
    // Mixed with digits and punctuation, preserving order and case
    auto result6 = extractVowelsAndConsonants("a1E2i3O4u5");
    assert(result6.first == "aEiOu");
    assert(result6.second == "");
    
    // String with spaces and special characters between letters
    auto result7 = extractVowelsAndConsonants("C++ is Fun! 2023");
    assert(result7.first == "i u");
    assert(result7.second == "CsFn");
    
    // Null pointer should be handled safely (returns empty strings)
    auto result8 = extractVowelsAndConsonants(nullptr);
    assert(result8.first == "");
    assert(result8.second == "");
    
    // Single character
    auto result9 = extractVowelsAndConsonants("Z");
    assert(result9.first == "");
    assert(result9.second == "Z");
    
    // Long string with all letters
    auto result10 = extractVowelsAndConsonants("AeIoU bCdEfG");
    assert(result10.first == "AeIoU E");
    assert(result10.second == "bCdfG");
    
    return 0;
}

#include <string>
#include <utility>

// Extract vowels and consonants from a C-string, preserving original case.
// Returns a pair: first = vowels, second = consonants. Non-alphabetic chars are ignored.
std::pair<std::string, std::string> extractVowelsAndConsonants(const char* str) {
    std::string vowels;
    std::string consonants;
    
    if (str == nullptr) {
        return {vowels, consonants};
    }
    
    for (int i = 0; str[i] != '\0'; ++i) {
        char ch = str[i];
        // Check for vowel (both cases)
        if (ch == 'a' || ch == 'e' || ch == 'i' || ch == 'o' || ch == 'u' ||
            ch == 'A' || ch == 'E' || ch == 'I' || ch == 'O' || ch == 'U') {
            vowels.push_back(ch);
        }
        // Check for consonant (any alphabetic character that is not a vowel)
        else if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')) {
            consonants.push_back(ch);
        }
        // Non-alphabetic characters are ignored
    }
    
    return {vowels, consonants};
}

// The solution is straightforward: initialize two empty strings (or vectors of characters) for vowels and consonants. Iterate through the input C-string until the null terminator `'\0'` is reached. For each character, check if it is a vowel by comparing against a set of ten characters (`a, e, i, o, u, A, E, I, O, U`). If it matches, append to the vowels string. Otherwise, check if it is an alphabetic letter using `(c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')`; if so, append to the consonants string. All other characters (digits, spaces, punctuation) are skipped. After the loop, return the pair containing both strings. Edge cases: empty input string results in both strings being empty; a string with only non-letters produces two empty strings; mixed uppercase and lowercase are preserved because we append the original character unchanged. Time complexity is \(O(n)\) where \(n\) is the length of the input string because we scan each character once. Space complexity is \(O(n)\) for the output strings, but auxiliary space (excluding the output) is \(O(1)\) as we only use a few index/counter variables.
