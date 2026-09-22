// Write a C++ function `recursiveInsertionSort` that sorts an array of integers in ascending order using a recursive implementation of insertion sort. The function should take a pointer to the first element of the array and the array size (number of elements) as parameters, and modify the array in-place. The sorting must be done recursively in two layers: one recursive function that processes the subarray from index 0 to n-1, and a second recursive helper function that inserts the last element of that subarray into its correct sorted position within the already-sorted prefix. The input array may contain duplicate values, negative numbers, and need not be initially sorted. The function must work correctly for arrays of size 0 or 1 (no modification needed). The implementation should not use any loops, and must not call any standard sorting functions. The function should have the signature `void recursiveInsertionSort(int* arr, int n)`.

#include <cassert>
#include <iostream>

// Function declaration (matching the solution)
void recursiveInsertionSort(int* arr, int n);

// Helper to check array equality
bool arraysEqual(const int* a, const int* b, int n) {
    for (int i = 0; i < n; ++i) {
        if (a[i] != b[i]) return false;
    }
    return true;
}

int main() {
    // Test 1: basic unsorted array
    int arr1[] = {5, 2, 9, 1, 5, 6};
    int expected1[] = {1, 2, 5, 5, 6, 9};
    recursiveInsertionSort(arr1, 6);
    assert(arraysEqual(arr1, expected1, 6));

    // Test 2: already sorted array
    int arr2[] = {1, 2, 3, 4, 5};
    int expected2[] = {1, 2, 3, 4, 5};
    recursiveInsertionSort(arr2, 5);
    assert(arraysEqual(arr2, expected2, 5));

    // Test 3: reverse sorted array (worst case)
    int arr3[] = {9, 7, 5, 3, 1};
    int expected3[] = {1, 3, 5, 7, 9};
    recursiveInsertionSort(arr3, 5);
    assert(arraysEqual(arr3, expected3, 5));

    // Test 4: array with negative numbers and duplicates
    int arr4[] = {-3, -10, 0, -3, 7, -10};
    int expected4[] = {-10, -10, -3, -3, 0, 7};
    recursiveInsertionSort(arr4, 6);
    assert(arraysEqual(arr4, expected4, 6));

    // Test 5: single element array
    int arr5[] = {42};
    int expected5[] = {42};
    recursiveInsertionSort(arr5, 1);
    assert(arraysEqual(arr5, expected5, 1));

    // Test 6: empty array (size 0) should not crash
    int* arr6 = nullptr;
    recursiveInsertionSort(arr6, 0);
    assert(true); // just verifying no crash

    // Test 7: array with all identical elements
    int arr7[] = {7, 7, 7, 7};
    int expected7[] = {7, 7, 7, 7};
    recursiveInsertionSort(arr7, 4);
    assert(arraysEqual(arr7, expected7, 4));

    std::cout << "All tests passed!" << std::endl;
    return 0;
}

#include <cstddef>

// Recursive helper that inserts 'key' into the sorted subarray arr[0..n-1]
// (the subarray currently has n elements ending at index n-1).
// It shifts elements as needed and places key in its correct position.
static void insertRecursively(int* arr, int n, int key) {
    // Base case: if we've reached the start of the array,
    // the key is the smallest so far, place it at index 0.
    if (n == 0) {
        arr[0] = key;
        return;
    }
    // If key is smaller than the last element of the subarray,
    // shift that element right and recurse on the smaller subarray.
    if (key < arr[n - 1]) {
        arr[n] = arr[n - 1];
        insertRecursively(arr, n - 1, key);
    } else {
        // Key is not smaller, so insert at current position n.
        arr[n] = key;
    }
}

// Recursive insertion sort: sorts arr[0..n-1] in ascending order.
// Uses recursion, no loops, and modifies the array in-place.
void recursiveInsertionSort(int* arr, int n) {
    // Base case: arrays of size 0 or 1 are already sorted.
    if (n <= 1) {
        return;
    }
    // Recursively sort the first n-1 elements.
    recursiveInsertionSort(arr, n - 1);
    // The last element (at index n-1) is the key to insert.
    int key = arr[n - 1];
    // Insert this key into the already-sorted subarray of length n-1.
    insertRecursively(arr, n - 1, key);
}

// The solution follows the classic insertion sort logic but replaces the outer loop with recursion and the inner shifting loop with a recursive insertion helper. The main recursive function processes the first `n-1` elements first (which sorts the prefix), then takes the element at index `n-1` (the last element of the current subarray) and inserts it into the sorted prefix of length `n-1`. The insertion helper function compares the key value with the last element of the prefix; if the key is smaller, it shifts that element one position to the right and recursively calls itself on the shortened prefix (n-1). If the key is not smaller, it places the key at the current position and returns. A base case handles when the prefix becomes empty (n == 0), where the key is placed at the very beginning. Edge cases to consider: an empty array or array of size 1 (the recursion base returns immediately), duplicate values (comparison uses `<`, so equal values are not shifted, preserving stability), and negative numbers are handled naturally. Time complexity is O(n^2) in the worst case (reverse sorted array) and O(n) in the best case (already sorted array), because the insertion helper terminates early when the key is already in place. Space complexity is O(n) due to recursion stack depth, which is typical for recursive implementations.
