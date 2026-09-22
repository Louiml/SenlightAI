/*
Write a C++ function named `computeFibonacci` that takes a non-negative integer `n` and returns the `n`-th Fibonacci number, where `F(0) = 0`, `F(1) = 1`, and `F(n) = F(n-1) + F(n-2)` for `n >= 2`. The function must handle `n` up to at least 30 without integer overflow, must be efficient (not exponential), and must use constant auxiliary memory (do not allocate a table proportional to `n`). Apply appropriate `const` correctness and include necessary headers. The function signature is `int computeFibonacci(int n)`, and it should be standalone with no `main` function.
*/
#include <cassert>

// Compute the n-th Fibonacci number (0-indexed) using constant auxiliary space.
// F(0) = 0, F(1) = 1, F(n) = F(n-1) + F(n-2) for n >= 2.
int computeFibonacci(const int n) {
    assert(n >= 0);
    if (n == 0) return 0;
    if (n == 1) return 1;

    int prev2 = 0;  // F(0)
    int prev1 = 1;  // F(1)

    for (int i = 2; i <= n; ++i) {
        const int current = prev1 + prev2;
        prev2 = prev1;
        prev1 = current;
    }
    return prev1;
}
int main() {
    // Base cases
    assert(computeFibonacci(0) == 0);
    assert(computeFibonacci(1) == 1);

    // Standard sequence values
    assert(computeFibonacci(2) == 1);
    assert(computeFibonacci(3) == 2);
    assert(computeFibonacci(4) == 3);
    assert(computeFibonacci(5) == 5);
    assert(computeFibonacci(6) == 8);

    // Larger values (within int range)
    assert(computeFibonacci(10) == 55);
    assert(computeFibonacci(20) == 6765);
    assert(computeFibonacci(30) == 832040);

    return 0;
}
// The core idea is dynamic programming with state reduction. Instead of storing the entire `dp` array, we only need the last two computed values. We maintain two variables: `prev2` representing `F(i-2)` and `prev1` representing `F(i-1)`. For each `i` from 2 to `n`, we compute `current = prev1 + prev2`, then shift: `prev2 = prev1`, `prev1 = current`. After the loop, `prev1` holds `F(n)`. Edge cases: if `n == 0`, return 0; if `n == 1`, return 1. Since we only loop from 2 to `n` once, the time complexity is `O(n)`, and auxiliary space is `O(1)`. This avoids recursion stack depth and extra memory. Overflow is not a concern for `n <= 30` because `F(30) = 832040`, well within `int` range.
