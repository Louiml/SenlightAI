/*
Write a C++ function named `greatestCommonDivisor` that takes two non-negative integers as parameters and returns their greatest common divisor (GCD) using the Euclidean algorithm (recursive or iterative). The function must be self-contained with proper `const` correctness (parameters passed by value are fine since they are simple integers, but ensure the function itself is `const`-correct by not modifying any external state). Additionally, write a small test program that verifies the function with several pairs, including edge cases like one or both numbers being zero, equal numbers, and prime pairs. The function must handle the case where both inputs are zero by returning 0 (since GCD(0,0) is conventionally 0).
*/

#include <cstdlib> // for std::abs

// Recursively compute the greatest common divisor of two non-negative integers.
// Uses the Euclidean algorithm: gcd(a, b) = gcd(b, a % b) until b == 0.
int greatestCommonDivisor(int a, int b) {
    // Take absolute values to handle any negative inputs robustly.
    a = std::abs(a);
    b = std::abs(b);
    
    // Base case: if b is zero, gcd is a (or 0 if both are zero).
    if (b == 0) {
        return a;
    }
    // Recursive step: replace (a, b) with (b, a % b).
    return greatestCommonDivisor(b, a % b);
}

#include <cassert>

int greatestCommonDivisor(int a, int b); // declaration for test use

int main() {
    // Standard case
    assert(greatestCommonDivisor(12, 18) == 6);
    // One number is zero
    assert(greatestCommonDivisor(0, 5) == 5);
    assert(greatestCommonDivisor(5, 0) == 5);
    // Both zero
    assert(greatestCommonDivisor(0, 0) == 0);
    // Equal numbers
    assert(greatestCommonDivisor(7, 7) == 7);
    // Prime pair
    assert(greatestCommonDivisor(17, 13) == 1);
    // Larger numbers
    assert(greatestCommonDivisor(270, 192) == 6);
    // One is a multiple of the other
    assert(greatestCommonDivisor(100, 25) == 25);
    // Negative inputs (robustness)
    assert(greatestCommonDivisor(-12, 18) == 6);
    assert(greatestCommonDivisor(12, -18) == 6);
    return 0;
}

// The Euclidean algorithm works by repeatedly replacing the larger number with the difference (or more efficiently, the remainder) between the two numbers until one becomes zero. The GCD is the remaining non-zero number. For example, `gcd(48, 18)`: 48 % 18 = 12, then 18 % 12 = 6, then 12 % 6 = 0, so GCD is 6. The algorithm is correct because any common divisor of both numbers also divides their remainder, and vice versa. Edge cases: if either number is 0, the GCD is the other number (since GCD(n, 0) = n). If both are 0, we define GCD as 0. The recursion depth is O(log min(a,b)) because the remainder decreases rapidly; however, in the worst case (consecutive Fibonacci numbers), it is O(log n). Time complexity is O(log min(a,b)) and space complexity is O(log min(a,b)) for recursion stack (or O(1) if iterative). The solution below uses a recursive approach that is clean and matches the snippet’s style, but we ensure correctness for negative inputs by taking absolute values (though the task specifies non-negative, we can still handle robustness).
