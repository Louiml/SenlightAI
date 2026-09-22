// Write a C++ function named `greatestCommonDivisor` that takes two non-negative integers `a` and `b` (where both may be zero, but not both) and returns their greatest common divisor (GCD) using the Euclidean algorithm implemented with recursion. The function must handle the case where either input is zero correctly, and it must be `const`-correct (i.e., parameters passed by value are fine, but ensure no side effects). The function should be declared in a header-free standalone manner, and you must provide test assertions verifying correctness for typical cases, edge cases including zeros, and cases where one number is a multiple of the other.

#include <cassert>

int main() {
    // Typical cases
    assert(greatestCommonDivisor(18, 48) == 6);
    assert(greatestCommonDivisor(48, 18) == 6);
    assert(greatestCommonDivisor(100, 10) == 10);
    assert(greatestCommonDivisor(17, 19) == 1);

    // Edge cases with one zero
    assert(greatestCommonDivisor(0, 5) == 5);
    assert(greatestCommonDivisor(7, 0) == 7);

    // Equal numbers
    assert(greatestCommonDivisor(12, 12) == 12);

    // Larger values
    assert(greatestCommonDivisor(270, 192) == 6);
    assert(greatestCommonDivisor(1, 1) == 1);
}

#include <cstdint>

// Computes the greatest common divisor of two non-negative integers.
// Both a and b may be zero, but not both simultaneously.
// Uses the recursive Euclidean algorithm.
int greatestCommonDivisor(int a, int b) {
    if (b == 0) {
        return a;
    }
    return greatestCommonDivisor(b, a % b);
}

// The Euclidean algorithm states that the GCD of two numbers `a` and `b` (with `a > b` typically, but works generally) is the same as the GCD of `b` and `a % b`. The base case occurs when `b == 0`, in which case the GCD is `a`. This recursive process reduces the problem size rapidly because `a % b` is always less than `b`. For example, `gcd(48, 18)` → `gcd(18, 12)` → `gcd(12, 6)` → `gcd(6, 0)` → 6. Edge cases: if `b` is zero, return `a`. If `a` is zero and `b` is nonzero, then `gcd(0, b)` becomes `gcd(b, 0)` after one recursive call, returning `b`. If both are zero, the algorithm would never reach a base case (infinite recursion), so the problem explicitly forbids both being zero. The time complexity is O(log(min(a, b))) in the worst case, because each recursion reduces the numbers significantly. The space complexity is O(log(min(a, b))) due to the recursion stack depth, which is acceptable for typical integer ranges but could be optimized to iterative if needed.
