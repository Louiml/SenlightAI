// Write a C++ function that takes a non-empty string containing words separated by single spaces (with no leading or trailing spaces) and returns a new string with the order of the words reversed, while preserving the order of letters within each word. For example, the input `"one two three"` should produce `"three two one"`. The function must handle multiple spaces? No—the input is guaranteed to have exactly one space between words, but the string may contain any number of words (including just one). The function should not modify the original string and must be `const` correct. You may assume the input contains only printable ASCII characters and no leading/trailing spaces.

The simplest approach is to split the input string into tokens using a delimiter (single space). We can iterate over the string, collecting substrings between space characters, and store them in a vector. After collecting all words, we build the result by iterating backward through the vector, appending each word followed by a space (except after the last word, which becomes the first element). Alternatively, we could reverse the entire string and then reverse each word individually, but the vector-based approach is more straightforward. Edge cases include a single-word input (returns the same word), an empty string (not allowed per specification), and words with internal punctuation (treated as normal characters). Time complexity is O(n) where n is the total length of the input string (we traverse it once to split and once to build the result). Space complexity is O(n) because we store all words in a vector and build a new string of length roughly equal to the input (plus spaces if original had multiple spaces—but we assume single spaces, so the output length equals the input length).

#include <string>
#include <vector>

// Returns a new string with the words in `input` reversed in order.
// Words are separated by exactly one space, and no leading/trailing spaces.
std::string reverseWords(const std::string& input) {
    std::vector<std::string> words;
    std::string word;
    
    // Split the input into words by space.
    for (char c : input) {
        if (c == ' ') {
            if (!word.empty()) {
                words.push_back(word);
                word.clear();
            }
        } else {
            word += c;
        }
    }
    // The last word (if any) is not followed by a space.
    if (!word.empty()) {
        words.push_back(word);
    }
    
    // Build the reversed string.
    std::string result;
    for (int i = static_cast<int>(words.size()) - 1; i >= 0; --i) {
        result += words[i];
        if (i > 0) {
            result += ' ';
        }
    }
    return result;
}

#include <cassert>
#include <string>

// The solution function is declared above (in the same translation unit).
// For the test, we replicate the function definition or include the header.

int main() {
    assert(reverseWords("hello") == "hello");
    assert(reverseWords("one two three") == "three two one");
    assert(reverseWords("apple banana cherry date") == "date cherry banana apple");
    assert(reverseWords("a b c") == "c b a");
    assert(reverseWords("same same") == "same same");
    assert(reverseWords("word") == "word");
    assert(reverseWords("x y z") == "z y x");
    assert(reverseWords("alpha beta") == "beta alpha");
    assert(reverseWords("test") == "test");
    assert(reverseWords("one") == "one");
    return 0;
}
