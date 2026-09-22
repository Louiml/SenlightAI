// Write a C++ function `long long countPowerfulDivisors(long long n)` that takes a positive integer `n` and returns the number of distinct integers `x` (where `x` is greater than 1) such that `x` divides some power of a prime factor of `n` (i.e., there exists a prime `p` dividing `n` and an exponent `k ≥ 1` such that `x = p^k` and `p^k ≤ n`). Equivalently, count all distinct prime powers (including the prime itself with exponent 1, and higher powers like `p^2`, `p^3`, …) that appear in the prime factorization of `n`, where each such prime power is considered exactly once even if it divides `n` multiple times. For example, if `n = 12 = 2^2 * 3^1`, the prime powers are 2, 4, and 3, so the answer is 3. If `n = 100 = 2^2 * 5^2`, prime powers are 2, 4, 5, 25 → answer 4. If `n` is a prime power like `n = 8 = 2^3`, then prime powers are 2, 4, 8 → answer 3. If `n` is prime, answer is 1. The function must handle `n` up to 10^12 efficiently.
// The key insight is that for each prime factor `p` of `n`, we must count all distinct powers `p^1, p^2, …, p^e` where `e` is the exponent of `p` in the factorization of `n`. The naive loop over all divisors of `n` would be too slow for `n` up to 10^12, but we only need to iterate over primes up to `sqrt(n)`. For each prime `d` that divides `n`, we compute the full exponent `e` by repeatedly dividing `n` by `d`. Then we add `e` to the answer because all `p^1, p^2, …, p^e` are distinct. However, if `n` itself is a prime greater than `sqrt(n)` (i.e., after removing all factors up to `sqrt(n)`, the remaining value is greater than 1), that remaining value is a distinct prime and must be counted as well (with exponent 1). To avoid double-counting, we must be careful: if `n` is a perfect square of a prime, we will have already counted the prime when iterating up to its square root; but if after dividing all factors up to `sqrt(original_n)` the remaining `n` is a prime, that prime is > `sqrt(original_n)` and was not counted before, so we add 1 for it. Edge cases: `n = 1` returns 0 (no prime powers), `n` prime returns 1, `n` a power of a single prime returns the exponent. Time complexity: we iterate `d` from 2 to `sqrt(n)`, and for each divisor we do a while loop that divides `n` by `d`; the total number of divisions is the sum of exponents, which is at most `log2(n)`. So worst-case time is `O(sqrt(n))` with a small constant factor, but in practice for `n` up to 10^12 that is about 10^6 iterations, which is acceptable. Space complexity: `O(1)` auxiliary.
#include <cstdint>
#include <cmath>

// Count distinct prime powers (p^1, p^2, ..., p^e) that divide n.
long long countPowerfulDivisors(long long n) {
    if (n <= 1) return 0;

    long long answer = 0;
    long long remaining = n;

    // Check all possible prime factors up to sqrt(original n).
    for (long long p = 2; p * p <= remaining; ++p) {
        if (remaining % p == 0) {
            // Count all powers of p that divide the original n.
            while (remaining % p == 0) {
                remaining /= p;
                ++answer;
            }
        }
    }

    // If any part > 1 remains, it is a prime factor that was not counted.
    if (remaining > 1) {
        ++answer;
    }

    return answer;
}
#include <cassert>

int main() {
    // Example from problem statement: 12 = 2^2 * 3^1 -> 3 prime powers
    assert(countPowerfulDivisors(12) == 3);

    // 100 = 2^2 * 5^2 -> 4 prime powers
    assert(countPowerfulDivisors(100) == 4);

    // 8 = 2^3 -> 3 prime powers
    assert(countPowerfulDivisors(8) == 3);

    // Prime number 7 -> 1
    assert(countPowerfulDivisors(7) == 1);

    // n = 1 has no prime powers
    assert(countPowerfulDivisors(1) == 0);

    // n = 2^10 = 1024 -> 10 prime powers
    assert(countPowerfulDivisors(1024) == 10);

    // n = 2^1 * 3^1 * 5^1 = 30 -> 3
    assert(countPowerfulDivisors(30) == 3);

    // n = 2^2 * 3^1 * 5^0 = 36 -> 3 (2,4,3)
    assert(countPowerfulDivisors(36) == 3);

    // Largest prime under 10^12: 999999999989 -> 1
    assert(countPowerfulDivisors(999999999989LL) == 1);

    // n = 2^2 * 9999999967 (prime) -> 2 + 1 = 3
    long long composite = 4LL * 9999999967LL;
    assert(countPowerfulDivisors(composite) == 3);
}
