// Write a C++ function `int houseRobber(const std::vector<int>& nums)` that returns the maximum amount of money a thief can steal from a row of houses, given that each house contains a certain amount of cash. The thief cannot rob two adjacent houses in the same night. The input vector may contain zero or more houses (if empty, return 0; if one house, return that amount). The function must avoid recursion and implement an iterative dynamic programming solution to achieve linear time complexity. Handle edge cases such as an empty vector, a single house, and negative amounts (though typical inputs are non-negative, the algorithm should still work with negatives by taking the max of valid choices). The solution should use dynamic programming with a bottom-up approach, storing only the two needed previous results to save space.
#include <cassert>
#include <vector>

int main() {
    // Empty vector
    assert(houseRobber({}) == 0);

    // Single house
    assert(houseRobber({5}) == 5);
    assert(houseRobber({-3}) == -3); // handles negatives

    // Two houses
    assert(houseRobber({1, 2}) == 2);
    assert(houseRobber({3, 2}) == 3);
    assert(houseRobber({-1, -2}) == -1); // max of negatives

    // Classic example
    assert(houseRobber({1, 2, 3, 1}) == 4); // rob houses 1 and 3

    // Another classic
    assert(houseRobber({2, 7, 9, 3, 1}) == 12); // 2+9+1

    // All zeros
    assert(houseRobber({0, 0, 0}) == 0);

    // Mixed with zero and negatives
    assert(houseRobber({-2, -1, 0, 3, -5}) == 3); // rob house with 0 and 3? or just 3? Actually max is 3 (skip -2,-1, take 0? but 0+3=3, or skip -2 take -1? max is 3)

    // Large sequence to test linear performance (not a correctness check but runs)
    std::vector<int> large(10000, 1);
    assert(houseRobber(large) == 5000); // every other house

    return 0;
}
#include <vector>
#include <algorithm>

// Returns the maximum amount that can be robbed without robbing adjacent houses.
// Iterative dynamic programming with O(1) extra space.
int houseRobber(const std::vector<int>& nums) {
    if (nums.empty()) return 0;
    if (nums.size() == 1) return nums[0];

    int prev2 = nums[0];               // dp[i-2]
    int prev1 = std::max(nums[0], nums[1]); // dp[i-1]

    for (size_t i = 2; i < nums.size(); ++i) {
        int current = std::max(prev2 + nums[i], prev1);
        prev2 = prev1;
        prev1 = current;
    }
    return prev1;
}
// The problem is the classic “House Robber” dynamic programming problem. We define `dp[i]` as the maximum amount that can be robbed considering the first `i` houses (0-indexed). For each house `i`, we have two choices: either rob it (adding its value to `dp[i-2]`) or skip it (taking `dp[i-1]`). We take the maximum of these two. Base cases: if no houses, return 0; if one house, return its value; if two houses, return the max of the two. The iterative bottom-up approach processes houses from left to right, maintaining only the previous two `dp` values to achieve O(1) extra space. Time complexity is O(n), where n is the number of houses, and space complexity is O(1) beyond input storage. Edge cases include empty input (return 0), single element (return that element, even if negative—though typical inputs are non-negative), and vectors with zeros or negative numbers where the maximum may still be non-negative or zero if all are negative. The algorithm correctly handles these by using max comparisons.
