Write a C++ function `frequencySort(const std::vector<int>& sequence, int maxValue)` that takes a non-empty sequence of integers (each between 1 and `maxValue`) and returns a new vector containing all elements of the sequence sorted by frequency of occurrence in **descending** order. When two or more numbers have the same frequency, they must appear in the order in which they **first appeared** in the original sequence. The function must not modify the input vector, and must handle duplicate values correctly. For example, given `{2, 1, 2, 3, 1, 2}`, the frequencies are: 2 appears 3 times, 1 appears 2 times, 3 appears 1 time. Since 2 has the highest frequency, it comes first (repeated 3 times), then 1 (since 1 and 3 have different frequencies, order based on frequency), and finally 3. If two numbers had the same frequency, say `{1, 2, 1, 2}`, both have frequency 2, but 1 appears first, so output `{1, 1, 2, 2}`.
// The problem requires stable sorting by frequency (descending) and then by first occurrence (ascending index). The core idea is to first count the frequency of each number using a hash map (`std::unordered_map` or `std::map`). Simultaneously, record the first occurrence index of each number. Then, build a vector of pairs `(frequency, value)` and sort it using a custom comparator: if frequencies differ, the higher frequency comes first; if frequencies are equal, the one with the smaller first-occurrence index comes first. Since the original sequence indices are not global positions but we need the relative ordering, we store the first occurrence index (0-based) when the number is first seen. After sorting, iterate through the sorted vector and append each value `frequency` times to the result. Edge cases: an empty sequence (though the task says non-empty, handle gracefully by returning an empty vector); numbers with frequency 0 should not appear; duplicate values handled by counting; stable order preserved via the comparator. Time complexity is O(N + K log K) where N is the length of the sequence and K is the number of distinct values, since we iterate once to count and record first occurrence (O(N)), then sort K pairs (O(K log K)), then output N elements again (O(N)). Space complexity is O(K) for the maps and O(N) for the output vector.
#include <vector>
#include <unordered_map>
#include <algorithm>

// Sort a sequence by frequency (descending), then by first occurrence (ascending).
std::vector<int> frequencySort(const std::vector<int>& sequence, int maxValue) {
    // Edge case: empty input sequence.
    if (sequence.empty()) {
        return {};
    }

    // Count frequencies and record first occurrence index.
    std::unordered_map<int, int> freq;
    std::unordered_map<int, int> firstIndex; // store 0-based index
    for (size_t i = 0; i < sequence.size(); ++i) {
        int value = sequence[i];
        if (freq.find(value) == freq.end()) {
            firstIndex[value] = static_cast<int>(i);
        }
        ++freq[value];
    }

    // Build vector of pairs {frequency, value}.
    std::vector<std::pair<int, int>> frequencyValuePairs;
    for (const auto& entry : freq) {
        frequencyValuePairs.emplace_back(entry.second, entry.first);
    }

    // Custom comparator: higher frequency first; if equal, earlier first occurrence first.
    auto comparator = [&firstIndex](const std::pair<int, int>& a, const std::pair<int, int>& b) {
        if (a.first != b.first) {
            return a.first > b.first; // descending frequency
        }
        return firstIndex[a.second] < firstIndex[b.second]; // ascending first occurrence
    };

    std::sort(frequencyValuePairs.begin(), frequencyValuePairs.end(), comparator);

    // Build the result by expanding values according to frequency.
    std::vector<int> result;
    for (const auto& pair : frequencyValuePairs) {
        int value = pair.second;
        int count = pair.first;
        for (int i = 0; i < count; ++i) {
            result.push_back(value);
        }
    }
    return result;
}
#include <cassert>
#include <vector>

// Assume the solution function is included above.

int main() {
    // Simple case: all unique.
    std::vector<int> seq1 = {3, 1, 2};
    assert((frequencySort(seq1, 100) == std::vector<int>{3, 1, 2})); // all freq 1, first occurrence order preserved

    // Classic case from the problem.
    std::vector<int> seq2 = {2, 1, 2, 3, 1, 2};
    assert((frequencySort(seq2, 100) == std::vector<int>{2, 2, 2, 1, 1, 3}));

    // Equal frequencies: first occurrence order.
    std::vector<int> seq3 = {1, 2, 1, 2};
    assert((frequencySort(seq3, 100) == std::vector<int>{1, 1, 2, 2}));

    // Single element.
    std::vector<int> seq4 = {5};
    assert((frequencySort(seq4, 100) == std::vector<int>{5}));

    // All same element.
    std::vector<int> seq5 = {7, 7, 7};
    assert((frequencySort(seq5, 100) == std::vector<int>{7, 7, 7}));

    // Mixed with multiple frequencies and ties.
    std::vector<int> seq6 = {9, 4, 9, 2, 4, 4, 9, 2, 2, 4};
    // Frequencies: 4 appears 4 times, 9 appears 3 times, 2 appears 3 times. 4 first, then 9 (since first appears at index 0) before 2 (first at index 3).
    assert((frequencySort(seq6, 100) == std::vector<int>{4, 4, 4, 4, 9, 9, 9, 2, 2, 2}));

    // Edge: maxValue might be larger than any value.
    std::vector<int> seq7 = {1, 1, 2};
    assert((frequencySort(seq7, 1000) == std::vector<int>{1, 1, 2}));

    // Empty sequence (though task says non-empty, function handles it).
    std::vector<int> seq8;
    assert((frequencySort(seq8, 100)) == std::vector<int>{});

    return 0;
}
