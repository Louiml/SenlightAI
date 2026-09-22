// Write a C++ function `nextRoundWithZeros(unsigned int X, unsigned int K)` that takes a non-negative integer `X` and a non-negative integer `K` and returns the smallest integer strictly greater than `X` whose last `K` decimal digits are all zeros. For example, for `X = 1234` and `K = 2`, the result is `1300`. If `K = 0`, the function should return `X + 1` (since the last 0 digits means no trailing zero requirement). The function must handle any value of `K` that fits in `unsigned int`, and if `X` itself is already a multiple of `10^K`, the result should be exactly `X + 10^K`. Ensure no overflow occurs for the given ranges (i.e., work with `unsigned long long` internally if needed).
#include <cassert>

int main() {
    assert(nextRoundWithZeros(0, 0) == 1);
    assert(nextRoundWithZeros(7, 0) == 8);
    assert(nextRoundWithZeros(1234, 1) == 1240);
    assert(nextRoundWithZeros(1240, 1) == 1250);
    assert(nextRoundWithZeros(1234, 2) == 1300);
    assert(nextRoundWithZeros(1300, 2) == 1400);
    assert(nextRoundWithZeros(999, 3) == 1000);
    assert(nextRoundWithZeros(1000, 3) == 2000);
    assert(nextRoundWithZeros(9, 5) == 100000);
    assert(nextRoundWithZeros(99999, 5) == 100000);
    return 0;
}
#include <cstdint>

// Returns the smallest unsigned integer greater than X whose last K decimal digits are zero.
unsigned int nextRoundWithZeros(unsigned int X, unsigned int K) {
    unsigned long long unit = 1;
    for (unsigned int i = 0; i < K; ++i) {
        unit *= 10ULL;
    }
    unsigned long long value = static_cast<unsigned long long>(X);
    unsigned long long result = (value / unit + 1ULL) * unit;
    return static_cast<unsigned int>(result);
}
// The task requires computing the next multiple of `10^K` that is strictly greater than `X`. First, compute `unit = 10^K` using a loop or `pow`, but since `K` is an unsigned integer, a simple loop multiplying by 10 is safe and avoids floating-point issues. Then, the next multiple is `floor(X / unit) * unit + unit`. Because division truncates, `X / unit` gives the integer quotient. Adding `1` to that quotient and multiplying by `unit` yields the smallest multiple of `unit` greater than `X`. Edge cases: (1) If `K = 0`, `unit = 1`, and the formula gives `(X / 1 + 1) * 1 = X + 1`, which matches the requirement. (2) If `X` is already a multiple of `unit`, e.g., `X = 1200`, `unit = 100`, then `X / 100 = 12`, `12 + 1 = 13`, result `1300` (strictly greater). (3) Potential overflow: if `X` is near the maximum of `unsigned int`, but the result could exceed the maximum representable value. To be safe, use `unsigned long long` internally for the computation and cast the result back to `unsigned int` only if it fits; the task assumes inputs are such that output fits within `unsigned int`. Time complexity is `O(K)` due to the loop to compute `10^K`, and space complexity is `O(1)`.
