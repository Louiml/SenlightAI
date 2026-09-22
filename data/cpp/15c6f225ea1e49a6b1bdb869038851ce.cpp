// Given integers `n`, `m`, and a sequence `a` of length `n`, write a C++ function `countValidMultiples` that returns the number of odd integers `k` such that `1 ≤ k ≤ m` and `k` is a multiple of the least common multiple of all values `(a_i / 2)`. If the chosen LCM is greater than `m`, the count is 0. Additionally, if the exponent of 2 in the prime factorization of any `a_i / 2` differs from that of `a_0 / 2`, return 0. The function must handle large inputs (`n`, `m`, `a_i` up to `10^9`) using a 64-bit integer type. The input sequence is guaranteed non-empty. The function should be self-contained, not rely on global state, and be usable in a standalone testing environment.

// The key observation is that a number `X` having the form `a_i / 2 * (2*p + 1)` (i.e., an odd multiple of `a_i/2`) has an odd quotient when divided by `a_i/2`. For `X` to be a multiple of all `a_i/2`, it must be a multiple of their LCM `L`. Since `X = L * t` for some integer `t`, `t` must be odd to satisfy the form for every `i`. Therefore, the count of valid odd multiples of `L` up to `m` is `(m / L + 1) / 2` when `L ≤ m`, else 0. However, the LCM could become huge; but if it exceeds `m` (or during calculation, if it would overflow 64-bit, we can cap at `m+1`). Additionally, the condition about the power of 2 is necessary: if the exponents of 2 in the prime factorizations of all `a_i/2` are not all equal, then no number `X` can be an odd multiple of every `a_i/2` because the smallest possible `X` would require different powers of 2. So we first divide all `a_i` by 2, compute the LCM with overflow protection, and check the 2-exponent consistency. Complexity: O(n log(max a)) time, O(1) extra space.

#include <cstdint>
#include <numeric>

using int64 = std::int64_t;

// Count odd integers k in [1, m] such that k is a multiple of lcm(a_i/2).
// Returns 0 if the powers of 2 in a_i/2 are inconsistent or if lcm > m.
int64 countValidMultiples(int n, int64 m, const int64* a) {
    // Compute LCM of (a[i]/2) with early overflow cap.
    int64 L = 1;
    for (int i = 0; i < n; ++i) {
        int64 val = a[i] / 2;  // guaranteed even input, but we force division
        // Check divisibility to avoid overflow: L * (val / gcd) > m?
        int64 g = std::gcd(L, val);
        int64 next = val / g;
        if (L > m / next) {
            return 0;  // LCM exceeds m
        }
        L *= next;
    }

    // Check that all a[i]/2 have the same power of 2.
    int exp0 = 0;
    {
        int64 temp = a[0] / 2;
        while (temp % 2 == 0) {
            temp /= 2;
            exp0++;
        }
    }
    for (int i = 1; i < n; ++i) {
        int64 temp = a[i] / 2;
        int exp = 0;
        while (temp % 2 == 0) {
            temp /= 2;
            exp++;
        }
        if (exp != exp0) {
            return 0;
        }
    }

    if (L > m) return 0;
    int64 q = m / L;
    return (q + 1) / 2;
}

#include <cassert>
#include <cstdint>

// Assume solution function is declared above.
int main() {
    // Test 1: Simple case, n=1, a=[4], m=10 -> a/2=2, L=2, odds up to 10 multiples of 2: 2,4,6,8,10 -> odd multiples: 2,6,10 -> 3
    int64 a1[] = {4};
    assert(countValidMultiples(1, 10, a1) == 3);

    // Test 2: n=2, a=[6,10], a/2={3,5}, L=15, m=40 -> multiples of 15 up to 40: 15,30 -> odd multiples: 15 only -> 1
    int64 a2[] = {6, 10};
    assert(countValidMultiples(2, 40, a2) == 1);

    // Test 3: Inconsistent powers of 2: a=[2,6], a/2={1,3} -> exp0=0, exp1=0 same; but if a=[4,12] a/2={2,6} -> exp0=1, exp1=1; inconsistent? a=[4,8] a/2={2,4}-> exp0=1, exp1=2 -> return 0
    int64 a3[] = {4, 8};
    assert(countValidMultiples(2, 100, a3) == 0);

    // Test 4: L > m
    int64 a4[] = {1000000000LL};
    assert(countValidMultiples(1, 1, a4) == 0);

    // Test 5: Large n, simple
    int64 a5[] = {2, 2, 2, 2};
    assert(countValidMultiples(4, 1000000000LL, a5) == 500000000LL);

    // Test 6: Case where L exactly equals m
    int64 a6[] = {6}; // a/2=3, L=3, m=3 -> odds: 3 only -> 1
    assert(countValidMultiples(1, 3, a6) == 1);

    // Test 7: Even multiples not counted
    int64 a7[] = {2}; // a/2=1, L=1, m=4 -> odd multiples of 1: 1,3 -> 2
    assert(countValidMultiples(1, 4, a7) == 2);

    // Test 8: Inconsistent powers with large numbers
    int64 a8[] = {8, 24}; // a/2={4,12} -> exp0=2, exp1=2, consistent? Actually 12=4*3, exp=2 yes. L=12, m=50 -> odd multiples: 12*1,12*3 = 12,36 -> 2
    assert(countValidMultiples(2, 50, a8) == 2);

    // Test 9: Ensure early overflow cap does not cause false results
    int64 a9[] = {1000000000LL, 1000000000LL};
    // a/2={500000000,500000000} L=500000000, m=500000000 -> q=1 -> (1+1)/2=1
    assert(countValidMultiples(2, 500000000LL, a9) == 1);

    // Test 10: n=0? Not allowed per spec, but check n=1 with smallest
    int64 a10[] = {2};
    assert(countValidMultiples(1, 1, a10) == 1);
}
