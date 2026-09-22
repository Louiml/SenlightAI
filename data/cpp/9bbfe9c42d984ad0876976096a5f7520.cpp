/*
Write a C++ function named `computeResult` that takes two non-negative integer inputs `n` and `m` (both within the range `[0, 10^9]`), and returns an integer according to the following logic: if `m == 1`, return `0`; otherwise, return `n + (m - 2)`. The function must be pure (no I/O), efficient for large inputs, and handle the edge case where both inputs are zero by treating it as a normal case (i.e., since `m != 1`, the result would be `0 + (0 - 2) = -2`, which is allowed). The function is intended to emulate the behavior of the loop in the given snippet for individual pairs rather than processing multiple test cases. No input validation is required beyond the constraints. The function should use appropriate `const` correctness and be a free function (not a method of a class).
*/
#include <cstdint>

// Returns the computed result for a single (n, m) pair.
// If m == 1, result is 0; otherwise, result is n + (m - 2).
// Both n and m are non-negative and may be up to 1e9, so use 64-bit integers.
int64_t computeResult(const int64_t n, const int64_t m) {
    if (m == 1) {
        return 0;
    }
    return n + (m - 2);
}
#include <cassert>
#include <cstdint>

// Declaration of the solution function (assumed defined elsewhere).
int64_t computeResult(const int64_t n, const int64_t m);

int main() {
    // Basic cases
    assert(computeResult(0, 0) == -2);   // m != 1, so 0 + (0 - 2)
    assert(computeResult(5, 1) == 0);    // m == 1 -> 0
    assert(computeResult(10, 2) == 10);  // 10 + (2 - 2) = 10
    assert(computeResult(3, 5) == 6);    // 3 + (5 - 2) = 6

    // Edge values near the upper bound
    assert(computeResult(1000000000LL, 1000000000LL) == 1999999998LL);
    assert(computeResult(1000000000LL, 1) == 0);

    // m = 1 with large n still gives 0
    assert(computeResult(999999999LL, 1) == 0);

    // Zero n with m > 1
    assert(computeResult(0, 3) == 1);

    // Small values
    assert(computeResult(1, 2) == 1);
    assert(computeResult(1, 1) == 0);

    return 0;
}
// The problem is a direct translation of the arithmetic in the provided code snippet. The snippet processes pairs `(n, m)` until both are zero, and for each pair prints `0` if `m == 1`, otherwise it prints `n + (m - 2)`. The task simplifies this to a single function that computes the result for one pair. The only edge case is `m == 1`, where the result is always `0`, regardless of `n`. For all other `m`, the result is a simple linear expression. Note that because `n` and `m` can be as large as `10^9`, the result can be as large as about `2 * 10^9`, which fits safely in a 64-bit integer but not in a 32-bit `int`. Therefore, we should use a 64-bit type (e.g., `long long` or `int64_t`) for both inputs and the return type. The function does not need any auxiliary data structures. Time complexity is `O(1)` and space complexity is `O(1)`. The implementation is trivial, but for teaching purposes, we should emphasize correct use of integer types, const-qualified parameters (since they are not modified), and clarity.
