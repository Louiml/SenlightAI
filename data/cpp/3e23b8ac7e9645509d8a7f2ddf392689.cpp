/*
Write a C++ function named `topKFrequentWords` that takes a vector of strings (words) and an integer `k` as input, and returns a vector containing the `k` most frequent words in descending order of frequency. If two words have the same frequency, they must appear in lexicographical (alphabetical) order. The input vector may contain duplicate words, and `k` is guaranteed to be between 1 and the number of unique words (inclusive). The function should handle case-sensitive comparisons (i.e., "Apple" and "apple" are different) and should not modify the input vector.
*/

#include <vector>
#include <string>
#include <map>
#include <queue>
#include <utility>

// Return the k most frequent words, sorted by frequency descending, then lexicographically for ties.
std::vector<std::string> topKFrequentWords(const std::vector<std::string>& words, int k) {
    std::map<std::string, int> frequency;
    for (const auto& word : words) {
        ++frequency[word];
    }

    // Max-heap by frequency (integer) — ties are broken arbitrarily by the heap.
    std::priority_queue<std::pair<int, std::string>> heap;
    for (const auto& entry : frequency) {
        heap.push({entry.second, entry.first});
    }

    // Extract all entries in frequency-descending order.
    std::vector<std::string> all_ordered;
    all_ordered.reserve(frequency.size());
    while (!heap.empty()) {
        all_ordered.push_back(heap.top().second);
        heap.pop();
    }

    // Fix ordering for equal frequencies by sorting lexicographically within same-frequency groups.
    // Use a simple bubble sort over adjacent equal-frequency entries.
    int n = static_cast<int>(all_ordered.size());
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n - 1; ++j) {
            if (frequency[all_ordered[j]] == frequency[all_ordered[j + 1]] &&
                all_ordered[j] > all_ordered[j + 1]) {
                std::swap(all_ordered[j], all_ordered[j + 1]);
            }
        }
    }

    // Return the first k elements.
    return std::vector<std::string>(all_ordered.begin(), all_ordered.begin() + k);
}

#include <cassert>
#include <vector>
#include <string>

int main() {
    // Basic case with distinct frequencies
    std::vector<std::string> words1 = {"i", "love", "leetcode", "i", "love", "coding"};
    assert(topKFrequentWords(words1, 2) == std::vector<std::string>({"i", "love"}));

    // Tie in frequency — lexicographic order
    std::vector<std::string> words2 = {"the", "day", "is", "sunny", "the", "the", "the", "sunny", "is", "is"};
    assert(topKFrequentWords(words2, 4) == std::vector<std::string>({"the", "is", "sunny", "day"}));

    // All same frequency
    std::vector<std::string> words3 = {"a", "b", "c", "a", "b", "c"};
    assert(topKFrequentWords(words3, 3) == std::vector<std::string>({"a", "b", "c"}));

    // Only one unique word
    std::vector<std::string> words4 = {"apple", "apple", "banana", "banana", "banana"};
    assert(topKFrequentWords(words4, 1) == std::vector<std::string>({"banana"}));

    // k equals number of unique words
    std::vector<std::string> words5 = {"x", "y", "z"};
    assert(topKFrequentWords(words5, 3) == std::vector<std::string>({"x", "y", "z"}));

    // Case-sensitive — "A" and "a" are different
    std::vector<std::string> words6 = {"A", "a", "A", "a", "A"};
    assert(topKFrequentWords(words6, 2) == std::vector<std::string>({"A", "a"}));

    // Larger test with tie across multiple words
    std::vector<std::string> words7 = {"a", "b", "c", "d", "a", "b", "c", "d", "e", "e", "e"};
    // frequencies: a:2, b:2, c:2, d:2, e:3 → top 3: e, then a, b (lex order among ties)
    assert(topKFrequentWords(words7, 3) == std::vector<std::string>({"e", "a", "b"}));

    return 0;
}

// The solution counts the frequency of each word using a `std::map<string, int>`, which automatically sorts keys lexicographically. Then, we push all pairs `(frequency, word)` into a `std::priority_queue<pair<int, string>>`; by default, this is a max-heap, so it pops the highest frequency first. Because the map iterates in lexicographic order of words, for equal frequencies we rely on that order; however, the priority queue does not maintain lexicographic ordering for ties directly. A simple approach is to pop all entries into a result vector (ordered by frequency descending, but ties may be in arbitrary order). To fix tie ordering, we perform a bubble-sort pass over the result vector: whenever two adjacent elements have equal frequency and the earlier word is lexicographically greater than the later word, swap them. After that, the first `k` elements of the sorted vector are returned. Complexity: counting frequencies takes O(n log u) where n is total words and u is unique words (due to map). The heap operations take O(u log u). The bubble-sort pass is O(u^2) in the worst case, but u ≤ n, so overall time is O(n log n + n^2) worst-case—acceptable for typical constraints. Space complexity is O(u) for the map and heap. Edge cases: duplicate words, all words same frequency, `k` equals number of unique words, and empty input (though constraints guarantee at least one word).
