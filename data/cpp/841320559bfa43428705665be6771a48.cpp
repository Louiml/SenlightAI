Write a C++ function that takes a sorted array of integers, its size, and a target key, and returns the index of the key if found, or -1 if not found. The function must implement binary search iteratively, handle arrays of any valid size (including empty), and assume the input array is sorted in ascending order. The function should be `const`-correct for the array parameter and work correctly for negative keys, single-element arrays, and keys at the boundaries.

Binary search works by repeatedly dividing the search interval in half. Initialize `left = 0` and `right = size - 1`. While `left <= right`, compute `mid = left + (right - left) / 2` (to avoid integer overflow) and compare `arr[mid]` with the key. If equal, return `mid`. If `arr[mid] < key`, move `left` to `mid + 1`; otherwise, move `right` to `mid - 1`. If the loop exits without finding the key, return -1. Edge cases: an empty array (size 0) should immediately return -1; a single-element array checks that one element; keys smaller than the smallest or larger than the largest element will cause the loop to correctly terminate with -1. Time complexity is \(O(\log n)\) for the number of comparisons, and space complexity is \(O(1)\) auxiliary.

#include <cstddef> // for size_t

// Binary search on a sorted array (ascending order). Returns the index of
// the key if found, otherwise -1. Accepts an empty array (size 0).
int binarySearch(const int arr[], std::size_t size, int key) {
    if (size == 0) {
        return -1;
    }
    std::size_t left = 0;
    std::size_t right = size - 1;
    while (left <= right) {
        // Use left + (right - left) / 2 to avoid potential overflow.
        std::size_t mid = left + (right - left) / 2;
        if (arr[mid] == key) {
            return static_cast<int>(mid);
        } else if (arr[mid] < key) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    return -1;
}

#include <cassert>
#include <cstddef>

int main() {
    int arr1[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::size_t sz1 = sizeof(arr1) / sizeof(arr1[0]);
    assert(binarySearch(arr1, sz1, 7) == 6);
    assert(binarySearch(arr1, sz1, 1) == 0);
    assert(binarySearch(arr1, sz1, 10) == 9);
    assert(binarySearch(arr1, sz1, 0) == -1);
    assert(binarySearch(arr1, sz1, 11) == -1);

    int arr2[] = {-5, -3, 0, 2, 4};
    std::size_t sz2 = sizeof(arr2) / sizeof(arr2[0]);
    assert(binarySearch(arr2, sz2, -3) == 1);
    assert(binarySearch(arr2, sz2, 0) == 2);
    assert(binarySearch(arr2, sz2, -1) == -1);

    int arr3[] = {42};
    std::size_t sz3 = sizeof(arr3) / sizeof(arr3[0]);
    assert(binarySearch(arr3, sz3, 42) == 0);
    assert(binarySearch(arr3, sz3, 43) == -1);

    // Empty array test
    int* arr4 = nullptr;
    std::size_t sz4 = 0;
    assert(binarySearch(arr4, sz4, 5) == -1);
}
