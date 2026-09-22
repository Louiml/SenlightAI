Given a non-empty string containing lowercase English letters and spaces (where words are separated by single spaces and there are no leading or trailing spaces), write a C++ function that returns a new string where each word is reversed while keeping the original word order and the spaces between words unchanged. For example, the input "Let's take LeetCode contest" (note the apostrophe, which would be treated as part of the word) should produce "s'teL ekat edoCteeL tsetnoc". Assume all characters except spaces are part of words, and the input contains at least one word. The function must not modify the input string.
// The task requires reversing each word independently while preserving the order of words and the single spaces between them. The main algorithm involves scanning the input string to identify word boundaries: a word is a contiguous sequence of non-space characters, and spaces are delimiters. For each word, we can reverse it directly in-place within a copy of the input string (since we must not modify the input, we create a mutable copy). We track the start index of the current word. When we encounter a space (or reach the end of the string), we reverse the substring from the start index to the position just before the space (or end). After reversing, we update the start index to the position after the space for the next word. Edge cases to consider: the string may consist of a single word (so no spaces), or multiple words; the final word ends at the end of the string, so we must handle that case separately after the loop. The complexity is O(n) time, where n is the length of the string, because each character is visited once during scanning and once during reversal (total O(n) per word, summing to O(n) overall). The auxiliary space is O(n) for the copy of the string; the reversal itself uses O(1) extra space.
#include <string>
#include <algorithm>

// Reverse each word in the given string while preserving word order and spaces.
std::string reverseWords(const std::string& input) {
    std::string result = input;  // mutable copy
    int n = static_cast<int>(result.size());
    int start = 0;

    for (int i = 0; i <= n; ++i) {
        // If we're at a space or at the end of the string, reverse the current word.
        if (i == n || result[i] == ' ') {
            std::reverse(result.begin() + start, result.begin() + i);
            start = i + 1;  // move to the character after the space
        }
    }

    return result;
}
#include <cassert>
#include <string>

// Assume reverseWords is defined as above.

int main() {
    // Single word
    assert(reverseWords("hello") == "olleh");
    // Multiple words with single spaces
    assert(reverseWords("Let's take LeetCode contest") == "s'teL ekat edoCteeL tsetnoc");
    // Two words
    assert(reverseWords("ab cd") == "ba dc");
    // Words of length 1
    assert(reverseWords("a b c") == "a b c");
    // Longer string with repeated patterns
    assert(reverseWords("code") == "edoc");
    // Sentence with punctuation in words
    assert(reverseWords("Hello, world!") == ",olleH !dlrow");
    // Empty check? Not required but the function would return empty; we skip as task says non-empty.
    // Edge: many spaces? Not allowed per spec, but just in case:
    // assert(reverseWords("a  b") == "a  b"); // Would fail due to double spaces; task says single spaces.
    // Final check with a known example
    assert(reverseWords("abc def ghi") == "cba fed ihg");
    return 0;
}
