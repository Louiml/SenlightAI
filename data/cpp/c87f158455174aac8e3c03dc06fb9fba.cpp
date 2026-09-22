// Write a C++ function `int maxProfitWithAtMostTwoTransactions(const std::vector<int>& prices)` that computes the maximum profit you can achieve from a sequence of daily stock prices, given that you may complete at most two transactions. A transaction consists of buying one share and later selling that share; you cannot hold more than one share at a time (i.e., you must sell before buying again). You may also choose to not perform any transaction. The function should return the maximum profit possible. If the input contains fewer than two prices, or if no profitable transactions exist, return 0.
#include <cassert>
#include <vector>

int main() {
    // Basic cases
    assert(maxProfitWithAtMostTwoTransactions({3,3,5,0,0,3,1,4}) == 6);
    assert(maxProfitWithAtMostTwoTransactions({1,2,3,4,5}) == 4);
    assert(maxProfitWithAtMostTwoTransactions({7,6,4,3,1}) == 0);
    assert(maxProfitWithAtMostTwoTransactions({1}) == 0);
    assert(maxProfitWithAtMostTwoTransactions({}) == 0);

    // Two transactions can be better than one
    assert(maxProfitWithAtMostTwoTransactions({1,2,1,2}) == 2);
    assert(maxProfitWithAtMostTwoTransactions({3,2,6,5,0,3}) == 7);

    // Only one optimal transaction even with capacity 2
    assert(maxProfitWithAtMostTwoTransactions({1,2,3}) == 2);

    // Duplicates and zero prices
    assert(maxProfitWithAtMostTwoTransactions({0,0,0}) == 0);
    assert(maxProfitWithAtMostTwoTransactions({5,5,5,5}) == 0);

    // Larger mixed case
    assert(maxProfitWithAtMostTwoTransactions({2,1,4,5,2,9,7}) == 11);

    return 0;
}
#include <vector>
#include <algorithm>
#include <cstring>

// Computes the maximum profit from at most two stock transactions.
// `prices` is a sequence of daily stock prices.
// Returns the maximum profit, or 0 if no profitable transaction is possible.
int maxProfitWithAtMostTwoTransactions(const std::vector<int>& prices) {
    const int n = static_cast<int>(prices.size());
    if (n < 2) return 0;

    // dp[day][hold][cap] = max profit from day `day` with hold state `hold` and `cap` transactions left.
    std::vector<std::vector<std::vector<int>>> dp(
        n, std::vector<std::vector<int>>(2, std::vector<int>(3, -1)));

    // Recursive helper with memoization.
    // `day` is the current index, `hold` is 1 if we own a share, `cap` is remaining transactions.
    std::function<int(int,int,int)> solve = [&](int day, int hold, int cap) -> int {
        if (day == n || cap == 0) return 0;
        if (dp[day][hold][cap] != -1) return dp[day][hold][cap];

        int profit = 0;
        if (hold) {
            // Option 1: buy at current price and move to next day with hold=0.
            // Option 2: skip buying.
            profit = std::max(-prices[day] + solve(day + 1, 0, cap),
                              solve(day + 1, 1, cap));
        } else {
            // Option 1: sell at current price and consume one transaction.
            // Option 2: skip selling.
            profit = std::max(prices[day] + solve(day + 1, 1, cap - 1),
                              solve(day + 1, 0, cap));
        }
        dp[day][hold][cap] = profit;
        return profit;
    };

    return solve(0, 1, 2); // Start with no share, allow up to 2 transactions.
}
// The problem is a classic dynamic programming exercise where we track the state as `(day_index, hold_state, transactions_left)`. Here, `hold_state` is `1` if you currently own a share (and are allowed to buy), and `0` if you do not own a share (and are allowed to sell). The capacity is the number of remaining transactions, starting at `2`. At each day, you have two choices: do nothing and move to the next day, or perform the allowed action (buy if you don't own, sell if you own). Buying deducts the current price from your profit, while selling adds the current price and decrements the transaction count. The recursion stops when you've reached the end of the price list or when you have no transactions left. To avoid recomputation, we memoize results in a 3D DP table of size `n x 2 x 3`, initialized to `-1`. Base cases return 0 for out-of-bounds or exhausted capacity. The time complexity is \(O(n \cdot 2 \cdot 3) = O(6n)\), which simplifies to \(O(n)\), and space complexity is \(O(6n)\) for the memo table (plus recursion stack depth up to `n`). Edge cases include empty or single‑element arrays (return 0), strictly decreasing prices (return 0), and cases where only one transaction is optimal even though two are allowed.
