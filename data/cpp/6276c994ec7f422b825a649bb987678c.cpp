Write a C++ function named `selectionSortAscending` that takes a mutable `std::vector<int>&` and sorts its elements in strictly ascending order using the selection sort algorithm (repeatedly finding the minimum element of the unsorted portion and swapping it with the first unsorted element). The function must modify the vector in place, return `void`, and handle arrays of any size, including empty vectors and vectors with duplicate values. For an empty vector, the function should do nothing. For a vector with one element, no swaps should occur. The function should use `const` correctness where appropriate for internal bounds but not on the input parameter since it is modified.

#include <cassert>
#include <vector>
#include <algorithm>

int main() {
    // Empty vector: no change.
    std::vector<int> empty;
    selectionSortAscending(empty);
    assert(empty.empty());

    // Single element.
    std::vector<int> single = {42};
    selectionSortAscending(single);
    assert(single == std::vector<int>({42}));

    // Already sorted.
    std::vector<int> sorted = {1, 2, 3, 4};
    selectionSortAscending(sorted);
    assert(sorted == std::vector<int>({1, 2, 3, 4}));

    // Reverse sorted.
    std::vector<int> reverse = {5, 4, 3, 2, 1};
    selectionSortAscending(reverse);
    assert(reverse == std::vector<int>({1, 2, 3, 4, 5}));

    // Unsorted with duplicates.
    std::vector<int> duplicates = {3, 1, 2, 1, 3, 0};
    selectionSortAscending(duplicates);
    assert(duplicates == std::vector<int>({0, 1, 1, 2, 3, 3}));

    // Negative numbers.
    std::vector<int> negatives = {-5, -10, 0, 7, -1};
    selectionSortAscending(negatives);
    assert(negatives == std::vector<int>({-10, -5, -1, 0, 7}));

    // Verify with std::is_sorted for a larger random-ish case.
    std::vector<int> mixed = {9, -2, 4, 8, 1, 0, -7, 3, 3, 5};
    selectionSortAscending(mixed);
    assert(std::is_sorted(mixed.begin(), mixed.end()));

    return 0;
}

#include <vector>
#include <algorithm> // for std::swap

// Sorts the vector in place using selection sort (ascending order).
void selectionSortAscending(std::vector<int>& arr) {
    const size_t n = arr.size();
    for (size_t i = 0; i + 1 < n; ++i) {
        // Find the index of the minimum element from i to end.
        size_t min_idx = i;
        for (size_t j = i + 1; j < n; ++j) {
            if (arr[j] < arr[min_idx]) {
                min_idx = j;
            }
        }
        // Swap the found minimum with the first unsorted position.
        if (min_idx != i) {
            std::swap(arr[i], arr[min_idx]);
        }
    }
}

// The solution follows the classic selection sort algorithm: For each position `i` from 0 to `n-2`, find the index of the minimum element in the subarray `arr[i..n-1]`, then swap it with the element at position `i`. This guarantees that after each outer-loop iteration, the first `i+1` elements are in their final sorted positions. Edge cases: an empty vector (loop simply doesn't execute) and a single-element vector (loop for `i < 0` never runs). Duplicate values are handled naturally because the algorithm only swaps when a strictly smaller element is found, preserving stable behavior for equal elements (though stability is not required). Time complexity is `O(n^2)` for all cases because of the nested loops; space complexity is `O(1)` auxiliary space since only a few index and temporary variables are used, and the sort is in-place.
