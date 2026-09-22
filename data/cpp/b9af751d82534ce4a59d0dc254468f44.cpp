/*
Write a C++ function `int findTargetIndex(const int arr[], int size, int target)` that performs a recursive binary search on a sorted array of integers and returns the index of the target if found, or `-1` if not found. The function must use recursion only (no loops) and must not modify the input array. Handle edge cases such as an empty array, a single-element array, and targets smaller than the smallest or larger than the largest element. The function signature must be const-correct, and you should implement a helper recursive function if needed (but the final exposed function should have this exact signature). In your solution, include only the function definition and necessary headers; do not include `main`.
*/
#include <vector> // Not needed, but included for completeness if using vector; here we use raw array.

// Helper function for recursive binary search.
int binarySearchHelper(const int arr[], int low, int high, int target) {
    // Base case: search space is empty.
    if (low > high) {
        return -1;
    }
    // Compute mid to avoid overflow.
    int mid = low + (high - low) / 2;
    // If found, return index.
    if (arr[mid] == target) {
        return mid;
    }
    // If target is greater, search right half; otherwise search left half.
    if (arr[mid] < target) {
        return binarySearchHelper(arr, mid + 1, high, target);
    } else {
        return binarySearchHelper(arr, low, mid - 1, target);
    }
}

// Public function that matches the task specification.
int findTargetIndex(const int arr[], int size, int target) {
    // Edge case: empty array.
    if (size <= 0) {
        return -1;
    }
    // Call helper with full range.
    return binarySearchHelper(arr, 0, size - 1, target);
}
#include <cassert>

// The solution function is declared above (assume it is present).
// This is the test main function.
int main() {
    int arr1[] = {10, 20, 30, 40, 50, 60, 70, 80};
    assert(findTargetIndex(arr1, 8, 70) == 6); // target at index 6
    assert(findTargetIndex(arr1, 8, 10) == 0); // first element
    assert(findTargetIndex(arr1, 8, 80) == 7); // last element
    assert(findTargetIndex(arr1, 8, 45) == -1); // not present
    assert(findTargetIndex(arr1, 8, 5) == -1); // smaller than min
    assert(findTargetIndex(arr1, 8, 100) == -1); // larger than max

    int arr2[] = {5}; // single element
    assert(findTargetIndex(arr2, 1, 5) == 0);
    assert(findTargetIndex(arr2, 1, 3) == -1);

    int arr3[] = {}; // empty array (size 0)
    assert(findTargetIndex(arr3, 0, 10) == -1);

    int arr4[] = {1, 3, 3, 3, 5}; // duplicates
    int idx = findTargetIndex(arr4, 5, 3);
    assert(idx >= 1 && idx <= 3); // any occurrence is valid

    // Additional check: const-correctness (passing const array)
    const int arr5[] = {2, 4, 6, 8};
    assert(findTargetIndex(arr5, 4, 6) == 2);
    assert(findTargetIndex(arr5, 4, 7) == -1);
}
// The solution uses a recursive binary search algorithm. The main function `findTargetIndex` simply calls a private helper that takes the array, the low and high indices, and the target. The helper computes the middle index using `mid = low + (high - low) / 2` to avoid integer overflow. The base case is when `low > high`, meaning the search space is exhausted, and we return `-1`. Otherwise, we compare the middle element to the target. If equal, return `mid`. If the target is greater than the middle element, recurse on the right half (`mid+1` to `high`); otherwise recurse on the left half (`low` to `mid-1`). Edge cases include an empty array where `size=0` – we should immediately return `-1`. A single-element array works naturally because if the element matches, `mid==low==high` and we return it; if not, the next call will have `low > high` and return `-1`. The array is assumed to be sorted in non-decreasing order; duplicates are handled normally (any matching index is acceptable). Time complexity is O(log n) for a balanced recursion tree, and space complexity is O(log n) due to recursion stack depth.
