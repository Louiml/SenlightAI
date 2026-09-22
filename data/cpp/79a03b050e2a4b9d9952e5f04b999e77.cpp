Write a standalone C++ function named `computeGCD` that takes two integers `a` and `b` (which may be negative, zero, or positive) and returns their greatest common divisor (GCD) as a non-negative integer. The function must use the Euclidean algorithm (repeatedly taking the remainder until zero) to compute the result, and it must handle edge cases such as zero inputs and negative values correctly. Your implementation should be self-contained, include necessary headers, and be suitable for direct use in a test program.
#include <cassert>

int main() {
    // Positive numbers
    assert(computeGCD(12, 18) == 6);
    assert(computeGCD(100, 25) == 25);
    
    // One zero input
    assert(computeGCD(0, 5) == 5);
    assert(computeGCD(7, 0) == 7);
    
    // Both zero
    assert(computeGCD(0, 0) == 0);
    
    // Negative inputs (GCD is always non-negative)
    assert(computeGCD(-12, 18) == 6);
    assert(computeGCD(12, -18) == 6);
    assert(computeGCD(-12, -18) == 6);
    
    // Coprime numbers
    assert(computeGCD(17, 23) == 1);
    assert(computeGCD(1, 100) == 1);
    
    // Large numbers
    assert(computeGCD(1071, 462) == 21);
    
    return 0;
}
#include <cstdlib>   // for std::abs
#include <algorithm> // for std::max (optional, not strictly needed)

// Compute the greatest common divisor of two integers.
// Returns a non-negative integer. Handles negative inputs and zero.
int computeGCD(int a, int b) {
    // Use non-negative values for the Euclidean algorithm.
    a = std::abs(a);
    b = std::abs(b);
    
    // Euclidean algorithm: repeat until remainder is zero.
    while (b != 0) {
        int remainder = a % b;
        a = b;
        b = remainder;
    }
    return a;  // When b is zero, 'a' holds the GCD.
}
// The provided code snippet computes the GCD of two integers using the Euclidean algorithm, but it assumes both inputs are positive (since `a%b` with negative inputs can produce negative remainders and the loop would not terminate correctly). The solution must first take the absolute values of both inputs to ensure the algorithm works with non-negative numbers. The GCD of a number and zero is the absolute value of the non-zero number, and the GCD(0,0) is conventionally 0. After converting to absolute values, repeatedly replace `(a, b)` with `(b, a % b)` until `b` becomes zero, then return `a`. This works because the Euclidean algorithm preserves the GCD through each step. Edge cases: negative inputs are handled via `std::abs`; zero inputs are handled naturally by the loop condition (if `b` is zero, the loop exits and returns `a`). Time complexity is O(log(min(a,b))) in the worst case, and space complexity is O(1) since only a few integer variables are used.
