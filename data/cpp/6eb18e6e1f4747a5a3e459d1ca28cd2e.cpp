Given an array of `n` distinct integers and an integer `k` (1 ≤ k ≤ n), write a C++ function named `selectKLargestAndSplit` that takes a `std::vector<long long>` and an integer `k` as parameters, selects the `k` largest elements, marks them by setting their values to `LLONG_MAX` (a sentinel for "removed"), and then returns a `std::vector<int>` containing the sizes of the contiguous segments of the original array after removing those `k` largest elements, in the order they appear from left to right. If no elements remain after removal, return an empty vector. The function must preserve the original ordering of the remaining (non-removed) elements and must not modify the input vector (it may create a copy internally). The returned segment sizes must sum exactly to `n - k`.

// The core idea is to first identify which indices hold the `k` largest values. Since the order of elements matters for splitting into segments, we cannot simply sort the values; we need the original indices. Approach: create a vector of pairs `(value, index)` from the input, sort it in descending order of value (or ascending with negative values), and pick the first `k` entries to mark those indices as removed. Then, create a copy of the input vector and set the values at those marked indices to `LLONG_MAX` as a sentinel. Finally, traverse the modified array from left to right, counting the length of each maximal block of elements that are **not** equal to `LLONG_MAX`. Each time we encounter a sentinel, we end the current block (if its length > 0) and store that length in the result vector. After the loop, if the last block has length > 0, push it as well. Edge cases: if `k = n`, all elements are removed, so the result is empty. If `k = 0` (though constraints say k≥1, be safe), return a single segment of size n. Time complexity is O(n log n) due to the sort, and space complexity is O(n) for the pairs and the copy. The sentinel value `LLONG_MAX` is safe because all original values are normal long long integers (presumably not equal to `LLONG_MAX`).

#include <vector>
#include <algorithm>
#include <cstdint>
#include <climits>

// Selects the k largest elements (by value) from the input vector, marks them as removed,
// and returns the sizes of contiguous segments of remaining elements in original order.
// The input vector is not modified.
std::vector<int> selectKLargestAndSplit(const std::vector<long long>& values, int k) {
    const int n = static_cast<int>(values.size());
    if (k <= 0) {
        // No removals: one segment of size n.
        return (n > 0) ? std::vector<int>{n} : std::vector<int>{};
    }
    if (k >= n) {
        // All elements removed: empty result.
        return {};
    }

    // Build vector of pairs (value, original index) and sort descending by value.
    std::vector<std::pair<long long, int>> indexed;
    indexed.reserve(n);
    for (int i = 0; i < n; ++i) {
        indexed.emplace_back(values[i], i);
    }
    std::sort(indexed.begin(), indexed.end(),
              [](const std::pair<long long, int>& a, const std::pair<long long, int>& b) {
                  return a.first > b.first; // descending
              });

    // Mark the first k indices as removed by setting them to LLONG_MAX.
    std::vector<long long> arr = values; // copy
    for (int i = 0; i < k; ++i) {
        int idx = indexed[i].second;
        arr[idx] = LLONG_MAX;
    }

    // Traverse and count segments of non-removed elements.
    std::vector<int> segments;
    int current_len = 0;
    for (int i = 0; i < n; ++i) {
        if (arr[i] == LLONG_MAX) {
            if (current_len > 0) {
                segments.push_back(current_len);
                current_len = 0;
            }
        } else {
            ++current_len;
        }
    }
    if (current_len > 0) {
        segments.push_back(current_len);
    }

    return segments;
}

#include <cassert>
#include <vector>
#include <cstdint>
#include <climits>

// Declare the function (already defined above) for the test.
std::vector<int> selectKLargestAndSplit(const std::vector<long long>& values, int k);

int main() {
    // Basic case: remove 2 largest (10 and 9) from [1,2,10,3,9,4] -> remaining [1,2,3,4] -> one segment of size 4.
    std::vector<long long> v1 = {1, 2, 10, 3, 9, 4};
    assert(selectKLargestAndSplit(v1, 2) == std::vector<int>({4}));

    // Case with multiple segments: remove 2 largest (100 and 50) from [100,1,2,50,3,4] -> remaining [1,2,3,4] -> segments [2,2].
    std::vector<long long> v2 = {100, 1, 2, 50, 3, 4};
    assert(selectKLargestAndSplit(v2, 2) == std::vector<int>({2, 2}));

    // Remove all elements: k = n.
    std::vector<long long> v3 = {5, 3, 8, 1};
    assert(selectKLargestAndSplit(v3, 4).empty());

    // Remove zero elements (though k>=1, test anyway): returns one segment of size n.
    std::vector<long long> v4 = {7, 2, 9};
    assert(selectKLargestAndSplit(v4, 0) == std::vector<int>({3}));

    // Single element, k=1 -> empty.
    std::vector<long long> v5 = {42};
    assert(selectKLargestAndSplit(v5, 1).empty());

    // Negative values: remove the most negative (i.e., smallest) is not correct; here remove largest which are -1 and -2.
    std::vector<long long> v6 = {-5, -1, -2, -10};
    // largest are -1 and -2, remove them -> remaining [-5, -10] -> segments [1,1] because -10 is after -2.
    assert(selectKLargestAndSplit(v6, 2) == std::vector<int>({1, 1}));

    // Duplicates test (though problem says distinct, but robust): [5,5,1] remove 1 largest -> remaining [5,1] -> segments [1,1].
    std::vector<long long> v7 = {5, 1, 5};
    assert(selectKLargestAndSplit(v7, 1) == std::vector<int>({1, 1}));

    // Two segments with one removed in middle: [1,2,3,4] remove 3 -> segments [2,1].
    std::vector<long long> v8 = {1, 2, 3, 4};
    assert(selectKLargestAndSplit(v8, 1) == std::vector<int>({2, 1}));

    // Large values near LLONG_MAX: test that sentinel does not conflict.
    std::vector<long long> v9 = {LLONG_MAX - 1, 123, LLONG_MAX - 2, 456};
    // largest are LLONG_MAX-1 and LLONG_MAX-2, remove them -> remaining [123,456] -> segment [2].
    assert(selectKLargestAndSplit(v9, 2) == std::vector<int>({2}));

    // All remaining in one block after removing first few: [10,9,1,2] remove 2 largest (10,9) -> [1,2] -> [2].
    std::vector<long long> v10 = {10, 9, 1, 2};
    assert(selectKLargestAndSplit(v10, 2) == std::vector<int>({2}));

    return 0;
}
