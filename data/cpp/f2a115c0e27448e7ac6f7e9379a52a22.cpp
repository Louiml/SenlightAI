/*
Given a vector of positive integers `nums` and a positive integer `target`, write a C++ function `int countCombinations(const std::vector<int>& nums, int target)` that returns the number of possible ways to form a sequence (order matters) that sums to exactly `target`, using the numbers from `nums` any number of times. For example, if `nums = {1,2,3}` and `target = 4`, the valid sequences are `{1,1,1,1}`, `{1,1,2}`, `{1,2,1}`, `{2,1,1}`, `{2,2}`, `{1,3}`, `{3,1}` — that’s 7 ways. The answer may be large; if at any point the intermediate count exceeds `INT_MAX - 1`, the function should return `INT_MAX` (to indicate overflow prevention). The input vector is non-empty, all values are positive, and `target` is positive. You must handle the case where no combination exists (return 0), and the case where `target` is very large but `nums` are large too, avoiding integer overflow in the accumulation. Time complexity must be `O(target * nums.size())` and space complexity `O(target)`.
*/

#include <vector>
#include <climits>

// Returns the number of ordered sequences of nums elements that sum to target.
// If the count would exceed INT_MAX, returns INT_MAX.
int countCombinations(const std::vector<int>& nums, int target) {
    // dp[i] = number of ordered combinations summing to i.
    std::vector<long long> dp(target + 1, 0);
    dp[0] = 1; // One way: empty sequence.

    for (int i = 1; i <= target; ++i) {
        for (int x : nums) {
            if (i >= x) {
                // Add dp[i - x] to dp[i], but cap at INT_MAX to avoid overflow.
                if (dp[i - x] > 0) {
                    if (dp[i] > INT_MAX - dp[i - x]) {
                        dp[i] = INT_MAX;
                    } else {
                        dp[i] += dp[i - x];
                    }
                }
            }
        }
    }

    return static_cast<int>(dp[target]);
}

#include <cassert>
#include <vector>
#include <climits>

int main() {
    // Basic case
    assert(countCombinations({1,2,3}, 4) == 7);
    // Single number
    assert(countCombinations({2}, 6) == 1); // {2,2,2}
    assert(countCombinations({2}, 3) == 0); // impossible
    // Multiple numbers but no way
    assert(countCombinations({3,5}, 4) == 0);
    // Order matters: 1 and 2
    assert(countCombinations({1,2}, 3) == 3); // {1,1,1}, {1,2}, {2,1}
    // Large numbers push to overflow cap
    assert(countCombinations({1}, 40) == 1);
    // Overflow: all ones with target 31 gives 2^31 which overflows int
    assert(countCombinations({1}, 31) == INT_MAX); // 2^31 exceeds int max
    // Multiple large values
    assert(countCombinations({1,2}, 30) == INT_MAX); // 2^30 + ... exceeds
    // target=1 with one number
    assert(countCombinations({1}, 1) == 1);
    // with nums containing larger than target
    assert(countCombinations({5}, 3) == 0);
    return 0;
}

// The solution is a classic dynamic programming problem where `dp[i]` stores the number of sequences that sum to `i`. Initialize `dp[0] = 1` (one way to make sum 0: use nothing). For each sum `i` from 1 to `target`, iterate over each candidate number `x` in `nums`. If `x <= i`, then any sequence summing to `i - x` can be extended by appending `x` to create a sequence summing to `i`. So we add `dp[i - x]` to `dp[i]`. Since order matters, when we process sums in increasing order and iterate over `nums` inside, each distinct ordering is counted separately (because the last element is considered as each possible `x`, and the prefix is `dp[i - x]`). Edge cases: if `dp[i - x]` is already `INT_MAX`, then adding it could overflow, so we cap at `INT_MAX`. Also, if `dp[i]` is already `INT_MAX`, we keep it. If no combination exists, `dp[target]` remains 0. Time complexity is `O(target * n)` where `n = nums.size()`, space is `O(target)`. The overflow check uses `if (dp[i] > INT_MAX - dp[i - x]) dp[i] = INT_MAX; else dp[i] += dp[i - x];` to avoid signed overflow.
