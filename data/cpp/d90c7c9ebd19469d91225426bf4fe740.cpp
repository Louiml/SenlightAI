/*
Write a C++ function named `maximumHandshakes` that takes an integer `N` (where `N >= 0`) representing the number of people in a room and returns the maximum number of handshakes possible if every pair of people shakes hands exactly once. Assume that a person cannot shake hands with themselves, and each handshake is counted only once per pair. The function must return a `long long` to safely handle large values of `N` (up to `10^7` or more) without overflow.
*/

#include <cstdint>

// Compute the maximum number of handshakes among N people.
// Each pair shakes hands exactly once. Returns a long long to avoid overflow.
long long maximumHandshakes(int N) {
    // Convert to long long for safe multiplication
    long long n = static_cast<long long>(N);
    // n choose 2 formula: n * (n - 1) / 2
    return n * (n - 1) / 2;
}

#include <cassert>

int main() {
    assert(maximumHandshakes(0) == 0);
    assert(maximumHandshakes(1) == 0);
    assert(maximumHandshakes(2) == 1);
    assert(maximumHandshakes(3) == 3);
    assert(maximumHandshakes(4) == 6);
    assert(maximumHandshakes(5) == 10);
    assert(maximumHandshakes(10) == 45);
    assert(maximumHandshakes(100) == 4950);
    // Large value to test overflow handling
    assert(maximumHandshakes(100000) == 4999950000LL);
    assert(maximumHandshakes(1000000) == 499999500000LL);
}

// The problem is a classic combinatorics question: the number of ways to choose 2 distinct people out of `N` people. This is given by the binomial coefficient `C(N, 2) = N * (N-1) / 2`. The formula is derived from the fact that each of the `N` people can shake hands with `N-1` others, giving `N*(N-1)` ordered pairs, but since each pair is counted twice (once from each side), we divide by 2. Edge cases: for `N = 0` or `N = 1`, the formula gives `0 * -1 / 2 = 0` (which is correct because no handshakes are possible), but careful with the sign: `0 * -1 = 0`, and `1 * 0 = 0`, both fine. For `N = 2`, result is `2*1/2 = 1`. The multiplication `N*(N-1)` can overflow for `N` near `2^31`, so we cast `N` to `long long` before multiplication. The algorithm runs in `O(1)` time and `O(1)` auxiliary space.
