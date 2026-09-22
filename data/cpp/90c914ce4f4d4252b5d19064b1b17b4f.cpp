Given an array of positive integers representing coin values, write a C++ function `subsetSumPossible` that takes a vector of integers and returns a vector of all possible sums that can be formed using any non-empty subset of the coins (each coin can be used at most once). The returned sums should be sorted in ascending order. For example, with coins `{1,2,3}`, the possible sums are `1,2,3,4,5,6`. The function must handle arrays with up to 10^5 elements, where the total sum of all coins does not exceed 2*10^5. The output must exclude the sum 0 (empty subset). If no non-empty subset produces a sum, return an empty vector.

This problem is a classic subset sum reachability check. We use dynamic programming with a boolean 2D table `dp[i][j]` where `dp[i][j]` is `true` if a sum `j` can be formed using the first `i+1` coins (indices 0..i). Initialize `dp[i][0] = true` for all `i` because an empty subset sums to 0. Also, if `a[0] <= targetSum`, set `dp[0][a[0]] = true`. For each coin from index 1 onward and each target sum from 1 to `totalSum`, the recurrence is: `dp[i][j] = dp[i-1][j]` (not taking the current coin) OR `dp[i-1][j-a[i]]` (taking it, if `a[i] <= j`). After filling the table, iterate over `j` from 1 to `totalSum` and collect all `j` where `dp[n-1][j]` is true. Edge cases include duplicates (they are naturally handled), a single coin, and when the total sum is 0 (not possible with positive coins). Time complexity is `O(n * totalSum)` and space is `O(n * totalSum)`. This is acceptable because `totalSum ≤ 2e5` and `n ≤ 1e5` (worst case when all coins are 1). We can optimize space to `O(totalSum)` using a 1D array, but the problem statement does not require it; the reference solution uses the straightforward 2D DP for clarity.

#include <bits/stdc++.h>

// Returns all possible subset sums (non-empty) from the given coins, sorted ascending.
std::vector<int> subsetSumPossible(const std::vector<int>& coins) {
    const int n = static_cast<int>(coins.size());
    if (n == 0) return {};

    int totalSum = 0;
    for (int coin : coins) totalSum += coin;

    // dp[i][j] = true if sum j is reachable using coins[0..i]
    std::vector<std::vector<bool>> dp(n, std::vector<bool>(totalSum + 1, false));

    // Base cases: sum 0 always reachable with empty subset
    for (int i = 0; i < n; ++i) {
        dp[i][0] = true;
    }

    // Base case for first coin
    if (coins[0] <= totalSum) {
        dp[0][coins[0]] = true;
    }

    // Fill DP table
    for (int i = 1; i < n; ++i) {
        for (int j = 1; j <= totalSum; ++j) {
            bool notTaken = dp[i - 1][j];
            bool taken = false;
            if (coins[i] <= j) {
                taken = dp[i - 1][j - coins[i]];
            }
            dp[i][j] = taken || notTaken;
        }
    }

    // Collect all reachable sums > 0
    std::vector<int> result;
    for (int j = 1; j <= totalSum; ++j) {
        if (dp[n - 1][j]) {
            result.push_back(j);
        }
    }
    return result;
}

#include <bits/stdc++.h>
#include <cassert>

// Declare the function (as if from the solution file)
std::vector<int> subsetSumPossible(const std::vector<int>& coins);

int main() {
    // Test 1: simple case
    std::vector<int> coins1 = {1, 2, 3};
    std::vector<int> result1 = subsetSumPossible(coins1);
    std::vector<int> expected1 = {1, 2, 3, 4, 5, 6};
    assert(result1 == expected1);

    // Test 2: single coin
    std::vector<int> coins2 = {5};
    std::vector<int> result2 = subsetSumPossible(coins2);
    std::vector<int> expected2 = {5};
    assert(result2 == expected2);

    // Test 3: duplicates
    std::vector<int> coins3 = {2, 2};
    std::vector<int> result3 = subsetSumPossible(coins3);
    std::vector<int> expected3 = {2, 4};
    assert(result3 == expected3);

    // Test 4: all ones
    std::vector<int> coins4 = {1, 1, 1};
    std::vector<int> result4 = subsetSumPossible(coins4);
    std::vector<int> expected4 = {1, 2, 3};
    assert(result4 == expected4);

    // Test 5: larger sum with no gaps
    std::vector<int> coins5 = {1, 2, 4, 8};
    std::vector<int> result5 = subsetSumPossible(coins5);
    std::vector<int> expected5 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    assert(result5 == expected5);

    // Test 6: empty vector
    std::vector<int> coins6 = {};
    std::vector<int> result6 = subsetSumPossible(coins6);
    assert(result6.empty());

    // Test 7: coins with large values but small count
    std::vector<int> coins7 = {100, 50, 1};
    std::vector<int> result7 = subsetSumPossible(coins7);
    std::vector<int> expected7 = {1, 50, 51, 100, 101, 150, 151};
    assert(result7 == expected7);

    return 0;
}
