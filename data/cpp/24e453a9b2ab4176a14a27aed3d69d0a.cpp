/*
Write a C++ function that takes a vector of strings `words` and a string `s`, and returns the count of strings in `words` that are prefixes of `s`. A string `w` is a prefix of `s` if `s` starts with exactly `w` (i.e., `w` is a prefix, not necessarily equal, but the first `w.size()` characters of `s` match `w`). Empty strings in `words` should be counted as prefixes (since the empty string is a prefix of any string). The input strings may contain any characters including spaces, digits, and punctuation. The function should be const-correct and handle cases where `words` is empty.
*/

#include <string>
#include <vector>

// Count how many strings in `words` are prefixes of the string `s`.
// A string w is a prefix of s if s starts with w (including empty string).
int countPrefixWords(const std::vector<std::string>& words, const std::string& s) {
    int count = 0;
    for (const std::string& w : words) {
        // Check if s starts with w using compare: if equal, returns 0.
        if (s.compare(0, w.size(), w) == 0) {
            ++count;
        }
    }
    return count;
}

#include <cassert>
#include <vector>
#include <string>

// Assume countPrefixWords is defined above.

int main() {
    std::vector<std::string> words1 = {"a", "ab", "abc", "b", "cab"};
    assert(countPrefixWords(words1, "abc") == 3); // "a", "ab", "abc" are prefixes

    std::vector<std::string> words2 = {};
    assert(countPrefixWords(words2, "anything") == 0);

    std::vector<std::string> words3 = {"", "a", "", "b"};
    assert(countPrefixWords(words3, "apple") == 3); // empty strings count, "a" counts, "b" does not

    std::vector<std::string> words4 = {"abc", "abcd", "ab", "bc"};
    assert(countPrefixWords(words4, "abcd") == 3); // "abc", "abcd", "ab" are prefixes; "bc" is not

    std::vector<std::string> words5 = {"hello world", "hello", "world", "hello worl"};
    assert(countPrefixWords(words5, "hello world!") == 3); // "hello world", "hello", "hello worl"

    std::vector<std::string> words6 = {"a", "ab", "abc", "abcde"};
    assert(countPrefixWords(words6, "abc") == 3); // "a", "ab", "abc" are prefixes; "abcde" is longer than s

    std::vector<std::string> words7 = {"x", "y"};
    assert(countPrefixWords(words7, "z") == 0);

    std::vector<std::string> words8 = {"", ""};
    assert(countPrefixWords(words8, "") == 2);

    std::vector<std::string> words9 = {"a", "a", "a"};
    assert(countPrefixWords(words9, "a") == 3);

    std::vector<std::string> words10 = {" "};
    assert(countPrefixWords(words10, " ") == 1); // single space is a prefix of a string starting with a space

    return 0;
}

// The straightforward solution is to iterate over each word in the vector and check if it is a prefix of `s`. The most direct check is to use the standard string member function `std::string::compare` or `std::string::find`. A simple approach is to use `s.find(word) == 0` because `find` returns the position of the first occurrence of `word` in `s`; if it is at position 0, then `word` is a prefix. However, using `find` may scan the whole string if the word is not at the start, but given the problem constraints (typical coding interview), it's acceptable and simple. Alternatively, use `s.rfind(word, 0) == 0` which is more efficient because it looks for the word only at the beginning (searching backwards from position 0). Or use `s.compare(0, word.size(), word) == 0`, which is the most explicit and safe. Edge cases: if `word` is empty, then `s.compare(0,0,"")` returns 0, so it counts. If `word` is longer than `s`, `compare` will return a non-zero value (since it will compare beyond the end), so it won't count. Time complexity is O(n * L) where n is the number of words and L is the length of the longest word (or length of `s` for `find`), and space complexity is O(1) auxiliary (not counting the input storage).
