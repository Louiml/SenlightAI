Write a C++ function `canMakeWholeSegments(int N, int K)` that takes two positive integers, `N` and `K`. The function should return an integer: `0` if `N` can be divided into `K` equal whole parts (i.e., `N` is a multiple of `K`), and `1` otherwise. This is a direct simulation of the given snippet but packaged as a reusable, standalone function. The function must handle all positive integer inputs, including cases where `K` is larger than `N`. The solution must be self-contained, avoid global state, and use only standard library facilities.

#include <cassert>

int main() {
    // Standard divisibility cases
    assert(canMakeWholeSegments(10, 5) == 0);
    assert(canMakeWholeSegments(10, 3) == 1);
    // N is smaller than K
    assert(canMakeWholeSegments(4, 7) == 1);
    // N equals K
    assert(canMakeWholeSegments(6, 6) == 0);
    // K equals 1
    assert(canMakeWholeSegments(1, 1) == 0);
    assert(canMakeWholeSegments(100, 1) == 0);
    // N is a multiple of K
    assert(canMakeWholeSegments(25, 5) == 0);
    // N is not a multiple of K
    assert(canMakeWholeSegments(25, 7) == 1);
    // Large numbers
    assert(canMakeWholeSegments(1000000000, 1000000) == 0);
    assert(canMakeWholeSegments(1000000000, 999999) == 1);
    return 0;
}

#include <cstddef>

// Returns 0 if N can be divided into K equal whole parts, 1 otherwise.
// Assumes N and K are positive integers.
int canMakeWholeSegments(int N, int K) {
    return (N % K == 0) ? 0 : 1;
}

// The task reduces to checking divisibility: `N % K == 0`. If the remainder is zero, return `0`; otherwise return `1`. Edge cases: when `N < K`, the remainder is always `N` (nonzero unless `N` is 0, but inputs are positive), so the result is `1`. When `K` equals 1, every positive `N` is divisible, so the result is always `0`. When `N` equals `K`, the remainder is 0. The modulo operation is constant time, so time complexity is O(1). Space complexity is O(1) as no additional storage is used. The function should take parameters by value for simplicity (no large objects), and the return type is an integer (`int`), matching the original snippet.
