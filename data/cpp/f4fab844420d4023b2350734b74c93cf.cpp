/*
Write a C++ function `factorialIterative` that computes the factorial of a non-negative integer `n` using an iterative loop instead of recursion. The function must return an `unsigned long long` to safely handle larger results, and it must treat the input as an `int`. Handle the edge case where `n` is 0 by returning 1. If the input is negative, the function should return 0 (since factorial is undefined for negative numbers). The function must be `const`-correct and should not use any recursive calls.
*/

#include <cstdint>

// Compute the factorial of a non-negative integer n iteratively.
// Returns 0 for negative inputs (undefined factorial).
unsigned long long factorialIterative(int n) {
    if (n < 0) {
        return 0ULL;
    }
    unsigned long long result = 1ULL;
    for (int i = 1; i <= n; ++i) {
        result *= static_cast<unsigned long long>(i);
    }
    return result;
}

#include <cassert>

int main() {
    assert(factorialIterative(0) == 1ULL);
    assert(factorialIterative(1) == 1ULL);
    assert(factorialIterative(2) == 2ULL);
    assert(factorialIterative(3) == 6ULL);
    assert(factorialIterative(4) == 24ULL);
    assert(factorialIterative(5) == 120ULL);
    assert(factorialIterative(6) == 720ULL);
    assert(factorialIterative(10) == 3628800ULL);
    assert(factorialIterative(-1) == 0ULL);
    assert(factorialIterative(-100) == 0ULL);
}

// The solution replaces recursion with a simple loop. We initialize a result variable to 1 (since 0! = 1). Then for each integer from 1 to `n`, we multiply the result by the current value. For `n = 0`, the loop body does not execute, so the result remains 1. For negative `n`, we immediately return 0 to indicate an invalid input. Since the function returns `unsigned long long`, we must be careful: if a large input like `n = 20` or higher, the result may overflow; the function will wrap around (unsigned overflow is well-defined), but for typical test cases we stick to small values. Time complexity is O(n) because we perform n multiplications. Space complexity is O(1) since we only use a few local variables.
