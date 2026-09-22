/*
Write a C++ function `int maxProfitWithAtMostKTransactions(int k, const std::vector<int>& prices)` that, given a maximum number of allowed transactions `k` (each transaction consists of one buy and one sell, and you may not hold more than one share at a time) and a chronological list of daily stock prices, returns the maximum profit achievable. You may complete at most `k` transactions. If it is impossible to make a profit, return 0. The function must handle empty or single-element price lists, negative prices (though normally non‑negative, treat them generically), and large values of `k` efficiently.
*/
#include <vector>
#include <algorithm>

// Returns the maximum profit achievable with at most k transactions
// given a chronological list of prices. Each transaction is a buy followed
// by a sell. You may not hold more than one share at a time.
int maxProfitWithAtMostKTransactions(int k, const std::vector<int>& prices) {
    const size_t n = prices.size();
    if (n < 2 || k <= 0) return 0;

    // If k is large enough, we can capture every upward movement.
    if (static_cast<size_t>(k) >= (n >> 1)) {
        int totalProfit = 0;
        for (size_t i = 1; i < n; ++i) {
            int diff = prices[i] - prices[i - 1];
            if (diff > 0) totalProfit += diff;
        }
        return totalProfit;
    }

    // DP arrays:
    // f[t] = max profit with at most t transactions up to current day.
    // g[t] = max profit with at most t transactions and a sale on current day.
    std::vector<int> f(k + 1, 0);
    std::vector<int> g(k + 1, 0);

    for (size_t i = 1; i < n; ++i) {
        int diff = prices[i] - prices[i - 1];
        // tmp holds f[t-1] from the previous day iteration.
        int tmp = f[0];
        for (int t = 1; t <= k; ++t) {
            // g[t] = max(g[t] from previous day (i-1), f[t-1] from previous day) + diff
            g[t] = std::max(g[t], tmp) + diff;
            tmp = f[t];
            // f[t] = max(not selling today (previous f[t]), selling today)
            f[t] = std::max(tmp, g[t]);
        }
    }

    return f[k];
}
#include <cassert>
#include <vector>

int main() {
    // Basic test: two transactions allowed, prices go up and down.
    std::vector<int> prices1 = {3, 3, 5, 0, 0, 3, 1, 4};
    assert(maxProfitWithAtMostKTransactions(2, prices1) == 6);

    // Only one transaction allowed.
    std::vector<int> prices2 = {1, 2, 3, 4, 5};
    assert(maxProfitWithAtMostKTransactions(1, prices2) == 4);

    // Prices only go down, no profit possible.
    std::vector<int> prices3 = {7, 6, 4, 3, 1};
    assert(maxProfitWithAtMostKTransactions(2, prices3) == 0);

    // Large k (>= n/2) triggers greedy path.
    std::vector<int> prices4 = {1, 2, 3, 4, 5};
    assert(maxProfitWithAtMostKTransactions(10, prices4) == 4);

    // Empty and single-element lists.
    std::vector<int> prices5;
    assert(maxProfitWithAtMostKTransactions(3, prices5) == 0);
    std::vector<int> prices6 = {5};
    assert(maxProfitWithAtMostKTransactions(3, prices6) == 0);

    // Zero transactions.
    std::vector<int> prices7 = {1, 2, 3};
    assert(maxProfitWithAtMostKTransactions(0, prices7) == 0);

    // Negative prices (generic handling).
    std::vector<int> prices8 = {-1, -2, -3, 0, 5};
    assert(maxProfitWithAtMostKTransactions(2, prices8) == 8); // buy at -3, sell at 0, then buy at 0, sell at 5? Actually best: buy at -3, sell at 5 = 8, or two transactions? Let's compute: (0 - (-3)) + (5 - 0) = 8, same.

    // Mixed: multiple transactions but k limits.
    std::vector<int> prices9 = {2, 1, 4, 5, 2, 9, 7};
    assert(maxProfitWithAtMostKTransactions(2, prices9) == 11); // buy at 1, sell at 5 (profit 4), then buy at 2, sell at 9 (profit 7) => 11.

    // Price list with equal consecutive prices.
    std::vector<int> prices10 = {1, 1, 1, 1};
    assert(maxProfitWithAtMostKTransactions(3, prices10) == 0);

    return 0;
}
// The core challenge is that a naive DP over days and transactions would be too slow if `k` is large. We observe two cases:
// 1. If `k` is at least half the number of days (since each transaction requires at least two days), then we can make as many transactions as we want. In that case, the optimal strategy is to sum every positive daily price difference (`prices[i] - prices[i-1]` when positive). This is because you can buy and sell on consecutive days to capture every upward movement.
// 2. Otherwise, we use a dynamic programming approach that tracks two quantities per transaction count:
//    - `f[t]`: the maximum profit achievable in the processed prefix (up to current day) with at most `t` transactions.
//    - `g[t]`: the maximum profit achievable in the processed prefix with at most `t` transactions *and* a sale occurring on the current day.
//    
//    The recurrence is derived by considering whether to not sell today (`f[t]` stays from previous day) or to sell today (`g[t]`). To compute `g[t]` we need the best possible `f[t-1][j] - prices[j]` over all previous days `j`, which we can maintain incrementally. The final answer after iterating over all days is `f[k]`.
//
// Important edge cases:
// - `prices.size() < 2` → return 0.
// - `k <= 0` → return 0.
// - Negative price differences are ignored (we never take a loss).
// - When `k` is very large, we switch to the greedy sum-of-positive-differences method to avoid O(n·k) time.
//
// Time complexity: O(n·k) for the general DP case, or O(n) for the large‑k case. Space complexity: O(k) for the DP arrays (or O(1) for the greedy approach).
