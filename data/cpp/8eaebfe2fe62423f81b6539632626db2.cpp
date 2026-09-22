/*
Write a C++ function named `findFirstOccurrence` that takes a constant reference to an array of integers, its size (as an `int`), and a target value (as an `int`). The function must return the zero-based index of the **first** occurrence of the target value in the array. If the target is not present, return `-1`. The function must work correctly for arrays that are unsorted, contain duplicate values (returning the smallest index where the target appears), and handle edge cases such as an empty array (size 0). Do not modify the input array; ensure the parameter is `const`. The function must use linear search (no binary search) and be implemented in a way that is safe for any valid array size.
*/

#include <cstddef>  // for std::size_t (optional, but good practice)

// Finds the first occurrence of target in arr.
// Returns the zero-based index, or -1 if not found.
// The input array is treated as read-only.
int findFirstOccurrence(const int arr[], int size, int target) {
    for (int i = 0; i < size; ++i) {
        if (arr[i] == target) {
            return i;
        }
    }
    return -1;
}

#include <cassert>

int main() {
    // Normal case with duplicates: first occurrence should be index 1
    int a1[] = {5, 3, 7, 3, 9};
    assert(findFirstOccurrence(a1, 5, 3) == 1);

    // Target at first position
    int a2[] = {42, 10, 20};
    assert(findFirstOccurrence(a2, 3, 42) == 0);

    // Target at last position
    int a3[] = {1, 2, 3, 4, 5};
    assert(findFirstOccurrence(a3, 5, 5) == 4);

    // Target not present
    int a4[] = {10, 20, 30};
    assert(findFirstOccurrence(a4, 3, 99) == -1);

    // Empty array (size 0)
    int a5[] = {};
    assert(findFirstOccurrence(a5, 0, 1) == -1);

    // Single element, present
    int a6[] = {7};
    assert(findFirstOccurrence(a6, 1, 7) == 0);

    // Single element, absent
    int a7[] = {7};
    assert(findFirstOccurrence(a7, 1, 8) == -1);

    // All identical values
    int a8[] = {4, 4, 4, 4};
    assert(findFirstOccurrence(a8, 4, 4) == 0);

    // Negative numbers
    int a9[] = {-5, -2, -2, 0};
    assert(findFirstOccurrence(a9, 4, -2) == 1);

    // Large size, target in middle
    int a10[100];
    for (int i = 0; i < 100; ++i) a10[i] = i * 2;
    assert(findFirstOccurrence(a10, 100, 66) == 33);
}

// The solution uses a simple sequential scan from index `0` to `size-1`. For each index, compare `arr[i]` with the target. On the first match, immediately return that index, which guarantees the first occurrence when duplicates exist. If the loop completes without finding the target, return `-1`. Edge cases: if `size` is `0`, the loop does not execute and the function returns `-1`. If the target appears multiple times, the first match is returned. Time complexity is `O(n)` in the worst case (target absent or at the last position) and `O(1)` in the best case (target at index 0). Space complexity is `O(1)` because only a loop counter is used. The function is `const`-correct by taking `const int arr[]` (equivalent to `const int*`), which prevents accidental modification of the input.
