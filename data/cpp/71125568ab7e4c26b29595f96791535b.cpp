// Given a positive integer `n` and a positive integer `k`, write a C++ function `kthFactorValue` that returns the `k`th factor of `n` in increasing order (starting from 1). If `n` has fewer than `k` factors, the function should return `-1`. For example, for `n = 12`, the factors in increasing order are `1, 2, 3, 4, 6, 12`, so `kthFactorValue(12, 3)` returns `3`, and `kthFactorValue(12, 7)` returns `-1`. The function must handle edge cases where `k` is 1 (always returns 1) and where `n` is 1 (only factor is 1).
#include <cassert>

// Global main function for self-testing.
int main() {
    // Basic cases
    assert(kthFactorValue(12, 1) == 1);
    assert(kthFactorValue(12, 2) == 2);
    assert(kthFactorValue(12, 3) == 3);
    assert(kthFactorValue(12, 4) == 4);
    assert(kthFactorValue(12, 5) == 6);
    assert(kthFactorValue(12, 6) == 12);
    assert(kthFactorValue(12, 7) == -1);

    // Edge: n = 1
    assert(kthFactorValue(1, 1) == 1);
    assert(kthFactorValue(1, 2) == -1);

    // Edge: prime number
    assert(kthFactorValue(7, 1) == 1);
    assert(kthFactorValue(7, 2) == 7);
    assert(kthFactorValue(7, 3) == -1);

    // Larger n with many factors
    assert(kthFactorValue(100, 1) == 1);
    assert(kthFactorValue(100, 9) == 100);
    assert(kthFactorValue(100, 10) == -1);

    // k = 1 always returns 1 for any positive n
    assert(kthFactorValue(999, 1) == 1);
    assert(kthFactorValue(2, 1) == 1);

    return 0;
}
#include <cstddef>  // for size_t (optional, not strictly needed)

// Returns the k-th factor of n in increasing order, or -1 if n has fewer than k factors.
// Preconditions: n > 0, k > 0.
int kthFactorValue(int n, int k) {
    int remaining = k;
    for (int i = 1; i <= n; ++i) {
        if (n % i == 0) {
            --remaining;
            if (remaining == 0) {
                return i;
            }
        }
    }
    return -1;
}
// The simplest approach is to iterate from `1` to `n` and count each integer that divides `n` evenly. We maintain a counter starting at `k`; each time we find a divisor, we decrement the counter. When the counter reaches zero, we return the current integer because that is the `k`th divisor. If the loop ends without reaching zero, there are fewer than `k` factors, so we return `-1`. This works because divisors are naturally found in increasing order by this linear scan. Edge cases: `n = 1` has exactly one factor (1), so `k > 1` returns `-1`; `k = 1` always returns `1` for any positive `n`. Time complexity is `O(n)` because we iterate through all numbers from 1 to `n`. Space complexity is `O(1)` since we only use a few integer variables.
