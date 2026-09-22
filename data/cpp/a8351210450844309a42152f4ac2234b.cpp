Write a C++ function that takes a string as input (which may contain letters, digits, punctuation, and whitespace) and returns the number of "words" in it, where a word is defined as a maximal contiguous sequence of alphabetic characters (both uppercase and lowercase). Sequential alphabetic runs should be counted as a single word, and any non-alphabetic character separates words. The function should handle empty strings and strings with only punctuation/whitespace correctly (returning 0). The solution must be a standalone free function with a descriptive name, take a `const std::string&` parameter, and return an `int`. Do not use any global variables or I/O inside the function.
#include <cassert>
#include <string>

// Forward declaration (included here for clarity; in practice the function is above).
int countWords(const std::string& text);

int main() {
    assert(countWords("") == 0);
    assert(countWords("   ") == 0);
    assert(countWords("!@#$") == 0);
    assert(countWords("abc") == 1);
    assert(countWords("  abc  ") == 1);
    assert(countWords("abc def") == 2);
    assert(countWords("a1b2c") == 3);
    assert(countWords("Hello, world!") == 2);
    assert(countWords("one-two three_four") == 4);
    assert(countWords("ABC") == 1);
    assert(countWords("a b c d") == 4);
}
#include <string>
#include <cctype>

// Count the number of maximal alphabetic sequences in a string.
int countWords(const std::string& text) {
    int wordCount = 0;
    bool inWord = false;

    for (char c : text) {
        if (std::isalpha(static_cast<unsigned char>(c))) {
            if (!inWord) {
                ++wordCount;
                inWord = true;
            }
        } else {
            inWord = false;
        }
    }

    return wordCount;
}
// The approach is to iterate through each character of the string once. Maintain a boolean flag that indicates whether we are currently inside a word (i.e., the previous character was alphabetic). Initially, the flag is false. When we encounter an alphabetic character and the flag is false, we have found the start of a new word, so increment the word counter and set the flag to true. If the character is alphabetic and the flag is true, we are continuing the same word, so do nothing. If the character is non-alphabetic, set the flag to false. This correctly counts maximal contiguous alphabetic sequences. Edge cases: empty string returns 0; a string with only non-alphabetic characters returns 0; a string starting with a letter or ending with a letter is handled because the flag resets correctly on the first non-letter. Time complexity is O(n) where n is string length, and space complexity is O(1) auxiliary (ignoring the input string's storage). The original snippet uses `gets` (obsolete) and C-style char arrays; we modernize to `std::string` and `std::isalpha` for portability and safety.
