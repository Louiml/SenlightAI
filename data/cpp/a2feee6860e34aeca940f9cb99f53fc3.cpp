Write a C++ function `int maximumProfit(const std::vector<int>& prices)` that, given a vector of daily stock prices (where each element represents the price on a given day), returns the maximum profit that can be achieved by buying on one day and selling on a later day. If no profit can be made (prices only decrease or stay flat), return 0. The function must be `const`-correct, handle an empty vector (return 0), and must not modify the input vector.
The simplest correct approach is a brute-force double loop: for each possible buy day `i`, consider every sell day `j` where `j > i` (or `j >= i`, but buying and selling the same day yields zero profit, so it's harmless). Compute the difference `prices[j] - prices[i]` and track the maximum positive difference seen. Initialize the result to 0, so if all differences are negative or zero, the answer stays 0. Edge cases include an empty vector (return 0 immediately), a single element (also 0), and all-decreasing prices (return 0). Time complexity is \(O(n^2)\) where \(n\) is the number of prices, and space complexity is \(O(1)\) besides the input vector. This matches the logic in the given snippet, but we improve naming, const-correctness, and remove the broken `removeDuplicates` call from the snippet.
#include <vector>
#include <algorithm>

// Returns the maximum profit from buying on one day and selling on a later day.
// Returns 0 if no profit is possible.
int maximumProfit(const std::vector<int>& prices) {
    if (prices.empty()) return 0;
    int best = 0;
    int n = static_cast<int>(prices.size());
    for (int buy = 0; buy < n; ++buy) {
        for (int sell = buy + 1; sell < n; ++sell) {
            int profit = prices[sell] - prices[buy];
            best = std::max(best, profit);
        }
    }
    return best;
}
#include <cassert>
#include <vector>

// forward declaration of the solution function
int maximumProfit(const std::vector<int>& prices);

int main() {
    // Example from the snippet: expected max profit is 5 (buy at 1, sell at 6)
    std::vector<int> prices1 = {7, 1, 5, 3, 6, 4};
    assert(maximumProfit(prices1) == 5);

    // Strictly decreasing prices: no profit
    std::vector<int> prices2 = {7, 6, 4, 3, 1};
    assert(maximumProfit(prices2) == 0);

    // Empty vector
    std::vector<int> prices3;
    assert(maximumProfit(prices3) == 0);

    // Only one price
    std::vector<int> prices4 = {10};
    assert(maximumProfit(prices4) == 0);

    // All equal prices
    std::vector<int> prices5 = {5, 5, 5, 5};
    assert(maximumProfit(prices5) == 0);

    // Profit occurs at the end
    std::vector<int> prices6 = {9, 8, 7, 1, 2, 3};
    assert(maximumProfit(prices6) == 2);

    // Large profit with negative values (though prices typically ≥0, function handles it)
    std::vector<int> prices7 = {-3, -1, -2, 0};
    assert(maximumProfit(prices7) == 3);

    return 0;
}
