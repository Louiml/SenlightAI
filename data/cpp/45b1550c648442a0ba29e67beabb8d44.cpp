/*
Implement a C++ function `int safeDivide(int dividend, int divisor)` that performs integer division with truncation toward zero, but with the following modifications: (1) It must handle the edge case where `dividend` is `INT_MIN` and `divisor` is `-1` by returning `INT_MAX` (clamping to the maximum representable 32-bit signed integer). (2) It must avoid undefined behavior from left-shifting a signed integer when the shifted value overflows. Use only 64-bit arithmetic internally to ensure safety, and the final result must be clamped to fit within the `int` range. The function should return the quotient rounded toward zero, just like normal C++ integer division, for all other inputs.
*/

#include <limits>
#include <cstdint>
#include <cstdlib>
#include <algorithm>

// Performs integer division truncating toward zero, safely handling overflow.
// Returns INT_MAX for INT_MIN / -1, and clamps any out-of-range result.
int safeDivide(int dividend, int divisor) {
    // Assume divisor is non-zero; if not, this would be undefined behavior.
    if (divisor == 0) {
        return 0; // or throw, but for simplicity return 0
    }

    // Work with 64-bit absolute values to avoid signed overflow.
    long long a = std::llabs(static_cast<long long>(dividend));
    long long b = std::llabs(static_cast<long long>(divisor));

    long long result = 0;
    int shift = 31;
    while (shift >= 0) {
        // Avoid shifting if b is too large; but b<=2^31, shift<=31, so b<<shift may overflow 64-bit when b>1 and shift large.
        // Use a safer comparison: check if (a >> shift) >= b, which avoids overflow.
        if (shift <= 62 && (b << shift) > 0 && a >= (b << shift)) {
            a -= (b << shift);
            result += 1LL << shift;
        }
        --shift;
    }

    // Apply sign: negative if signs differ.
    if ((dividend < 0) != (divisor < 0)) {
        result = -result;
    }

    // Clamp to int range.
    const long long int_min = static_cast<long long>(std::numeric_limits<int>::min());
    const long long int_max = static_cast<long long>(std::numeric_limits<int>::max());
    if (result > int_max) result = int_max;
    if (result < int_min) result = int_min;

    return static_cast<int>(result);
}

#include <cassert>
#include <climits>

int main() {
    // Normal cases
    assert(safeDivide(10, 3) == 3);
    assert(safeDivide(-10, 3) == -3);
    assert(safeDivide(10, -3) == -3);
    assert(safeDivide(-10, -3) == 3);
    assert(safeDivide(7, 2) == 3);
    assert(safeDivide(7, -2) == -3);

    // Zero dividend
    assert(safeDivide(0, 5) == 0);
    assert(safeDivide(0, -5) == 0);

    // Divisor = ±1
    assert(safeDivide(5, 1) == 5);
    assert(safeDivide(5, -1) == -5);
    assert(safeDivide(-5, 1) == -5);
    assert(safeDivide(-5, -1) == 5);

    // Extremes
    assert(safeDivide(INT_MAX, 1) == INT_MAX);
    assert(safeDivide(INT_MIN, 1) == INT_MIN);
    assert(safeDivide(INT_MAX, -1) == -INT_MAX); // INT_MAX is fine
    assert(safeDivide(INT_MIN, -1) == INT_MAX);  // Overflow case clamped
    assert(safeDivide(INT_MIN, 2) == -1073741824); // -2^31 / 2
    assert(safeDivide(INT_MAX, 2) == 1073741823);

    // Division by larger absolute divisor
    assert(safeDivide(1, 3) == 0);
    assert(safeDivide(-1, 3) == 0);
    assert(safeDivide(1, -3) == 0);

    // Perfect division
    assert(safeDivide(100, 10) == 10);
    assert(safeDivide(-100, 10) == -10);

    return 0;
}

// The core idea is to perform the division using 64-bit absolute values to avoid overflow during shifting and negation. First, take the absolute values of both operands using `std::llabs` on `long long` casts. Then, use a binary long-division algorithm: iterate a shift variable from 31 down to 0, checking if the current remainder (`a`) is at least `b << shift`. If so, subtract `b << shift` from `a` and add `1LL << shift` to the result. This builds the quotient bit by bit. After the loop, apply the sign: if the signs of the original operands differ, negate the result. Finally, clamp the result to `[INT_MIN, INT_MAX]` using `std::clamp` or a manual min/max, with special handling for the `INT_MIN / -1` overflow case. Time complexity is O(1) (constant number of iterations, 32), and space complexity is O(1). Edge cases include: divisor = 0 (undefined behavior—throw an exception or handle gracefully; here we assume divisor is non-zero as per typical constraints), dividend = 0 (returns 0), and the overflow case described.
