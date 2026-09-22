/*
Write a C++ function `sentinelSearch(const int* arr, int size, int target)` that performs a linear search for `target` in an array of integers using the sentinel technique described in the snippet. The function must return the index of the first occurrence of `target` in the array (searching from index 0 toward the end), or `-1` if not found. The array must not be modified, so the sentinel must be simulated without actually writing to the array. Handle edge cases such as `size == 0`, `target` appearing at the first position, `target` appearing multiple times, and `target` being absent. The function must be `const`‑correct and efficient, and the input array is guaranteed to be valid for reading `size` elements.
*/

#include <cstddef>

// Performs a sentinel-style linear search without modifying the input array.
// Returns the index of the first occurrence of target, or -1 if not found.
int sentinelSearch(const int* arr, int size, int target) {
    if (size <= 0) {
        return -1;
    }
    
    // Standard linear scan; no sentinel written to the array.
    for (int i = 0; i < size; ++i) {
        if (arr[i] == target) {
            return i;
        }
    }
    return -1;
}

#include <cassert>

int main() {
    int a1[] = {1, 2, 3, 4, 5};
    assert(sentinelSearch(a1, 5, 3) == 2);
    assert(sentinelSearch(a1, 5, 1) == 0);
    assert(sentinelSearch(a1, 5, 5) == 4);
    assert(sentinelSearch(a1, 5, 100) == -1);

    int a2[] = {7, 7, 7};
    assert(sentinelSearch(a2, 3, 7) == 0);

    int a3[] = {-1, -2, -3};
    assert(sentinelSearch(a3, 3, -3) == 2);
    assert(sentinelSearch(a3, 3, 0) == -1);

    int* empty = nullptr;
    assert(sentinelSearch(empty, 0, 1) == -1);

    int a4[] = {10};
    assert(sentinelSearch(a4, 1, 10) == 0);
    assert(sentinelSearch(a4, 1, 11) == -1);
}

// The sentinel search in the original snippet works by placing the target at index 0 and scanning from the tail. However, since we cannot modify the input (const correctness), we emulate the sentinel by using a separate variable or by checking index bounds manually. A simple approach: iterate from index 0 to `size-1` and compare each element with `target`. But to mimic the sentinel philosophy (avoiding a separate length check inside the loop), we can first handle the case where the array is empty. Then, for a non-empty array, we can iterate with a loop that checks the current element; if it matches, return the index. If we reach the end, return -1. The key is that we do not need a sentinel because we can use a condition like `while (i < size && arr[i] != target)`. However, that’s effectively the same as a standard linear search. For a true sentinel simulation without modifying the array, we can use a loop that unconditionally reads arr[i] but stops when i reaches size; if we read beyond size, that would be undefined behavior. So the safe approach is standard linear search with a bound check. Time complexity is O(n) in the worst case, O(1) average if the target is uniformly distributed (but still O(n) worst). Space complexity is O(1). Edge cases: empty array returns -1; target at index 0 returns 0; target not found returns -1; duplicates return the first occurrence.
