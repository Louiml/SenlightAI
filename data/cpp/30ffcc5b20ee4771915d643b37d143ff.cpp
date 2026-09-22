You are given an array of `n` positive integers representing daily returns, and you must choose exactly `k` distinct days to "invest", where `k <= n`. The total return from a chosen set of days is the sum of the selected array values, but each selected day after the first must be at most `d` days apart from the previous selected day (i.e., if you select indices `i_1 < i_2 < ... < i_k`, then `i_{j+1} - i_j <= d` for all `j`). Additionally, there is a multiplier `m` (a positive integer) that scales every return after the first selected day: the first selected day contributes its original value, and each subsequent selected day contributes `m` times its original value. Write a C++ function `int maxReturn(int n, int k, int d, int m, const std::vector<int>& arr)` that returns the maximum possible total return under these rules. You may assume that `n >= 1`, `1 <= k <= n`, `0 <= d < n`, `m >= 1`, and that the total sum of array values fits in a 32-bit signed integer (but intermediate sums may be larger, so use 64-bit internally). The input array values are positive, and you must handle the case where no valid selection exists (i.e., when `(k-1)*d < n-1`? Actually, because you can skip days, the condition for existence is that there is at least one sequence of `k` indices with gaps ≤ `d`, which is always true if `k <= n` and `d >= 0`, but for `d=0` you can only pick same-day? Wait: `d` is the maximum gap between consecutive selected indices, so if `d=0`, you can only pick consecutive indices? Actually gap means difference, so if `d=0`, the difference must be ≤ 0, which forces consecutive selected indices to be identical, impossible for distinct days. So for `d=0` and `k>1`, no valid selection exists; in that case return 0. Similarly, if `d` is small, it might be impossible to pick `k` distinct indices with gaps ≤ `d` if `n` is too small. The general condition is that you need `(k-1) * d >= k-1`? Actually the maximum number of indices you can pick with gaps ≤ `d` from `n` days is bounded by `floor((n-1)/d)+1` if `d>0`, else 1. So if `k` exceeds that, return 0.
// The core of the problem is a dynamic programming over a sliding window of possible previous selected indices. Define `dp[t][i]` as the maximum total return after selecting `t` days, with the last selected day being index `i` (0-based). The first selected day has no multiplier, so `dp[1][i] = arr[i]`. For `t >= 2`, `dp[t][i] = arr[i]*m + max_{j < i, i-j <= d} dp[t-1][j]`. The answer is `max_i dp[k][i]`. Directly computing this takes `O(n^2 * k)` time, which is too slow for large `n` and `k`. Instead, we can use a sliding-window maximum for each `t`: as we iterate `i` from left to right, we maintain a deque of indices with decreasing `dp[t-1]` values, and keep only those with index difference ≤ `d`. Then for each `i`, the best previous value is at the front of the deque. This reduces per-layer cost to `O(n)`, so total time is `O(n*k)`. Memory can be optimized to `O(n)` by keeping only two layers. Edge cases: if `k == 1`, return the maximum element in `arr` (since no multiplier applies). If `d == 0`, only `k == 1` is possible; otherwise return 0 (since you can't pick distinct days with gap 0). Also, if `k > n`, return 0 (though problem states `k <= n`). Use `long long` for sums because `m` can be large (up to int range) and `n` up to, say, 10^5, so intermediate sums may exceed 32-bit. The time complexity is `O(n*k)`, and space is `O(n)`.
#include <vector>
#include <deque>
#include <algorithm>
#include <cstdint>

// Returns the maximum total return when selecting exactly k days
// with max gap d between consecutive selections.
// m is the multiplier for every day after the first.
std::int64_t maxReturn(int n, int k, int d, int m, const std::vector<int>& arr) {
    // Quick feasibility checks
    if (k > n) return 0;
    if (d == 0) return (k == 1) ? *std::max_element(arr.begin(), arr.end()) : 0;
    // Maximum number of days we can pick with gap <= d from n days:
    // If d >= 1, we can pick at most floor((n-1)/d) + 1 days.
    int maxPossible = (n - 1) / d + 1;
    if (k > maxPossible) return 0;

    // dp[i] = best return for current layer ending at index i
    std::vector<std::int64_t> dp(n, 0), prev(n, 0);

    // Base layer: choosing 1 day, no multiplier
    for (int i = 0; i < n; ++i) prev[i] = arr[i];
    if (k == 1) return *std::max_element(prev.begin(), prev.end());

    // Build layers t = 2 .. k
    for (int t = 2; t <= k; ++t) {
        // Sliding window maximum over prev[], restricted to indices j < i and i - j <= d
        std::deque<int> dq; // stores indices with decreasing prev values
        for (int i = 0; i < n; ++i) {
            // Remove indices that are too far left (gap > d)
            while (!dq.empty() && dq.front() < i - d) dq.pop_front();
            // Add current index i-1 to the deque (since we need j < i, so we add i-1)
            // Actually we need to add index i-1 before using for i
            if (i > 0) {
                int idx = i - 1;
                while (!dq.empty() && prev[dq.back()] <= prev[idx]) dq.pop_back();
                dq.push_back(idx);
            }
            // Now the best previous value for index i is at front
            if (dq.empty()) {
                dp[i] = -1; // impossible
            } else {
                dp[i] = (std::int64_t)arr[i] * m + prev[dq.front()];
            }
        }
        // Prepare for next layer
        prev.swap(dp);
        // Reset dp to -infinity for next iteration? Actually we will overwrite all entries,
        // but we can just leave it; each entry is written in the loop.
    }

    // Answer is max over all ending indices
    std::int64_t ans = *std::max_element(prev.begin(), prev.end());
    return ans;
}
#include <cassert>
#include <vector>
#include <cstdint>

// Assume the solution function is defined above (paste it here for testing)
// For brevity, I'll include a minimal copy.

int main() {
    // Test 1: Simple case
    {
        std::vector<int> arr = {1, 2, 3, 4, 2};
        // n=5, k=3, d=2, m=11
        // Manually compute best: pick indices 1 (2), 3 (4*11=44), 5? Actually we have 0-based: 1 (value 2), 3 (value 4*11), and maybe 4 (value 2*11) but gap from 3 to 4 is 1<=2, so sum=2+44+22=68. But maybe pick 0,2,4: 1 + 3*11 + 2*11 = 1+33+22=56. So max is 68.
        std::int64_t result = maxReturn(5, 3, 2, 11, arr);
        assert(result == 68);
    }
    // Test 2: k=1, returns max element
    {
        std::vector<int> arr = {5, 10, 7};
        assert(maxReturn(3, 1, 20, 3, arr) == 10);
    }
    // Test 3: d=0, k>1 -> 0
    {
        std::vector<int> arr = {1, 2, 3};
        assert(maxReturn(3, 2, 0, 5, arr) == 0);
    }
    // Test 4: Impossible due to gap constraint
    {
        // n=4, k=3, d=1 => need two gaps of at most 1, but total span needed is at least 2, possible? Indices 0,1,2 works (gaps 1,1). Actually possible. Let's try n=3,k=3,d=1: must pick all three, gaps 1 and 1, ok. To be impossible: n=5,k=4,d=1 => need span at least 3, but 0,1,2,3 works (gap 1 each) actually possible. Try n=5,k=5,d=1 => must pick all, gaps 1 each, possible. So d=1 always allows all n? Actually maxPossible = (n-1)+1 = n, so always possible. Let's test d=0 already done. For a real impossible case: n=4,k=3,d=0? already. n=6,k=4,d=1 => choose 0,2,4,5? gaps 2,2,1 -> 2>1 invalid. But can we choose 0,1,2,3? gaps 1,1,1 ok. So many possible. Let's make n=5,k=4,d=1 => choose 0,1,2,3 works. So hard to make impossible with d>=1 unless k > maxPossible. For n=5,d=1, maxPossible=5, so all k possible. Let's set n=10,d=2,k=6 => maxPossible = floor(9/2)+1 = 4+1=5, so k=6 > 5 -> return 0.
        std::vector<int> arr = {1,2,3,4,5,6,7,8,9,10};
        assert(maxReturn(10, 6, 2, 1, arr) == 0);
    }
    // Test 5: Negative? Not allowed (positive), but test large multiplication
    {
        std::vector<int> arr = {1000000, 1000000, 1000000, 1000000, 1000000};
        // n=5, k=5, d=4, m=1000000 => each after first is multiplied by 1e6
        // Choose all indices: sum = 1e6 + 4*1e12 = 4e12+1e6
        std::int64_t expected = (std::int64_t)1000000 + 4 * (std::int64_t)1000000 * 1000000;
        assert(maxReturn(5, 5, 4, 1000000, arr) == expected);
    }
    // Test 6: All same values and d large
    {
        std::vector<int> arr = {7,7,7,7,7};
        // k=3,d=2,m=2 => best pick any 3, sum = 7 + 2*7 + 2*7 = 35
        assert(maxReturn(5, 3, 2, 2, arr) == 35);
    }
    // Test 7: d large enough to pick any
    {
        std::vector<int> arr = {1, 5, 3, 9, 2};
        // k=2, d=10, m=1 => best pair is 5+9=14 (since first no multiplier, second *1)
        assert(maxReturn(5, 2, 10, 1, arr) == 14);
    }
    return 0;
}
