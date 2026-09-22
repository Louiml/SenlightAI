// Given two vectors of integers `nums1` and `nums2`, write a C++ function named `findMaxRepeatedSubarrayLength` that returns the length of the longest contiguous subarray (subsequence of consecutive elements) that appears in both vectors. The subarray must be exactly the same sequence of values and appear consecutively in both input vectors. For example, if `nums1 = {1,2,3,2,1}` and `nums2 = {3,2,1,4,7}`, the longest common subarray is `{3,2,1}` with length 3; the function should return 3. If there is no common subarray, return 0. The function should handle empty vectors, vectors with different lengths, and negative numbers. The implementation must be efficient for large inputs (up to 1000 elements per vector).
#include <cassert>
#include <vector>

int main() {
    std::vector<int> a1 = {1, 2, 3, 2, 1};
    std::vector<int> b1 = {3, 2, 1, 4, 7};
    assert(findMaxRepeatedSubarrayLength(a1, b1) == 3);

    std::vector<int> a2 = {0, 0, 0, 0, 0};
    std::vector<int> b2 = {0, 0, 0};
    assert(findMaxRepeatedSubarrayLength(a2, b2) == 3);

    std::vector<int> a3 = {1, 2, 3};
    std::vector<int> b3 = {4, 5, 6};
    assert(findMaxRepeatedSubarrayLength(a3, b3) == 0);

    std::vector<int> a4 = {};
    std::vector<int> b4 = {1, 2};
    assert(findMaxRepeatedSubarrayLength(a4, b4) == 0);

    std::vector<int> a5 = {-1, -2, -3};
    std::vector<int> b5 = {-2, -3, 0};
    assert(findMaxRepeatedSubarrayLength(a5, b5) == 2);

    std::vector<int> a6 = {7};
    std::vector<int> b6 = {7};
    assert(findMaxRepeatedSubarrayLength(a6, b6) == 1);

    std::vector<int> a7 = {1, 2, 1, 2, 1};
    std::vector<int> b7 = {2, 1, 2};
    assert(findMaxRepeatedSubarrayLength(a7, b7) == 3);

    std::vector<int> a8 = {1, 2, 3, 4};
    std::vector<int> b8 = {4, 3, 2, 1};
    assert(findMaxRepeatedSubarrayLength(a8, b8) == 1);

    std::vector<int> a9 = {1, 2, 3, 4, 5};
    std::vector<int> b9 = {3, 4, 5, 6, 7};
    assert(findMaxRepeatedSubarrayLength(a9, b9) == 3);

    std::vector<int> a10 = {5, 5, 5, 5};
    std::vector<int> b10 = {5, 5, 5, 5, 5};
    assert(findMaxRepeatedSubarrayLength(a10, b10) == 4);

    return 0;
}
#include <vector>
#include <algorithm>

// Returns the length of the longest contiguous subarray common to both input vectors.
int findMaxRepeatedSubarrayLength(const std::vector<int>& nums1, const std::vector<int>& nums2) {
    int m = static_cast<int>(nums1.size());
    int n = static_cast<int>(nums2.size());
    
    if (m == 0 || n == 0) {
        return 0;
    }
    
    std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1, 0));
    int longest = 0;
    
    for (int i = 1; i <= m; ++i) {
        for (int j = 1; j <= n; ++j) {
            if (nums1[i - 1] == nums2[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1] + 1;
                longest = std::max(longest, dp[i][j]);
            } else {
                dp[i][j] = 0;
            }
        }
    }
    
    return longest;
}
// This problem is a classic dynamic programming problem similar to the longest common substring. We define a 2D DP table `dp[i][j]` where `i` ranges from 1 to `nums1.size()` and `j` from 1 to `nums2.size()`. The value `dp[i][j]` represents the length of the longest common suffix of subarrays ending at index `i-1` in `nums1` and `j-1` in `nums2`. The recurrence is: if `nums1[i-1] == nums2[j-1]`, then `dp[i][j] = dp[i-1][j-1] + 1`; otherwise, `dp[i][j] = 0`. We track the maximum value seen in the entire table, which is the answer. The base cases are when either input is empty, the result is 0. Edge cases include duplicate values, overlapping matches, and negative numbers, all handled naturally by the DP. Time complexity is O(m*n) and space complexity is O(m*n) because we store the full table. We can optimize space to O(min(m,n)) using a rolling array, but the straightforward 2D table is acceptable for the given constraints.
