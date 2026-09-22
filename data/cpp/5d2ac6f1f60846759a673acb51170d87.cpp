// Write a C++ function `computePolynomial` that takes three integer values `a`, `b`, and `c` as inputs and returns the result of evaluating the expression `a * b * c + a + b * b + c * c * c`. Use 64-bit signed integer arithmetic to prevent overflow from the multiplication and exponentiation (cube of `c`). The function should be pure (no side effects), compile with strict warning flags, and handle any legal `long long` values safely (note: overflow beyond 64-bit is undefined, but input values will be within reasonable limits for the test cases).
#include <cassert>

int main() {
    // Basic positive numbers
    assert(computePolynomial(1, 2, 3) == 1*2*3 + 1 + 2*2 + 3*3*3);  // 6+1+4+27=38
    // Negative values
    assert(computePolynomial(-1, -2, -3) == (-1)*(-2)*(-3) + (-1) + (-2)*(-2) + (-3)*(-3)*(-3)); // -6-1+4-27 = -30
    // Zero values
    assert(computePolynomial(0, 5, 7) == 0*5*7 + 0 + 5*5 + 7*7*7); // 0+0+25+343=368
    assert(computePolynomial(3, 0, 0) == 3*0*0 + 3 + 0*0 + 0*0*0); // 0+3+0+0=3
    // Larger values within 64-bit range
    long long a = 1000000, b = 1000000, c = 1000000;
    assert(computePolynomial(a, b, c) == a*b*c + a + b*b + c*c*c);
    // Mixed signs
    assert(computePolynomial(10, -3, 2) == 10*(-3)*2 + 10 + (-3)*(-3) + 2*2*2); // -60+10+9+8=-33
    // Simple case with ones
    assert(computePolynomial(1, 1, 1) == 1*1*1 + 1 + 1*1 + 1*1*1); // 1+1+1+1=4
    // Negative and positive mix
    assert(computePolynomial(-5, 2, -1) == (-5)*2*(-1) + (-5) + 2*2 + (-1)*(-1)*(-1)); // 10-5+4-1=8
    // Edge: large negative cube
    long long bigNeg = -1000000;
    assert(computePolynomial(1, 1, bigNeg) == 1*1*bigNeg + 1 + 1*1 + bigNeg*bigNeg*bigNeg);
    return 0;
}
#include <cstdint>

// Evaluates a*b*c + a + b*b + c*c*c using 64-bit arithmetic.
long long computePolynomial(long long a, long long b, long long c) {
    long long term1 = a * b * c;          // a*b*c
    long long term2 = a;                  // a
    long long term3 = b * b;              // b^2
    long long term4 = c * c * c;          // c^3
    
    return term1 + term2 + term3 + term4;
}
// The problem is a straightforward arithmetic evaluation with three variables. The key considerations are: (1) the order of operations follows standard C++ precedence — multiplication and exponentiation (via repeated multiplication, since `c * c * c` is equivalent to `c^3`) are performed before addition; (2) we must use `long long` to accommodate potentially large intermediate results, since `a * b * c` could be up to `(10^9)^3 = 10^27`, which exceeds 32-bit but fits in 64-bit (max ~9.2e18). However, to be safe for the given inputs, all operations are done in `long long`. Edge cases include negative numbers (the product and sums work naturally) and zeros (which make terms vanish). Time complexity is O(1) because it's a fixed number of arithmetic operations. Space complexity is O(1) — only a few local variables.
