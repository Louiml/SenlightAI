Write a C++ function named `isInArithmeticSequence` that takes three integers `A`, `B`, and `C` as input, where `A` is the first term of an arithmetic sequence, `C` is the common difference, and `B` is a candidate value. The function should return `true` if `B` appears somewhere in the infinite arithmetic sequence defined by `A, A+C, A+2C, A+3C, ...` (continuing in both positive and negative index directions), and `false` otherwise. The sequence can have a negative common difference, zero common difference, and can include negative numbers for `A` and `B`. The function must be efficient and handle boundary cases correctly, particularly when `C == 0`.
The key observation is that for `B` to belong to the arithmetic sequence, the difference `(B - A)` must be an exact integer multiple of the common difference `C` (including zero and negative multiples). This means `(B - A) % C == 0`. However, edge cases must be handled carefully:

1. **When `C == 0`**: The sequence consists only of the value `A` repeated forever (since adding zero each time). Therefore `B` belongs if and only if `B == A`. If `B != A`, return `false`.

2. **When `C != 0`**: The condition `(B - A) % C == 0` is necessary and sufficient. However, the modulo operation in C++ with negative numbers can be implementation-defined for negative operands, leading to non-zero remainders even when mathematically divisible. To avoid this, we can check divisibility using the formula `(B - A) / C * C == (B - A)`, which works safely with integers regardless of sign (as long as no overflow occurs). This avoids modulo pitfalls and also handles negative values of `C` and negative `(B - A)` correctly.

3. **No need for additional sign checks**: Since the arithmetic sequence extends infinitely in both directions (positive and negative indices), any `B` that satisfies the divisibility condition is valid, regardless of whether it's greater or less than `A` and regardless of the sign of `C`. For example, with `A=5, C=-2`, the sequence is `5, 3, 1, -1, -3, ...`; `B=-1` belongs because `(-1-5) = -6`, and `-6 / -2 = 3` exactly.

The time complexity is O(1) and space complexity O(1) since only a few arithmetic operations are performed.
#include <cstddef>  // for std::ptrdiff_t if needed, but not necessary here

// Checks if B appears in the arithmetic sequence starting at A with common difference C.
// Returns true if there exists an integer n such that B == A + n*C.
bool isInArithmeticSequence(int A, int B, int C) {
    // If common difference is zero, the sequence is just A repeated.
    if (C == 0) {
        return A == B;
    }
    // Check if (B - A) is an exact multiple of C.
    // Avoid modulo with negative numbers; use division check instead.
    long long diff = static_cast<long long>(B) - A;  // use long long to avoid overflow
    long long C_ll = C;
    return (diff % C_ll == 0);
}
#include <cassert>

int main() {
    // Standard positive difference
    assert(isInArithmeticSequence(1, 5, 2) == true);   // 1,3,5
    assert(isInArithmeticSequence(1, 4, 2) == false);  // not in sequence

    // Negative common difference
    assert(isInArithmeticSequence(10, 4, -3) == true); // 10,7,4
    assert(isInArithmeticSequence(10, 5, -3) == false); // 10,7,4,1...

    // Zero common difference
    assert(isInArithmeticSequence(7, 7, 0) == true);   // always 7
    assert(isInArithmeticSequence(7, 8, 0) == false);  // never 8

    // Negative values and cross-zero cases
    assert(isInArithmeticSequence(-5, 5, 2) == true);  // -5,-3,-1,1,3,5
    assert(isInArithmeticSequence(-5, 4, 2) == false);

    // A equals B
    assert(isInArithmeticSequence(3, 3, 5) == true);   // first term matches

    // Large values to check overflow safety
    assert(isInArithmeticSequence(1000000, -1000000, -2) == true); // diff = -2,000,000, divisible by -2
    assert(isInArithmeticSequence(1000000, -1000001, -2) == false);

    // Edge case: diff not divisible exactly
    assert(isInArithmeticSequence(0, 1, 3) == false);  // 0,3,6...
    assert(isInArithmeticSequence(0, 0, 3) == true);   // first term

    return 0;
}
