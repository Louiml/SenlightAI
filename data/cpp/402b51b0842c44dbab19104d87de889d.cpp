/*
Write a C++ function named `first_occurrence` that accepts a constant array of integers, its length, and a target value, then returns the index of the first occurrence of the target if it exists, or `-1` if it does not appear. The function must be const-correct (take `const int arr[]`), use a linear scan, and must not rely on any external libraries beyond the standard headers. The task is to implement the search logic independently, test it with a provided main, and ensure it behaves correctly for empty arrays, duplicate targets, and targets not present in the array.
*/
#include <cstddef>  // for size_t (optional, but good practice)

// Returns the index of the first occurrence of target in arr, or -1 if not found.
// Performs a linear scan from the beginning of the array.
int first_occurrence(const int arr[], int n, int target) {
    for (int i = 0; i < n; ++i) {
        if (arr[i] == target) {
            return i;  // first match found
        }
    }
    return -1;  // target not present
}
#include <cassert>

int main() {
    int arr1[] = {10, 20, 30, 60, 80, 100, 110, 130, 170, 180};
    assert(first_occurrence(arr1, 10, 110) == 6);
    assert(first_occurrence(arr1, 10, 10) == 0);
    assert(first_occurrence(arr1, 10, 180) == 9);
    assert(first_occurrence(arr1, 10, 50) == -1);

    int arr2[] = {5, 5, 5, 5};
    assert(first_occurrence(arr2, 4, 5) == 0);
    assert(first_occurrence(arr2, 4, 6) == -1);

    int arr3[] = {42};
    assert(first_occurrence(arr3, 1, 42) == 0);
    assert(first_occurrence(arr3, 1, 0) == -1);

    int arr4[] = {};
    assert(first_occurrence(arr4, 0, 1) == -1);

    int arr5[] = {-3, -1, 0, 2, -1};
    assert(first_occurrence(arr5, 5, -1) == 1);
    assert(first_occurrence(arr5, 5, 2) == 3);

    return 0;
}
// The solution uses a simple linear search: iterate through the array from index 0 to `n-1`, and as soon as an element equals the target, return that index immediately—this guarantees the first (lowest) occurrence. If the loop finishes without a match, return `-1`. Edge cases include: an empty array (`n == 0`) where the loop never runs and `-1` is returned; duplicates where the first match is returned (e.g., array `[5,5]`, target `5` → index `0`); and a target not present, returning `-1`. The algorithm runs in O(n) time in the worst case (scanning all elements) and O(1) auxiliary space since only a loop counter is used. No sorting or binary search is needed because the requirement explicitly asks for linear search and no assumption of order.
