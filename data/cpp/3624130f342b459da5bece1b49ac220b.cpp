Implement a C++ function named `binarySearchIndex` that, given a sorted array of integers, its length, and a target value, returns the 1-based index of the target if it exists in the array; otherwise, it returns -1. The function must use an iterative binary search algorithm. The array is guaranteed to be sorted in non-decreasing order and has at least one element. The function signature must be `int binarySearchIndex(const int arr[], int length, int target)`, where `length` is the number of elements, and the function must not modify the array. Additionally, the function must handle duplicate values correctly (returning any valid index) and be efficient for large arrays.
The solution uses the classic iterative binary search on a sorted array. We maintain two pointers, `left` starting at index 0 and `right` at index `length-1`. In each iteration, we compute the middle index `mid = left + (right - left) / 2` (using this formula to avoid potential integer overflow with large arrays). Compare the target with `arr[mid]`:
- If `target > arr[mid]`, the target must lie on the right side, so set `left = mid + 1`.
- If `target < arr[mid]`, the target lies on the left, so set `right = mid - 1`.
- If equal, return `mid + 1` (converting to 1-based index as specified).

If the loop ends without finding the target, we return -1. Edge cases include: target is the first or last element, target is smaller than all or larger than all elements (immediately returning -1), and duplicates (we still return the first found mid, which is acceptable). Time complexity is O(log n) because each step halves the search space. Space complexity is O(1) since we only use a few integer variables and do not allocate extra data structures (the input array is treated as read-only).
#include <cstddef> // for size_t, not strictly needed but good practice

// Returns the 1-based index of target in a sorted array arr of given length,
// or -1 if target is not present.
int binarySearchIndex(const int arr[], int length, int target) {
    int left = 0;
    int right = length - 1;

    while (left <= right) {
        int mid = left + (right - left) / 2; // avoid overflow

        if (target > arr[mid]) {
            left = mid + 1;
        } else if (target < arr[mid]) {
            right = mid - 1;
        } else {
            // Found target; return 1-based index.
            return mid + 1;
        }
    }
    return -1; // not found
}
#include <cassert>

int main() {
    int arr1[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    assert(binarySearchIndex(arr1, 10, 5) == 5);
    assert(binarySearchIndex(arr1, 10, 2) == 2);
    assert(binarySearchIndex(arr1, 10, 8) == 8);
    assert(binarySearchIndex(arr1, 10, 8923) == -1);
    assert(binarySearchIndex(arr1, 10, 1) == 1);
    assert(binarySearchIndex(arr1, 10, 10) == 10);

    // Duplicate values
    int arr2[] = {2, 2, 2, 2};
    int idx = binarySearchIndex(arr2, 4, 2);
    assert(idx >= 1 && idx <= 4); // any valid index acceptable

    // Single element
    int arr3[] = {42};
    assert(binarySearchIndex(arr3, 1, 42) == 1);
    assert(binarySearchIndex(arr3, 1, 43) == -1);

    // Negative numbers
    int arr4[] = {-10, -5, 0, 3};
    assert(binarySearchIndex(arr4, 4, -5) == 2);
    assert(binarySearchIndex(arr4, 4, 0) == 3);
    assert(binarySearchIndex(arr4, 4, -1) == -1);
}
