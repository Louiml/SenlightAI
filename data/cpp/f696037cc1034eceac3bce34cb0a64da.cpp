Write a C++ function `selectionSort(int arr[], int size)` that sorts an array of integers in ascending order using the selection sort algorithm. The function should modify the array in place and return `void`. The input array may contain duplicate values, negative numbers, and any size from 1 upward. The function must not use any standard library sorting functions (e.g., `std::sort`), and must handle edge cases such as an empty array (size 0) or a single-element array gracefully without errors. The implementation should be self-contained, portable, and use only standard C++ headers.

#include <cassert>

int main() {
    // Test 1: Basic unsorted array
    int arr1[] = {5, 2, 9, 1, 5, 6};
    selectionSort(arr1, 6);
    assert(arr1[0] == 1);
    assert(arr1[1] == 2);
    assert(arr1[2] == 5);
    assert(arr1[3] == 5);
    assert(arr1[4] == 6);
    assert(arr1[5] == 9);

    // Test 2: Already sorted array
    int arr2[] = {1, 2, 3, 4};
    selectionSort(arr2, 4);
    for (int i = 0; i < 4; ++i) {
        assert(arr2[i] == i + 1);
    }

    // Test 3: Reverse sorted array
    int arr3[] = {10, 8, 6, 4, 2};
    selectionSort(arr3, 5);
    assert(arr3[0] == 2);
    assert(arr3[1] == 4);
    assert(arr3[2] == 6);
    assert(arr3[3] == 8);
    assert(arr3[4] == 10);

    // Test 4: Single element array
    int arr4[] = {42};
    selectionSort(arr4, 1);
    assert(arr4[0] == 42);

    // Test 5: Empty array (size 0) - should not crash
    int arr5[] = {};
    selectionSort(arr5, 0);

    // Test 6: All equal elements
    int arr6[] = {7, 7, 7, 7};
    selectionSort(arr6, 4);
    for (int i = 0; i < 4; ++i) {
        assert(arr6[i] == 7);
    }

    // Test 7: Negative and positive numbers
    int arr7[] = {-3, 5, -1, 0, 2};
    selectionSort(arr7, 5);
    assert(arr7[0] == -3);
    assert(arr7[1] == -1);
    assert(arr7[2] == 0);
    assert(arr7[3] == 2);
    assert(arr7[4] == 5);

    return 0;
}

#include <algorithm>  // for std::swap (optional, but convenient)

// Sorts an array of integers in ascending order using selection sort.
// Takes an array and its size. Modifies the array in place. Returns void.
void selectionSort(int arr[], int size) {
    // Outer loop: each iteration places the smallest remaining element at position i.
    for (int i = 0; i < size - 1; ++i) {
        int minIndex = i;  // Assume the current position holds the smallest element.

        // Inner loop: scan the unsorted part to find the actual minimum.
        for (int j = i + 1; j < size; ++j) {
            if (arr[j] < arr[minIndex]) {
                minIndex = j;  // Update minIndex if a smaller element is found.
            }
        }

        // If a smaller element was found elsewhere, swap it into place.
        if (minIndex != i) {
            std::swap(arr[i], arr[minIndex]);
        }
    }
}

// Selection sort works by repeatedly finding the minimum element from the unsorted part of the array and swapping it with the first element of that unsorted part. The algorithm maintains two subarrays: a sorted prefix (initially empty) and an unsorted suffix. For each position `i` from 0 to `size-2`, we scan the subarray from `i+1` to `size-1` to find the index `minIndex` of the smallest element. If `minIndex` is not equal to `i`, we swap `arr[i]` with `arr[minIndex]`. This ensures that after each pass, the smallest remaining element is placed at position `i`, and the sorted prefix grows by one. For an empty array or size 0, the loop simply does not execute. For size 1, the outer loop runs from 0 to -1, so it also does nothing, leaving the array unchanged—both are valid no-op cases. The algorithm is stable only if we choose the first occurrence of the minimum when duplicates exist (which we naturally do by scanning left to right and only updating `minIndex` when a strictly smaller element is found). Time complexity is always \(O(n^2)\) for comparisons (even if already sorted, because the inner scan does not short-circuit) and \(O(1)\) auxiliary space (only a few local variables), with at most \(n-1\) swaps. Edge case: if the array contains negative numbers, the comparison `arr[j] < arr[minIndex]` works correctly with standard integer ordering. Duplicates are handled by leaving the earlier duplicate in place unless a strictly smaller value appears later.
