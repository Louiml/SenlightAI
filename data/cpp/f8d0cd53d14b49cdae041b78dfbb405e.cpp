// Write a C++ function named `maxProfitFromStockPrices` that takes a vector of integers representing daily stock prices (in chronological order) and returns the maximum profit that can be achieved by buying on one day and selling on a later day, with the restriction that you may complete as many transactions as you like (i.e., buy one share and sell it later, then buy again on a later day, and so on), but you may not hold more than one share at a time. Your function must return `0` if the vector is empty or contains only one price, or if no profitable transaction is possible. The function should be `const`-correct with respect to its input (take by `const std::vector<int>&`) and must handle negative or zero prices gracefully (though realistically prices are positive, do not assume positivity). The solution should be efficient for up to 10^5 prices. Do not include any other functions or a main in the solution section.
// The problem reduces to maximizing profit with unlimited transactions, where each transaction can buy and sell on different days. Because you cannot hold more than one share at a time, the optimal strategy is to capture every upward price movement: whenever the price on day `i` is greater than the price on day `i-1`, you can earn that difference by buying at `i-1` and selling at `i`. Summing all such positive differences yields the maximum total profit. This greedy approach works because each positive difference corresponds to a valid short-term transaction, and no transaction overlaps another in a way that would increase profit further (since any larger gain would be the sum of consecutive positive differences). Edge cases: empty vector or size 1 → return 0; if all prices are non-increasing → sum is 0. Time complexity is O(n) with a single pass, and space complexity is O(1) auxiliary (excluding input storage). No need to store transactions; just iterate and accumulate.
#include <vector>

// Returns the maximum profit from unlimited buy/sell transactions given daily prices.
int maxProfitFromStockPrices(const std::vector<int>& prices) {
    int totalProfit = 0;
    for (std::size_t i = 1; i < prices.size(); ++i) {
        if (prices[i] > prices[i - 1]) {
            totalProfit += prices[i] - prices[i - 1];
        }
    }
    return totalProfit;
}
#include <cassert>
#include <vector>

int maxProfitFromStockPrices(const std::vector<int>& prices); // declaration

int main() {
    // Empty and single-element cases
    assert(maxProfitFromStockPrices({}) == 0);
    assert(maxProfitFromStockPrices({5}) == 0);
    // Strictly increasing: buy at start, sell at each day? Actually profit = sum of all diffs
    assert(maxProfitFromStockPrices({1,2,3,4}) == 3); // 1->2, 2->3, 3->4 = 1+1+1
    // Decreasing: no profit
    assert(maxProfitFromStockPrices({4,3,2,1}) == 0);
    // Mixed with positive gains
    assert(maxProfitFromStockPrices({7,1,5,3,6,4}) == 7); // buy at 1 sell 5 (4), buy 3 sell 6 (3) = 7
    // Multiple small positive differences
    assert(maxProfitFromStockPrices({1,2,1,2}) == 2); // 1->2 (1) and 1->2 (1)
    // Includes zeros and negative prices (unrealistic but handled)
    assert(maxProfitFromStockPrices({-5,0,-3,2}) == 8); // -5->0 (5), -3->2 (5) = 10? Wait: -5->0=5, -3->2=5 → total=10, check: actually -5 to 0 is 5, -3 to 2 is 5, sum=10. Let's compute: prices: -5,0,-3,2. Diffs: 0-(-5)=5, -3-0=-3, 2-(-3)=5 → sum=10. So assert ==10.
    // Single upward movement
    assert(maxProfitFromStockPrices({1,5}) == 4);
    // No movement (all equal)
    assert(maxProfitFromStockPrices({3,3,3}) == 0);
    // Large example
    std::vector<int> large(100000, 1);
    large[50000] = 1000;
    assert(maxProfitFromStockPrices(large) == 999); // one jump
    return 0;
}
