Write a C++ function named `selectionSort` that takes a non-empty `std::vector<int>` by value, sorts it in ascending order using the selection sort algorithm, and returns the sorted vector. The function must not modify the original input vector (pass by value ensures a copy, but the implementation must work on the copy). Handle vectors with duplicate values, negative numbers, and up to 20 elements. The function should be `const`-correct in the sense that it does not alter any external state.

The selection sort algorithm repeatedly finds the minimum element from the unsorted part of the array and swaps it with the first unsorted position. We iterate an outer loop from `k = 0` to `n-2`, where `n` is the size of the vector. For each `k`, we assume `a[k]` is the smallest in the unsorted portion (indices `k` to `n-1`) by setting `index = k`. Then an inner loop from `i = k+1` to `n-1` scans for any element smaller than `a[index]`; if found, update `index`. After the inner loop, swap `a[index]` with `a[k]`. This ensures that after `k` iterations, the first `k+1` elements are the smallest `k+1` elements in sorted order. Edge cases: single-element vectors (outer loop runs zero times, returns as is), duplicates (they compare equal and do not cause incorrect swaps), negative numbers work fine. Time complexity is O(n²) for both average and worst cases because of two nested loops. Space complexity is O(1) auxiliary (only a few temporary variables), but the function returns a copy of the input, so the original stays unchanged.

#include <vector>
#include <algorithm> // for std::swap

// Sorts a copy of the input vector in ascending order using selection sort.
// Returns the sorted vector. The original vector remains unchanged.
std::vector<int> selectionSort(std::vector<int> a) {
    int n = a.size();
    for (int k = 0; k < n - 1; ++k) {
        int index = k;               // assume current position is smallest
        for (int i = k + 1; i < n; ++i) {
            if (a[i] < a[index]) {
                index = i;           // found a smaller element
            }
        }
        // Swap the found minimum with the first unsorted position
        if (index != k) {
            std::swap(a[k], a[index]);
        }
    }
    return a;
}

#include <cassert>
#include <vector>

// The function declaration (assume included from above)
std::vector<int> selectionSort(std::vector<int> a);

int main() {
    // Basic sorting
    assert((selectionSort({3, 1, 2}) == std::vector<int>{1, 2, 3}));
    // Already sorted
    assert((selectionSort({1, 2, 3, 4}) == std::vector<int>{1, 2, 3, 4}));
    // Reverse sorted
    assert((selectionSort({5, 4, 3, 2, 1}) == std::vector<int>{1, 2, 3, 4, 5}));
    // Single element
    assert((selectionSort({7}) == std::vector<int>{7}));
    // Duplicates
    assert((selectionSort({4, 2, 4, 2, 4}) == std::vector<int>{2, 2, 4, 4, 4}));
    // Negative numbers
    assert((selectionSort({-3, 0, -10, 5}) == std::vector<int>{-10, -3, 0, 5}));
    // All same values
    assert((selectionSort({9, 9, 9}) == std::vector<int>{9, 9, 9}));
    // Larger vector up to 20 elements
    std::vector<int> input = {20, 15, 3, 8, 1, 19, 4, 11, 0, 17, 6, 12, 5, 9, 14, 2, 18, 7, 16, 13};
    std::vector<int> expected = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
    assert((selectionSort(input) == expected));

    return 0;
}
