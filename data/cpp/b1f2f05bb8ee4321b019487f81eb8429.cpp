/*
Write a C++ function named `countWords` that takes a single `std::string` as input and returns an `int` representing the number of words in that string. Words are defined as sequences of non-space characters separated by one or more spaces. The input may contain leading, trailing, or multiple consecutive spaces, and it always ends with a newline character (which should be ignored, not counted as a space). The string is guaranteed to be non-empty and may contain only printable ASCII characters (including letters, digits, punctuation). Do not use any standard library function that directly splits strings (e.g., `std::getline`, `std::istringstream` with extraction). Instead, implement the counting manually by iterating through characters and tracking state. The function must be `const`-correct and should not modify the input.
*/
#include <string>

// Count the number of words in a string, where words are separated by spaces.
// Leading, trailing, and multiple consecutive spaces are ignored.
int countWords(const std::string& text) {
    int count = 0;
    bool inWord = false;

    for (char ch : text) {
        if (ch == ' ') {
            if (inWord) {
                ++count;
                inWord = false;
            }
        } else {
            inWord = true;
        }
    }

    // If the string ended while still inside a word, count that last word.
    if (inWord) {
        ++count;
    }

    return count;
}
#include <cassert>
#include <string>

int main() {
    assert(countWords("") == 0); // Though originally non-empty, robust check.
    assert(countWords("hello") == 1);
    assert(countWords("hello world") == 2);
    assert(countWords("  hello   world  ") == 2);
    assert(countWords("a b c") == 3);
    assert(countWords(" single ") == 1);
    assert(countWords("one two  three   four") == 4);
    assert(countWords(" ") == 0);
    assert(countWords("word1 word2 word3") == 3);
    assert(countWords("a") == 1);
}
// The main idea is to iterate over every character of the input string. We maintain a boolean flag `inWord` that indicates whether we are currently inside a word (i.e., the previous character was non-space). For each character: if it is a space and `inWord` is true, we have just finished a word, so increment the word count and set `inWord` to false. If the character is not a space, set `inWord` to true. After the loop ends, if `inWord` is still true (meaning the string ended with a non-space character), we need to increment the count one final time. Edge cases: (1) multiple consecutive spaces — handled because we only increment when transitioning from a non-space to a space; (2) leading spaces — handled because `inWord` starts false, so spaces at the beginning do not increment; (3) trailing spaces — handled because after the loop, if the last character is a space, `inWord` is false, so no extra increment. Note that the original snippet's use of a fixed-size array is replaced by a proper string; also the original snippet had a bug when the string starts with a space (it decremented the count incorrectly). Our solution avoids that. Time complexity is O(n) where n is the length of the string; space complexity is O(1) extra.
