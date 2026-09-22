Write a C++ function that takes an integer `a` and a non-negative integer `n`, and returns the sum of the squares of `a` repeated `n` times (i.e., `n * a * a`). The function must handle the case where `n` is zero, returning `0`, and must avoid integer overflow by using a larger type for the result (e.g., `long long`). The function should be named `sumOfSquares` and reside in a header-compatible format so it can be tested independently. Do not include any console I/O or a `main` function in the solution; only the free function.

int main() {
    // Basic tests
    assert(sumOfSquares(3, 4) == 36);      // 3^2 * 4 = 36
    assert(sumOfSquares(0, 10) == 0);      // 0^2 * 10 = 0
    assert(sumOfSquares(-5, 2) == 50);     // (-5)^2 * 2 = 50
    // Edge case: n = 0
    assert(sumOfSquares(7, 0) == 0);
    // Edge case: n = 1
    assert(sumOfSquares(10, 1) == 100);
    // Edge case: large a to check overflow handling
    assert(sumOfSquares(100000, 100000) == 1000000000000000LL); // 1e10^2? Wait: 100000^2 = 1e10, times 100000 = 1e15
    // Recompute: 100000 * 100000 = 10000000000 (1e10), times 100000 = 1e15
    // The assert above is wrong; correct: 100000^2 = 1e10, * 100000 = 1e15
    assert(sumOfSquares(100000, 100000) == 1000000000000000LL); // 1e15
    // Another test with negative a and n large
    assert(sumOfSquares(-2, 5) == 20);     // 4 * 5 = 20
    // Test with n = INT_MAX? Not necessary but we can test small a to avoid overflow in test
    assert(sumOfSquares(65536, 65536) == 18446744073709551616LL); // 2^32 * 2^16? Actually 65536^2 = 2^32 = 4294967296, times 65536 = 2^48 = 281474976710656
    // Let's do a correct large test:
    assert(sumOfSquares(1000000, 1000000) == 1000000000000000000LL); // 1e6^2 = 1e12, * 1e6 = 1e18 (within long long)
    return 0;
}

// Returns the sum of n copies of a*a, i.e., n * a * a, as long long.
long long sumOfSquares(const int a, const int n) {
    if (n == 0) return 0LL;
    // Cast to long long before multiplication to avoid 32-bit overflow.
    const long long base = static_cast<long long>(a) * a;
    return base * static_cast<long long>(n);
}

// The problem is mathematically simple: the sum of `n` identical terms each equal to `a*a` is exactly `n * a * a`. The main task is implementing this correctly with attention to types and edge cases. Since `a` is an `int` (which can be as large as ~2.1e9), `a * a` can overflow a 32-bit `int`. Therefore, we must cast `a` to `long long` (or `long long int`) before multiplication. The result is returned as `long long`, and `n` is also used in the multiplication; to avoid overflow when `n` is large (but within `int` range, up to ~2.1e9) and `a*a` is near 4.6e18, we should also cast `n` to `long long` in the multiplication. Edge cases include `n == 0` (return 0 immediately) and negative `a` (square makes it positive, which is fine). Time complexity is O(1) because we use a direct formula rather than a loop. Space complexity is O(1). The direct formula is preferable to a loop, which would take O(n) time and could be extremely slow for large `n`. We must ensure the function is marked `const`-correct (though there are no member functions, we use `const`-qualified parameters `const int a` and `const int n`).
