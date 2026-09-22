// Write a C++ function `rearrangeBarcodes` that takes a non-empty vector of integers representing barcode labels and returns a new vector where no two adjacent elements are equal. The input may contain duplicate values, and it is guaranteed that such a rearrangement is always possible (i.e., the maximum frequency of any single element is at most the ceiling of `n/2` where `n` is the vector size). The function must preserve the multiset of all input elements exactly. Return the rearranged vector; the order of distinct elements in the output is not specified, only that adjacency constraint holds.

The core idea is to place the most frequent element first at even indices, then fill the remaining even indices, and finally odd indices, with the remaining elements in decreasing order of frequency. Since the most frequent element appears at most `ceil(n/2)` times, placing it at every other position (starting at index 0) guarantees no two are adjacent. A simpler and robust approach: count frequencies using `unordered_map`, sort pairs by frequency descending, then fill the result vector by iterating over the sorted pairs and placing each element at positions `0, 2, 4, ...` and then wrapping to `1, 3, 5, ...` after reaching the end. Edge cases: a single distinct element (which is only valid if `n==1`), all elements identical (invalid unless `n==1`), and multiple elements with equal highest frequency — the sorting handles them arbitrarily but the adjacency constraint still holds because the gap placement separates equal values. Time complexity: `O(n + k log k)` where `k` is the number of distinct elements (for sorting), but we can also do `O(n + k)` using bucket placement if we avoid sorting by only extracting the most frequent one first as in the snippet; however, the simpler sort-based approach is clear and acceptable. Space complexity: `O(n + k)` for the result vector and hash map.

#include <vector>
#include <unordered_map>
#include <algorithm>

// Given a vector of integers, returns a rearranged vector where no two
// adjacent elements are equal. The input is guaranteed to be rearrangeable.
std::vector<int> rearrangeBarcodes(const std::vector<int>& barcodes) {
    std::unordered_map<int, int> counts;
    for (int code : barcodes) {
        ++counts[code];
    }

    // Sort (value, count) pairs by count descending.
    std::vector<std::pair<int, int>> pairs;
    for (const auto& [value, count] : counts) {
        pairs.emplace_back(count, value);
    }
    std::sort(pairs.begin(), pairs.end(),
              [](const auto& a, const auto& b) {
                  return a.first > b.first;
              });

    std::vector<int> result(barcodes.size());
    int index = 0;

    // Fill even indices first, then odd indices.
    for (const auto& [count, value] : pairs) {
        for (int i = 0; i < count; ++i) {
            result[index] = value;
            index += 2;
            if (index >= static_cast<int>(result.size())) {
                index = 1;
            }
        }
    }
    return result;
}

#include <cassert>
#include <vector>

int main() {
    // Basic case with alternating single element
    std::vector<int> b1 = {1, 1, 2};
    std::vector<int> r1 = rearrangeBarcodes(b1);
    assert(r1.size() == 3);
    for (size_t i = 0; i + 1 < r1.size(); ++i) {
        assert(r1[i] != r1[i + 1]);
    }

    // All distinct elements
    std::vector<int> b2 = {1, 2, 3};
    std::vector<int> r2 = rearrangeBarcodes(b2);
    assert(r2.size() == 3);
    for (size_t i = 0; i + 1 < r2.size(); ++i) {
        assert(r2[i] != r2[i + 1]);
    }

    // Single element
    std::vector<int> b3 = {7};
    std::vector<int> r3 = rearrangeBarcodes(b3);
    assert(r3 == std::vector<int>({7}));

    // Two elements with equal frequency
    std::vector<int> b4 = {1, 1, 2, 2};
    std::vector<int> r4 = rearrangeBarcodes(b4);
    assert(r4.size() == 4);
    for (size_t i = 0; i + 1 < r4.size(); ++i) {
        assert(r4[i] != r4[i + 1]);
    }

    // More complex: multiple duplicates
    std::vector<int> b5 = {1, 1, 1, 2, 2, 3};
    std::vector<int> r5 = rearrangeBarcodes(b5);
    assert(r5.size() == 6);
    for (size_t i = 0; i + 1 < r5.size(); ++i) {
        assert(r5[i] != r5[i + 1]);
    }

    // Verify multiset preserved for one case
    std::vector<int> b6 = {1, 1, 2, 2, 3};
    std::vector<int> r6 = rearrangeBarcodes(b6);
    std::vector<int> sorted_b6 = b6, sorted_r6 = r6;
    std::sort(sorted_b6.begin(), sorted_b6.end());
    std::sort(sorted_r6.begin(), sorted_r6.end());
    assert(sorted_b6 == sorted_r6);

    return 0;
}
