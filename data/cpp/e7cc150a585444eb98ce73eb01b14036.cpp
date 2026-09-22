/*
Write a C++ function that, given two integers `m` and `n` (both greater than or equal to 1), returns the number of distinct paths modulo `100000007` from the top-left cell `(0,0)` to the bottom-right cell `(m-1,n-1)` of an `m × n` grid, where movement is allowed only right or down. The function must compute this efficiently without using a full 2D array of size `m × n` if the grid is large; instead, use only two rows (or one row) of dynamic programming state. The result must be returned as a `long long int`. Handle the edge cases where `m == 1` or `n == 1` (only one possible path). Your function should be named `countGridPaths` and take two integer parameters.
*/
#include <vector>

// Count paths from top-left to bottom-right in an m x n grid with only right/down moves.
// Return modulo 100000007.
long long countGridPaths(int m, int n) {
    const long long MOD = 100000007LL;
    
    // Ensure we iterate over the smaller dimension for space efficiency.
    if (m < n) std::swap(m, n);
    
    std::vector<long long> dp(n, 1LL); // First row: exactly one way to reach each column.
    
    for (int i = 1; i < m; ++i) {
        for (int j = 1; j < n; ++j) {
            dp[j] = (dp[j] + dp[j-1]) % MOD;
        }
    }
    
    return dp[n-1];
}
#include <cassert>

int main() {
    // Base cases: single row or single column -> exactly 1 path.
    assert(countGridPaths(1, 1) == 1);
    assert(countGridPaths(1, 5) == 1);
    assert(countGridPaths(7, 1) == 1);
    
    // Small cases manually verifiable.
    assert(countGridPaths(2, 2) == 2);
    assert(countGridPaths(3, 3) == 6);
    assert(countGridPaths(2, 3) == 3);
    assert(countGridPaths(3, 2) == 3);
    
    // Slightly larger: check modulo behavior (10^8+7 = 100000007).
    // For 10x10, true count = 48620 (less than MOD), check normal.
    assert(countGridPaths(10, 10) == 48620);
    
    // Verify modulo is applied: choose a case where the exact count exceeds MOD.
    // For 30x30, exact number of paths is C(58,29) which is huge. 
    // We only check that the result fits in long long and is consistent with the recurrence.
    long long large = countGridPaths(30, 30);
    assert(large >= 0 && large < 100000007);
    
    // Check symmetry: m,n swapped gives same result.
    assert(countGridPaths(15, 20) == countGridPaths(20, 15));
    
    // Test a larger grid for performance sanity (no assertion on value).
    long long huge = countGridPaths(100, 100);
    (void)huge;
}
// The problem is the classic "unique paths" counting problem. For an `m × n` grid, the number of paths is given by the binomial coefficient `C(m+n-2, m-1)`, but we need it modulo `100000007` (a prime). We can compute it via dynamic programming: let `dp[j]` represent the number of ways to reach the current row's column `j` from the start. Initialize all `dp` entries to 1 (since first row has only one path from left). Then for each subsequent row (from index 1 to `m-1`), update `dp[j] = (dp[j] + dp[j-1]) % MOD` for `j` from 1 to `n-1`, because `dp[j]` before update is the value from the row above, and `dp[j-1]` is the value from the current row's left cell. After processing all rows, `dp[n-1]` is the answer. Edge cases: if `m == 1` or `n == 1`, return 1 (only straight line). Time complexity: O(m × n) but space O(n) (or O(min(m,n)) if we swap dimensions). We use 64-bit for intermediate sums to avoid overflow before modulo. The modulo is applied at each addition step, so the maximum intermediate value is less than 2*MOD, fitting in `long long`.
