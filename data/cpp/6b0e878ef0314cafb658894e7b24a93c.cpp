Write a C++ function that takes two integers `a` and `b` (each in the range `[-10^9, 10^9]`) and returns their sum as a `long long` to avoid integer overflow. The function must handle all valid inputs correctly, including negative numbers, zero, and boundary values near the limits of the 32-bit integer range. The function should be named `safeSum` and should take two `int` parameters, returning a `long long`.
// The core challenge is avoiding overflow when adding two 32-bit `int` values, since their sum can exceed the range of a signed 32-bit integer. The simplest and most robust approach is to cast each input to `long long` before performing the addition, because `long long` is guaranteed to hold the sum of two 32-bit ints (maximum sum is `2 * 10^9` which is well below the `long long` maximum of ~9.22e18). There are no special edge cases beyond ensuring the cast happens before the addition; no need to handle sign separately since `long long` arithmetic handles negative sums correctly. Time complexity is O(1) and space complexity is O(1) — the function performs a constant number of operations regardless of input size.
// Safely add two 32-bit integers without overflow by widening to long long.
long long safeSum(int a, int b) {
    return static_cast<long long>(a) + static_cast<long long>(b);
}
#include <cassert>

int main() {
    // Basic positive case
    assert(safeSum(3, 4) == 7LL);
    // Negative numbers
    assert(safeSum(-5, -2) == -7LL);
    // Mixed signs
    assert(safeSum(10, -3) == 7LL);
    // One zero
    assert(safeSum(0, 0) == 0LL);
    assert(safeSum(-1, 0) == -1LL);
    // Boundary values avoiding overflow
    assert(safeSum(2000000000, 2000000000) == 4000000000LL);
    assert(safeSum(-2000000000, -2000000000) == -4000000000LL);
    // Extremes of int range
    assert(safeSum(2147483647, 1) == 2147483648LL);
    assert(safeSum(-2147483647, -2) == -2147483649LL);
    return 0;
}
