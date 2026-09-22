// Write a C++ function `int maxProfit(const std::vector<int>& prices)` that, given a non-empty vector of daily stock prices in chronological order, returns the maximum profit that can be achieved by buying on one day and selling on a later day. If no profit is possible (prices are strictly non-increasing), the function must return 0. The vector may contain duplicates and very large values, but is guaranteed to be non-empty. You may only buy once and sell once, and must sell after buying.

// The solution uses a single pass through the vector while maintaining two variables: the minimum price seen so far (initially the first element) and the maximum profit found (initially 0). For each price, compute the difference between the current price and the running minimum. If that difference is negative, it means the current price is a new minimum, so update the running minimum to the current price. Otherwise, if the difference is larger than the current maximum profit, update the maximum profit. This works because the optimal strategy is to buy at the lowest price encountered so far and sell at the highest price that appears after it. Edge cases include a single-element vector (profit 0), an already decreasing list (profit 0), and duplicates (profit can be 0 or positive if a later price is higher). Time complexity is O(n) with O(1) auxiliary space.

#include <vector>
#include <algorithm>

// Given a non-empty vector of daily prices, return the maximum profit achievable
// by buying on one day and selling on a later day. If no profit is possible, return 0.
int maxProfit(const std::vector<int>& prices) {
    int min_price = prices.front();
    int best_profit = 0;

    for (int price : prices) {
        // If current price is lower than all seen so far, it's the best buying point.
        if (price < min_price) {
            min_price = price;
        } else {
            // Otherwise, check profit if we sell today after buying at min_price.
            best_profit = std::max(best_profit, price - min_price);
        }
    }
    return best_profit;
}

#include <cassert>
#include <vector>

int main() {
    assert(maxProfit({7,1,5,3,6,4}) == 5);
    assert(maxProfit({7,6,4,3,1}) == 0);
    assert(maxProfit({1}) == 0);
    assert(maxProfit({1,2,3,4,5}) == 4);
    assert(maxProfit({5,5,5,5}) == 0);
    assert(maxProfit({3,2,6,5,0,3}) == 4);
    assert(maxProfit({2,4,1}) == 2);
    assert(maxProfit({1,2}) == 1);
    assert(maxProfit({2,1}) == 0);
    assert(maxProfit({3,3,5,0,0,3,1,4}) == 4);
    return 0;
}
