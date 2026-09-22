/*
You are given `n` (1 ≤ n ≤ 3000) groups, where each group `i` contains a list of non-negative integer values (with 0 as a dummy first element, so if the group is described by m_i real elements, it has m_i+1 elements in the vector). You may select at most `k` (0 ≤ k ≤ 3000) items in total across all groups, but with the constraint that from each group you can only take a prefix (i.e., if you take the j-th item, you must take all earlier items in that group). Your goal is to maximize the total sum of selected values. Write a C++ function `long long maxPrefixSum(const std::vector<std::vector<long long>>& groups, int k)` that returns that maximum sum. The input vectors already include the leading 0, so for a group with elements [0, 10, 20, 5], taking 3 items means taking 0+10+20 = 30. You can take zero items from any group. Groups are independent; the only coupling is the global limit `k`. The values are non-negative but totals can be up to 3000*3000*1e9, so use 64-bit integers. Ensure your solution handles the case where `n` is large and `k` is large efficiently.
*/
#include <vector>
#include <algorithm>

// Returns the maximum sum of choosing prefixes from groups with total selected items limited to k.
long long maxPrefixSum(const std::vector<std::vector<long long>>& groups, int k) {
    int n = (int)groups.size();
    if (n == 0 || k == 0) return 0;
    
    std::vector<long long> dp(k + 1, 0); // dp[j] = max sum with exactly j items selected (or at most? we use exactly, but initialize 0 which acts as at most because we can always add zeros)
    
    long long answer = 0;
    
    // Helper function for divide and conquer
    // We use a recursive function that returns the answer but also modifies dp. Since we need to restore state, we copy dp before each side.
    // Implement via lambda recursion.
    std::function<void(int, int)> solve = [&](int l, int r) {
        if (l == r) {
            // Leaf: consider taking prefixes of lengths 0..min(m, k)
            int m = (int)groups[l].size() - 1; // number of real items
            int limit = std::min(m, k);
            for (int i = 0; i <= limit; ++i) {
                // dp[k] stores the best for exactly k items? Actually dp[j] is best sum for exactly j items.
                // But we want at most k total, so combine dp[k - i] with prefix sum of group l of length i.
                // dp[k-i] must be valid for at most k-i items (since we initialized with 0, it's fine).
                long long candidate = dp[k - i] + groups[l][i];
                if (candidate > answer) answer = candidate;
            }
            return;
        }
        int mid = l + (r - l) / 2;
        // Save state for left recursion
        std::vector<long long> saved = dp;
        
        // Add all groups from the right half as whole items (prefix full length)
        for (int i = mid + 1; i <= r; ++i) {
            int m = (int)groups[i].size() - 1;
            // If group has no real items, skip
            if (m == 0) continue;
            long long weight = m; // number of items taken
            long long value = groups[i][m]; // sum of all real items
            // 0/1 knapsack update
            for (int j = k; j >= (int)weight; --j) {
                dp[j] = std::max(dp[j], dp[j - (int)weight] + value);
            }
        }
        // Recurse left
        solve(l, mid);
        
        // Restore state and add left half as whole items, then recurse right
        dp = saved;
        for (int i = l; i <= mid; ++i) {
            int m = (int)groups[i].size() - 1;
            if (m == 0) continue;
            long long weight = m;
            long long value = groups[i][m];
            for (int j = k; j >= (int)weight; --j) {
                dp[j] = std::max(dp[j], dp[j - (int)weight] + value);
            }
        }
        solve(mid + 1, r);
        
        // No need to restore for final because answer already computed
        dp = saved;
    };
    
    solve(0, n - 1);
    return answer;
}
#include <cassert>
#include <vector>
#include <string>

// Include the solution function here (or declare it)

