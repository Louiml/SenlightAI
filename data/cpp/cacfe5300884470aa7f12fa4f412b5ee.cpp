Given a positive integer `n` and an array `p[0..n-1]` where `p[i]` represents the price of a rod of length `i+1`, write a C++ function `maxRodCutProfit` that returns the maximum revenue obtainable by cutting a rod of length `n` into integer-length pieces and selling each piece at its given price. You may make any number of cuts, including zero cuts, and the rod must be sold in whole pieces whose lengths sum exactly to `n`. The function should take `n` and the vector `p` as inputs and return the maximum profit (which may be zero or positive). Handle the case where `n` is 0 by returning 0 (no pieces can be sold).

#include <cassert>
#include <vector>

// Declaration of the function under test (as defined in the solution)
int maxRodCutProfit(int n, const std::vector<int>& p);

int main() {
    // Basic cases
    assert(maxRodCutProfit(1, {2}) == 2);
    assert(maxRodCutProfit(2, {1, 5}) == 5);  // better to sell whole rod
    assert(maxRodCutProfit(2, {3, 4}) == 6);  // cut into two 1-length pieces

    // Classic example: p = {1, 5, 8, 9}
    assert(maxRodCutProfit(4, {1, 5, 8, 9}) == 10); // 2+2 or 1+3? actually 2+2=10
    assert(maxRodCutProfit(5, {1, 5, 8, 9, 10}) == 13); // 2+3 = 5+8=13
    assert(maxRodCutProfit(3, {1, 5, 8}) == 8); // whole length 3

    // Edge: n = 0
    assert(maxRodCutProfit(0, {1,2}) == 0);

    // Increasing prices: always cut into 1s
    assert(maxRodCutProfit(3, {7, 14, 21}) == 21); // even cutting all 1s gives 21 equals whole rod
    assert(maxRodCutProfit(3, {10, 10, 10}) == 30); // cut into three 1s

    // Large n but simple
    assert(maxRodCutProfit(6, {2, 2, 2, 2, 2, 2}) == 12); // all 1s or any combination

    return 0;
}

#include <vector>
#include <algorithm>

// Returns the maximum profit obtainable by cutting a rod of length n.
// p[i] is the price for a piece of length i+1. p must have size at least n.
int maxRodCutProfit(int n, const std::vector<int>& p) {
    if (n <= 0) return 0;
    std::vector<int> dp(n + 1, 0); // dp[i] = max profit for rod length i

    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= i; ++j) {
            dp[i] = std::max(dp[i], dp[i - j] + p[j - 1]);
        }
    }
    return dp[n];
}

// The problem is the classic "Rod Cutting" dynamic programming problem. Define `dp[i]` as the maximum revenue obtainable for a rod of length `i` (where `i` ranges from 0 to `n`). The base case `dp[0] = 0` (no revenue from a rod of length 0). For each length `i` from 1 to `n`, we consider every possible first cut length `j` (where `1 <= j <= i`). If we cut off a piece of length `j`, we sell it at price `p[j-1]` and then optimally sell the remaining rod of length `i-j`, which yields `dp[i-j]`. Thus, the recurrence is: `dp[i] = max(dp[i], dp[i-j] + p[j-1])` for all `j` from 1 to `i`. By iterating `i` in increasing order, we ensure that `dp[i-j]` is already computed for all smaller lengths. The final answer is `dp[n]`. Edge cases: if `n` is 0, the loop does not execute and we return 0; if `p` is empty or shorter than `n`, the problem is ill-defined, so we assume `p` has at least `n` elements. Time complexity is O(n²) because there are O(n) states and each state considers up to `i` transitions. Space complexity is O(n) for the DP array.
