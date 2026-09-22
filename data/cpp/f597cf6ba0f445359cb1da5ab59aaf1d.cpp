// Write a standalone C++ function named `greatestCommonDivisor` that takes two integer parameters and returns their Greatest Common Divisor (GCD), also known as the Highest Common Factor (HCF). The function must handle positive integers, negative integers, zero, and cases where one or both numbers are zero. For zero and a non-zero number, the GCD is the absolute value of the non-zero number; for two zeros, the GCD is defined as 0. The function should not use the `std::gcd` standard library function; instead, implement the Euclidean algorithm iteratively for efficiency. The input numbers can be large, but the result must fit within the `int` range. The function must be `const`-correct (i.e., parameters passed by value, no modification of globals), and include appropriate `#include` directives and a short comment block describing the behavior.
The solution uses the Euclidean algorithm, which states that the GCD of two integers `a` and `b` is the same as the GCD of `b` and `a % b`, and when `b` becomes 0, `a` is the GCD. To handle negative numbers safely, we apply `std::abs` to both inputs before the algorithm, since the GCD is always non-negative. Special cases: if both numbers are zero, the loop would never run (since `b` is 0 initially), so we return 0 explicitly. If one number is zero and the other is non-zero, `std::abs` of the non-zero number is returned directly by the algorithm (the loop runs once, swapping values, then yields the non-zero absolute value). The iterative implementation avoids recursion overhead and potential stack overflow for large inputs. Time complexity is O(log(min(|a|,|b|))) in the worst case (Fibonacci numbers), and space complexity is O(1) auxiliary. Edge cases include: (0, 0) → 0, (0, 5) → 5, (-12, 18) → 6, (1, any) → 1, and large values like INT_MAX and INT_MAX-1.
#include <cstdlib> // for std::abs

// Returns the Greatest Common Divisor (GCD) of two integers.
// Handles positive, negative, and zero inputs. For zero and non-zero,
// returns the absolute value of the non-zero. For (0,0) returns 0.
int greatestCommonDivisor(int a, int b) {
    // Take absolute values to handle negatives; GCD is always non-negative.
    a = std::abs(a);
    b = std::abs(b);
    
    // Euclidean algorithm (iterative)
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a; // When b becomes 0, a holds the GCD
}
#include <cassert>
#include <climits>

// Assume the solution function greatestCommonDivisor is declared above.

int main() {
    // Basic positive numbers
    assert(greatestCommonDivisor(12, 18) == 6);
    assert(greatestCommonDivisor(7, 13) == 1);
    assert(greatestCommonDivisor(100, 10) == 10);
    
    // Negative numbers
    assert(greatestCommonDivisor(-12, 18) == 6);
    assert(greatestCommonDivisor(12, -18) == 6);
    assert(greatestCommonDivisor(-12, -18) == 6);
    
    // Zero cases
    assert(greatestCommonDivisor(0, 5) == 5);
    assert(greatestCommonDivisor(5, 0) == 5);
    assert(greatestCommonDivisor(0, 0) == 0);
    assert(greatestCommonDivisor(-7, 0) == 7);
    
    // Repeated values
    assert(greatestCommonDivisor(9, 9) == 9);
    assert(greatestCommonDivisor(1, 1) == 1);
    
    // Large values
    assert(greatestCommonDivisor(INT_MAX, INT_MAX - 1) == 1);
    assert(greatestCommonDivisor(INT_MAX, INT_MAX) == INT_MAX);
    
    return 0;
}
