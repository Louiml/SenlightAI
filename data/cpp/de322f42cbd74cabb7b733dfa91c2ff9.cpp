Write a C++ function named `binarySearchSorted` that takes a dynamically allocated array of integers (represented as a raw pointer), its size, and a search key, and returns the index of the search key if found after sorting the array in ascending order; otherwise, it should return `-1`. The function must first sort the array in non-decreasing order using any simple sorting algorithm (e.g., bubble sort), then perform a binary search on the sorted array. The function must handle edge cases such as an empty array (where it should return `-1`), duplicate search keys (returning any valid index), and keys smaller than the smallest or larger than the largest element. The input array is modified in-place as part of sorting, and the function must not use any standard library sorting or binary search functions—implement both algorithms manually.

// The solution has two clear phases: sorting and binary search. For sorting, a simple bubble sort (nested loops) is adequate since the task does not demand optimal sorting performance; it operates in O(n²) time and O(1) extra space. After sorting, we set `left = 0`, `right = size - 1`, and repeatedly compute the middle index as `mid = left + (right - left) / 2` (avoids overflow). If the middle element equals the key, return that index. If the middle element is less than the key, narrow to the right half (`left = mid + 1`); otherwise, narrow to the left half (`right = mid - 1`). If the loop exits without finding the key, return `-1`. Important edge cases: empty array (size 0) → immediately return `-1`; duplicates—the first match found is acceptable; key outside range—binary search naturally exits with `-1`. Time complexity is O(n²) for sorting plus O(log n) for searching, dominated by sorting; space complexity is O(1) auxiliary.

#include <cstddef>

// Sorts the array in ascending order using bubble sort, then binary searches for the key.
// Returns the index of the key if found, otherwise -1. Modifies the array in-place.
int binarySearchSorted(int* arr, std::size_t size, int key) {
    if (size == 0 || arr == nullptr) {
        return -1;
    }

    // Bubble sort in ascending order
    for (std::size_t i = 0; i < size - 1; ++i) {
        for (std::size_t j = 0; j < size - 1 - i; ++j) {
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }

    // Binary search
    std::size_t left = 0;
    std::size_t right = size - 1;
    while (left <= right) {
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

int binarySearchSorted(int* arr, std::size_t size, int key); // Declaration

int main() {
    // Test 1: Basic found case
    int arr1[] = {5, 2, 8, 1, 9};
    std::size_t n1 = 5;
    assert(binarySearchSorted(arr1, n1, 8) == 3); // sorted: 1,2,5,8,9

    // Test 2: Key not present
    int arr2[] = {3, 1, 4};
    std::size_t n2 = 3;
    assert(binarySearchSorted(arr2, n2, 2) == -1);

    // Test 3: Empty array
    int* arr3 = nullptr;
    assert(binarySearchSorted(arr3, 0, 5) == -1);

    // Test 4: Single element found
    int arr4[] = {42};
    assert(binarySearchSorted(arr4, 1, 42) == 0);

    // Test 5: Duplicate key - any valid index acceptable
    int arr5[] = {7, 7, 7};
    int result5 = binarySearchSorted(arr5, 3, 7);
    assert(result5 >= 0 && result5 <= 2);

    // Test 6: Key smaller than all elements
    int arr6[] = {10, 20, 30};
    assert(binarySearchSorted(arr6, 3, 5) == -1);

    // Test 7: Key larger than all elements
    int arr7[] = {1, 3, 5};
    assert(binarySearchSorted(arr7, 3, 9) == -1);

    // Test 8: Already sorted array
    int arr8[] = {1, 2, 3, 4, 5};
    assert(binarySearchSorted(arr8, 5, 4) == 3);

    // Test 9: Negative numbers
    int arr9[] = {-3, -1, -7, 0};
    assert(binarySearchSorted(arr9, 4, -7) == 0); // sorted: -7,-3,-1,0

    return 0;
}
