// Write a C++ function `int minimumTimeToBreakAll(std::vector<int>& strength, int K)` that solves the following problem: You are given a list of `n` integers `strength` representing the required energy to break each lock, and an integer `K`. You start with an initial energy multiplier `x = 1`. In one operation, you may choose any lock that has not yet been broken, spend `ceil(strength[i] / x)` units of time to break it, and then increase your multiplier by `K` (so `x` becomes `x + K`). You must break all locks. Your goal is to minimize the total time spent. The order in which you break locks is up to you. Return the minimum total time. The function must handle `n` up to 12 (so `1 << n` is valid for bitmask DP), values of `strength` up to 10^4, `K` positive up to 100. The initial `x` is always 1. Note that `ceil(a / b)` for positive integers can be computed as `(a + b - 1) / b` using integer arithmetic. The function should be efficient enough for the given constraints.
#include <cassert>
#include <vector>

int main() {
    // Test 1: simple case, all equal, K large
    {
        std::vector<int> strength = {2, 2};
        int K = 5;
        // Option1: break first with x=1: time=2, then x=6, break second: ceil(2/6)=1, total=3
        // Option2: symmetric, same total=3
        assert(minimumTimeToBreakAll(strength, K) == 3);
    }
    // Test 2: increasing strength, K=0? Actually K is positive, use K=1
    {
        std::vector<int> strength = {1, 3, 6};
        int K = 1;
        // Try order: 1 (x=1, time1), 3 (x=2, ceil(3/2)=2), 6 (x=3, ceil(6/3)=2) => total5
        // Other orders: e.g., 3 first (time3), then 1 (x=2, time1), then 6 (x=3, time2) total6; so min=5
        assert(minimumTimeToBreakAll(strength, K) == 5);
    }
    // Test 3: single lock
    {
        std::vector<int> strength = {10};
        int K = 100;
        assert(minimumTimeToBreakAll(strength, K) == 10);
    }
    // Test 4: all locks have strength 1, K=1
    {
        std::vector<int> strength = {1, 1, 1};
        int K = 1;
        // x=1,2,3 => times: 1 + 1 + 1 = 3
        assert(minimumTimeToBreakAll(strength, K) == 3);
    }
    // Test 5: n=12 random-like, expect non-negative result (basic sanity)
    {
        std::vector<int> strength = {5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60};
        int K = 3;
        int result = minimumTimeToBreakAll(strength, K);
        assert(result > 0);
        assert(result <= 12 * 60); // upper bound: all broken with x=1
    }
    // Test 6: zero strength? Problem says strength positive, but handle anyway
    {
        std::vector<int> strength = {0};
        int K = 1;
        // ceil(0/1)=0, so total 0
        assert(minimumTimeToBreakAll(strength, K) == 0);
    }
    return 0;
}
#include <vector>
#include <algorithm>
#include <climits>

// Returns the minimum total time to break all locks given their strengths and increment K.
// DP over bitmask: x is determined by popcount(mask) because each break adds K to x starting from 1.
int minimumTimeToBreakAll(std::vector<int>& strength, int K) {
    int n = (int)strength.size();
    if (n == 0) return 0;
    
    int fullMask = (1 << n) - 1;
    std::vector<int> dp(1 << n, INT_MAX);
    dp[fullMask] = 0; // all broken, no more time
    
    // Iterate from fullMask down to 0 (since transitions go from mask to supersets)
    for (int mask = fullMask; mask >= 0; --mask) {
        // x for this state: 1 + (number of broken locks) * K
        int broken = __builtin_popcount(mask);
        int x = 1 + broken * K;
        // Try breaking each remaining lock
        for (int i = 0; i < n; ++i) {
            if (mask & (1 << i)) continue; // already broken
            int nextMask = mask | (1 << i);
            int timeToBreak = (strength[i] + x - 1) / x; // ceil division
            if (dp[mask] > timeToBreak + dp[nextMask]) {
                dp[mask] = timeToBreak + dp[nextMask];
            }
        }
    }
    return dp[0];
}
// The problem is a classic assignment/ordering optimization that can be solved using bitmask dynamic programming (DP). Since `n <= 12`, we can represent the set of broken locks as a bitmask of length `n`. The state is `mask`, which indicates which locks have already been broken. The current multiplier `x` is completely determined by the number of broken locks: if `cnt` bits are set in `mask`, then `x = 1 + cnt * K` because each break increments the multiplier by `K` starting from 1. Therefore, we only need to memoize on the mask. The DP state is `dp[mask]` = minimum total time to reach the state where all locks in `mask` have been broken, starting from the initial state (empty mask). The transition: from state `mask`, for each lock `i` not in `mask`, we can break it next, spending `ceil(strength[i] / x)` where `x = 1 + popcount(mask) * K`, and then transition to `mask | (1 << i)`. The base case is `dp[fullMask] = 0` (or we can do recursive top-down with memoization as in the given snippet). Because `x` is deterministic given the mask, we don't need a second dimension. Edge cases: `n = 0` (should return 0, but input guarantees at least one lock? We'll handle it), `K` can be large so `x` grows quickly, integer overflow is not an issue within given constraints (max time per lock when x=1 is 10^4, and n<=12 so total < 10^5). The time complexity is O(n * 2^n) for the DP (each mask tries up to n transitions) and space O(2^n). This is efficient for n<=12.
