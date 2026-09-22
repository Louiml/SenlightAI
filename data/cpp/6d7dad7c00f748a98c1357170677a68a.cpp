/*
Write a C++ function `int maxProfitWithCooldown(const std::vector<int>& prices)` that computes the maximum profit you can achieve from a sequence of daily stock prices. You may complete as many transactions as you like (buy one share and sell one share per transaction), but you must sell before buying again, and after selling you must wait one full day before buying again (cooldown of 1 day). You may choose not to trade on any day. The function should return the maximum achievable profit. The input vector may be empty, in which case the profit is 0. The prices are non-negative integers.
*/
#include <vector>
#include <algorithm>

// Returns the maximum profit with a one-day cooldown after selling.
int maxProfitWithCooldown(const std::vector<int>& prices) {
    int n = static_cast<int>(prices.size());
    if (n == 0) return 0;

    // dp[i][0]: max profit from day i when we can buy (no stock held)
    // dp[i][1]: max profit from day i when we hold a stock
    std::vector<std::vector<int>> dp(n + 2, std::vector<int>(2, 0));

    // Because dp[i] depends on dp[i+1] and dp[i+2], iterate backwards.
    for (int i = n - 1; i >= 0; --i) {
        // State 0: can buy
        int skip_buy = dp[i + 1][0];
        int buy = -prices[i] + dp[i + 1][1];
        dp[i][0] = std::max(skip_buy, buy);

        // State 1: holding stock, can sell
        int skip_sell = dp[i + 1][1];
        // After selling, we must skip the next day (i+1), so we go to day i+2.
        int sell = prices[i] + dp[i + 2][0];
        dp[i][1] = std::max(skip_sell, sell);
    }

    return dp[0][0];
}
#include <cassert>
#include <vector>

int main() {
    // Empty list
    assert(maxProfitWithCooldown({}) == 0);

    // Single price, no transaction possible
    assert(maxProfitWithCooldown({5}) == 0);

    // All decreasing, no profit
    assert(maxProfitWithCooldown({5,4,3,2,1}) == 0);

    // All increasing, buy first sell last, no cooldown issue since only one transaction
    assert(maxProfitWithCooldown({1,2,3,4,5}) == 4);

    // Classic case with cooldown: [1,2,3,0,2] -> buy at 1, sell at 2, cooldown at 3, buy at 0, sell at 2 => profit (2-1)+(2-0)=3
    assert(maxProfitWithCooldown({1,2,3,0,2}) == 3);

    // Another cooldown example from LeetCode
    assert(maxProfitWithCooldown({1,2,4}) == 3); // buy at 1, sell at 4, no cooldown needed since only one transaction

    // Multiple transactions with cooldown: [2,1,4,5,2,9,7] -> buy at 1, sell at 5 (profit 4), cooldown at 2, buy at 2, sell at 9 (profit 7) total 11
    assert(maxProfitWithCooldown({2,1,4,5,2,9,7}) == 11);

    // Equal prices, no profit
    assert(maxProfitWithCooldown({3,3,3,3}) == 0);

    // Two days: buy then sell works if profit positive
    assert(maxProfitWithCooldown({1,2}) == 1);

    // Two days: decreasing, no profit
    assert(maxProfitWithCooldown({2,1}) == 0);

    return 0;
}
// The problem is a classic dynamic programming on stock trading with a cooldown. We maintain two states: `action=0` means we are allowed to buy (we have no stock in hand and no cooldown), `action=1` means we own a stock and can either hold or sell. However, the original code does not explicitly model the cooldown day; it simply alternates actions, which for this problem must be modified. The correct recurrence is:  
// - `dp[i][0]` = max profit from day `i` onwards when we are in "buying" state (can buy today).  
// - `dp[i][1]` = max profit from day `i` onwards when we own a stock (can sell today).  
// At each day, we can either skip (do nothing) or take the action (buy if state 0, sell if state 1). After selling, we must skip the next day (cooldown). The recurrence is:  
// `dp[i][0] = max(dp[i+1][0], -prices[i] + dp[i+1][1])`  
// `dp[i][1] = max(dp[i+1][1], prices[i] + dp[i+2][0])`  (note `dp[i+2][0]` because after selling we avoid day i+1).  
// Base cases: if `i >= n`, profit = 0. For `dp[i][1]` when `i == n-1`, `dp[i+2][0]` is out of bounds, so `dp[i+2][0]` is 0.  
// We can solve with memoization (top-down) or iteratively. Edge cases: empty prices → return 0; only one day → return 0 (cannot buy and sell profitably). The time complexity is O(n) and space O(n) for the DP table (we can optimize to O(1) but O(n) is fine given the original code's style).
