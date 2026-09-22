/*
Given a vector of pairs where each pair `(a, b)` represents an interval with `a < b`, write a C++ function `int longestChainLength(const std::vector<std::pair<int, int>>& intervals)` that returns the length of the longest chain of intervals such that in the chain, each interval's second value is strictly less than the next interval's first value (i.e., `interval[i].second < interval[i+1].first`). The intervals can be rearranged in any order to form the chain, but each interval can be used at most once. The input intervals are not necessarily sorted, and there may be duplicate intervals. If the input is empty, return `0`.
*/

#include <vector>
#include <algorithm>

// Returns the length of the longest chain of intervals where each interval's
// second value is strictly less than the next interval's first value.
int longestChainLength(const std::vector<std::pair<int, int>>& intervals) {
    if (intervals.empty()) {
        return 0;
    }
    
    // Copy and sort intervals by first, then second.
    std::vector<std::pair<int, int>> arr = intervals;
    std::sort(arr.begin(), arr.end());
    
    const int n = static_cast<int>(arr.size());
    std::vector<int> dp(n, 1);
    
    for (int i = 1; i < n; ++i) {
        for (int j = 0; j < i; ++j) {
            if (arr[j].second < arr[i].first) {
                dp[i] = std::max(dp[i], dp[j] + 1);
            }
        }
    }
    
    int max_length = 0;
    for (int value : dp) {
        max_length = std::max(max_length, value);
    }
    return max_length;
}

#include <cassert>
#include <vector>
#include <utility>

int longestChainLength(const std::vector<std::pair<int, int>>& intervals);

int main() {
    // Empty input.
    assert(longestChainLength({}) == 0);
    
    // Single interval.
    assert(longestChainLength({{1, 2}}) == 1);
    
    // Simple chain: (1,2) -> (3,4) -> (5,6)
    assert(longestChainLength({{3, 4}, {1, 2}, {5, 6}}) == 3);
    
    // Overlapping intervals cannot form a long chain.
    // (1,3), (2,4) cannot be chained (3 not < 2), each alone gives length 1.
    assert(longestChainLength({{1, 3}, {2, 4}}) == 1);
    
    // Chain with some gaps: (1,2), (4,5), (6,7)
    assert(longestChainLength({{6, 7}, {1, 2}, {4, 5}}) == 3);
    
    // Duplicate intervals cannot be chained together.
    assert(longestChainLength({{1, 2}, {1, 2}, {1, 2}}) == 1);
    
    // More complex: (1,2), (3,5), (4,7) -> best chain is (1,2)->(3,5) length 2.
    assert(longestChainLength({{1, 2}, {3, 5}, {4, 7}}) == 2);
    
    // All intervals disjoint.
    assert(longestChainLength({{1, 2}, {3, 4}, {5, 6}, {7, 8}}) == 4);
    
    // Unsorted with negative numbers: (-5,-3), (-2,0), (1,3)
    assert(longestChainLength({{1, 3}, {-5, -3}, {-2, 0}}) == 3);
    
    return 0;
}

// The problem is a variation of the Longest Increasing Subsequence (LIS) adapted to intervals. The key is to sort the intervals by their first coordinate (and if equal, by second coordinate to ensure deterministic grouping) because in an optimal chain, intervals must appear in non-decreasing order of their first coordinates. After sorting, we apply dynamic programming: define `dp[i]` as the length of the longest chain ending with the `i`-th interval (in sorted order). Initialize `dp[i] = 1` for each interval (a chain of just itself). Then for each `i` from 1 to n-1, we check every previous `j < i`: if `arr[j].second < arr[i].first`, then we can extend the chain ending at `j` by appending interval `i`, so `dp[i] = max(dp[i], dp[j] + 1)`. The answer is the maximum over all `dp[i]`. Edge cases: empty input returns 0; one interval returns 1; duplicate intervals can never be chained because the condition requires strict `second < first`, so they will each just be length 1 unless combined with other intervals. Time complexity is O(n^2) due to the nested loops, and O(n) auxiliary space for the dp array.
