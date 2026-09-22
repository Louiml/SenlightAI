Write a C++ function `maxValueInKnapsack` that takes three parameters: `count` (number of items), `capacity` (maximum knapsack weight), and two vectors `values` and `weights` of length `count` (where `values[i]` is the value of the i-th item and `weights[i]` is its weight). The function must return the maximum total value that can be achieved by selecting a subset of items such that the total weight does not exceed `capacity`. Items must be either fully included or excluded (0/1 knapsack). The function should handle up to 100 items and a capacity up to 10000, with weights and values being positive integers. The input vectors are 1-indexed for clarity; you may ignore index 0 or assume it is zero.

#include <cassert>
#include <vector>

int maxValueInKnapsack(int, int, const std::vector<int>&, const std::vector<int>&); // Declare

int main() {
    // Example from the snippet: n=1? Actually snippet uses first line t, then n, m, then d[], then w[].
    // Let's test with typical cases.
    
    // Case 1: Simple two items.
    std::vector<int> v1 = {0, 60, 100};
    std::vector<int> w1 = {0, 10, 20};
    assert(maxValueInKnapsack(2, 30, v1, w1) == 160);
    
    // Case 2: Only one item can fit.
    std::vector<int> v2 = {0, 10, 20, 30};
    std::vector<int> w2 = {0, 5, 10, 15};
    assert(maxValueInKnapsack(3, 10, v2, w2) == 20); // choose item 2 (value 20)
    
    // Case 3: No items fit (capacity too small).
    std::vector<int> v3 = {0, 100, 200};
    std::vector<int> w3 = {0, 50, 60};
    assert(maxValueInKnapsack(2, 10, v3, w3) == 0);
    
    // Case 4: Zero capacity.
    std::vector<int> v4 = {0, 1, 2};
    std::vector<int> w4 = {0, 3, 4};
    assert(maxValueInKnapsack(2, 0, v4, w4) == 0);
    
    // Case 5: Zero items.
    std::vector<int> v5 = {0};
    std::vector<int> w5 = {0};
    assert(maxValueInKnapsack(0, 10, v5, w5) == 0);
    
    // Case 6: Items with equal weight and value (multiple combos).
    std::vector<int> v6 = {0, 5, 5, 5};
    std::vector<int> w6 = {0, 2, 2, 2};
    assert(maxValueInKnapsack(3, 4, v6, w6) == 10);
    
    // Case 7: Larger test to ensure no overflow logic error.
    std::vector<int> v7 = {0, 10, 4, 9, 8, 2, 5, 6, 3, 1, 7};
    std::vector<int> w7 = {0, 3, 2, 5, 4, 1, 3, 2, 1, 2, 4};
    assert(maxValueInKnapsack(10, 10, v7, w7) == 32); // derived manually or by known result
    
    // Case 8: Exact fit.
    std::vector<int> v8 = {0, 100, 200, 300};
    std::vector<int> w8 = {0, 1, 2, 3};
    assert(maxValueInKnapsack(3, 6, v8, w8) == 600); // all three
    
    // Case 9: Duplicate weights but different values.
    std::vector<int> v9 = {0, 10, 20, 30};
    std::vector<int> w9 = {0, 5, 5, 5};
    assert(maxValueInKnapsack(3, 5, v9, w9) == 30); // choose the best value
    
    // Case 10: Capacity larger than total weight – take all.
    std::vector<int> v10 = {0, 1, 2, 3};
    std::vector<int> w10 = {0, 1, 1, 1};
    assert(maxValueInKnapsack(3, 10, v10, w10) == 6);
    
    return 0;
}

#include <vector>
#include <algorithm>

// Solves the 0/1 knapsack problem.
// values[i] is the value of item i, weights[i] is its weight, both 1-indexed (index 0 unused).
// Returns the maximum total value attainable with total weight <= capacity.
int maxValueInKnapsack(int count, int capacity, const std::vector<int>& values, const std::vector<int>& weights) {
    // DP array where dp[w] = max value with total weight exactly w or less.
    std::vector<int> dp(capacity + 1, 0);

    for (int i = 1; i <= count; ++i) {
        int weight = weights[i];
        int value = values[i];
        // If the item is too heavy, skip it.
        if (weight > capacity) continue;
        // Traverse capacity backwards to avoid reusing the same item.
        for (int w = capacity; w >= weight; --w) {
            dp[w] = std::max(dp[w], dp[w - weight] + value);
        }
    }
    return dp[capacity];
}

// This is the classic 0/1 knapsack problem solved via dynamic programming. We maintain a DP array `dp[w]` representing the maximum value achievable with a total weight exactly `w` (or at most `w` when initialized to zero). For each item, we iterate the capacity from high to low, updating `dp[j] = max(dp[j], dp[j - weight] + value)` for all `j >= weight`. Iterating descending prevents reusing the same item multiple times (which would make it an unbounded knapsack). Edge cases: if capacity or count is 0, result is 0; if an item's weight exceeds capacity, it is simply ignored. Time complexity is O(count * capacity) and space complexity is O(capacity), which is optimal for this problem. The solution uses a vector of ints for DP, initialized to zero.
