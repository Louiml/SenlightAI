/*
Write a C++ function `long long lcm(int a, int b)` that takes two integers (which may be negative, zero, or positive) and returns their least common multiple (LCM) as a `long long` to avoid overflow. The LCM must be non-negative (by convention, LCM(0, x) = 0). The function must compute the LCM using the Euclidean algorithm for the greatest common divisor (GCD) and the formula `lcm = |a * b| / gcd(a, b)`. Do not use any built-in GCD/LCM functions. The function must handle all edge cases correctly, including zero inputs and negative numbers, and avoid overflow by using `long long` for intermediate multiplication.
*/

#include <cstdlib> // for std::abs

// Compute the least common multiple of two integers.
// Returns 0 if either input is 0. Handles negative inputs correctly.
long long lcm(int a, int b) {
    if (a == 0 || b == 0) {
        return 0;
    }

    // Compute GCD using the Euclidean algorithm.
    int x = std::abs(a);
    int y = std::abs(b);
    while (y != 0) {
        int remainder = x % y;
        x = y;
        y = remainder;
    }
    int gcd = x;

    // Use long long to avoid overflow when multiplying.
    long long product = static_cast<long long>(a) * static_cast<long long>(b);
    long long result = std::abs(product) / gcd;
    return result;
}

#include <cassert>

int main() {
    // Basic positive cases
    assert(lcm(4, 6) == 12);
    assert(lcm(21, 6) == 42);
    assert(lcm(1, 1) == 1);

    // Zero cases
    assert(lcm(0, 5) == 0);
    assert(lcm(7, 0) == 0);
    assert(lcm(0, 0) == 0);

    // Negative inputs
    assert(lcm(-4, 6) == 12);
    assert(lcm(4, -6) == 12);
    assert(lcm(-4, -6) == 12);

    // Co-prime numbers
    assert(lcm(17, 19) == 323);

    // Large values to check overflow safety
    assert(lcm(100000, 99999) == 9999900000LL);
    assert(lcm(INT_MAX, 1) == static_cast<long long>(INT_MAX));

    // One divides the other
    assert(lcm(12, 4) == 12);
    assert(lcm(5, 15) == 15);
}

// The solution computes the GCD using the Euclidean algorithm, which repeatedly replaces `a` with `b` and `b` with `a % b` until `b` becomes zero. The GCD is then the last non-zero `a`. For the LCM, we use the property `lcm(a, b) = |a * b| / gcd(a, b)`. To avoid overflow when multiplying two large `int` values, we first cast `a` and `b` to `long long` before multiplication and take the absolute value. If either input is zero, the LCM is defined as 0, so we return 0 immediately. Negative inputs are handled naturally: taking the absolute value of the product ensures a non-negative LCM. The Euclidean algorithm runs in \(O(\log(\min(|a|,|b|)))\) time, and the function uses \(O(1)\) auxiliary space. Edge cases include: one input being zero (return 0), both inputs positive, both negative, one negative one positive, and large inputs near `INT_MAX` where multiplication could overflow if not using `long long`.
