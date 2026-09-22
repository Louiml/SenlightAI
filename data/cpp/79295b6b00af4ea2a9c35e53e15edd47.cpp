// Write a standalone C++ function named `sumFirstN` that takes a single non-negative integer `n` and returns the sum of all integers from `0` to `n-1` (inclusive). The function should handle large values of `n` (up to \(10^9\)) without overflow and must not use any loop or recursion—use the closed-form arithmetic formula. The function should be pure (no side effects), marked `const`-correct, and must be reusable in any program. Provide a reference implementation and a test harness that verifies correctness for edge cases such as `n = 0`, `n = 1`, `n = 2`, and a very large value like `n = 1'000'000'000`.
#include <cassert>

// Forward declaration (or include the solution header if separate)
long long sumFirstN(long long n);

int main() {
    assert(sumFirstN(0) == 0);
    assert(sumFirstN(1) == 0);
    assert(sumFirstN(2) == 1);   // 0 + 1
    assert(sumFirstN(3) == 3);   // 0 + 1 + 2
    assert(sumFirstN(10) == 45); // 0+1+...+9
    assert(sumFirstN(437) == 95266); // from original snippet
    assert(sumFirstN(1000) == 499500);
    assert(sumFirstN(1000000000LL) == 499999999500000000LL);
    assert(sumFirstN(1LL << 31) == 2305843005992468480LL); // 2^31
    return 0;
}
#include <cstdint>

// Returns the sum of integers from 0 to n-1 (inclusive).
// Precondition: n >= 0.
// Uses the closed-form formula to avoid loops and overflow.
long long sumFirstN(const long long n) {
    return (n * (n - 1)) / 2;
}
// The problem is to compute the sum of the first `n` non-negative integers, which is \(0 + 1 + 2 + \dots + (n-1)\). The closed-form formula is \(\frac{n \cdot (n-1)}{2}\). This works for all non-negative `n`. Edge cases: when `n = 0`, the sum is 0 (the formula gives 0 since \(0 \cdot -1 / 2 = 0\)); when `n = 1`, the sum is 0; these are handled naturally. For large `n`, the multiplication `n * (n-1)` can overflow a 32-bit `int`, so we use `long long` (64-bit) for both parameter and return type. The maximum `n = 10^9` gives \(n \cdot (n-1) \approx 10^{18}\), which fits within a 64-bit signed integer (max ~9.22 × \(10^{18}\)). No special handling is needed for negative `n` because we assume the input is non-negative; if negative `n` were passed, the formula would give a positive result but the semantics are undefined—we can optionally add an `assert` or just document the precondition. Time complexity is \(O(1)\) and space complexity is \(O(1)\).
