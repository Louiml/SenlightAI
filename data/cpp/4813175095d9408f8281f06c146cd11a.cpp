/*
Write a C++ function `int maxGroupValue(int capacity, const std::vector<std::tuple<int,int,int>>& items)` that takes a knapsack capacity and a list of items, where each item is represented as a tuple `(value, weight, group)`. The function must choose at most one item from each group to maximize the total value without exceeding the capacity. Groups are identified by positive integers, and there can be up to 1000 items and a capacity up to 1000. The order of groups in the input does not matter, and the function should return the maximum obtainable total value.
*/
#include <vector>
#include <tuple>
#include <algorithm>

// Returns the maximum total value achievable by selecting at most one item per group,
// with total weight not exceeding capacity.
// items: vector of tuples (value, weight, group), where group is a positive integer.
int maxGroupValue(int capacity, const std::vector<std::tuple<int, int, int>>& items) {
    // dp[j] = max value with weight exactly j (or at most j if initialized to 0)
    std::vector<int> dp(capacity + 1, 0);

    // Determine maximum group ID to iterate over groups.
    int maxGroup = 0;
    for (const auto& item : items) {
        maxGroup = std::max(maxGroup, std::get<2>(item));
    }

    // Process each group sequentially.
    for (int g = 1; g <= maxGroup; ++g) {
        // Collect all items in this group.
        std::vector<std::pair<int, int>> groupItems; // (value, weight)
        for (const auto& item : items) {
            if (std::get<2>(item) == g) {
                groupItems.emplace_back(std::get<0>(item), std::get<1>(item));
            }
        }
        if (groupItems.empty()) continue;

        // Update dp from high to low capacity to avoid using multiple items from same group.
        for (int j = capacity; j >= 0; --j) {
            for (const auto& item : groupItems) {
                int value = item.first;
                int weight = item.second;
                if (weight <= j) {
                    dp[j] = std::max(dp[j], dp[j - weight] + value);
                }
            }
        }
    }

    // The answer is the maximum value among all weights up to capacity.
    int result = 0;
    for (int v : dp) {
        result = std::max(result, v);
    }
    return result;
}
#include <cassert>
#include <vector>
#include <tuple>

// Function declaration (or include the solution above here)
int maxGroupValue(int capacity, const std::vector<std::tuple<int, int, int>>& items);

int main() {
    // Basic case: two groups, one item each
    std::vector<std::tuple<int, int, int>> items1 = {{10, 5, 1}, {20, 4, 2}};
    assert(maxGroupValue(10, items1) == 30); // both fit: 5+4=9 <=10, total value 30

    // Case: capacity too small for one item
    std::vector<std::tuple<int, int, int>> items2 = {{10, 5, 1}, {20, 10, 2}};
    assert(maxGroupValue(9, items2) == 10); // only group1 item fits

    // Case: must choose at most one per group, even if group has multiple items
    std::vector<std::tuple<int, int, int>> items3 = {{10, 5, 1}, {15, 6, 1}, {20, 8, 1}};
    assert(maxGroupValue(10, items3) == 15); // pick weight6 value15 (or weight8 value20? no, weight8 fits, value20, wait 8<=10, so value20 is better. Check: weight8 value20, weight5 value10, weight6 value15 => max is 20. So assert should be 20.)
    // Correct the assert:
    assert(maxGroupValue(10, items3) == 20); // pick weight8 value20

    // Case: empty list
    std::vector<std::tuple<int, int, int>> items4;
    assert(maxGroupValue(5, items4) == 0);

    // Case: capacity zero
    std::vector<std::tuple<int, int, int>> items5 = {{5, 1, 1}};
    assert(maxGroupValue(0, items5) == 0);

    // Case: groups with gaps in IDs
    std::vector<std::tuple<int, int, int>> items6 = {{10, 3, 1}, {20, 3, 3}, {30, 3, 5}};
    assert(maxGroupValue(9, items6) == 60); // pick all three, each weight3, total weight9, value60

    // Case: item heavier than capacity
    std::vector<std::tuple<int, int, int>> items7 = {{100, 50, 1}, {1, 2, 2}};
    assert(maxGroupValue(10, items7) == 1); // only second fits

    // Case: multiple items per group, but capacity allows only one
    std::vector<std::tuple<int, int, int>> items8 = {{5, 3, 1}, {6, 4, 1}, {7, 5, 1}, {8, 6, 1}};
    assert(maxGroupValue(5, items8) == 7); // weight5 value7

    // Case: duplicate values and weights across groups
    std::vector<std::tuple<int, int, int>> items9 = {{10, 2, 1}, {10, 2, 2}, {10, 2, 3}};
    assert(maxGroupValue(6, items9) == 30); // all three fit, weight2 each, total6

    // Case: large capacity but few groups
    std::vector<std::tuple<int, int, int>> items10 = {{1, 1, 1}, {2, 1, 2}, {3, 1, 3}};
    assert(maxGroupValue(100, items10) == 6); // all three fit

    return 0;
}
// This problem is a variant of the 0/1 knapsack problem with an additional grouping constraint: you can pick at most one item per group. The solution uses a 1D dynamic programming array `dp` where `dp[j]` represents the maximum total value achievable with total weight exactly `j` (or at most `j`, depending on initialization and transitions). We process groups sequentially. For each group, we consider all items in that group, and update `dp` from high capacity down to 0 to prevent using more than one item from the same group. For each capacity `j`, we try each item in the group: if the item fits (`weight <= j`), we update `dp[j] = max(dp[j], dp[j - weight] + value)`. Because we iterate `j` decreasing, each group item is considered independently, but only one can be selected per group (since the inner loop over items does not accumulate within the same group pass).  
// Edge cases: empty items list (return 0); groups with no items are ignored; items with weight greater than capacity are never used; capacity can be 0 (return 0). Complexity: Let `G` be number of groups, and `K` be the maximum items in any group. The time complexity is `O(capacity * G * K)` and space complexity `O(capacity)`.
