Write a C++ function named `selectionSortDescending` that sorts an array of integers in **descending** order using the **selection sort** algorithm. The function must take a non-empty array of integers and its size as parameters, modify the array in place, and return nothing. You may use helper functions if needed, but the main sorting logic must be selection sort. The function should handle arrays with duplicates, negative numbers, and all equal values correctly. Do not use any standard library sorting functions (e.g., `std::sort`). Your implementation must be self-contained and include appropriate `const` correctness only where applicable (the array itself is mutated, so the parameter should not be `const`).

// The selection sort algorithm selects the largest (or smallest) element in the unsorted portion and swaps it with the element at the current position. For descending order, repeatedly find the maximum element in the remaining unsorted subarray from index `i` to `n-1`, then swap it with the element at index `i`. This partitions the array: the prefix `[0, i-1]` is sorted descending, and the suffix `[i, n-1]` is unsorted. The algorithm runs in O(n²) time regardless of input because it always scans the entire unsorted portion to find the maximum. Space complexity is O(1) since only a few temporary variables are used. Edge cases: if the array has one element, no swaps occur; if all elements are equal, no swaps occur; duplicates are handled by the `>` condition when comparing, ensuring stable behavior in terms of element values (though not order, which is irrelevant for integers). Time complexity is O(n²) and space is O(1).

#include <utility>  // for std::swap

// Sorts an integer array in descending order using selection sort.
void selectionSortDescending(int arr[], int n) {
    for (int i = 0; i < n - 1; ++i) {
        // Find the index of the maximum element in the unsorted part.
        int maxIndex = i;
        for (int j = i + 1; j < n; ++j) {
            if (arr[j] > arr[maxIndex]) {
                maxIndex = j;
            }
        }
        // Swap the found maximum with the first unsorted position.
        if (maxIndex != i) {
            std::swap(arr[i], arr[maxIndex]);
        }
    }
}

#include <cassert>
#include <iostream>

// Forward declaration of the solution function (as defined above).
void selectionSortDescending(int arr[], int n);

int main() {
    // Test 1: Normal case with mixed values.
    int arr1[] = {3, 1, 4, 1, 5, 9, 2, 6};
    int n1 = sizeof(arr1) / sizeof(arr1[0]);
    selectionSortDescending(arr1, n1);
    int expected1[] = {9, 6, 5, 4, 3, 2, 1, 1};
    for (int i = 0; i < n1; ++i) assert(arr1[i] == expected1[i]);

    // Test 2: Already sorted descending.
    int arr2[] = {10, 7, 4, 1};
    int n2 = sizeof(arr2) / sizeof(arr2[0]);
    selectionSortDescending(arr2, n2);
    int expected2[] = {10, 7, 4, 1};
    for (int i = 0; i < n2; ++i) assert(arr2[i] == expected2[i]);

    // Test 3: Single element.
    int arr3[] = {42};
    int n3 = sizeof(arr3) / sizeof(arr3[0]);
    selectionSortDescending(arr3, n3);
    assert(arr3[0] == 42);

    // Test 4: All identical elements.
    int arr4[] = {5, 5, 5, 5};
    int n4 = sizeof(arr4) / sizeof(arr4[0]);
    selectionSortDescending(arr4, n4);
    int expected4[] = {5, 5, 5, 5};
    for (int i = 0; i < n4; ++i) assert(arr4[i] == expected4[i]);

    // Test 5: Negative numbers and zeros.
    int arr5[] = {-3, 0, -9, 2, -1, 0};
    int n5 = sizeof(arr5) / sizeof(arr5[0]);
    selectionSortDescending(arr5, n5);
    int expected5[] = {2, 0, 0, -1, -3, -9};
    for (int i = 0; i < n5; ++i) assert(arr5[i] == expected5[i]);

    // Test 6: Reverse of sorted ascending (worst case for selection, but still works).
    int arr6[] = {1, 2, 3, 4, 5};
    int n6 = sizeof(arr6) / sizeof(arr6[0]);
    selectionSortDescending(arr6, n6);
    int expected6[] = {5, 4, 3, 2, 1};
    for (int i = 0; i < n6; ++i) assert(arr6[i] == expected6[i]);

    std::cout << "All tests passed." << std::endl;
    return 0;
}
