You are given a vector `cost` of `n` positive integers, where `cost[i]` is the fee required to step on the i-th stair (0-indexed). You start at stair `0` or stair `1` for free, and from any stair `i` you may climb 1 or 2 steps, paying the fee of the stair you land on. Write a C++ function `minCostToReachTop` that takes `const std::vector<int>& cost` and returns the minimum total fee required to reach the top (which is the position just beyond the last stair, i.e., index `n`). The input vector is non‑empty, and all values are positive.
#include <cassert>
int main() {
    // Basic cases
    assert(minCostToReachTop({10, 15, 20}) == 15);
    // Example from LeetCode: [1,100,1,1,1,100,1,1,100,1] -> 6
    assert(minCostToReachTop({1,100,1,1,1,100,1,1,100,1}) == 6);
    // Only one stair
    assert(minCostToReachTop({5}) == 0);
    // Two stairs
    assert(minCostToReachTop({5,7}) == 0);
    // All equal
    assert(minCostToReachTop({3,3,3,3}) == 6);
    // Increasing fees
    assert(minCostToReachTop({1,2,3,4,5}) == 6);
    // Decreasing fees
    assert(minCostToReachTop({5,4,3,2,1}) == 4);
    // Large steps
    assert(minCostToReachTop({7,1,2,9,3}) == 4);
    // Two-element case with high fees
    assert(minCostToReachTop({100,100}) == 0);
    // Edge with many zeros (though positive values requested, test robustness)
    assert(minCostToReachTop({1,1,1,1,1}) == 2);
    return 0;
}
#include <vector>
#include <algorithm>

// Returns the minimum cost to climb to the top of the stairs.
// You may start at index 0 or 1 for free, and from any stair you may climb 1 or 2 steps.
int minCostToReachTop(const std::vector<int>& cost) {
    if (cost.empty()) return 0;
    int n = cost.size();
    // dp_prev2 = dp[0], dp_prev1 = dp[1]
    int dp_prev2 = 0; // dp[0]
    int dp_prev1 = 0; // dp[1]
    for (int i = 2; i <= n; ++i) {
        int current = std::min(dp_prev1 + cost[i - 1], dp_prev2 + cost[i - 2]);
        dp_prev2 = dp_prev1;
        dp_prev1 = current;
    }
    return (n >= 2) ? dp_prev1 : 0; // For n==1, dp[1]=0
}
// This is a classic dynamic programming problem. Let `dp[i]` be the minimum cost to reach position `i` (where `i` ranges from `0` to `n`, with `0` and `1` being starting positions at zero cost). For any `i >= 2`, you can arrive from `i-1` (paying `cost[i-1]`) or from `i-2` (paying `cost[i-2]`). Thus the recurrence is `dp[i] = min(dp[i-1] + cost[i-1], dp[i-2] + cost[i-2])`. The base cases are `dp[0] = 0` and `dp[1] = 0` because you may start on either stair without paying. The answer is `dp[n]`. Since the recurrence only depends on the previous two values, you can optimize space to O(1) by keeping two variables. Time complexity is O(n) because each position is processed once; space complexity is O(1) if we use the optimized version, or O(n) if we store the full DP array. Edge cases: `n = 1` (only one stair) → answer is 0 because you can start on it and immediately step to the top; `n = 2` → you start at either stair and step to the top, so answer is 0 as well. The recurrence naturally handles these since for `n=1` and `n=2` the loop never executes and `dp[0]`/`dp[1]` are 0.
