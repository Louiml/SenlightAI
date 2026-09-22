/*
Write a C++ function `fractionalKnapsackMaxValue(int capacity, const std::vector<Item>& items)` that takes a knapsack capacity (an integer) and a vector of `Item` structs (each containing an integer `value` and integer `weight`), and returns the maximum total value achievable when fractions of items may be taken. You may assume that all weights are positive, values are non-negative, and the capacity is non-negative. The function should sort items by their value-to-weight ratio in descending order and greedily take whole items when possible; when an item cannot fit completely, it takes only the fractional part that fills the remaining capacity. Return the answer as a `double`. Edge cases include an empty items list, a capacity of zero, and cases where an item's weight exactly equals the remaining capacity (must take the whole item and stop).
*/

#include <vector>
#include <algorithm>

struct Item {
    int value;
    int weight;
};

// Compute the maximum value obtainable in a fractional knapsack.
double fractionalKnapsackMaxValue(int capacity, const std::vector<Item>& items) {
    // Create a vector of items with their value-to-weight ratios.
    std::vector<std::pair<double, Item>> ratios;
    ratios.reserve(items.size());
    for (const auto& item : items) {
        ratios.emplace_back(static_cast<double>(item.value) / item.weight, item);
    }

    // Sort in descending order of ratio.
    std::sort(ratios.begin(), ratios.end(), [](const auto& a, const auto& b) {
        return a.first > b.first;
    });

    double totalValue = 0.0;
    int remainingCapacity = capacity;

    for (const auto& [ratio, item] : ratios) {
        if (item.weight <= remainingCapacity) {
            // Take the whole item.
            remainingCapacity -= item.weight;
            totalValue += item.value;
        } else {
            // Take a fraction of the item to fill the remaining capacity.
            totalValue += (static_cast<double>(remainingCapacity) / item.weight) * item.value;
            break; // No capacity left.
        }
    }

    return totalValue;
}

#include <cassert>
#include <vector>

int main() {
    // Test 1: Basic example (classic fractional knapsack).
    std::vector<Item> items1 = {{60, 10}, {100, 20}, {120, 30}};
    assert(fractionalKnapsackMaxValue(50, items1) == 240.0);

    // Test 2: Capacity of zero.
    std::vector<Item> items2 = {{10, 5}, {20, 10}};
    assert(fractionalKnapsackMaxValue(0, items2) == 0.0);

    // Test 3: Empty items list.
    std::vector<Item> items3 = {};
    assert(fractionalKnapsackMaxValue(10, items3) == 0.0);

    // Test 4: Take only a fraction of a single item.
    std::vector<Item> items4 = {{50, 10}};
    assert(fractionalKnapsackMaxValue(5, items4) == 25.0); // half of 50.

    // Test 5: All items fit exactly.
    std::vector<Item> items5 = {{10, 2}, {20, 3}};
    assert(fractionalKnapsackMaxValue(5, items5) == 30.0);

    // Test 6: Items with equal ratios, larger capacity than total weight.
    std::vector<Item> items6 = {{5, 1}, {10, 2}};
    assert(fractionalKnapsackMaxValue(10, items6) == 15.0);

    // Test 7: Zero-value items should not add value.
    std::vector<Item> items7 = {{0, 5}, {100, 1}};
    assert(fractionalKnapsackMaxValue(6, items7) == 100.0);

    // Test 8: Large values with fractions (verify correctness via ratio sorting).
    std::vector<Item> items8 = {{70, 20}, {30, 10}, {40, 20}};
    // Ratios: 3.5, 3, 2 -> take first (20), second (10), then fraction of third (10 left from 20) -> 40 * (10/20)=20.
    assert(fractionalKnapsackMaxValue(40, items8) == 120.0);

    // Test 9: Single item with capacity less than weight.
    std::vector<Item> items9 = {{100, 4}};
    assert(fractionalKnapsackMaxValue(2, items9) == 50.0);

    // Test 10: Capacity larger than total weight, all taken fully.
    std::vector<Item> items10 = {{10, 1}, {20, 2}, {30, 3}};
    assert(fractionalKnapsackMaxValue(100, items10) == 60.0);

    return 0;
}

// The solution uses the classic fractional knapsack greedy algorithm. First, compute the value-to-weight ratio for each item (as a `double` to avoid integer division issues) and store pairs of `{ratio, index}` or `{value, weight}`. Then sort the items in descending order of ratio. Iterate through the sorted items: if the current item's weight is less than or equal to the remaining capacity, take the entire item and subtract its weight from the capacity. If the item's weight exceeds the remaining capacity, take only a fraction equal to `(remaining_capacity / item.weight) * item.value` and return the accumulated total, since no more capacity remains. If all items fit entirely, return the accumulated total after processing all. An important edge case is when the ratio is equal—any ordering among them is fine since fractions are allowed. Time complexity is O(n log n) for sorting, and space complexity is O(n) for storing ratios (or O(1) if sorting in place). The algorithm is optimal because it always prioritizes items with the highest marginal value per unit weight.
