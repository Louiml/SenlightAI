Write a C++ function `minimumCoins(int n, int target, const std::vector<int>& coins)` that takes the number of coin denominations `n`, a target amount `target`, and a vector of coin values (each positive integer), and returns the minimum number of coins needed to make exactly `target` using unlimited coins of each denomination. If the target cannot be formed, return `-1`. The function must handle `target = 0` (return 0), and handle cases where some coin denominations are larger than the target. Assume all coin values are positive and `n >= 1`.

#include <cassert>
#include <vector>

int main() {
    // Basic cases
    assert(minimumCoins(3, 11, {1, 2, 5}) == 3);      // 5+5+1
    assert(minimumCoins(1, 0, {7}) == 0);              // zero amount
    assert(minimumCoins(2, 3, {2, 4}) == -1);          // impossible
    assert(minimumCoins(1, 5, {3}) == -1);             // coin larger than target

    // Large coin ignores, unlimited usage
    assert(minimumCoins(3, 10, {7, 3, 2}) == 4);       // 3+3+2+2

    // Single coin repeated
    assert(minimumCoins(1, 6, {2}) == 3);              // 2+2+2

    // Multiple same values
    assert(minimumCoins(2, 8, {4, 4}) == 2);           // 4+4

    // Several denominations, minimal combination
    assert(minimumCoins(4, 12, {1, 5, 2, 10}) == 2);   // 10+2

    // Large target small coin
    assert(minimumCoins(1, 1000, {1}) == 1000);

    // Target larger than all coins impossible
    assert(minimumCoins(2, 9, {4, 6}) == -1);

    return 0;
}

#include <vector>
#include <climits>
#include <algorithm>

// Returns minimum number of coins to make 'target' using unlimited coins from 'coins'.
// Returns -1 if target cannot be formed.
int minimumCoins(int n, int target, const std::vector<int>& coins) {
    const int INF = INT32_MAX;
    std::vector<int> dp(target + 1, INF);
    dp[0] = 0;

    for (int amount = 1; amount <= target; ++amount) {
        for (int j = 0; j < n; ++j) {
            int coin = coins[j];
            if (amount - coin >= 0 && dp[amount - coin] != INF) {
                dp[amount] = std::min(dp[amount], dp[amount - coin] + 1);
            }
        }
    }

    return (dp[target] == INF) ? -1 : dp[target];
}

// This is a classic unbounded knapsack / coin change problem solved via dynamic programming. We define a 1D DP array `dp[i]` = minimum coins needed to form amount `i`. Initialize `dp[0] = 0` and all other entries to a large sentinel (e.g., `INT32_MAX`). For each amount from 1 to `target`, iterate over all coin denominations. If a coin value `c` is ≤ `i` and `dp[i - c]` is not infinite, update `dp[i] = min(dp[i], dp[i - c] + 1)`. This works because we allow unlimited use of each coin (we can reuse smaller amounts already computed). After filling the table, if `dp[target]` is still infinite, return `-1`; otherwise return `dp[target]`. Edge cases: `target = 0` already returns 0; coins larger than target are naturally ignored because the condition `i - coins[j] >= 0` fails. Time complexity is O(target * n), space complexity O(target). No recursion is used, avoiding stack overflow for large targets.
