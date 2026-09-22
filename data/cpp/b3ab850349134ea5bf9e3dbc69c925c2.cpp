Write a C++ function `int removeLowestSellOrders(OrderBook& book, const std::string& symbol, int maxQuantity)` that, for a given order book (structured as in the provided snippet—an `OrderBook` class with private `map<double, vector<Order>> buyOrders` and `sellOrders`), removes the lowest-priced sell orders for the specified symbol until the total quantity removed is at least `maxQuantity` (but never exceeding it; if the total available quantity for that symbol at the lowest price is insufficient, remove all orders at that price and continue to the next price level). The function returns the total quantity actually removed. The removal must only affect sell orders, must consider all price levels even if some are empty, and must not alter buy orders. The `Order` struct should be defined exactly as in the snippet (`string id, symbol; double price; int quantity; string side`). The function should be a free function that takes a non-const `OrderBook` reference (so it can modify it) and returns the removed quantity. Edge cases: if `maxQuantity` is 0 or negative, return 0; if no sell orders exist for the symbol, return 0; if total sell quantity for the symbol is less than `maxQuantity`, remove all of them and return the total removed. The function must not assume the order book is sorted by price (since the map is ordered by price, lowest price is the first element of `sellOrders`).
// The solution leverages the fact that `sellOrders` is a `std::map<double, vector<Order>>`, which is automatically sorted by key (price) in ascending order. To remove lowest-priced sells first, iterate from the beginning of the map (`begin()`) to the end. For each price level, check each order in the vector: if it matches the given `symbol`, we consider it for removal. We accumulate a `remaining` counter initialized to `maxQuantity`. For each matching order, if `remaining` is greater than 0, we remove as much quantity as possible: take `removeQty = min(order.quantity, remaining)`. Subtract `removeQty` from the order's quantity and from `remaining`, and add `removeQty` to the total removed. If after updating, the order's quantity becomes 0, we erase that order from the vector. To avoid iterator invalidation, we either use index-based iteration (backwards) or rebuild the vector. A clean approach: for each price level, iterate over the vector using index forward, and if an order is fully removed, erase it (which shifts elements) but since we are only moving forward, we can instead build a new vector of remaining orders. Alternatively, use `while` loop with iterator. After processing all orders at a price level, if the vector becomes empty, erase that key from the map. Since we only remove from the front of the map, and we stop when `remaining` reaches 0, we break out of the entire loop. Time complexity: O(N) where N is the total number of sell orders across all prices, because we may inspect each order once. Space complexity: O(1) extra, aside from temporary modifications to the map.
#include <string>
#include <vector>
#include <map>
#include <algorithm>

struct Order {
    std::string id;
    std::string symbol;
    double price;
    int quantity;
    std::string side;
};

class OrderBook {
public:
    void addOrder(Order order) {
        if (order.side == "buy") {
            buyOrders[order.price].push_back(order);
        } else if (order.side == "sell") {
            sellOrders[order.price].push_back(order);
        }
    }

    void printOrderBook() {
        // Implementation omitted for brevity, not needed for the solution.
    }

    // Private members accessible only to the class; the free function will need to be a friend or we provide a public method? Since the task requires a free function, we'll add a friend declaration or make the maps public? Better: provide public accessors for testability? But the original snippet has private maps, so we'll add a friend declaration in the class.
    friend int removeLowestSellOrders(OrderBook& book, const std::string& symbol, int maxQuantity);

private:
    std::map<double, std::vector<Order>> buyOrders;
    std::map<double, std::vector<Order>> sellOrders;
};

