Write a C++ function named `updateBestBidAsk` that, given a reference to an `OrderBook` structure (as defined in the provided snippet) and a stock symbol string, updates the `bestBid` and `bestAsk` fields in the `OrderBook` to reflect the current top of book based on the existing price levels in that symbol’s buy and sell books. The function must correctly handle empty buy or sell books by setting the corresponding best price and total volume to zero. It must also update the `tob` (top of book) fields (bidPrice, bidQty, askPrice, askQty) in the `OrderBook` to match these values. The function should not perform any matching, order insertion, or cancellation—it only reads the current state of the order books and updates the best price levels. No output should be printed inside the function (i.e., do not call `publishTopOfBuyBook` or `publishTopOfSellBook`). Your implementation must be thread-safe by locking the mutex `m` inside the `OrderBook` before accessing or modifying any fields. Assume that the `PriceLevel` struct has `price`, `totalVolume`, and `orders` (a `std::list<Order>`) members, and that `BuyBook` and `SellBook` each contain a `limitMap` (a `std::map<int, PriceLevel, std::greater<int>>` for the buy side and a `std::map<int, PriceLevel>` for the sell side). The function should be defined as `void updateBestBidAsk(const std::string& symbol, OrderBook& orderBook)`.
#include <cassert>
#include <iostream>

// Include the solution here (or copy the code above into the test file)
// For demonstration, we include the necessary declarations and the function.

