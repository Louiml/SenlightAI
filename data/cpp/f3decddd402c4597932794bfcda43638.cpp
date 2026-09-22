Write a C++ function that performs a recursive binary search on a sorted array of integers. The function must take the array, the left index, the right index, and the target value as parameters, and return the index of the target if found, or -1 if not present. The input array is guaranteed to be sorted in non-decreasing order and may contain duplicate values; in the case of duplicates, return any valid index of the target. The array size can be zero, in which case the function should immediately return -1. The function must be const-correct (the array parameter should be a pointer to const int) and must not modify the array. Additionally, provide a test program that verifies correct behavior on sorted arrays including odd/even lengths, duplicates, a target smaller than all elements, a target larger than all elements, an empty array, and a single-element array.

// The solution uses a classic recursive binary search. The base case occurs when the left index exceeds the right index, meaning the search interval is empty, so return -1. Otherwise, compute the middle index as `left + (right - left) / 2` to avoid potential integer overflow (though for small examples `(left+right)/2` is fine, `left + (right-left)/2` is safer). Compare the middle element with the target: if equal, return the middle index; if the middle element is less than the target, recurse on the right half (`mid+1` to `right`); if greater, recurse on the left half (`left` to `mid-1`). Edge cases include an empty array (`right < left` immediately returns -1), a single-element array (the mid calculation works and compares correctly), and duplicate targets (the algorithm may return any occurrence, which satisfies the requirement). Time complexity is O(log n) for the average and worst case, because each recursion halves the search space. Space complexity is O(log n) due to the recursion stack depth, though iterative binary search would be O(1) auxiliary space; here the recursion overhead is acceptable.

#include <vector>

// Recursive binary search on a sorted array.
// Returns the index of the target if found, otherwise -1.
// The array is not modified; const correctness is applied.
int binarySearchRecursive(const int arr[], int left, int right, int target) {
    // Base case: empty search interval
    if (left > right) {
        return -1;
    }

    // Compute middle to avoid overflow
    int mid = left + (right - left) / 2;

    if (arr[mid] == target) {
        return mid;
    } else if (arr[mid] < target) {
        // Search the right half
        return binarySearchRecursive(arr, mid + 1, right, target);
    } else {
        // Search the left half
        return binarySearchRecursive(arr, left, mid - 1, target);
    }
}

#include <cassert>
#include <iostream>

// Forward declaration of the solution function (assume it's in the same file)
int binarySearchRecursive(const int arr[], int left, int right, int target);

int main() {
    // Test 1: Normal sorted array with odd length
    int arr1[] = {1, 3, 5, 7, 9};
    assert(binarySearchRecursive(arr1, 0, 4, 5) == 2);
    assert(binarySearchRecursive(arr1, 0, 4, 7) == 3);
    assert(binarySearchRecursive(arr1, 0, 4, 1) == 0);
    assert(binarySearchRecursive(arr1, 0, 4, 9) == 4);

    // Test 2: Normal sorted array with even length
    int arr2[] = {2, 4, 6, 8};
    assert(binarySearchRecursive(arr2, 0, 3, 6) == 2);
    assert(binarySearchRecursive(arr2, 0, 3, 2) == 0);
    assert(binarySearchRecursive(arr2, 0, 3, 8) == 3);

    // Test 3: Duplicate values – any valid index is acceptable
    int arr3[] = {1, 2, 2, 2, 3};
    int result = binarySearchRecursive(arr3, 0, 4, 2);
    assert(result >= 1 && result <= 3);

    // Test 4: Target smaller than all elements
    int arr4[] = {10, 20, 30};
    assert(binarySearchRecursive(arr4, 0, 2, 5) == -1);

    // Test 5: Target larger than all elements
    int arr5[] = {10, 20, 30};
    assert(binarySearchRecursive(arr5, 0, 2, 35) == -1);

    // Test 6: Empty array (left > right)
    assert(binarySearchRecursive(nullptr, 0, -1, 5) == -1);

    // Test 7: Single-element array
    int arr7[] = {42};
    assert(binarySearchRecursive(arr7, 0, 0, 42) == 0);
    assert(binarySearchRecursive(arr7, 0, 0, 43) == -1);

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
