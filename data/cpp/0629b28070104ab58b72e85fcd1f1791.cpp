// Write a standalone C++ function `double fractionalKnapsack(int capacity, const std::vector<Item>& items)` that solves the fractional knapsack problem. The function takes a knapsack capacity (an integer) and a vector of `Item` structs (where each `Item` has public `int value` and `int weight` fields). It must return the maximum total value obtainable when you may take fractions of items, as a `double`. You must sort items by value-to-weight ratio in descending order, take full items when possible, and take a fraction of the next item if it doesn't fully fit. The function must handle cases where the capacity is zero, items have zero weight (such items should be entirely ignored because their ratio is undefined or infinite), and where the total weight of all items is less than or equal to capacity (so all items fit fully). The returned value should be accurate to at least one decimal place.
// The main algorithm sorts all items by decreasing value-to-weight ratio (`value / weight`). After sorting, iterate through the items: if the current item's full weight fits entirely within the remaining capacity, add its full value and reduce remaining capacity by its weight. Otherwise, if the item is partially fillable, add the fraction of its value corresponding to the remaining capacity divided by the item's weight, and stop (since remaining capacity becomes zero). Edge cases: (1) If an item has weight 0, its ratio is infinite or undefined, so we skip it entirely; (2) If capacity is 0, return 0.0; (3) If the total weight of all items is ≤ capacity, the loop will take all items fully and no break occurs. Sorting uses `std::sort` with a comparator that returns `true` if the ratio of `a` is greater than `b`; for ties, the order doesn't matter. Time complexity is O(n log n) due to sorting, and space complexity is O(1) extra space (the sorting itself may require O(log n) stack space in typical implementations). The result is a double, and the problem expects one decimal place, but internal precision is fine.
#include <vector>
#include <algorithm>

struct Item {
    int value;
    int weight;
};

// Solves the fractional knapsack problem by value/weight ratio.
double fractionalKnapsack(int capacity, const std::vector<Item>& items) {
    // Filter out items with zero weight because their ratio is undefined.
    std::vector<Item> validItems;
    validItems.reserve(items.size());
    for (const Item& it : items) {
        if (it.weight > 0) {
            validItems.push_back(it);
        }
    }

    // Sort by decreasing value/weight ratio.
    std::sort(validItems.begin(), validItems.end(),
              [](const Item& a, const Item& b) {
                  double ratioA = static_cast<double>(a.value) / a.weight;
                  double ratioB = static_cast<double>(b.value) / b.weight;
                  return ratioA > ratioB;
              });

    double totalValue = 0.0;
    int remainingCapacity = capacity;

    for (const Item& it : validItems) {
        if (remainingCapacity <= 0) {
            break;
        }
        if (it.weight <= remainingCapacity) {
            // Take the whole item.
            remainingCapacity -= it.weight;
            totalValue += it.value;
        } else {
            // Take a fraction of the item.
            double fraction = static_cast<double>(remainingCapacity) / static_cast<double>(it.weight);
            totalValue += it.value * fraction;
            remainingCapacity = 0;
            break;
        }
    }

    return totalValue;
}
#include <cassert>
#include <cmath>
#include <vector>

struct Item {
    int value;
    int weight;
};

// The solution function (declared here for the test, normally would be included)
double fractionalKnapsack(int capacity, const std::vector<Item>& items);

int main() {
    // Example 1: Basic case with partial take.
    std::vector<Item> items1 = {{60, 10}, {100, 20}, {120, 30}};
    double res1 = fractionalKnapsack(50, items1);
    assert(std::fabs(res1 - 240.0) < 1e-9); // Take item 60, 100, and 80 fraction of 120.

    // Example 2: Capacity too small for any item.
    std::vector<Item> items2 = {{10, 5}, {20, 10}};
    double res2 = fractionalKnapsack(3, items2);
    assert(std::fabs(res2 - 6.0) < 1e-9); // Take fraction 3/5 of first item -> 6.0

    // Example 3: All items fit fully.
    std::vector<Item> items3 = {{5, 2}, {7, 3}, {1, 1}};
    double res3 = fractionalKnapsack(10, items3);
    assert(std::fabs(res3 - 13.0) < 1e-9); // Sum of all values = 13.

    // Example 4: Empty items.
    std::vector<Item> items4 = {};
    double res4 = fractionalKnapsack(10, items4);
    assert(std::fabs(res4 - 0.0) < 1e-9);

    // Example 5: Items with zero weight are ignored.
    std::vector<Item> items5 = {{100, 0}, {50, 1}, {30, 2}};
    double res5 = fractionalKnapsack(3, items5);
    assert(std::fabs(res5 - 80.0) < 1e-9); // Take 50 and 30 fully = 80.

    // Example 6: Capacity zero.
    std::vector<Item> items6 = {{100, 1}, {200, 2}};
    double res6 = fractionalKnapsack(0, items6);
    assert(std::fabs(res6 - 0.0) < 1e-9);

    // Example 7: Tie in ratios, any order works.
    std::vector<Item> items7 = {{10, 2}, {15, 3}, {5, 1}}; // All ratio 5
    double res7 = fractionalKnapsack(4, items7);
    assert(std::fabs(res7 - 20.0) < 1e-9); // Take full two items (e.g., 10 and 15? Actually max 20 because capacity 4: take 2+? Let's recalc: all have ratio 5, so best take any mix. Take 10 (2 weight) and 5 (1 weight) and half of 15 (1 weight of 3) -> total 10+5+7.5=22.5? Wait capacity 4, take items in sorted order: ratio 5 for all, any order. Suppose order is 10 (2), 15 (3), 5 (1). Take 10 fully (2 left), take 15 fully? weight 3 >2, so take fraction 2/3 of 15 = 10.0, total 20.0. So answer 20.0.
    // Verify with std::cout for debugging. But test expects 20.0.
    // Actually better to test with capacity 5 to get full of first two: 10+15=25.
    std::vector<Item> items7b = {{10, 2}, {15, 3}, {5, 1}};
    double res7b = fractionalKnapsack(5, items7b);
    assert(std::fabs(res7b - 25.0) < 1e-9); // Take 10 and 15 fully.

    return 0;
}
