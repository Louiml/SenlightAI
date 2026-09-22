// Write a C++ function that takes a vector of integers representing stock prices on consecutive days and returns the maximum profit that can be achieved by buying on one day and selling on a later day. You may engage in multiple transactions (i.e., you can buy and sell multiple times), but you must sell before you buy again. The function should handle empty vectors and vectors with only one price by returning 0, and it should correctly process both strictly increasing and strictly decreasing price sequences. Return the total profit as an integer; the input vector is not to be modified. The solution must not use any additional data structures beyond a few scalar variables.
The key observation is that in a multiple-transaction scenario, you can capture every positive price increase between consecutive days. If the price goes up from day i-1 to day i, you can profit by buying at day i-1 and selling at day i; summing all such positive differences yields the maximum possible total profit. This works because each profitable step is independent—since you can sell and rebuy on the same day, any sequence of increases can be broken into daily increments without losing profit. Edge cases: an empty vector or a single‑element vector has no transaction, so the profit is 0; a strictly decreasing sequence has no positive differences, so profit remains 0; duplicates do not contribute. The algorithm runs in O(n) time (single pass) and uses O(1) extra space, aside from the input vector itself. No sorting or dynamic programming is needed.
#include <vector>

// Return the maximum profit from multiple stock transactions (buy before sell).
// Each profitable consecutive-day price increase contributes to the total profit.
int maxProfitFromMultipleTransactions(const std::vector<int>& prices) {
    int profit = 0;
    if (prices.size() < 2) {
        return profit;
    }
    for (size_t i = 1; i < prices.size(); ++i) {
        if (prices[i] > prices[i - 1]) {
            profit += prices[i] - prices[i - 1];
        }
    }
    return profit;
}
#include <cassert>
#include <vector>

int maxProfitFromMultipleTransactions(const std::vector<int>& prices);

int main() {
    std::vector<int> empty;
    assert(maxProfitFromMultipleTransactions(empty) == 0);

    std::vector<int> single = {5};
    assert(maxProfitFromMultipleTransactions(single) == 0);

    std::vector<int> increasing = {1, 2, 3, 4, 5};
    assert(maxProfitFromMultipleTransactions(increasing) == 4); // 1+1+1+1

    std::vector<int> decreasing = {5, 4, 3, 2, 1};
    assert(maxProfitFromMultipleTransactions(decreasing) == 0);

    std::vector<int> mixed = {7, 1, 5, 3, 6, 4};
    assert(maxProfitFromMultipleTransactions(mixed) == 7); // (5-1)+(6-3)

    std::vector<int> withDuplicates = {2, 2, 2, 2};
    assert(maxProfitFromMultipleTransactions(withDuplicates) == 0);

    std::vector<int> zigzag = {1, 3, 2, 4, 0, 5, 2, 6};
    assert(maxProfitFromMultipleTransactions(zigzag) == 12); // (3-1)+(4-2)+(5-0)+(6-2)
}
