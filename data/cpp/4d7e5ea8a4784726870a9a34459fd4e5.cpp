/*
Write a C++ function `minimumCoinCount(int n)` that, given a positive integer `n` (1 ≤ n ≤ 100,000), returns the minimum number of coins needed to make exactly `n` using only powers of 6 (1, 6, 36, 216, …) and powers of 9 (1, 9, 81, 729, …). Each coin can be used any number of times, and you can mix coins of both types. For example, for `n = 10`, the answer is 2 (using 9 + 1), and for `n = 13`, the answer is 3 (9 + 1 + 1 + 1 + 1? Actually 13 = 9 + 1 + 1 + 1 + 1, that's 5, but better is 13 = 6 + 6 + 1, that's 3 coins). The function should efficiently handle all values up to 100,000.
*/
#include <vector>
#include <algorithm>

// Returns the minimum number of coins needed to make amount n using powers of 6 and 9.
int minimumCoinCount(int n) {
    const int INF = 1e9;
    std::vector<int> dp(n + 1, INF);
    dp[0] = 0;

    std::vector<int> coins;
    // powers of 6
    for (int p = 1; p <= n; p *= 6) {
        coins.push_back(p);
    }
    // powers of 9
    for (int p = 1; p <= n; p *= 9) {
        coins.push_back(p);
    }

    for (int coin : coins) {
        for (int amount = coin; amount <= n; ++amount) {
            dp[amount] = std::min(dp[amount], dp[amount - coin] + 1);
        }
    }
    return dp[n];
}
#include <cassert>

int main() {
    // Base case with just the 1-coin
    assert(minimumCoinCount(1) == 1);
    // 2 = 1 + 1
    assert(minimumCoinCount(2) == 2);
    // 6 = 6
    assert(minimumCoinCount(6) == 1);
    // 9 = 9
    assert(minimumCoinCount(9) == 1);
    // 10 = 9 + 1
    assert(minimumCoinCount(10) == 2);
    // 13 = 6 + 6 + 1 (3 coins) vs 9+1+1+1+1 (5 coins)
    assert(minimumCoinCount(13) == 3);
    // 36 = 36
    assert(minimumCoinCount(36) == 1);
    // 81 = 81
    assert(minimumCoinCount(81) == 1);
    // 100 = 81 + 9 + 9 + 1? Actually 81+9+9+1=100, 4 coins, but better 36+36+18? 18 not coin, so 81+9+9+1 = 4, or 36+36+9+9+9+1? worse. So 4.
    assert(minimumCoinCount(100) == 4);
    // 100000 is reachable (since 1 is included) and should be > 0
    assert(minimumCoinCount(100000) > 0);
    return 0;
}
// This is a classic unbounded knapsack (coin change) problem where we have two sets of coin denominations: powers of 6 and powers of 9, both starting from 1. We can precompute all coin denominations that are ≤ 100,000. Note that 6^6 = 46,656 and 6^7 = 279,936 > 100,000, so we include 6^0 through 6^6. Similarly 9^5 = 59,049 and 9^6 = 531,441 > 100,000, so we include 9^0 through 9^5. The dynamic programming approach uses a 1D array `dp[i]` representing the minimum number of coins to make amount `i`. Initialize `dp[0] = 0` and all other `dp[i]` to a large number (e.g., 1e9). For each coin denomination `c`, iterate `j` from `c` to `n` and update `dp[j] = min(dp[j], dp[j - c] + 1)`. This works because each coin can be used infinitely (unbounded). Edge case: `n = 1` should return 1 (using the 1-coin). Since the coin set always includes 1, every `n` is reachable. Time complexity is O((number of coins) * n), which is about (7 + 6) * 100,000 ≈ 1.3 million operations, very fast. Space complexity is O(n) for the DP array.
