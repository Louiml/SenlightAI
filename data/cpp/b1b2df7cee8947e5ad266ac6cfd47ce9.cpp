// Write a C++ function named `analyzeWords` that reads a sequence of whitespace-separated words from a `std::istream` (e.g., `std::cin`) and returns a `std::pair<std::string, std::string>` where the first element is the most frequently occurring word (in case of a tie, choose the one that appears first in the input stream), and the second element is the longest word (in case of a tie, choose the lexicographically smallest string). The input may contain punctuation-free lowercase or uppercase letters only, no leading/trailing whitespace issues, and at least one word is guaranteed. The function must not print anything; it must return the pair of strings with each word exactly as read (case-sensitive, so "Hello" and "hello" are distinct). The primary objective is to implement a robust solution using a `std::map<std::string, int>` to count frequencies and track the longest word, while correctly handling ties according to the specified rules. The function should be `const`-correct (no mutation of the input stream, but reading from it is fine), and must work for any number of words up to memory limits.
The solution uses a `std::map<std::string, int>` to store each unique word and its frequency. We read words one by one from the input stream using `>>` which naturally skips whitespace. For each new word, we insert or increment its count in the map. To handle the "most frequent word" tie-breaking rule (first occurrence in input stream), we need to track the order of first appearances. Since `std::map` iterates in lexicographical order (not insertion order), we maintain two separate variables: `longest` (the longest word seen so far, updated only when a strictly longer word appears, and for equal lengths we choose lexicographically smaller, but careful: the spec says tie for longest chooses lexicographically smallest; but also for frequency tie chooses first occurrence, not lexicographic). Because the map does not preserve insertion order, for frequency tie-breaking we track the first-occurrence order separately: either by storing a vector of unique words in order they were first seen, or by keeping a separate variable that is updated only if the current frequency exceeds the previous maximum (strictly greater), and never update on equal frequency. This ensures the earliest-seen word that achieves the current maximum frequency remains selected. For the longest word, we update when `word.length() > longest.length()` OR when `word.length() == longest.length() && word < longest` to satisfy lexicographic smallest among ties. However, the given code snippet does not handle the tie for longest (it just picks first in map iteration which is lexicographically smallest, but that's not guaranteed correct if a longer word appears later; actually it updates on longer, so ties keep the earlier one? The snippet uses `longest.length() < it->first.length()` so it only updates on strictly longer, which would keep the first encountered in map iteration order, which is lexicographic. That's fine for tie-breaking lexicographically smallest because map iterates in sorted order). So we can use the map's iteration order for longest tie-breaking by only updating on strictly longer. For frequency, the snippet's approach uses `if (table[freqest] < it->second)` which only updates on strictly greater frequency, so it keeps the first in map iteration order, which is lexicographically smallest, not first-occurrence. To meet the task's explicit tie rule (first occurrence), we must modify that. So the algorithm: read words using a `std::string` variable. For each word, increment `table[word]`. Also maintain an ordered vector of unique words as they first appear. After reading all words, iterate over that vector (or over map but use the vector to determine first occurrence) to find the word with maximum frequency, and if frequencies are equal, the one that appears earlier in the vector. For the longest, we can iterate over the map (which gives lexicographic order) and keep the one with maximum length, updating on strictly longer (so tie keeps lexicographically smallest because map order is sorted). Alternatively, we can do both in a single pass. Time complexity: O(N log U) where N is total words and U is unique words due to map operations, plus O(U) final scans. Space: O(U) for map and vector. Edge cases: empty input? Task guarantees at least one word. Case sensitivity handled by storing exact strings. For all identical words, both outputs are that word.
#include <map>
#include <string>
#include <vector>
#include <istream>
#include <utility>

// Analyze a stream of whitespace-separated words.
// Returns {most frequent word (first occurrence wins ties), longest word (lexicographically smallest on ties)}.
std::pair<std::string, std::string> analyzeWords(std::istream& input) {
    std::map<std::string, int> frequency;
    std::vector<std::string> order;  // unique words in first-occurrence order

    std::string word;
    while (input >> word) {
        if (frequency.find(word) == frequency.end()) {
            order.push_back(word);  // first time seeing this word
        }
        ++frequency[word];
    }

    // Find most frequent word with first-occurrence tie-breaking
    std::string mostFrequent = order.empty() ? "" : order[0];
    int maxFreq = order.empty() ? 0 : frequency[order[0]];
    for (size_t i = 1; i < order.size(); ++i) {
        const std::string& current = order[i];
        if (frequency[current] > maxFreq) {
            maxFreq = frequency[current];
            mostFrequent = current;
        }
        // On equal frequency, do nothing to keep earlier first occurrence
    }

    // Find longest word with lexicographically smallest tie-breaking
    // Using map iteration gives lexicographic order, so only update on strictly longer.
    std::string longest = frequency.begin()->first;
    for (const auto& entry : frequency) {
        if (entry.first.length() > longest.length()) {
            longest = entry.first;
        }
        // On equal length, keep earlier in map order (lexicographically smaller)
    }

    return {mostFrequent, longest};
}
#include <iostream>
#include <sstream>
#include <cassert>

// The solution function is declared here (include the header or copy above)
std::pair<std::string, std::string> analyzeWords(std::istream& input);

int main() {
    // Test 1: simple case
    {
        std::istringstream ss("apple banana apple cherry");
        assert(analyzeWords(ss) == std::make_pair(std::string("apple"), std::string("banana")));
    }
    // Test 2: tie frequency chooses first occurrence
    {
        std::istringstream ss("dog cat bird dog cat");
        // "dog" and "cat" both appear twice, "dog" appears first
        assert(analyzeWords(ss) == std::make_pair(std::string("dog"), std::string("bird")));
    }
    // Test 3: longest tie chooses lexicographically smallest
    {
        std::istringstream ss("apple banana berry");
        // "banana" and "berry" both length 5, "banana" < "berry"
        assert(analyzeWords(ss) == std::make_pair(std::string("apple"), std::string("banana")));
    }
    // Test 4: single word
    {
        std::istringstream ss("hello");
        assert(analyzeWords(ss) == std::make_pair(std::string("hello"), std::string("hello")));
    }
    // Test 5: all same word
    {
        std::istringstream ss("a a a");
        assert(analyzeWords(ss) == std::make_pair(std::string("a"), std::string("a")));
    }
    // Test 6: case sensitive
    {
        std::istringstream ss("Hi hi HI");
        // "Hi" and "hi" and "HI" each once; first occurrence "Hi" is most frequent
        // longest is "HI" (length 2) vs "Hi" (2) vs "hi" (2) -> lexicographically smallest is "HI"
        assert(analyzeWords(ss) == std::make_pair(std::string("Hi"), std::string("HI")));
    }
    // Test 7: longer word appears later
    {
        std::istringstream ss("short shorter shortest");
        // frequency all once -> first "short"
        // longest is "shortest"
        assert(analyzeWords(ss) == std::make_pair(std::string("short"), std::string("shortest")));
    }
    // Test 8: multiple spaces and newlines
    {
        std::istringstream ss("  one two\n two\tthree  one ");
        // "one" twice, "two" twice, "three" once -> tie "one" first occurrence
        // longest "three" (5) vs "two" (3) vs "one" (3)
        assert(analyzeWords(ss) == std::make_pair(std::string("one"), std::string("three")));
    }
    std::cout << "All tests passed!\n";
    return 0;
}
