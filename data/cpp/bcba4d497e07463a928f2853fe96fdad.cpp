// Write a C++ function `minCoins` that takes a vector of positive integer coin denominations and a target amount, and returns the minimum number of coins needed to make that amount exactly. If the amount cannot be made with any combination of the given coins, return -1. The function should handle any number of coins (including a single denomination), a target amount of 0 (return 0), and the possibility of using the same coin denomination multiple times (unlimited supply). Do not use any standard library containers other than `std::vector` for the input; the function should be recursive with memoization (top-down dynamic programming). The function signature must be `int minCoins(const std::vector<int>& coins, int amount)`, and the solution must be const-correct, with the input vector passed by const reference.
#include <cassert>
#include <vector>

int main() {
    // Basic cases
    assert(minCoins({1, 2, 5}, 11) == 3);      // 5+5+1
    assert(minCoins({2}, 3) == -1);            // impossible
    assert(minCoins({1}, 0) == 0);             // zero amount
    assert(minCoins({1, 2, 5}, 0) == 0);       // zero amount with coins
    assert(minCoins({2, 5, 10, 1}, 27) == 4);  // 10+10+5+2
    // Edge: single coin that works
    assert(minCoins({3}, 9) == 3);
    // Edge: coin larger than amount
    assert(minCoins({10}, 7) == -1);
    // Edge: duplicate denominations (should behave normally)
    assert(minCoins({2, 2, 5}, 9) == 3);       // 5+2+2
    // Edge: many coins
    assert(minCoins({1, 3, 4}, 6) == 2);       // 3+3
    assert(minCoins({1, 3, 4}, 7) == 2);       // 3+4
    // Large amount to ensure recursion works
    assert(minCoins({1, 5, 10, 25}, 100) == 4); // 25*4
}
#include <vector>
#include <algorithm>

// Returns the minimum number of coins (unlimited supply) to form 'amount',
// or -1 if impossible. Uses top-down DP with memoization.
int minCoins(const std::vector<int>& coins, int amount) {
    // Special cases
    if (amount == 0) return 0;
    if (coins.empty()) return -1;

    int n = coins.size();
    // dp[i][r] = -1 means not computed; large sentinel (1e9) for impossible.
    std::vector<std::vector<int>> dp(n, std::vector<int>(amount + 1, -1));

    // Recursive lambda or helper function; using a free function with references.
    // Define a nested lambda capturing dp, coins, amount by reference.
    // Since C++ lambdas can be recursive if we use std::function, but we'll
    // use a separate helper function defined inside minCoins to avoid std::function overhead.
    std::function<int(int, int)> f = [&](int ind, int rem) -> int {
        if (rem == 0) return 0; // no more coins needed
        if (ind == 0) {
            // Use only the first coin
            if (rem % coins[0] == 0) return rem / coins[0];
            else return 1e9; // impossible
        }
        if (dp[ind][rem] != -1) return dp[ind][rem];

        // Option 1: skip current coin
        int notPick = f(ind - 1, rem);
        // Option 2: take one copy of current coin if possible
        int pick = 1e9;
        if (rem >= coins[ind]) {
            pick = 1 + f(ind, rem - coins[ind]);
        }
        return dp[ind][rem] = std::min(pick, notPick);
    };

    int ans = f(n - 1, amount);
    return (ans >= 1e9) ? -1 : ans;
}
Note: The above solution uses `std::function` for recursion inside the function. To keep the solution self-contained and avoid `<functional>` inclusion, we could also implement with a helper function that takes dp by reference, but the above is acceptable. However, to strictly match the requirement of "free function" and avoid `std::function` (which adds overhead), a cleaner implementation would use a private helper. I'll provide a version without `std::function`:

#include <vector>
#include <algorithm>

namespace {
    // Helper function for recursive DP
    int solve(int ind, int rem, const std::vector<int>& coins, std::vector<std::vector<int>>& dp) {
        if (rem == 0) return 0;
        if (ind == 0) {
            if (rem % coins[0] == 0) return rem / coins[0];
            return 1e9; // sentinel for impossible
        }
        if (dp[ind][rem] != -1) return dp[ind][rem];
        int notPick = solve(ind - 1, rem, coins, dp);
        int pick = 1e9;
        if (rem >= coins[ind]) {
            pick = 1 + solve(ind, rem - coins[ind], coins, dp);
        }
        dp[ind][rem] = std::min(pick, notPick);
        return dp[ind][rem];
    }
}

// Returns the minimum number of coins (unlimited supply) to form 'amount',
// or -1 if impossible.
int minCoins(const std::vector<int>& coins, int amount) {
    if (amount == 0) return 0;
    if (coins.empty()) return -1;
    int n = coins.size();
    std::vector<std::vector<int>> dp(n, std::vector<int>(amount + 1, -1));
    int ans = solve(n - 1, amount, coins, dp);
    return (ans >= 1e9) ? -1 : ans;
}
// The problem is a classic unbounded knapsack variant (coin change with unlimited coins). The solution uses a recursive function `f(ind, rem)` where `ind` is the index of the coin denomination currently considered (processing from the last coin backward) and `rem` is the remaining amount to form. Base case: when `ind == 0`, if `rem` is divisible by `coins[0]`, we can form it using exactly `rem / coins[0]` copies of that coin; otherwise it's impossible, so return a large sentinel (e.g., `1e9`). For other indices, we have two choices: (1) not take the current coin, so recurse to `f(ind-1, rem)`; (2) take one copy of the current coin if `rem >= coins[ind]`, then add 1 and recurse to `f(ind, rem - coins[ind])` (note the same `ind` because we can reuse coins). The result is the minimum of these two options. Memoize results in a 2D vector `dp` of size `(coins.size()) x (amount+1)` initialized to -1. If the final answer is the sentinel, return -1. Edge cases: amount = 0 returns 0 immediately (no coins needed); if `coins` is empty, cannot form any positive amount, return -1; if any coin denomination is 0 or negative, but the problem states positive integers, so assume valid input. Time complexity: O(n * amount) where n = coins.size(), because each state `(ind, rem)` is computed once. Space complexity: O(n * amount) for the memoization table plus O(n) recursion stack depth (which is actually bounded by `ind` in the not-pick branch, but recursion depth is at most n + amount in the pick branch? Actually the recursion depth can be up to amount in the pick branch due to repeatedly picking the same coin, but that is handled by memoization; the call stack depth in the worst case is about O(n + amount). However, with memoization, the recursion depth is at most n (for the notPick chain) plus amount (for consecutive picks), but in practice the maximum depth is O(n + amount). But standard analysis: each state is computed once, and the maximum recursion depth is O(n + amount) because we can go down a chain of picks. Nevertheless, the dominant complexity is the DP table size.)
