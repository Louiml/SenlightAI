Write a C++ function `int findLastOccurrence(const int* arr, int n, int target)` that returns the index of the last occurrence of `target` in the array `arr` of length `n`, or `-1` if `target` is not present. The function must scan the array from the end toward the beginning (right-to-left) and return the first match found in that direction. Handle edge cases where `n` is 0 or negative (return `-1` immediately), and where the target appears multiple times (return the highest index). The function must be `const`-correct, meaning it accepts a pointer to constant data, and it must not modify the array. You may assume `n` is a valid non-negative integer in normal use, but your implementation should still be defensive.
// The solution iterates from the last valid index (`n-1`) down to index 0, checking each element for equality with `target`. The loop condition must handle `n <= 0` by skipping the loop entirely and returning `-1`. This is straightforward with a `for` loop: `for (int i = n - 1; i >= 0; --i)`. If a match is found, return `i` immediately. If the loop completes without a match, return `-1`. Edge cases: `n = 0` → loop start `i = -1`, condition `i >= 0` is false, so returns `-1`; `n` negative → similarly `i` starts negative and loop doesn't execute. For an array with duplicates, the first hit from the right is naturally the highest index. Time complexity is O(n) in the worst case (target absent or at index 0), O(1) best case (target at last index). Space complexity is O(1) auxiliary. The function signature uses `const int*` to enforce read-only access, and we avoid any modification.
#include <cstddef>  // for ptrdiff_t as index type, but using int for simplicity

// Returns the index of the last occurrence of target in arr[0..n-1],
// or -1 if not found. Scans from right to left.
int findLastOccurrence(const int* arr, int n, int target) {
    // Defensive: if n is invalid or pointer is null, return -1.
    if (arr == nullptr || n <= 0) {
        return -1;
    }
    
    // Scan from the end of the array to the beginning.
    for (int i = n - 1; i >= 0; --i) {
        if (arr[i] == target) {
            return i;  // First match encountered from the right is the last occurrence.
        }
    }
    
    return -1;  // Target not found.
}
#include <cassert>

int main() {
    // Basic cases
    int arr1[] = {1, 2, 3, 4, 5};
    assert(findLastOccurrence(arr1, 5, 3) == 2);
    assert(findLastOccurrence(arr1, 5, 1) == 0);
    assert(findLastOccurrence(arr1, 5, 5) == 4);

    // Target not present
    assert(findLastOccurrence(arr1, 5, 9) == -1);

    // Duplicates - should return highest index
    int arr2[] = {4, 2, 4, 4, 7, 4};
    assert(findLastOccurrence(arr2, 6, 4) == 5);

    // Edge cases: empty array or invalid n
    int arr3[] = {10};
    assert(findLastOccurrence(arr3, 0, 10) == -1);
    assert(findLastOccurrence(arr3, -3, 10) == -1);

    // Null pointer
    assert(findLastOccurrence(nullptr, 5, 1) == -1);

    // Single element matches
    assert(findLastOccurrence(arr3, 1, 10) == 0);
    assert(findLastOccurrence(arr3, 1, 99) == -1);

    // Negative values in array
    int arr4[] = {-3, -1, -3, -5};
    assert(findLastOccurrence(arr4, 4, -3) == 2);
    assert(findLastOccurrence(arr4, 4, -5) == 3);
    assert(findLastOccurrence(arr4, 4, 0) == -1);

    return 0;
}
