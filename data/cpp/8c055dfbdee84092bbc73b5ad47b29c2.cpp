Write a standalone C++ function named `findLagrangianPair` that, given a vector of positive weight-penalty pairs `(weight, penalty)` and a capacity limit, determines whether there exists a non-empty subset of items whose total weight does not exceed the capacity and whose total penalty is minimized. The function should return the minimal achievable total penalty, or `-1` if no non-empty subset fits within the capacity. The input is a `std::vector<std::pair<int,int>>` where each pair represents `(weight, penalty)`, and a positive integer `capacity`. The function must handle up to 20 items, weights and penalties up to 10^6, and capacity up to 10^9, using an efficient algorithm that avoids exponential enumeration over all subsets.
This is a classic 0/1 knapsack-style problem but with the objective of minimizing penalty subject to a weight constraint, and with the requirement that the subset must be non-empty. The key challenge is the combination of potentially large capacity (up to 1e9) and moderate item count (up to 20). Brute-force over all subsets (2^20 ≈ 1 million) is feasible but could be optimized further; however, the main trick is to handle large weights without allocating an array sized to capacity. The standard approach is to use a store of best (minimum penalty) for each achievable total weight, but since capacity is huge, we cannot allocate an array of that size. Instead, we can use a dynamic programming over items with a dictionary/map that stores the minimum penalty for each total weight. The number of distinct total weights reachable after processing each item is at most 2^i, so with 20 items it's at most 1,048,576 states — feasible in memory and time. We start with a map containing only weight 0 with penalty 0 (representing empty subset). For each item, we create a new map that includes both not taking the item (retaining existing states) and taking it (adding weight and penalty if the weight does not exceed capacity). To enforce the non-empty subset at the end, we ignore the state with weight 0 when determining the minimal penalty. We must also handle duplicates and ensure we pick the minimal penalty for each weight. Time complexity is O(n * number_of_reachable_weights) in the worst case, which is O(n * 2^n) but with n=20 that's about 20 million operations, well within limits. Space complexity is O(2^n) for the dict. Edge cases include: all items weigh more than capacity (then return -1), and when there is an item with weight <= capacity, at least one feasible subset exists.
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <limits>

// Given a list of (weight, penalty) pairs and a capacity, return the minimal total penalty
// of a non-empty subset with total weight <= capacity, or -1 if no such subset exists.
int findLagrangianPair(const std::vector<std::pair<int,int>>& items, int capacity) {
    // Use a map from total weight to minimal penalty for that weight.
    // Start with the empty subset: weight 0, penalty 0.
    std::unordered_map<long long, long long> dp;
    dp[0] = 0;

    for (const auto& item : items) {
        int w = item.first;
        int p = item.second;
        // Create a copy of current dp to iterate over while modifying a new map.
        std::unordered_map<long long, long long> next = dp; // not taking this item
        for (const auto& entry : dp) {
            long long newWeight = entry.first + w;
            if (newWeight > capacity) continue; // prune if exceeding capacity
            long long newPenalty = entry.second + p;
            auto it = next.find(newWeight);
            if (it == next.end() || newPenalty < it->second) {
                next[newWeight] = newPenalty;
            }
        }
        dp = std::move(next);
    }

    // Find the minimal penalty among all non-empty subsets (i.e., weight > 0).
    long long best = std::numeric_limits<long long>::max();
    for (const auto& entry : dp) {
        if (entry.first > 0 && entry.second < best) {
            best = entry.second;
        }
    }
    return best == std::numeric_limits<long long>::max() ? -1 : static_cast<int>(best);
}
#include <cassert>
#include <vector>
#include <utility>

int main() {
    // Basic case: one item fits
    assert(findLagrangianPair({{3, 5}}, 10) == 5);
    // No item fits (capacity too small)
    assert(findLagrangianPair({{10, 1}}, 5) == -1);
    // Multiple items, pick minimal penalty that fits
    assert(findLagrangianPair({{2, 3}, {3, 4}, {1, 2}}, 3) == 2); // {1,2}
    // Exactly fits
    assert(findLagrangianPair({{5, 7}, {2, 1}, {3, 2}}, 5) == 1); // {2,1}
    // Must choose non-empty, even if empty subset has penalty 0
    assert(findLagrangianPair({{5, 10}, {1, 2}}, 1) == 2); // only {1,2} fits
    // Larger capacity, multiple combinations
    assert(findLagrangianPair({{2, 5}, {3, 1}, {4, 2}, {1, 3}}, 5) == 1); // {3,1}
    // All items too heavy, return -1
    assert(findLagrangianPair({{10, 1}, {20, 2}}, 5) == -1);
    // Zero weight item? Not typical but handle if weight 0 given; it's always feasible, but we require non-empty
    assert(findLagrangianPair({{0, 3}, {5, 1}}, 5) == 3); // {0,3} is non-empty and fits
    // Large penalties and weights within limits
    assert(findLagrangianPair({{1000000, 1000000}, {500000, 500000}}, 1500000) == 500000);
    // Duplicate weights, choose minimal penalty
    assert(findLagrangianPair({{2, 10}, {2, 3}, {3, 1}}, 2) == 3);
    return 0;
}
