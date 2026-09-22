// Write a C++ function `long long knapsackMaxValue(int n, int capacity, const std::vector<int>& weights, const std::vector<int>& values)` that solves the classic 0/1 knapsack problem using **memoized recursion (top-down dynamic programming)**. The function must return the maximum total value obtainable by selecting a subset of items such that the sum of their weights does not exceed the given capacity. Each item can be chosen at most once. The input will always have valid sizes (n ≥ 0), and weights and values arrays will have length exactly n. Return 0 if no items can be chosen. The capacity and weights are non-negative integers, and values are positive integers. You must not modify the input vectors.

#include <cassert>
#include <vector>

// The solution function is declared above; include it here in a full program.
// (For brevity, the function definition is assumed present.)

int main() {
    // Basic case
    {
        std::vector<int> w = {2, 3, 4, 5};
        std::vector<int> v = {3, 4, 5, 6};
        assert(knapsackMaxValue(4, 5, w, v) == 7); // items 0+1 (weight 5) value 7
    }
    // Zero items
    {
        std::vector<int> w = {};
        std::vector<int> v = {};
        assert(knapsackMaxValue(0, 10, w, v) == 0);
    }
    // Zero capacity but zero-weight items
    {
        std::vector<int> w = {0, 0, 0};
        std::vector<int> v = {5, 7, 3};
        assert(knapsackMaxValue(3, 0, w, v) == 15);
    }
    // Cannot take any item due to weight > capacity
    {
        std::vector<int> w = {3, 5};
        std::vector<int> v = {10, 20};
        assert(knapsackMaxValue(2, 2, w, v) == 0);
    }
    // Large capacity, all items fit
    {
        std::vector<int> w = {1, 2, 3};
        std::vector<int> v = {10, 20, 30};
        assert(knapsackMaxValue(3, 10, w, v) == 60);
    }
    // Capacity exactly equal to one item's weight
    {
        std::vector<int> w = {3, 4};
        std::vector<int> v = {8, 9};
        assert(knapsackMaxValue(2, 4, w, v) == 9);
    }
    // Multiple optimal solutions, choose max value
    {
        std::vector<int> w = {2, 2, 2};
        std::vector<int> v = {1, 2, 3};
        assert(knapsackMaxValue(3, 4, w, v) == 5); // take items 1 and 2 (value 5)
    }
    // Large numbers, ensure long long return
    {
        std::vector<int> w = {1000, 2000};
        std::vector<int> v = {1000000, 2000000};
        assert(knapsackMaxValue(2, 3000, w, v) == 3000000LL);
    }
    return 0;
}

#include <vector>
#include <cstring>

// Solve 0/1 knapsack via memoized recursion.
// Returns the maximum total value without exceeding capacity.
long long knapsackMaxValue(int n, int capacity,
                           const std::vector<int>& weights,
                           const std::vector<int>& values) {
    // dp[i][cw] = max value from items i..n-1 with current weight cw.
    // Use -1 as uncomputed marker.
    std::vector<std::vector<long long>> dp(n + 1,
        std::vector<long long>(capacity + 1, -1));

    // Recursive lambda with memoization.
    std::function<long long(int, int)> solve = [&](int i, int cw) -> long long {
        if (i == n) return 0;
        if (dp[i][cw] != -1) return dp[i][cw];

        long long skip = solve(i + 1, cw);
        long long take = 0;
        if (cw + weights[i] <= capacity) {
            take = values[i] + solve(i + 1, cw + weights[i]);
        }
        return dp[i][cw] = std::max(skip, take);
    };

    return solve(0, 0);
}

// The problem is a standard 0/1 knapsack. We use a recursive function `solve(i, currentWeight)` that considers items from index `i` to `n-1`, given the current accumulated weight `currentWeight`. At each step, we have two choices: skip the current item (resulting in `solve(i+1, currentWeight)`) or take it (if adding its weight doesn't exceed capacity, resulting in `values[i] + solve(i+1, currentWeight + weights[i])`). We take the maximum of these two. To avoid recomputation, we memoize results in a 2D table `dp[i][currentWeight]` initialized to -1 (meaning uncomputed). The base case is when `i == n` (no more items), return 0. Important edge cases: (1) If no items exist (n==0), simply return 0. (2) If capacity is 0 but items have zero weight, we can take them for free; our recursive logic handles this correctly because `currentWeight + weights[i]` might equal `capacity`. (3) The number of states is `(n+1) * (capacity+1)`. Each state does constant work, so time complexity is `O(n * capacity)`. Space complexity is also `O(n * capacity)` for the memo table, plus recursion stack depth `O(n)`. We must ensure the memo table is a 2D vector of `long long` initialized to -1.
