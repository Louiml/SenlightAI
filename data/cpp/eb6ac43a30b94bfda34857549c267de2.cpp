// Write a C++ function `long long fibonacciValue(int n)` that takes a non-negative integer `n` and returns the `n`-th Fibonacci number, where `F(0) = 0` and `F(1) = 1`. The function must use an iterative approach with a fixed-size array of length `max(n, 2)` to store intermediate values (do not use recursion or memoization maps). The function must handle `n = 0` and `n = 1` correctly, and for `n >= 2`, compute the value by summing the previous two entries. The function should use `long long` to avoid overflow for larger inputs (up to `n = 90`). The function must be `const`-correct (parameters passed by value are fine) and should not print anything.
#include <cassert>
#include <vector>

// Declaration of the function to test.
long long fibonacciValue(int n);

int main() {
    // Base cases.
    assert(fibonacciValue(0) == 0);
    assert(fibonacciValue(1) == 1);
    
    // Small known values.
    assert(fibonacciValue(2) == 1);
    assert(fibonacciValue(3) == 2);
    assert(fibonacciValue(4) == 3);
    assert(fibonacciValue(5) == 5);
    assert(fibonacciValue(6) == 8);
    
    // Larger values.
    assert(fibonacciValue(10) == 55);
    assert(fibonacciValue(20) == 6765);
    assert(fibonacciValue(30) == 832040);
    assert(fibonacciValue(50) == 12586269025LL);
    assert(fibonacciValue(90) == 2880067194370816120LL);
    
    return 0;
}
#include <vector>

// Return the n-th Fibonacci number using an iterative array approach.
long long fibonacciValue(int n) {
    // Handle base cases directly to avoid out-of-bounds for small n.
    if (n < 0) return -1; // Invalid input, though not expected.
    if (n == 0) return 0;
    if (n == 1) return 1;

    // Use a vector sized n+1 to store values up to index n.
    std::vector<long long> fib(n + 1);
    fib[0] = 0;
    fib[1] = 1;

    for (int i = 2; i <= n; ++i) {
        fib[i] = fib[i - 1] + fib[i - 2];
    }

    return fib[n];
}
// The Fibonacci sequence is defined recursively as `F(0)=0`, `F(1)=1`, and `F(n)=F(n-1)+F(n-2)` for `n≥2`. A straightforward iterative solution uses a dynamic array (or vector) of size `n+1` and fills it in increasing order, which guarantees each value is computed exactly once. Since `n` is required to be non-negative, we must handle the base cases `n=0` and `n=1` specially — the array must have at least size 2 so we can safely initialize indices 0 and 1. For `n≥2`, the loop runs from 2 to `n`, each iteration performing a single addition. Time complexity is `O(n)` because we do exactly `n-1` additions. Space complexity is `O(n)` due to the array; we could reduce it to `O(1)` using two variables, but the task explicitly requires an array. Edge cases include `n=0` (return 0) and `n=1` (return 1), and large `n` values requiring `long long` to stay within range (the 90th Fibonacci number is ~2.88e18, which fits in signed 64-bit).
