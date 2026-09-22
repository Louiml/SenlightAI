Write a C++ function that, given a vector of strings `words` and a character `x`, returns a vector of integers containing the indices (in ascending order) of all words that contain the character `x` at least once. The function should be named `findWordsContaining` and should handle an empty input vector (returning an empty result), single-character strings, and repeated occurrences of `x` within a word (still only one index per word). Assume lowercase English letters for words, but `x` can be any printable ASCII character. The input vector and character should not be modified by the function.

// The solution iterates over each string in the input vector using its index `i`. For each word, use the standard `std::string::find` method to check whether character `x` exists in the current word. If the returned position is not equal to `std::string::npos`, the character is present, so push the current index `i` into the result vector. Since we iterate in order from `0` to `words.size()-1`, the result indices will naturally be in ascending order. Edge cases: an empty vector yields an empty result; a word where `x` appears multiple times still yields only one index because we push only once per word; empty strings (if any) will not contain `x`, so they are skipped. Time complexity is O(n * l), where n is the number of words and l is the average length of a word, because `find` scans each character of each word in the worst case. Space complexity is O(n) for the result vector, plus O(1) auxiliary space beyond the input and output.

#include <string>
#include <vector>

// Return indices of all words in 'words' that contain character 'x'.
// Indices are in ascending order. Empty input yields empty output.
std::vector<int> findWordsContaining(const std::vector<std::string>& words, char x) {
    std::vector<int> result;
    for (int i = 0; i < static_cast<int>(words.size()); ++i) {
        if (words[i].find(x) != std::string::npos) {
            result.push_back(i);
        }
    }
    return result;
}

#include <cassert>
#include <vector>
#include <string>

int main() {
    // Basic case with multiple matches
    std::vector<std::string> words1 = {"apple", "banana", "cherry", "date"};
    assert(findWordsContaining(words1, 'a') == std::vector<int>({0, 1, 3}));

    // No matches
    std::vector<std::string> words2 = {"dog", "cat", "bird"};
    assert(findWordsContaining(words2, 'z') == std::vector<int>({}));

    // All words match
    std::vector<std::string> words3 = {"ab", "ba", "aa"};
    assert(findWordsContaining(words3, 'a') == std::vector<int>({0, 1, 2}));

    // Empty vector
    std::vector<std::string> words4;
    assert(findWordsContaining(words4, 'x') == std::vector<int>({}));

    // Character appears multiple times in one word — only one index
    std::vector<std::string> words5 = {"book", "look", "cook"};
    assert(findWordsContaining(words5, 'o') == std::vector<int>({0, 1, 2}));

    // Single word, no match
    std::vector<std::string> words6 = {"hello"};
    assert(findWordsContaining(words6, 'z') == std::vector<int>({}));

    // Single word, match
    std::vector<std::string> words7 = {"hello"};
    assert(findWordsContaining(words7, 'h') == std::vector<int>({0}));

    // Mixed with empty string (if allowed)
    std::vector<std::string> words8 = {"", " ", "a"};
    assert(findWordsContaining(words8, ' ') == std::vector<int>({1}));

    // Check that original vector is not modified
    std::vector<std::string> original = {"test", "example"};
    std::vector<std::string> copy = original;
    findWordsContaining(original, 'e');
    assert(original == copy);
}
