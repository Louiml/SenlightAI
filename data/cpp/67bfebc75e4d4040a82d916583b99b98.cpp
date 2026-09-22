Write a C++ function that takes a vector of positive integers (coin values) and returns a vector of all possible sums (greater than 0) that can be formed using any subset of the coins (each coin can be used at most once). The function must return the sums in increasing order. Assume the input vector may be empty, may contain duplicates, and the maximum possible sum is bounded by 100,000. The function signature is `std::vector<int> possibleSums(const std::vector<int>& coins)`. The function should compute all subset sums, not just whether a particular sum is reachable.

This is a classic subset-sum reachability problem solved via dynamic programming. Define a boolean DP table `dp[i][s]` where `i` ranges from 0 to `n` (number of coins considered) and `s` ranges from 0 to `maxSum` (sum of all coins, but capped at 100,000 for safety). `dp[i][s]` is true if there exists a subset of the first `i` coins that sums to exactly `s`. The base case is `dp[0][0] = true` (empty subset sum 0). For each coin, we either skip it (`dp[i][s] = dp[i-1][s]`) or include it (`dp[i][s] = true` if `s - coin >= 0` and `dp[i-1][s-coin]` is true). After filling the table, collect all `s` from 1 to `maxSum` where `dp[n][s]` is true and return them in order. Important edge cases: an empty coin list yields an empty result; duplicate coin values are handled naturally by the DP because subsets are distinct by indices; the maximum sum is the sum of all coins, but to avoid out-of-bounds we allocate 100,001 columns. Time complexity is O(n * maxSum), and space complexity is O(n * maxSum), which is acceptable given constraints. We can optimize space to O(maxSum) by using a 1D array iterating backwards, but a 2D table is simpler for clarity.

#include <vector>
#include <algorithm>

// Returns all possible subset sums (positive) of the given coin values.
std::vector<int> possibleSums(const std::vector<int>& coins) {
    // Total sum of coins; if coins empty, sum is 0.
    int totalSum = 0;
    for (int value : coins) {
        totalSum += value;
    }
    // Cap max sum to 100000 to match problem constraint.
    const int maxSum = std::min(totalSum, 100000);

    // DP table: dp[i][s] = true if first i coins can make sum s.
    // Use vector of vectors for clarity.
    std::vector<std::vector<bool>> dp(coins.size() + 1, std::vector<bool>(maxSum + 1, false));
    dp[0][0] = true;

    for (size_t i = 1; i <= coins.size(); ++i) {
        int currentCoin = coins[i - 1];
        for (int s = 0; s <= maxSum; ++s) {
            // Skip current coin
            dp[i][s] = dp[i - 1][s];
            // Include current coin if possible
            if (s >= currentCoin && dp[i - 1][s - currentCoin]) {
                dp[i][s] = true;
            }
        }
    }

    // Collect all reachable sums > 0
    std::vector<int> result;
    for (int s = 1; s <= maxSum; ++s) {
        if (dp[coins.size()][s]) {
            result.push_back(s);
        }
    }
    return result;
}

#include <cassert>
#include <vector>

// Declare the solution function (in real usage, include the header).
std::vector<int> possibleSums(const std::vector<int>& coins);

int main() {
    // Empty input
    assert(possibleSums({}) == std::vector<int>());

    // Single coin
    assert(possibleSums({5}) == std::vector<int>({5}));

    // Two distinct coins
    assert(possibleSums({1, 2}) == std::vector<int>({1, 2, 3}));

    // Duplicate coins
    assert(possibleSums({3, 3}) == std::vector<int>({3, 6}));

    // Larger set with overlapping sums
    assert(possibleSums({1, 2, 3}) == std::vector<int>({1, 2, 3, 4, 5, 6}));

    // Coins that sum to exactly 100000 (max limit)
    std::vector<int> bigCoins(1000, 100); // sum = 100000
    auto bigResult = possibleSums(bigCoins);
    assert(bigResult.size() == 1000);
    assert(bigResult.front() == 100);
    assert(bigResult.back() == 100000);

    // Check ordering and no duplicates
    auto result = possibleSums({2, 2, 3});
    assert(result == std::vector<int>({2, 3, 4, 5, 7}));

    return 0;
}
