Write a C++ function named `maxProfitFromDailyPrices` that accepts a non-empty vector of integers representing daily stock prices in chronological order and returns the maximum profit achievable by buying on one day and selling on a later day, but with the crucial twist: you are allowed to make multiple transactions (buy then sell, then buy again, etc.), as long as you hold at most one share at a time (i.e., you must sell before buying again). If no profit is possible, return 0. The function must be `const`-correct with respect to the input vector.

The greedy strategy works because with unlimited transactions, every time the price rises from day `i-1` to day `i`, you can "collect" that difference by buying on day `i-1` and selling on day `i`. Summing all positive consecutive day-to-day differences yields the maximum total profit, because any profit is the sum of these individual upward movements, and skipping a rise would lose that gain while buying later and selling earlier would reduce profit. Edge cases: a single-element vector yields 0 (no transaction possible); all-declining prices yield 0 (no profitable day pair, and we never incur losses). Time complexity is O(n) for n prices, space complexity is O(1) beyond input storage.

#include <vector>
#include <algorithm>

// Returns maximum profit from multiple buy-sell transactions.
// Prices are given in chronological order; must sell before re-buying.
int maxProfitFromDailyPrices(const std::vector<int>& prices) {
    int totalProfit = 0;
    for (std::size_t i = 1; i < prices.size(); ++i) {
        int gain = prices[i] - prices[i - 1];
        if (gain > 0) {
            totalProfit += gain;
        }
    }
    return totalProfit;
}

#include <cassert>
#include <vector>

int maxProfitFromDailyPrices(const std::vector<int>& prices);

int main() {
    assert(maxProfitFromDailyPrices({7,1,5,3,6,4}) == 7); // 5-1 + 6-3
    assert(maxProfitFromDailyPrices({1,2,3,4,5}) == 4);   // 5-1
    assert(maxProfitFromDailyPrices({5,4,3,2,1}) == 0);   // no profitable trade
    assert(maxProfitFromDailyPrices({1}) == 0);           // only one day
    assert(maxProfitFromDailyPrices({3,3,3,3}) == 0);     // equal prices
    assert(maxProfitFromDailyPrices({2,1,2,1,2}) == 2);   // 2-1 + 2-1
    assert(maxProfitFromDailyPrices({1,10,2,9}) == 16);   // 10-1 + 9-2
    assert(maxProfitFromDailyPrices({10,5,8,3,7}) == 7);  // 8-5 + 7-3
    return 0;
}
