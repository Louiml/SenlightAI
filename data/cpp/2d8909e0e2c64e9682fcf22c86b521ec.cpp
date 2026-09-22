Write a C++ function `minimumMovesToSortedGroups` that takes three integers `k1`, `k2`, `k3` and three vectors (one for each group) of distinct integers from 1 to `n = k1+k2+k3` (each integer appears in exactly one of the three vectors). The function must return the minimum number of moves required to rearrange the concatenated sequence into a non-decreasing order where all elements of group 1 come first, then all of group 2, then all of group 3. A move consists of taking any element and moving it to any position (the rest shift accordingly). The original sequence order is determined by sorting all integers from 1 to `n` in ascending order of their numeric labels; each position `i` (0-indexed) has a group label (0,1,2) indicating which group the integer `i+1` belongs to. You may reorder the sequence arbitrarily by moving elements, but moving an element counts as 1 move regardless of distance. The goal is to achieve a final sequence that is sorted by group labels (all 0s, then all 1s, then all 2s) with the minimum number of element relocations.
The problem is a classic minimum moves to sort a sequence where the target order is known. The answer is `n - L`, where `L` is the length of the longest subsequence that is already in the correct relative order (i.e., the longest subsequence that does not need to be moved). Since each move can fix at most one element's position, the minimum moves equals the number of elements that are not part of the longest "good" subsequence.

We can compute this via dynamic programming over the sequence of group labels `a[0..n-1]` (where `a[i] = 0,1,2`). The target order is non-decreasing: 0s then 1s then 2s. The longest subsequence that is non-decreasing in this sense is exactly the longest subsequence where the group labels are non-decreasing. This can be found with DP: let `dp[i]` be the length of the longest valid subsequence ending at position `i` with value `a[i]`. For each `i`, we look back at all `j < i` where `a[j] <= a[i]` and take max. This is O(n^2) which is fine for n up to 2e5? Actually O(n^2) is too slow for n=2e5. But there is a smarter approach using the observation that the groups are only 3 distinct values. We can compute the longest non-decreasing subsequence in O(n) by maintaining the longest valid prefix for each possible last value. Specifically, let `best[0], best[1], best[2]` represent the longest valid subsequence that ends with the given group label. For each element `x` in order, we can extend any subsequence whose last value is `<= x`. So we update: for each `v` from 0 to 2, if `a[i] >= v`, then `best[a[i]] = max(best[a[i]], best[v] + 1)`. But careful: this must be done in correct order to avoid reusing the element twice. Since we process elements one by one, we can just compute for the current `x`: `best[x] = max(best[0..x]) + 1` where `best[0..x]` are the previous bests. Actually we can do a simpler DP: `dp[0]` = longest subsequence with only group 0; `dp[1]` = longest subsequence with groups 0 then 1; `dp[2]` = longest with 0,1,2. For each element, if it is 0: `dp[0]++`; if 1: `dp[1] = max(dp[0], dp[1]) + 1`, but we must take the max of the old dp[0] and dp[1] before incrementing. If 2: `dp[2] = max(dp[0], dp[1], dp[2]) + 1`. This yields the longest non-decreasing subsequence. Then answer = n - max(dp[0], dp[1], dp[2]).

Edge cases: all elements same group, empty vectors? The problem states each integer appears exactly once, so n>=3? Actually k1,k2,k3 can be zero? The snippet assumes positive? The original code initializes ans as n - (number of 2s) then adjusts, but the DP approach is cleaner. For n up to 2e5, O(n) time and O(1) space is optimal. We ignore the actual integer values except to build the label array.
#include <vector>
#include <algorithm>

// Compute minimum moves to make the sequence sorted by group (0, then 1, then 2).
// k1, k2, k3 are group sizes. group1, group2, group3 contain distinct integers from 1..n.
int minimumMovesToSortedGroups(int k1, int k2, int k3,
                               const std::vector<int>& group1,
                               const std::vector<int>& group2,
                               const std::vector<int>& group3) {
    int n = k1 + k2 + k3;
    std::vector<int> label(n, -1); // label[i] = group of integer i+1

    for (int x : group1) label[x-1] = 0;
    for (int x : group2) label[x-1] = 1;
    for (int x : group3) label[x-1] = 2;

    // dp0: longest valid subsequence consisting of only 0s
    // dp1: longest valid subsequence consisting of 0s then 1s
    // dp2: longest valid subsequence consisting of 0s, 1s, then 2s
    int dp0 = 0, dp1 = 0, dp2 = 0;

    for (int i = 0; i < n; ++i) {
        int val = label[i];
        if (val == 0) {
            ++dp0;
        } else if (val == 1) {
            dp1 = std::max(dp0, dp1) + 1;
        } else { // val == 2
            dp2 = std::max({dp0, dp1, dp2}) + 1;
        }
    }

    int longestNonDecreasing = std::max({dp0, dp1, dp2});
    return n - longestNonDecreasing;
}
#include <cassert>
#include <vector>

