// Write a C++ function named `leastCommonMultiple` that takes two non-negative integers `a` and `b` and returns their least common multiple (LCM). The function must compute the LCM using the relationship `LCM(a,b) = a / GCD(a,b) * b` (to avoid overflow issues from multiplying before dividing), where GCD is computed using Euclid's recursive algorithm. Handle the edge case where either input is zero by returning 0 (since LCM(0, x) = 0 for any non-negative x). Do not use any built-in GCD or LCM functions; implement GCD yourself. The function should be `const`-correct, take parameters by value, and be placed in a header-only style (no `main` function). The inputs are non-negative and fit within the range of `int`, but their product may exceed `int`, so use `long long` for intermediate calculations and the return type to ensure correctness.

The solution relies on Euclid's algorithm to compute the greatest common divisor (GCD) of two numbers in `O(log(min(a,b)))` time. The algorithm repeatedly replaces `(a, b)` with `(b, a % b)` until `b` becomes 0, at which point `a` is the GCD. For the LCM, the standard formula is `a * b / GCD(a,b)`, but to avoid overflow when `a` and `b` are large, we compute `(a / GCD(a,b)) * b` first, because the division by GCD reduces one factor before multiplication. Since the inputs are non-negative, we handle the case where either is zero by returning 0 immediately (LCM(0, anything) = 0). The function uses recursion for GCD, which is clean and matches the classic implementation. Edge cases include: both inputs zero (return 0), one zero (return 0), and large values where `a * b` exceeds `int` range—our use of `long long` for the multiplication and return type safely handles this. Time complexity is `O(log(min(a,b)))` due to Euclid's algorithm, and space complexity is `O(log(min(a,b)))` due to recursion stack depth, though it can be considered `O(1)` if iterative, but here recursion is used.

#include <cstdint>

// Compute the greatest common divisor of two non-negative integers using Euclid's algorithm.
// Recursive version: returns GCD(a, b). Assumes b >= 0; if b == 0, returns a.
int gcd(int a, int b) {
    if (b == 0) {
        return a;
    }
    return gcd(b, a % b);
}

// Compute the least common multiple of two non-negative integers.
// Returns 0 if either input is 0. Uses long long to avoid overflow.
long long leastCommonMultiple(int a, int b) {
    if (a == 0 || b == 0) {
        return 0;
    }
    // Compute (a / gcd) * b to avoid overflow from a * b.
    long long result = static_cast<long long>(a / gcd(a, b)) * static_cast<long long>(b);
    return result;
}

#include <cassert>

int main() {
    // Basic positive cases
    assert(leastCommonMultiple(12, 15) == 60);
    assert(leastCommonMultiple(4, 6) == 12);
    assert(leastCommonMultiple(21, 6) == 42);
    
    // One input is zero
    assert(leastCommonMultiple(0, 5) == 0);
    assert(leastCommonMultiple(7, 0) == 0);
    assert(leastCommonMultiple(0, 0) == 0);
    
    // Same numbers
    assert(leastCommonMultiple(8, 8) == 8);
    
    // Large numbers that would overflow int if multiplied directly
    // GCD(100000, 99999) = 1, so LCM = 100000 * 99999 = 9999900000 (fits in long long)
    assert(leastCommonMultiple(100000, 99999) == 9999900000LL);
    
    // One is a multiple of the other
    assert(leastCommonMultiple(7, 14) == 14);
    assert(leastCommonMultiple(14, 7) == 14);
    
    // Primes
    assert(leastCommonMultiple(17, 19) == 323);
    
    return 0;
}
