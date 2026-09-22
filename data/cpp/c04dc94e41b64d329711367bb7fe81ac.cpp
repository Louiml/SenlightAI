/*
Write a C++ function named `fibonacciValue` that takes a single non-negative integer `n` and returns the `n`-th Fibonacci number, where `fibonacciValue(0) = 0`, `fibonacciValue(1) = 1`, `fibonacciValue(2) = 1`, and for `n > 2`, `fibonacciValue(n) = fibonacciValue(n-1) + fibonacciValue(n-2)`. The function must handle all valid inputs without recursion, using an iterative approach to compute the result. The function should be declared as `int fibonacciValue(int n)` and must be `const`-correct (i.e., it does not modify any input or global state). Assume the input is always a non-negative integer, and that the result fits within the range of a 32-bit signed integer (`int`). The function must be self-contained, include only necessary headers, and be ready for direct use in a test program.
*/

#include <cstddef> // For size_t if needed, but not required here

// Returns the n-th Fibonacci number (0-indexed) using an iterative approach.
// Assumes n is non-negative and the result fits in a 32-bit signed integer.
int fibonacciValue(int n) {
    if (n == 0) return 0;
    if (n == 1 || n == 2) return 1;

    int twoBack = 1; // F(n-2) for n=3 start
    int oneBack = 1; // F(n-1) for n=3 start
    int current = 0;

    for (int i = 3; i <= n; ++i) {
        current = twoBack + oneBack;
        twoBack = oneBack;
        oneBack = current;
    }
    return current;
}

#include <cassert>

int main() {
    // Base cases
    assert(fibonacciValue(0) == 0);
    assert(fibonacciValue(1) == 1);
    assert(fibonacciValue(2) == 1);

    // Standard values
    assert(fibonacciValue(3) == 2);
    assert(fibonacciValue(4) == 3);
    assert(fibonacciValue(5) == 5);
    assert(fibonacciValue(6) == 8);
    assert(fibonacciValue(7) == 13);
    assert(fibonacciValue(8) == 21);
    assert(fibonacciValue(9) == 34);

    // Larger value that still fits in int
    assert(fibonacciValue(20) == 6765);
    assert(fibonacciValue(30) == 832040);

    return 0;
}

// The Fibonacci sequence is defined by `F(0)=0`, `F(1)=1`, `F(2)=1`, and `F(n)=F(n-1)+F(n-2)` for `n≥3`. The naive recursive approach has exponential time complexity, so we use an iterative method. We maintain two variables, `twoBack` (representing `F(n-2)`) and `oneBack` (representing `F(n-1)`), and update them in a loop from `n=2` upward. For `n=0` or `n=1`, we return the base cases directly. For `n≥2`, we loop from `3` to `n`, computing `current = twoBack + oneBack`, then shifting `twoBack = oneBack` and `oneBack = current`. This yields `O(n)` time complexity and `O(1)` auxiliary space. Edge cases: `n=0` returns `0`, `n=1` and `n=2` return `1`. For large `n` (up to ~46 for 32-bit int), the values fit within `int`, but for `n` beyond that, integer overflow may occur—this is acceptable given the problem constraints. The solution avoids recursion, making it stack-safe and efficient.
