Write a C++ function `int minimumShiftsToCollect(int n, int m, const std::vector<std::string>& grid)` where `grid` is an `n`-by-`m` matrix of characters `'o'` (means the shop has the item) or `'x'` (does not). You start with zero items. In one move, you may visit any row (shop) and collect all items from that row that you don't already have. The goal is to determine the minimum number of shop visits needed to collect all `m` distinct items (one per column). You may visit the same shop multiple times, but each visit you only gain items you didn't have before. The function returns the minimal number of visits (at least 1 if possible, but assume the union of all rows covers all columns). The function must be efficient for up to `n = 1000` and `m = 20` (so `m` is small, use bitmasks). If it's impossible (not covering all columns), return -1.
// We model each row as a bitmask of length `m`, where bit `j` (from least significant) is 1 if the shop has the item in column `j`. The problem reduces to finding the minimum number of bitmasks whose bitwise OR equals the full mask `(1<<m)-1`. Since `m≤20`, the number of distinct masks is at most `2^m` (up to ~1 million), but `n` can be large. We can compress: for each mask that appears in the input, keep only one representative (duplicates are redundant). Then we want the minimum subset whose OR covers the full mask. This is a classic set cover problem, but with a small universe (m bits), we can use dynamic programming over masks: `dp[mask]` = minimal number of rows needed to achieve exactly that covered mask. Initialize `dp[0]=0`, all others to large. For each distinct row mask `r`, for each current mask `cur`, we update `dp[cur | r] = min(dp[cur | r], dp[cur]+1)`. This is similar to a shortest path on the mask space. Complexity: O(2^m * number_of_distinct_masks). With `m≤20`, that's at most 1,048,576 * min(n,2^m) which is acceptable if we prune distinct. Edge cases: if the union of all rows doesn't cover full mask, return -1. Also, if a row is all zeros, it doesn't help. The result is the minimal number of distinct masks needed. Time complexity: O(2^m * D) where D is distinct rows (≤ min(n, 2^m)). Space O(2^m).
#include <vector>
#include <string>
#include <algorithm>
#include <limits>

// Returns the minimum number of rows needed to cover all m columns.
// grid: n rows, each of length m, characters 'o' (has item) or 'x' (not).
// If impossible, returns -1.
int minimumShiftsToCollect(int n, int m, const std::vector<std::string>& grid) {
    const int FULL = (1 << m) - 1;
    
    // Build a set of distinct bitmasks from the grid.
    std::vector<bool> seen(1 << m, false);
    std::vector<int> masks;
    masks.reserve(n);
    for (int i = 0; i < n; ++i) {
        int mask = 0;
        for (int j = 0; j < m; ++j) {
            if (grid[i][j] == 'o') {
                mask |= (1 << j);
            }
        }
        if (!seen[mask]) {
            seen[mask] = true;
            masks.push_back(mask);
        }
    }
    
    // Check if full cover is possible at all.
    int unionMask = 0;
    for (int mask : masks) {
        unionMask |= mask;
    }
    if (unionMask != FULL) {
        return -1;
    }
    
    // dp[mask] = minimal number of rows to achieve exactly this covered set.
    const int INF = std::numeric_limits<int>::max() / 2;
    std::vector<int> dp(1 << m, INF);
    dp[0] = 0;
    
    for (int mask : masks) {
        if (mask == 0) continue; // zero mask never helps
        for (int cur = 0; cur <= FULL; ++cur) {
            if (dp[cur] == INF) continue;
            int next = cur | mask;
            dp[next] = std::min(dp[next], dp[cur] + 1);
        }
    }
    
    return dp[FULL] == INF ? -1 : dp[FULL];
}
#include <cassert>
#include <vector>
#include <string>

// Declare the function (or include the above solution)
int minimumShiftsToCollect(int n, int m, const std::vector<std::string>& grid);

int main() {
    // Simple full coverage in one row
    assert(minimumShiftsToCollect(1, 3, {"ooo"}) == 1);
    // Need two rows to cover all columns
    assert(minimumShiftsToCollect(2, 3, {"oxx", "xxo"}) == 2);
    // Three rows but one suffices
    assert(minimumShiftsToCollect(3, 2, {"xo", "ox", "xx"}) == 2);
    // All rows together still missing a column -> -1
    assert(minimumShiftsToCollect(2, 3, {"oxx", "xox"}) == -1);
    // Edge: m=1, single column, multiple rows with 'o' and 'x'
    assert(minimumShiftsToCollect(2, 1, {"o", "x"}) == 1);
    // Duplicate masks should be ignored, still need 2
    assert(minimumShiftsToCollect(4, 2, {"xo", "xo", "ox", "ox"}) == 2);
    // Larger m (4) with two rows covering complementary halves
    assert(minimumShiftsToCollect(2, 4, {"oxxo", "xoox"}) == 2);
    // One row provides all, others duplicates
    assert(minimumShiftsToCollect(3, 3, {"ooo", "xoo", "oox"}) == 1);
    // Impossible even with many rows
    assert(minimumShiftsToCollect(3, 3, {"oxx", "xox", "xxo"}) == -1); // each missing the first column? Check: row0 has col1, row1 has col2, row2 has col0? Actually row2 "xxo" has col2 only, so union has col0,1,2? row0: col1, row1: col2, row2: col2 only -> missing col0? Wait row0 has col1, row1 has col2, row2 has col2, so col0 missing -> -1. Correct.
    return 0;
}
