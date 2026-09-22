Write a C++ function `int countValidPyramids(const std::vector<std::vector<int>>& grid)` that takes a 2D binary grid (each cell is 0 or 1) and returns the total number of "valid pyramids" it contains. A valid pyramid is any set of cells that forms an upright or inverted triangle made entirely of 1s, where each level (row) of the triangle is exactly one cell shorter than the level below/above it. Specifically, an upright pyramid of height `h` (with `h >= 2`) has its top cell at some `(i,j)`, and for each subsequent row below it, the 1-cells form a contiguous block of length `2k+1` centered at column `j`, for `k = 1` to `h-1`. An inverted pyramid is the same but mirrored vertically (top row is the widest). Count each individual pyramid separately; overlapping pyramids are allowed. The grid dimensions are at least 1x1 and at most 200x200. Return the total count of both upright and inverted pyramids.
#include <cassert>
#include <vector>

int countValidPyramids(const std::vector<std::vector<int>>& grid);

int main() {
    // Single 1 cell: no pyramids.
    assert(countValidPyramids({{1}}) == 0);
    
    // 2x3 grid with a full block of ones: exactly one upright and one inverted pyramid (each height 2).
    assert(countValidPyramids({{1,1,1},{1,1,1}}) == 2);
    
    // 3x3 grid with all ones: 
    // Upright: apex at (0,1) -> 1 pyramid; apex at (1,0),(1,1),(1,2) -> 0 because no row below.
    // Inverted: similarly 1. Total = 2.
    assert(countValidPyramids({{1,1,1},{1,1,1},{1,1,1}}) == 2);
    
    // 3x5 grid: 
    // Upright: apex at (0,2) -> base row 1 has 3 ones, row 2 has 5 ones -> dp=2 -> counts 1 pyramid height 2, and apex at (0,1) and (0,3) also valid? No because center column not symmetrical? Actually for apex (0,2), dp=2. For apex (0,1): base at (1,0),(1,1),(1,2) all 1, but (2,0),(2,1),(2,2) not relevant because dp[1][*] would be 0 if no row below? Actually dp from bottom: for i=1, j=1: base row 2 all ones, so dp[1][1]=1; then for i=0, j=1: base (1,0),(1,1),(1,2) all ones, min(dp[1][0],dp[1][1],dp[1][2]) but dp[1][0] and dp[1][2] are 0 (since j=0 and j=2 at row 1 have no left/right neighbors? Actually j=0 has no j-1, so dp[1][0]=0). So dp[0][1]=0. Similarly j=3 gives 0. Only j=2 gives dp[0][2]=1+min(dp[1][1],dp[1][2],dp[1][3]) but dp[1][1]=1 (since base row 2 all ones) and dp[1][2]=0 due to missing left? Actually row 1, j=2 -> base row 2: (1,3),(2,2),(3,2) all 1, so dp[1][2]=1. dp[1][3]=0 because j+1 out of range? For row 1 j=3, need j+1=4 out of range, so dp[1][3]=0. So dp[0][2]=1+min(1,1,0)=1. So upright=1. Inverted similarly=1. Total=2.
    assert(countValidPyramids({{1,1,1,1,1},{1,1,1,1,1},{1,1,1,1,1}}) == 2);
    
    // 4x5 all ones: 
    // Upright: apex at (0,2) -> dp[0][2] = 1 + min(dp[1][1],dp[1][2],dp[1][3]). dp[1][1]=? base row2: (2,0),(2,1),(2,2) all 1, but dp[2][0]=? row2 j=0 -> base row3 requires j-1=-1 out of range, so 0. So dp[1][1]=0. dp[1][2]=1+min(dp[2][1],dp[2][2],dp[2][3]). dp[2][1]=1+min(dp[3][0]? out of range -> 0), so dp[2][1]=0. Actually careful: dp[2][1] base row3 (3,0),(3,1),(3,2) all 1, but dp[3][0]=0 because no row below. So dp[2][1]=1+min(0,0,0)=1? Wait min of dp[3][0], dp[3][1], dp[3][2] all 0 because dp[3][*] = 0 (no row below). So dp[2][1]=1. Similarly dp[2][2]=1, dp[2][3]=1. So dp[1][2]=1+min(1,1,1)=2. dp[1][3]=? similarly 2? Actually dp[1][3] base row2 (2,2),(2,3),(2,4) all 1, min(dp[2][2],dp[2][3],dp[2][4]) = min(1,1,0?) dp[2][4] is 0 because j+1=5 out of range. So dp[1][3]=1+min(1,1,0)=1. So dp[0][2]=1+min(dp[1][1]=0? dp[1][1] we computed 0? Let's recompute dp[1][1]: base row2 (2,0),(2,1),(2,2) all 1, but dp[2][0]=0 (no left neighbor), dp[2][1]=1, dp[2][2]=1, so min=0, dp[1][1]=1. So dp[0][2]=1+min(1,2,1)=2. That counts 2 pyramids (height2 and height3) with apex at (0,2). Also apex at (0,1) and (0,3) might give? They would have base on row1 but row1 (j-1..j+1) includes j=0 or j=4 with dp[1][0] or dp[1][4] zero, so 0. So upright total = 2. Similarly inverted = 2. Total=4.
    assert(countValidPyramids({{1,1,1,1,1},{1,1,1,1,1},{1,1,1,1,1},{1,1,1,1,1}}) == 4);
    
    // Mixed grid with holes.
    assert(countValidPyramids({{1,0,1},{1,1,1},{1,1,1}}) == 1); // only upright at (0,2)? Actually (0,2) base (1,1),(1,2),(1,3)? No (row1 has 3 cells, j=2 -> needs j+1=3 out of bounds). So 0. (1,1) base row2 all ones but dp[2][*]=0 so dp[1][1]=1? Wait dp[1][1] counts pyramid of height2? It requires base row2 (2,0),(2,1),(2,2) all 1, yes all 1, min(dp[2][0],dp[2][1],dp[2][2]) = 0, so dp[1][1]=1, which counts one pyramid height2 with apex at (1,1). Also (0,1) has grid[0][1]=0 so skip. Upright=1. Inverted: flip, then rows become {(1,1,1),(1,1,1),(1,0,1)}. In upside orientation (now top row is (1,1,1)), apex at (0,1) -> base row1 (1,0),(1,1),(1,2) all 1, dp[1][0]=? base row2 (2,-1?) out of range, so 0, so dp[0][1]=1? Actually dp[1][0] is 0, min=0, so dp[0][1]=1. So inverted also 1. Total=2. But our assertion expects 1? Let's recalc: original grid rows: row0: 1 0 1; row1: 1 1 1; row2: 1 1 1. Upright pyramids: apex must have a row below. Only apex at row0 or row1. For row0: column1 has 0 so skip; column0 needs j+1<3? j=0 -> j+1=1 ok, but base row1 columns -1,0,1? Actually j-1 = -1 out of bounds, so cannot. j=2 -> j+1=3 out of bounds. So row0 no. For row1: j=0 -> j-1=-1 out; j=1 -> base row2 columns 0,1,2 all 1, dp[2][*] all 0 -> dp[1][1]=1 -> counts 1 pyramid (height2). j=2 -> j+1=3 out. So upright=1. Inverted: flip vertically gives row0: 1 1 1; row1: 1 1 1; row2: 1 0 1. Now apex at row0: j=1 -> base row1 cols0,1,2 all 1, dp[1][0]=? base row2 cols -1,0,1? j=0, j-1=-1 out -> dp[1][0]=0; dp[1][1] base row2 cols0,1,2: (row2 [1,0,1]) so col0=1, col1=0, col2=1 -> not all 1, so dp[1][1]=0; dp[1][2] base row2 cols1,2,3: col3 out, so 0. So min=0, dp[0][1]=1 -> counts 1 pyramid. Also apex at row0 j=0? j-1 out; j=2? j+1 out. So inverted=1. Total=2. So assertion should be 2. Adjust test to expect 2.
    assert(countValidPyramids({{1,0,1},{1,1,1},{1,1,1}}) == 2);
    
    // All zeros.
    assert(countValidPyramids({{0,0,0},{0,0,0}}) == 0);
    
    // One row only.
    assert(countValidPyramids({{1,1,1,1,1}}) == 0);
    
    return 0;
}
#include <vector>
#include <algorithm>

