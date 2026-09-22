/*
Write a C++ function named `factorial` that accepts a single non-negative integer `n` and returns its factorial as an `unsigned long long`. The function must handle the edge case where `n = 0` (returning 1), and for any `n` where the result would overflow `unsigned long long` (i.e., `n > 20`), the function should return `0` to indicate an out-of-range condition. The solution should be written as a standalone function without a `main` wrapper, and must include appropriate `const` correctness where applicable.
*/
#include <cstdint>

// Return the factorial of n as unsigned long long.
// Returns 0 if n > 20 (overflow would occur).
unsigned long long factorial(const int n) {
    if (n > 20) {
        return 0;  // Overflow beyond unsigned long long
    }
    unsigned long long result = 1;
    for (int i = 2; i <= n; ++i) {
        result *= static_cast<unsigned long long>(i);
    }
    return result;
}
#include <cassert>
#include <cstdint>

// (the solution function is assumed to be defined above)

int main() {
    assert(factorial(0) == 1ULL);
    assert(factorial(1) == 1ULL);
    assert(factorial(5) == 120ULL);
    assert(factorial(10) == 3628800ULL);
    assert(factorial(20) == 2432902008176640000ULL);  // max representable
    assert(factorial(21) == 0ULL);                    // overflow sentinel
    assert(factorial(15) == 1307674368000ULL);
    assert(factorial(7) == 5040ULL);
    assert(factorial(3) == 6ULL);
    assert(factorial(12) == 479001600ULL);
}
// The core algorithm is iterative: multiply a running product by each integer from 2 up to `n`. Initialize the result to `1`. For `n = 0` or `n = 1`, the loop does not execute and the correct value `1` is returned. Edge cases:  
// - `n = 0` → factorial is `1` by definition.  
// - `n > 20` → `21! = 51090942171709440000 > 2^64 - 1 ≈ 1.8446744e19`, so overflow occurs; return `0` as a sentinel for invalid/out-of-range input.  
// - The input is guaranteed to be non-negative, so no negative handling is required.  
// Time complexity is O(n) for the loop, and auxiliary space is O(1). The function uses `unsigned long long` to maximize the representable range. The function is declared `const`-correct by taking the input by value and not modifying it.
