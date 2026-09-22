Write a C++ function that, given a string `s` containing words separated by whitespace (spaces, tabs, newlines), returns the length of the last word in the string. A word is defined as a maximal contiguous sequence of non-whitespace characters. The input string may contain leading, trailing, or multiple consecutive whitespace characters, and it is guaranteed to contain at least one word. The function must be case-sensitive, handle strings with only one word, and preserve efficiency for potentially very long inputs.

#include <cassert>
#include <string>

// Function declaration (provided in the solution section above).
int lengthOfLastWord(const std::string& s);

int main() {
    // Basic case with three words.
    assert(lengthOfLastWord("Hello World") == 5);
    // Leading and trailing spaces.
    assert(lengthOfLastWord("   fly me   to   the moon  ") == 4);
    // Single word only.
    assert(lengthOfLastWord("luffy") == 5);
    // Tabs and newlines as separators.
    assert(lengthOfLastWord("one\ttwo\nthree") == 5);
    // Multiple spaces between words, last word short.
    assert(lengthOfLastWord("a b   c") == 1);
    // Last word longer than first, extra spaces at end.
    assert(lengthOfLastWord("    goodbye   world     ") == 5);
    // Word followed by trailing newline.
    assert(lengthOfLastWord("hello\n") == 5);
    // Only one word with leading spaces.
    assert(lengthOfLastWord("   single") == 6);
    // Last word contains digits and punctuation.
    assert(lengthOfLastWord("abc def123!") == 7);
    // Very long last word.
    assert(lengthOfLastWord("short verylongwordhere") == 18);
    return 0;
}

#include <string>
#include <sstream>

// Returns the length of the last whitespace-separated word in the input string.
// The input must contain at least one non-whitespace character.
int lengthOfLastWord(const std::string& s) {
    std::istringstream stream(s);
    std::string word;
    while (stream >> word) {
        // Continuously overwrite word until the last token is read.
    }
    return static_cast<int>(word.length());
}

// The simplest and most robust approach is to use a `std::istringstream` to tokenize the input by whitespace. The stream's `operator>>` automatically skips leading, trailing, and repeated whitespace, and extracts each word until the next whitespace. We repeatedly read words into a `std::string` variable, and after the loop completes, that variable will hold the last word read, because the loop continues until the stream is exhausted. If the string contains at least one word (guaranteed by the problem), the variable is guaranteed to be non-empty after the loop. The length can then be obtained with `std::string::length()`. This approach avoids manually trimming spaces or scanning from the end, though it is slightly less efficient than a reverse scan. Time complexity is O(n) where n is the length of the string, since each character is processed once during tokenization. Space complexity is O(1) auxiliary space for the word string (though the stream buffer may internally allocate), making it suitable for large inputs. Edge cases include strings with only whitespace around a single word, multiple spaces between words, and strings with only one word.
