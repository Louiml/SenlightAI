// Write a C++ function named `fibonacciTerm` that takes a non-negative integer `n` and returns the `n`-th Fibonacci number using recursion, where `fib(0) = 0`, `fib(1) = 1`, and `fib(n) = fib(n-1) + fib(n-2)` for `n >= 2`. The function must be const-correct (the parameter should be `const int n`), should not rely on global state, and must handle the base cases properly. Your implementation should exactly match the behavior of the provided snippet but be more robust: assume the input is always non-negative (≥0), and you may return an `int` (note that for `n` above 46 the result may overflow, but you do not need to handle that). Provide the function only, without a `main` or any other wrapper.
The solution is a direct recursive implementation of the Fibonacci sequence. The base cases are when `n == 0` (returns 0) and `n == 1` (returns 1). For any `n >= 2`, the function returns the sum of `fibonacciTerm(n-1)` and `fibonacciTerm(n-2)`. Edge cases include `n = 0` and `n = 1` (must not recurse), and very small values like `n = 2` (which recurses to base cases). The recursion tree for `n` has exponential branching: each call spawns two more calls, leading to a time complexity of \(O(2^n)\) and space complexity \(O(n)\) due to the recursion depth (the maximum stack depth is `n`). This is inefficient for large `n`, but it is the exact algorithm from the snippet and is intended for teaching recursion. The function is pure and does not modify any external state, and the parameter is passed by value as `const int` to indicate it is not modified.
#include <cstddef> // for std::size_t, though not strictly needed

// Return the n-th Fibonacci number using recursion.
// Precondition: n >= 0.
int fibonacciTerm(const int n) {
    // Base cases: fib(0) = 0, fib(1) = 1
    if (n < 2) {
        return n;
    }
    // Recursive case: fib(n) = fib(n-1) + fib(n-2)
    return fibonacciTerm(n - 1) + fibonacciTerm(n - 2);
}
#include <cassert>

// Declaration of the solution function (assume it is included above).
int fibonacciTerm(const int n);

int main() {
    // Test base cases
    assert(fibonacciTerm(0) == 0);
    assert(fibonacciTerm(1) == 1);
    
    // Test small values
    assert(fibonacciTerm(2) == 1);
    assert(fibonacciTerm(3) == 2);
    assert(fibonacciTerm(4) == 3);
    assert(fibonacciTerm(5) == 5);
    assert(fibonacciTerm(6) == 8);
    
    // Test a slightly larger value
    assert(fibonacciTerm(10) == 55);
    
    // Test a value that fits in int (fib(20) = 6765)
    assert(fibonacciTerm(20) == 6765);
    
    // Test that the function is const-correct by using a const variable
    const int n = 7;
    assert(fibonacciTerm(n) == 13);
    
    return 0;
}
