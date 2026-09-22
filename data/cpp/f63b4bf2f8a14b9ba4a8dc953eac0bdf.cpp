/*
Write a C++ function that, given a sorted array of integers (sorted in non-decreasing order), its size, and a key value, returns a `std::pair<int, int>` containing the indices of the first and last occurrence of that key in the array. If the key is not present, return `{-1, -1}`. The function must operate in O(log n) time using a binary-search-based approach. Assume the array is not empty, and duplicate values may exist. The solution should not rely on any external sorting or linear scanning after finding one occurrence.
*/

#include <utility> // for std::pair

// Returns the first and last occurrence indices of key in sorted array arr.
// If key is absent, returns {-1, -1}.
std::pair<int, int> findFirstLastOccurrences(const int arr[], int size, int key) {
    // Binary search for first occurrence
    int first = -1;
    int low = 0;
    int high = size - 1;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (arr[mid] == key) {
            first = mid;
            high = mid - 1; // look for earlier duplicate
        } else if (arr[mid] < key) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    if (first == -1) {
        return {-1, -1}; // key not found
    }

    // Binary search for last occurrence
    int last = -1;
    low = 0;
    high = size - 1;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (arr[mid] == key) {
            last = mid;
            low = mid + 1; // look for later duplicate
        } else if (arr[mid] < key) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    // last cannot be -1 here because we already found at least one occurrence
    return {first, last};
}

#include <cassert>
#include <utility>

int main() {
    int arr1[] = {1, 2, 3, 3, 5};
    assert(findFirstLastOccurrences(arr1, 5, 3) == std::make_pair(2, 3));
    assert(findFirstLastOccurrences(arr1, 5, 2) == std::make_pair(1, 1));
    assert(findFirstLastOccurrences(arr1, 5, 6) == std::make_pair(-1, -1));
    assert(findFirstLastOccurrences(arr1, 5, 0) == std::make_pair(-1, -1));

    int arr2[] = {7};
    assert(findFirstLastOccurrences(arr2, 1, 7) == std::make_pair(0, 0));
    assert(findFirstLastOccurrences(arr2, 1, 8) == std::make_pair(-1, -1));

    int arr3[] = {2, 2, 2, 2, 2};
    assert(findFirstLastOccurrences(arr3, 5, 2) == std::make_pair(0, 4));

    int arr4[] = {-5, -5, -3, 0, 0, 0, 4};
    assert(findFirstLastOccurrences(arr4, 7, 0) == std::make_pair(3, 5));
    assert(findFirstLastOccurrences(arr4, 7, -3) == std::make_pair(2, 2));

    return 0;
}

// The solution uses two separate binary searches. For the first occurrence, we modify standard binary search: when we find the key, we record the index and continue searching in the left half (decrease `end` to `mid-1`) to see if an earlier duplicate exists. For the last occurrence, when we find the key, we record the index and continue searching in the right half (increase `start` to `mid+1`). Edge cases include: key smaller than all elements (returns `{-1,-1}`), key larger than all elements, key present exactly once (both indices equal), and array of size 1. Complexity: each binary search runs in O(log n), so total O(log n) time and O(1) auxiliary space. Using `std::pair<int,int>` is natural and avoids outputting directly.