// Counts the number of valid pyramids (both upright and inverted) in a binary grid.
int countValidPyramids(const std::vector<std::vector<int>>& grid) {
    const int rows = static_cast<int>(grid.size());
    const int cols = static_cast<int>(grid[0].size());
    
    // Lambda that counts pyramids oriented in one direction (apex at top).
    auto countOrientation = [&](const std::vector<std::vector<int>>& g) -> int {
        if (rows < 2 || cols < 3) return 0;
        std::vector<std::vector<int>> dp(rows, std::vector<int>(cols, 0));
        int total = 0;
        // Process from the bottom row upward (for upright pyramids).
        for (int i = rows - 2; i >= 0; --i) {
            for (int j = 1; j + 1 < cols; ++j) {
                if (g[i][j] == 1) {
                    if (g[i+1][j-1] == 1 && g[i+1][j] == 1 && g[i+1][j+1] == 1) {
                        int val = 1 + std::min({dp[i+1][j-1], dp[i+1][j], dp[i+1][j+1]});
                        dp[i][j] = val;
                        total += val;  // val counts all pyramid heights from 2..val with apex at (i,j)
                    }
                }
            }
        }
        return total;
    };
    
    int upright = countOrientation(grid);
    
    // Create a vertically flipped copy for inverted pyramids.
    std::vector<std::vector<int>> flipped = grid;
    std::reverse(flipped.begin(), flipped.end());
    int inverted = countOrientation(flipped);
    
    return upright + inverted;
}
// The key insight is that a pyramid of height `h` can be broken down recursively: a pyramid of height `h` exists at cell `(i,j)` if the three cells directly below/above it (at row `i±1`, columns `j-1`, `j`, `j+1`) are all 1, and each of those three cells itself supports a pyramid of height `h-1` (or is a single-cell base). This leads to dynamic programming. Define `dp[i][j]` as the maximum height of a pyramid whose **top** (for upright) or **bottom** (for inverted) cell is at `(i,j)` — but more precisely, `dp[i][j]` is the height of the largest pyramid whose apex (the single-cell tip) is at `(i,j)`. For an upright pyramid, the apex is at the top row, and for inverted, at the bottom row. The recurrence: if `grid[i][j]==1` and the three cells in the row below (for upright) are all 1, then `dp[i][j] = 1 + min(dp[i+1][j-1], dp[i+1][j], dp[i+1][j+1])`. If any of those three cells are 0, `dp[i][j]=0`. The value `dp[i][j]` directly counts how many upright pyramids have their apex at `(i,j)`: if `dp[i][j] = h`, then there are pyramids of heights 2,3,...,h all with that apex, contributing `h` pyramids total (since a single cell alone is not a pyramid; only heights ≥2 count). Summing `dp[i][j]` over all cells gives the total number of upright pyramids. For inverted pyramids, we can simply reverse the grid rows (or vice versa) and run the same DP on the reversed grid, summing the results. Edge cases: pyramids of height 1 are not counted (they are just single cells), so the DP value itself is used directly (since it already requires at least 3 cells in the next row). The grid could be all zeros, resulting in 0. Also, if the grid has fewer than 3 columns or 2 rows, no valid pyramids exist because the base of even the smallest pyramid needs 3 cells in one row. Time complexity is O(m*n) for each pass (two passes total), so O(m*n) overall. Space complexity is O(m*n) for the DP table.
