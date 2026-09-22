// Write a C++ function named `fibonacciTerm` that takes a non-negative integer `n` and returns the `n`-th Fibonacci number using recursion, where `fibonacciTerm(0) = 0`, `fibonacciTerm(1) = 1`, and for `n >= 2`, `fibonacciTerm(n) = fibonacciTerm(n-1) + fibonacciTerm(n-2)`. The function must handle edge cases such as `n = 0` and `n = 1` correctly and should be declared with `const`-correctness where applicable (e.g., the parameter can be `const int n`). The solution should be a standalone free function without any `main` function or user input/output; it must be self-contained with necessary header includes.
The solution uses a simple recursive algorithm directly mirroring the mathematical definition of the Fibonacci sequence. The base cases are `n == 0` and `n == 1`, which return `n` directly, preventing infinite recursion. For larger `n`, the function calls itself twice: once for `n-1` and once for `n-2`, summing the results. This is the classic naive recursion. The main edge case is negative input — though the task specifies non-negative, the function can guard by returning `0` or handling it gracefully (but it's not required). The time complexity is exponential, specifically \(O(2^n)\), because each call spawns two branches, and the recursion tree has about \(2^n\) nodes. The space complexity is \(O(n)\) due to the maximum recursion depth being `n`, which occupies the call stack. For small `n` (e.g., up to 30) it’s acceptable, but for larger `n` it would be inefficient; however, the task only asks for correct implementation, not optimization.
// Returns the n-th Fibonacci number using recursion.
// Precondition: n must be a non-negative integer.
// For n = 0 returns 0, n = 1 returns 1, otherwise fib(n-1) + fib(n-2).
int fibonacciTerm(const int n) {
    if (n <= 1) {
        return n;  // Base cases handle n == 0 and n == 1.
    }
    return fibonacciTerm(n - 1) + fibonacciTerm(n - 2);
}
#include <cassert>

int fibonacciTerm(const int n); // Declaration for test use.

int main() {
    // Basic base cases.
    assert(fibonacciTerm(0) == 0);
    assert(fibonacciTerm(1) == 1);
    // Known sequence values.
    assert(fibonacciTerm(2) == 1);
    assert(fibonacciTerm(3) == 2);
    assert(fibonacciTerm(4) == 3);
    assert(fibonacciTerm(5) == 5);
    assert(fibonacciTerm(6) == 8);
    assert(fibonacciTerm(7) == 13);
    assert(fibonacciTerm(10) == 55);
    assert(fibonacciTerm(15) == 610);
    return 0;
}
