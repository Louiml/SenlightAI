// Write a C++ function that takes a vector of strings and returns a new vector containing the unique strings sorted first by length (shortest to longest) and then lexicographically (alphabetically) for strings of equal length. The input vector may contain duplicate strings, empty strings, and strings with mixed case; preserve the original case when sorting and comparing. The function should not modify the input vector.
The solution copies the input into a vector of pairs, where each pair holds the string's length and the string itself. Sorting is performed on this pair vector using a custom comparator that first compares lengths; if lengths differ, the shorter string comes first. If lengths are equal, the comparator falls back to lexicographic comparison of the strings using `operator<` (which compares character by character respecting case). After sorting, the `std::unique` algorithm is applied to remove consecutive duplicates. Since the vector is already sorted, duplicates are adjacent, and `unique` will remove them, returning an iterator to the new logical end; then `erase` trims the vector. Finally, we extract the strings from the unique pairs into a new vector to return. Edge cases include empty strings (length 0, sorted first) and duplicate strings that appear non-consecutively before sorting; sorting ensures they become consecutive. Time complexity is O(N log N) for sorting, where N is the number of strings, and O(N) for unique/erase. Space complexity is O(N) for the pair vector and the output vector.
#include <vector>
#include <string>
#include <algorithm>
#include <utility>

// Sorts and deduplicates strings by length, then lexicographically.
std::vector<std::string> sortedUniqueByLength(const std::vector<std::string>& input) {
    // Build a vector of (length, string) pairs.
    std::vector<std::pair<int, std::string>> pairs;
    pairs.reserve(input.size());
    for (const auto& s : input) {
        pairs.emplace_back(static_cast<int>(s.size()), s);
    }

    // Sort by length, then by string.
    std::sort(pairs.begin(), pairs.end(),
              [](const std::pair<int, std::string>& a,
                 const std::pair<int, std::string>& b) {
                  if (a.first != b.first) {
                      return a.first < b.first;
                  }
                  return a.second < b.second;
              });

    // Remove consecutive duplicates (which are now adjacent).
    auto last = std::unique(pairs.begin(), pairs.end(),
                            [](const std::pair<int, std::string>& a,
                               const std::pair<int, std::string>& b) {
                                return a.second == b.second;
                            });
    pairs.erase(last, pairs.end());

    // Extract the strings into the result.
    std::vector<std::string> result;
    result.reserve(pairs.size());
    for (const auto& p : pairs) {
        result.push_back(p.second);
    }
    return result;
}
#include <cassert>
#include <vector>
#include <string>

// Assuming the solution function is defined above.

int main() {
    // basic sorting by length
    std::vector<std::string> v1 = {"a", "bb", "ccc"};
    assert(sortedUniqueByLength(v1) == (std::vector<std::string>{"a", "bb", "ccc"}));

    // same length sorted lexicographically
    std::vector<std::string> v2 = {"banana", "apple", "cherry"};
    assert(sortedUniqueByLength(v2) == (std::vector<std::string>{"apple", "banana", "cherry"}));

    // duplicates removed
    std::vector<std::string> v3 = {"cat", "dog", "cat", "bird"};
    assert(sortedUniqueByLength(v3) == (std::vector<std::string>{"cat", "dog", "bird"}));

    // empty string sorts first
    std::vector<std::string> v4 = {"", "a", ""};
    assert(sortedUniqueByLength(v4) == (std::vector<std::string>{"", "a"}));

    // mixed case is case-sensitive
    std::vector<std::string> v5 = {"A", "a", "ab"};
    assert(sortedUniqueByLength(v5) == (std::vector<std::string>{"A", "a", "ab"}));

    // all same string
    std::vector<std::string> v6 = {"x", "x", "x"};
    assert(sortedUniqueByLength(v6) == (std::vector<std::string>{"x"}));

    // input not modified
    std::vector<std::string> v7 = {"b", "a"};
    sortedUniqueByLength(v7);
    assert(v7 == (std::vector<std::string>{"b", "a"}));

    // larger test
    std::vector<std::string> v8 = {"hello", "hi", "hey", "hi", "a", "hello"};
    std::vector<std::string> expected8 = {"a", "hi", "hey", "hello"};
    assert(sortedUniqueByLength(v8) == expected8);

    // all same length with duplicates and lexicographic
    std::vector<std::string> v9 = {"b", "c", "a", "b"};
    assert(sortedUniqueByLength(v9) == (std::vector<std::string>{"a", "b", "c"}));

    return 0;
}