int main() {
    OrderBook book;

    // Test 1: Empty book -> all zeros
    updateBestBidAsk("AAPL", book);
    assert(book.bestBid.price == 0);
    assert(book.bestBid.totalVolume == 0);
    assert(book.bestAsk.price == 0);
    assert(book.bestAsk.totalVolume == 0);
    assert(book.tob.bidPrice == 0);
    assert(book.tob.bidQty == 0);
    assert(book.tob.askPrice == 0);
    assert(book.tob.askQty == 0);

    // Test 2: Add a buy order at price 100, qty 10
    {
        Order o{OrderType::NEW, 1, 101, "AAPL", 100, 10, 'B'};
        book.buyBook["AAPL"].limitMap[100].price = 100;
        book.buyBook["AAPL"].limitMap[100].totalVolume = 10;
        book.buyBook["AAPL"].limitMap[100].orders.push_back(o);
    }
    updateBestBidAsk("AAPL", book);
    assert(book.bestBid.price == 100);
    assert(book.bestBid.totalVolume == 10);
    assert(book.tob.bidPrice == 100);
    assert(book.tob.bidQty == 10);
    // Sell side still empty
    assert(book.bestAsk.price == 0);
    assert(book.tob.askPrice == 0);

    // Test 3: Add a sell order at price 105, qty 20
    {
        Order o{OrderType::NEW, 2, 201, "AAPL", 105, 20, 'S'};
        book.sellBook["AAPL"].limitMap[105].price = 105;
        book.sellBook["AAPL"].limitMap[105].totalVolume = 20;
        book.sellBook["AAPL"].limitMap[105].orders.push_back(o);
    }
    updateBestBidAsk("AAPL", book);
    assert(book.bestAsk.price == 105);
    assert(book.bestAsk.totalVolume == 20);
    assert(book.tob.askPrice == 105);
    assert(book.tob.askQty == 20);
    assert(book.bestBid.price == 100);
    assert(book.bestBid.totalVolume == 10);

    // Test 4: Add a higher bid (price 110) -> bestBid updates
    {
        Order o{OrderType::NEW, 3, 301, "AAPL", 110, 5, 'B'};
        book.buyBook["AAPL"].limitMap[110].price = 110;
        book.buyBook["AAPL"].limitMap[110].totalVolume = 5;
        book.buyBook["AAPL"].limitMap[110].orders.push_back(o);
    }
    updateBestBidAsk("AAPL", book);
    assert(book.bestBid.price == 110);
    assert(book.bestBid.totalVolume == 5);
    assert(book.tob.bidPrice == 110);
    assert(book.tob.bidQty == 5);

    // Test 5: Add a lower ask (price 90) -> bestAsk updates
    {
        Order o{OrderType::NEW, 4, 401, "AAPL", 90, 8, 'S'};
        book.sellBook["AAPL"].limitMap[90].price = 90;
        book.sellBook["AAPL"].limitMap[90].totalVolume = 8;
        book.sellBook["AAPL"].limitMap[90].orders.push_back(o);
    }
    updateBestBidAsk("AAPL", book);
    assert(book.bestAsk.price == 90);
    assert(book.bestAsk.totalVolume == 8);
    assert(book.tob.askPrice == 90);
    assert(book.tob.askQty == 8);

    // Test 6: Remove the highest bid (price 110) by clearing the map for buy side
    book.buyBook["AAPL"].limitMap.clear();
    updateBestBidAsk("AAPL", book);
    assert(book.bestBid.price == 0);
    assert(book.bestBid.totalVolume == 0);
    assert(book.tob.bidPrice == 0);
    assert(book.tob.bidQty == 0);
    // Sell side unchanged
    assert(book.bestAsk.price == 90);
    assert(book.tob.askPrice == 90);

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
#include <mutex>
#include <map>
#include <list>
#include <string>

// Assuming the structs and enums from the snippet are defined elsewhere.
// To keep the solution self-contained, include necessary declarations.
enum class OrderType { NEW, CANCEL };
enum class Side { Buy, Sell };

struct Order {
    OrderType type;
    int user;
    int userOrderId;
    std::string symbol;
    int price;
    int qty;
    char side;
    bool operator<(const Order &rhs) const { return price < rhs.price; }
    bool operator==(const Order& other) const { return userOrderId == other.userOrderId; }
};

struct Trade {
    int userIdBuy;
    int userOrderIdBuy;
    int userIdSell;
    int userOrderIdSell;
    int price;
    int qty;
};

struct TopOfBook {
    int bidPrice;
    int bidQty;
    int askPrice;
    int askQty;
    void reset() { bidPrice = 0; bidQty = 0; askPrice = 0; askQty = 0; }
};

struct PriceLevel {
    int price;
    int totalVolume;
    std::list<Order> orders;
    void reset() { price = 0; totalVolume = 0; orders.clear(); }
};

struct greater {
    template<class T>
    bool operator()(T const &a, T const &b) const { return a > b; }
};

struct BuyBook {
    std::map<int, PriceLevel, std::greater<int> > limitMap;
};

struct SellBook {
    std::map<int, PriceLevel> limitMap;
};

struct OrderBook {
    std::map<std::string, BuyBook> buyBook;
    std::map<std::string, SellBook> sellBook;
    PriceLevel bestBid;
    PriceLevel bestAsk;
    std::map<int, Order> orderMap;
    std::vector<Trade> trades;
    std::mutex m;
    TopOfBook tob;
    void flush() {
        std::lock_guard<std::mutex> lock(m);
        buyBook.clear();
        sellBook.clear();
        bestBid.reset();
        bestAsk.reset();
        orderMap.clear();
        trades.clear();
        tob.reset();
    }
};

// Update the best bid and ask (top of book) for a given symbol.
void updateBestBidAsk(const std::string& symbol, OrderBook& orderBook) {
    std::lock_guard<std::mutex> lock(orderBook.m);

    // Update buy side (highest bid)
    auto& buyBook = orderBook.buyBook[symbol];
    if (!buyBook.limitMap.empty()) {
        auto it = buyBook.limitMap.begin(); // first element = highest price
        orderBook.bestBid.price = it->first;
        orderBook.bestBid.totalVolume = it->second.totalVolume;
        orderBook.tob.bidPrice = it->first;
        orderBook.tob.bidQty = it->second.totalVolume;
    } else {
        orderBook.bestBid.price = 0;
        orderBook.bestBid.totalVolume = 0;
        orderBook.tob.bidPrice = 0;
        orderBook.tob.bidQty = 0;
    }

    // Update sell side (lowest ask)
    auto& sellBook = orderBook.sellBook[symbol];
    if (!sellBook.limitMap.empty()) {
        auto it = sellBook.limitMap.begin(); // first element = lowest price
        orderBook.bestAsk.price = it->first;
        orderBook.bestAsk.totalVolume = it->second.totalVolume;
        orderBook.tob.askPrice = it->first;
        orderBook.tob.askQty = it->second.totalVolume;
    } else {
        orderBook.bestAsk.price = 0;
        orderBook.bestAsk.totalVolume = 0;
        orderBook.tob.askPrice = 0;
        orderBook.tob.askQty = 0;
    }
}
// The solution involves locking the mutex `m` using a `std::lock_guard<std::mutex>` at the start of the function to ensure thread safety. Then, the algorithm accesses the buy and sell books for the given symbol via `orderBook.buyBook[symbol]` and `orderBook.sellBook[symbol]`. For the buy side, since the map is sorted in descending order (due to `std::greater<int>`), the first element (if any) is the highest bid price. If the map is non-empty, the function retrieves the first element's price and `totalVolume` and assigns them to `orderBook.bestBid.price` and `orderBook.bestBid.totalVolume`. It also updates `orderBook.tob.bidPrice` and `orderBook.tob.bidQty` with the same values. If the buy map is empty, it sets both `bestBid` and `tob` bid fields to zero. Similarly, for the sell side, the map is sorted in ascending order (default `std::less<int>`), so the first element is the lowest ask price. The same logic applies: if non-empty, assign the first element's price and volume to `bestAsk` and `tob` ask fields; otherwise, set them to zero. Edge cases include when the buy book is empty but the sell book is not, or vice versa—each side must be handled independently. The time complexity is O(1) because we only inspect the first element of each map (constant-time access to the beginning iterator). The space complexity is O(1) as no additional data structures are used.
