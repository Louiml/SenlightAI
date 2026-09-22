/*
Write a C++ function named `bubbleSortAscending` that takes a C-style array of integers and its size as parameters, sorts the array in ascending order using the bubble sort algorithm, and returns `void`. The function must modify the array in place and must not use any standard sorting functions or extra arrays. Additionally, apply `const` correctness where appropriate (e.g., the size parameter should be `const int size`). The function should handle arrays of any positive size, including size 1 (already sorted). You are not required to write a `main` function in the solution, but you will provide test code separately.
*/
#include <cstddef>  // for size_t, optional but good practice

// Sorts the given array in ascending order using bubble sort.
// The array is modified in place. The size parameter is const to reflect
// that the function does not change the number of elements.
void bubbleSortAscending(int arr[], const int size) {
    // For each pass, we reduce the effective range by one
    // because the largest element bubbles to the end.
    for (int i = 0; i < size - 1; ++i) {
        bool swapped = false;  // optimization: break early if no swap
        for (int j = 0; j < size - 1 - i; ++j) {
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
                swapped = true;
            }
        }
        // If no swaps occurred, the array is already sorted
        if (!swapped) {
            break;
        }
    }
}
#include <cassert>

// Forward declaration of the function to test
void bubbleSortAscending(int arr[], const int size);

int main() {
    // Test 1: Basic unsorted array
    int arr1[] = {5, 2, 9, 1, 5, 6};
    bubbleSortAscending(arr1, 6);
    assert(arr1[0] == 1 && arr1[1] == 2 && arr1[2] == 5 && arr1[3] == 5 && arr1[4] == 6 && arr1[5] == 9);

    // Test 2: Already sorted array
    int arr2[] = {1, 2, 3, 4};
    bubbleSortAscending(arr2, 4);
    assert(arr2[0] == 1 && arr2[1] == 2 && arr2[2] == 3 && arr2[3] == 4);

    // Test 3: Reverse sorted array
    int arr3[] = {10, 7, 4, 1, 0};
    bubbleSortAscending(arr3, 5);
    assert(arr3[0] == 0 && arr3[1] == 1 && arr3[2] == 4 && arr3[3] == 7 && arr3[4] == 10);

    // Test 4: Array with duplicates
    int arr4[] = {3, 3, 3};
    bubbleSortAscending(arr4, 3);
    assert(arr4[0] == 3 && arr4[1] == 3 && arr4[2] == 3);

    // Test 5: Single element array
    int arr5[] = {42};
    bubbleSortAscending(arr5, 1);
    assert(arr5[0] == 42);

    // Test 6: Array with negative numbers
    int arr6[] = {-5, -2, -10, 0, 7};
    bubbleSortAscending(arr6, 5);
    assert(arr6[0] == -10 && arr6[1] == -5 && arr6[2] == -2 && arr6[3] == 0 && arr6[4] == 7);

    // Test 7: Size zero (should not crash)
    int arr7[0];  // Empty array (size 0)
    bubbleSortAscending(arr7, 0);
    // No elements to check, just ensure no crash

    return 0;
}
// The solution uses nested loops: the outer loop runs from 0 to `size-2` (i.e., `size-1` passes), and the inner loop runs from 0 to `size-2` (comparing adjacent pairs). On each pass, if `arr[j] > arr[j+1]`, swap them. This pushes the largest unsorted element to the end of the unsorted section. After `size-1` passes, the entire array is sorted. Edge cases: if `size` is 0 or 1, the loops do not execute and the array remains as is (which is correct). For duplicate values, the condition `>` ensures stability (relative order of equal elements is preserved, though not required). Time complexity is \(O(n^2)\) for all cases (best, average, worst) because the inner loop always runs `size-1` times per pass; space complexity is \(O(1)\) (only a temporary variable for swapping). No special handling is needed for negative numbers; comparisons work naturally.
