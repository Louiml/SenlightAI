// Write a C++ function `int longestChainLength(vector<vector<int>>& pairs)` that, given a list of integer pairs where each pair represents an interval `[first, second]`, returns the length of the longest chain of pairs such that for every consecutive pair `(a, b)` and `(c, d)` in the chain, `b < c` (the end of one interval is strictly less than the start of the next). You may reorder the pairs in any way to maximize the chain. The input may contain duplicate pairs, negative numbers, and intervals where `first` is not necessarily less than `second` — but for this task you may assume `first <= second` for every pair. If the list is empty, return 0; if it has one pair, return 1.
The problem is a variation of the Longest Increasing Subsequence (LIS) applied to intervals, but we can reorder pairs arbitrarily. Sorting the pairs by their starting value (and if equal, by ending value) is crucial because it allows a greedy-like dynamic programming approach: after sorting, any valid chain must respect the sorted order (since if we have `(a,b)` followed by `(c,d)` with `b < c`, then `a <= b < c <= d`, so `a < c`, meaning the sorted order is consistent). We then use DP: `dp[i]` = length of longest chain ending with pair `i`. For each `i`, we look at all previous `j < i` and if `pairs[j][1] < pairs[i][0]`, we can extend the chain: `dp[i] = max(dp[i], dp[j] + 1)`. Initialize all `dp[i] = 1` (a single pair forms a chain of length 1). The answer is the maximum over all `dp[i]`. Edge cases: empty input returns 0; duplicate pairs are handled because the strict inequality `pairs[j][1] < pairs[i][0]` prevents using the same interval or overlapping ones. The provided snippet uses a recursive memoization approach with `prev` and `indx`, but the iterative DP is simpler and cleaner. Time complexity is O(n²) due to the nested loops, space O(n) for the DP array. Sorting is O(n log n). This is optimal for this problem because the constraint is a partial order.
#include <vector>
#include <algorithm>

// Given a list of intervals [first, second], return the length of the longest chain
// where for each consecutive pair (a,b) and (c,d), we have b < c.
// Pairs may be reordered arbitrarily to maximize the chain.
int longestChainLength(std::vector<std::vector<int>>& pairs) {
    if (pairs.empty()) return 0;
    
    // Sort by start, then by end, to allow DP in order.
    std::sort(pairs.begin(), pairs.end(), 
              [](const std::vector<int>& a, const std::vector<int>& b) {
                  if (a[0] != b[0]) return a[0] < b[0];
                  return a[1] < b[1];
              });
    
    int n = static_cast<int>(pairs.size());
    std::vector<int> dp(n, 1); // each pair alone forms a chain of length 1
    
    int best = 1;
    for (int i = 1; i < n; ++i) {
        for (int j = 0; j < i; ++j) {
            if (pairs[j][1] < pairs[i][0]) {
                dp[i] = std::max(dp[i], dp[j] + 1);
            }
        }
        best = std::max(best, dp[i]);
    }
    
    return best;
}
#include <cassert>
#include <vector>

// Function declaration from the solution
int longestChainLength(std::vector<std::vector<int>>& pairs);

int main() {
    // Basic test
    std::vector<std::vector<int>> pairs1 = {{1,2}, {2,3}, {3,4}};
    assert(longestChainLength(pairs1) == 2); // e.g., [1,2] -> [3,4]
    
    // Unsorted input; reorder possible
    std::vector<std::vector<int>> pairs2 = {{5,6}, {1,2}, {3,4}};
    assert(longestChainLength(pairs2) == 2); // [1,2] -> [3,4] or [3,4] -> [5,6]
    
    // All overlapping - only one can be chosen
    std::vector<std::vector<int>> pairs3 = {{1,10}, {2,3}, {4,5}};
    assert(longestChainLength(pairs3) == 2); // [2,3] -> [4,5]
    
    // Negative numbers
    std::vector<std::vector<int>> pairs4 = {{-5,-3}, {-2,0}, {1,2}};
    assert(longestChainLength(pairs4) == 3); // -3 < -2 and 0 < 1, so all chain
    
    // Duplicates
    std::vector<std::vector<int>> pairs5 = {{1,2}, {1,2}, {2,3}};
    assert(longestChainLength(pairs5) == 2); // one [1,2] -> [2,3] (strict: 2 < 2? No, so only 2 length possible as [1,2] and can't chain two [1,2] together)
    
    // Single element
    std::vector<std::vector<int>> pairs6 = {{1,5}};
    assert(longestChainLength(pairs6) == 1);
    
    // Empty
    std::vector<std::vector<int>> pairs7;
    assert(longestChainLength(pairs7) == 0);
    
    // Equal starts but different ends
    std::vector<std::vector<int>> pairs8 = {{1,2}, {1,3}, {4,5}};
    assert(longestChainLength(pairs8) == 2); // either [1,2] -> [4,5] or [1,3] -> [4,5]
    
    return 0;
}
