/*
Write a C++ function named `hasRepeatingLetters` that takes a string containing only lowercase alphabetic characters and returns a `bool` indicating whether the string contains any character that appears more than once. The function should treat an empty string as having no repeating letters, and should efficiently handle strings up to a few thousand characters. The function must not modify the input string and must use appropriate `const` qualifiers.
*/
#include <string>
#include <array>

// Returns true if the input string (lowercase letters only) contains any character more than once.
bool hasRepeatingLetters(const std::string& input) {
    std::array<int, 26> counts{};
    
    for (char c : input) {
        int index = c - 'a';
        ++counts[index];
        if (counts[index] > 1) {
            return true;
        }
    }
    return false;
}
#include <cassert>

int main() {
    assert(hasRepeatingLetters("abc") == false);
    assert(hasRepeatingLetters("abca") == true);
    assert(hasRepeatingLetters("hello") == true);
    assert(hasRepeatingLetters("") == false);
    assert(hasRepeatingLetters("z") == false);
    assert(hasRepeatingLetters("abcdefghijklmnopqrstuvwxyz") == false);
    assert(hasRepeatingLetters("aabcdef") == true);
    assert(hasRepeatingLetters("abcdefa") == true);
    assert(hasRepeatingLetters("aa") == true);
    assert(hasRepeatingLetters("abcde") == false);
}
// The solution approach is straightforward: use a fixed-size frequency array of 26 integers (one per lowercase letter) initialized to zero. Iterate through each character in the input string, increment its corresponding counter. If a counter reaches 2, immediately return `true` because a duplicate has been found. If the loop completes without any counter exceeding 1, return `false`. This handles the empty string correctly since the loop body never executes. No special handling is needed for edge cases like repeated letters at the start or the end—the algorithm naturally detects the first duplicate. Time complexity is \(O(n)\) where \(n\) is the length of the string, and space complexity is \(O(1)\) since the frequency array has a fixed size of 26.
