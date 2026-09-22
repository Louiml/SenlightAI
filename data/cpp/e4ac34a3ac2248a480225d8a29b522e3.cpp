// Write a C++ function `insertionSortStable(std::vector<int>& data)` that sorts a vector of integers in ascending order using the insertion sort algorithm. The function must modify the vector in place. For testing purposes, create a helper function `isSorted(const std::vector<int>& v)` that returns `true` if the vector is non-decreasing (i.e., each element is ≤ the next). The task focuses on implementing a correct, efficient insertion sort with proper handling of duplicate values and negative numbers, and verifying the result against `std::sort`. Do not use any built-in sorting functions inside your implementation (though you may use them in the test harness). Ensure the implementation is `const`-correct where appropriate and uses standard library facilities only where necessary (e.g., `std::vector`, `std::size_t`). The function should work for vectors of any size, including empty and single-element vectors, and must not cause undefined behavior.

// The insertion sort algorithm works by building a sorted portion of the array from left to right. Starting from index 1, we take the current element (`key`) and compare it with elements to its left (which are already sorted). We shift each larger element one position to the right until we find the correct position for `key`, then insert it. This is done in-place, so no extra array is needed. The algorithm is stable (equal elements retain their relative order) because we only shift elements strictly greater than the key. Time complexity is O(n²) in the worst and average cases, and O(n) in the best case (already sorted). Space complexity is O(1) auxiliary. Edge cases: empty vector (loop does not run), single element (loop runs once but no shifts), all equal elements (no shifts, O(n) time), reverse-sorted (max shifts). Duplicates and negatives require no special treatment. For the helper function `isSorted`, simply loop through the vector and check if `v[i] <= v[i+1]` for all i; handle empty and size-1 vectors by returning `true`. The verification in the test will compare the sorted result with `std::sort` on a copy to ensure correctness.

#include <vector>
#include <cstddef>

// Sorts a vector in ascending order using the insertion sort algorithm.
// Modifies the input vector in place. Stable: equal elements retain relative order.
void insertionSortStable(std::vector<int>& data) {
    for (std::size_t i = 1; i < data.size(); ++i) {
        int key = data[i];
        std::size_t j = i;
        // Shift elements greater than key to the right
        while (j > 0 && data[j - 1] > key) {
            data[j] = data[j - 1];
            --j;
        }
        data[j] = key;
    }
}

// Returns true if the vector is non-decreasing (sorted ascending).
bool isSorted(const std::vector<int>& v) {
    for (std::size_t i = 1; i < v.size(); ++i) {
        if (v[i - 1] > v[i]) {
            return false;
        }
    }
    return true;
}

#include <cassert>
#include <vector>
#include <algorithm>

int main() {
    // Test empty vector
    std::vector<int> v0 = {};
    insertionSortStable(v0);
    assert(isSorted(v0));
    assert(v0.empty());

    // Test single element
    std::vector<int> v1 = {42};
    insertionSortStable(v1);
    assert(isSorted(v1) && v1 == std::vector<int>{42});

    // Test already sorted
    std::vector<int> v2 = {1, 2, 3, 4, 5};
    insertionSortStable(v2);
    assert(isSorted(v2) && v2 == std::vector<int>{1, 2, 3, 4, 5});

    // Test reverse sorted
    std::vector<int> v3 = {5, 4, 3, 2, 1};
    insertionSortStable(v3);
    assert(isSorted(v3) && v3 == std::vector<int>{1, 2, 3, 4, 5});

    // Test with duplicates and negatives
    std::vector<int> v4 = {3, -1, 2, -1, 3, 0, -5};
    std::vector<int> expected4 = v4;
    std::sort(expected4.begin(), expected4.end());
    insertionSortStable(v4);
    assert(isSorted(v4) && v4 == expected4);

    // Test random vector against std::sort
    std::vector<int> v5 = {10, 7, 8, 9, 1, 5, 3, 2, 4, 6, 0, -2, -2, 100, -100};
    std::vector<int> expected5 = v5;
    std::sort(expected5.begin(), expected5.end());
    insertionSortStable(v5);
    assert(isSorted(v5) && v5 == expected5);

    // Test all equal elements
    std::vector<int> v6 = {7, 7, 7, 7};
    insertionSortStable(v6);
    assert(isSorted(v6) && v6 == std::vector<int>(4, 7));

    // Test large-ish vector with random values
    std::vector<int> v7 = {5, 3, 8, 1, 9, 2, 7, 4, 6, 0, -1, -3, 11, 10, -2};
    std::vector<int> expected7 = v7;
    std::sort(expected7.begin(), expected7.end());
    insertionSortStable(v7);
    assert(isSorted(v7) && v7 == expected7);

    return 0;
}
