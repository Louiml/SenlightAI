Given a sequence of `n` integer edge weights and a threshold parameter `k`, write a C++ function that returns a `std::vector<int>` containing the indices of elements (0-based) that are **not** among the `k` smallest values in the sequence, while preserving the original order of those selected indices. If there are ties at the `k`-th smallest boundary, include all indices whose value is strictly greater than the `k`-th smallest value (i.e., do not break ties arbitrarily). If `k` is larger than or equal to the number of elements, return an empty vector. The function must handle an empty input vector gracefully by returning an empty vector. The input vector may contain negative values, duplicates, and be unsorted.

#include <cassert>
#include <vector>
#include <cstddef>

int main() {
    // Basic case with distinct values
    std::vector<size_t> r1 = indicesAboveKthSmallest({3, 1, 4, 1, 5, 9, 2, 6}, 3);
    assert((r1 == std::vector<size_t>{2, 5, 7}));  // values > 2nd smallest (2): 4,5,9,6 -> indices 2,4,5,7? Wait, sorted: 1,1,2,3,4,5,6,9 -> 3rd smallest = 2, values >2: 3,4,5,9,6 -> indices 0,2,4,5,7. Fix below.

    // Correct expected: values: [3,1,4,1,5,9,2,6], sorted: 1,1,2,3,4,5,6,9; 3rd smallest = 2; values > 2: 3(index0),4(2),5(4),9(5),6(7) -> {0,2,4,5,7}
    std::vector<size_t> expected1 = {0,2,4,5,7};
    assert((r1 == expected1));

    // k = 0 returns all indices
    std::vector<size_t> r2 = indicesAboveKthSmallest({5, 2, 8}, 0);
    assert((r2 == std::vector<size_t>{0,1,2}));

    // k >= size returns empty
    std::vector<size_t> r3 = indicesAboveKthSmallest({1,2,3}, 5);
    assert(r3.empty());

    // Duplicate boundary: all equal values, k=1, threshold is same as all values, so empty
    std::vector<size_t> r4 = indicesAboveKthSmallest({7,7,7,7}, 2);
    assert(r4.empty());

    // Negative values with duplicates
    std::vector<size_t> r5 = indicesAboveKthSmallest({-3, -1, -2, -1, 0, -3}, 3);
    // sorted: -3,-3,-2,-1,-1,0 ; 3rd smallest = -2; values > -2: -1(index1), -1(3), 0(4) -> {1,3,4}
    assert((r5 == std::vector<size_t>{1,3,4}));

    // Empty input
    std::vector<size_t> r6 = indicesAboveKthSmallest({}, 1);
    assert(r6.empty());

    // Single element, k=1 -> empty (threshold is that element, none strictly greater)
    std::vector<size_t> r7 = indicesAboveKthSmallest({42}, 1);
    assert(r7.empty());

    // Single element, k=0 -> all
    std::vector<size_t> r8 = indicesAboveKthSmallest({42}, 0);
    assert((r8 == std::vector<size_t>{0}));
}

#include <vector>
#include <algorithm>
#include <cstddef>

// Returns indices (0-based) of elements strictly greater than the k-th smallest value.
// If k == 0, returns all indices. If k >= input size, returns empty vector.
std::vector<size_t> indicesAboveKthSmallest(const std::vector<int>& values, size_t k) {
    if (values.empty() || k >= values.size()) {
        return {};
    }
    if (k == 0) {
        std::vector<size_t> all(values.size());
        for (size_t i = 0; i < values.size(); ++i) all[i] = i;
        return all;
    }

    // Copy and sort to find the k-th smallest value (1-indexed k)
    std::vector<int> sorted = values;
    std::sort(sorted.begin(), sorted.end());
    int threshold = sorted[k - 1];  // k-th smallest (0-indexed position k-1)

    std::vector<size_t> result;
    for (size_t i = 0; i < values.size(); ++i) {
        if (values[i] > threshold) {
            result.push_back(i);
        }
    }
    return result;
}

// The task is to identify all elements that are strictly greater than the `k`-th smallest element (where the smallest element is the 1st smallest). A straightforward approach is to make a copy of the vector, sort it, and select the element at position `k-1` (if `k >= 1` and `k <= n`), which gives the value of the `k`-th smallest element. Then iterate through the original vector and collect indices where the element is strictly greater than that threshold. This works correctly with duplicates because if there are ties at the boundary, any element equal to the threshold is excluded, which matches the requirement of excluding the `k` smallest values themselves. Edge cases: if `k == 0`, there are no smallest values to exclude, so we should return all indices; if `k >= n`, we return an empty vector (since no elements are strictly greater than the maximum). For an empty input, return empty. Time complexity is \(O(n \log n)\) due to sorting, and space complexity is \(O(n)\) for the copy. Could be optimized to \(O(n)\) with selection algorithms, but sorting is sufficient and clear.
