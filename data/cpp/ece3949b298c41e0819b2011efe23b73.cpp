Write a C++ function named `countPairs` that takes two non-negative integers `A` and `B` as parameters and returns the sum of the count of unordered pairs of distinct numbers selected from the set `{1, 2, ..., A}` and the count of unordered pairs of distinct numbers selected from the set `{1, 2, ..., B}`. In other words, compute \(\binom{A}{2} + \binom{B}{2}\) where \(\binom{n}{2} = n(n-1)/2\). The function should handle `A` and `B` up to \(10^9\), so it must use a 64-bit integer type. Return the result as a `long long`.
// The problem is a straightforward combinatorial calculation. The number of unordered pairs of distinct elements from a set of size \(n\) is given by the binomial coefficient \(\binom{n}{2} = \frac{n(n-1)}{2}\). Since the sets for `A` and `B` are independent, the total count is simply \(\binom{A}{2} + \binom{B}{2}\). However, naive multiplication `A*(A-1)/2` can overflow a 32-bit integer because `A` can be up to \(10^9\), and `A*(A-1)` can be up to about \(10^{18}\), which fits in a 64-bit signed integer (`long long`). The division by 2 should be done after multiplication to avoid truncation, since `n(n-1)` is always even. Edge cases: when `A` is 0 or 1, the term is 0 because there are no pairs; the formula naturally handles this. Also, when `A` or `B` is negative (though the task specifies non-negative inputs), the formula would give a positive result, but we can ignore that given the constraint. Time complexity: \(O(1)\) arithmetic operations. Space complexity: \(O(1)\).
#include <cstdint>

// Compute the sum of unordered pairs from two independent sets of sizes a and b.
// Equivalent to C(a,2) + C(b,2) = a*(a-1)/2 + b*(b-1)/2.
long long countPairs(long long a, long long b) {
    const long long pairsA = a * (a - 1) / 2;
    const long long pairsB = b * (b - 1) / 2;
    return pairsA + pairsB;
}
#include <cassert>

int main() {
    // Basic cases
    assert(countPairs(2, 2) == 2); // 1+1
    assert(countPairs(1, 1) == 0); // 0+0
    assert(countPairs(0, 5) == 10); // 0+10
    assert(countPairs(5, 0) == 10); // 10+0

    // Larger values to verify no overflow (10^9)
    assert(countPairs(1000000000LL, 1000000000LL) == 999999999000000000LL);
    assert(countPairs(1000000000LL, 1LL) == 499999999500000000LL);

    // Mixed values
    assert(countPairs(3, 4) == 9); // 3+6
    assert(countPairs(4, 3) == 9); // 6+3

    // Edge: both zero
    assert(countPairs(0, 0) == 0);

    // Edge: one is 1, the other large
    assert(countPairs(1, 1000000LL) == 499999500000LL);
}
