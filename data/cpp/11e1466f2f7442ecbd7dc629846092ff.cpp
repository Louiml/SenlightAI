Write a C++ function named `maxPairSequence` that accepts no parameters and returns a `std::vector<std::pair<int, int>>` containing all unique pairs from a predefined set of pairs, sorted in descending order by first element, and if two pairs have the same first element, sorted in descending order by second element. The function must internally use a `std::set<pair<int,int>, greater<pair<int,int>>>` to insert the following pairs in this exact order: `{10,1}, {10,2}, {9,2}, {8,3}, {7,4}, {6,5}`. Note that duplicate pairs are automatically removed by the set. Return the contents of the set as a vector in the set’s iteration order. The function must not read any input, must not use global variables, and must be `const`-correct where applicable.
// The core idea is to exploit `std::set` with a custom comparator `greater<pair<int,int>>`, which orders elements in descending lexicographical order (first by first element descending, then by second element descending). Inserting the given pairs in any order—here we insert `{10,1}` first, then `{10,2}`, and the rest—will result in the set storing them sorted descending. Since sets store unique keys, a duplicate pair like `{10,1}` inserted again would be ignored, but in our provided list there are no exact duplicates, so all six unique pairs are retained. After insertion, we iterate through the set and push each pair into a `std::vector`. The time complexity is O(n log n) for n insertions into a set (here n=6), and O(n) to copy into the vector. Space complexity is O(n) for the set and vector. Edge cases: if the set is empty, the vector is empty; but here we always have at least one pair. Because we use `greater<pair<int,int>>`, the order is strictly descending by first then second, matching the expected output.
#include <vector>
#include <set>
#include <utility>

// Return all pairs from a fixed set, sorted descending by (first, second).
std::vector<std::pair<int, int>> maxPairSequence() {
    std::set<std::pair<int, int>, std::greater<std::pair<int, int>>> s;
    s.insert({10, 1});
    s.insert({10, 2});
    s.insert({9, 2});
    s.insert({8, 3});
    s.insert({7, 4});
    s.insert({6, 5});

    std::vector<std::pair<int, int>> result;
    result.reserve(s.size());
    for (const auto& p : s) {
        result.push_back(p);
    }
    return result;
}
#include <cassert>
#include <vector>
#include <utility>

// Assume maxPairSequence from the solution is available.

int main() {
    auto result = maxPairSequence();
    std::vector<std::pair<int, int>> expected = {
        {10, 2}, {10, 1}, {9, 2}, {8, 3}, {7, 4}, {6, 5}
    };
    assert(result.size() == expected.size());
    for (size_t i = 0; i < expected.size(); ++i) {
        assert(result[i] == expected[i]);
    }

    // Check that order is descending first, then descending second.
    for (size_t i = 1; i < result.size(); ++i) {
        assert(result[i-1].first > result[i].first ||
              (result[i-1].first == result[i].first && result[i-1].second > result[i].second));
    }

    // Check uniqueness (no exact duplicates).
    for (size_t i = 0; i < result.size(); ++i) {
        for (size_t j = i + 1; j < result.size(); ++j) {
            assert(result[i] != result[j]);
        }
    }
}
