// Given a non-empty vector of unique integers and a separate vector of integer "keys" (some of which may not be present in the data vector), write a C++ function that returns a new vector containing the input data sorted in ascending order by the values of the keys, but with a twist: the keys define a partial order only. Specifically, for any two elements in the data vector, if both of their key values exist in the key vector, then they must be ordered by those key values (ascending). If the key for an element does not exist in the key vector, that element must be placed at the very end of the result, preserving its relative order with other such "missing-key" elements (i.e., stable insertion at the end, in the original order among themselves). Elements with existing keys preserve their relative order among themselves when keys are equal (which cannot happen if keys are unique, but the input data values themselves are unique—however, keys may not be unique; handle ties stably). The function should take the data vector and the key vector as inputs (e.g., `std::vector<int> data`, `std::vector<int> keys`), and return a new sorted vector. For example, given `data = {3, 1, 4, 2}` and `keys = {4, 2}`, the result should be `{2, 4, 3, 1}` because keys 2 and 4 sort to 2 then 4, and elements 3 and 1 have missing keys and appear at the end in original order (3 then 1). If a key appears multiple times in the key vector, treat it as a single distinct key (i.e., use a set). The input vectors may be empty or contain only missing keys; handle those cases gracefully. Write a standalone function `std::vector<int> sortByKeys(const std::vector<int>& data, const std::vector<int>& keys)` that implements this logic.
#include <cassert>
#include <vector>
#include <iostream>

// Include the solution function here (or include a header) – for brevity, place it above.

int main() {
    // Basic case with both existing and missing keys.
    std::vector<int> data = {3, 1, 4, 2};
    std::vector<int> keys = {4, 2};
    std::vector<int> result = sortByKeys(data, keys);
    assert(result == std::vector<int>({2, 4, 3, 1}));

    // All keys exist: normal sorting.
    std::vector<int> data2 = {5, 1, 3, 2};
    std::vector<int> keys2 = {1, 2, 3, 5};
    assert(sortByKeys(data2, keys2) == std::vector<int>({1, 2, 3, 5}));

    // No keys exist: original order preserved.
    std::vector<int> data3 = {9, 8, 7};
    std::vector<int> keys3 = {10, 11};
    assert(sortByKeys(data3, keys3) == std::vector<int>({9, 8, 7}));

    // Empty data.
    assert(sortByKeys({}, {1,2}) == std::vector<int>());

    // Multiple elements share same key (but data values are unique, so not possible; simulate with non-unique data? The spec says unique integers, but we handle duplicates anyway).
    std::vector<int> data4 = {2, 1, 2}; // violates "unique" but test robustness
    std::vector<int> keys4 = {2};
    // Elements with key 2 appear first (stable ordering: both 2s in original order), then missing 1.
    // Original order: [2(1st), 1, 2(2nd)] -> with key: [2(1st), 2(2nd)], without: [1] -> result [2,2,1]
    assert(sortByKeys(data4, keys4) == std::vector<int>({2, 2, 1}));

    // Missing keys must preserve original relative order among themselves.
    std::vector<int> data5 = {4, 6, 1, 3, 2};
    std::vector<int> keys5 = {2};
    // With key: [2], without (in order): [4,6,1,3] -> result [2,4,6,1,3]
    assert(sortByKeys(data5, keys5) == std::vector<int>({2, 4, 6, 1, 3}));

    std::cout << "All tests passed." << std::endl;
    return 0;
}
#include <vector>
#include <unordered_set>
#include <algorithm>
#include <utility>

// Sort data by keys: elements whose value is present in the keys set are sorted
// in ascending order by that key value (stable), and all other elements are
// appended at the end in their original relative order.
std::vector<int> sortByKeys(const std::vector<int>& data, const std::vector<int>& keys) {
    // Build a set of valid keys for O(1) lookup.
    std::unordered_set<int> keySet(keys.begin(), keys.end());

    // Partition data into those with a key and those without.
    std::vector<std::pair<int, int>> withKey; // (key value, original data value)
    std::vector<int> withoutKey;              // preserve original order
    withKey.reserve(data.size());
    withoutKey.reserve(data.size());

    for (int value : data) {
        if (keySet.find(value) != keySet.end()) {
            withKey.emplace_back(value, value); // key = value itself? Wait: key is derived from the value? Need careful.
        }
    }
    // The problem statement says: "keys define a partial order ... for any two elements, if both of their key values exist in the key vector, then they must be ordered by those key values." The key for an element is the element's value itself, but only if that value is present in the keys vector. So the key is exactly the value. That simplifies: we just need to sort the subset of data whose values are in keys, in ascending order, preserving original order for equal values (which are unique, but we handle stable anyway). Elements not in keys go to the end in original order.
    // Let's redo cleanly.
    std::vector<int> withKeyValues;
    std::vector<int> withoutKeyValues;
    for (int value : data) {
        if (keySet.count(value)) {
            withKeyValues.push_back(value);
        } else {
            withoutKeyValues.push_back(value);
        }
    }
    // Sort withKeyValues ascending. Since values are unique, stable sort is trivial but use stable_sort for safety.
    std::stable_sort(withKeyValues.begin(), withKeyValues.end());
    // Concatenate: sorted keys first, then missing keys in original order.
    withKeyValues.insert(withKeyValues.end(), withoutKeyValues.begin(), withoutKeyValues.end());
    return withKeyValues;
}
// The core challenge is to sort elements that have a valid key (present in the key set) by their key values, while moving elements without a valid key to the end, preserving their original relative order. We first build a `std::unordered_set<int>` (or `std::set`) from the keys vector to allow O(1) lookup for whether a key exists. Then we partition the data into two groups: those with keys present, and those without. For the "with-key" group, we need to sort by key value stably. Since we must preserve original relative order when keys are equal (or when the same key is used multiple times), we can pair each element with its key and use `std::stable_sort` with a comparator that compares only the key values. After sorting, we concatenate the sorted "with-key" group with the original-order "missing-key" group. Edge cases include empty data (return empty), empty keys (all elements are missing-key, so return a copy of data), and duplicate keys (we treat them as a set, but if multiple elements have the same key, `stable_sort` preserves their original order). Time complexity: building the set takes O(k) where k is the number of keys. Partitioning takes O(n) for n data elements. `stable_sort` is O(m log m) where m is the number of elements with keys (worst-case n). So overall O(n log n) time, O(n) auxiliary space for the result and partition vectors.
