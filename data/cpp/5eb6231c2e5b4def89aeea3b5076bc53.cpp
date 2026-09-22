Write a C++ function named `recursiveBubbleSort` that takes an array of integers and its size as parameters and sorts the array in ascending order using a recursive implementation of the bubble sort algorithm. The function must modify the array in place and return `void`. The recursion should stop early if the array becomes sorted before reaching the base case of size 1, avoiding unnecessary recursive calls. Also write a separate helper function `isSorted` that checks whether a given integer array is sorted in non-decreasing order, but for this task, only the sorting function is required to be implemented as a free function. The input array may contain negative numbers, zeros, and duplicates. The function should handle an empty array (size 0) gracefully by doing nothing.

// The recursive bubble sort works by performing one full pass over the array from index 0 to n-2, swapping adjacent elements that are out of order. After one pass, the largest element "bubbles up" to the last position. The recursion then sorts the first n-1 elements, since the last element is already in its correct final position. The base case is when n is 0 or 1, in which case the array (or subarray) is already sorted and we return. An important optimization is to track a counter of swaps performed during the pass; if no swaps occur, the array is already sorted and we can return immediately without further recursion. Edge cases include an empty array (n=0) where nothing should happen, a single-element array (n=1) which is trivially sorted, and arrays already sorted or containing duplicates where the early-exit condition triggers. Time complexity is O(n^2) in the worst and average case, but early exit improves best case to O(n) when the array is already sorted. Space complexity is O(n) due to recursion call stack depth.

#include <cstddef>

// Recursively sorts an integer array in ascending order using bubble sort.
// The array is modified in place. If size is 0 or 1, no action is taken.
void recursiveBubbleSort(int arr[], size_t n) {
    // Base case: empty or single-element array is already sorted.
    if (n <= 1) {
        return;
    }
    
    bool swapped = false;
    // One pass over the unsorted portion.
    for (size_t i = 0; i + 1 < n; ++i) {
        if (arr[i] > arr[i + 1]) {
            // Swap adjacent elements.
            int temp = arr[i];
            arr[i] = arr[i + 1];
            arr[i + 1] = temp;
            swapped = true;
        }
    }
    
    // If no swaps occurred, the entire array is already sorted.
    if (!swapped) {
        return;
    }
    
    // The last element is fixed; recurse on the remaining n-1 elements.
    recursiveBubbleSort(arr, n - 1);
}

#include <cassert>

int main() {
    // Test 1: General unsorted array.
    int arr1[] = {22, 4, 55, 5, 90, 8, 432, 4, 1, 0, 11};
    recursiveBubbleSort(arr1, 11);
    int expected1[] = {0, 1, 4, 4, 5, 8, 11, 22, 55, 90, 432};
    for (int i = 0; i < 11; ++i) {
        assert(arr1[i] == expected1[i]);
    }
    
    // Test 2: Already sorted array (early exit).
    int arr2[] = {1, 2, 3, 4, 5};
    recursiveBubbleSort(arr2, 5);
    int expected2[] = {1, 2, 3, 4, 5};
    for (int i = 0; i < 5; ++i) {
        assert(arr2[i] == expected2[i]);
    }
    
    // Test 3: Reverse sorted array.
    int arr3[] = {5, 4, 3, 2, 1};
    recursiveBubbleSort(arr3, 5);
    int expected3[] = {1, 2, 3, 4, 5};
    for (int i = 0; i < 5; ++i) {
        assert(arr3[i] == expected3[i]);
    }
    
    // Test 4: Array with negative numbers and duplicates.
    int arr4[] = {-3, 0, -7, 5, -3, 2};
    recursiveBubbleSort(arr4, 6);
    int expected4[] = {-7, -3, -3, 0, 2, 5};
    for (int i = 0; i < 6; ++i) {
        assert(arr4[i] == expected4[i]);
    }
    
    // Test 5: Single element.
    int arr5[] = {42};
    recursiveBubbleSort(arr5, 1);
    assert(arr5[0] == 42);
    
    // Test 6: Empty array (size 0) — should do nothing.
    int arr6[] = {};  // zero-length array
    recursiveBubbleSort(arr6, 0);  // no crash
    
    // Test 7: Array with two elements already in order.
    int arr7[] = {10, 20};
    recursiveBubbleSort(arr7, 2);
    assert(arr7[0] == 10 && arr7[1] == 20);
    
    // Test 8: Array with two elements out of order.
    int arr8[] = {20, 10};
    recursiveBubbleSort(arr8, 2);
    assert(arr8[0] == 10 && arr8[1] == 20);
    
    // Test 9: All identical elements.
    int arr9[] = {7, 7, 7, 7};
    recursiveBubbleSort(arr9, 4);
    for (int i = 0; i < 4; ++i) {
        assert(arr9[i] == 7);
    }
    
    // Test 10: Larger array with mixed values.
    int arr10[] = {100, -5, 0, 42, 3, 3, -100, 50};
    recursiveBubbleSort(arr10, 8);
    int expected10[] = {-100, -5, 0, 3, 3, 42, 50, 100};
    for (int i = 0; i < 8; ++i) {
        assert(arr10[i] == expected10[i]);
    }
    
    return 0;
}
