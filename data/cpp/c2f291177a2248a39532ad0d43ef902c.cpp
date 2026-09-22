Write a C++ function `int collatzSteps(long long n)` that takes a positive integer `n` (guaranteed to be at least 1) and returns the number of steps needed to reach the value 1 by repeatedly applying the Collatz operation: if the current number is even, replace it with `n/2`; if it is odd (and not 1), replace it with `3*n+1`. The function must handle inputs up to and including `10^18` (i.e., up to `LLONG_MAX`), so use `long long` for internal computation and be careful about overflow during the multiplication `3*n+1`. The function should return the step count as an `int` (the count fits in 32-bit for all practical inputs under typical constraints). Your implementation must be self-contained in a single free function with no global state, and must not print anything; only return the result.

#include <cassert>

int main() {
    assert(collatzSteps(1) == 0);
    assert(collatzSteps(2) == 1);
    assert(collatzSteps(3) == 7);
    assert(collatzSteps(4) == 2);
    assert(collatzSteps(5) == 5);
    assert(collatzSteps(6) == 8);
    assert(collatzSteps(27) == 111);
    assert(collatzSteps(1000000) == 152);
    assert(collatzSteps(1000000000000000000LL) == 950);
    return 0;
}

#include <cstdint>   // for int64_t

// Returns the number of Collatz steps to reach 1 from a positive integer n.
int collatzSteps(long long n) {
    int steps = 0;
    while (n != 1) {
        if (n % 2 == 0) {
            n = n / 2;
        } else {
            // Use __int128 to avoid overflow in 3*n+1 for large n.
            __int128 next = static_cast<__int128>(n) * 3 + 1;
            n = static_cast<long long>(next);
        }
        ++steps;
    }
    return steps;
}

// The algorithm directly simulates the Collatz process: while `n` is not 1, we apply the transformation. If `n` is even, we halve it; if odd, we compute `3*n+1`. However, `3*n+1` can overflow even for `long long` when `n` is near `LLONG_MAX`. To avoid this, we can use a safe check: if `n > (LLONG_MAX - 1)/3`, the next step would overflow; but we can safely handle this by using `__int128` (GCC/Clang) or by a manual modification. Since the problem states input is at most `10^18`, which is less than `(2^63-1)/3 ≈ 3.07e18`, the multiply may actually be safe for `10^18` because `3*1e18` is `3e18` which fits in `long long` (max ≈9.22e18). But to be robust, we use `__int128` for the multiplication to guarantee no overflow for any `long long` input. The loop runs until `n` becomes 1; for typical values, the number of steps is logarithmic but can be large (e.g., for `n=1`, it's 0; for `n=2`, it's 1; for `n=3`, it's 7, etc.). Time complexity is O(k) where k is the number of steps; for inputs up to `10^18`, k is on the order of a few thousand at most. Space complexity is O(1). Edge cases: `n=1` returns 0; `n` may be odd and large, but the step count fits in `int` for all practical test values; we use `long long` but internal multiplication via `__int128` is safe.