// Remove lowest-priced sell orders for a given symbol up to maxQuantity total.
// Returns total quantity removed.
int removeLowestSellOrders(OrderBook& book, const std::string& symbol, int maxQuantity) {
    if (maxQuantity <= 0) return 0;

    int remaining = maxQuantity;
    int removedTotal = 0;

    // sellOrders is a map sorted by price ascending; begin() is lowest price.
    auto it = book.sellOrders.begin();
    while (it != book.sellOrders.end() && remaining > 0) {
        // Work on a copy of the vector to safely erase while iterating.
        std::vector<Order>& vec = it->second;
        std::vector<Order> newVec;
        newVec.reserve(vec.size());

        for (const Order& order : vec) {
            if (order.symbol == symbol && remaining > 0) {
                int removeQty = std::min(order.quantity, remaining);
                if (removeQty > 0) {
                    remaining -= removeQty;
                    removedTotal += removeQty;
                    // If not fully removed, push the remainder back.
                    if (order.quantity > removeQty) {
                        Order remainder = order;
                        remainder.quantity -= removeQty;
                        newVec.push_back(remainder);
                    }
                    // If fully removed, do not push anything.
                    continue;
                }
            }
            // Not matching or remaining==0, keep as is.
            newVec.push_back(order);
        }

        // Replace the old vector.
        vec.swap(newVec);

        // If vector is now empty, erase this price level.
        if (vec.empty()) {
            it = book.sellOrders.erase(it);
        } else {
            ++it;
        }
    }

    return removedTotal;
}
#include <cassert>
#include <string>
#include <vector>
#include <map>

// Include the solution code here (or link appropriately). We'll inline the struct and class for the test.

// (The solution code from above is assumed to be included here.)

int main() {
    OrderBook book;

    // Test 1: Basic removal from lowest price.
    book.addOrder({"sell1", "AAPL", 100.0, 50, "sell"});
    book.addOrder({"sell2", "AAPL", 101.0, 30, "sell"});
    book.addOrder({"sell3", "AAPL", 99.0, 20, "sell"});
    book.addOrder({"sell4", "MSFT", 400.0, 10, "sell"}); // different symbol

    // Remove 45 shares from lowest price (99) first, then 25 from next (100).
    assert(removeLowestSellOrders(book, "AAPL", 45) == 45);
    // After removal: price 99 has 0 (removed all 20), price 100 has 50-(45-20)=25 left, price 101 still 30.
    assert(book.sellOrders[99.0].empty() == true);
    assert(book.sellOrders[100.0].size() == 1 && book.sellOrders[100.0][0].quantity == 25);
    assert(book.sellOrders[101.0].size() == 1 && book.sellOrders[101.0][0].quantity == 30);

    // Test 2: Remove more than available.
    int available = 25 + 30; // from 100 and 101, 99 is gone
    assert(removeLowestSellOrders(book, "AAPL", 1000) == available);
    assert(book.sellOrders.find(100.0) == book.sellOrders.end());
    assert(book.sellOrders.find(101.0) == book.sellOrders.end());

    // Test 3: No matching symbol.
    book.addOrder({"sell5", "AAPL", 50.0, 5, "sell"});
    book.addOrder({"sell6", "GOOG", 2000.0, 7, "sell"});
    assert(removeLowestSellOrders(book, "TSLA", 10) == 0);
    assert(book.sellOrders[50.0].size() == 1);
    assert(book.sellOrders[2000.0].size() == 1);

    // Test 4: maxQuantity zero or negative.
    assert(removeLowestSellOrders(book, "AAPL", 0) == 0);
    assert(removeLowestSellOrders(book, "AAPL", -5) == 0);

    // Test 5: Remove exactly entire order.
    assert(removeLowestSellOrders(book, "AAPL", 5) == 5);
    assert(book.sellOrders.find(50.0) == book.sellOrders.end());

    // Test 6: Ensure buy orders are untouched.
    book.addOrder({"buy1", "AAPL", 90.0, 100, "buy"});
    assert(book.buyOrders.size() == 1);
    // Call removal on sells, buy remains.
    removeLowestSellOrders(book, "AAPL", 1);
    assert(book.buyOrders.size() == 1);

    return 0;
}
