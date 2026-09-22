// Write a C++ function `int computePowerConsumption(int n, int p1, int p2, int p3, int t1, int t2, const std::vector<int>& l, const std::vector<int>& r)` that models the energy consumption of a device that alternates between active and idle periods. The device is active during each interval `[l[i], r[i]]` (in minutes, inclusive of both endpoints), and for each active minute it consumes `p1` watts. Between two consecutive active intervals (from `r[i]` to `l[i+1]`), the device is idle. For the first `t1` idle minutes, it still consumes `p1` watts (so-called "standby" mode). After `t1` minutes, for the next `t2` idle minutes, it consumes `p2` watts. After that, for any remaining idle minutes, it consumes `p3` watts. You are given `n` as the number of active intervals, and the arrays `l` and `r` of length `n` (with `l[i] < r[i]` and `l[i+1] > r[i]` for all valid `i`). The function should return the total watt-minutes consumed over the entire timeline from the start of the first active interval to the end of the last active interval. All inputs are non-negative integers, and `t1` and `t2` can be zero. Ensure your function is robust for edge cases such as `n = 1` (no idle gaps) and very large idle gaps.
#include <cassert>
#include <vector>

int computePowerConsumption(int, int, int, int, int, int,
                            const std::vector<int>&, const std::vector<int>&);

int main() {
    // Single active interval, no idle.
    std::vector<int> l1 = {0}, r1 = {5};
    assert(computePowerConsumption(1, 2, 3, 4, 1, 1, l1, r1) == 10);

    // Two intervals with a small gap under t1.
    std::vector<int> l2 = {0, 7}, r2 = {5, 10};
    // Active: 5*2 + 3*2 = 16; gap = 2 (< t1=3) -> idle at p1: 2*2=4; total=20
    assert(computePowerConsumption(2, 2, 3, 4, 3, 2, l2, r2) == 20);

    // Gap crosses t1 but not t2.
    std::vector<int> l3 = {0, 8}, r3 = {5, 10};
    // Active: 5*2 + 2*2 = 14; gap=3, t1=1 -> 1*2=2, remaining gap=2, t2=2 -> 2*3=6, total=14+8=22
    assert(computePowerConsumption(2, 2, 3, 4, 1, 2, l3, r3) == 22);

    // Gap crosses both thresholds.
    std::vector<int> l4 = {0, 10}, r4 = {5, 12};
    // Active: 5*2 + 2*2 = 14; gap=5, t1=1 ->2, remaining 4, t2=2 -> 2*3=6, remaining 2 -> 2*4=8, total=14+2+6+8=30
    assert(computePowerConsumption(2, 2, 3, 4, 1, 2, l4, r4) == 30);

    // Zero t1 and t2: idle all at p3.
    std::vector<int> l5 = {0, 5}, r5 = {2, 7};
    // Active: 2*1 + 2*1 = 4; gap=3, t1=0 ->0, t2=0 ->0, remaining 3*5=15, total=19
    assert(computePowerConsumption(2, 1, 2, 5, 0, 0, l5, r5) == 19);

    // t1 zero, t2 large enough to cover gap.
    std::vector<int> l6 = {0, 4}, r6 = {1, 6};
    // Active: 1*2 + 2*2 = 6; gap=3, t1=0, t2=5 -> 3*3=9, total=15
    assert(computePowerConsumption(2, 2, 3, 4, 0, 5, l6, r6) == 15);

    // Large gap using p3.
    std::vector<int> l7 = {0, 100}, r7 = {10, 110};
    // Active: 10*2 + 10*2 = 40; gap=90, t1=2 ->4, remaining 88, t2=3 -> 3*5=15, remaining 85*8=680, total=40+4+15+680=739
    assert(computePowerConsumption(2, 2, 5, 8, 2, 3, l7, r7) == 739);

    // n=0 returns 0 (robustness).
    std::vector<int> l8, r8;
    assert(computePowerConsumption(0, 1, 2, 3, 1, 1, l8, r8) == 0);

    // All active intervals, zero-length idle (though constraints say gap positive, test robustness).
    std::vector<int> l9 = {0, 5}, r9 = {5, 10};
    // Active: 5*1 + 5*1 = 10; gap=0 -> no idle, total=10
    assert(computePowerConsumption(2, 1, 2, 3, 2, 2, l9, r9) == 10);

    // Very tight gap exactly equal to t1.
    std::vector<int> l10 = {0, 5}, r10 = {2, 7};
    // Active: 2*3 + 2*3 = 12; gap=3, t1=3 -> 3*3=9, total=21
    assert(computePowerConsumption(2, 3, 4, 5, 3, 0, l10, r10) == 21);

    return 0;
}
#include <vector>
#include <algorithm>

// Computes total watt-minutes consumed over all active and idle periods.
// n: number of active intervals.
// p1, p2, p3: consumption rates for active, idle tier1, idle tier2 (watts per minute).
// t1, t2: duration thresholds (minutes) for idle tiers.
// l, r: vectors of start and end times of active intervals (l[i] < r[i], strictly increasing and non-overlapping).
int computePowerConsumption(int n, int p1, int p2, int p3, int t1, int t2,
                            const std::vector<int>& l, const std::vector<int>& r) {
    if (n <= 0) return 0;

    int total = 0;

    // Active consumption for each interval.
    for (int i = 0; i < n; ++i) {
        total += p1 * (r[i] - l[i]);
    }

    // Idle consumption between consecutive intervals.
    for (int i = 0; i < n - 1; ++i) {
        int gap = l[i + 1] - r[i];

        int first_tier = std::min(gap, t1);
        total += p1 * first_tier;
        gap -= first_tier;

        int second_tier = std::min(gap, t2);
        total += p2 * second_tier;
        gap -= second_tier;

        total += p3 * gap;
    }

    return total;
}
// The total consumption is the sum of two parts: active consumption and idle consumption. For each active interval `[l[i], r[i]]`, the active minutes are exactly `r[i] - l[i]` (since the interval is inclusive, but the difference in minutes works because the device is on for the whole closed interval, and each minute is counted once). So for each interval, add `p1 * (r[i] - l[i])`.
//
// For each gap between interval `i` and `i+1`, let `gap = l[i+1] - r[i]` (the number of idle minutes, since the device is off from the end of interval `i` to the start of interval `i+1`; assuming intervals are disjoint and ordered, `gap` is positive). Then apply the tiered idle rates: first `min(gap, t1)` minutes at `p1`, then `min(max(gap - t1, 0), t2)` minutes at `p2`, then `max(gap - t1 - t2, 0)` minutes at `p3`. Sum these for all gaps.
//
// Edge cases: If `t1` or `t2` is zero, the corresponding tier contributes nothing. If `n == 1`, there are no gaps, so only active consumption is counted. The input vectors are guaranteed to be non-empty if `n > 0`, but for robustness, the function should handle `n == 0` (return 0) even though the problem statement implies `n >= 1`. Time complexity is O(n) because we iterate through the intervals once. Space complexity is O(1) auxiliary, ignoring input storage.
