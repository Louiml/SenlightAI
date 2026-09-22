// Write a C++ function `countWaysToMakeSum` that takes two integer parameters: `n` (number of coin denominations, between 1 and 100) and `x` (target sum, between 1 and 10^6), followed by a `std::vector<int>& coins` containing `n` positive integers (each between 1 and 10^6). The function must return the number of distinct ways to make exactly the sum `x` using an unlimited supply of each coin, where the order of coins does not matter (i.e., combinations, not permutations). The result must be returned modulo `1'000'000'007`. For example, with coins `[2,3]` and target `5`, there are 2 ways: `2+3` and `3+2` are considered the same combination, so only 1 way (since order doesn't matter, but the DP counts combinations when iterating coins outer loop). However, the given snippet uses an outer coin loop and inner sum loop, which counts combinations. Ensure your function handles large `x` efficiently. The function should be self-contained, with no global variables.
The problem is a classic unbounded knapsack combination count. We use dynamic programming where `dp[i]` represents the number of ways to make sum `i` using the coins processed so far. Initialize `dp[0] = 1` (one way to make sum 0). For each coin `c` in the given order, we iterate the sum from `c` to `x` and update `dp[i] = (dp[i] + dp[i - c]) % MOD`. This order (coin outer, sum inner) ensures that each combination is counted exactly once regardless of coin order, because we are considering adding the current coin to sums that already use only earlier coins, thus avoiding permutations. Important edge cases: coins with value greater than `x` are irrelevant (they won't contribute), and if `x` is 0, the answer is 1 (empty combination) per the problem statement (though `x` is at least 1 here). Also, if no combination is possible, `dp[x]` remains 0. Time complexity is O(n * x) because we iterate all coins and all sums. Space complexity is O(x) for the DP array. The modulo operation must be applied after each addition to avoid overflow (since `int` may hold up to ~2e9, sum of two mod values < 2e9 < 2^31, but repeated addition could overflow if not modded each time; here we mod after each addition, so safe).
#include <vector>

// Count number of combinations to make target sum using unlimited coins, modulo 1e9+7.
// Order of coins does not matter (combinations, not permutations).
int countWaysToMakeSum(int n, int x, const std::vector<int>& coins) {
    const int MOD = 1000000007;
    std::vector<int> dp(x + 1, 0);
    dp[0] = 1; // one way to make sum 0 (choose no coins)
    
    for (int j = 0; j < n; ++j) {
        int c = coins[j];
        // For sums >= c, we can add this coin to ways of sum (i - c)
        for (int i = c; i <= x; ++i) {
            dp[i] = (dp[i] + dp[i - c]) % MOD;
        }
    }
    return dp[x];
}
#include <cassert>
#include <vector>

// The solution function is declared above. For testing, include it here or via header.

int main() {
    // Test 1: Basic example from snippet - n=3? Actually snippet uses 1-index, but here we use 0-index.
    // coins [1,2,3] target 4 -> combinations: 1+1+1+1, 1+1+2, 1+3, 2+2 -> 4 ways.
    std::vector<int> coins1 = {1, 2, 3};
    assert(countWaysToMakeSum(3, 4, coins1) == 4);
    
    // Test 2: coins [2,3] target 5 -> only 2+3 -> 1 way (combinations, not permutations).
    std::vector<int> coins2 = {2, 3};
    assert(countWaysToMakeSum(2, 5, coins2) == 1);
    
    // Test 3: No combination possible.
    std::vector<int> coins3 = {2, 4};
    assert(countWaysToMakeSum(2, 5, coins3) == 0);
    
    // Test 4: Single coin, target multiple of coin.
    std::vector<int> coins4 = {3};
    assert(countWaysToMakeSum(1, 9, coins4) == 1);
    
    // Test 5: Large target and many coins, check modulo works.
    std::vector<int> coins5(100, 1); // 100 coins of value 1
    // Number of ways to make sum 100 using 1's is 1 (only all 1's), regardless of coin count.
    assert(countWaysToMakeSum(100, 100, coins5) == 1);
    
    // Test 6: Target zero? Not in spec but robust: one way (empty set).
    std::vector<int> coins6 = {1, 2};
    assert(countWaysToMakeSum(2, 0, coins6) == 1);
    
    // Test 7: Modulo large number - e.g., coins all 1, target 1000000 -> exactly 1 way (all ones).
    std::vector<int> coins7 = {1};
    assert(countWaysToMakeSum(1, 1000000, coins7) == 1);
    
    // Test 8: More complex, coins [1,2] target 3 -> ways: 1+1+1, 1+2 -> 2 ways.
    std::vector<int> coins8 = {1, 2};
    assert(countWaysToMakeSum(2, 3, coins8) == 2);
    
    return 0;
}
