// Write a C++ function `int coinChangeCombinations(int amount, const std::vector<int>& coins)` that returns the number of distinct combinations of coins (unlimited supply of each coin type) that sum to exactly `amount`. The order of coins in a combination does not matter (e.g., `[1,2]` and `[2,1]` are the same combination). The function must handle `amount == 0` by returning `1` (the empty combination), `amount < 0` by returning `0`, and any case where the coins list is empty (with non-zero amount) by returning `0`. You are expected to use memoized recursion (top-down dynamic programming) where the state depends only on the remaining amount and the index of the coin type currently being considered to avoid duplicate combinations.
// The core problem is a classic "coin change combinations" (also known as "ways to make change") where unbounded coins are allowed and order does not matter. The provided snippet uses a recursive function with memoization over `(amount, index)`. At each state, we iterate over all coin types starting from the current index (not from 0), which ensures we only generate each combination once (by forcing a non-decreasing order of coin indices). If `amount` becomes exactly 0, we return 1. If it becomes negative, we return 0. The memoization stores results in a 2D table of size `(amount+1) x (coins.size())`. Edge cases: (1) `amount == 0` returns 1 regardless of coins (empty combination). (2) If `coins` is empty and `amount > 0`, the recursion naturally returns 0 (no loop iterations). (3) If `amount` is negative, returns 0. The time complexity is O(amount * coins.size()) states, each doing up to coins.size() work, so O(amount * n^2) in the worst case (where n is the number of coin types). However, with careful indexing, we can reduce the per-state work to O(n) as in the snippet, leading to O(amount * n^2) overall. Space complexity is O(amount * n) for the memo table plus recursion stack depth O(amount) in the worst case.
#include <vector>
#include <cstring>

// Returns the number of distinct combinations of coins (unlimited supply) that sum to exactly 'amount'.
// Combination order does not matter. Memoized recursive solution.
int coinChangeCombinations(int amount, const std::vector<int>& coins) {
    int n = static_cast<int>(coins.size());
    if (amount == 0) return 1;
    if (amount < 0 || n == 0) return 0;

    // dp[rem][idx] = number of ways to make 'rem' using coins from index idx to end (inclusive).
    std::vector<std::vector<int>> dp(amount + 1, std::vector<int>(n, -1));

    // Recursive helper with memoization.
    // We force non-decreasing order of coin indices to avoid permutations.
    std::function<int(int, int)> solve = [&](int rem, int idx) -> int {
        if (rem == 0) return 1;
        if (rem < 0 || idx >= n) return 0;
        if (dp[rem][idx] != -1) return dp[rem][idx];

        int ways = 0;
        // Try using each coin type starting from 'idx' (so we don't go back to smaller indices).
        for (int i = idx; i < n; ++i) {
            ways += solve(rem - coins[i], i);
        }
        dp[rem][idx] = ways;
        return ways;
    };

    return solve(amount, 0);
}
#include <cassert>
#include <vector>
#include "solution.h" // assuming the solution is in this header

int main() {
    // Basic tests
    assert(coinChangeCombinations(5, {1,2,5}) == 4); // {1,1,1,1,1}, {1,1,1,2}, {1,2,2}, {5}
    assert(coinChangeCombinations(3, {2}) == 0);     // cannot make 3 with only 2s
    assert(coinChangeCombinations(0, {1,2,3}) == 1); // empty combination
    assert(coinChangeCombinations(10, {}) == 0);     // no coins

    // Larger test with repeated use of a single coin
    assert(coinChangeCombinations(4, {1,2}) == 3);   // {1,1,1,1}, {1,1,2}, {2,2}

    // Test with coins where order matters (should still be combinations)
    assert(coinChangeCombinations(4, {3,1}) == 2);   // {1,1,1,1}, {3,1} (1+3 is same as 3+1)

    // Test that negative amount returns 0
    assert(coinChangeCombinations(-5, {1,2}) == 0);

    // Stress test with a moderate amount
    assert(coinChangeCombinations(10, {2,5,3}) == 3); // {2,2,2,2,2}, {5,5}, {5,3,2} (and {3,3,2,2}? wait: 3+3+2+2=10, yes so 4? Actually let's compute: combos: 5+5, 5+3+2, 3+3+2+2, 2+2+2+2+2 -> 4)
    // Corrected: 5+5, 5+3+2, 3+3+2+2, 2+2+2+2+2 => 4
    assert(coinChangeCombinations(10, {2,5,3}) == 4);
    return 0;
}
