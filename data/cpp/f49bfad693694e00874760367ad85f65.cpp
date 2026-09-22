Write a standalone C++ function that, given a vector of integer values representing symbol qualities (higher is better) and a vector of integer IDs, returns a new vector of IDs sorted by descending quality, with ties broken by ascending ID. The function must be const-correct, take inputs by const reference, and handle empty inputs gracefully by returning an empty vector. This task is inspired by the index-building step in the provided code where symbols are sorted by quality before constructing inverted indexes.

The solution is straightforward: create a vector of pairs (quality, ID), sort it with a custom comparator that places higher quality first and, for equal quality, lower ID first. Since the input sizes are equal (each ID corresponds to a quality at the same index), we iterate over indices to build the pairs. If inputs have mismatched sizes, we should handle that by either asserting or returning an empty vector; for simplicity, we assume equal-length inputs but can add a guard. Edge cases: empty vectors (return empty), single element (return that ID), duplicate qualities (ties broken by ID). Time complexity is O(n log n) due to sorting, space complexity O(n) for the pairs vector. We use `std::stable_sort` or `std::sort` with a strict weak ordering comparator; `std::sort` suffices because tie-breaking handles equal qualities explicitly.

#include <vector>
#include <algorithm>
#include <cassert>

// Given parallel vectors of symbol qualities and IDs (same size, where
// quality[i] corresponds to ID[i]), return a vector of IDs sorted by
// descending quality, and for equal qualities, ascending ID.
std::vector<int> sortByQuality(const std::vector<float>& qualities,
                               const std::vector<int>& ids) {
    assert(qualities.size() == ids.size());
    std::vector<std::pair<float, int>> pairs;
    pairs.reserve(ids.size());
    for (size_t i = 0; i < ids.size(); ++i) {
        pairs.emplace_back(qualities[i], ids[i]);
    }
    std::sort(pairs.begin(), pairs.end(),
              [](const auto& a, const auto& b) {
                  if (a.first != b.first) {
                      return a.first > b.first; // higher quality first
                  }
                  return a.second < b.second;   // lower ID first on ties
              });
    std::vector<int> result;
    result.reserve(ids.size());
    for (const auto& p : pairs) {
        result.push_back(p.second);
    }
    return result;
}

#include <cassert>
#include <vector>
#include <iostream>

int main() {
    // Basic ordering by quality
    std::vector<float> q1 = {1.0f, 3.0f, 2.0f};
    std::vector<int> i1 = {10, 20, 30};
    assert(sortByQuality(q1, i1) == std::vector<int>({20, 30, 10}));

    // Ties broken by ascending ID
    std::vector<float> q2 = {2.0f, 2.0f, 1.0f};
    std::vector<int> i2 = {5, 3, 7};
    assert(sortByQuality(q2, i2) == std::vector<int>({3, 5, 7}));

    // Empty input
    std::vector<float> q3;
    std::vector<int> i3;
    assert(sortByQuality(q3, i3).empty());

    // Single element
    std::vector<float> q4 = {4.5f};
    std::vector<int> i4 = {42};
    assert(sortByQuality(q4, i4) == std::vector<int>({42}));

    // All equal qualities and IDs sorted by ID
    std::vector<float> q5 = {1.0f, 1.0f, 1.0f};
    std::vector<int> i5 = {9, 2, 5};
    assert(sortByQuality(q5, i5) == std::vector<int>({2, 5, 9}));

    // Negative qualities and multiple ties
    std::vector<float> q6 = {-1.0f, 0.0f, -1.0f, 0.0f};
    std::vector<int> i6 = {4, 3, 2, 1};
    // Sorted by quality: 0.0 (ID1), 0.0 (ID3), -1.0 (ID2), -1.0 (ID4) -> 1,3,2,4
    assert(sortByQuality(q6, i6) == std::vector<int>({1, 3, 2, 4}));

    std::cout << "All tests passed." << std::endl;
    return 0;
}
