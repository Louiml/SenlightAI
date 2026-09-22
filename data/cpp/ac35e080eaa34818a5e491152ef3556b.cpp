Write a C++ function `knapsackMaxValue` that takes four parameters: a vector of item weights, a vector of item values, the number of items (which will equal the size of the vectors), and a knapsack capacity (a positive integer). The function must return the maximum total value that can be obtained by selecting a subset of items such that the total weight does not exceed the capacity. Each item can be taken at most once (0/1 knapsack). The weights and values vectors will always be non-empty, the number of items is at least 1, all weights and capacities are non-negative integers, and the solution must handle cases where an item’s weight exceeds the capacity (in which case it cannot be taken). Implement an efficient dynamic programming solution with constant space optimization.
// The problem is the classic 0/1 knapsack. We use a 1D DP array `dp[w]` representing the maximum value achievable with capacity `w` using items processed so far. Initialize `dp` to all zeros. For each item with weight `wi` and value `vi`, we iterate capacities from `capacity` down to `wi` (to ensure we don’t reuse the same item multiple times in one iteration) and update `dp[w] = max(dp[w], dp[w - wi] + vi)`. This works because when we go right-to-left, `dp[w - wi]` still reflects the state before including the current item. The answer is `dp[capacity]`. Edge cases: an item heavier than the capacity will never be included; if capacity is 0, the answer is 0; if all weights are 0 (which is allowed per problem, though unusual), the algorithm still works because we can take all items with zero weight and collect all values. Time complexity is O(n * capacity) and space complexity O(capacity). The single-array optimization reduces space from O(n*capacity) to O(capacity) compared to tabulation.
#include <vector>
#include <algorithm>

// Returns the maximum total value for a 0/1 knapsack with given weights, values, and capacity.
int knapsackMaxValue(const std::vector<int>& weights, const std::vector<int>& values, int n, int capacity) {
    // dp[w] = max value achievable with capacity w using processed items
    std::vector<int> dp(capacity + 1, 0);

    for (int i = 0; i < n; ++i) {
        int w = weights[i];
        int v = values[i];
        // Traverse backwards to avoid using the same item more than once
        for (int currCap = capacity; currCap >= w; --currCap) {
            dp[currCap] = std::max(dp[currCap], dp[currCap - w] + v);
        }
    }
    return dp[capacity];
}
#include <cassert>
#include <vector>

// Forward declaration of the tested function (assumed to be defined above)
int knapsackMaxValue(const std::vector<int>& weights, const std::vector<int>& values, int n, int capacity);

int main() {
    // Basic test from the prompt
    std::vector<int> w1 = {1, 2, 3};
    std::vector<int> v1 = {10, 15, 40};
    assert(knapsackMaxValue(w1, v1, 3, 5) == 55); // items 2+3 (weight 2+3=5, value 15+40)

    // Item heavier than capacity
    std::vector<int> w2 = {4, 1, 2};
    std::vector<int> v2 = {100, 20, 30};
    assert(knapsackMaxValue(w2, v2, 3, 3) == 50); // only items 2 and 3 (weight 1+2=3, value 50)

    // Single item that fits exactly
    std::vector<int> w3 = {7};
    std::vector<int> v3 = {9};
    assert(knapsackMaxValue(w3, v3, 1, 7) == 9);

    // Single item that does not fit
    std::vector<int> w4 = {8};
    std::vector<int> v4 = {5};
    assert(knapsackMaxValue(w4, v4, 1, 7) == 0);

    // Capacity zero => nothing can be taken
    std::vector<int> w5 = {1, 2, 3};
    std::vector<int> v5 = {10, 20, 30};
    assert(knapsackMaxValue(w5, v5, 3, 0) == 0);

    // All weights zero => take all items for total value
    std::vector<int> w6 = {0, 0, 0};
    std::vector<int> v6 = {3, 5, 2};
    assert(knapsackMaxValue(w6, v6, 3, 10) == 10);

    // Larger test to confirm optimality
    std::vector<int> w7 = {2, 3, 4, 5};
    std::vector<int> v7 = {3, 4, 5, 6};
    assert(knapsackMaxValue(w7, v7, 4, 5) == 7); // items 2 and 3 (weight 2+3=5, value 3+4=7)

    // Duplicate weights and values
    std::vector<int> w8 = {2, 2, 2};
    std::vector<int> v8 = {10, 10, 10};
    assert(knapsackMaxValue(w8, v8, 3, 4) == 20); // any two items

    // All items exceed capacity
    std::vector<int> w9 = {6, 7, 8};
    std::vector<int> v9 = {1, 2, 3};
    assert(knapsackMaxValue(w9, v9, 3, 5) == 0);

    return 0;
}
