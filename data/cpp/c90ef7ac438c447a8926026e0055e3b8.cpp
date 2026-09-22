Given an array of `n` coin denominations where denomination `a` appears `b` times (i.e., there are `b` copies of coin value `a`, for each pairing), and a target sum `x`, write a C++ function `bool canMakeSum(int n, long long x, const std::vector<std::pair<long long, long long>>& coins)` that returns `true` if the target sum can be formed by selecting any subset of these coins (each coin can be used at most once), and `false` otherwise. Note that `n` is the number of distinct denomination–count pairs, and the total number of coins can be up to 5000, while the target `x` can be up to 10000. The function must use a bottom-up dynamic programming approach (not recursion) to avoid stack overflow and must handle large inputs efficiently.

#include <cassert>
#include <vector>
#include <utility>

// Assume canMakeSum is declared above

int main() {
    // Test 1: Basic case
    std::vector<std::pair<long long, long long>> coins1 = {{2, 3}, {5, 2}}; // 2,2,2,5,5
    assert(canMakeSum(2, 9, coins1) == true);  // 2+2+5 = 9
    assert(canMakeSum(2, 10, coins1) == true); // 5+5 = 10
    assert(canMakeSum(2, 11, coins1) == false);
    assert(canMakeSum(2, 0, coins1) == true);  // empty subset

    // Test 2: Single denomination multiple copies
    std::vector<std::pair<long long, long long>> coins2 = {{3, 4}}; // 3,3,3,3
    assert(canMakeSum(1, 6, coins2) == true);
    assert(canMakeSum(1, 7, coins2) == false);
    assert(canMakeSum(1, 12, coins2) == true);

    // Test 3: Large target greater than total sum
    std::vector<std::pair<long long, long long>> coins3 = {{1, 1}, {2, 1}};
    assert(canMakeSum(2, 4, coins3) == false);

    // Test 4: Duplicate values and target exactly sum of all
    std::vector<std::pair<long long, long long>> coins4 = {{4, 2}, {1, 3}}; // 4,4,1,1,1
    assert(canMakeSum(2, 11, coins4) == true); // 4+4+1+1+1 = 11
    assert(canMakeSum(2, 10, coins4) == true); // 4+4+1+1 = 10
    assert(canMakeSum(2, 9, coins4) == false);

    // Test 5: Single coin large count
    std::vector<std::pair<long long, long long>> coins5 = {{7, 100}};
    assert(canMakeSum(1, 700, coins5) == true);
    assert(canMakeSum(1, 701, coins5) == false);

    // Test 6: No coins (n=0)
    std::vector<std::pair<long long, long long>> coins6;
    assert(canMakeSum(0, 0, coins6) == true);
    assert(canMakeSum(0, 5, coins6) == false);

    return 0;
}

#include <vector>
#include <cstdint>

// Determine if target sum can be made using given coins.
// coins: vector of pairs (denomination, count)
bool canMakeSum(int n, long long x, const std::vector<std::pair<long long, long long>>& coins) {
    // Expand coins into individual values (total count may be large, but manageable)
    std::vector<long long> values;
    for (const auto& p : coins) {
        for (long long i = 0; i < p.second; ++i) {
            values.push_back(p.first);
            // Optional early break if we exceed x? but skip for simplicity
        }
    }

    // DP array: dp[s] = true if sum s is achievable
    std::vector<bool> dp(x + 1, false);
    dp[0] = true;

    for (long long val : values) {
        // Traverse downwards so each coin is used at most once
        for (long long s = x; s >= val; --s) {
            if (dp[s - val]) {
                dp[s] = true;
            }
        }
    }

    return dp[x];
}

// We first expand the distinct denominations into a list of individual coin values. Then we compute a boolean DP array `dp[0..x]` where `dp[s]` is true if sum `s` is achievable. We initialize `dp[0] = true`. For each coin value `val` in the expanded list, we iterate `s` from `x` down to `val` (to avoid reusing the same coin) and set `dp[s] = dp[s] || dp[s - val]`. After processing all coins, the answer is `dp[x]`. Edge cases: if the target is 0, the answer is always true (empty subset). If the sum of all coins is less than `x`, the answer is false (but DP handles it naturally). Complexity: Let `m` be the total number of coins (sum of all `b`). Building the expanded list takes `O(m)` time. The DP has `m * x` states, so time is `O(m * x)` and space is `O(x)` because we only keep one boolean array. The recursion in the original snippet was replaced by an iterative bottom-up approach to avoid stack overflow for large `m`.
