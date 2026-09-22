/*
Write a C++ function `longestIncreasingSubsequence` that takes a vector of integers by const reference and returns the length of the longest strictly increasing subsequence (LIS). A subsequence is obtained by deleting zero or more elements from the original sequence without changing the order of the remaining elements. The subsequence must be strictly increasing, meaning each element is greater than the previous one. The input vector may be empty (return 0), may contain duplicates (each value cannot be reused in the subsequence if it equals the previous), and may contain negative numbers. The function should work efficiently for vectors up to 10,000 elements. You must provide only the function definition (no main) with appropriate includes and const-correctness.
*/

#include <vector>
#include <algorithm>

// Returns the length of the longest strictly increasing subsequence.
int longestIncreasingSubsequence(const std::vector<int>& nums) {
    // patience-sorting based solution
    std::vector<int> tails;
    for (int x : nums) {
        auto it = std::lower_bound(tails.begin(), tails.end(), x);
        if (it == tails.end()) {
            tails.push_back(x);
        } else {
            *it = x;
        }
    }
    return static_cast<int>(tails.size());
}

#include <cassert>
#include <vector>
#include <iostream>

int longestIncreasingSubsequence(const std::vector<int>& nums);

int main() {
    assert(longestIncreasingSubsequence({10, 9, 2, 5, 3, 7, 101, 18}) == 4);
    assert(longestIncreasingSubsequence({0, 1, 0, 3, 2, 3}) == 4);
    assert(longestIncreasingSubsequence({7, 7, 7, 7, 7, 7, 7}) == 1);
    assert(longestIncreasingSubsequence({}) == 0);
    assert(longestIncreasingSubsequence({1, 2, 3, 4, 5}) == 5);
    assert(longestIncreasingSubsequence({5, 4, 3, 2, 1}) == 1);
    assert(longestIncreasingSubsequence({-5, -1, -10, 0, 3}) == 4);
    assert(longestIncreasingSubsequence({1, 3, 6, 7, 9, 4, 10, 5, 6}) == 5);
    assert(longestIncreasingSubsequence({1, 1, 1, 2, 2, 3}) == 3);
    assert(longestIncreasingSubsequence({3, 1, 2, 1, 8, 5, 6}) == 4);
    std::cout << "All tests passed!\n";
    return 0;
}

// The classic solution uses dynamic programming with `dp[i]` = length of LIS ending at index `i`. Initialize `dp[i] = 1` for all `i` because any single element is a valid increasing subsequence. Then for each `i` from 1 to n-1, check all previous indices `j < i`, if `nums[j] < nums[i]`, update `dp[i] = max(dp[i], dp[j] + 1)`. The answer is the maximum value across all `dp[i]`. This runs in O(n^2) time and O(n) space. For better efficiency with larger inputs, use a patience-sorting inspired approach: maintain a vector `tails` where `tails[k]` is the smallest possible tail value for an increasing subsequence of length `k+1`. For each number, use binary search (lower_bound) to find the first tail that is >= number; if found, replace it; otherwise append. This yields O(n log n) time and O(n) space. Edge cases: empty vector returns 0; all equal elements returns 1 (since strictly increasing requires >, so you cannot include equal consecutive elements); decreasing sequence returns 1. For duplicates, using `lower_bound` works because strictly increasing means you can’t take two equal values, so you replace the first tail that is >= number.
