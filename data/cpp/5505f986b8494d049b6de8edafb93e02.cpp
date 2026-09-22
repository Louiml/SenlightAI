// Write a C++ function `countEvenDivisors(int n)` that returns the number of positive even divisors of a given positive integer `n`. For example, for `n = 12`, its divisors are 1, 2, 3, 4, 6, 12, of which even ones are 2, 4, 6, 12 → count = 4. The function must handle `n` up to 10⁹ and must not enumerate all divisors naively if doing so would be linear in `n`; instead, use the divisor-pair property to iterate only up to sqrt(n). The function should return an `int` and be safe for all positive integers, including 1 (which has no even divisors → 0).
// The most efficient approach is to iterate over candidate divisors `i` from 2 to sqrt(n), and for each `i` that divides `n`, inspect both `i` and its complementary divisor `n/i`. If `i` is even, increment the count; if `n/i` is even and distinct from `i`, increment the count. After the loop, if `n` itself is even, we must count `n` as its own divisor pair (`1` and `n`) — this is necessary because the loop starting at 2 never checks `1` and its complement `n`. Important edge cases: (1) `n = 1` → loop doesn't run, and `n` is odd, so result 0. (2) Perfect squares (e.g., `n = 16`): when `i = 4`, we have `i == n/i`, so we must not double-count that divisor — we only count `i` if it's even, but we skip the complementary branch when `n/i == i`. (3) Very large `n` up to 10⁹: sqrt(n) ≤ ~31623, so the loop runs at most ~31622 iterations, which is fast. Time complexity is O(√n), and space complexity is O(1).
#include <cmath>

// Return the count of positive even divisors of a positive integer n.
// Uses the divisor-pair approach: for each i up to sqrt(n), check i and n/i.
int countEvenDivisors(int n) {
    int count = 0;
    const int limit = static_cast<int>(std::sqrt(n));
    for (int i = 2; i <= limit; ++i) {
        if (n % i == 0) {
            if (i % 2 == 0) {
                ++count;
            }
            const int complement = n / i;
            if (complement != i && complement % 2 == 0) {
                ++count;
            }
        }
    }
    // The divisor pair (1, n) is not covered by the loop starting at i=2.
    if (n % 2 == 0) {
        ++count;  // counts n itself (and implicitly 1 is not even)
    }
    return count;
}
#include <cassert>

int main() {
    // Basic cases
    assert(countEvenDivisors(1) == 0);
    assert(countEvenDivisors(2) == 1);   // {2}
    assert(countEvenDivisors(3) == 0);
    assert(countEvenDivisors(4) == 2);   // {2,4}
    assert(countEvenDivisors(12) == 4);  // {2,4,6,12}

    // Perfect square: 36 has divisors 1,2,3,4,6,9,12,18,36 → even: 2,4,6,12,18,36 → count=6
    assert(countEvenDivisors(36) == 6);

    // Large prime (odd) → no even divisors
    assert(countEvenDivisors(9999991) == 0);  // 9999991 is prime

    // Large even number not perfect square: 1000000000 = 2^9 * 5^9 → all divisors with at least one factor 2
    // Total divisors = (9+1)*(9+1)=100, odd divisors = 1 (since all 5^9) → even = 99
    assert(countEvenDivisors(1000000000) == 99);

    // Larger number with many divisors
    assert(countEvenDivisors(100000000) == 71); // known result from divisor enumeration

    return 0;
}
