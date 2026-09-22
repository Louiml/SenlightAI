// Write a C++ function named `maxProfitWithCooldown` that takes a `const std::vector<int>&` representing daily stock prices (in chronological order) and returns the maximum profit achievable. You may complete at most one transaction per day (buy then sell), but after selling, you must wait exactly one day (a cooldown) before buying again. You may hold at most one share at any time, and you can engage in multiple transactions as long as the cooldown rule is respected. The function should handle an empty price list (return 0) and any number of days. Implement the solution using dynamic programming with O(n) time complexity and O(1) auxiliary space (or O(n) if you choose to store the full DP table, but the expected solution uses a rolling-array approach). The function must be `const` correct and not modify the input vector.

// The problem is a variation of the classic "Best Time to Buy and Sell Stock" with a cooldown period. The key state transition is captured in the original snippet: `dp[i][buy]` represents the maximum profit starting from day `i` where `buy == 1` means we are allowed to buy (or already in a "buying state"), and `buy == 0` means we are holding a stock (so we can only sell or hold). However, the snippet's implementation has a subtle off-by-one: when we sell on day `i`, the next buying opportunity is at day `i+2` (one day cooldown). The original code uses `dp[i+2]` when selling, but in its iterative version, it correctly computes `dp[i+2]` because the DP table has size `n+2` and is initialized to 0. The main algorithm: process days from the last to the first. For each day, `dp[i][1]` (can buy) is max of (buy today: `-prices[i] + dp[i+1][0]`) or (skip: `dp[i+1][1]`). `dp[i][0]` (holding) is max of (sell today: `prices[i] + dp[i+2][1]`) or (hold: `dp[i+1][0]`). Since only the states for `i+1` and `i+2` are needed, we can optimize to O(1) space by maintaining variables for `dp[i+2][0]`, `dp[i+1][0]`, `dp[i+2][1]`, `dp[i+1][1]`. Edge cases: empty vector (return 0), one day (cannot make profit), all decreasing prices (best to not buy). Time complexity O(n), space O(1) in the optimized version.

#include <vector>
#include <algorithm>

// Returns the maximum profit achievable with a one-day cooldown after selling.
// The input vector 'prices' is not modified.
int maxProfitWithCooldown(const std::vector<int>& prices) {
    int n = static_cast<int>(prices.size());
    if (n == 0) return 0;

    // State variables:
    // dp_i1_canBuy   : max profit starting from day i with permission to buy.
    // dp_i1_holding  : max profit starting from day i while holding a stock.
    // dp_i2_canBuy   : same but for day i+1 (used for cooldown after selling).
    // dp_i2_holding  : same but for day i+1.
    int dp_i1_canBuy = 0;
    int dp_i1_holding = 0;
    int dp_i2_canBuy = 0;
    int dp_i2_holding = 0;

    // Process from last day to first.
    for (int i = n - 1; i >= 0; --i) {
        int new_canBuy = 0;
        int new_holding = 0;

        // If we can buy today, either buy (cost prices[i] and become holding) or skip.
        new_canBuy = std::max(-prices[i] + dp_i1_holding, dp_i1_canBuy);

        // If we are holding today, either sell (gain prices[i] and wait one day cooldown)
        // or continue holding.
        new_holding = std::max(prices[i] + dp_i2_canBuy, dp_i1_holding);

        // Shift states: today's become tomorrow's for next iteration.
        dp_i2_canBuy = dp_i1_canBuy;
        dp_i2_holding = dp_i1_holding;
        dp_i1_canBuy = new_canBuy;
        dp_i1_holding = new_holding;
    }

    return dp_i1_canBuy; // Start with permission to buy on day 0.
}

#include <cassert>
#include <vector>

int main() {
    // Empty list
    assert(maxProfitWithCooldown({}) == 0);

    // Single day
    assert(maxProfitWithCooldown({5}) == 0);

    // All decreasing, best to never buy
    assert(maxProfitWithCooldown({5,4,3,2,1}) == 0);

    // Simple case: buy day 0, sell day 1, cooldown day 2, then buy day 3 sell day 4
    // prices: [1,2,3,0,2] => profit = (2-1)+(2-0)=3
    assert(maxProfitWithCooldown({1,2,3,0,2}) == 3);

    // Buy day 1 sell day 2, cooldown day 3, no further trades
    // prices: [2,1,4] => profit = 3
    assert(maxProfitWithCooldown({2,1,4}) == 3);

    // Case where skipping is better than buying early
    // prices: [1,2,4] => buy day 0 sell day 2 => 3 (same as buy day1 sell day2 => 2, so best is 3)
    assert(maxProfitWithCooldown({1,2,4}) == 3);

    // Multiple cycles with cooldown
    // prices: [1,2,3,1,2] => profit = (2-1)+(2-1)=2 (cooldown between sells)
    assert(maxProfitWithCooldown({1,2,3,1,2}) == 2);

    // Long alternating pattern
    // prices: [3,2,6,5,0,3] => best: buy 2 sell 6 (4), cooldown, buy 0 sell 3 (3) total 7
    assert(maxProfitWithCooldown({3,2,6,5,0,3}) == 7);

    // All equal prices, no profit
    assert(maxProfitWithCooldown({4,4,4,4}) == 0);

    // Big drop then rise
    // prices: [10,1,5,1,6] => buy 1 sell 5 (4), cooldown, buy 1 sell 6 (5) total 9
    assert(maxProfitWithCooldown({10,1,5,1,6}) == 9);

    return 0;
}
