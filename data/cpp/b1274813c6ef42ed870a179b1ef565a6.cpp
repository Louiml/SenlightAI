// Write a standalone C++ function named `maxProfitWithFee` that takes a vector of integers representing daily stock prices and a non-negative integer transaction fee, and returns the maximum profit achievable. You may buy and sell the stock multiple times, but you must sell before buying again, and a fee is charged per completed transaction (one buy + one sell). Assume the input vector is non-empty, prices are positive, and the fee is non-negative. The function should handle cases where no profitable trade exists (returning 0) and where prices are strictly decreasing. Do not include a `main` function or any entry-point wrapper; provide only the function implementation with necessary headers.
The problem is a classic state-machine dynamic programming problem. We maintain two states at each day:  
- `hold`: maximum profit achievable on that day if we currently own one share of stock.  
- `cash`: maximum profit achievable on that day if we currently hold no stock (i.e., we are "free" or have sold everything).  

Initialize `hold = -prices[0]` (buy on day 0) and `cash = 0` (no transactions yet). For each subsequent day, we update:  
- `new_hold = max(hold, cash - prices[i])` – either keep holding from previous day, or buy today (using the best cash state before buying).  
- `new_cash = max(cash, hold + prices[i] - fee)` – either keep the cash state, or sell the stock held from a previous day, paying the fee.  

At the end, `cash` is the answer because we want to end with no stock (final profit is realized only after selling). Edge cases:  
- If the fee is extremely high, the algorithm naturally returns 0 because no trade would be profitable (`hold + price - fee` would be less than `cash`).  
- Strictly decreasing prices: `cash` remains 0, `hold` becomes negative or stays negative, and the final answer is 0.  
Time complexity: \(O(n)\) with one pass over the array. Space complexity: \(O(1)\) since we only keep two integer variables.
#include <vector>
#include <algorithm>

// Returns the maximum profit given a vector of daily prices and a transaction fee.
// Multiple transactions are allowed but must not overlap (sell before buying again).
int maxProfitWithFee(const std::vector<int>& prices, int fee) {
    if (prices.empty()) return 0;

    int hold = -prices[0]; // profit if we own a share today
    int cash = 0;          // profit if we hold no share today

    for (size_t i = 1; i < prices.size(); ++i) {
        int new_hold = std::max(hold, cash - prices[i]);
        int new_cash = std::max(cash, hold + prices[i] - fee);
        hold = new_hold;
        cash = new_cash;
    }

    return cash;
}
#include <cassert>
#include <vector>

int main() {
    // Basic case with profit
    assert(maxProfitWithFee({1, 3, 2, 8, 4, 9}, 2) == 8);

    // No profit possible (fee too high)
    assert(maxProfitWithFee({1, 2, 3}, 5) == 0);

    // Strictly decreasing prices
    assert(maxProfitWithFee({9, 8, 7, 6}, 1) == 0);

    // Single day, no transaction possible
    assert(maxProfitWithFee({5}, 0) == 0);

    // Zero fee, multiple trades
    assert(maxProfitWithFee({1, 2, 3, 4}, 0) == 3);

    // Decreasing then increasing
    assert(maxProfitWithFee({5, 4, 3, 7, 6, 9}, 1) == 7);

    // All equal prices
    assert(maxProfitWithFee({4, 4, 4}, 2) == 0);

    // Large price swing with small fee
    assert(maxProfitWithFee({1, 100, 1, 100}, 5) == 188);

    return 0;
}
