// Write a standalone C++ function named `greatestCommonDivisor` that takes two non-negative integer parameters (long long for safety) and returns their greatest common divisor (GCD) using the Euclidean algorithm. The function must handle the case where either or both numbers are zero, returning the non‑zero value in that case. Also, ensure the function is efficient for very large inputs (e.g., up to 10^18). Do not use built‑in functions like `std::gcd` or `__gcd`; implement the algorithm manually. The function should be declared with proper `const` correctness (though parameters are passed by value, you may mark the function itself as `constexpr` if desired) and reside in a header‑style presentation (but for this task, just provide the function definition). Provide a reference solution and test code that verifies correctness against known cases including zeros, equal numbers, and large values.
// The Euclidean algorithm works by repeatedly replacing the larger number with the remainder of dividing the larger by the smaller, until one becomes zero. The GCD is then the remaining non‑zero number. If both numbers are zero, the mathematical convention is GCD(0,0) = 0, which the algorithm handles naturally (loop won’t run, return 0).  
// Edge cases:  
// - If one input is zero and the other is non‑zero, the loop immediately stops because `b == 0`, returning the non‑zero number.  
// - If both are equal, the loop runs once: `a % b` becomes `0`, and the function returns `b` (equal to the input).  
// - For very large numbers (up to 10^18), using `long long` is safe, but the modulo operation is still safe for these ranges (no overflow).  
// Time complexity is O(log(min(a,b))) in the worst case (Fibonacci numbers), and O(1) space. The algorithm is iterative and avoids recursion depth issues.
#include <cstdint>

// Compute the greatest common divisor of two non-negative integers.
// Uses the Euclidean algorithm iteratively.
// Handles zero cases: GCD(0,0)=0, GCD(a,0)=a, GCD(0,b)=b.
long long greatestCommonDivisor(long long a, long long b) {
    // Euclidean algorithm: while both not zero, reduce using modulo.
    while (b != 0) {
        long long remainder = a % b;
        a = b;
        b = remainder;
    }
    // When b becomes 0, a holds the GCD (or 0 if both were 0).
    return a;
}
#include <cassert>

int main() {
    // Basic cases
    assert(greatestCommonDivisor(6, 20) == 2);
    assert(greatestCommonDivisor(20, 6) == 2);
    assert(greatestCommonDivisor(17, 19) == 1);
    
    // Zero cases
    assert(greatestCommonDivisor(0, 0) == 0);
    assert(greatestCommonDivisor(12, 0) == 12);
    assert(greatestCommonDivisor(0, 15) == 15);
    
    // Equal numbers
    assert(greatestCommonDivisor(100, 100) == 100);
    assert(greatestCommonDivisor(7, 7) == 7);
    
    // Large numbers
    assert(greatestCommonDivisor(1000000000000000000LL, 1000000000000000000LL) == 1000000000000000000LL);
    assert(greatestCommonDivisor(123456789123456789LL, 987654321987654321LL) == 1); // likely coprime
    
    // More complex known values
    assert(greatestCommonDivisor(48, 18) == 6);
    assert(greatestCommonDivisor(270, 192) == 6);
    assert(greatestCommonDivisor(1071, 462) == 21);
    
    // Large but not coprime
    assert(greatestCommonDivisor(1000000000000LL, 500000000000LL) == 500000000000LL);
    
    return 0;
}