// The solution function is assumed to be defined above.

int main() {
    // Test 1: Basic sorted already: 1,2,3 each group in order
    {
        std::vector<int> g1 = {1};
        std::vector<int> g2 = {2};
        std::vector<int> g3 = {3};
        assert(minimumMovesToSortedGroups(1,1,1, g1,g2,g3) == 0);
    }
    // Test 2: Reverse order: 3,2,1 -> need to move all but one?
    {
        std::vector<int> g1 = {3};
        std::vector<int> g2 = {2};
        std::vector<int> g3 = {1};
        // labels: position0(int1)->2, pos1(int2)->1, pos2(int3)->0 => [2,1,0]
        // longest non-decreasing subsequence length? [2] or [1] or [0] or [1,0? no] => 1, so moves=2
        assert(minimumMovesToSortedGroups(1,1,1, g1,g2,g3) == 2);
    }
    // Test 3: All same group
    {
        std::vector<int> g1 = {1,2,3};
        std::vector<int> g2 = {};
        std::vector<int> g3 = {};
        assert(minimumMovesToSortedGroups(3,0,0, g1,g2,g3) == 0);
    }
    // Test 4: Mixed example: n=5, labels = [0,1,2,0,1]? Create accordingly
    // Let groups: g1={1,4}, g2={2,5}, g3={3}
    {
        std::vector<int> g1 = {1,4};
        std::vector<int> g2 = {2,5};
        std::vector<int> g3 = {3};
        // labels: [0,1,2,0,1] -> longest non-decreasing: [0,1,2] length 3? Check: positions 0=0,1=1,2=2 works. Actually also [0,1] at pos0,1 then pos3=0? No. So length 3, moves=2.
        assert(minimumMovesToSortedGroups(2,2,1, g1,g2,g3) == 2);
    }
    // Test 5: Larger n with alternating
    {
        std::vector<int> g1 = {2,4} ; // gives labels for int2 and int4 => position1=0, pos3=0
        std::vector<int> g2 = {1,5} ; // pos0=1, pos4=1
        std::vector<int> g3 = {3}   ; // pos2=2
        // labels: [1,0,2,0,1] -> longest non-decreasing? [0,2] or [0,1]? Actually positions: 1(0),3(0),4(1) gives [0,0,1] length 3? That's non-decreasing (0,0,1) valid. So length 3, moves=2.
        assert(minimumMovesToSortedGroups(2,2,1, g1,g2,g3) == 2);
    }
    // Test 6: All mixed, not sorted
    {
        std::vector<int> g1 = {1,3};
        std::vector<int> g2 = {2};
        std::vector<int> g3 = {4,5};
        // labels: [0,1,0,2,2] -> longest [0,1,2,2] length 4? Actually [0,1,2,2] at positions 0,1,3,4 yes. moves=1
        assert(minimumMovesToSortedGroups(2,1,2, g1,g2,g3) == 1);
    }
    // Test 7: Edge case single element
    {
        std::vector<int> g1 = {1};
        std::vector<int> g2 = {};
        std::vector<int> g3 = {};
        assert(minimumMovesToSortedGroups(1,0,0, g1,g2,g3) == 0);
    }
    // Test 8: n=2 reversed
    {
        std::vector<int> g1 = {2};
        std::vector<int> g2 = {1};
        std::vector<int> g3 = {};
        // labels: [1,0] -> longest non-decreasing: [1] or [0] length1, moves=1
        assert(minimumMovesToSortedGroups(1,1,0, g1,g2,g3) == 1);
    }
    // Test 9: n=3: [0,2,1] -> longest [0,2] length2, moves=1
    {
        std::vector<int> g1 = {1};
        std::vector<int> g2 = {3};
        std::vector<int> g3 = {2};
        // labels: [0,2,1] -> moves=1
        assert(minimumMovesToSortedGroups(1,1,1, g1,g2,g3) == 1);
    }
    // Test 10: Perfect reverse of groups: [2,1,0,1,2]? That's not strictly decreasing but let's test
    {
        std::vector<int> g1 = {3};
        std::vector<int> g2 = {2,4};
        std::vector<int> g3 = {1,5};
        // labels: [2,1,0,1,2] -> longest non-decreasing: possible [0,1,2] at positions 2(0),3(1),4(2) length 3, moves=2
        assert(minimumMovesToSortedGroups(1,2,2, g1,g2,g3) == 2);
    }
    return 0;
}
