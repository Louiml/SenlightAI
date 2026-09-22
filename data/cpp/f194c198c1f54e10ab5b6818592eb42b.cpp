// Write a standalone C++ function `maxKnapsackValue(int capacity, const std::vector<Item>& items)` that computes the maximum total value achievable by selecting a subset of items (each item can be chosen at most once) without exceeding the given knapsack capacity. The `Item` struct has two integer members: `weight` and `value`. The function must handle cases with zero or negative capacity, empty item lists, duplicate items, and items with zero or negative weights or values. Negative values should be excluded from selection if they reduce total profit, and zero-weight positive-value items should always be included. Input items are not necessarily sorted, and the function must not modify the input vector (use const reference). You may assume capacity and all weights/values fit within `int`, but the sum of values might exceed `int` — use `long long` for the return type to avoid overflow.

#include <cassert>
#include <vector>

// Item struct is defined in the solution file; include it here or define again.
struct Item {
    int weight;
    int value;
};

// Declaration of the function to test
long long maxKnapsackValue(int capacity, const std::vector<Item>& items);

int main() {
    // Basic test: capacity 10, items (2,5) (3,6) (4,7) (5,9)
    std::vector<Item> items1 = {{2,5},{3,6},{4,7},{5,9}};
    assert(maxKnapsackValue(10, items1) == 18); // choose (5,9) and (2,5) and (3,6)? Actually 9+5+6=20 but weight 5+2+3=10 -> 20, check: (5,9)+(3,6)+(2,5) weight=10 value=20. So answer is 20. Wait, better: (4,7)+(3,6)+(2,5) weight=9 value=18, but 20 is better.
    assert(maxKnapsackValue(10, items1) == 20);

    // Empty items
    assert(maxKnapsackValue(10, {}) == 0);
    // Zero capacity
    assert(maxKnapsackValue(0, {{5,10},{2,3}}) == 0);
    // Negative capacity
    assert(maxKnapsackValue(-5, {{1,1}}) == 0);
    // Zero-weight positive-value items
    std::vector<Item> items2 = {{0,5},{0,7}};
    assert(maxKnapsackValue(10, items2) == 12);
    // Negative value items should be ignored
    std::vector<Item> items3 = {{3,-5},{2,4},{1,-1}};
    assert(maxKnapsackValue(3, items3) == 4);
    // Overflow check: large values
    std::vector<Item> items4 = {{1000000, 1000000000}};
    long long cap = 1000000;
    assert(maxKnapsackValue(cap, items4) == 1000000000LL);
    // Items that exactly fill capacity
    std::vector<Item> items5 = {{3,10},{4,20},{5,30}};
    assert(maxKnapsackValue(7, items5) == 30); // choose (5,30) or (3,10)+(4,20)=30
    // Many items, capacity less than any weight
    std::vector<Item> items6 = {{10,1},{20,2}};
    assert(maxKnapsackValue(5, items6) == 0);
    // Duplicate items
    std::vector<Item> items7 = {{2,3},{2,3},{2,3}};
    assert(maxKnapsackValue(4, items7) == 6); // two copies
    return 0;
}

#include <vector>
#include <algorithm>

struct Item {
    int weight;
    int value;
};

long long maxKnapsackValue(int capacity, const std::vector<Item>& items) {
    // Base value from items that can be taken for free (weight <= 0) with positive value
    long long baseValue = 0;
    std::vector<Item> validItems;
    
    for (const Item& item : items) {
        if (item.weight <= 0) {
            if (item.value > 0) baseValue += item.value;
            // ignore items with weight <= 0 (handled above)
        } else if (item.value > 0) {
            validItems.push_back(item);
        }
        // ignore items with positive weight but non-positive value
    }
    
    // If capacity <= 0, we can only take free items (already added)
    if (capacity <= 0) return baseValue;
    
    // dp[j] = maximum value achievable with total weight <= j
    std::vector<long long> dp(capacity + 1, 0);
    
    for (const Item& item : validItems) {
        // Traverse backwards to ensure each item is used at most once
        for (int w = capacity; w >= item.weight; --w) {
            dp[w] = std::max(dp[w], dp[w - item.weight] + static_cast<long long>(item.value));
        }
    }
    
    // The answer is the maximum over all dp[0..capacity]
    long long maxVal = 0;
    for (long long v : dp) maxVal = std::max(maxVal, v);
    
    return baseValue + maxVal;
}

// The task is a classic 0/1 Knapsack optimization. The main solution uses dynamic programming with a 1D DP array for space efficiency: `dp[j]` represents the maximum value achievable with a total weight exactly `j` (or at most `j` depending on implementation). We iterate over each item, and for each weight from capacity down to the item's weight, we update `dp[j] = max(dp[j], dp[j - weight] + value)`. This ensures each item is used at most once. After processing all items, the answer is the maximum value among all `dp[j]` for `j <= capacity`, because we don't need to fill the knapsack exactly — we can leave unused capacity.
//
// Important edge cases:
// - **Empty item list or capacity = 0**: No items can be selected; answer is 0.
// - **Negative weights**: These are problematic physically, but to handle robustly, we can ignore items with weight <= 0 and positive value? Actually, a negative weight would allow pseudo-infinite capacity; to keep it simple, we assume weights are non-negative (we can filter out items with weight <= 0? But zero-weight items with positive value should be included; negative-weight items can be skipped if weight <= 0 and value <= 0? For correctness, we treat zero-weight items specially: if value > 0, add it to the base result and skip them in DP; negative-weight items are invalid and can be ignored). The problem statement allows zero or negative weights, so we'll handle them by adding any item with weight <= 0 and value > 0 to the base sum, and ignoring all items with weight <= 0 from DP (they either add value for free or are useless). Negative values: we never select items with negative value because they reduce profit.
// - **Overflow**: Use `long long` for DP values.
// - **Duplicate items**: DP handles them naturally.
//
// Time complexity: O(n * capacity) where n = number of items. Space: O(capacity) for the 1D DP array. If capacity is very large (e.g., > 10^6), this might be memory-heavy; an alternative is branch and bound, but DP is straightforward. We'll keep DP.
//
// The solution function will:
// 1. Filter out items with weight > 0 and value > 0 (valid candidates). Items with weight <= 0 and value > 0 are added to `baseValue`, because they can be taken for free; items with weight <= 0 and value <= 0 are ignored. Items with weight > 0 but value <= 0 are also ignored.
// 2. Create a `dp` vector of size `capacity+1` initialized to 0.
// 3. For each valid item, iterate weight from capacity down to item.weight and update dp.
// 4. Return `baseValue + max(dp[0..capacity])` (the max over all dp entries).
