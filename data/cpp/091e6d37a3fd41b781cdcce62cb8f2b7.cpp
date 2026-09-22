// Write a C++ function that takes an array of integers and its size as parameters, sorts the array in ascending order using the selection sort algorithm (as shown in the snippet), and returns the sorted array by modifying it in place. The function must handle arrays of any non-negative size, including zero-length arrays, and must properly sort arrays with duplicate values and negative numbers. The sorting must be performed exactly as in the provided snippet: repeatedly find the smallest element from the unsorted portion and swap it into the correct position.
#include <cassert>

int main() {
    // Test 1: Normal unsorted array with distinct positive numbers
    int a1[] = {5, 2, 9, 1, 7};
    selectionSort(a1, 5);
    int expected1[] = {1, 2, 5, 7, 9};
    for (int i = 0; i < 5; ++i) assert(a1[i] == expected1[i]);

    // Test 2: Array with duplicates and negative numbers
    int a2[] = {-3, 0, -3, 2, 2, 10};
    selectionSort(a2, 6);
    int expected2[] = {-3, -3, 0, 2, 2, 10};
    for (int i = 0; i < 6; ++i) assert(a2[i] == expected2[i]);

    // Test 3: Single element array
    int a3[] = {42};
    selectionSort(a3, 1);
    assert(a3[0] == 42);

    // Test 4: Empty array (size 0)
    int a4[] = {};  // In C++, zero-size arrays are not standard; use pointer to a dummy
    int dummy = 0;
    selectionSort(&dummy, 0);  // size 0, no access happens
    assert(true);  // Just ensure no crash

    // Test 5: Already sorted array
    int a5[] = {1, 2, 3, 4};
    selectionSort(a5, 4);
    int expected5[] = {1, 2, 3, 4};
    for (int i = 0; i < 4; ++i) assert(a5[i] == expected5[i]);

    // Test 6: Reverse sorted array
    int a6[] = {9, 7, 5, 3};
    selectionSort(a6, 4);
    int expected6[] = {3, 5, 7, 9};
    for (int i = 0; i < 4; ++i) assert(a6[i] == expected6[i]);

    return 0;
}
#include <cstddef>  // for size_t

// Sorts the given array in ascending order using selection sort.
// Modifies the array in place. Accepts an array of any size (including 0).
void selectionSort(int arr[], size_t size) {
    if (size < 2) return;  // Empty or single-element arrays are already sorted

    for (size_t i = 0; i < size - 1; ++i) {
        // Find the index of the minimum element in the unsorted portion [i+1, size)
        size_t minIndex = i;
        for (size_t j = i + 1; j < size; ++j) {
            if (arr[j] < arr[minIndex]) {
                minIndex = j;
            }
        }
        // Swap the found minimum element with the element at position i
        if (minIndex != i) {
            int temp = arr[i];
            arr[i] = arr[minIndex];
            arr[minIndex] = temp;
        }
    }
}
// The solution directly implements selection sort. In each outer loop iteration, fix the current position `i` and scan the remaining subarray from `i+1` to the end to find the index of the minimum value. If a smaller element is found at index `j`, swap `arr[i]` and `arr[j]`. This guarantees that after `i` passes, the first `i+1` elements are in final sorted order. Edge cases: an empty array (size 0) or a single-element array (size 1) require no swaps and return immediately. Duplicate values are handled naturally since the comparison uses `<`, so equal elements are never swapped; the algorithm is not stable, but that is not required. Time complexity is \(O(n^2)\) due to the nested loops, with \(O(1)\) auxiliary space because the sort is performed in place using only a temporary variable for swapping.
