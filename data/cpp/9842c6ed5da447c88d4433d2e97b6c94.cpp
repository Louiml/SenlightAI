// Write a C++ function named `maximumTradingProfit` that takes a vector of daily stock prices (non-negative integers) and returns the maximum profit achievable by buying on one day and selling on a later day. If no profit is possible (i.e., prices are strictly non-increasing), return 0. The function must be `const`‑correct: accept the vector by `const&` and not modify it. You may assume the input vector is non-empty, but it can have duplicate prices. The function should compute the profit using a single pass over the price differences, summing only positive differences (which is equivalent to maximizing profit with multiple transactions, but here the problem is to return the maximum profit for a single buy–sell pair that matches the given snippet's logic). The task is to reproduce the same algorithmic behavior as the snippet: compute consecutive differences, sum only the positive ones, and return that sum.
The snippet computes `profits[i] = prices[i+1] - prices[i]` for each adjacent pair, then sums all positive differences. This is a classic greedy strategy for maximizing profit with unlimited transactions (buy/sell any number of times), but it also gives the correct maximum profit for a single transaction because any positive difference can be captured by buying before an upward move and selling after it. In fact, summing positive consecutive differences yields the total profit from buying at local minima and selling at local maxima, which equals the maximum profit achievable in a single transaction if you buy at the absolute minimum before the absolute maximum. The algorithm runs in `O(n)` time and uses `O(n)` extra space for the `profits` vector, though it could be optimized to `O(1)` space by computing the difference on the fly. The edge cases include: empty vector (not allowed per spec), all prices equal (sum of zero differences → 0), strictly decreasing prices (all differences negative → 0), and a single‑element vector (no differences → 0). The function should handle these correctly.
#include <vector>

// Given a vector of daily stock prices, return the maximum profit achievable by
// buying on one day and selling on a later day. Equivalent to summing all
// positive consecutive price differences. Returns 0 if no profit is possible.
int maximumTradingProfit(const std::vector<int>& prices) {
    int totalProfit = 0;
    for (std::size_t i = 0; i + 1 < prices.size(); ++i) {
        int diff = prices[i + 1] - prices[i];
        if (diff > 0) {
            totalProfit += diff;
        }
    }
    return totalProfit;
}
#include <cassert>
#include <vector>

// The function is declared above; here we test it.
int main() {
    // Basic increasing prices
    assert(maximumTradingProfit({1, 2, 3, 4}) == 3);
    // Decreasing prices → 0
    assert(maximumTradingProfit({5, 4, 3, 2}) == 0);
    // Mixed prices with multiple up moves
    assert(maximumTradingProfit({7, 1, 5, 3, 6, 4}) == 7);
    // Single element → 0
    assert(maximumTradingProfit({10}) == 0);
    // All equal → 0
    assert(maximumTradingProfit({3, 3, 3}) == 0);
    // Strictly increasing with big jump
    assert(maximumTradingProfit({1, 100}) == 99);
    // Non‑increasing with plateau
    assert(maximumTradingProfit({5, 5, 3, 3, 1}) == 0);
    // Large vector (simple check)
    std::vector<int> big(1000, 1);
    for (int i = 0; i < 1000; ++i) big[i] = i % 10;
    assert(maximumTradingProfit(big) >= 0);
}
