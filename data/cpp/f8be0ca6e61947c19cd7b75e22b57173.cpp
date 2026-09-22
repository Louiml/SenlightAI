// Write a standalone C++ function `splitAndSortWords(const std::string& input, char delimiter)` that takes a string containing words separated by a single delimiter character (which may be a space, comma, tab, etc., but will never appear inside a word), and returns a new string containing the same words in lexicographically ascending order, separated by the same delimiter character. The input may have leading/trailing delimiters and consecutive delimiters; those should be ignored (i.e., empty tokens between delimiters are not considered words). The function should handle empty input (return an empty string) and input with no delimiters (return the same word unchanged). Words are case-sensitive, and the comparison must be based on the standard lexicographical ordering of their characters (i.e., using `std::string` comparison operators). Your function must not modify the input string and must use `const` correctness appropriately.

// The solution approach involves tokenizing the input string by the given delimiter. We iterate through the string, accumulating characters into a current word. When we hit the delimiter or reach the end of the string, we check if the accumulated word is non-empty; if so, we push it into a `std::vector<std::string>`. We must skip empty tokens that arise from leading, trailing, or consecutive delimiters—this is handled naturally by only adding a word when the current buffer is non-empty and the delimiter (or end) is encountered. After tokenization, we sort the vector using `std::sort` with the default string comparison (which is lexicographical and case-sensitive). Finally, we join the sorted words back into a single string, using the delimiter character as the separator, and return it. Edge cases include: empty input (return empty string), input with only delimiters (return empty string), single word with no delimiter (tokenized as one word and returned unchanged), and words that are identical—sorting preserves duplicates, so they will appear multiple times in the output. Time complexity is O(n + m log m), where n is the length of the input and m is the number of distinct tokens; space complexity is O(n) for storing the tokens and the result string.

#include <string>
#include <vector>
#include <algorithm>

// Splits a string by a delimiter, ignores empty tokens, sorts words lexicographically,
// and rejoins them with the same delimiter.
std::string splitAndSortWords(const std::string& input, char delimiter) {
    std::vector<std::string> words;
    std::string currentWord;

    // Tokenize the input
    for (char ch : input) {
        if (ch == delimiter) {
            if (!currentWord.empty()) {
                words.push_back(currentWord);
                currentWord.clear();
            }
        } else {
            currentWord += ch;
        }
    }
    // Handle last token (if any)
    if (!currentWord.empty()) {
        words.push_back(currentWord);
    }

    // Sort words lexicographically (case-sensitive, standard string ordering)
    std::sort(words.begin(), words.end());

    // Join sorted words with delimiter
    std::string result;
    for (size_t i = 0; i < words.size(); ++i) {
        if (i > 0) {
            result += delimiter;
        }
        result += words[i];
    }
    return result;
}

#include <cassert>
#include <string>

// Function under test is declared above.
// Add a main function for testing.

int main() {
    // Basic space-delimited words
    assert(splitAndSortWords("banana apple cherry", ' ') == "apple banana cherry");

    // Comma delimiter, no spaces
    assert(splitAndSortWords("dog,cat,bird", ',') == "bird,cat,dog");

    // Leading/trailing and consecutive delimiters
    assert(splitAndSortWords("  hello   world  again ", ' ') == "again hello world");

    // Single word, no delimiter
    assert(splitAndSortWords("solo", ',') == "solo");

    // Empty input
    assert(splitAndSortWords("", ',') == "");

    // Only delimiters
    assert(splitAndSortWords(",,,", ',') == "");

    // Case-sensitive sorting (uppercase letters come before lowercase)
    assert(splitAndSortWords("banana Apple cherry", ' ') == "Apple banana cherry");

    // Duplicate words
    assert(splitAndSortWords("z a z b a", ' ') == "a a b z z");

    // Tab delimiter
    assert(splitAndSortWords("x\ty\tz", '\t') == "x\ty\tz");

    // Mixed case and digits
    assert(splitAndSortWords("10 2 1", ' ') == "1 10 2");

    // Numeric strings lexicographically, not numerically
    assert(splitAndSortWords("9 10 8", ' ') == "10 8 9");

    return 0;
}
