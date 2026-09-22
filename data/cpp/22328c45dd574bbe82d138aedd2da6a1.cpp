// Write a C++ function that solves the 0/1 knapsack problem: given `n` items each with a value and a weight, and a knapsack capacity `W`, return the maximum total value that can be placed into the knapsack without exceeding its capacity. Each item can be selected at most once (not divisible). The function should accept arrays (or vectors) of values and weights, the number of items, and the capacity. If there are zero items or the capacity is zero, the result should be 0. Items may have weights greater than the capacity (cannot be taken) and values that are non-negative integers. The function must handle `n` up to 1000 and `W` up to 10000 efficiently within typical time limits.
// The solution uses dynamic programming with a 2D table `dp[i][j]` where `i` indexes items from 0 to n-1 and `j` indexes capacities from 0 to W. The entry `dp[i][j]` stores the maximum value achievable using the first `i+1` items (indices 0..i) with a knapsack capacity exactly `j`. Initialize the first row: for capacity `j`, if `j >= wt[0]` then `dp[0][j] = va[0]`, else 0. For each subsequent item `i`, for each capacity `j`, we either exclude the item (`dp[i-1][j]`) or include it if `j >= wt[i]` (`dp[i-1][j-wt[i]] + va[i]`), taking the maximum. The answer is `dp[n-1][W]`. Edge cases: if `n==0` or `W==0` return 0; if an item's weight exceeds the current capacity, it cannot be included; duplicate items or equal weights are handled naturally. Time complexity is O(n*W) and space complexity is O(n*W) (can be optimized to O(W) but the task does not require it). Since constraints are modest (n ≤ 1000, W ≤ 10000), the 2D table is feasible.
#include <vector>
#include <algorithm>

// Solve 0/1 knapsack: return maximum value for given values, weights, and capacity.
long long knapsackMaxValue(const std::vector<long long>& values,
                           const std::vector<long long>& weights,
                           long long capacity) {
    long long n = static_cast<long long>(values.size());
    if (n == 0 || capacity <= 0) return 0;

    std::vector<std::vector<long long>> dp(n, std::vector<long long>(capacity + 1, 0));

    // Initialize first row
    for (long long j = 0; j <= capacity; ++j) {
        if (j >= weights[0]) {
            dp[0][j] = values[0];
        }
    }

    // Fill DP table
    for (long long i = 1; i < n; ++i) {
        for (long long j = 1; j <= capacity; ++j) {
            dp[i][j] = dp[i - 1][j]; // exclude item i
            if (j >= weights[i]) {
                dp[i][j] = std::max(dp[i][j], dp[i - 1][j - weights[i]] + values[i]);
            }
        }
    }

    return dp[n - 1][capacity];
}
#include <cassert>
#include <vector>

// Declaration of the solution function
long long knapsackMaxValue(const std::vector<long long>& values,
                           const std::vector<long long>& weights,
                           long long capacity);

int main() {
    // Basic case
    assert(knapsackMaxValue({60, 100, 120}, {10, 20, 30}, 50) == 220);
    // Empty items
    assert(knapsackMaxValue({}, {}, 10) == 0);
    // Zero capacity
    assert(knapsackMaxValue({5, 6}, {2, 3}, 0) == 0);
    // Single item fits
    assert(knapsackMaxValue({10}, {5}, 5) == 10);
    // Single item does not fit
    assert(knapsackMaxValue({10}, {6}, 5) == 0);
    // Multiple items, capacity less than all weights
    assert(knapsackMaxValue({10, 20}, {5, 6}, 4) == 0);
    // Duplicate weights and values
    assert(knapsackMaxValue({5, 5, 5}, {2, 2, 2}, 4) == 10);
    // Large capacity, only some items selected
    assert(knapsackMaxValue({100, 50, 60}, {10, 5, 8}, 13) == 110); // 50+60=110, 100+50=150 but capacity 13? 10+5=15>13 so 100 alone or 50+60=110
    // All items fit
    assert(knapsackMaxValue({1, 2, 3}, {1, 1, 1}, 3) == 6);
    // Items with zero weight
    assert(knapsackMaxValue({5, 7}, {0, 0}, 10) == 12);
    // Large test for performance (optional, but ensure correct)
    std::vector<long long> large_values(1000, 1);
    std::vector<long long> large_weights(1000, 1);
    assert(knapsackMaxValue(large_values, large_weights, 10000) == 1000);
    
    return 0;
}
