Write a C++ function that implements a recursive binary search to locate a given integer key within a sorted array of integers. The function should accept the array, its left and right boundary indices (inclusive), and the target key, returning the index of the key if found, or -1 if not present. The array is guaranteed to be sorted in non-decreasing order and non-empty. The function must be `const`-correct, meaning it should accept the array as a `const` pointer/reference and not modify it.

The solution uses the classic recursive binary search algorithm. The base case occurs when the left index exceeds the right index, meaning the element is not present, so return -1. Otherwise, compute the middle index as `left + (right - left) / 2` to avoid potential integer overflow. Compare the middle element with the target: if equal, return the middle index; if the middle element is less than the target, recursively search the right half (from `mid+1` to `right`); if greater, recursively search the left half (from `left` to `mid-1`). The recursion ensures `O(log n)` time complexity because each step halves the search space, and `O(log n)` auxiliary space due to the call stack depth. Edge cases include searching for the first or last element, duplicate values (any matching index is acceptable), and the key being smaller than all or larger than all elements.

#include <vector>

// Recursive binary search on a sorted integer array.
// Returns the index of the key if found, otherwise -1.
int recursiveBinarySearch(const int arr[], int left, int right, int key) {
    // Base case: element not found
    if (left > right) {
        return -1;
    }
    
    // Compute mid to avoid overflow
    int mid = left + (right - left) / 2;
    
    // Found the key
    if (arr[mid] == key) {
        return mid;
    }
    
    // Search right half
    if (arr[mid] < key) {
        return recursiveBinarySearch(arr, mid + 1, right, key);
    }
    
    // Search left half
    return recursiveBinarySearch(arr, left, mid - 1, key);
}

#include <cassert>
#include <iostream>

int recursiveBinarySearch(const int arr[], int left, int right, int key);

int main() {
    // Test 1: Sorted array with distinct elements
    int arr1[] = {8, 11, 23, 34, 39, 46, 68, 71, 86};
    assert(recursiveBinarySearch(arr1, 0, 8, 8) == 0);
    assert(recursiveBinarySearch(arr1, 0, 8, 86) == 8);
    assert(recursiveBinarySearch(arr1, 0, 8, 39) == 4);
    assert(recursiveBinarySearch(arr1, 0, 8, 40) == -1);
    assert(recursiveBinarySearch(arr1, 0, 8, 7) == -1);
    assert(recursiveBinarySearch(arr1, 0, 8, 87) == -1);

    // Test 2: Array with duplicate values
    int arr2[] = {1, 3, 3, 3, 5, 7};
    // Any index of 3 is valid; we just check it's not -1
    assert(recursiveBinarySearch(arr2, 0, 5, 3) >= 1);
    assert(recursiveBinarySearch(arr2, 0, 5, 3) <= 3);
    assert(recursiveBinarySearch(arr2, 0, 5, 5) == 4);

    // Test 3: Small array
    int arr3[] = {42};
    assert(recursiveBinarySearch(arr3, 0, 0, 42) == 0);
    assert(recursiveBinarySearch(arr3, 0, 0, 41) == -1);

    // Test 4: Negative numbers
    int arr4[] = {-10, -5, -2, 0, 3};
    assert(recursiveBinarySearch(arr4, 0, 4, -5) == 1);
    assert(recursiveBinarySearch(arr4, 0, 4, 0) == 3);
    assert(recursiveBinarySearch(arr4, 0, 4, -1) == -1);

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
