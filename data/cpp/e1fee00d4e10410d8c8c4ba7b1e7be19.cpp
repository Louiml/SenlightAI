/*
Implement a C++ function that performs k-th (0-indexed) removal from an integer array, and before removing, it must find the position of a given target value using linear search. Specifically, write a function `int removeTargetValue(int a[], int n, int x, int& newSize)` that searches for `x` in the array `a` of length `n`. If found, it removes the element at that found index by shifting all subsequent elements left by one, updates `newSize` to `n-1`, and returns the index where the value was found. If not found, it should leave the array unchanged, set `newSize` to `n`, and return `-1`. Assume the array has enough capacity to hold `n` elements and that shifting does not exceed the storage bound. The function must handle duplicate values by removing the first occurrence only, and must work for any valid index, including the last element (where shifting involves no moves) and the first element (where all others shift). The function should not print anything.
*/

#include <cstddef> // for size_t if needed, but int is fine

// Searches for x in a[0..n-1], removes the first occurrence if found,
// shifts remaining elements left, updates newSize, and returns the removed index.
// Returns -1 and leaves newSize unchanged if x is not found.
int removeTargetValue(int a[], int n, const int x, int& newSize) {
    // Linear search for first occurrence of x
    int k = -1;
    for (int i = 0; i < n; ++i) {
        if (a[i] == x) {
            k = i;
            break;
        }
    }
    
    if (k == -1) {
        newSize = n; // unchanged size
        return -1;
    }
    
    // Shift elements left to fill the gap at index k
    for (int i = k + 1; i < n; ++i) {
        a[i - 1] = a[i];
    }
    
    newSize = n - 1;
    return k;
}

#include <cassert>

int main() {
    // Test 1: remove a middle element
    int a1[6] = {22, 44, 33, 66, 11, 55};
    int n1 = 6;
    int newSize1;
    int idx1 = removeTargetValue(a1, n1, 33, newSize1);
    assert(idx1 == 2);
    assert(newSize1 == 5);
    assert(a1[0] == 22 && a1[1] == 44 && a1[2] == 66 && a1[3] == 11 && a1[4] == 55);

    // Test 2: remove first element
    int a2[4] = {10, 20, 30, 40};
    int n2 = 4;
    int newSize2;
    int idx2 = removeTargetValue(a2, n2, 10, newSize2);
    assert(idx2 == 0);
    assert(newSize2 == 3);
    assert(a2[0] == 20 && a2[1] == 30 && a2[2] == 40);

    // Test 3: remove last element
    int a3[3] = {5, 6, 7};
    int n3 = 3;
    int newSize3;
    int idx3 = removeTargetValue(a3, n3, 7, newSize3);
    assert(idx3 == 2);
    assert(newSize3 == 2);
    assert(a3[0] == 5 && a3[1] == 6);

    // Test 4: target not found
    int a4[3] = {5, 6, 7};
    int n4 = 3;
    int newSize4 = 99; // arbitrary initial
    int idx4 = removeTargetValue(a4, n4, 100, newSize4);
    assert(idx4 == -1);
    assert(newSize4 == 3);
    assert(a4[0] == 5 && a4[1] == 6 && a4[2] == 7);

    // Test 5: duplicate values, remove first occurrence
    int a5[5] = {1, 2, 2, 3, 2};
    int n5 = 5;
    int newSize5;
    int idx5 = removeTargetValue(a5, n5, 2, newSize5);
    assert(idx5 == 1);
    assert(newSize5 == 4);
    assert(a5[0] == 1 && a5[1] == 2 && a5[2] == 3 && a5[3] == 2);

    // Test 6: empty array (n=0)
    int a6[1] = {0};
    int n6 = 0;
    int newSize6 = 0;
    int idx6 = removeTargetValue(a6, n6, 10, newSize6);
    assert(idx6 == -1);
    assert(newSize6 == 0);

    return 0;
}

// The solution requires a linear search to find the first index `k` where `a[k] == x`. If no such index exists, return `-1` and keep the size unchanged. If found, perform in-place removal: for `i = k+1` to `n-1`, copy `a[i]` into `a[i-1]`, effectively shifting elements left. Then set the new size to `n-1` via the reference parameter. The algorithm runs in `O(n)` time because the search and shift each scan the array once, and uses `O(1)` auxiliary space since all operations are in-place. Edge cases: if `x` is at the last index, the shift loop runs zero times; if `x` is not present, no shift occurs; if `n` is 0, search returns -1 and no modification happens. The function must be `const`-correct? Since it modifies the array, it cannot be `const` on the array pointer, but the target value parameter can be `const int x`. Also, the new size is an output parameter, so we use a reference.
