// Write a C++ function named `reverseWordOrder` that takes a C-style string (null-terminated array of `char`) as input and returns a `std::string` containing the same words from the input but in reverse order, separated by exactly one space, with no leading or trailing spaces. A "word" is a maximal contiguous sequence of non-space characters. The input may contain multiple consecutive spaces, leading/trailing spaces, or be empty. For example, given `"  hello   world  "`, the function must return `"world hello"`. The function must not modify the input (use `const char*`).

// The core task is to tokenize the input string by spaces and reconstruct the output by prepending each word to the result. The simplest approach is to iterate through the input character by character, identifying word boundaries. When a non-space character is found, extract the whole word (until the next space or end-of-string), then insert this word at the beginning of the result string. To do this efficiently, we can build the result by adding each new word in front of the current result, with a space separator if the result is not empty. Alternatively, collect all words into a `std::vector` and then reverse with a loop. Key edge cases: empty input → return empty string; only spaces → return empty string; single word → return that word; multiple spaces between words → ignore them. Time complexity is `O(n)` where `n` is the string length, because we scan the input once and build the output by prepending (which in C++ `std::string` might be `O(n)` per operation, but overall still `O(n^2)` in worst case if prepending many times; to be safe, we can collect and then reverse, giving `O(n)`). Space complexity is `O(n)` for the output and temporary storage. The function uses `const` on the input pointer to guarantee no modification.

#include <string>

/*
 * Reverse the order of words in a C-string.
 * Words are sequences of non-space characters; multiple spaces are ignored.
 * Returns a std::string with words reversed and single-space separated.
 */
std::string reverseWordOrder(const char* input) {
    std::string result;
    std::string currentWord;
    
    for (int i = 0; input[i] != '\0'; ++i) {
        if (input[i] != ' ') {
            currentWord += input[i];
        } else {
            if (!currentWord.empty()) {
                if (!result.empty()) {
                    result = currentWord + " " + result;
                } else {
                    result = currentWord;
                }
                currentWord.clear();
            }
        }
    }
    // Handle last word (if any)
    if (!currentWord.empty()) {
        if (!result.empty()) {
            result = currentWord + " " + result;
        } else {
            result = currentWord;
        }
    }
    return result;
}

#include <cassert>
#include <string>

// Assume reverseWordOrder is declared above.

int main() {
    // Basic reversal
    assert(reverseWordOrder("hello world") == "world hello");
    // Multiple spaces and leading/trailing
    assert(reverseWordOrder("  a   b  c   ") == "c b a");
    // Single word
    assert(reverseWordOrder("single") == "single");
    // Empty string
    assert(reverseWordOrder("") == "");
    // Only spaces
    assert(reverseWordOrder("      ") == "");
    // Words with numbers and punctuation
    assert(reverseWordOrder("one 2 three!") == "three! 2 one");
    // Long sentence
    assert(reverseWordOrder("the quick brown fox") == "fox brown quick the");
    // Single space separator
    assert(reverseWordOrder("a b c") == "c b a");
    // Consecutive spaces in middle
    assert(reverseWordOrder("  x    y   ") == "y x");
    // Already reversed
    assert(reverseWordOrder("z y x") == "x y z");
    return 0;
}
