// Write a C++ function that processes two "inventory" maps representing an existing stock (`stock`) and a demanded order (`order`), both keyed by item IDs (integers) with integer quantities. For each item present in the order, the function must fulfill as much of the demand as possible from the available stock (i.e., take the minimum of stock and demand), then reduce the stock quantity by that fulfilled amount and set the order quantity to the fulfilled amount (so it records what was actually shipped). After processing all order items, return a `std::vector<std::pair<int, int>>` containing all items that remain in stock (quantity > 0) after fulfillment, sorted by item ID. Items with zero remaining stock and zero shipped quantity should be omitted entirely. The input maps may contain arbitrary item IDs (including negative, zero, or large values), and quantities are non-negative.

// The main algorithm iterates over each entry in the order map. For each key, it looks up the current stock quantity using `find` to avoid accidentally inserting a zero entry if the key does not exist (though using `operator[]` would also work, since the initial value is 0 and the subsequent arithmetic still yields correct results; using `find` explicitly avoids modifying the stock map unnecessarily). The fulfilled amount is computed as `min(stock_quantity, order_quantity)`. Then the stock quantity is decremented by that amount, and the order entry is replaced by the fulfilled amount. After this loop, a new vector is built by iterating through the stock map and pushing any pair whose quantity is strictly greater than zero. Since `std::map` is ordered by key, the resulting vector is automatically sorted by item ID. Edge cases include: (1) an item in the order but not in stock — the stock lookup should return 0, so fulfilled is 0, stock entry is not added, and the order entry becomes 0 (and will be omitted when constructing the output); (2) an item in stock but not in the order — untouched, remains in the output; (3) duplicates of item IDs in the maps are impossible by construction (maps store unique keys). Time complexity is O((n + m) log(n + m)) dominated by map operations and the final iteration over the stock map, where n and m are the sizes of the respective maps. Space complexity is O(n + m) for the maps and the output vector.

#include <vector>
#include <map>
#include <utility>
#include <algorithm>

// Fulfill an order from stock, then return the remaining stock items (quantity > 0), sorted by ID.
std::vector<std::pair<int, int>> fulfillOrder(
    std::map<int, int> stock,
    std::map<int, int>& order
) {
    for (auto& orderItem : order) {
        const int itemId = orderItem.first;
        const int available = stock.count(itemId) ? stock.at(itemId) : 0;
        const int shipped = std::min(available, orderItem.second);
        
        // Reduce stock
        stock[itemId] = available - shipped;
        // Record shipped quantity (0 if not fulfilled)
        orderItem.second = shipped;
    }
    
    // Collect remaining stock with positive quantity
    std::vector<std::pair<int, int>> remaining;
    for (const auto& item : stock) {
        if (item.second > 0) {
            remaining.push_back(item);
        }
    }
    return remaining;
}

#include <cassert>
#include <map>
#include <vector>

// Declaration of the function under test (include the solution as-is or link it)
std::vector<std::pair<int, int>> fulfillOrder(
    std::map<int, int> stock,
    std::map<int, int>& order
);

int main() {
    // Basic fulfillment
    {
        std::map<int, int> stock = {{1, 10}, {2, 5}};
        std::map<int, int> order = {{1, 3}, {2, 8}};
        auto remaining = fulfillOrder(stock, order);
        assert((remaining == std::vector<std::pair<int, int>>{{1, 7}, {2, 5}})); // stock after: 1→7, 2→0 (omitted)
        // Order is modified to shipped quantities
        assert(order[1] == 3);
        assert(order[2] == 5);
    }
    // Item in order but not in stock
    {
        std::map<int, int> stock = {{1, 10}};
        std::map<int, int> order = {{1, 4}, {2, 7}};
        auto remaining = fulfillOrder(stock, order);
        assert((remaining == std::vector<std::pair<int, int>>{{1, 6}})); // 1→10-4=6, 2 not in stock => nothing
        assert(order[1] == 4);
        assert(order[2] == 0);
    }
    // Item in stock but not in order remains untouched
    {
        std::map<int, int> stock = {{3, 1}, {5, 2}};
        std::map<int, int> order = {{3, 1}};
        auto remaining = fulfillOrder(stock, order);
        assert((remaining == std::vector<std::pair<int, int>>{{5, 2}})); // 3 fully used, 5 untouched
        assert(order[3] == 1);
    }
    // Exact match
    {
        std::map<int, int> stock = {{7, 3}};
        std::map<int, int> order = {{7, 3}};
        auto remaining = fulfillOrder(stock, order);
        assert(remaining.empty());
        assert(order[7] == 3);
    }
    // Zero quantities and negative IDs
    {
        std::map<int, int> stock = {{-2, 0}, {0, 5}}; // -2 has zero stock, should not appear
        std::map<int, int> order = {{-2, 0}, {0, 4}};
        auto remaining = fulfillOrder(stock, order);
        assert((remaining == std::vector<std::pair<int, int>>{{0, 1}})); // -2 zero stock omitted, 0 becomes 1
        assert(order[-2] == 0);
        assert(order[0] == 4);
    }
    return 0;
}
