/*
Write a C++ function `double fractionalKnapsackValue(const std::vector<int>& values, const std::vector<int>& weights, int capacity)` that solves the fractional knapsack problem: given two equal-length vectors representing item values and weights, and a knapsack capacity (an integer), return the maximum total value that can be obtained by taking fractions of items. Each item can be taken entirely, partially, or not at all, and the goal is to maximize the total value without exceeding the capacity. The function must handle empty inputs (return 0.0), cases where capacity is 0, and cases where all items fit within the capacity. You may assume capacity is non-negative, and all values and weights are positive integers.
*/
#include <vector>
#include <algorithm>
#include <utility>

// Returns the maximum value obtainable in the fractional knapsack problem.
// values and weights must have equal positive size; capacity >= 0.
double fractionalKnapsackValue(const std::vector<int>& values, const std::vector<int>& weights, int capacity) {
    if (values.empty() || weights.empty() || capacity == 0) {
        return 0.0;
    }
    const size_t n = values.size();
    std::vector<std::pair<int, int>> items;
    items.reserve(n);
    for (size_t i = 0; i < n; ++i) {
        items.emplace_back(values[i], weights[i]);
    }
    // Sort by descending value/weight ratio
    std::sort(items.begin(), items.end(), [](const auto& a, const auto& b) {
        return (static_cast<double>(a.first) / a.second) > (static_cast<double>(b.first) / b.second);
    });

    double totalValue = 0.0;
    int currentWeight = 0;
    for (const auto& item : items) {
        if (currentWeight + item.second <= capacity) {
            currentWeight += item.second;
            totalValue += item.first;
        } else {
            int remaining = capacity - currentWeight;
            totalValue += (static_cast<double>(item.first) / item.second) * remaining;
            break;
        }
    }
    return totalValue;
}
#include <cassert>
#include <cmath>

int main() {
    // Basic case with fraction needed
    assert(std::abs(fractionalKnapsackValue({60, 100, 120}, {10, 20, 30}, 50) - 240.0) < 1e-9);
    // All items fit
    assert(std::abs(fractionalKnapsackValue({10, 20, 30}, {1, 2, 3}, 100) - 60.0) < 1e-9);
    // Capacity exactly matches total weight
    assert(std::abs(fractionalKnapsackValue({5, 5}, {2, 3}, 5) - 10.0) < 1e-9);
    // Only one item, partial
    assert(std::abs(fractionalKnapsackValue({10}, {4}, 2) - 5.0) < 1e-9);
    // Empty input
    assert(std::abs(fractionalKnapsackValue({}, {}, 10) - 0.0) < 1e-9);
    // Zero capacity
    assert(std::abs(fractionalKnapsackValue({5, 6}, {2, 3}, 0) - 0.0) < 1e-9);
    // Single item fits exactly
    assert(std::abs(fractionalKnapsackValue({7}, {3}, 3) - 7.0) < 1e-9);
    // case where lower ratio item must be skipped
    assert(std::abs(fractionalKnapsackValue({10, 1}, {1, 100}, 1) - 10.0) < 1e-9);
    // Equal ratios
    assert(std::abs(fractionalKnapsackValue({2, 4}, {1, 2}, 2) - 4.0) < 1e-9);
    // All weights larger than capacity, take fraction of best
    assert(std::abs(fractionalKnapsackValue({100, 1}, {10, 100}, 5) - 50.0) < 1e-9);
}
// The solution uses a greedy approach: compute the value-per-unit-weight ratio for each item, sort items in descending order of this ratio, and then take items greedily. Start with total value 0.0 and current filled weight 0. For each item in sorted order, if adding the entire item does not exceed the capacity, add its full value and weight. Otherwise, take the fraction that exactly fills the remaining capacity: add `(value / weight) * remaining_capacity` to total value and break the loop. This works because the fractional knapsack problem has an optimal substructure and greedy choice property—taking the highest ratio first yields an optimal solution. Edge cases: if the input vectors are empty, return 0.0; if capacity is 0, return 0.0; if total weight of all items ≤ capacity, the result is simply the sum of all values. Time complexity is O(n log n) due to sorting, and space complexity is O(n) for the auxiliary vector of pairs (value, weight).
