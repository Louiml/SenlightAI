// Given a positive integer `n` representing the total number of cells to reach starting from exactly one cell, and three non-negative integers `x`, `y`, `z` representing the costs of multiplying a cell (double the count), incrementing the cell count by one, and decrementing the cell count by one respectively, write a C++ function `long long minimumCostToReachCells(int n, int x, int y, int z)` that returns the minimum total cost to reach exactly `n` cells. You may apply any sequence of operations: double the current count (cost `x`), add one (cost `y`), or subtract one (cost `z`), starting from count `1`. You cannot go below `1`. Return the minimal cost, which fits in a 64-bit signed integer.
// This problem is a graph shortest-path variant where each state is the current number of cells and transitions have different costs. We can solve it using dynamic programming (bottom-up) because there is an optimal substructure: to reach count `i`, we either came from a doubling operation (if `i` is even, from `i/2` with cost `x`; if `i` is odd, from `(i+1)/2` after first adding one then doubling, or from `(i-1)/2` then doubling and subtracting one — but simpler to use the given recurrence), or from an increment of `i-1` with cost `y`. However, for odd `i`, the optimal may involve subtracting from a higher number, so we need to consider both possibilities. The provided snippet uses a recurrence that for even `i`: `dp[i] = min(dp[i/2]+x, dp[i-1]+y)`; for odd `i`: `dp[i] = min(dp[(i+1)/2]+x+z, dp[i-1]+y)`. This works because for odd `i`, to use doubling, we can go from `(i+1)/2` down by one (cost `z`) after doubling (since `(i+1)/2 * 2 = i+1`, then subtract one). Alternatively, from `(i-1)/2` doubling gives `i-1` then add one (cost `y`) — but that's equivalent to the `dp[i-1]+y` option already covered. Edge cases: `n=1` returns 0 because we start with one cell. All costs are non-negative, so DP is monotonic. The bottom-up approach fills an array of size `n+1` in increasing order. Time complexity is O(n), space O(n). We must ensure results fit in `long long` since costs and `n` can be large (up to maybe 1e5, but costs up to 1e9, so product can exceed int). Use `long long` for DP array and return type.
#include <vector>
#include <algorithm>
#include <climits>

// Returns the minimum cost to reach exactly n cells starting from 1 cell.
// Operations: double (cost x), add one (cost y), subtract one (cost z).
// n must be >= 1.
long long minimumCostToReachCells(int n, int x, int y, int z) {
    if (n <= 1) return 0;
    
    // dp[i] = minimum cost to reach exactly i cells
    std::vector<long long> dp(n + 1, LLONG_MAX);
    dp[1] = 0;
    
    for (int i = 2; i <= n; ++i) {
        if (i % 2 == 0) {
            // From i/2 by doubling, or from i-1 by adding one
            dp[i] = std::min(dp[i], dp[i/2] + static_cast<long long>(x));
            dp[i] = std::min(dp[i], dp[i-1] + static_cast<long long>(y));
        } else {
            // For odd i, doubling then subtracting one from (i+1)/2, or add one from i-1
            dp[i] = std::min(dp[i], dp[(i+1)/2] + static_cast<long long>(x) + static_cast<long long>(z));
            dp[i] = std::min(dp[i], dp[i-1] + static_cast<long long>(y));
        }
    }
    
    return dp[n];
}
#include <cassert>

int main() {
    // Basic case: reach 2 from 1 by doubling only
    assert(minimumCostToReachCells(2, 5, 100, 100) == 5);
    // Reach 3: either double 1->2 (cost 5) then add one (cost 1) = 6, or from 2 add one = 1+1=2? Actually better: from 2 (cost 5) then add 1 (cost 1) = 6, but from 1 add one twice = 2+2=4? Wait: operations: start 1, add 1 -> 2 (cost 2), add 1 -> 3 (cost 2) total 4, but doubling 1->2 cost x=5, so add-one path is better. Test: n=3, x=5,y=1,z=100 -> expected 2 (two add-ones)
    assert(minimumCostToReachCells(3, 5, 1, 100) == 2);
    // Use double and subtract: reach 3 from 1: double to 2 (x=1), add one (y=100) =101, or double to 4 (x=1) then subtract (z=1) =2
    assert(minimumCostToReachCells(3, 1, 100, 1) == 2);
    // Edge: n=1
    assert(minimumCostToReachCells(1, 10, 20, 30) == 0);
    // Larger n with zero costs
    assert(minimumCostToReachCells(10, 0, 0, 0) == 0);
    // Check a known case: n=5, x=1,y=1,z=1 -> optimal? double 1->2 (1), add 1->3 (1), add 1->4 (1), add 1->5 (1) total 4; or double to 2, add to 3, add to 4, add to 5 = 4; or double to 2, double to 4, add to 5 = 3; or double to 4, add to 5 = 2? Actually from 1 double to 2 (1), double to 4 (1), add to 5 (1) = 3. So expected 3.
    assert(minimumCostToReachCells(5, 1, 1, 1) == 3);
    // Negative check: all positive costs, ensure no overflow for moderate n
    assert(minimumCostToReachCells(1000, 1000000, 1, 1000000) == 1000 - 1); // only add-ones cheapest
    return 0;
}
