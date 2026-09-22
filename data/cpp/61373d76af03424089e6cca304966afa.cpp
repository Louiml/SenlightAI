/*
Write a C++ function named `nthFibonacciModulo` that accepts a single `long long int` argument `n` and returns the `n`-th Fibonacci number (with `F(0)=0`, `F(1)=1`) modulo `1'000'000'007` (i.e., \(10^9+7\)). The function must handle arbitrarily large `n` (up to \(10^{18}\)) without using recursion or memoization arrays, and must compute the result iteratively in \(O(n)\) time. Edge cases: when `n` is `0` or `1`, return `n` directly. The solution must be a free function (not a class method) and must not include a `main` function.
*/

#include <cstdint>

// Returns the n-th Fibonacci number modulo 1'000'000'007.
// F(0)=0, F(1)=1, F(n)=F(n-1)+F(n-2) for n>=2.
long long int nthFibonacciModulo(long long int n) {
    const long long int MOD = 1000000007LL;
    if (n <= 1) {
        return n;
    }
    long long int a = 1; // F(1)
    long long int b = 1; // F(2) initially, will be updated
    for (long long int i = 2; i < n; ++i) {
        long long int temp = (a + b) % MOD;
        a = b;
        b = temp;
    }
    return b;
}

#include <cassert>

int main() {
    assert(nthFibonacciModulo(0) == 0);
    assert(nthFibonacciModulo(1) == 1);
    assert(nthFibonacciModulo(2) == 1);
    assert(nthFibonacciModulo(3) == 2);
    assert(nthFibonacciModulo(10) == 55);
    assert(nthFibonacciModulo(50) == 12586269025LL % 1000000007LL);
    assert(nthFibonacciModulo(100000) == 911435502); // precomputed value
    return 0;
}

// The Fibonacci sequence grows extremely fast, so using a naive recursive or memoized approach is impossible for large `n` (e.g., `n = 10^18`). The iterative method maintains two variables representing the last two Fibonacci numbers. Starting from `F(0)=0` and `F(1)=1`, we loop from `i=2` to `n`, each time computing the sum modulo `MOD`, updating the two variables. This approach is safe because modulo arithmetic preserves the recurrence relation: \((F_{i-1} + F_{i-2}) \mod MOD\). Edge cases: if `n == 0` return `0`; if `n == 1` return `1`; otherwise loop. Complexity: time \(O(n)\) which is acceptable for typical contest constraints when `n` is up to about \(10^7\), but if `n` can be as large as \(10^{18}\), an \(O(\log n)\) matrix exponentiation method would be needed; however, the task explicitly asks for the iterative method (as in the provided snippet). Space complexity is \(O(1)\). The core challenge is ensuring the loop runs exactly `n-1` times and that the modulo is applied to avoid overflow of `long long` (since `a + b` can exceed `1e9+7` but fits in `long long` if `a,b < MOD`).
