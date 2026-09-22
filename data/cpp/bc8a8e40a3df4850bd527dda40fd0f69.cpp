Write a C++ function that takes an integer array and its size as parameters and returns the first value that appears more than once in the array (i.e., the earliest element whose duplicate appears later in the array). Traverse the array from left to right; for each element, check all subsequent elements for a match. If a duplicate is found, return that value immediately. If no duplicate exists, return -1. The function must be `const`-correct (accept array as `const int*` or `const int arr[]`) and handle arrays of any length, including empty arrays (return -1) and arrays with all unique elements (return -1). Duplicates may repeat multiple times, but the first encountered pair (by the outer loop index) wins. Do not modify the input array.

The algorithm uses a nested loop: the outer loop iterates over each index `i` from 0 to `n-1`, and the inner loop checks every index `j` from `i+1` to `n-1`. If `arr[i] == arr[j]`, we return `arr[i]` immediately because this is the first occurrence (smallest `i`) that has any duplicate later in the array. Since we scan `i` in increasing order, the first `i` that finds a match yields the earliest element (by first occurrence) that repeats. Edge cases: an empty array (`n == 0`) should return -1; an array with all distinct elements returns -1; if duplicates exist but the first occurrence is at index 0 and its duplicate at index 5, we still return that value because `i=0` is checked before `i=1`. Time complexity is O(n²) in the worst case (e.g., all elements unique or the duplicate is at the end), and O(1) auxiliary space. Space complexity excludes input storage. The function should be declared `int firstDuplicate(const int arr[], int n)` and marked `const` for the array parameter.

#include <cstddef> // for size_t if needed, but we use int

// Returns the first value that appears more than once in the array.
// The "first" is defined by the earliest index i whose value has a duplicate later.
// Returns -1 if no duplicate exists or if the array is empty.
int firstDuplicate(const int arr[], int n) {
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            if (arr[i] == arr[j]) {
                return arr[i];
            }
        }
    }
    return -1;
}

#include <cassert>

int main() {
    int arr1[] = {0, 2, 1, 3, 2, 2};
    assert(firstDuplicate(arr1, 6) == 2);

    int arr2[] = {1, 2, 3, 4};
    assert(firstDuplicate(arr2, 4) == -1);

    int arr3[] = {5, 5, 5, 5};
    assert(firstDuplicate(arr3, 4) == 5);

    int arr4[] = {7, 1, 7, 2, 1};
    assert(firstDuplicate(arr4, 5) == 7); // Index 0 value 7 duplicates at index 2

    int arr5[] = {9};
    assert(firstDuplicate(arr5, 1) == -1);

    assert(firstDuplicate(nullptr, 0) == -1); // Empty array via pointer

    int arr6[] = {3, 1, 3, 1, 3};
    assert(firstDuplicate(arr6, 5) == 3); // First occurrence at index 0

    int arr7[] = {-1, 0, -1, 0};
    assert(firstDuplicate(arr7, 4) == -1); // Careful: -1 is both a value and our sentinel, but this is correct because -1 appears at index 0 and 2; the return value is -1 (the actual element), which is indistinguishable from "no duplicate" but semantically correct.

    int arr8[] = {2, 2, 2, 1, 1};
    assert(firstDuplicate(arr8, 5) == 2);

    int arr9[] = {10, 20, 10, 20, 30};
    assert(firstDuplicate(arr9, 5) == 10);
    return 0;
}
