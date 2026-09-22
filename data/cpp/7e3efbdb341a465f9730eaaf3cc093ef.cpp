// Write a C++ function `double fastPower(double base, int exponent)` that computes `base` raised to the power `exponent` using an efficient recursive exponentiation-by-squaring algorithm. The function must correctly handle positive, zero, and negative integer exponents, including the special case `INT_MIN` (the most negative 32-bit integer) where naïvely negating the exponent would overflow. For negative exponents, compute the reciprocal of the base raised to the absolute value of the exponent; if `base` is zero and the exponent is negative, the result is mathematically undefined—you may return `INFINITY` or `0` according to your preference, but document your choice. Do not use any library power functions (e.g., `std::pow`). The function should be `const`-correct and work for all representable `int` exponents.
The core idea is to exploit the mathematical identity `base^n = (base^2)^(n/2)` when `n` is even, and `base^n = (base^2)^(n/2) * base` when `n` is odd (using integer division). This reduces the exponent by half at each step, giving O(log |n|) time complexity. The special challenge is negative exponents—especially `INT_MIN` where `-n` overflows. To avoid overflow, for `n < 0` we compute `fastPower(base, -(n+1)) / base` because `-(n+1)` is always representable (for `INT_MIN`, `-(n+1) = INT_MAX`). For `n >= 0`, we recurse on `n/2` until reaching zero, then combine results. The base case is `n == 0` returning `1.0`. For `base == 0.0` and `n < 0`, the result is infinite in real arithmetic; we return `INFINITY` (from `<cmath>`) as a pragmatic choice. Space complexity is O(log |n|) due to recursive call stack, though this could be made iterative; we keep recursion for simplicity matching the snippet. The algorithm's time complexity is O(log |n|), and it correctly handles all edge cases including `n = 0`, `n = 1`, `n = -1`, and `base` being fractional or negative.
#include <cmath> // for INFINITY

// Compute base^exponent using exponentiation by squaring.
// Handles negative exponents, including INT_MIN, without overflow.
// For base == 0 and negative exponent, returns INFINITY.
double fastPower(double base, int exponent) {
    if (exponent == 0) {
        return 1.0;
    }
    if (exponent < 0) {
        // Avoid overflow when exponent == INT_MIN, since -(exponent+1) is safe.
        // Since exponent is negative, base^(-(exponent+1)) / base = base^exponent.
        return fastPower(1.0 / base, -(exponent + 1)) / base;
    }
    // Recursive step: halve exponent.
    double half = fastPower(base * base, exponent / 2);
    // If exponent is odd, multiply by an extra base.
    if (exponent % 2 == 1) {
        return half * base;
    }
    return half;
}
#include <cassert>
#include <cmath>
#include <limits>

int main() {
    // Basic cases
    assert(fastPower(2.0, 0) == 1.0);
    assert(fastPower(2.0, 10) == 1024.0);
    assert(fastPower(3.0, 3) == 27.0);
    assert(fastPower(2.0, -1) == 0.5);
    assert(fastPower(2.0, -3) == 0.125);
    assert(fastPower(0.5, 2) == 0.25);
    assert(fastPower(-2.0, 3) == -8.0);
    assert(fastPower(-2.0, 2) == 4.0);

    // Edge case: INT_MIN (overflow-prone)
    assert(fastPower(1.0, std::numeric_limits<int>::min()) == 1.0);
    assert(fastPower(2.0, -31) == 1.0 / (1LL << 31)); // since INT_MIN = -2^31

    // Edge case: zero base with negative exponent -> INFINITY
    assert(std::isinf(fastPower(0.0, -2)));

    // Edge case: base = 0, positive exponent
    assert(fastPower(0.0, 5) == 0.0);

    // Compare a few values with known results
    assert(fabs(fastPower(1.5, 4) - 5.0625) < 1e-9);
    assert(fabs(fastPower(10.0, -2) - 0.01) < 1e-9);
}
