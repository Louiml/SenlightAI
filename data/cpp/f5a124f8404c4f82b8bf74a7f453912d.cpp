Write a C++ function named `reverseWordsOrder` that takes a single line of text as a `std::string` and returns a new string with the order of words reversed, preserving the exact words (including case and punctuation) but removing all extra whitespace between them. Words are defined as any contiguous sequence of non-whitespace characters. The input may contain leading, trailing, or multiple consecutive spaces, but the output must have exactly one space between words and no leading or trailing spaces. If the input contains only whitespace, the function returns an empty string. The function should not modify the original string.
// The core idea is to tokenize the input string by whitespace, store the tokens in a LIFO (stack) structure, and then reconstruct the output by popping tokens from the stack. This reverses the order of words. Use `std::istringstream` to split the input by whitespace automatically (it skips any amount of whitespace between tokens). Push each token onto a `std::stack<std::string>`. After processing all tokens, pop from the stack and append each token to the result, adding a single space between consecutive tokens. Important edge cases: empty input or input with only spaces → the stack is empty and the function returns an empty string. Duplicate words, punctuation, and uppercase/lowercase characters are preserved exactly as they appear. Time complexity is O(n) where n is the total length of the input string, because each character is processed once during tokenization and once during output construction. Space complexity is O(n) because we store all tokens in the stack and then build a result string of similar size.
#include <string>
#include <sstream>
#include <stack>

// Reverse the order of whitespace-separated words in a string.
// Returns a new string with words in reverse order, separated by single spaces.
// Returns an empty string if the input contains no words.
std::string reverseWordsOrder(const std::string& input) {
    std::istringstream iss(input);
    std::stack<std::string> words;
    std::string word;

    // Extract each whitespace-delimited token
    while (iss >> word) {
        words.push(word);
    }

    // Build the result by popping from the stack
    std::string result;
    while (!words.empty()) {
        if (!result.empty()) {
            result += " ";
        }
        result += words.top();
        words.pop();
    }

    return result;
}
#include <cassert>
#include <string>

// Include the solution function here or link it

int main() {
    assert(reverseWordsOrder("hello world") == "world hello");
    assert(reverseWordsOrder("  one   two  three  ") == "three two one");
    assert(reverseWordsOrder("single") == "single");
    assert(reverseWordsOrder("") == "");
    assert(reverseWordsOrder("   ") == "");
    assert(reverseWordsOrder("a b c d") == "d c b a");
    assert(reverseWordsOrder("C++ is fun") == "fun is C++");
    assert(reverseWordsOrder("multiple   spaces   here") == "here spaces multiple");
    assert(reverseWordsOrder("leading and trailing   ") == "trailing and leading");
    assert(reverseWordsOrder("punctuation, stays, intact!") == "intact! stays, punctuation,");
    return 0;
}
