// Write a C++ function named `fibonacciTerm` that takes a single non-negative integer `n` and returns the `n`-th term of the Fibonacci sequence, where the sequence is defined as: `fib(1) = 1`, `fib(2) = 1`, and for `n > 2`, `fib(n) = fib(n-1) + fib(n-2)`. The function must be implemented recursively, and it should handle the edge case `n = 0` by returning `0`. For any input, the function must not use loops, global variables, or iterative constructs. The task is to provide a standalone, reusable function (no `main` included in the solution) that can be called from external test code to validate its correctness for various inputs including small values, zero, and possibly larger values where the result fits within an `int`.
// The solution uses a straightforward recursive definition of the Fibonacci sequence. The base cases are `n == 0` (return 0), `n == 1` (return 1), and `n == 2` (return 1). For any `n > 2`, the function recursively calls itself with `n-1` and `n-2` and returns their sum. Edge cases: if `n` is negative, it is not part of the specification, but we can either ignore or handle it gracefully—here we assume input is non-negative, but for robustness we can return 0 for `n < 0` if needed. The time complexity is exponential, specifically \(O(2^n)\), because each call branches into two recursive calls, leading to repeated computation of the same subproblems. Space complexity is \(O(n)\) due to the recursion stack depth, which equals `n` in the worst case. This is inefficient for large `n` (e.g., `n > 30` may take noticeable time), but it matches the requirement of a recursive subprogram and is acceptable for small to moderate inputs.
#include <cassert>  // Not required for function, but included for safety—remove if needed.

// Return the n-th Fibonacci number (0-indexed: fib(0)=0, fib(1)=1, fib(2)=1).
// Precondition: n >= 0. If n < 0, returns 0 for safety.
int fibonacciTerm(int n) {
    if (n <= 0) {
        return 0;  // Handles n == 0 and negative inputs gracefully.
    }
    if (n == 1 || n == 2) {
        return 1;
    }
    return fibonacciTerm(n - 1) + fibonacciTerm(n - 2);
}
int main() {
    assert(fibonacciTerm(0) == 0);
    assert(fibonacciTerm(1) == 1);
    assert(fibonacciTerm(2) == 1);
    assert(fibonacciTerm(3) == 2);
    assert(fibonacciTerm(4) == 3);
    assert(fibonacciTerm(5) == 5);
    assert(fibonacciTerm(6) == 8);
    assert(fibonacciTerm(7) == 13);
    assert(fibonacciTerm(10) == 55);
    assert(fibonacciTerm(15) == 610);
    // Negative input is not in spec but test for robustness.
    assert(fibonacciTerm(-5) == 0);
    return 0;
}
