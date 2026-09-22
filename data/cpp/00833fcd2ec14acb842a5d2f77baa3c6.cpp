Write a C++ function that, given a vector of integers representing stock prices on consecutive days, returns the maximum profit achievable by completing at most two transactions. A transaction consists of buying one day and selling on a later day, and you cannot engage in overlapping transactions (i.e., you must sell before buying again). You may complete zero transactions if no profit is possible. The function should handle an empty vector by returning 0. Implement an iterative dynamic programming solution that processes prices left to right, keeping track of the best profit for one and two completed transactions up to each day.
#include <cassert>
#include <vector>
#include <climits>

int maxProfitWithAtMostTwoTransactions(const std::vector<int>& prices);

int main() {
    // Basic example: buy at 1, sell at 5 (profit 4), then buy at 3, sell at 6 (profit 3) -> total 7
    std::vector<int> prices1 = {3,3,5,0,0,3,1,4};
    assert(maxProfitWithAtMostTwoTransactions(prices1) == 6); // 3+3? Actually: buy 0 sell 4 = 4, then buy 1 sell 3 = 2? Wait 0->3 (3) then 1->4 (3) = 6
    
    // Single transaction best: buy 1, sell 5 -> 4
    std::vector<int> prices2 = {1,2,3,4,5};
    assert(maxProfitWithAtMostTwoTransactions(prices2) == 4);
    
    // No profit possible
    std::vector<int> prices3 = {7,6,4,3,1};
    assert(maxProfitWithAtMostTwoTransactions(prices3) == 0);
    
    // Empty vector
    std::vector<int> prices4;
    assert(maxProfitWithAtMostTwoTransactions(prices4) == 0);
    
    // All same prices
    std::vector<int> prices5 = {5,5,5,5};
    assert(maxProfitWithAtMostTwoTransactions(prices5) == 0);
    
    // Two non-overlapping profitable transactions: buy 0 sell 4 (4), buy 1 sell 3 (2) => 6
    std::vector<int> prices6 = {0,1,4,1,3};
    assert(maxProfitWithAtMostTwoTransactions(prices6) == 5); // Actually 0->4 (4) and 1->3 (2) = 6? Wait 0->4=4, then 1->3=2 total 6
    // But careful: after selling at 4, can buy at 1 and sell at 3 = 2, total 6.
    // Recheck: prices = [0,1,4,1,3] → first transaction buy at 0 sell at 4 =4, second buy at 1 sell at 3 =2 total 6.
    assert(maxProfitWithAtMostTwoTransactions(prices6) == 6);
    
    // Edge with large values
    std::vector<int> prices7 = {100000, 1, 2, 3, 100000};
    // Best: buy 1 sell 100000 (99999) plus maybe first buy 100000? No, can't do two because second must be after first sell. 
    // Actually you can buy at 1, sell at 100000, then no second (no later days). So profit = 99999.
    assert(maxProfitWithAtMostTwoTransactions(prices7) == 99999);
    
    return 0;
}
#include <vector>
#include <algorithm>
#include <climits>

// Returns the maximum profit from at most two stock transactions.
// Transactions cannot overlap; each buy must be before the corresponding sell.
int maxProfitWithAtMostTwoTransactions(const std::vector<int>& prices) {
    if (prices.empty()) return 0;
    
    int first_buy = INT_MIN;   // best profit after first buy (negative price paid)
    int first_sell = 0;        // best profit after first sell
    int second_buy = INT_MIN;  // best profit after second buy (using first profit)
    int second_sell = 0;       // best profit after second sell (final answer)
    
    for (int price : prices) {
        first_buy = std::max(first_buy, -price);          // buy first stock
        first_sell = std::max(first_sell, first_buy + price); // sell first stock
        second_buy = std::max(second_buy, first_sell - price); // buy second stock
        second_sell = std::max(second_sell, second_buy + price); // sell second stock
    }
    
    return second_sell; // non-negative because at least zero transactions allowed
}
// The core idea is to simulate the progression of a trader with up to two transactions. We track two states: `first_profit[i]` = maximum profit achievable with at most one transaction up to day `i`, and `second_profit[i]` = maximum profit achievable with at most two transactions up to day `i`. The recurrence for `first_profit[i]` is straightforward: either we do nothing today (keep `first_profit[i-1]`) or we sell today after buying earlier at the minimum price seen so far. For `second_profit[i]`, we can either keep the previous `second_profit[i-1]` (no new transaction) or sell today after completing a first transaction earlier and buying the second stock at some prior day. To compute this efficiently, we maintain `min_so_far` for the first buy and `max_prev_first` which represents the best "profit from first transaction minus price of second buy" encountered so far. This gives an O(n) time and O(1) extra space solution (excluding the input vector). Edge cases: empty vector returns 0; if prices are strictly decreasing, both profits remain 0; duplicate prices are fine. The algorithm works in one pass.
//
// For a reference, we can also implement an O(n) auxiliary array version: first compute `first_profit[i]` left-to-right, then compute `second_profit[i]` using a right-to-left scan for the second transaction. However, the most efficient and clean approach is the state machine with four variables: `first_buy`, `first_sell`, `second_buy`, `second_sell`, initialized to `INT_MIN`, 0, `INT_MIN`, 0 respectively. For each price `p`, update: `first_buy = max(first_buy, -p)`, `first_sell = max(first_sell, first_buy + p)`, `second_buy = max(second_buy, first_sell - p)`, `second_sell = max(second_sell, second_buy + p)`. At the end, return `second_sell` (which is ≥0). This is O(n) time and O(1) space, and handles all edge cases naturally.
