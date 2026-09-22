/*
Write a C++ function `divideIntegers(int dividend, int divisor)` that performs integer division without using the multiplication `*`, division `/`, or modulo `%` operators. The function must return the quotient truncated toward zero. It must handle edge cases: if the divisor is zero, return `INT_MAX`; if the dividend is `INT_MIN` and the divisor is `-1`, return `INT_MAX` (to avoid overflow). The function should correctly handle negative dividends and divisors, and the returned result must fit within the 32-bit signed integer range. Do not use any built-in division-related operations; implement the algorithm manually using bit shifts and addition/subtraction.
*/

#include <climits>
#include <cstdlib>

// Perform integer division without using *, /, or %.
// Returns INT_MAX on divisor zero or INT_MIN / -1 overflow.
int divideIntegers(int dividend, int divisor) {
    if (divisor == 0) return INT_MAX;
    if (dividend == INT_MIN && divisor == -1) return INT_MAX;

    // Use long long to safely take absolute values.
    long long de = std::labs(static_cast<long long>(dividend));
    long long ds = std::labs(static_cast<long long>(divisor));

    int sign = ((dividend < 0) ^ (divisor < 0)) ? -1 : 1;

    int res = 0;
    long long temp = ds;

    while (de >= ds) {
        int mul = 1;
        // Find the largest shift of divisor that is <= de.
        while (de >= (temp << 1)) {
            temp <<= 1;
            mul <<= 1;
        }
        de -= temp;
        res += mul;
        temp = ds; // reset for next iteration
    }

    return sign == -1 ? -res : res;
}

#include <cassert>
#include <climits>

int divideIntegers(int, int); // forward declaration for test clarity

int main() {
    assert(divideIntegers(10, 3) == 3);
    assert(divideIntegers(-10, 3) == -3);
    assert(divideIntegers(10, -3) == -3);
    assert(divideIntegers(-10, -3) == 3);
    assert(divideIntegers(7, -3) == -2);
    assert(divideIntegers(0, 5) == 0);
    assert(divideIntegers(5, 0) == INT_MAX);
    assert(divideIntegers(INT_MIN, -1) == INT_MAX);
    assert(divideIntegers(INT_MIN, 1) == INT_MIN);
    assert(divideIntegers(INT_MAX, 1) == INT_MAX);
    return 0;
}

// The core idea is to use binary long division (similar to decimal long division) by repeatedly subtracting the largest possible shifted multiple of the divisor from the dividend. First, handle special cases: zero divisor and the overflow case (`INT_MIN / -1`). Then use `long long` (via `labs`) to take absolute values safely, avoiding overflow when `INT_MIN` is negated. Determine the sign of the result by XORing the signs of the inputs. Initialize a result `res = 0` and `temp = ds` (absolute divisor). While the absolute dividend `de` is greater than or equal to `ds`, find the largest shift such that `(temp << 1)` does not exceed `de`. Each time we shift `temp` left by one, we also shift a multiplier `mul` left by one. Subtract `temp` from `de` and add `mul` to `res`. Reset `temp` to `ds` for the next iteration. The loop runs at most `log2(INT_MAX)` times for each subtraction chain, because the multiplier grows exponentially. After the loop, apply the correct sign to `res`. Time complexity is O(log(dividend)) in the worst case, with constant auxiliary space.
