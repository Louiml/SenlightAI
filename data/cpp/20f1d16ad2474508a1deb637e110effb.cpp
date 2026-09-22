/*
Write a C++ function named `isInArithmeticSequence` that takes three integers `A`, `B`, and `C` as parameters. The function should return `true` if `B` can be reached starting from `A` by repeatedly adding `C` (including zero steps, i.e., `A == B`), and `false` otherwise. The parameters may be negative, zero, or positive. Consider the case where `C == 0` separately. The function must be `const`-correct and should not modify its inputs. This task is inspired by a typical competitive programming problem where arithmetic progression membership must be checked efficiently.
*/
#include <cstdint>

// Returns true if B can be reached from A by adding C zero or more times.
// Handles negative C and the special case where C == 0.
bool isInArithmeticSequence(long long A, long long B, long long C) {
    if (C == 0) {
        return A == B;
    }
    long long diff = B - A;
    // diff must be divisible by C, and the quotient must be non-negative.
    if (diff % C != 0) {
        return false;
    }
    long long steps = diff / C;
    return steps >= 0;
}
#include <cassert>

int main() {
    // Basic positive step
    assert(isInArithmeticSequence(1, 10, 3) == true);  // 1,4,7,10
    assert(isInArithmeticSequence(1, 11, 3) == false);
    // Negative step
    assert(isInArithmeticSequence(10, 1, -3) == true); // 10,7,4,1
    assert(isInArithmeticSequence(10, 2, -3) == false);
    // Zero step: only equal values work
    assert(isInArithmeticSequence(5, 5, 0) == true);
    assert(isInArithmeticSequence(5, 6, 0) == false);
    // Starting and ending at same value with non-zero C
    assert(isInArithmeticSequence(7, 7, 2) == true);  // zero steps
    assert(isInArithmeticSequence(7, 7, -2) == true);
    // Large values to test overflow safety
    assert(isInArithmeticSequence(-1000000000LL, 1000000000LL, 1) == false); // too many steps positive, but checks divisibility
    assert(isInArithmeticSequence(-1000000000LL, -1000000000LL, 1000000000LL) == true);
    // Negative diff with positive C
    assert(isInArithmeticSequence(5, 2, 3) == false);
    // Negative diff with negative C (should be true if quotient non-negative)
    assert(isInArithmeticSequence(5, 2, -3) == true); // 5,2
    return 0;
}
// The main idea is based on the definition of an arithmetic sequence: starting from `A`, after `k` steps (where `k` is a non-negative integer), the value reached is `A + k * C`. We need to check if there exists a non-negative integer `k` such that `A + k * C == B`. Rearranging gives `k * C == B - A`. If `C == 0`, then the equation holds only if `B - A == 0`, i.e., `A == B`. If `C != 0`, then `k` must be an integer, so `(B - A)` must be divisible by `C`, and the quotient must be non-negative (since we can only add, not subtract, when using positive steps; if `C` is negative, the direction reverses but the quotient still must be non-negative because `k` is a step count). Edge cases: `A == B` always returns `true` for any `C` (including `C == 0`). When `C != 0`, we check divisibility and sign. Potential integer overflow is avoided by using long long for intermediate arithmetic. Time complexity is O(1) and space complexity is O(1).
