// Write a C++ function named `minimumCoins` that takes a vector of positive integers representing coin denominations and an integer amount, and returns the minimum number of coins needed to make up that amount. If the amount cannot be made up exactly with the given denominations, return -1. You may assume the amount is non-negative and the coin vector has at least one element. The function should solve the problem even when coin denominations are not canonical (i.e., greedy selection of the largest coin first does not always give the optimal answer).

// The problem is a classic unbounded knapsack variant: each coin can be used unlimited times. A brute-force greedy approach fails because, for example, with coins {1, 3, 4} and amount 6, greedy gives 4+1+1 (3 coins) but optimal is 3+3 (2 coins). The correct approach is dynamic programming with memoization (top-down). Define a recursive function `solve(remain)` that returns the minimum coins needed to form `remain`. Base case: `remain == 0` returns 0. For any positive remain, we iterate over all coins, and if a coin is not larger than remain, we compute `1 + solve(remain - coin)`. The answer for `remain` is the minimum over all such recursive calls. If no coin can be used (i.e., all coins are larger than remain), the result is infinity (represented by a large sentinel). Memoization stores results for each `remain` from 0 to amount to avoid recomputation. Edge cases: when amount == 0, the answer is 0 (no coins needed); when no combination sums exactly to the amount, the recursive function returns infinity, and the outer function returns -1. Time complexity is O(amount * n), where n is the number of coin denominations, since each state (0..amount) is computed once and for each state we iterate over all coins. Space complexity is O(amount) for the memoization table plus O(amount) for the recursion stack in the worst case.

#include <vector>
#include <algorithm>
#include <climits>
#include <cstring>

// Function: minimumCoins
// Computes the minimum number of coins (unlimited supply) to make exactly 'amount'.
// Returns -1 if it's impossible.
int minimumCoins(const std::vector<int>& coins, int amount) {
    if (amount == 0) return 0; // Trivial case

    // dp[i] = minimum coins for amount i, -1 means not computed yet
    std::vector<int> dp(amount + 1, -1);
    
    // Recursive lambda with memoization
    std::function<int(int)> solve = [&](int rem) -> int {
        if (rem == 0) return 0;
        if (dp[rem] != -1) return dp[rem];
        
        int best = INT_MAX;
        for (int coin : coins) {
            if (coin <= rem) {
                int sub = solve(rem - coin);
                if (sub != INT_MAX) {
                    best = std::min(best, 1 + sub);
                }
            }
        }
        dp[rem] = best;
        return best;
    };
    
    int result = solve(amount);
    return (result == INT_MAX) ? -1 : result;
}

#include <cassert>
#include <vector>

int main() {
    // Basic cases
    assert(minimumCoins({1, 2, 5}, 11) == 3);     // 5+5+1
    assert(minimumCoins({2}, 3) == -1);           // impossible
    assert(minimumCoins({1}, 0) == 0);            // zero amount
    assert(minimumCoins({1, 3, 4}, 6) == 2);      // 3+3, greedy would fail
    assert(minimumCoins({5, 7}, 10) == 2);        // 5+5
    assert(minimumCoins({5, 7}, 11) == -1);       // cannot make 11
    assert(minimumCoins({1, 2, 5, 10}, 27) == 4); // 10+10+5+2
    assert(minimumCoins({7, 9}, 14) == 2);        // 7+7
    assert(minimumCoins({3, 5}, 4) == -1);        // neither fits exactly
    assert(minimumCoins({1}, 5) == 5);            // single denomination
}
