/*
Write a C++ function that takes a positive integer `n` and returns the largest integer whose square does not exceed `n` (i.e., the integer part of the square root of `n`), but with a twist: the function must count how many integers from 0 upward satisfy this condition and return that count, not the root itself. Specifically, given `n`, return the number of non-negative integers `k` such that `k * k <= n`. For example, for `n = 26`, the integers 0,1,2,3,4,5 all satisfy (since 5²=25 ≤ 26, but 6²=36 > 26), so the count is 6. The original snippet incorrectly returns the root (5) but the intended task is to return the count (which is root + 1). Ensure the function handles `n = 0` (only k=0 qualifies, count=1) and `n` large enough to avoid overflow by using `long long` for intermediate multiplications.
*/

// Count the number of non-negative integers k such that k*k <= n.
// For n >= 0, returns floor(sqrt(n)) + 1.
unsigned int countSquaresWithin(long long n) {
    // n is expected to be non-negative; if not, handle gracefully.
    if (n < 0) return 0;

    unsigned int count = 0;
    // Use long long for i*i to avoid overflow.
    for (long long i = 0; i * i <= n; ++i) {
        ++count;
    }
    return count;
}

#include <cassert>

int main() {
    // Basic cases
    assert(countSquaresWithin(0) == 1);      // only 0^2 = 0
    assert(countSquaresWithin(1) == 2);      // 0^2 and 1^2
    assert(countSquaresWithin(2) == 2);      // 0^2 and 1^2 (2^2 > 2)
    assert(countSquaresWithin(3) == 2);
    assert(countSquaresWithin(4) == 3);      // 0,1,2 (2^2=4)
    assert(countSquaresWithin(25) == 6);     // 0..5 (5^2=25)
    assert(countSquaresWithin(26) == 6);     // 0..5 (6^2=36 > 26)
    // Large value near INT_MAX (2147483647) suitable for 32-bit int
    assert(countSquaresWithin(2147483647LL) == 46341); // floor(sqrt(2147483647))=46340, +1 = 46341
    // Large value beyond 32-bit range to test overflow safety
    assert(countSquaresWithin(1000000000000LL) == 1000001); // sqrt(1e12)=1,000,000 → +1
    return 0;
}

// The problem reduces to finding the largest integer `r` such that `r*r <= n`, then returning `r + 1` because all integers from 0 to `r` inclusive satisfy the condition. The simplest correct algorithm iterates `i` from 0 upward while `i*i <= n`, incrementing a counter each time. For efficiency, we can instead compute the integer square root by a linear scan (since `n` is a typical 32‑bit int, at most 46340 iterations for n up to 2^31-1) or use binary search. A linear scan is simpler and matches the original snippet’s structure. Edge cases: `n=0` → only i=0 qualifies, count=1. `n=1` → i=0 and i=1 qualify, count=2. Negative `n` should not occur per specification but we can assert `n >= 0`. Use `long long` for `i*i` to avoid overflow when `i` is near 46340 (46340² ≈ 2.147e9 fits in 32-bit signed but borderline). Time complexity is O(√n) for the linear scan, or O(log n) if binary search is used; auxiliary space O(1).
