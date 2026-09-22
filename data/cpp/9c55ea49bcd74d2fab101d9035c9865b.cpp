// Write a C++ function, `int minimumCoins(int n, int k, const std::vector<int>& coinValues)`, that computes the minimum number of coins needed to make a target amount `k` cents using coin denominations of `coinValues`, where the supply of each coin is unlimited. The input coin denominations are guaranteed to be positive integers, sorted in strictly increasing order, and it is guaranteed that the largest coin value is no greater than 1,000,000. Additionally, it is guaranteed that it is always possible to make the exact amount `k` using the given denominations (i.e., the first coin is always 1). The function must return the minimum number of coins. For example, if `n=3`, `k=11`, and coin values are `[1,5,10]`, the answer is `3` (10+1). However, note that the basic greedy approach of always taking the largest coin works for these coin systems (like the one in the snippet), but your solution must be correct for *all* such coin systems (including those where greedy fails, e.g., `[1,3,4]` and `k=6`, greedy gives 4+1+1 = 3 coins, but optimal is 3+3 = 2 coins). Therefore, your implementation must handle arbitrary coin systems (with the guarantee that coin[0] == 1) and compute the true minimum.

#include <cassert>
#include <vector>
#include <iostream>

// (The solution function above is assumed to be included here.)

int main() {
    // Basic tests
    assert(minimumCoins(3, 11, std::vector<int>{1,5,10}) == 3);
    assert(minimumCoins(1, 0, std::vector<int>{1}) == 0);
    assert(minimumCoins(1, 5, std::vector<int>{1}) == 5);

    // Greedy fails here
    assert(minimumCoins(3, 6, std::vector<int>{1,3,4}) == 2); // 3+3, not 4+1+1

    // Larger amounts
    assert(minimumCoins(4, 13, std::vector<int>{1,2,5,10}) == 3); // 10+2+1

    // Edge case: all ones
    assert(minimumCoins(2, 10, std::vector<int>{1,2}) == 5); // five 2s

    // Another greedy-fail case
    assert(minimumCoins(4, 8, std::vector<int>{1,3,5,6}) == 2); // 3+5

    // Large target (just ensure it runs quickly)
    assert(minimumCoins(2, 100000, std::vector<int>{1,99999}) == 2); // 99999+1

    std::cout << "All tests passed!\n";
    return 0;
}

#include <vector>
#include <algorithm>
#include <climits>

// Returns the minimum number of coins to make amount k
// using the given coin denominations (coinValues) with unlimited supply.
// It is guaranteed that coinValues[0] == 1 and the denominations are sorted.
int minimumCoins(int n, int k, const std::vector<int>& coinValues) {
    const int INF = 1e9;
    std::vector<int> dp(k + 1, INF);
    dp[0] = 0;

    for (int i = 0; i < n; ++i) {
        int coin = coinValues[i];
        for (int amount = coin; amount <= k; ++amount) {
            if (dp[amount - coin] + 1 < dp[amount]) {
                dp[amount] = dp[amount - coin] + 1;
            }
        }
    }

    return (dp[k] == INF) ? -1 : dp[k];
}

// The problem is the classic unbounded knapsack (coin change) problem. Since the number of coin types `n` and target `k` can both be up to (say) 10,000, we use dynamic programming (DP). Let `dp[x]` be the minimum number of coins to make amount `x` (for `x` from 0 to `k`). Initialize `dp[0] = 0`, and all other `dp[x] = INF` (a large number like 1e9). For each coin value `c` and for each amount `x` from `c` to `k`, we update `dp[x] = min(dp[x], dp[x-c] + 1)`. The nested loops can be coin-outer or amount-outer; both work for unbounded knapsack. Important edge cases: if `k` == 0, return 0; if no combination possible (though guaranteed not to happen because coin[0]=1), return -1. Time complexity: O(n*k), space complexity: O(k) (or O(k+1) for the DP array). The greedy approach from the snippet is not universally optimal, so we must use DP.
