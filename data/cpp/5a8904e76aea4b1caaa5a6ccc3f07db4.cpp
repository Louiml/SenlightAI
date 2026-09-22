// Write a C++ function that computes the nth Fibonacci number using memoization (top-down dynamic programming) to avoid redundant recursive calls. The function must accept an unsigned 64-bit integer `n` and return the nth Fibonacci number, where the sequence is defined as F(1) = 1, F(2) = 1, and F(n) = F(n-1) + F(n-2) for n > 2. The function must handle `n` values up to 90 without overflow (as the 90th Fibonacci number is 2880067194370816120, which fits in `uint64_t`). Use a static memoization table to cache results, and ensure the function is reusable across multiple calls (i.e., results persist between calls). The solution must be self-contained, include necessary headers, and be free of any global mutable state other than the memoization table.

// The solution uses top-down dynamic programming with a static memoization array of size 500, initialized to zero. The base cases are `n <= 2`, returning 1. If the memoized value for `n` is non-zero, it is returned directly. Otherwise, the function recursively computes `fibo(n-1) + fibo(n-2)`, stores the result in the memo array, and returns it. Edge cases: `n = 0` is not part of the sequence (the function assumes `n >= 1`); `n = 1` and `n = 2` return 1 directly. The memoization table is static, so it persists across function calls and is populated incrementally—this ensures that once a value is computed, subsequent calls for the same or smaller `n` are O(1). Time complexity: the first call for a given `n` requires computing all Fibonacci numbers up to `n` exactly once, giving O(n) time. Subsequent calls for the same or any smaller `n` are O(1). Space complexity: O(n) for the memo array, but the array is fixed at 500 entries, so effectively O(1) for practical usage within the limit. For `n` beyond 90, `uint64_t` may overflow; the problem restricts `n` to 90 to avoid this. The static array is safe to use across multiple calls because it is thread-unsafe but acceptable for a simple sequential program.

#include <cstdint>

// Compute the nth Fibonacci number (1-indexed) using memoization.
// F(1) = 1, F(2) = 1, F(n) = F(n-1) + F(n-2) for n > 2.
// Supports n up to 90 without overflow (uint64_t).
uint64_t fiboMemo(uint64_t n) {
    static uint64_t memo[91] = {0}; // 91 entries enough for n up to 90

    if (n <= 2) {
        return 1;
    }
    if (memo[n] != 0) {
        return memo[n];
    }
    memo[n] = fiboMemo(n - 1) + fiboMemo(n - 2);
    return memo[n];
}

#include <cassert>
#include <cstdint>
#include <iostream>

// Include the solution function here (or link against it)

int main() {
    // Base cases
    assert(fiboMemo(1) == 1);
    assert(fiboMemo(2) == 1);

    // Known Fibonacci numbers
    assert(fiboMemo(3) == 2);
    assert(fiboMemo(4) == 3);
    assert(fiboMemo(5) == 5);
    assert(fiboMemo(10) == 55);
    assert(fiboMemo(20) == 6765);
    assert(fiboMemo(30) == 832040);
    assert(fiboMemo(50) == 12586269025ULL);
    assert(fiboMemo(90) == 2880067194370816120ULL);

    // Test that memoization caches correctly: calling again returns same result quickly
    assert(fiboMemo(30) == 832040);
    assert(fiboMemo(50) == 12586269025ULL);

    std::cout << "All tests passed!\n";
    return 0;
}
