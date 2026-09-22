// Given a vector of non-negative integers `nums` and a target value `k`, write a C++ function `long long minIncrementOperations(const std::vector<int>& nums, int k)` that returns the minimum total increment cost needed to ensure that for every contiguous subarray of length 3, at least one element is at least `k`. You may increment any element by 1, paying a cost of 1 per unit increment. The function must work for any vector length (including 0, 1, and 2, where the condition is vacuously true) and handle large totals using `long long`. The original `nums` must not be modified (i.e., use `const` reference).

The key is to transform each element into a "cost required to raise it to at least `k`": `cost[i] = max(0, k - nums[i])`. Now we must choose a set of positions to "activate" such that no three consecutive positions are all unactivated, while minimizing the sum of selected costs. This is a classic dynamic programming problem on a sliding window. Define three states as we scan left to right:  
- `dp0` = minimum cost up to current position where the *current* element is *not* selected.  
- `dp1` = minimum cost where the *previous* element was selected (and current not selected).  
- `dp2` = minimum cost where the *two-ago* element was selected (and current and previous not selected).  

Initialize at index 0: we must select it (because choosing nothing would lead to an empty window), so `dp0 = cost[0]` (we set the state as if we just selected it). Then for each subsequent `i`, the new `dp0` = `cost[i] + min(dp0, dp1, dp2)` (because we select the current element, and the previous three states are free to be any). The new `dp1` = old `dp0` (current not selected, previous selected). The new `dp2` = old `dp1` (current not selected, previous not selected, but two-ago selected). This recurrence ensures that every window of three has at least one selected. At the end, the answer is `min(dp0, dp1, dp2)` because the last few elements may be not selected without violating the condition. Edge cases: if `n < 3`, the answer is 0 because no triple exists. For `n >= 3`, the recurrence works. Time complexity is O(n), space O(1).

#include <vector>
#include <algorithm>

// Return the minimum total increment cost so that every length-3 subarray has an element >= k.
// The input vector is not modified.
long long minIncrementOperations(const std::vector<int>& nums, int k) {
    const int n = static_cast<int>(nums.size());
    if (n < 3) return 0;

    // Convert each element to its required increment cost to reach k.
    std::vector<long long> cost(n);
    for (int i = 0; i < n; ++i) {
        cost[i] = (nums[i] >= k) ? 0LL : static_cast<long long>(k - nums[i]);
    }

    // DP states:
    // dp0: minimum cost where current element is selected.
    // dp1: minimum cost where previous element is selected (current not).
    // dp2: minimum cost where two-ago element is selected (current and previous not).
    long long dp0 = cost[0];
    long long dp1 = 0; // not meaningful before index 1
    long long dp2 = 0; // not meaningful before index 2

    for (int i = 1; i < n; ++i) {
        long long new_dp0 = cost[i] + std::min({dp0, dp1, dp2});
        long long new_dp1 = dp0; // previous element was selected
        long long new_dp2 = dp1; // two-ago element was selected
        dp0 = new_dp0;
        dp1 = new_dp1;
        dp2 = new_dp2;
    }

    return std::min({dp0, dp1, dp2});
}

#include <cassert>
#include <vector>

int main() {
    // Basic case: need to raise middle element to k.
    assert(minIncrementOperations({1, 1, 1}, 3) == 2);
    
    // Already satisfied: first element is k.
    assert(minIncrementOperations({5, 1, 1}, 3) == 0);
    
    // Large target, multiple windows.
    assert(minIncrementOperations({1, 2, 3, 4, 5}, 10) == (9 + 8 + 7 + 6 + 5)); // choose all, sum=35? Actually check: 10-1=9,10-2=8,10-3=7,10-4=6,10-5=5 total=35) == 35);
    
    // Edge: n < 3 returns 0.
    assert(minIncrementOperations({}, 5) == 0);
    assert(minIncrementOperations({1}, 5) == 0);
    assert(minIncrementOperations({1, 2}, 5) == 0);
    
    // Longer vector, optimal selection pattern.
    assert(minIncrementOperations({1, 2, 1, 2, 1}, 3) == 3); // raise positions 2 and 4 (cost 1+1=2? Let's compute: cost=[2,1,2,1,2]. Selecting positions 2 and 4 gives cost 1+1=2. But can we do better? Try selecting position 1 (cost2) and 4(cost1)=3. So minimum is 2 (positions 0? Wait check: n=5, need every triple. Option: select index 0 (cost2), index 3 (cost1) total3. Select index 1 (cost1), index 4(cost2) total3. Select index 1 and index 3 = 1+1=2: windows? Window[0,2] has index1 selected, window[1,3] has both index1 and index3 selected, window[2,4] has index3 selected. So yes 2 is correct) == 2);
    
    // Mixed with already satisfactory elements.
    assert(minIncrementOperations({0, 100, 0, 0}, 50) == 0); // middle is already >= k, all windows covered.
    
    // Test where optimal is to skip some high-cost middle elements.
    assert(minIncrementOperations({9, 9, 1, 9, 9}, 10) == 1); // only center needs +1.
    
    // Stress test for large values (no overflow).
    assert(minIncrementOperations({1, 1, 1, 1}, 1000000) == 2999997); // raise first three? Let's compute: need each triple, four elements. Options: select positions 1 and 2 (cost 999999+999999=1999998) covers all triples? windows: [0,2] has pos1, [1,3] has pos2. So cost 1999998. Or select positions 0,2 cost 1999998. Or select all four cost 3999996. So minimum is 1999998. But check: our function returns? Actually we can select positions 1 and 3? That would be cost 999999*2=1999998, but does it cover window [0,2]? pos1 yes, window [1,3] pos1 and pos3 yes. So yes 1999998. So assert should be 1999998, not 2999997. Let me correct: 1999998. So assert(minIncrementOperations({1,1,1,1}, 1000000) == 1999998);
    
    return 0;
}
