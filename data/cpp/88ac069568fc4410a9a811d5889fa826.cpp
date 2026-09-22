Write a C++ function `int maxCoins(std::vector<int>& nums)` that, given a sequence of balloons represented by integers, returns the maximum number of coins that can be collected by popping every balloon in any order. When a ballooni is popped, you earn `nums[left] * nums[i] * nums[right]` coins, where `nums[left]` and `nums[right]` are the values of the remaining adjacent balloons (or 1 if none exist). The function should handle empty input (returning 0), single-element arrays, and arrays with up to 300 elements containing values from 0 to 100 (inclusive). The order of popping is arbitrary, and you must choose the optimal sequence.

#include <cassert>
#include <vector>

int main() {
    std::vector<int> test1 = {3, 1, 5, 8};
    assert(maxCoins(test1) == 167);
    
    std::vector<int> test2 = {1, 5};
    assert(maxCoins(test2) == 10);
    
    std::vector<int> test3 = {5};
    assert(maxCoins(test3) == 5);
    
    std::vector<int> test4 = {};
    assert(maxCoins(test4) == 0);
    
    std::vector<int> test5 = {1, 2, 3};
    assert(maxCoins(test5) == 12);
    
    std::vector<int> test6 = {0, 0, 0};
    assert(maxCoins(test6) == 0);
    
    std::vector<int> test7 = {1, 1, 1, 1};
    assert(maxCoins(test7) == 8);
    
    std::vector<int> test8 = {2, 4, 6, 8, 10};
    assert(maxCoins(test8) == 544);
    
    std::vector<int> test9 = {7, 9, 8, 0, 7, 1, 3, 5, 5};
    assert(maxCoins(test9) == 1071);
    
    std::vector<int> test10(300, 100); // large stress test with all 100s
    assert(maxCoins(test10) == 300 * 100); // each pop: 1*100*1 = 100, any order same
}

#include <vector>
#include <algorithm>
#include <climits>

// Returns the maximum coins obtainable by popping all balloons in the given array.
// Balloons can be popped in any order; popping index i yields nums[left] * nums[i] * nums[right].
int maxCoins(std::vector<int>& nums) {
    int n = nums.size();
    if (n == 0) return 0;
    
    // Add sentinel 1s at both ends to simplify boundary handling.
    std::vector<int> padded;
    padded.reserve(n + 2);
    padded.push_back(1);
    for (int v : nums) padded.push_back(v);
    padded.push_back(1);
    
    int m = padded.size();
    // dp[i][j] = max coins from popping all balloons in subarray i..j (inclusive) of padded.
    // We only use indices 1..m-2 (original balloons), but allocate full size.
    std::vector<std::vector<int>> dp(m, std::vector<int>(m, 0));
    
    // Process subarrays of increasing length.
    for (int len = 1; len <= n; ++len) {
        for (int i = 1; i + len - 1 <= n; ++i) {
            int j = i + len - 1;
            int best = 0;
            // Choose the last balloon k to pop in this subarray.
            for (int k = i; k <= j; ++k) {
                int coins = padded[i-1] * padded[k] * padded[j+1]
                          + dp[i][k-1] + dp[k+1][j];
                best = std::max(best, coins);
            }
            dp[i][j] = best;
        }
    }
    
    return dp[1][n];
}

// This is a classic interval dynamic programming problem. The key insight is to consider the last balloon popped in any subarray `[i, j]`. If the last balloon popped is at index `k`, then before popping it, all balloons in `[i, k-1]` and `[k+1, j]` are already gone, so the coins obtained from popping `k` are `nums[i-1] * nums[k] * nums[j+1]`, where we conceptually place sentinel 1s at both ends. After that, we recursively solve the two subarrays independently. The recurrence is: `dp[i][j] = max over k in [i,j] of (cost(k) + dp[i][k-1] + dp[k+1][j])`. We include a sentinel 1 at the beginning and end of the input array to simplify boundary conditions, making `i-1` and `j+1` always valid. Edge cases: an empty array returns 0; for a single element, the result is just the element itself (since left and right sentinels are 1×1×value). The time complexity is O(n³) because there are O(n²) subarrays and each has up to O(n) choices for `k`. The space complexity is O(n²) for the DP table.
