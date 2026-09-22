// Write a C++ function `maxProfitFromPrices` that takes a non-empty `vector<int>` of daily stock prices (where each element represents the price on a given day in chronological order) and returns the maximum profit that can be achieved by buying on one day and selling on a later day. If no profit is possible (i.e., prices only decrease or stay flat), return 0. The function must operate in a single pass over the array and handle cases with duplicate prices, negative prices (treat them as normal values, though they may represent unusual market conditions), and arrays of length 1 (where no transaction is possible, so the result is 0). You must not use any additional data structures; only constant extra space is allowed.

#include <cassert>
#include <vector>

int maxProfitFromPrices(const std::vector<int>& prices);

int main() {
    // Ordinary increasing prices
    assert(maxProfitFromPrices({7, 1, 5, 3, 6, 4}) == 5);
    // Strictly decreasing prices -> no profit
    assert(maxProfitFromPrices({7, 6, 4, 3, 1}) == 0);
    // Constant prices -> no profit
    assert(maxProfitFromPrices({5, 5, 5}) == 0);
    // Single element -> no transaction possible
    assert(maxProfitFromPrices({10}) == 0);
    // Prices with negative values
    assert(maxProfitFromPrices({-2, -5, -3, -1}) == 4);
    // Buy at lowest point and sell at highest later
    assert(maxProfitFromPrices({3, 2, 6, 5, 0, 9}) == 9);
    // Duplicates and high spike
    assert(maxProfitFromPrices({1, 2, 1, 1, 3, 2}) == 2);
    // Already sorted descending, but later has a spike
    assert(maxProfitFromPrices({9, 8, 7, 10, 6, 12}) == 5);
    // Large range
    assert(maxProfitFromPrices({100000, 1, 99999, 50000}) == 99998);
    return 0;
}

#include <vector>
#include <algorithm>

// Return the maximum profit from buying and selling once, given daily prices.
// If no profit is possible, return 0.
int maxProfitFromPrices(const std::vector<int>& prices) {
    if (prices.empty()) {
        return 0;
    }

    int minBuy = prices[0];
    int maxProfit = 0;

    for (size_t i = 1; i < prices.size(); ++i) {
        minBuy = std::min(minBuy, prices[i]);
        int currentProfit = prices[i] - minBuy;
        maxProfit = std::max(maxProfit, currentProfit);
    }

    return maxProfit;
}

// The core idea is to track the lowest price seen so far as we iterate through the array, and at each position compute the potential profit if we sell on that day (current price minus the lowest buy price seen so far). We maintain a running maximum profit that is updated whenever the current profit exceeds it. Initially, set `minBuy` to the first price and `maxProfit` to 0. For each subsequent price, if it is lower than `minBuy`, update `minBuy` to that price (because a lower buy price can only improve future profits). Then compute `currentProfit = price - minBuy` and update `maxProfit = max(maxProfit, currentProfit)`. This works because buying at the minimum price seen so far before the current day guarantees the maximum possible profit for selling on that day. Edge cases include: an array with a single element, where the loop over remaining elements does nothing and maxProfit remains 0; an array with strictly decreasing prices, where at every step currentProfit is negative (since price < minBuy? No—if price is less than minBuy, we update minBuy first, so currentProfit becomes 0; otherwise it's negative, but we never take negative profits because we start maxProfit at 0 and only keep it if it's greater). The algorithm runs in O(n) time and O(1) auxiliary space, where n is the number of prices.
