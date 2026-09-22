/*
Write a C++ function `string getHangmanDisplay(const string& word, const vector<char>& guessed)` that, given a target word and a vector of characters that have been guessed so far, returns a string showing the word with each letter replaced by a hyphen `-` unless that letter has been guessed (case-insensitively). The function must handle words containing both uppercase and lowercase letters by treating guesses as case-insensitive: if a guess `'A'` is present, it should reveal both `'a'` and `'A'` in the display. The returned display string must preserve the original case of the letters in the word. Guard against an empty guessed vector (return all hyphens) and an empty word (return an empty string). The function must not modify the input strings or vectors, and should use `const` references for parameters.
*/

#include <string>
#include <vector>
#include <cctype>

// Returns a display string where each character of word is shown if it has been guessed (case-insensitive), else '-'.
std::string getHangmanDisplay(const std::string& word, const std::vector<char>& guessed) {
    std::string display;
    display.reserve(word.size());

    for (char ch : word) {
        char lowerCh = static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));
        bool found = false;
        for (char g : guessed) {
            char lowerG = static_cast<char>(std::tolower(static_cast<unsigned char>(g)));
            if (lowerCh == lowerG) {
                found = true;
                break;
            }
        }
        display += found ? ch : '-';
    }
    return display;
}

#include <cassert>
#include <string>
#include <vector>

// assume getHangmanDisplay is declared above or included here

int main() {
    std::vector<char> guessed1 = {'a', 'e'};
    assert(getHangmanDisplay("apple", guessed1) == "a---e");

    std::vector<char> guessed2 = {'A', 'E'};
    assert(getHangmanDisplay("Apple", guessed2) == "A---e");

    std::vector<char> guessed3 = {'b', 'c'};
    assert(getHangmanDisplay("abc", guessed3) == "-bc");

    std::vector<char> guessed4;
    assert(getHangmanDisplay("hello", guessed4) == "-----");

    std::vector<char> guessed5 = {'h', 'e', 'l', 'o'};
    assert(getHangmanDisplay("HELLO", guessed5) == "HELLO");

    std::vector<char> guessed6 = {'x'};
    assert(getHangmanDisplay("", guessed6) == "");

    std::vector<char> guessed7 = {'p', 'P'};
    assert(getHangmanDisplay("Pp", guessed7) == "Pp");
}

// The solution iterates through each character of the input `word` and checks whether that character (converted to lowercase) appears in the `guessed` vector (also converted to lowercase) using a linear search. For each letter, if found, we append the original character from `word` to the result; otherwise, we append `'-'`. The case-insensitive comparison is achieved by calling `std::tolower` on both the word character and each guessed character. Edge cases include an empty word (immediately return an empty string), an empty guessed vector (all hyphens), and duplicates in the guessed vector—duplicates do not affect correctness because we only check for presence, not count. The time complexity is \(O(n \cdot m)\), where \(n\) is the length of the word and \(m\) is the number of guessed characters (since for each letter we scan the guessed vector). The space complexity is \(O(n)\) for the returned string; no additional significant auxiliary space is used.
