Write a C++ function named `bubbleSortAscending` that takes an array of integers and its size as parameters, sorts the array in ascending order using the same comparison-and-swap logic as the provided snippet (specifically, the nested loop style where each pair `(i, j)` is compared and swapped if `a[i] < a[j]`), and returns nothing since the array is modified in place. The function must correctly handle arrays of any positive size, including size 1 (where no swaps occur) and arrays with duplicate or negative values. The function should be `const`-correct for inputs that are not modified—though here the array is intentionally modified, so apply `const` to the size parameter. You may use any standard library headers. Do not include a `main` function in the solution section.

The solution replicates the exact nested-loop structure from the snippet: an outer loop runs from `i = 0` to `size - 1`, and an inner loop runs from `j = 0` to `size - 1`. For every pair `(i, j)`, if `a[i] < a[j]`, the two elements are swapped. This is not the standard bubble sort (which uses adjacent comparisons), but it is a valid sorting algorithm (essentially a bidirectional selection-like sort) that produces a fully sorted array in ascending order. The key insight is that repeated comparisons and swaps across all index pairs eventually move the smallest elements to the front and the largest to the back, even though it performs many redundant swaps. The algorithm terminates with all elements in non-decreasing order. Edge cases: size 1 or 0 (if allowed) require no work; duplicate values are naturally handled because the condition is strict `<`, so equal values are never swapped unnecessarily. Negative integers sort correctly because integer comparisons are used. Time complexity is \(O(n^2)\) due to the double loop, and space complexity is \(O(1)\) extra space (only a temporary variable for swapping). The function modifies the array in place, so it is not `const` for the array parameter, but the size parameter should be `const` to signal it is not modified.

#include <cstddef> // for size_t

// Sorts an integer array in ascending order using the exact nested-loop comparison
// and swap logic from the provided snippet. Modifies the array in place.
void bubbleSortAscending(int* array, const std::size_t size) {
    for (std::size_t i = 0; i < size; ++i) {
        for (std::size_t j = 0; j < size; ++j) {
            if (array[i] < array[j]) {
                int temp = array[i];
                array[i] = array[j];
                array[j] = temp;
            }
        }
    }
}

#include <cassert>
#include <cstddef>

// Declaration of the function to test (in real project, this would be in a header)
void bubbleSortAscending(int* array, const std::size_t size);

int main() {
    // Test 1: Basic unsorted array (mirrors the snippet's example)
    int a1[5] = {6, 9, 8, 7, 3};
    bubbleSortAscending(a1, 5);
    int expected1[5] = {3, 6, 7, 8, 9};
    for (std::size_t i = 0; i < 5; ++i) {
        assert(a1[i] == expected1[i]);
    }

    // Test 2: Array with negative and zero values
    int a2[4] = {-5, 10, 0, -2};
    bubbleSortAscending(a2, 4);
    int expected2[4] = {-5, -2, 0, 10};
    for (std::size_t i = 0; i < 4; ++i) {
        assert(a2[i] == expected2[i]);
    }

    // Test 3: Already sorted array (should remain unchanged)
    int a3[3] = {1, 2, 3};
    bubbleSortAscending(a3, 3);
    int expected3[3] = {1, 2, 3};
    for (std::size_t i = 0; i < 3; ++i) {
        assert(a3[i] == expected3[i]);
    }

    // Test 4: Reverse sorted array
    int a4[4] = {9, 7, 5, 3};
    bubbleSortAscending(a4, 4);
    int expected4[4] = {3, 5, 7, 9};
    for (std::size_t i = 0; i < 4; ++i) {
        assert(a4[i] == expected4[i]);
    }

    // Test 5: Array with duplicates
    int a5[6] = {4, 4, 1, 4, 2, 1};
    bubbleSortAscending(a5, 6);
    int expected5[6] = {1, 1, 2, 4, 4, 4};
    for (std::size_t i = 0; i < 6; ++i) {
        assert(a5[i] == expected5[i]);
    }

    // Test 6: Single element array (no swaps needed)
    int a6[1] = {42};
    bubbleSortAscending(a6, 1);
    assert(a6[0] == 42);

    // Test 7: All identical elements
    int a7[3] = {7, 7, 7};
    bubbleSortAscending(a7, 3);
    int expected7[3] = {7, 7, 7};
    for (std::size_t i = 0; i < 3; ++i) {
        assert(a7[i] == expected7[i]);
    }

    // Test 8: Large values (including INT_MAX and INT_MIN)
    int a8[4] = {2147483647, -2147483648, 0, 1};
    bubbleSortAscending(a8, 4);
    int expected8[4] = {-2147483648, 0, 1, 2147483647};
    for (std::size_t i = 0; i < 4; ++i) {
        assert(a8[i] == expected8[i]);
    }

    return 0;
}
