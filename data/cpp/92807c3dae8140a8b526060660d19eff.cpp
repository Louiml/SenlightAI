Write a C++ function that solves the classic "Rod Cutting" problem: given a rod of length `n` (where `n` equals the number of price entries) and a price array `price[0..n-1]` where `price[i]` is the price for a rod piece of length `i+1`, determine the maximum revenue obtainable by cutting the rod into pieces and selling them. The function should take a `const std::vector<int>&` representing the prices and return the maximum revenue as an `int`. The input prices are all positive integers. Cuts are free and unlimited; the rod may be left uncut (selling the whole rod) or divided into any number of pieces (including multiple pieces of the same length). The solution must be efficient using dynamic programming with space optimization, and the function must be `const`-correct (i.e., it does not modify the input vector).

The problem is a classic unbounded knapsack variant where the "item" is a rod piece of length `L` (from 1 to `n`) with value `price[L-1]`, and the capacity is the rod length `n`. We can take any piece multiple times. The DP state is `dp[length]` = maximum revenue for a rod of exactly `length`. Base case: for length 0, revenue is 0; but since the rod length starts at 1, we handle the first piece (length 1) directly. The recurrence: for each piece length `rodLength` from 1 to `n`, and for each current rod length `len` from `rodLength` to `n`, update `dp[len] = max(dp[len], price[rodLength-1] + dp[len-rodLength])`. This is the standard bottom-up tabulation. Alternatively, the snippet uses a 1D array with iterative updates in increasing `len` order, which allows unlimited use of each piece (unbounded). The time complexity is O(n²) because we iterate over `n` piece lengths and for each, over up to `n` rod lengths. The space complexity is O(n) for the 1D DP array. Edge cases: if the input vector is empty (though problem statement says prices are positive and likely non-empty, we can handle gracefully by returning 0), or if prices are very large, ensure no integer overflow (though typical constraints won't cause overflow). The algorithm correctly handles the case where cutting is not beneficial (e.g., price[0] >= all other per-length ratios).

#include <vector>
#include <algorithm>
#include <cstddef>

// Computes the maximum revenue obtainable by cutting a rod of length n,
// where n = price.size(), and price[i] is the price of a piece of length i+1.
// Uses bottom-up DP with a 1D array (space-optimized unbounded knapsack).
int maxRodCuttingRevenue(const std::vector<int>& price) {
    const std::size_t n = price.size();
    if (n == 0) {
        return 0;
    }

    // dp[len] = max revenue for a rod of length len (0 <= len <= n)
    std::vector<int> dp(n + 1, 0);

    // Base case: for a rod of length 0, revenue is 0 (already set).

    // Iterate over each possible piece length (1 to n).
    // Notice: for unbounded items, we iterate lengths in increasing order.
    for (std::size_t pieceLength = 1; pieceLength <= n; ++pieceLength) {
        const int piecePrice = price[pieceLength - 1];
        for (std::size_t len = pieceLength; len <= n; ++len) {
            dp[len] = std::max(dp[len], piecePrice + dp[len - pieceLength]);
        }
    }

    return dp[n];
}

#include <cassert>
#include <vector>

// The solution function is declared above, but for testing we include it here.
// In a real scenario, the function is already defined.
int maxRodCuttingRevenue(const std::vector<int>& price) {
    const std::size_t n = price.size();
    if (n == 0) return 0;
    std::vector<int> dp(n + 1, 0);
    for (std::size_t pieceLength = 1; pieceLength <= n; ++pieceLength) {
        const int piecePrice = price[pieceLength - 1];
        for (std::size_t len = pieceLength; len <= n; ++len) {
            dp[len] = std::max(dp[len], piecePrice + dp[len - pieceLength]);
        }
    }
    return dp[n];
}

int main() {
    // Basic case from the snippet: prices = {2,5,7,8,10}, n=5
    // Best: cut into three pieces of length 1 (3*2=6) and one of length 2? Actually optimum is 12: two pieces of length 2 (5+5=10) and one of length 1 (2) = 12; or five pieces of length 1 = 10. So max is 12.
    assert(maxRodCuttingRevenue({2,5,7,8,10}) == 12);

    // Single piece: sell the whole rod.
    assert(maxRodCuttingRevenue({1}) == 1);
    assert(maxRodCuttingRevenue({42}) == 42);

    // All pieces same per-length value (price[i] = i+1), so no benefit cutting: whole rod is max.
    assert(maxRodCuttingRevenue({1,2,3,4}) == 4);  // whole rod length 4 price = 4

    // High first price makes cutting worse.
    assert(maxRodCuttingRevenue({10,1,1,1}) == 10); // sell whole rod length 4 = 10

    // Unbounded use: price[0]=3, price[1]=5 (length 1 price 3, length 2 price 5). For n=2, best is two length 1 = 6, or one length 2 = 5, so 6.
    assert(maxRodCuttingRevenue({3,5}) == 6);

    // Larger example: price = {1, 5, 8, 9, 10, 17, 17, 20}, n=8. Known answer from classic problem: 22 (2+2+2+2? Actually 8 length: 5+5+5+5=20? Let's compute: length 6=17, length 2=5, total 22; or length 8=20, but 22 is better).
    assert(maxRodCuttingRevenue({1,5,8,9,10,17,17,20}) == 22);

    // Edge: empty vector returns 0.
    assert(maxRodCuttingRevenue({}) == 0);

    return 0;
}
