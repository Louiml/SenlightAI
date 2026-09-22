// Write a C++ function `long long leastCommonMultiple(long long a, long long b)` that takes two positive integers (each up to \(10^9\)) and returns their least common multiple (LCM). The function must handle inputs where one number is a multiple of the other, and it must avoid overflow that could occur from multiplying the two numbers directly before reducing by the greatest common divisor (GCD). The solution must use the Euclidean algorithm to compute the GCD, then compute the LCM as `(a / gcd) * b` (dividing first to prevent overflow). The function should not read from or write to standard input/output; it should be pure and usable in unit tests.
The LCM of two numbers \(a\) and \(b\) is given by \(|a \cdot b| / \gcd(a,b)\). To avoid overflow when \(a\) and \(b\) are large (each up to \(10^9\), so their product can reach \(10^{18}\) which fits in `long long`, but it’s safer to divide first), compute the GCD using the Euclidean algorithm. The algorithm works by repeatedly replacing `(a, b)` with `(b, a % b)` until `b` becomes zero; the last non‑zero remainder is the GCD. Edge cases: (1) if either number is 0, the LCM is conventionally 0 (though the problem states positive inputs, we can still guard); (2) when one divides the other, the GCD is the smaller number, and the LCM is the larger; (3) the division is performed before multiplication to keep intermediate results within `long long`. Time complexity is \(O(\log(\min(a,b)))\) for the Euclidean algorithm, and space complexity is \(O(1)\).
#include <cstdlib> // for std::llabs (though inputs are positive, kept for safety)

// Compute the greatest common divisor of two non-negative integers.
long long gcd(long long a, long long b) {
    while (b != 0) {
        long long temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// Return the least common multiple of two positive integers.
// Divides before multiplying to avoid overflow.
long long leastCommonMultiple(long long a, long long b) {
    if (a == 0 || b == 0) {
        return 0;
    }
    // Use absolute values in case of negative inputs (though problem says positive).
    long long absA = std::llabs(a);
    long long absB = std::llabs(b);
    long long divisor = gcd(absA, absB);
    return (absA / divisor) * absB;
}
#include <cassert>

int main() {
    // Basic cases
    assert(leastCommonMultiple(4, 6) == 12);
    assert(leastCommonMultiple(12, 18) == 36);
    assert(leastCommonMultiple(21, 6) == 42);
    
    // One divides the other
    assert(leastCommonMultiple(8, 4) == 8);
    assert(leastCommonMultiple(5, 15) == 15);
    assert(leastCommonMultiple(7, 7) == 7);
    
    // Co-prime numbers
    assert(leastCommonMultiple(13, 17) == 221);
    
    // Large numbers, ensure no overflow (product = 1e18, fits in long long)
    assert(leastCommonMultiple(1000000000, 999999937) == 999999937000000000LL);
    
    // Edge case: zero (not in problem statement but safe)
    assert(leastCommonMultiple(0, 5) == 0);
    assert(leastCommonMultiple(5, 0) == 0);
    
    // Negative inputs (not expected but correct behavior)
    assert(leastCommonMultiple(-4, 6) == 12);
    
    return 0;
}
