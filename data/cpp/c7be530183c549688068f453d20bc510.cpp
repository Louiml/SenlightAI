// Write a C++ function `maxGoldValue` that takes a vector of gold items (each with a weight and value) and a knapsack capacity, and returns the maximum total value that can be carried if items can be taken in fractions (i.e., fractional knapsack problem). The function should sort items by value per unit weight (value/weight) in descending order, take full items that fit, and take a fraction of the next item if the remaining capacity is less than its weight. The return value should be the integer part of the maximum value (i.e., truncate any fractional part). The input vector and capacity are given; the function should handle cases where all items fit, no items fit, capacity is zero, or items have zero weight (in which case unit value should be treated as infinite, and the item's full value should be added if capacity permits, but for simplicity assume weight > 0). Edge case: if capacity is 0, return 0. The function must be self-contained, use appropriate includes, and use `const` correctness for the input vector.
The problem is the classic fractional knapsack optimization. Since we can take fractions, the optimal strategy is to always take the item with the highest value per unit weight first. This is a greedy algorithm because each step we pick the most "valuable" item by ratio, and this yields the globally optimal solution for fractional knapsack. Steps: (1) compute unit value = value / weight for each item. (2) Sort items descending by unit value; if two items have equal ratio, order does not matter. (3) Iterate through sorted items; if the current item's weight <= remaining capacity, take it fully: add its value, subtract weight. Else, take a fraction equal to remaining capacity / item's weight, add that fraction times value, and set remaining capacity to 0. The final accumulated value may be fractional; cast to `int` to truncate (floor for positive numbers). Edge cases: capacity zero → return 0; an item with weight 0 and positive value would cause division by zero, so we assume weight > 0 per problem statement (or we handle it by treating such items as immediately adding their full value, but we exclude that). Time complexity is O(N log N) for sorting, O(N) for traversal, total O(N log N). Space is O(1) extra besides the input vector (we can sort a copy to avoid modifying caller's data).
#include <vector>
#include <algorithm>

// Computes maximum value obtainable from a fractional knapsack.
// Each item has (weight, value). We may take fractions of items.
// Returns integer part of the maximum total value.
int maxGoldValue(const std::vector<std::pair<int,int>>& items, int capacity) {
    if (capacity <= 0) return 0;

    // Create a modifiable copy and compute unit value
    struct Item {
        int weight;
        int value;
        double unit_value;
    };
    std::vector<Item> item_list;
    for (const auto& p : items) {
        // assume weight > 0
        double uv = static_cast<double>(p.second) / p.first;
        item_list.push_back({p.first, p.second, uv});
    }

    // Sort descending by unit value
    std::sort(item_list.begin(), item_list.end(),
              [](const Item& a, const Item& b) {
                  return a.unit_value > b.unit_value;
              });

    double total_value = 0.0;
    int remaining = capacity;

    for (const auto& it : item_list) {
        if (remaining == 0) break;
        if (it.weight <= remaining) {
            total_value += it.value;
            remaining -= it.weight;
        } else {
            total_value += it.unit_value * remaining;
            remaining = 0;
        }
    }

    return static_cast<int>(total_value);
}
#include <cassert>
#include <vector>
using namespace std;

// forward declaration of the function (or include the solution code above)
int maxGoldValue(const vector<pair<int,int>>& items, int capacity);

int main() {
    // Basic case: take the highest value/weight first
    vector<pair<int,int>> items1 = {{10, 60}, {20, 100}, {30, 120}}; // unit: 6,5,4
    assert(maxGoldValue(items1, 50) == 240); // full 10+20=30, then 20 of the 30-weight item => 60+100+ (20*4)=240

    // Capacity zero
    assert(maxGoldValue(items1, 0) == 0);

    // All items fit
    assert(maxGoldValue(items1, 60) == 280); // 60+100+120

    // Only one item, partial
    vector<pair<int,int>> items2 = {{5, 10}}; // unit=2
    assert(maxGoldValue(items2, 3) == 6); // 3*2 = 6

    // Empty vector
    assert(maxGoldValue({}, 10) == 0);

    // Duplicate ratio, ordering doesn't affect total
    vector<pair<int,int>> items3 = {{2, 4}, {4, 8}}; // both unit=2
    assert(maxGoldValue(items3, 3) == 6); // take 2*2 + 1*2 = 6

    // Capacity less than smallest weight
    assert(maxGoldValue(items1, 5) == 30); // 5*6 = 30

    // Large numbers, integer truncation
    vector<pair<int,int>> items4 = {{3, 10}}; // unit=3.333
    assert(maxGoldValue(items4, 1) == 3); // 3.333 floor = 3

    return 0;
}
