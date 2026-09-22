// Given four positive integers `a`, `b`, `c`, and `d`, write a C++ function that computes how many times each of the first two integers fully divides the corresponding integer from the second pair, and returns the sum of those two quotients. Specifically, the result is `(c / a) + (d / b)`, using integer division (truncating any remainder). Assume all inputs are positive integers, so no division by zero occurs. The function should be pure, taking four `int` parameters and returning an `int`.

#include <cassert>

// Declaration of the function under test.
int sumOfQuotients(int a, int b, int c, int d);

int main() {
    // Basic case: both divisions exact.
    assert(sumOfQuotients(2, 3, 4, 6) == 2 + 2); // 2 + 2 = 4
    // One division exact, one with remainder (truncated).
    assert(sumOfQuotients(2, 3, 5, 7) == 2 + 2); // 2 + 2 = 4
    // Both divisions are less than 1 → quotient 0.
    assert(sumOfQuotients(5, 5, 2, 2) == 0 + 0); // 0
    // One quotient is 0, the other is large.
    assert(sumOfQuotients(10, 2, 5, 20) == 0 + 10); // 0 + 10 = 10
    // Denominators are 1 → quotient equals the numerator.
    assert(sumOfQuotients(1, 1, 7, 9) == 7 + 9); // 16
    // Large inputs still fit in int.
    assert(sumOfQuotients(1000000, 1000000, 2000000, 1999999) == 2 + 1); // 3
    // Mixed values.
    assert(sumOfQuotients(3, 4, 10, 9) == 3 + 2); // 5
    // Exact divisions with same denominators.
    assert(sumOfQuotients(7, 7, 14, 21) == 2 + 3); // 5
    // One denominator larger than numerator → 0.
    assert(sumOfQuotients(8, 3, 4, 15) == 0 + 5); // 5
    // Both quotients are equal.
    assert(sumOfQuotients(6, 6, 12, 18) == 2 + 3); // 5
    return 0;
}

#include <cstdint>

// Computes the sum of integer divisions (c / a) + (d / b).
// Requires a > 0 and b > 0.
int sumOfQuotients(int a, int b, int c, int d) {
    // Integer division truncates toward zero for positive operands.
    const int quotient_c = c / a;
    const int quotient_d = d / b;
    return quotient_c + quotient_d;
}

// The task is straightforward: compute the integer quotient of `c` divided by `a`, and the integer quotient of `d` divided by `b`, then sum them. Since `a` and `b` are guaranteed positive (per problem constraints), integer division is well-defined and truncates toward zero. The main algorithm is simply two divisions and one addition. Edge cases include when `a` or `c` is smaller than `c` or `a` respectively (quotient is 0), or when `a` equals `c` (quotient is 1), or when `a` is 1 (quotient is `c`). No special handling is needed beyond using `int` arithmetic. Time complexity is O(1) and space complexity is O(1), as only a few variables are used.
