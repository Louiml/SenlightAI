Write a C++ function `bool wordPatternMatch(const std::string& pattern, const std::string& s)` that determines whether a given `pattern` (consisting only of lowercase English letters) and a string `s` (consisting of lowercase words separated by single spaces) follow the same bijective mapping: each letter in the pattern corresponds to exactly one word in `s`, and each word in `s` corresponds to exactly one letter in the pattern. The strings are non‑empty, the pattern has no spaces, and `s` has no leading/trailing spaces and contains only single spaces between words. Return `true` if the mapping is consistent, otherwise `false`. Your function must handle arbitrary pattern lengths and word counts, and must not rely on any external libraries beyond the standard C++ library.

The solution compares the pattern and the word sequence by replacing each distinct symbol (letter or word) with an integer ID that represents its first occurrence index. For the pattern, iterate over each character; if a character is new, assign it the next sequential integer. Build a string `patternIDs` of these IDs. For the words, split `s` by spaces; for each word, similarly assign an ID on first encounter and append to `wordIDs`. Finally, the pattern matches if and only if `patternIDs == wordIDs`. Edge cases: if the number of letters in the pattern differs from the number of words, the function must return `false` immediately. Another edge case is when a letter or word appears again but is mapped to a different counterpart—this is automatically caught because the ID comparison will fail. The algorithm runs in O(n + m) time, where n is pattern length and m is total characters in `s`, and uses O(n + m) space for the maps and ID strings.

#include <string>
#include <unordered_map>
#include <sstream>

// Check whether 'pattern' and 's' follow the same bijective word-letter mapping.
bool wordPatternMatch(const std::string& pattern, const std::string& s) {
    // Build ID sequence for pattern
    std::unordered_map<char, int> letterToId;
    std::string patternIDs;
    int nextId = 0;
    for (char ch : pattern) {
        auto it = letterToId.find(ch);
        if (it == letterToId.end()) {
            letterToId[ch] = nextId++;
        }
        patternIDs += std::to_string(letterToId[ch]);
    }

    // Split s into words and build ID sequence
    std::unordered_map<std::string, int> wordToId;
    std::string wordIDs;
    nextId = 0;
    std::istringstream stream(s);
    std::string word;
    while (stream >> word) {
        auto it = wordToId.find(word);
        if (it == wordToId.end()) {
            wordToId[word] = nextId++;
        }
        wordIDs += std::to_string(wordToId[word]);
    }

    // If lengths differ, mapping is impossible
    if (patternIDs.length() != wordIDs.length()) {
        return false;
    }
    return patternIDs == wordIDs;
}

#include <cassert>
#include <string>

// Function declaration from the solution (assume it is included above)
bool wordPatternMatch(const std::string& pattern, const std::string& s);

int main() {
    // Basic matching cases
    assert(wordPatternMatch("abba", "dog cat cat dog") == true);
    assert(wordPatternMatch("abba", "dog cat cat fish") == false);
    assert(wordPatternMatch("aaaa", "dog cat cat dog") == false);
    assert(wordPatternMatch("aaaa", "dog dog dog dog") == true);

    // Single-letter pattern
    assert(wordPatternMatch("a", "dog") == true);
    assert(wordPatternMatch("a", "dog cat") == false);

    // Repeated pattern letters with different words
    assert(wordPatternMatch("aba", "dog cat dog") == true);
    assert(wordPatternMatch("aba", "dog cat cat") == false);

    // Same word used for different pattern letters
    assert(wordPatternMatch("ab", "dog dog") == false);
    assert(wordPatternMatch("abc", "dog cat fish") == true);

    // Pattern longer than words and vice versa
    assert(wordPatternMatch("abc", "dog cat") == false);
    assert(wordPatternMatch("ab", "dog cat bird") == false);

    // Non-trivial mapping with multiple distinct letters
    assert(wordPatternMatch("xyzxyz", "a b c a b c") == true);
    assert(wordPatternMatch("xyzxyz", "a b c a b d") == false);
}
