/*
Given a positive integer `n` and a list of `k` distinct positive integers, write a C++ function `countNumbersNotDivisible` that returns the count of integers in the range `[1, n]` that are **not** divisible by any of the given numbers. The function should take `n` (as `long long`), `k` (as `int`), and a vector of `long long` values, and return a `long long`. Handle the constraint that `n` can be as large as \(10^{18}\) and `k` can be as large as 20. If `k` is 0, return `n` (since no numbers to exclude). The implementation must avoid integer overflow when computing the least common multiple (LCM) of subsets; if a partial LCM would exceed `n`, stop that subset immediately. The order of the input numbers does not matter, and all given numbers are guaranteed to be pairwise distinct.
*/
#include <vector>
#include <numeric>

// Returns the count of numbers in [1, n] not divisible by any of the given divisors.
// Uses inclusion-exclusion with LCM for each subset.
long long countNumbersNotDivisible(long long n, const std::vector<long long>& divisors) {
    int k = static_cast<int>(divisors.size());
    if (k == 0) {
        return n;
    }

    long long divisibleCount = 0;
    int totalSubsets = 1 << k; // 2^k

    for (int mask = 1; mask < totalSubsets; ++mask) {
        long long lcm = 1;
        int sign = -1; // will flip per included element

        for (int bit = 0; bit < k; ++bit) {
            if (mask & (1 << bit)) {
                sign *= -1;
                long long d = divisors[bit];
                // Check if lcm * d / gcd would overflow: if lcm > n / d, then lcm * d > n
                if (lcm > n / d) {
                    lcm = n + 1; // mark invalid
                    break;
                }
                long long g = std::gcd(lcm, d);
                lcm = lcm / g * d; // safe because lcm * d <= n * d but we already ensured lcm <= n/d
            }
        }

        if (lcm <= n) {
            divisibleCount += sign * (n / lcm);
        }
    }

    return n - divisibleCount;
}
#include <cassert>
#include <vector>

// Test helper function declared above is used.

int main() {
    // Basic case: no divisors, all numbers count
    assert(countNumbersNotDivisible(10, {}) == 10);

    // Single divisor
    assert(countNumbersNotDivisible(10, {3}) == 7); // 1,2,4,5,7,8,10

    // Two divisors
    assert(countNumbersNotDivisible(10, {2,3}) == 3); // 1,5,7

    // Divisor 1 excludes everything
    assert(countNumbersNotDivisible(100, {1}) == 0);

    // Large n, multiple divisors
    assert(countNumbersNotDivisible(1000000000000000000LL, {2,3,5,7}) == 1000000000000000000LL - (500000000000000000LL + 333333333333333333LL + 200000000000000000LL + 142857142857142857LL - 166666666666666666LL - 100000000000000000LL - 71428571428571428LL - 66666666666666666LL - 47619047619047619LL - 28571428571428571LL + 33333333333333333LL + 23809523809523809LL + 14285714285714285LL + 9523809523809523LL - 4761904761904761LL));

    // Case where LCM would overflow but we correctly cap
    // n = 100, divisors = {2, 50} -> LCM = 100, fine; {2, 51} not in range? but check with n=10
    assert(countNumbersNotDivisible(10, {6, 10}) == 6); // 1,2,3,4,7,8,9

    // Pairwise distinct but not necessarily coprime
    assert(countNumbersNotDivisible(20, {4,6}) == 13); // count of numbers not divisible by 4 or 6

    // All small, no intersections
    assert(countNumbersNotDivisible(5, {2,3}) == 2); // 1,5

    // Single large divisor
    assert(countNumbersNotDivisible(100000, {99991}) == 100000 - 1); // only 99991 itself

    // Multiple with 1 combined
    assert(countNumbersNotDivisible(50, {1,2}) == 0);

    // Random check with brute force for small n
    {
        long long n = 30;
        std::vector<long long> divs = {4, 6, 9};
        long long expected = 0;
        for (long long i = 1; i <= n; ++i) {
            bool bad = false;
            for (long long d : divs) if (i % d == 0) { bad = true; break; }
            if (!bad) ++expected;
        }
        assert(countNumbersNotDivisible(n, divs) == expected);
    }

    return 0;
}
// The problem asks for numbers in `[1, n]` not divisible by any element of a given set. The complement — numbers divisible by at least one element — can be counted using the inclusion–exclusion principle. For each non-empty subset of the `k` numbers, we compute the LCM of the subset. The count of numbers divisible by that LCM is `n / LCM`. The inclusion–exclusion formula adds counts for subsets of odd size and subtracts counts for subsets of even size. Since `k <= 20`, we can iterate over all `2^k` subsets (excluding the empty subset) using a bitmask. For each subset, we compute the LCM iteratively: starting with `lcm = 1`, for each bit set, update `lcm = lcm * a[bit] / gcd(lcm, a[bit])`. To avoid overflow when `lcm` exceeds `n`, we detect at each multiplication step using `lcm > n / a[bit]` (or compare before multiplication) and break early, setting `lcm = n + 1` to invalidate the subset. The sign flips once per included element, so we start with `sign = -1` and multiply by -1 for each bit, making odd subsets positive and even subsets negative. Edge cases: if `k = 0`, there are no exclusions, so the answer is `n`. If the set contains `1`, then every number is divisible by 1, so the count is 0 — the formula naturally handles this because the LCM of any subset containing 1 is 1, and the inclusion–exclusion yields 0. Time complexity is `O(2^k * k)` for the subset enumeration and LCM computation, with `k <= 20` giving at most about 20 million operations, which is acceptable. Space complexity is `O(k)` for the input array (or `O(1)` extra besides input). The answer may be up to `n` which fits in `long long`.
