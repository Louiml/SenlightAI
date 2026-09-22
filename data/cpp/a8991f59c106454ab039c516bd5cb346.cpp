Write a C++ function named `findElement` that performs a recursive binary search on a sorted integer array. The function must accept a `const int[]` array, its size, and a query value, and return the index of the query if found, or -1 if not present. The array is guaranteed to be sorted in ascending order and may contain duplicate values—in that case, any valid index of the query may be returned. The function must not modify the array, must handle empty arrays (size 0) gracefully, and must avoid integer overflow when computing the middle index.
// The solution uses the classic recursive binary search divide-and-conquer approach. The search space is defined by two indices, `start` and `end`, initially `0` and `size - 1`. The base case occurs when `start > end`, meaning the subarray is empty and the query is not present—return -1. Otherwise, compute the middle index as `start + (end - start) / 2` (this avoids overflow compared to `(start + end) / 2`). If the query equals the middle element, return the middle index (for duplicates, it may not be the first occurrence, but that is acceptable per the task). If the query is greater, recurse on the right half with `start = middle + 1`; if smaller, recurse on the left half with `end = middle - 1`. Edge cases include an empty array (size 0) that immediately returns -1, and a query smaller than the first or larger than the last element, which will quickly reduce the search space and hit the base case. Time complexity is O(log n) for a sorted array of size n, and space complexity is O(log n) due to recursion stack depth.
#include <cstddef> // for size_t

// Recursive binary search on a sorted const array.
// Returns the index of query if found, otherwise -1.
// The array is not modified.
int findElement(const int array[], size_t size, int query) {
    // Helper lambda for recursion to keep the public signature clean.
    auto search = [&](auto&& self, int start, int end) -> int {
        if (start > end) {
            return -1;
        }

        // Avoid overflow: use (start + (end - start) / 2)
        int middle = start + (end - start) / 2;

        if (query > array[middle]) {
            return self(self, middle + 1, end);
        } else if (query < array[middle]) {
            return self(self, start, middle - 1);
        } else {
            return middle;
        }
    };

    if (size == 0) {
        return -1;
    }

    return search(search, 0, static_cast<int>(size) - 1);
}
#include <cassert>

int main() {
    int arr1[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    // Element present
    assert(findElement(arr1, 10, 5) == 4);
    assert(findElement(arr1, 10, 1) == 0);
    assert(findElement(arr1, 10, 10) == 9);
    // Element not present
    assert(findElement(arr1, 10, 0) == -1);
    assert(findElement(arr1, 10, 11) == -1);
    assert(findElement(arr1, 10, -5) == -1);

    // Duplicate values - any valid index is acceptable
    int arr2[] = {2, 2, 2, 2};
    int idx = findElement(arr2, 4, 2);
    assert(idx >= 0 && idx < 4);

    // Single element
    int arr3[] = {42};
    assert(findElement(arr3, 1, 42) == 0);
    assert(findElement(arr3, 1, 41) == -1);

    // Empty array
    int arr4[] = {};
    assert(findElement(arr4, 0, 5) == -1);

    // Large numbers to test overflow avoidance
    int arr5[] = {1000000000, 2000000000};
    assert(findElement(arr5, 2, 2000000000) == 1);
    assert(findElement(arr5, 2, 1500000000) == -1);
}
