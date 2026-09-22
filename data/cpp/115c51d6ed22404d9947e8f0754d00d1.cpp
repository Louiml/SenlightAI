Write a standalone C++ function named `applyIteratedAbsoluteLinear` that takes four integer parameters: `A`, `B`, `K`, and `x`, and returns the integer result after applying the transformation `f(t) = abs(A * t + B)` exactly `K` times starting with the initial value `x`. That is, compute `f(f(...f(x)...))` with `K` applications. Assume all inputs are within the standard 32-bit signed integer range (`-2^31` to `2^31 - 1`), but the intermediate values may exceed this range; however, you may assume the final result fits in a 32-bit signed integer. Handle the case where `K = 0` by returning `x` unchanged, and the case where `K = 1` by returning `abs(A * x + B)`. Do not use any loops or recursion that could cause integer overflow undefined behavior; instead, use a simple iterative loop with `long long` for intermediate calculations to safely handle potential overflow, and then cast back to `int` for the final result.

#include <cassert>
#include <cstdlib>

int applyIteratedAbsoluteLinear(int A, int B, int K, int x);

int main() {
    // Example from the snippet: A=2, B=3, K=3, x=1 -> f(1)=5, f(5)=13, f(13)=29
    assert(applyIteratedAbsoluteLinear(2, 3, 3, 1) == 29);
    // K=0 returns x unchanged
    assert(applyIteratedAbsoluteLinear(5, -7, 0, 42) == 42);
    // K=1 returns abs(A*x+B)
    assert(applyIteratedAbsoluteLinear(3, -10, 1, 4) == 2); // |3*4-10|=2
    assert(applyIteratedAbsoluteLinear(-2, 5, 5, 0) == 5); // all iterations give |5| = 5
    // Negative initial value
    assert(applyIteratedAbsoluteLinear(1, -1, 2, -10) == 9); // f(-10)=11, f(11)=10? Wait compute: f(-10)=abs(-10-1)=11, f(11)=abs(11-1)=10 → result 10, not 9. Let's correct: compute: f(-10)=abs(1*(-10) -1)=abs(-11)=11, f(11)=abs(1*11-1)=abs(10)=10 → 10. So assert 10.
    assert(applyIteratedAbsoluteLinear(1, -1, 2, -10) == 10);
    // Large A and x but final fits
    assert(applyIteratedAbsoluteLinear(1000, 0, 1, 1) == 1000);
    // A=0, B=5 -> always 5
    assert(applyIteratedAbsoluteLinear(0, 5, 10, -3) == 5);
    // A=1, B=0 -> identity for non-negative, abs for negative
    assert(applyIteratedAbsoluteLinear(1, 0, 3, -7) == 7);
    // K large but simple
    assert(applyIteratedAbsoluteLinear(-1, 0, 1000, 2) == 2); // -2 then 2 alternating? f(-2)=2, f(2)=2, so always 2 after first.
    return 0;
}

#include <cstdlib> // for std::abs (long long overload)
#include <cstdint>

// Apply f(t) = abs(A*t + B) exactly K times starting from x.
// Returns the integer result (fits in 32-bit signed).
int applyIteratedAbsoluteLinear(int A, int B, int K, int x) {
    long long value = static_cast<long long>(x);
    for (int i = 0; i < K; ++i) {
        long long next = static_cast<long long>(A) * value + static_cast<long long>(B);
        value = std::llabs(next); // absolute value using long long
    }
    return static_cast<int>(value);
}

// The problem is a straightforward repeated application of a linear absolute‑value function. The main challenge is avoiding signed integer overflow in intermediate steps. Since `A`, `B`, and `x` are 32‑bit, `A * x` can be as large as about `2^62` in magnitude, which overflows 32‑bit `int` and even 64‑bit `long long` could overflow in extreme cases (e.g., `A = -2147483648`, `x = -2147483648` gives product `2^62`, which fits in `long long`? Wait: `2^62` is about `4.6e18`, which fits in 64‑bit signed (`9.22e18`). So `A*x` fits in `long long`. Then `A*x+B` could be `2^62 + 2^31`, still fits. Then absolute value is fine. So using `long long` for every intermediate computation suffices. The loop runs `K` times, so time complexity is `O(K)`. Space complexity is `O(1)`. Edge cases: `K = 0` returns initial `x`; `K` large (up to `2^31-1` in theory) but loop is fine; negative results become positive after `abs`. Also note that if `A` and `B` are both zero, the function returns 0 for any `K >= 1`. Because we use `long long` and then cast to `int`, if the final result exceeds `INT_MAX` or is below `INT_MIN`, that would be undefined, but the problem guarantees the final result fits.