int main() {
    // Test 1: basic case
    {
        std::vector<std::vector<long long>> groups = {
            {0, 10, 20},
            {0, 5, 15},
            {0, 1}
        };
        int k = 3;
        long long result = maxPrefixSum(groups, k);
        assert(result == 35); // Take group1 fully (2 items, sum 30) + 1 from group2 (sum 5) = 35; or group1 1 item + group2 2 items = 10+15=25; best is 30+5=35
    }
    // Test 2: single group, k limits prefix
    {
        std::vector<std::vector<long long>> groups = {{0, 100, 200, 300}};
        assert(maxPrefixSum(groups, 2) == 300); // take first two: 100+200=300
        assert(maxPrefixSum(groups, 5) == 600); // take all three: 100+200+300=600
        assert(maxPrefixSum(groups, 0) == 0);
    }
    // Test 3: empty groups
    {
        std::vector<std::vector<long long>> groups = {{0}, {0, 7}, {0}};
        assert(maxPrefixSum(groups, 1) == 7);
        assert(maxPrefixSum(groups, 3) == 7);
    }
    // Test 4: all zeros, k large
    {
        std::vector<std::vector<long long>> groups(100, std::vector<long long>(3, 0)); // each {0,0,0}
        assert(maxPrefixSum(groups, 50) == 0);
    }
    // Test 5: large values, check overflow handling
    {
        std::vector<std::vector<long long>> groups = {
            {0, 1000000000LL, 1000000000LL},
            {0, 1000000000LL, 1000000000LL}
        };
        assert(maxPrefixSum(groups, 4) == 4000000000LL);
        assert(maxPrefixSum(groups, 3) == 3000000000LL);
    }
    // Test 6: only one group with many items, k equal to total
    {
        std::vector<long long> g = {0};
        for (int i = 1; i <= 10; ++i) g.push_back(i);
        std::vector<std::vector<long long>> groups = {g};
        assert(maxPrefixSum(groups, 10) == 55);
    }
    // Test 7: multiple groups, k smaller than any group size
    {
        std::vector<std::vector<long long>> groups = {
            {0, 100, 1, 2},
            {0, 50, 50, 1}
        };
        // k=2: options: take 2 from group0 (101) or 2 from group1 (100) or 1 each (150) => 150
        assert(maxPrefixSum(groups, 2) == 150);
    }
    // Test 8: k larger than total items available
    {
        std::vector<std::vector<long long>> groups = {
            {0, 1, 2},
            {0, 3}
        };
        assert(maxPrefixSum(groups, 100) == 6); // sum all
    }
    // Test 9: groups with many items, n large (performance smoke)
    {
        std::vector<std::vector<long long>> groups;
        for (int i = 0; i < 50; ++i) {
            std::vector<long long> g = {0};
            for (int j = 1; j <= 50; ++j) g.push_back(j);
            groups.push_back(g);
        }
        // Total sum if take all = 50* (50*51/2) = 50*1275 = 63750
        assert(maxPrefixSum(groups, 2500) == 63750LL);
    }
    // Test 10: mixed sizes
    {
        std::vector<std::vector<long long>> groups = {
            {0, 5, 1},
            {0, 2},
            {0, 3, 3, 3}
        };
        // k=3: best: take group0 fully (2 items, 6) + group1 (1 item, 2) = 8; or group0 1 item (5) + group2 2 items (6) = 11; take group2 3 items (9) alone = 9; best is 11
        assert(maxPrefixSum(groups, 3) == 11);
    }
    return 0;
}
// This is a classic "grouped knapsack with prefix sums" problem, but with the twist that each group is a sequence where you must take a prefix. A naive DP over groups and capacity would be O(n*k*max_group_size) which is too slow if group sizes are large (up to 3000 each). The original snippet uses a divide-and-conquer (D&C) DP optimization where the "items" are the full-group choices (take all or nothing) but with the ability to "peel off" individual items at the leaves. Specifically, the algorithm recursively splits the groups into two halves. In the left half recursion, we first add all groups from the right half as "entire groups" (i.e., update the DP with the full-sum of each right group, which is optimal if we take any from a right group we might as well take all because all values are non-negative). Then we recurse into the left half. At a leaf (single group), we try all possible prefix lengths `i` from 0 to min(m_i, k) and combine with the current DP state `f[k-i]` plus the prefix sum `a[l][i]` to update the answer. After processing a branch, we restore the DP state snapshot before the branch to process the other branch. This works because non-negative values mean that for the "other" half, when we recurse into one side, we temporarily commit to taking the entire other side (which is optimal if we take any from that side). After the recursion, we restore to explore the other side. The time complexity is O(n*k*log n) because at each level of recursion we do O(k) work per group (inserting each group as an item into the knapsack DP), and there are O(log n) levels. Space complexity is O(k) for the DP array plus O(k*log n) for temporary snapshots during recursion (or O(k) if we reuse a global stack but we need to save/restore). Edge cases: `k=0` means answer 0; empty groups (just [0]) contribute nothing; prefix lengths capped by k; the leaf case must consider taking zero items from that group (i=0). All values are non-negative so full-group addition is always beneficial when taking any from that group.
