Write a C++ function `unsigned long long recursiveFactorial(int n)` that computes the factorial of a non-negative integer `n` using recursion. The input `n` is guaranteed to be between 0 and 20 inclusive (so the result fits within `unsigned long long`). The function must handle the base case for `n == 0` returning 1, and for any invalid negative input it should return 0 (since factorial is undefined for negative integers). Your function must be recursive (not iterative), use `unsigned long long` as the return type, and be `const`-correct where appropriate. You are not allowed to use any external libraries beyond standard headers; include only `<iostream>` if needed in tests. Provide the implementation only for the free function (no `main`), and then write test assertions in a separate `main` function.

The solution uses recursion to compute the factorial: if `n` is negative, return 0 (an error sentinel). If `n == 0`, the base case returns 1. Otherwise, the recursive case returns `n * recursiveFactorial(n-1)`. Edge cases include `n = 0` (returns 1), `n = 1` (returns 1), negative input (returns 0), and large `n` up to 20 (fits in `unsigned long long`; maximum 20! = 2,432,902,008,176,640,000). Time complexity is O(n) due to `n+1` recursive calls, and space complexity is O(n) because the call stack grows to depth `n+1`. No loops or iterative optimization are used since recursion is required.

#include <cstddef> // For size_t if needed, but not strictly necessary here

/**
 * Computes the factorial of a non-negative integer recursively.
 * Returns 0 for negative input (undefined domain).
 * Valid range: 0 <= n <= 20 (fits in unsigned long long).
 */
unsigned long long recursiveFactorial(int n) {
    if (n < 0) {
        return 0ULL; // Invalid input
    }
    if (n == 0) {
        return 1ULL; // Base case: 0! = 1
    }
    // Recursive case: n! = n * (n-1)!
    return static_cast<unsigned long long>(n) * recursiveFactorial(n - 1);
}

#include <cassert>

int main() {
    // Base case
    assert(recursiveFactorial(0) == 1ULL);
    // Small values
    assert(recursiveFactorial(1) == 1ULL);
    assert(recursiveFactorial(2) == 2ULL);
    assert(recursiveFactorial(3) == 6ULL);
    assert(recursiveFactorial(4) == 24ULL);
    assert(recursiveFactorial(5) == 120ULL);
    // Larger values
    assert(recursiveFactorial(10) == 3628800ULL);
    assert(recursiveFactorial(20) == 2432902008176640000ULL);
    // Invalid negative input returns 0
    assert(recursiveFactorial(-1) == 0ULL);
    assert(recursiveFactorial(-100) == 0ULL);
    return 0;
}
