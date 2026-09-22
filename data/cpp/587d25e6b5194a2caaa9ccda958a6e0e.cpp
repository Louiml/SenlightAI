// Write a C++ function named `binarySearchIndex` that takes a sorted array of integers (in non-decreasing order), its size, and a target value, and returns the index of the target if it exists in the array, or -1 if it does not. The function must implement the standard binary search algorithm iteratively (not recursively) and must use `const` correctness where appropriate (i.e., the array parameter should be `const int[]` or `const int*`). The array will contain at least one element. Test the function with several cases including: target at the first position, target at the last position, target in the middle, target not present (smaller than first, larger than last, and between values), and a single-element array.
The solution uses the classic iterative binary search. We maintain two pointers: `left` (start) and `right` (end), initially set to `0` and `size-1`. In a while loop, we compute the middle index as `mid = left + (right - left) / 2` (this avoids potential overflow compared to `(left+right)/2`). If the element at `mid` equals the target, we return `mid`. If the target is less than the middle element, we narrow the search to the left half by setting `right = mid - 1`; otherwise, we narrow to the right half by setting `left = mid + 1`. The loop continues while `left <= right`. If we exit the loop without a match, the target is not present, and we return -1. Edge cases: for a single-element array, the loop executes once and either returns 0 or -1. If the target is smaller than the smallest element or larger than the largest, the loop will shrink the range until left > right and return -1. Time complexity is O(log n) because the search space halves each iteration. Space complexity is O(1) because we only use a few integer variables.
#include <cstddef>

// Returns the index of target in a sorted array arr of given size, or -1 if not found.
int binarySearchIndex(const int arr[], std::size_t size, int target) {
    std::size_t left = 0;
    std::size_t right = size - 1;

    while (left <= right) {
        std::size_t mid = left + (right - left) / 2;

        if (arr[mid] == target) {
            return static_cast<int>(mid);
        } else if (arr[mid] > target) {
            right = mid - 1;
        } else {
            left = mid + 1;
        }
    }

    return -1;
}
#include <cassert>

int main() {
    int arr1[] = {1, 2, 3, 4, 5};
    assert(binarySearchIndex(arr1, 5, 5) == 4); // last element
    assert(binarySearchIndex(arr1, 5, 1) == 0); // first element
    assert(binarySearchIndex(arr1, 5, 3) == 2); // middle element
    assert(binarySearchIndex(arr1, 5, 0) == -1); // smaller than min
    assert(binarySearchIndex(arr1, 5, 6) == -1); // larger than max
    assert(binarySearchIndex(arr1, 5, 2) == 1);  // between values

    int arr2[] = {7};
    assert(binarySearchIndex(arr2, 1, 7) == 0);  // single element present
    assert(binarySearchIndex(arr2, 1, 8) == -1); // single element not present

    int arr3[] = {-10, -5, 0, 3, 8};
    assert(binarySearchIndex(arr3, 5, -5) == 1); // negative target
    assert(binarySearchIndex(arr3, 5, 4) == -1); // between values, not present

    return 0;
}
