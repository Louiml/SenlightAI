Write a C++ function `int coinChangeCombinations(int amount, std::vector<int> coins)` that returns the number of distinct combinations of coins (unlimited supply of each coin) that sum exactly to the given `amount`. The order of coins in a combination does not matter (e.g., {1,2} and {2,1} are the same combination). Coins may contain duplicates and positive integers only. If no combination exists, return 0. Use a memorized dynamic programming approach. The function must handle an empty `coins` vector and `amount == 0` correctly (return 1 for amount 0, since the empty combination is valid).

The problem is a classic unbounded knapsack counting problem. We sort the coins in ascending order to allow early termination in the first recursive version, but the reference solution below uses the second, more standard formulation. Define `dp[i][a]` = number of ways to form amount `a` using the first `i+1` coin types (indices 0..i). Base cases: if `a == 0`, return 1 (empty combination). If `a < 0`, return 0 (invalid). If `i == 0`, then only the first coin type is available; return 1 if `a % coins[0] == 0`, else 0. For general `i`, the recurrence is: `dp[i][a] = dp[i-1][a]` (not using the i-th coin) `+ dp[i][a - coins[i]]` (using the i-th coin at least once, allowing unlimited use). This correctly avoids permutations because we only move forward in coin indices. We initially fill the `dp` table with -1 for memorization. Edge cases: empty coins and amount > 0 returns 0 (since the base case i==0 never triggers, but the recursion will eventually hit i<0? Actually we must clamp: if i<0, return 0 except when amount==0). In the code, we handle i==0 as the base and if coins is empty, we handle early by returning (amount==0 ? 1 : 0). Time complexity is O(n * amount) because each state (i, a) is computed once, and each state does O(1) work. Space complexity is O(n * amount) for the dp table.

#include <vector>
#include <algorithm>

// Returns the number of distinct combinations of coins (unlimited supply) that sum to amount.
// Order does not matter. Assumes all coin values are positive integers.
int coinChangeCombinations(int amount, std::vector<int> coins) {
    if (coins.empty()) {
        return amount == 0 ? 1 : 0;
    }
    // Remove duplicates to avoid double counting identical coin values, though not strictly necessary.
    std::sort(coins.begin(), coins.end());
    coins.erase(std::unique(coins.begin(), coins.end()), coins.end());
    const int n = static_cast<int>(coins.size());
    // dp[i][a] = number of ways using coins[0..i] to sum to a, -1 means unknown.
    std::vector<std::vector<int>> dp(n, std::vector<int>(amount + 1, -1));

    // Helper lambda for recursion with memorization.
    std::function<int(int, int)> solve = [&](int i, int a) -> int {
        if (a == 0) return 1;
        if (a < 0) return 0;
        if (i < 0) return 0; // no coins left
        if (dp[i][a] != -1) return dp[i][a];
        // Not using coin i, and using coin i (allowing multiple uses).
        int not_taken = solve(i - 1, a);
        int taken = solve(i, a - coins[i]);
        return dp[i][a] = taken + not_taken;
    };

    return solve(n - 1, amount);
}

#include <cassert>
#include <vector>

// The solution function is declared above. Include it here for the test.

int main() {
    // Basic examples
    assert(coinChangeCombinations(5, {1, 2, 3}) == 5); // {1,1,1,1,1}, {1,1,1,2}, {1,2,2}, {1,1,3}, {2,3}
    assert(coinChangeCombinations(4, {1, 2}) == 3);    // {1,1,1,1}, {1,1,2}, {2,2}
    assert(coinChangeCombinations(3, {2}) == 0);       // Cannot make 3
    assert(coinChangeCombinations(0, {2}) == 1);       // Empty combination
    assert(coinChangeCombinations(0, {}) == 1);        // Empty coins, amount 0
    assert(coinChangeCombinations(5, {}) == 0);        // Empty coins, amount > 0
    // Duplicate coin values should not affect count
    assert(coinChangeCombinations(4, {1, 1, 2}) == 3); // Same as {1,2}
    // Larger amount with a single coin
    assert(coinChangeCombinations(10, {5}) == 1);
    assert(coinChangeCombinations(11, {5}) == 0);
    // Standard LeetCode example
    assert(coinChangeCombinations(5, {1, 2, 5}) == 4); // {1,1,1,1,1}, {1,1,1,2}, {1,2,2}, {5}
    return 0;
}
