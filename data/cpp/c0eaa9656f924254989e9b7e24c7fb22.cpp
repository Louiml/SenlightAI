Write a C++ function named `siteScore` that takes four integers `a`, `b`, `c`, and `d` as parameters and returns the integer result of the expression `56*a + 24*b + 14*c + 6*d`. The function should be `const`-correct (though the parameters are passed by value, so no mutation is possible), and it should handle any integer inputs, including negative values and zero. The returned value must fit within the range of a signed 32-bit integer. No input validation is required; assume the caller provides valid integers.

#include <cassert>

int siteScore(const int a, const int b, const int c, const int d);

int main() {
    // Basic positive inputs from the original problem
    assert(siteScore(1, 1, 1, 1) == 100);           // 56+24+14+6 = 100
    // All zeros
    assert(siteScore(0, 0, 0, 0) == 0);
    // Negative inputs
    assert(siteScore(-1, -1, -1, -1) == -100);
    // Mixed signs
    assert(siteScore(2, -3, 4, -5) == (112 - 72 + 56 - 30) == 66);
    // Large values within 32-bit range
    assert(siteScore(10000, 10000, 10000, 10000) == 1000000);
    // Single non-zero
    assert(siteScore(1, 0, 0, 0) == 56);
    assert(siteScore(0, 1, 0, 0) == 24);
    assert(siteScore(0, 0, 1, 0) == 14);
    assert(siteScore(0, 0, 0, 1) == 6);
    // Edge: maximum typical values (e.g., all 100000)
    assert(siteScore(100000, 100000, 100000, 100000) == 10000000);
    return 0;
}

// Computes 56*a + 24*b + 14*c + 6*d.
int siteScore(const int a, const int b, const int c, const int d) {
    return (56 * a) + (24 * b) + (14 * c) + (6 * d);
}

// The problem is a straightforward arithmetic computation. The main algorithm simply multiplies each input by its corresponding fixed coefficient and sums the products: `result = 56*a + 24*b + 14*c + 6*d`. There are no loops, conditional branches, or data structures needed. Edge cases include negative inputs, which work naturally with integer multiplication and addition, and zero values, which contribute nothing. The only potential concern is integer overflow if the inputs are extremely large; since the coefficients are positive and the inputs are assumed to be within reasonable bounds (the original problem constraints likely keep the result within 32-bit range), no special handling is needed. Time complexity is O(1) because the computation involves a constant number of arithmetic operations. Space complexity is O(1) as only a single local result variable is used.
