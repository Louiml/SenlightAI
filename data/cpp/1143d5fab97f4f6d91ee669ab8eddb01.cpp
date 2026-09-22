// You are given a knapsack-like problem where each item has a weight and a value, and you must choose a subset of items whose total weight does not exceed a given capacity `W`, maximizing the total value. Write a C++ function `int knapsackMaxValue(int W, const std::vector<int>& weights, const std::vector<int>& values)` that returns the maximum achievable value. The input vectors are parallel: `weights[i]` and `values[i]` belong to the same item. The number of items equals `weights.size()` (which must equal `values.size()`). All weights and values are non‑negative integers, and `W` is a non‑negative integer. If both vectors are empty, return 0. Handle cases where an item’s weight exceeds `W` gracefully (it is simply not considered). You may use the classic 0/1 knapsack dynamic programming approach. The function must be `const`‑correct (i.e., it should not modify the input vectors). The solution should be self‑contained, with necessary headers, and must not include a `main` function.

// The problem is the classic 0/1 knapsack: each item can be taken at most once. We solve it via dynamic programming using a 2D table `dp[i][w]` where `i` ranges from 0 to `n` (number of items) and `w` from 0 to `W`. `dp[i][w]` represents the maximum value achievable using the first `i` items (indices 0..i-1) with a total weight limit of `w`. The recurrence: for each item `i-1` (since table index offset), if `weights[i-1] <= w`, then `dp[i][w] = max(dp[i-1][w], values[i-1] + dp[i-1][w - weights[i-1]])`; otherwise, `dp[i][w] = dp[i-1][w]`. Base cases: when `i == 0` or `w == 0`, `dp[i][w] = 0`. The answer is `dp[n][W]`. Edge cases include: empty inputs (return 0), zero capacity (return 0), items with zero weight but positive value (they can always be taken), and items heavier than capacity (they are skipped). Time complexity is O(n·W), space complexity O(n·W). We can optimize space to O(W) using a 1D array, but for clarity and direct correspondence to the snippet, the 2D version is acceptable. The function does not modify inputs, so pass by `const&` for vectors.

#include <vector>
#include <algorithm>

// Returns the maximum total value that can be achieved without exceeding weight limit W.
// Each item (weights[i], values[i]) can be chosen at most once.
// The vectors are parallel and must have the same size.
int knapsackMaxValue(int W, const std::vector<int>& weights, const std::vector<int>& values) {
    int n = weights.size();
    // Handle empty inputs or zero capacity directly.
    if (n == 0 || W == 0) {
        return 0;
    }

    // dp[i][w] = max value using first i items (indices 0..i-1) with weight limit w.
    std::vector<std::vector<int>> dp(n + 1, std::vector<int>(W + 1, 0));

    for (int i = 1; i <= n; ++i) {
        int weight = weights[i - 1];
        int value = values[i - 1];
        for (int w = 1; w <= W; ++w) {
            if (weight <= w) {
                dp[i][w] = std::max(dp[i - 1][w], value + dp[i - 1][w - weight]);
            } else {
                dp[i][w] = dp[i - 1][w];
            }
        }
    }

    return dp[n][W];
}

#include <cassert>
#include <vector>

// (Assuming the solution function is defined above.)

int main() {
    // Test 1: Classic example from the snippet (capacity 15, 5 items).
    std::vector<int> weights1 = {12, 1, 4, 1, 2};
    std::vector<int> values1  = {4, 2, 10, 1, 2};
    assert(knapsackMaxValue(15, weights1, values1) == 15);

    // Test 2: Empty inputs.
    std::vector<int> w_empty;
    std::vector<int> v_empty;
    assert(knapsackMaxValue(10, w_empty, v_empty) == 0);

    // Test 3: Capacity zero always yields zero.
    std::vector<int> w3 = {2, 3};
    std::vector<int> v3 = {5, 7};
    assert(knapsackMaxValue(0, w3, v3) == 0);

    // Test 4: All items too heavy.
    std::vector<int> w4 = {10, 20};
    std::vector<int> v4 = {100, 200};
    assert(knapsackMaxValue(5, w4, v4) == 0);

    // Test 5: Can take all items when capacity is sufficient.
    std::vector<int> w5 = {1, 2, 3};
    std::vector<int> v5 = {6, 10, 12};
    assert(knapsackMaxValue(10, w5, v5) == 28);

    // Test 6: Zero‑weight item with positive value always included.
    std::vector<int> w6 = {0, 5};
    std::vector<int> v6 = {100, 3};
    assert(knapsackMaxValue(5, w6, v6) == 103);

    // Test 7: Single item that fits.
    std::vector<int> w7 = {4};
    std::vector<int> v7 = {9};
    assert(knapsackMaxValue(4, w7, v7) == 9);

    // Test 8: Single item that does not fit.
    assert(knapsackMaxValue(3, w7, v7) == 0);

    // Test 9: Duplicate weights/values, choose optimally.
    std::vector<int> w9 = {2, 2, 2};
    std::vector<int> v9 = {1, 1, 1};
    assert(knapsackMaxValue(3, w9, v9) == 1);

    // Test 10: Larger capacity can take two items with same weight.
    assert(knapsackMaxValue(4, w9, v9) == 2);

    return 0;
}
