/*
Write a C++ function `findPair(int l, int r)` that takes two integers `l` and `r` (with `1 ≤ l ≤ r ≤ 10^9`) and returns a `pair<long long, long long>` of two distinct integers `x` and `y` such that `l ≤ x < y ≤ r` and `x` divides `y`. If no such pair exists, return `{-1, -1}`. The function must handle the case where `l == 1` specially, and must correctly produce a valid pair for all valid inputs (it is guaranteed that at least one pair always exists for the given constraints). The function should be efficient and avoid overflow.
*/
#include <utility> // for std::pair

// Given l and r (1 <= l <= r <= 1e9), find a pair (x, y) with l <= x < y <= r
// such that x divides y. Returns {-1, -1} if no such pair exists (though per
// problem constraints, a pair always exists).
std::pair<long long, long long> findPair(long long l, long long r) {
    if (l == 1) {
        // 1 divides 2, and since a pair exists, r >= 2.
        return {1LL, 2LL};
    }
    // For l > 1, the minimal possible pair is (l, 2*l).
    // If 2*l <= r, this is valid.
    if (2LL * l <= r) {
        return {l, 2LL * l};
    }
    // Otherwise, no multiple of l fits. But because l > r/2, no x in [l, r]
    // can have a multiple in [l, r] either (since 2*x > r for all x >= l).
    // So no pair exists (this case is guaranteed not to occur under constraints).
    return {-1LL, -1LL};
}
#include <cassert>
#include <utility>

int main() {
    // Basic tests
    assert(findPair(1, 5) == std::make_pair(1LL, 2LL));
    assert(findPair(2, 4) == std::make_pair(2LL, 4LL));
    assert(findPair(3, 6) == std::make_pair(3LL, 6LL));
    assert(findPair(4, 8) == std::make_pair(4LL, 8LL));
    // l=1 with large r
    assert(findPair(1, 1000000000) == std::make_pair(1LL, 2LL));
    // Large l and r where 2*l just fits
    assert(findPair(500000000, 1000000000) == std::make_pair(500000000LL, 1000000000LL));
    // l just above r/2 (should be impossible by constraints, but check function handles it)
    // For completeness, if it were possible, it would return {-1,-1}; here we test that it does.
    assert(findPair(6, 10) == std::make_pair(-1LL, -1LL)); // 6*2=12 > 10
    // Another valid pair with l>1
    assert(findPair(7, 21) == std::make_pair(7LL, 14LL));
    // Edge where l=2 and r=2? Not valid since no x<y, but guaranteed not to happen; just check no crash
    // Not tested because it violates constraints.
    return 0;
}
// The key observation is that we need two distinct numbers in `[l, r]` where the smaller divides the larger. The simplest approach is to try to use the smallest possible numbers. If `l` is 1, then `1` divides every integer, so we can pick `(1, 2)` if `r >= 2` (which is guaranteed since `l ≤ r` and `l=1` implies `r ≥ 1`, but if `r == 1` there is no pair; however constraints say a pair always exists, so for `l=1` we must have `r ≥ 2`). For `l > 1`, the smallest candidate is `(l, 2*l)`. This pair is valid if `2*l ≤ r`. If `2*l > r`, then we need to search for another pair. However, note that if `2*l > r`, then the interval length is less than `l`, so the only possible pairs are when a small number divides a larger one within this short interval. In fact, the smallest possible multiple of any `x` is `2*x`, which is already greater than `r` if `x > r/2`. So for `x > r/2`, no multiple exists. Therefore we only need to check `x` from `l` up to `r/2`. For each such `x`, check if `2*x ≤ r`; if yes, return `(x, 2*x)`. Since `l > 1` and if `2*l > r`, then `l > r/2`, so `l` is already > r/2, meaning there is no `x` in `[l, r/2]`, so no pair exists. But the problem guarantees a pair exists, so we will always find one. The main algorithm: if `l == 1`, return `{1, 2}` (since `2 ≤ r` because a pair exists). Otherwise, return `{l, 2*l}` if `2*l ≤ r`; else return `{-1, -1}` (but this case won't happen under problem constraints). Time complexity: O(1). Space complexity: O(1). Edge cases: `l == 1` and `r` possibly 1? But problem guarantees at least one pair, so `r >= 2` when `l==1`. Also handle overflow: use `long long` for `2*l` to avoid int overflow.
