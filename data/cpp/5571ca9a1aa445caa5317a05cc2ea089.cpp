/*
Write a C++ function named `mostFrequentWord` that accepts a string containing a sentence (words separated by single spaces, but possibly with leading/trailing whitespace) and returns a `std::pair<std::string, int>` containing the word that appears most frequently and its occurrence count. If multiple words tie for the highest frequency, return the one that appears **first** in the sentence (i.e., the earliest occurrence among the tied words). Words are case-sensitive, and punctuation is not present (only letters and spaces). The input string will always contain at least one word. The function must handle empty words gracefully (e.g., multiple spaces should be treated as separators, not as empty words).
*/

#include <string>
#include <map>
#include <sstream>
#include <utility>
#include <limits>

// Return the most frequent word in a whitespace-separated sentence.
// If there is a tie, returns the word that appears first in the sentence.
std::pair<std::string, int> mostFrequentWord(const std::string& sentence) {
    std::istringstream stream(sentence);
    std::string word;
    std::map<std::string, int> frequency;
    std::string bestWord;
    int bestCount = std::numeric_limits<int>::min();

    while (stream >> word) {
        frequency[word]++;
        if (frequency[word] > bestCount) {
            bestCount = frequency[word];
            bestWord = word;
        }
    }
    return {bestWord, bestCount};
}

#include <cassert>
#include <string>
#include <utility>

int main() {
    // Basic case with unique most frequent
    assert(mostFrequentWord("apple banana apple cherry") == std::make_pair(std::string("apple"), 2));

    // Tie: both "a" and "b" appear twice, but "a" comes first
    assert(mostFrequentWord("a b a b") == std::make_pair(std::string("a"), 2));

    // Single word
    assert(mostFrequentWord("hello") == std::make_pair(std::string("hello"), 1));

    // All unique words: first word wins
    assert(mostFrequentWord("cat dog bird") == std::make_pair(std::string("cat"), 1));

    // Leading/trailing and multiple spaces
    assert(mostFrequentWord("  one   two two  ") == std::make_pair(std::string("two"), 2));

    // Case sensitivity: "Hello" and "hello" are different
    assert(mostFrequentWord("Hello hello") == std::make_pair(std::string("Hello"), 1));

    // Three-way tie, earliest word wins
    assert(mostFrequentWord("x y z x y z") == std::make_pair(std::string("x"), 2));

    // Longer sentence with repeated patterns
    assert(mostFrequentWord("aa bb cc aa dd bb aa") == std::make_pair(std::string("aa"), 3));

    // Empty words between spaces are ignored
    assert(mostFrequentWord("  only   ") == std::make_pair(std::string("only"), 1));

    // All same word
    assert(mostFrequentWord("same same same") == std::make_pair(std::string("same"), 3));
}

// The solution uses a `std::map<std::string, int>` to count occurrences of each word. As we parse the sentence using `std::istringstream` (which automatically splits on whitespace and ignores leading/trailing spaces), for each word we increment its count in the map. We maintain a running maximum count and the corresponding word. The key insight is to update both the maximum count and the word **only when** the current word's count exceeds the current maximum (strictly greater). This ensures that if a tie occurs later in the sentence, the earliest word (the first one to reach the maximum count) remains the answer. This works because the first time a word reaches a new high count, it is the earliest occurrence among all words with that count; later words that tie will have `mp[word] == maxCount`, not greater, so they will not overwrite. Edge cases: a single word (returns that word with count 1), all words unique (returns the first word with count 1), and multiple spaces (handled by `istringstream`). Time complexity is O(W log U) where W is the total number of words and U is the number of unique words, due to map operations. Space complexity is O(U) for the map.
