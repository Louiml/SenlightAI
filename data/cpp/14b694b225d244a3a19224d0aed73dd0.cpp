/*
Given an integer array, its size `n`, and a target value `x`, write a C++ function named `findPosition` that returns the first index (0-based) where `x` appears in the array, or `-1` if `x` is not found. The function must not modify the array, must use a linear search (stopping as soon as the first match is found), and must handle edge cases such as an empty array (size 0), the target not present, and duplicate occurrences (return the earliest index). The function should be declared with `const int*` for the array parameter to enforce read-only access.
*/
#include <cstddef> // for size_t if needed, but we use int

// Searches for the first occurrence of x in the array of size n.
// Returns the index of the first match, or -1 if x is not found.
int findPosition(const int array[], int n, int x) {
    for (int i = 0; i < n; ++i) {
        if (array[i] == x) {
            return i;
        }
    }
    return -1;
}
#include <cassert>

int main() {
    // Regular case: x present in the middle
    int arr1[] = {1, 2, 3, 4, 5};
    assert(findPosition(arr1, 5, 3) == 2);

    // x at the start
    assert(findPosition(arr1, 5, 1) == 0);

    // x at the end
    assert(findPosition(arr1, 5, 5) == 4);

    // x not present
    assert(findPosition(arr1, 5, 6) == -1);

    // Empty array
    int arr2[] = {};
    assert(findPosition(arr2, 0, 10) == -1);

    // Duplicate values: return first occurrence
    int arr3[] = {7, 8, 7, 9, 7};
    assert(findPosition(arr3, 5, 7) == 0);

    // Negative values
    int arr4[] = {-3, -1, -3, 0};
    assert(findPosition(arr4, 4, -3) == 0);
    assert(findPosition(arr4, 4, 0) == 3);
    assert(findPosition(arr4, 4, -2) == -1);
}
// The solution uses a simple linear scan over the array. Start from index 0 and iterate up to `n-1`. For each element, compare it to `x` using `==`. If a match is found, immediately return that index (this guarantees the earliest occurrence because we scan left to right). If the loop completes without finding `x`, return `-1`. Important edge cases: (1) `n == 0` — the loop does not execute, and we return `-1`; (2) `x` appears multiple times — the first match is returned; (3) `x` is at the last position — the loop returns `n-1`; (4) `x` is not present — returns `-1`. The time complexity is `O(n)` in the worst case (when `x` is absent or at the end), and `O(1)` when `x` is found at index 0. Space complexity is `O(1)` because only a single integer variable is used for the loop index.
