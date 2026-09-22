Write a C++ function named `sumOfSquares` that takes a single non-negative integer `n` (where `0 ≤ n ≤ 100`) and returns the sum of squares from `1²` to `n²`. That is, it should compute `1² + 2² + ... + n²`. The function must use a recursive approach with memoization (dynamic programming) to compute the result efficiently for multiple calls, and it should be able to handle the base case `n = 0` returning `0`. The function must be `const`-correct (i.e., it should not modify any global state aside from the memoization table that is initialized once). The solution must be self-contained, meaning no external libraries beyond the standard C++ headers are needed, and it should be safe for repeated calls with different `n` values.

The problem reduces to computing the sum of squares formula, but we are required to use recursion with memoization. The natural recursive definition is: `f(n) = n² + f(n-1)` with base case `f(0) = 0`. This directly matches the provided snippet. To avoid repeated recomputation when the function is called multiple times (e.g., in a loop), we use a static memoization array (or a local static array inside the function) initialized to a sentinel value (e.g., `-1`). On each call, if `dp[n] != -1`, we return the cached value. Otherwise, we compute recursively and store the result. Edge cases: `n = 0` must return `0` immediately; `n` is guaranteed to be in `[0, 100]`, but we can add defensive checks. The time complexity for each distinct `n` is `O(n)` due to recursion, but with memoization, each value from `0` to `100` is computed at most once, so total across all calls is `O(100)`. Space complexity is `O(100)` for the memoization table. Since the function is `const`-correct, we can mark the memoization array as `static` inside the function and initialize it once. To avoid overwriting on each call, we can use a static initialization flag or initialize with a sentinel value.

#include <vector>
#include <cstring>

// Computes sum of squares from 1^2 to n^2 using recursion and memoization.
// n must be in [0, 100].
int sumOfSquares(int n) {
    // Static memoization table, initialized once.
    static int dp[101];
    static bool initialized = false;
    if (!initialized) {
        std::memset(dp, -1, sizeof(dp));
        dp[0] = 0;
        initialized = true;
    }
    // If already computed, return cached value.
    if (dp[n] != -1) return dp[n];
    // Recursive compute and store.
    return dp[n] = n * n + sumOfSquares(n - 1);
}

#include <cassert>

int main() {
    // Basic cases
    assert(sumOfSquares(0) == 0);
    assert(sumOfSquares(1) == 1);
    assert(sumOfSquares(2) == 5);   // 1 + 4
    assert(sumOfSquares(3) == 14);  // 1+4+9
    assert(sumOfSquares(4) == 30);  // 1+4+9+16

    // Edge case upper bound
    assert(sumOfSquares(10) == 385); // known formula n(n+1)(2n+1)/6 = 10*11*21/6 = 385
    assert(sumOfSquares(100) == 338350); // 100*101*201/6

    // Repeated calls to ensure memoization doesn't break
    assert(sumOfSquares(5) == 55);
    assert(sumOfSquares(3) == 14);
    assert(sumOfSquares(100) == 338350);
    assert(sumOfSquares(0) == 0);

    // Verify against known formula for random values within range
    for (int n = 0; n <= 100; ++n) {
        int expected = n * (n + 1) * (2 * n + 1) / 6;
        assert(sumOfSquares(n) == expected);
    }

    return 0;
}
