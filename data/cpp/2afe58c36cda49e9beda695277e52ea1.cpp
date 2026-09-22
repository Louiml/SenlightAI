// Write a C++ function that takes a positive integer `n` and returns the `n`-th Fibonacci number, where the sequence is defined as `F(1) = 1`, `F(2) = 1`, and `F(k) = F(k-1) + F(k-2)` for `k > 2`. The function must be efficient enough to handle `n` up to 90 without stack overflow or significant slowdown, and must use iterative computation (not recursion) to avoid exponential time. The function should be named `fibonacci` and accept a single `int` argument, returning a `long long` to accommodate large values. Assume input is always valid (`n >= 1`), but the function should still be robust to any positive integer input.

The main algorithm is iterative: start with two variables representing `F(1)` and `F(2)`, both initialized to 1. For `n = 1` or `n = 2`, return 1 immediately. For `n > 2`, loop from 3 to `n`, updating the two variables: `next = prev1 + prev2`, then shift `prev1 = prev2` and `prev2 = next`. After the loop, `prev2` holds `F(n)`. Edge cases: `n = 1` returns 1, `n = 2` returns 1, and the loop handles `n >= 3` correctly. The iterative approach uses `O(1)` auxiliary space and `O(n)` time, which is linear and safe for `n` up to 90 (the maximum Fibonacci value fits in `long long`, since `F(90)` is about `2.8e18`). For `n` larger than 90, `long long` would overflow, but we are not required to handle that. The solution avoids recursion, so no stack overflow risk.

#include <cstddef>

// Computes the n-th Fibonacci number iteratively.
// F(1) = 1, F(2) = 1, F(k) = F(k-1) + F(k-2) for k > 2.
// Returns a long long to accommodate large results.
long long fibonacci(const int n) {
    if (n <= 2) {
        return 1LL;
    }

    long long prev1 = 1LL; // F(1)
    long long prev2 = 1LL; // F(2)

    for (int i = 3; i <= n; ++i) {
        long long next = prev1 + prev2;
        prev1 = prev2;
        prev2 = next;
    }

    return prev2;
}

#include <cassert>

int main() {
    assert(fibonacci(1) == 1);
    assert(fibonacci(2) == 1);
    assert(fibonacci(3) == 2);
    assert(fibonacci(4) == 3);
    assert(fibonacci(5) == 5);
    assert(fibonacci(10) == 55);
    assert(fibonacci(20) == 6765);
    assert(fibonacci(50) == 12586269025LL);
    assert(fibonacci(90) == 2880067194370816120LL);
    return 0;
}
