/*
Write a C++ function `countWays` that takes a positive integer `n` representing the number of available coin denominations, a vector of distinct positive integers `coins` of length `n`, and a target sum `x` (non-negative integer), and returns the number of ways to form the sum `x` using any number of coins from the given set, where the order of coins matters (i.e., sequences are distinct). Since the answer can be large, return the result modulo \(10^9+7\). The function must handle edge cases such as `x = 0` (return 1, as the empty sequence is the only way), and coins that may be larger than `x`. For example, with coins `{2,3}` and target `5`, the valid sequences are `[2,3]` and `[3,2]`, so the answer is 2. The input will be valid: `n >= 1`, coins are distinct positive integers, and `x >= 0`. Use dynamic programming with an iterative bottom-up approach, where `dp[i]` stores the number of ways to sum exactly `i`, and compute `dp` from 0 to `x` inclusive.
*/
#include <vector>
#include <cstdint>

// Count the number of ordered sequences to sum to x using given coins.
// Returns the count modulo 1'000'000'007.
long long countWays(const std::vector<int>& coins, int n, int x) {
    const long long MOD = 1000000007LL;
    // dp[i] = number of ways to sum to exactly i
    std::vector<long long> dp(x + 1, 0);
    dp[0] = 1; // Only one way: the empty sequence

    for (int i = 0; i <= x; ++i) {
        for (int j = 0; j < n; ++j) {
            if (i - coins[j] >= 0) {
                dp[i] = (dp[i] + dp[i - coins[j]]) % MOD;
            }
        }
    }
    return dp[x];
}
#include <cassert>
#include <vector>

// Assume countWays is declared above.

int main() {
    // Basic examples
    assert(countWays({2, 3}, 2, 5) == 2); // [2,3], [3,2]
    assert(countWays({1, 2}, 2, 3) == 3); // [1,1,1], [1,2], [2,1]
    assert(countWays({1}, 1, 5) == 1);    // all ones

    // Edge cases
    assert(countWays({5}, 1, 0) == 1);    // empty sequence
    assert(countWays({2, 4}, 2, 1) == 0); // impossible
    assert(countWays({3}, 1, 6) == 1);    // [3,3]

    // Larger target with multiple coins
    assert(countWays({1, 2, 5}, 3, 5) == 9); // sequences summing to 5

    // Modulo behavior with large values (avoid overflow in test)
    // Example: coins {1}, target 1000 -> only 1 way
    assert(countWays({1}, 1, 1000) == 1);

    // Coins larger than target are ignored
    assert(countWays({10, 20}, 2, 5) == 0);

    // Single coin equal to target
    assert(countWays({7}, 1, 7) == 1);

    // Two coins where each can appear multiple times
    assert(countWays({1, 3}, 2, 4) == 3); // [1,1,1,1], [1,3], [3,1]

    // Test modulo with a large number to ensure no overflow
    // For coins {1,2}, target 100, answer is Fibonacci(101) mod 1e9+7,
    // but we just check it's non-negative and less than MOD (implicitly)
    long long result = countWays({1, 2}, 2, 100);
    assert(result >= 0 && result < 1000000007LL);

    return 0;
}
// The problem is the classic "Coin Change (Order Matters)" problem, also known as "Combination Sum IV". The main approach uses a 1D dynamic programming array `dp` of size `x+1`, initialized with `dp[0] = 1` (since there is exactly one way to make sum 0: the empty sequence). For every target `i` from 0 to `x`, we iterate over all coin denominations. If `i - coin >= 0`, then any way to form `i - coin` can be extended by appending this coin at the end, contributing the same number of ways. We accumulate these contributions modulo \(10^9+7\). This works because the order of iterations (outer loop over sums, inner loop over coins) ensures that sequences are counted with order, as we are effectively constructing sequences by considering each possible last coin. Edge cases: if `x = 0`, the loop runs and `dp[0]` remains 1, which is correct. If a coin is larger than `x`, the condition `i - coin >= 0` is never satisfied for that coin, so it is ignored. Time complexity is \(O(x \cdot n)\), where `n` is the number of coins. Space complexity is \(O(x)\) for the DP array. The solution is efficient for reasonable constraints (e.g., `x` up to \(10^5\) and `n` up to \(100\)).
