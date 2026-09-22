// Write a C++ function `longestWord` that takes a single string `s` (which may contain spaces, tabs, and punctuation) and returns the lexicographically **largest** word from the string, where words are defined as maximal sequences of non-space characters. If the input string is empty or contains only whitespace, return an empty string. The comparison between words must be case‑sensitive and based on standard ASCII ordering (e.g., `"Z"` is greater than `"a"`). The function must handle multiple consecutive spaces, leading/trailing spaces, and tabs as separators. Do **not** modify the input string.
The task is a direct extension of the given snippet: iterate through the string, accumulate characters into a current word until a whitespace character (space `' '`, tab `'\t'`, newline `'\n'`, carriage return `'\r'`, etc.) is encountered. On each whitespace, compare the accumulated word with the current best using `std::max` (which uses `operator<` for `std::string`, i.e., lexicographic order) and reset the word. After the loop, compare the last word (since the string may not end with whitespace). Edge cases: (1) empty or all‑whitespace input → return empty string; (2) punctuation like commas or periods are part of the word, not separators; (3) tabs and other non‑space whitespace must be recognized via `std::isspace` (cast `unsigned char` to avoid undefined behavior); (4) multiple consecutive whitespace characters are handled by simply resetting the word repeatedly. Time complexity is O(n) where n is the length of the string, because each character is visited once, and `std::max` compares strings of total length O(n) overall in the worst case (since each character is compared at most once per pair of words, but the total work is bounded by O(n) amortized). Space complexity is O(m) for the longest word, with m ≤ n.
#include <string>
#include <cctype>
#include <algorithm>

// Return the lexicographically largest word in a whitespace-separated string.
// Words are maximal sequences of non-whitespace characters. Empty input yields empty string.
std::string longestWord(const std::string& s) {
    std::string currentWord;
    std::string bestWord;

    for (char ch : s) {
        // Use unsigned char to avoid issues with negative char values in std::isspace.
        if (std::isspace(static_cast<unsigned char>(ch))) {
            // End of a word: update best if current is larger.
            bestWord = std::max(bestWord, currentWord);
            currentWord.clear();
        } else {
            // Append non-whitespace character to current word.
            currentWord += ch;
        }
    }

    // Handle the last word if no trailing whitespace.
    bestWord = std::max(bestWord, currentWord);
    return bestWord;
}
#include <cassert>
#include <string>

// Forward declaration of the function under test.
std::string longestWord(const std::string& s);

int main() {
    // Basic case with spaces.
    assert(longestWord("apple banana cherry") == "cherry");

    // Lexicographic order is case-sensitive.
    assert(longestWord("apple Banana") == "apple");  // "apple" > "Banana" because 'a' (97) > 'B' (66)

    // Leading, trailing, and multiple consecutive spaces.
    assert(longestWord("   one  two three   ") == "two");

    // Tab as separator.
    assert(longestWord("hello\tworld") == "world");  // "world" > "hello"

    // Empty string and only whitespace.
    assert(longestWord("") == "");
    assert(longestWord("   \t \n  ") == "");

    // Single word.
    assert(longestWord("solitary") == "solitary");

    // Words with punctuation.
    assert(longestWord("end, start. middle") == "start.");  // '.' (46) > ',' (44)
}
