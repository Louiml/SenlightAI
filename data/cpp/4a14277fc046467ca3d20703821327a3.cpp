/*
Write a C++ function `int calculateGCD(int a, int b)` that computes the greatest common divisor (GCD) of two non-negative integers using a recursive Euclidean algorithm, but with a crucial fix: the recursion must base its termination on the **remainder** (i.e., when `a % b == 0` or `b % a == 0`), not on simple zero checks, to avoid division by zero. The function should handle the case where either argument is zero (returning the other argument as the GCD), and also handle the case where both are zero (returning 0). The function must be `const`-correct (although it doesn't modify inputs, mark parameters as `int a, int b` and no `const` needed on values). Guarantee that for any valid input, the function terminates without runtime errors and returns the correct GCD. Provide only the free function (no `main`), and then write tests using `assert` to verify correctness for typical cases, edge cases (zeros, swapped arguments), and larger numbers.
*/
#include <cstdlib> // for abs, but we'll handle non-negatives only

// Compute the greatest common divisor (GCD) of two non-negative integers.
// Handles zero arguments: gcd(0, b) = b, gcd(a, 0) = a, gcd(0,0) = 0.
int calculateGCD(int a, int b) {
    // Use absolute values to be safe (though inputs are non-negative, we keep it robust)
    a = std::abs(a);
    b = std::abs(b);

    // Base cases for zero
    if (a == 0) return b;
    if (b == 0) return a;

    // Standard recursive Euclidean algorithm:
    // gcd(a, b) = gcd(b, a % b) as long as b != 0
    // Since b is non-zero here, we can safely compute a % b.
    // The base case is when a % b == 0, then b is the GCD.
    if (a % b == 0) {
        return b;
    }
    // Otherwise, recurse with (b, a % b) — but careful: a % b < b always, so no risk of division by zero.
    return calculateGCD(b, a % b);
}
#include <cassert>

int main() {
    // Basic cases
    assert(calculateGCD(12, 18) == 6);
    assert(calculateGCD(18, 12) == 6); // swapped order
    assert(calculateGCD(7, 13) == 1);  // coprime
    assert(calculateGCD(100, 10) == 10);
    
    // Zero cases
    assert(calculateGCD(0, 5) == 5);
    assert(calculateGCD(5, 0) == 5);
    assert(calculateGCD(0, 0) == 0);
    
    // Large numbers (quick check)
    assert(calculateGCD(1071, 462) == 21);
    assert(calculateGCD(123456, 7890) == 6);
    
    // Same values
    assert(calculateGCD(8, 8) == 8);
    
    // One is multiple of other
    assert(calculateGCD(34, 17) == 17);
    
    // Negative inputs (if function handles absolute values, though spec says non-negative)
    // This test is optional but we include it to show robustness if needed.
    // assert(calculateGCD(-12, 18) == 6);
    
    return 0;
}
// The main algorithm is the Euclidean algorithm, which states that the GCD of two numbers is the same as the GCD of the smaller number and the remainder when the larger is divided by the smaller. The given snippet has a bug: it checks `if(a%b==0)` but then recursively calls `egcd_gcd(b%a,a)`, which can cause division by zero when `a` is larger than `b` (since `b%a` becomes `b` but `a` might be zero in later recursion). A robust fix is to ensure both arguments are positive and that the larger is always divided by the smaller, or to use the standard recursive form: `gcd(a, b) = gcd(b, a % b)` with base case `gcd(a, 0) = a`. We must also handle zeros: if `a == 0` return `b`, else if `b == 0` return `a`, else use recursion. The termination condition is when the remainder is zero, at which point the divisor is the GCD. Edge cases include inputs (0,0) → 0, (0,5) → 5, (5,0) → 5, (12,18) → 6, (18,12) → same, and large numbers. The recursion depth is at most logarithmic in the size of the numbers (O(log min(a,b)) recursive calls), and each call does constant work, so time complexity is O(log min(a,b)) and space complexity is O(log min(a,b)) due to call stack.
