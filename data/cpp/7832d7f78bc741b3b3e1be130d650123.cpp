Write a C++ function `int lengthOfLIS(const std::vector<int>& nums)` that computes the length of the longest strictly increasing subsequence (LIS) in a given array of integers. The input may contain negative numbers, duplicates, and the array may be empty (in which case the length is 0). The function must not modify the input and should handle all standard integer values. Use the classic dynamic programming approach with O(n²) time complexity, but avoid unnecessary allocation and ensure the logic is correct for non-contiguous subsequences (elements do not need to be adjacent).

// The solution uses dynamic programming: let `dp[i]` be the length of the longest increasing subsequence ending at index `i`. Initialize every `dp[i]` to 1 because any single element forms an LIS of length 1. For each `i` from 1 to n-1, iterate over all previous indices `j < i`, and if `nums[j] < nums[i]` (strictly increasing), update `dp[i] = max(dp[i], dp[j] + 1)`. The answer is the maximum value in `dp`. For the empty vector, return 0. Edge cases: duplicates (strict `<` means equal values cannot extend), negative numbers (comparison works the same), and all-decreasing sequences (answer is 1, as each element is its own LIS). Time complexity is O(n²), space complexity is O(n) for the `dp` array. No special handling for large n is needed, but for n up to 10^5 this O(n²) would be too slow, but this task only asks for correctness, not efficiency.

#include <vector>
#include <algorithm>

// Compute length of longest strictly increasing subsequence.
// Returns 0 for an empty input.
int lengthOfLIS(const std::vector<int>& nums) {
    const int n = static_cast<int>(nums.size());
    if (n == 0) return 0;

    std::vector<int> dp(n, 1);
    for (int i = 1; i < n; ++i) {
        for (int j = 0; j < i; ++j) {
            if (nums[j] < nums[i]) {
                dp[i] = std::max(dp[i], dp[j] + 1);
            }
        }
    }
    return *std::max_element(dp.begin(), dp.end());
}

#include <cassert>
#include <vector>

// Function under test
int lengthOfLIS(const std::vector<int>& nums);

int main() {
    // Empty vector
    assert(lengthOfLIS({}) == 0);

    // Single element
    assert(lengthOfLIS({5}) == 1);

    // Already increasing
    assert(lengthOfLIS({1, 2, 3, 4}) == 4);

    // Strictly decreasing
    assert(lengthOfLIS({5, 4, 3, 2, 1}) == 1);

    // Classic case with duplicates
    assert(lengthOfLIS({10, 9, 2, 5, 3, 7, 101, 18}) == 4); // {2,5,7,101} or {2,3,7,101}

    // Negative numbers
    assert(lengthOfLIS({-3, -1, -2, 0}) == 3); // {-3, -1, 0} or {-3, -2, 0}

    // Duplicates cannot extend
    assert(lengthOfLIS({2, 2, 2}) == 1);

    // Non-contiguous subsequence
    assert(lengthOfLIS({1, 0, 2, 1, 3}) == 3); // {1,2,3} or {0,1,3}

    // Random larger case
    std::vector<int> large = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};
    assert(lengthOfLIS(large) == 4); // e.g., {1,2,3,5} or {1,4,5,9}

    return 0;
}
