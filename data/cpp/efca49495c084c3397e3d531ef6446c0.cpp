Write a C++ function that sorts an array of integers in ascending order using the bubble sort algorithm. The function should take a pointer to the first element of the array and its size as parameters. It should sort the array in-place and return the sorted array by modifying the original. The function must also handle edge cases: empty arrays (size 0) and arrays with duplicate elements. For testing purposes, the function should be named `bubbleSortArray` and accept parameters `(int arr[], int size)`. The function must not print anything; it should only modify the array. Provide a separate test harness using `assert` to verify correctness.

#include <cassert>

int main() {
    // Test 1: Normal unsorted array
    int arr1[] = {5, 2, 9, 1, 7};
    bubbleSortArray(arr1, 5);
    assert(arr1[0] == 1 && arr1[1] == 2 && arr1[2] == 5 && arr1[3] == 7 && arr1[4] == 9);

    // Test 2: Already sorted array
    int arr2[] = {1, 2, 3, 4};
    bubbleSortArray(arr2, 4);
    assert(arr2[0] == 1 && arr2[1] == 2 && arr2[2] == 3 && arr2[3] == 4);

    // Test 3: Reverse sorted array
    int arr3[] = {10, 8, 6, 4, 2, 0};
    bubbleSortArray(arr3, 6);
    assert(arr3[0] == 0 && arr3[1] == 2 && arr3[2] == 4 && arr3[3] == 6 && arr3[4] == 8 && arr3[5] == 10);

    // Test 4: Array with duplicates
    int arr4[] = {3, 1, 3, 2, 1};
    bubbleSortArray(arr4, 5);
    assert(arr4[0] == 1 && arr4[1] == 1 && arr4[2] == 2 && arr4[3] == 3 && arr4[4] == 3);

    // Test 5: Single element array
    int arr5[] = {42};
    bubbleSortArray(arr5, 1);
    assert(arr5[0] == 42);

    // Test 6: Empty array (size 0) should not crash
    int arr6[] = {}; // empty array, but size is 0
    bubbleSortArray(arr6, 0);
    // No assertion needed, just ensure it runs

    // Test 7: Negative numbers
    int arr7[] = {-5, -1, -10, 0, 3};
    bubbleSortArray(arr7, 5);
    assert(arr7[0] == -10 && arr7[1] == -5 && arr7[2] == -1 && arr7[3] == 0 && arr7[4] == 3);

    return 0;
}

#include <cstddef> // for size_t

// Sorts an integer array in ascending order using bubble sort.
// Modifies the input array in-place.
void bubbleSortArray(int arr[], std::size_t size) {
    if (size == 0) return;
    for (std::size_t i = 0; i < size - 1; ++i) {
        for (std::size_t j = 0; j < size - i - 1; ++j) {
            if (arr[j] > arr[j + 1]) {
                // Swap using a temporary variable
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

// The bubble sort algorithm repeatedly steps through the list, compares adjacent elements, and swaps them if they are in the wrong order. This pass is repeated until no swaps are needed, meaning the list is sorted. The standard implementation uses two nested loops: the outer loop runs `size-1` times, and the inner loop compares adjacent elements up to the unsorted portion (which shrinks each pass). The algorithm is stable (equal elements retain relative order) and works in-place.
//
// Edge cases: 
// - Empty array (`size == 0`): The function should do nothing. The loops will not execute because the outer loop condition `i < size-1` becomes `0 < -1` which is false.
// - Array with one element: Already sorted; outer loop runs `0` times.
// - Duplicates: The comparison uses `>` (strictly greater), so equal elements are not swapped, preserving stability.
//
// Time complexity: Best-case (already sorted) is `O(n)` if we add an early-exit flag, but the basic version is `O(n^2)` in all cases. For a teaching task, we can use the basic version. Space complexity: `O(1)` auxiliary space, since only a temporary variable for swapping is used.
