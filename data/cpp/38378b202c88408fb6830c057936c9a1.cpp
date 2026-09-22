Write a C++ function that solves the 0/1 knapsack problem using dynamic programming with memoization. The function takes a knapsack capacity `W` (an integer ≥ 0), a vector of weights `wt`, and a vector of values `val` (both non-empty, same size, with weights ≥ 0 and values ≥ 0). It must return the maximum total value achievable by selecting a subset of items such that the sum of their weights does not exceed `W`. Each item can be taken at most once (0/1 property). The solution should handle cases where the capacity is zero, where an item’s weight exceeds the capacity (in which case it is skipped), and where all items can be taken if capacity allows. Use a recursive helper with memoization on the index and remaining capacity. The function signature should be `int knapsack01(int capacity, const std::vector<int>& weights, const std::vector<int>& values)`.

#include <cassert>
#include <vector>

// Function declaration (assumed from solution) for testing.
int knapsack01(int capacity, const std::vector<int>& weights, const std::vector<int>& values);

int main() {
    // Basic test
    std::vector<int> w1 = {1, 3, 4, 5};
    std::vector<int> v1 = {1, 4, 5, 7};
    assert(knapsack01(7, w1, v1) == 9);  // Items 2 and 3? Actually 4+5=9 (items with weights 3 and 4)

    // Single item fits
    assert(knapsack01(3, {2}, {10}) == 10);
    // Single item does not fit
    assert(knapsack01(1, {2}, {10}) == 0);
    // Capacity zero
    assert(knapsack01(0, {1,2}, {5,6}) == 0);
    // All items fit
    assert(knapsack01(10, {2,3,4}, {5,6,7}) == 18);
    // Duplicate weights and values, larger capacity
    std::vector<int> w2 = {2, 2, 3, 3};
    std::vector<int> v2 = {3, 3, 4, 4};
    assert(knapsack01(6, w2, v2) == 8);  // Take both weight-3 items (value 4+4=8)
    // Empty weights (though spec says non-empty, but robust)
    assert(knapsack01(5, {}, {}) == 0);
    // Large capacity, all items fit
    assert(knapsack01(100, {10,20}, {100,200}) == 300);
    return 0;
}

#include <vector>
#include <algorithm>

// Helper function for memoized 0/1 knapsack.
// Returns max value from items starting at index i with remaining capacity w.
int knapsackHelper(int i, int w, const std::vector<int>& weights, const std::vector<int>& values, std::vector<std::vector<int>>& memo) {
    // Base case: no more items or no capacity left.
    if (i >= static_cast<int>(weights.size()) || w == 0) {
        return 0;
    }
    // Return cached result if already computed.
    if (memo[i][w] != -1) {
        return memo[i][w];
    }
    // Option 1: exclude current item.
    int exclude = knapsackHelper(i + 1, w, weights, values, memo);
    // Option 2: include current item if it fits.
    int include = 0;
    if (weights[i] <= w) {
        include = knapsackHelper(i + 1, w - weights[i], weights, values, memo) + values[i];
    }
    // Store and return the maximum.
    return memo[i][w] = std::max(exclude, include);
}

// Main function: returns max value for 0/1 knapsack.
int knapsack01(int capacity, const std::vector<int>& weights, const std::vector<int>& values) {
    int n = static_cast<int>(weights.size());
    // Memoization table: n+1 rows (some extra), capacity+1 columns, initialized to -1.
    std::vector<std::vector<int>> memo(n + 1, std::vector<int>(capacity + 1, -1));
    return knapsackHelper(0, capacity, weights, values, memo);
}

// The problem is a classic 0/1 knapsack. We define a recursive function `solve(i, w)` that returns the maximum value obtainable from items `i` to `n-1` with remaining capacity `w`. The base case is when `i >= n` (no more items) or `w == 0` (no capacity), returning 0. For each item `i`, we compute two options: exclude it (`solve(i+1, w)`) or include it if `weights[i] <= w` (value = `solve(i+1, w - weights[i]) + values[i]`). The result is the maximum of these two. To avoid recomputation, we use a 2D memoization table `memo[i][w]` initialized to `-1` (since values are non-negative, -1 denotes "not computed"). The table has dimensions `(n+1) x (W+1)`, where `n` is the number of items. Time complexity is O(n * W) because each state `(i, w)` is computed at most once. Space complexity is O(n * W) for the memoization table plus O(n) recursion stack depth (worst-case). Edge cases: empty capacity yields 0; an item heavier than current capacity is simply skipped; if all items fit, the answer is the sum of all values. The solution uses a helper function with `const` references for the input vectors to avoid copying.
