// Write a C++ function named `maxRobbedAmount` that takes a non-empty vector of non-negative integers representing the amount of money in each house along a street. A robber cannot rob two adjacent houses (houses at index `i` and `i+1`), but may rob houses that are separated by at least one house (indices `i` and `i+2` or further). The function must return the maximum total amount that can be robbed without robbing adjacent houses. The input vector may be of any length from 1 to 10^5, and all values are non-negative (including zero). The function must handle edge cases like a single house or two houses correctly and must be efficient for large inputs.

// The problem is a classic dynamic programming "House Robber" variant. The given snippet has a bug: it unconditionally accesses `dp[1]` even when `nums.size() == 1`, leading to undefined behavior. The correct approach: define `dp[i]` as the maximum amount that can be robbed from the first `i+1` houses (indices 0..i). For each house `i`, either we skip it (then the best is `dp[i-1]`) or we rob it (then we add `nums[i]` to the best from index `i-2`, i.e., `dp[i-2]`). So the recurrence is: `dp[i] = max(dp[i-1], nums[i] + dp[i-2])`. Base cases: `dp[0] = nums[0]` (if size >=1), and for size >=2, `dp[1] = max(nums[0], nums[1])`. The answer is `dp[n-1]`. Edge cases: size 1 returns `nums[0]`; size 2 returns max of the two. If all zeros, the function still works. The time complexity is O(n) with O(n) space, but we can reduce auxiliary space to O(1) by keeping only the last two computed values because the recurrence only uses the previous two. The implementation below uses O(1) space. The given snippet attempts a more complex recurrence that is incorrect and has out-of-bounds access; we replace it with the cleaner, correct DP.

#include <vector>
#include <algorithm> // for std::max

/**
 * Compute the maximum amount that can be robbed from a street of houses
 * represented by the non-negative values in nums, without robbing adjacent houses.
 * @param nums non-empty vector of non-negative integers (house amounts)
 * @return maximum total stolen value
 */
int maxRobbedAmount(const std::vector<int>& nums) {
    int n = nums.size();
    if (n == 1) return nums[0];
    if (n == 2) return std::max(nums[0], nums[1]);

    // dp values for the last two houses:
    int prev2 = nums[0];          // best for houses 0..0
    int prev1 = std::max(nums[0], nums[1]); // best for houses 0..1

    for (int i = 2; i < n; ++i) {
        int curr = std::max(prev1, nums[i] + prev2);
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}

#include <cassert>
#include <vector>

int main() {
    // Single house
    assert(maxRobbedAmount({5}) == 5);
    // Two houses
    assert(maxRobbedAmount({2, 3}) == 3);
    assert(maxRobbedAmount({3, 2}) == 3);
    // Standard case from the snippet
    assert(maxRobbedAmount({1, 2, 3, 1}) == 4); // rob 1+3 or 2+1 → max 4
    assert(maxRobbedAmount({2, 7, 9, 3, 1}) == 12); // rob 7+3+1? Actually 2+9+1=12 or 7+3=10 → 12
    // All zeros
    assert(maxRobbedAmount({0, 0, 0, 0}) == 0);
    // Larger values
    assert(maxRobbedAmount({10, 1, 1, 10}) == 20); // rob 10+10
    assert(maxRobbedAmount({5, 3, 4, 11, 2}) == 16); // 5+11=16
    assert(maxRobbedAmount({1, 3, 1, 3, 100}) == 103); // 3+100=103
    // Edge with many alternating numbers
    assert(maxRobbedAmount({3, 2, 3, 2, 3}) == 9); // 3+3+3
    return 0;
}
