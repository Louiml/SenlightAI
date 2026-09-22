/*
Write a C++ function that, given a positive integer `n` (with `n > 1`), returns the maximum `k` such that `n` can be expressed as `k`-th power of an integer base, i.e., `n = b^k` for some integer `b ≥ 2`. If no such representation exists with `k > 1`, the function should return `1`. The input `n` may be as large as `10^12`, and your solution must handle this efficiently without overflow. The function should be named `maxExponent` and take a single `long long` parameter.
*/
#include <cmath>
#include <cstdint>

// Returns the maximum exponent k such that n = b^k for some integer b >= 2.
// If no such k > 1 exists, returns 1. n must be positive and > 1.
long long maxExponent(long long n) {
    long long best = 1;  // At minimum, n = n^1
    long long limit = static_cast<long long>(std::sqrt(static_cast<long double>(n)));

    for (long long base = 2; base <= limit; ++base) {
        if (n % base == 0) {
            long long temp = n;
            long long counter = 0;
            // Count how many times base divides n
            while (temp % base == 0) {
                ++counter;
                temp /= base;
            }
            // If base^counter exactly equals n, then n is a perfect counter-th power
            // Avoid overflow: use pow and compare with n
            long double check = std::pow(static_cast<long double>(base), static_cast<long double>(counter));
            if (std::llabs(static_cast<long long>(check) - n) <= 0.5) {
                if (counter > best) {
                    best = counter;
                }
            }
        }
    }
    return best;
}
#include <cassert>
#include <iostream>

int main() {
    // Perfect squares: k=2
    assert(maxExponent(4) == 2);
    assert(maxExponent(9) == 2);
    assert(maxExponent(100) == 2);  // 10^2

    // Perfect cubes: k=3
    assert(maxExponent(8) == 3);
    assert(maxExponent(27) == 3);
    assert(maxExponent(125) == 3);

    // Higher powers: k=4,5,6
    assert(maxExponent(16) == 4);   // 2^4
    assert(maxExponent(32) == 5);   // 2^5
    assert(maxExponent(64) == 6);   // 2^6, also 4^3, 8^2, but max is 6
    assert(maxExponent(81) == 4);   // 3^4

    // Composite bases: e.g., 216 = 6^3, but also 2^3 * 3^3, so k=3
    assert(maxExponent(216) == 3);
    // 1024 = 2^10, k=10
    assert(maxExponent(1024) == 10);

    // Numbers that are not perfect powers: return 1
    assert(maxExponent(2) == 1);
    assert(maxExponent(6) == 1);
    assert(maxExponent(10) == 1);
    assert(maxExponent(14) == 1);

    // Large prime power: 2^20 = 1048576
    assert(maxExponent(1048576) == 20);

    // Large number that is a perfect square but also has higher power? 36 = 6^2, but also 2^2*3^2, so max is 2
    assert(maxExponent(36) == 2);

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
// The problem is to find the maximum exponent `k` for which `n` is a perfect `k`-th power. The key observation is that if `n = b^k`, then every prime factor of `n` must appear with an exponent that is a multiple of `k`. Actually, the maximum such `k` is the greatest common divisor (GCD) of the exponents in the prime factorization of `n`. For example, if `n = 2^6 * 3^9`, then exponents are 6 and 9, GCD is 3, so `n = (2^2 * 3^3)^3`, hence maximum `k = 3`. However, the given code snippet in the problem uses a different, brute-force approach: it iterates over all possible bases `i` from 2 to `sqrt(n)`, checks if `i` divides `n` completely, and then counts how many times `i` divides `n`. If `i^counter == n`, that means `n` is exactly a power of `i` with exponent `counter`. It then takes the maximum such `counter`. This method is correct because if `n = b^k` with `b ≥ 2`, then `b` must be a divisor of `n` and also `b^k = n`. The smallest such base is the prime factor with the smallest exponent, but the algorithm checks all divisors `i` up to `sqrt(n)`. For example, for `n = 64`, it checks `i=2`: counter=6, `2^6=64` → max=6. For `i=4`, counter=3, `4^3=64` → max remains 6. The algorithm returns the maximum counter found, but it starts with `ans = -1e15` and returns `max(ans, 1)` at the end. Edge case: when `n` is negative? The original snippet handles negative `n` by taking absolute value and toggling a flag, but for this task we only consider positive `n`. Also, note that if `n` is a perfect square, cube, etc., the algorithm will find it. The time complexity is `O(sqrt(n))` because we loop up to `sqrt(n)`, and for each divisor we might do a while loop that divides `n` repeatedly, but that amortizes to logarithmic in `n` per divisor. Overall, worst-case `O(sqrt(n) log n)` which is fine for `n ≤ 10^12` (sqrt ~ 1e6). Space complexity is `O(1)`.
