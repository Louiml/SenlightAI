// Write a C++ function named `factorial` that takes a single non-negative integer `n` as input and returns its factorial as an unsigned 64-bit integer type (`unsigned long long`). The factorial of 0 is defined as 1. The function should handle values of `n` up to 20 without overflow. For values greater than 20, the function may return a wrapped or incorrect result, but it must not crash or cause undefined behavior (use `unsigned long long` arithmetic naturally). Do not use recursion; implement it iteratively using a loop.
// The standard iterative factorial algorithm multiplies an accumulator starting at 1 by every integer from 2 up to `n`. For `n = 0` or `n = 1`, the loop does not execute, and the accumulator remains 1, correctly returning 1. The main edge cases are `n = 0` (returns 1) and `n = 1` (returns 1). Since we use `unsigned long long`, overflow for `n > 20` happens due to wrap-around modulo 2^64, but this is well-defined in C++ for unsigned types and does not produce undefined behavior. Time complexity is O(n) because we iterate `n-1` times. Space complexity is O(1) as we use only constant extra memory.
#include <cstdint>

// Compute the factorial of a non-negative integer n iteratively.
// Returns 1 for n = 0 or n = 1, and the product 2*3*...*n otherwise.
// Space complexity: O(1), time complexity: O(n).
unsigned long long factorial(int n) {
    unsigned long long result = 1;
    for (int i = 2; i <= n; ++i) {
        result *= static_cast<unsigned long long>(i);
    }
    return result;
}
#include <cassert>

// Forward declaration of the function under test.
unsigned long long factorial(int n);

int main() {
    // Base cases: 0! and 1! both equal 1
    assert(factorial(0) == 1ULL);
    assert(factorial(1) == 1ULL);
    // Small known values
    assert(factorial(2) == 2ULL);
    assert(factorial(3) == 6ULL);
    assert(factorial(4) == 24ULL);
    assert(factorial(5) == 120ULL);
    // Larger values that fit in 64-bit unsigned
    assert(factorial(10) == 3628800ULL);
    assert(factorial(15) == 1307674368000ULL);
    // Maximum representable factorial (20! = 2432902008176640000)
    assert(factorial(20) == 2432902008176640000ULL);
    // 21! exceeds 64-bit, but the function should still return without crashing
    // (result wraps modulo 2^64; we just check it's non-zero here to confirm no UB)
    unsigned long long wrapped = factorial(21);
    assert(wrapped != 0ULL);  // 21! mod 2^64 is non-zero
    return 0;
}
