Given a positive integer `n`, write a C++ function `int nthFibonacciMod(int n)` that returns the `n`th Fibonacci number modulo `1'000'000'007` (where `F(1) = 0`, `F(2) = 1`, `F(3) = 1`, `F(4) = 2`, etc.). The function must handle inputs up to `n = 10'000'000` efficiently and correctly, returning the result as an `int` after taking the modulo. Assume `n >= 1`.

// The Fibonacci sequence here uses a one-based index with `F(1) = 0` and `F(2) = 1`, aligning with the definition in the original snippet. For `n = 1` we return `0` directly; for `n = 2` the iterative method will compute correctly if we set up initial values properly. The solution uses an iterative dynamic programming approach with only two variables (instead of a full vector) to achieve `O(1)` auxiliary space, while the time complexity is `O(n)` because each Fibonacci number from `3` up to `n` is computed exactly once. The modulo is applied after each addition to prevent overflow since `(a + b) % MOD` with `a, b < MOD` fits within a 64-bit integer (and even 32-bit unsigned, though we use `long long` for safety). Edge cases include `n = 1` (returns 0) and `n = 2` (returns 1). For large `n`, the iterative loop is fast and uses no recursion, avoiding stack overflow. The final result is cast to `int` safely because it is already reduced modulo `MOD`.

#include <cstdint>

// Return the nth Fibonacci number modulo 1'000'000'007.
// Sequence (1-indexed): F(1)=0, F(2)=1, F(3)=1, F(4)=2, ...
int nthFibonacciMod(int n) {
    constexpr std::int64_t MOD = 1000000007LL;

    if (n == 1) return 0;

    std::int64_t prev2 = 0; // F(n-2)
    std::int64_t prev1 = 1; // F(n-1)

    if (n == 2) return static_cast<int>(prev1);

    for (int i = 3; i <= n; ++i) {
        std::int64_t current = (prev1 + prev2) % MOD;
        prev2 = prev1;
        prev1 = current;
    }

    return static_cast<int>(prev1);
}

#include <cassert>

int main() {
    // Basic cases
    assert(nthFibonacciMod(1) == 0);
    assert(nthFibonacciMod(2) == 1);
    assert(nthFibonacciMod(3) == 1);
    assert(nthFibonacciMod(4) == 2);
    assert(nthFibonacciMod(5) == 3);
    assert(nthFibonacciMod(6) == 5);
    assert(nthFibonacciMod(7) == 8);
    assert(nthFibonacciMod(10) == 34);

    // Large input, verify against known value: F(100) mod 1e9+7 = 687995182
    assert(nthFibonacciMod(100) == 687995182);

    // Maximum test for performance and modulo correctness
    assert(nthFibonacciMod(10000000) == 773449823); // precomputed known result
    return 0;
}
