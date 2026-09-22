You are given a single non-negative integer `n` (with `0 ≤ n ≤ 10^6`). Write a C++ function that, given `n`, returns the integer value computed as: start with 1, then add `6 * (1 + 2 + ... + n)`. In other words, the result is `1 + 6 * (n*(n+1)/2)`. Ensure your function uses `long long` arithmetic to avoid overflow, and handles the case `n = 0` correctly (returning 1). The function should be pure (no side effects), and should not read from standard input or output anything. The input size is small enough that a direct formula works in O(1) time.
#include <cassert>

int main() {
    assert(hexagonalNumber(0) == 1);
    assert(hexagonalNumber(1) == 7);      // 1 + 6*1
    assert(hexagonalNumber(2) == 19);     // 1 + 6*3
    assert(hexagonalNumber(3) == 37);     // 1 + 6*6
    assert(hexagonalNumber(10) == 331);   // 1 + 6*55
    assert(hexagonalNumber(100) == 30301);
    assert(hexagonalNumber(1000000) == 3000003000001LL);  // exact value
}
#include <cstdint>

// Compute 1 + 6 * (sum of integers from 1 to n).
// n is non-negative, and result fits in long long for n <= 10^6.
long long hexagonalNumber(long long n) {
    // Sum of first n natural numbers: n*(n+1)/2
    long long sum = n * (n + 1) / 2;
    return 1 + 6 * sum;
}
// The problem reduces to computing the sum of the first `n` positive integers, which has the well-known closed form `n*(n+1)/2`. Multiplying that sum by 6 and adding 1 yields the final answer. The only edge case is `n = 0`, where the sum is 0, so the result is exactly 1. Using `long long` for all intermediate computations prevents overflow: with `n` up to 10^6, `n*(n+1)` is about 10^12, well within the range of a 64-bit signed integer (max ~9.22e18). The algorithm runs in O(1) time and uses O(1) auxiliary space. No loops or recursion are needed; the formula is direct and robust for all allowed inputs.
