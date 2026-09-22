Write a C++ function `int minCoinsToReachTarget(int target, const std::vector<std::pair<int,int>>& items)` that takes a target amount `target` (positive integer, `<= 20000`) and a list of `items` where each pair represents `(denomination, quantity)` available. Each item can be used at most `quantity` times, and you must reach exactly `target` using whole denominations. The function should return the minimum total number of coins/items used to reach exactly `target`. If it's impossible, return `-1`. Denominations and quantities are positive integers, and the total number of items is at most 50. Note that unlike the original snippet, do not sort the items; the order of input is irrelevant.

This is a bounded knapsack problem where each item type has a weight equal to its denomination and a cost of 1 per unit used (we minimize the count). We need exactly the target sum. A standard approach is dynamic programming with a 1D array `dp[0..target]` initialized to `INF` (a large sentinel) with `dp[0]=0`. For each item type, we apply the binary splitting technique to convert the bounded quantity into several 0/1 items: for quantity `q`, we decompose into pieces of sizes 1,2,4,... up to the remaining remainder. Each piece has weight `piece_size * denomination` and cost `piece_size`. Then for each such piece, we perform a 0/1 update backward from `target` down to the piece weight. After processing all items, if `dp[target]` is still `INF`, return `-1`; else return that value. Edge cases: target 0 (should return 0), denominations larger than target (skip), quantities that exceed the needed amount (the binary splitting handles this naturally). Time complexity: O(target * sum of log(quantity_i)) which for constraints is at most about `20000 * 50 * 16` ≈ 16 million operations, acceptable. Space: O(target).

#include <vector>
#include <algorithm>
#include <climits>

// Returns minimum number of items to reach exactly target, or -1 if impossible.
int minCoinsToReachTarget(int target, const std::vector<std::pair<int,int>>& items) {
    const int INF = 0x3f3f3f3f;
    std::vector<int> dp(target + 1, INF);
    dp[0] = 0;

    for (const auto& [denom, quantity] : items) {
        if (denom <= 0 || quantity <= 0) continue;
        int k = 1;
        int remaining = quantity;
        while (k < remaining) {
            int weight = k * denom;
            int cost = k;
            if (weight <= target) {
                for (int v = target; v >= weight; --v) {
                    if (dp[v - weight] != INF) {
                        dp[v] = std::min(dp[v], dp[v - weight] + cost);
                    }
                }
            }
            remaining -= k;
            k <<= 1;
        }
        // last piece
        int weight = remaining * denom;
        int cost = remaining;
        if (weight <= target) {
            for (int v = target; v >= weight; --v) {
                if (dp[v - weight] != INF) {
                    dp[v] = std::min(dp[v], dp[v - weight] + cost);
                }
            }
        }
    }

    return (dp[target] == INF) ? -1 : dp[target];
}

#include <cassert>
#include <vector>

// The solution function is declared above. Provide main with asserts.
int main() {
    // Basic exact match
    assert(minCoinsToReachTarget(10, {{10,1}}) == 1);

    // Multiple coins of same denomination
    assert(minCoinsToReachTarget(10, {{5,2}}) == 2);

    // Combination of denominations
    assert(minCoinsToReachTarget(15, {{10,1},{5,1}}) == 2);

    // Impossible when too little total value
    assert(minCoinsToReachTarget(100, {{10,1},{20,1}}) == -1);

    // Large quantity, need minimum count
    assert(minCoinsToReachTarget(12, {{6,2},{4,3}}) == 2); // 6+6

    // Over-supply but not enough of smaller bills to reach exactly
    assert(minCoinsToReachTarget(3, {{2,100}}) == -1);

    // Target 0 always possible with zero coins
    assert(minCoinsToReachTarget(0, {{1,1}}) == 0);

    // Mixed denominations and quantities
    assert(minCoinsToReachTarget(17, {{7,2},{3,5}}) == 3); // 7+7+3

    // Exact with many coins
    assert(minCoinsToReachTarget(100, {{1,100}}) == 100);

    // Ensure no sorting effect
    assert(minCoinsToReachTarget(20, {{15,2},{5,1}}) == 2); // 15+5

    return 0;
}
