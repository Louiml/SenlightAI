// Write a C++ function that, given three integers `n`, `a`, and `b` (where `n > 0` and `a, b >= 0`), returns a sorted vector of all distinct possible last-stone values obtainable in a game where a player starts at position 0, takes exactly `n-1` steps, and at each step moves either `a` or `b` units forward. The possible final positions are exactly `i*a + (n-1-i)*b` for `i = 0, 1, ..., n-1`, where `i` is the number of `a`‑steps. The returned vector must be sorted in ascending order and contain no duplicate values.

#include <cassert>
#include <vector>

int main() {
    // n=1: no steps, only position 0
    assert(distinctStoneValues(1, 3, 5) == std::vector<int>({0}));

    // n=2: two possible positions: a and b (different)
    assert(distinctStoneValues(2, 3, 5) == std::vector<int>({3, 5}));

    // a == b: only one distinct value repeated n times
    assert(distinctStoneValues(5, 2, 2) == std::vector<int>({8}));

    // n=3, a=1, b=2: positions 0+2+2=4, 1+2=3, 2+2=4? Wait correctly: i=0->4, i=1->1+2=3, i=2->2+0=2 => {2,3,4}
    assert(distinctStoneValues(3, 1, 2) == std::vector<int>({2, 3, 4}));

    // n=4, a=7, b=7: all identical (all 7*3=21)
    assert(distinctStoneValues(4, 7, 7) == std::vector<int>({21}));

    // n=6, a=0, b=3: only multiples of 3, but distinct steps? i=0 -> 15, i=5 -> 0, others 12,9,6,3 => sorted {0,3,6,9,12,15}
    assert(distinctStoneValues(6, 0, 3) == std::vector<int>({0, 3, 6, 9, 12, 15}));

    // Larger values, a and b swapped
    assert(distinctStoneValues(4, 10, 1) == std::vector<int>({3, 12, 21, 30}));

    // Ensure no duplicates when a != b (n > 1)
    auto v = distinctStoneValues(10, 2, 5);
    assert(v.size() == 10);  // all distinct

    // Check one specific value manually
    // n=10, a=2, b=5 -> i=0 => 9*5=45, i=9 => 9*2=18
    assert(v.front() == 18 && v.back() == 45);

    return 0;
}

#include <vector>
#include <set>

// Return a sorted vector of all distinct possible final stone values.
// n > 0, a >= 0, b >= 0
std::vector<int> distinctStoneValues(int n, int a, int b) {
    std::set<int> values;  // automatically deduplicates and sorts

    for (int i = 0; i < n; ++i) {
        // i steps of size a, (n-1-i) steps of size b
        values.insert(i * a + (n - 1 - i) * b);
    }

    return std::vector<int>(values.begin(), values.end());
}

// The key observation is that each possible final position is determined solely by the count `i` of how many times the player chose the `a`‑step (the remaining `n-1-i` steps being `b`‑steps). Since `i` ranges from 0 to `n-1`, all `n` combinations are distinct in terms of the order of steps, but the resulting final positions may coincide when `a == b` (then all values are identical) or when the formula produces duplicate sums for different `i` values (this happens only when `a == b`, because the expression is linear in `i` with coefficient `(a - b)`, so if `a != b` the values are strictly monotonic and thus unique). To handle duplicates elegantly, we insert each computed value into a `std::set`, which automatically removes duplicates and keeps the values sorted. Finally, we convert the set to a vector. Edge cases: `n = 1` (no steps, result is just `[0]`), large `n` (values can be large, so `int` is sufficient if constraints fit; could use `long long` for safety), and `a == b` (all values equal). Time complexity is `O(n log n)` due to `n` insertions into a set, with `O(n)` auxiliary space for the set; the final vector conversion is linear. Space complexity is `O(n)` overall.
