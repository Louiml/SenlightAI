Write a C++ function `int computeGCD(int a, int b)` that calculates the greatest common divisor (GCD) of two non-negative integers using the Euclidean algorithm via recursion. The function must handle the case where either or both inputs are zero (the GCD of 0 and any integer is the absolute value of the non-zero integer, and GCD(0,0) is defined as 0). The function must accept the parameters by value, use `const` where appropriate, and avoid any standard library functions other than basic arithmetic operations. The solution must be self-contained (no external dependencies beyond `<iostream>` for testing) and must not include a `main` function in the solution section, only the free function.
#include <cassert>

int computeGCD(const int a, const int b); // declaration for testing

int main() {
    // Standard cases
    assert(computeGCD(48, 18) == 6);
    assert(computeGCD(100, 25) == 25);
    assert(computeGCD(17, 5) == 1);

    // Zero cases
    assert(computeGCD(0, 12) == 12);
    assert(computeGCD(12, 0) == 12);
    assert(computeGCD(0, 0) == 0);

    // Equal numbers
    assert(computeGCD(7, 7) == 7);
    assert(computeGCD(1, 1) == 1);

    // Larger numbers
    assert(computeGCD(270192, 31416) == 24);
    assert(computeGCD(123456, 789012) == 12);
    assert(computeGCD(1, 0) == 1);
    assert(computeGCD(0, 1) == 1);

    return 0;
}
// Returns the greatest common divisor of two non-negative integers a and b.
// Uses the Euclidean algorithm via recursion.
// GCD(0, b) = b, GCD(a, 0) = a, GCD(0, 0) = 0.
int computeGCD(const int a, const int b) {
    // Base case: when b becomes 0, a holds the GCD.
    if (b == 0) {
        return a;
    }
    // Recursive step: replace (a, b) with (b, a % b).
    return computeGCD(b, a % b);
}
// The Euclidean algorithm repeatedly replaces the larger number with the remainder of dividing it by the smaller number until one of them becomes zero. The non-zero number at that point is the GCD. Recursively, `computeGCD(a, b)` can be defined as: if `b == 0`, return `a`; otherwise, return `computeGCD(b, a % b)`. This works because `gcd(a, b) == gcd(b, a % b)`. Edge cases: if `a == 0` and `b != 0`, the function will eventually return `b` after swapping (since `computeGCD(0, b)` recurses to `computeGCD(b, 0 % b)` which is `computeGCD(b, 0)` → returns `b`). If both are zero, the base case triggers immediately (since `b == 0` is true) and returns `a` (which is 0), so the result is 0. The recursion depth is proportional to the logarithm of the larger input (Fibonacci-like worst-case), so time complexity is O(log(min(a,b))) for typical inputs, and O(1) auxiliary space if we ignore recursion stack (but realistically O(log(min(a,b))) stack space due to recursion). Negative inputs are not expected per the task, but if they occur, the modulo operation may yield negative values; the task specifies non-negative inputs, so we can assume correct behavior. However, to be robust, we could use absolute values, but the specification says non-negative, so it is safe.
