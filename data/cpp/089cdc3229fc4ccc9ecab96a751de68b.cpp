/*
Write a C++ function named `fibonacciNumber` that takes a non-negative integer `n` and returns the `n`-th Fibonacci number, where the sequence is defined as `F(0) = 0`, `F(1) = 1`, and `F(n) = F(n-1) + F(n-2)` for `n >= 2`. The function must handle inputs up to `n = 20` without overflow (result fits in 32-bit `int`) and must use recursion. If the input is negative, the function should return `-1` as an error indication instead of causing undefined behavior. The task is to implement only this function (no `main` function), with proper `const` correctness where applicable.
*/

// Return the n-th Fibonacci number (F(0)=0, F(1)=1).
// For n < 0, returns -1 to indicate invalid input.
int fibonacciNumber(const int n) {
    if (n < 0) {
        return -1;               // invalid input
    }
    if (n == 0) {
        return 0;                // base case F(0)
    }
    if (n == 1) {
        return 1;                // base case F(1)
    }
    return fibonacciNumber(n - 1) + fibonacciNumber(n - 2); // recursive definition
}

#include <cassert>

int fibonacciNumber(const int n); // declaration for testing

int main() {
    // Base cases
    assert(fibonacciNumber(0) == 0);
    assert(fibonacciNumber(1) == 1);
    // Small values
    assert(fibonacciNumber(2) == 1);
    assert(fibonacciNumber(3) == 2);
    assert(fibonacciNumber(4) == 3);
    assert(fibonacciNumber(5) == 5);
    // Larger values within range
    assert(fibonacciNumber(10) == 55);
    assert(fibonacciNumber(15) == 610);
    assert(fibonacciNumber(20) == 6765);
    // Invalid negative input
    assert(fibonacciNumber(-1) == -1);
    assert(fibonacciNumber(-100) == -1);
    return 0;
}

// The solution uses a direct recursive implementation mirroring the mathematical definition. The base cases are `n == 0` (return 0) and `n == 1` (return 1). For `n >= 2`, the function calls itself with `n-1` and `n-2` and sums the results. The key edge case is negative input: since the Fibonacci sequence is only defined for non-negative indices, the function must explicitly check for `n < 0` and return `-1` to signal an error. This is important because without this check, the recursion would run indefinitely or cause stack overflow. Time complexity is exponential \(O(2^n)\) due to repeated recomputation of overlapping subproblems; space complexity is \(O(n)\) due to the recursion call stack depth (maximum depth is `n`). The value of `n` is passed by value, so we can mark the parameter as `const int` inside the function declaration for clarity, but in practice it does not affect behavior. No external libraries beyond `<cstdint>` or basic headers are needed, but we include `<stdexcept>` optionally to note that we avoid exceptions for simplicity.
