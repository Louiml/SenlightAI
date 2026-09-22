// Write a C++ function `largestPerfectPowerAtMost` that accepts a single positive integer `x` (where `1 ≤ x ≤ 1000`) and returns the largest integer `n` such that `n ≤ x` and `n` can be expressed as a perfect power: `n = a^b` for some integer `a ≥ 2` and integer `b ≥ 2`. Note that `1` should always be considered a valid perfect power (as `1^b` for any `b ≥ 2`). The function must handle all inputs in the given range, including edge cases where `x` itself is a perfect power, and where the largest perfect power is much smaller than `x` (e.g., for `x = 2`, the answer is `1`). The function should not rely on floating-point arithmetic; use integer operations only. The time and space complexity should be efficient for the given constraint.

#include <cassert>

// Declaration of the function under test (provided in the solution section).
int largestPerfectPowerAtMost(int);

int main() {
    // Edge cases
    assert(largestPerfectPowerAtMost(1) == 1);
    assert(largestPerfectPowerAtMost(2) == 1);
    assert(largestPerfectPowerAtMost(3) == 1);

    // Exact perfect powers
    assert(largestPerfectPowerAtMost(4) == 4);
    assert(largestPerfectPowerAtMost(8) == 8);
    assert(largestPerfectPowerAtMost(9) == 9);
    assert(largestPerfectPowerAtMost(16) == 16);
    assert(largestPerfectPowerAtMost(27) == 27);
    assert(largestPerfectPowerAtMost(32) == 32);
    assert(largestPerfectPowerAtMost(36) == 36);
    assert(largestPerfectPowerAtMost(100) == 100);
    assert(largestPerfectPowerAtMost(125) == 125);
    assert(largestPerfectPowerAtMost(216) == 216);
    assert(largestPerfectPowerAtMost(256) == 256);
    assert(largestPerfectPowerAtMost(512) == 512);
    assert(largestPerfectPowerAtMost(729) == 729);
    assert(largestPerfectPowerAtMost(961) == 961);
    assert(largestPerfectPowerAtMost(1000) == 1000);

    // Numbers between perfect powers
    assert(largestPerfectPowerAtMost(5) == 4);
    assert(largestPerfectPowerAtMost(10) == 9);
    assert(largestPerfectPowerAtMost(17) == 16);
    assert(largestPerfectPowerAtMost(28) == 27);
    assert(largestPerfectPowerAtMost(33) == 32);
    assert(largestPerfectPowerAtMost(37) == 36);
    assert(largestPerfectPowerAtMost(101) == 100);
    assert(largestPerfectPowerAtMost(130) == 128);
    assert(largestPerfectPowerAtMost(217) == 216);
    assert(largestPerfectPowerAtMost(257) == 256);
    assert(largestPerfectPowerAtMost(600) == 576);

    // Near the upper bound
    assert(largestPerfectPowerAtMost(999) == 961);

    return 0;
}

#include <set>
#include <vector>
#include <algorithm>

// Return the largest perfect power (a^b with a>=2, b>=2, or 1) that is <= x.
// x is guaranteed to be between 1 and 1000 inclusive.
int largestPerfectPowerAtMost(int x) {
    // Precompute all perfect powers <= 1000.
    static const std::set<int> perfectPowers = [] {
        std::set<int> result;
        result.insert(1);  // 1 is treated as perfect power.
        const int limit = 1000;
        for (int base = 2; base * base <= limit; ++base) {
            int value = base;
            while (value <= limit / base) {  // avoid overflow
                value *= base;
                result.insert(value);
            }
        }
        return result;
    }();

    // Find the first perfect power greater than x, then step back.
    auto it = perfectPowers.upper_bound(x);
    --it;  // Since x >= 1 and 1 is always in the set, it will be valid.
    return *it;
}

// The key observation is that perfect powers up to 1000 can be enumerated by iterating over bases `a` from 2 up to 31 (since `31^2 = 961`, and `32^2 = 1024 > 1000`) and for each base, multiplying repeatedly until the value exceeds 1000, inserting each result into a `std::set<int>`. Additionally, `1` must be inserted manually because `a ≥ 2` will never produce `1`. Once the set of all perfect powers is built (it contains only a small number of elements—about 38 or so, as seen in the given snippet), we can answer any query `x` by using `std::set::upper_bound(x)` to find the first element greater than `x`, then decrementing the iterator to get the largest perfect power ≤ `x`. Alternatively, we can precompute a sorted vector and use binary search or the `std::lower_bound` function. Edge cases: `x = 1` returns `1`; `x = 2` returns `1`; `x = 1000`, note that `1000` is not a perfect power (since `10^3 = 1000`, but `10` is base, `3` is exponent, actually wait `1000 = 10^3`, so yes it is a perfect power! Indeed `10^3 = 1000` with base 10, exponent 3. So it should be included). The enumeration perfectly captures all such values because we try all bases and multiply up to 1000. Complexity: enumerating all perfect powers ≤ 1000 takes about `O(sqrt(1000) * log(1000))` time, which is effectively constant. Query time is `O(log k)` where `k` is the size of the set (roughly 40), so effectively `O(1)` per query. Space is `O(k)`.
