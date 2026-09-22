// Write a C++ function `int houseRobberCircular(const std::vector<int>& nums)` that solves the House Robber II problem: given a circular array of non-negative integers representing money in houses, return the maximum amount of money you can rob tonight without robbing two adjacent houses (including the constraint that the first and last houses are adjacent). The input vector has length at least 1.

#include <cassert>
#include <vector>

int main() {
    // Basic test: single house
    assert(houseRobberCircular({5}) == 5);

    // Two houses: cannot rob both, choose max
    assert(houseRobberCircular({2, 3}) == 3);
    assert(houseRobberCircular({10, 1}) == 10);

    // Three houses: circular, cannot rob adjacent, so max is max(nums) because robbing one house is always allowed
    assert(houseRobberCircular({1, 2, 3}) == 3);

    // Four houses: typical circular case
    // Options: rob 1+3 = 6, or 2+4 = 6 → max 6
    assert(houseRobberCircular({1, 2, 3, 4}) == 6);

    // Five houses: case from the given snippet logic
    // Exclude first: rob houses 2,4 = 400+100 = 500; exclude last: rob 1,3,5 = 300+200+50 = 550 → max 550
    assert(houseRobberCircular({300, 400, 200, 100, 50}) == 550);

    // All zeros
    assert(houseRobberCircular({0, 0, 0, 0}) == 0);

    // Large values and alternating pattern
    // Exclude first: rob indexes 2,4 = 5+7 = 12; exclude last: rob 1,3,5 = 4+6+8 = 18
    assert(houseRobberCircular({4, 5, 6, 7, 8}) == 18);

    // Two equal values
    assert(houseRobberCircular({7, 7}) == 7);

    // Duplicate pattern, circular effect: 3,2,3,2 → Exclude first: 2+2=4; exclude last: 3+3=6
    assert(houseRobberCircular({3, 2, 3, 2}) == 6);

    return 0;
}

#include <vector>
#include <algorithm>

// Returns the maximum money that can be robbed from a circular array of houses
// where adjacent houses (including first and last) cannot both be robbed.
int houseRobberCircular(const std::vector<int>& nums) {
    const int n = static_cast<int>(nums.size());
    if (n == 1) return nums[0];
    if (n == 2) return std::max(nums[0], nums[1]);

    // Helper lambda for linear house robber on a range [start, end] inclusive.
    auto linearRob = [&nums](int start, int end) {
        const int len = end - start + 1;
        std::vector<int> dp(len, 0);
        for (int i = 0; i < len; ++i) {
            const int value = nums[start + i];
            if (i == 0) dp[i] = value;
            else if (i == 1) dp[i] = std::max(dp[0], value);
            else if (i == 2) dp[i] = dp[0] + value;
            else dp[i] = value + std::max(dp[i-2], dp[i-3]);
        }
        return dp[len-1];
    };

    // Case 1: exclude last house (rob from 0 to n-2).
    // Case 2: exclude first house (rob from 1 to n-1).
    return std::max(linearRob(0, n-2), linearRob(1, n-1));
}

// The key challenge is the circular adjacency between the first and last houses. A standard linear House Robber DP can be applied twice, excluding either the first or the last house, and taking the maximum of the two results. For each linear subproblem, define `dp[i]` as the maximum money up to house index `i`. The recurrence is `dp[i] = nums[i] + max(dp[i-2], dp[i-3])` (or `dp[i] = nums[i]` for the first two houses), because you cannot rob adjacent houses. This handles skips of one or two houses. When the array has only one element, the answer is that element because there is no adjacency issue. When it has two, you choose the larger of the two. For the circular case, run the linear DP on `[0, n-2]` and on `[1, n-1]`, and take the maximum. Edge cases: `n==1` returns `nums[0]`; `n==2` returns `max(nums[0], nums[1])`; for `n>=3`, both subproblems are non-empty. Time complexity is O(n) per subproblem, so O(n) total; space complexity is O(1) if we optimize the DP to only track the last three values, but the reference solution uses an O(n) vector for clarity.
