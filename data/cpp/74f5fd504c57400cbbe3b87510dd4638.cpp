Given a positive integer `n` (with `n >= 2`), write a C++ function that finds and returns the largest integer `x` such that `2 <= x <= n` and `x` can be expressed as a perfect power of some integer base `b >= 2`, i.e., `x = b^k` for some integer exponent `k >= 2`. For example, for `n = 10`, numbers like 4 (=2²), 8 (=2³), and 9 (=3²) qualify, and the maximum is 9. For `n = 2`, there is no such number (since the smallest perfect power is 4), so the function should return `1`. The function must handle `n` up to `10^9` efficiently.

// The problem asks for the maximum perfect power (with exponent ≥ 2) not exceeding `n`. A naive approach would iterate over all numbers up to `n` and test if each is a perfect power, but that is too slow for large `n`. Instead, we can iterate over possible bases `b` from 2 up to `sqrt(n)` (since if `b > sqrt(n)`, then `b^2 > n` and no higher power can fit). For each base `b`, we compute successive powers `b^2, b^3, ...` while the power ≤ `n`, updating the maximum found. We start the result at 1, which serves as the fallback if no perfect power exists (which only happens when `n < 4`). Edge cases: `n` exactly equal to a perfect power (e.g., `n=16`, result is 16), `n` slightly below a perfect power (e.g., `n=15`, result is 9), and small `n<4` where result is 1. Since we only check bases up to `sqrt(n)`, the number of bases is at most 31623 for `n=10^9`, and for each base we do about `log(n)` multiplications, making the total time roughly `O(sqrt(n) log n)`. Space is `O(1)`.

#include <cmath>
#include <cstdint>

// Return the largest perfect power (exponent >= 2) not exceeding n.
// If no such number exists (i.e., n < 4), return 1.
long long largestPerfectPower(long long n) {
    long long result = 1;
    // Only bases up to sqrt(n) can have b^2 <= n.
    for (long long b = 2; b * b <= n; ++b) {
        long long power = b * b;  // b^2
        while (power <= n) {
            if (power > result) {
                result = power;
            }
            // Avoid overflow: if power > n / b, next multiplication would exceed n.
            if (power > n / b) break;
            power *= b;
        }
    }
    return result;
}

#include <cassert>
#include <cmath>

// Declaration from solution (usually included via header)
long long largestPerfectPower(long long n);

int main() {
    // Basic cases
    assert(largestPerfectPower(2) == 1);     // no perfect power <= 2
    assert(largestPerfectPower(3) == 1);     // no perfect power <= 3
    assert(largestPerfectPower(4) == 4);     // 2^2
    assert(largestPerfectPower(5) == 4);     // 4 <= 5
    assert(largestPerfectPower(9) == 9);     // 3^2
    assert(largestPerfectPower(10) == 9);    // max is 9

    // Exact boundaries
    assert(largestPerfectPower(16) == 16);   // 2^4 or 4^2
    assert(largestPerfectPower(15) == 9);    // 9 < 15 < 16
    assert(largestPerfectPower(27) == 27);   // 3^3
    assert(largestPerfectPower(26) == 25);   // 25 < 26

    // Larger values
    assert(largestPerfectPower(100) == 100); // 10^2
    assert(largestPerfectPower(99) == 81);   // 9^2 = 81
    assert(largestPerfectPower(1000) == 1000); // 10^3
    assert(largestPerfectPower(999) == 961); // 31^2 = 961

    // Very large n, ensure no overflow
    assert(largestPerfectPower(1000000000LL) == 1000000000LL); // 1000^3 or 31622^2? 31622^2=999950884, but 1000000000 = 10^9 = (10^3)^3, so yes
    // Test a value just below a large power: 10^9 - 1 = 999999999, expect largest perfect power <= that
    // 31622^2 = 999950884, 31623^2 is too big, so answer 999950884
    assert(largestPerfectPower(999999999LL) == 999950884LL);

    // Edge case n = 1? Not in spec (n>=2), but if given, should return 1
    // assert(largestPerfectPower(1) == 1); // optional

    return 0;
}
