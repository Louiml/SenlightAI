/*
Write a standalone C++ function `long long minKForHalfCandies(long long n)` that, given a positive integer `n` representing the total number of candies, computes and returns the smallest positive integer `k` such that if a person eats `k` candies per day (but if after eating, the remaining candies exceed 10, they also discard `1/10` of the remaining candies in that same day), the total candies eaten by the person over all days is at least half of `n` (i.e., `eaten >= ceil(n/2)`). The problem mimics a scenario where you choose your daily intake `k` to maximize the fraction you personally consume before the rest is lost. The function must handle values of `n` up to `10^18` efficiently. Do not include a `main` function in your solution; provide only the free function with necessary headers. For the test section, provide `assert` checks for various `n` values including small numbers, powers of two, large values, and edge cases like `n=1` or `n=2`.
*/

#include <cstdint>

// Simulate how many candies are eaten if daily intake is k, starting with n total.
static long long candyEaten(long long k, long long n) {
    long long eaten = 0;
    while (n > 0) {
        if (n <= k) {
            eaten += n;
            n = 0;
        } else {
            n -= k;
            eaten += k;
            if (n > 10) {
                n -= n / 10;
            }
        }
    }
    return eaten;
}

// Return the smallest k such that total eaten >= ceil(n/2).
long long minKForHalfCandies(long long n) {
    long long lo = 1, hi = n;
    long long ans = n;
    while (lo <= hi) {
        long long mid = lo + (hi - lo) / 2;
        long long eaten = candyEaten(mid, n);
        if (2 * eaten >= n) {
            ans = mid;
            hi = mid - 1;
        } else {
            lo = mid + 1;
        }
    }
    return ans;
}

#include <cassert>

// Already declared above; include the solution code here or link.

int main() {
    assert(minKForHalfCandies(1) == 1);
    assert(minKForHalfCandies(2) == 1);
    assert(minKForHalfCandies(3) == 1);
    assert(minKForHalfCandies(10) == 1);
    assert(minKForHalfCandies(11) == 1);
    assert(minKForHalfCandies(100) == 1);
    assert(minKForHalfCandies(101) == 2);
    assert(minKForHalfCandies(1000) == 2);
    assert(minKForHalfCandies(1000000000000000000LL) == 100000000000000000LL);
    assert(minKForHalfCandies(999999999999999999LL) == 100000000000000000LL);
    return 0;
}

// The problem is solved by first noting that for any fixed `k`, we can simulate the eating process in `O(log n)` time because each day the remaining candies are reduced by at least `k` (or by `n/10` if that is larger), and after the first day the remainder shrinks by a constant fraction. The simulation function `candy(k, n)` computes the total eaten. The key observation is that the total eaten is monotonically non-decreasing with respect to `k` (a larger `k` never results in fewer candies eaten). Therefore, we can use binary search over the range `[1, n]` to find the smallest `k` such that `2 * candy(k, n) >= n` (since `candy` is integer, checking `2 * eaten >= n` is equivalent to `eaten >= ceil(n/2)`). The binary search is correct because the predicate is monotonic: if `k` works, any larger `k` also works. The simulation itself: while remaining `n > 0`, if `n <= k`, eat all and stop; otherwise eat `k` and subtract it, then if the new remaining `n > 10`, also discard `n/10` (integer division). This must be done carefully with `long long` to avoid overflow. Time complexity is `O(log n * log n)` for binary search (each simulation takes `O(log n)` steps because the remainder shrinks exponentially once it exceeds `k`). Space complexity is `O(1)`.
