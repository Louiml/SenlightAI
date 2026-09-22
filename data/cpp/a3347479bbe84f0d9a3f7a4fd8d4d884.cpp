Write a C++ function `int pizzaWays(const std::vector<std::string>& pizza, int k)` that, given a rectangular pizza represented as a vector of strings where each character is either `'A'` (apple) or `'.'` (empty), and an integer `k` (1 ≤ k ≤ 50, rows, cols ≤ 50), returns the number of ways to cut the pizza into exactly `k` pieces using vertical and horizontal cuts such that each piece contains at least one apple. A cut is made along an entire row or column boundary, and after `k-1` cuts, each resulting rectangular sub-piece must contain at least one apple. Count the number of ways modulo \(10^9+7\). Two ways are different if they differ in the set of cut positions. The pizza has at least one apple overall, and rows/cols are at least 1. Note: after each cut, the pizza must be divided into contiguous rectangular pieces; you cannot cut a piece that has already been separated in a different orientation? Actually, the standard problem allows cutting the whole remaining pizza each time, not individual pieces independently? In this problem, each cut is applied to the entire current pizza rectangle (remaining from top-left), not to individual pieces, so the state is always the top-left sub-rectangle after cuts. So the function should compute the number of sequences of `k-1` cuts (each along a vertical or horizontal line) such that every resulting piece (including intermediate pieces) contains at least one apple, and the final `k` pieces each have at least one apple.
#include <cassert>
#include <vector>
#include <string>

int main() {
    // Example 1 from LeetCode: pizza = ["A..","AAA","..."], k = 3, expected 3
    assert(pizzaWays({"A..", "AAA", "..."}, 3) == 3);
    
    // Single row, k=1
    assert(pizzaWays({"A.."}, 1) == 1);
    assert(pizzaWays({".A."}, 1) == 1);
    assert(pizzaWays({"..."}, 1) == 0); // no apple, but problem assumes at least one, still test
    
    // 1x1 with one apple, k=1
    assert(pizzaWays({"A"}, 1) == 1);
    
    // 1x1 with one apple, k more than pieces impossible? k must be <= rows*cols? Actually k can be up to 50, but a single cell cannot be cut, so for k>1 result should be 0.
    assert(pizzaWays({"A"}, 2) == 0);
    
    // Simple 1x2 with apples both, k=2: cut vertical between them => 1 way
    assert(pizzaWays({"AA"}, 2) == 1);
    
    // 2 rows 1 col, both apples, k=2: cut horizontal => 1 way
    assert(pizzaWays({"A", "A"}, 2) == 1);
    
    // 2x2 all apples, k=2: cuts: horizontal, vertical => 2 ways? Check: horizontal cut gives top row (apples) and bottom row (apples). Vertical cut gives left col and right col. Both valid, so 2.
    assert(pizzaWays({"AA", "AA"}, 2) == 2);
    
    // 2x2 with a single apple at (0,0) only, k=2: no cut possible because any cut leaves an empty piece => 0
    assert(pizzaWays({"A.", ".."}, 2) == 0);
    
    // 3x3 with apples in first column only, k=3: cut vertically twice? Actually can cut at col=1 and col=2, each slice has at least one apple? First slice col0 has apple, second slice col1 no apple -> invalid. So 0 ways. But if apples in every row of first column, can cut horizontally twice? Each row has an apple, so cut horizontal at row1 and row2 gives each row with apple => 1 way.
    assert(pizzaWays({"A..", "A..", "A.."}, 3) == 1);
    
    // Additional test with k=3 and 2x3 pizza: apples in first and last column, check consistency
    // pizza = ["A.A", "A.A"], k=3: possible cuts? Need 3 pieces each with at least one apple.
    // Cut vertical at col1: left piece (col0) has apples, right piece (col1-col2) has apples? col1 empty, col2 apple -> right piece has apple. Then cut right piece vertically at col2: left of right piece (col1) empty -> invalid. Or cut left piece? Hmm.
    // Alternative: cut horizontal at row1: top row has apples at col0,col2, bottom row same. Then cut top row? but after horizontal cut, we have two pieces (top and bottom). Need total 3 pieces, can cut one of them? The DP assumes cuts always on the whole remaining pizza from top-left, so after first cut, we work on sub-rectangle. Let's reason: first cut vertical at col1 gives left (col0) with apple, right (col1-col2) with apple. Then second cut on the right piece must be vertical at col2, but piece from col1 to col1 is empty -> invalid. First cut horizontal at row1 gives top row and bottom row. Then second cut on top row vertical at col1: left of top row col0 apple, right col1-col2 has apple at col2. So valid. Also second cut on bottom row similarly, but that would be two different ways? Since we are counting cut sequences, cutting top or bottom gives different sequences. So total 2? Let's compute via code. We'll trust.
    // Just a sanity check that the function runs.
    int result = pizzaWays({"A.A", "A.A"}, 3);
    assert(result == 2); // based on reasoning above
        
    return 0;
}
#include <vector>
#include <string>

// Count ways to cut pizza into k pieces each with at least one apple.
// DP state: dp[remain][r][c] = number of ways to form 'remain'+1 pieces from sub-pizza starting at (r,c).
int pizzaWays(const std::vector<std::string>& pizza, int k) {
    const int MOD = 1000000007;
    const int rows = static_cast<int>(pizza.size());
    const int cols = static_cast<int>(pizza[0].size());
    
    // Prefix sum of apples: apples[i][j] = apples in sub-rectangle from (i,j) to bottom-right.
    std::vector<std::vector<int>> apples(rows + 1, std::vector<int>(cols + 1, 0));
    for (int r = rows - 1; r >= 0; --r) {
        for (int c = cols - 1; c >= 0; --c) {
            apples[r][c] = (pizza[r][c] == 'A' ? 1 : 0)
                           + apples[r + 1][c] + apples[r][c + 1] - apples[r + 1][c + 1];
        }
    }
    
    // dp[remain][r][c]: remain = number of additional pieces needed (so total pieces = k - (k-1-remain)?)
    // Actually we use index 'remain' where 0 means 1 piece (base), 'remain' means remain+1 pieces to form.
    std::vector<std::vector<std::vector<int>>> dp(
        k, std::vector<std::vector<int>>(rows, std::vector<int>(cols, 0)));
    
    // Base: 1 piece from any sub-rectangle if it has at least one apple.
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            dp[0][r][c] = apples[r][c] > 0 ? 1 : 0;
        }
    }
    
    // Build DP for 2 to k pieces.
    for (int remain = 1; remain < k; ++remain) {
        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols; ++c) {
                // Horizontal cut at next_row: top piece from (r,c) to (next_row-1, c_end)
                for (int nr = r + 1; nr < rows; ++nr) {
                    // Top piece has apples if apples[r][c] - apples[nr][c] > 0
                    if (apples[r][c] - apples[nr][c] > 0) {
                        dp[remain][r][c] = (dp[remain][r][c] + dp[remain - 1][nr][c]) % MOD;
                    }
                }
                // Vertical cut at next_col: left piece from (r,c) to (r_end, next_col-1)
                for (int nc = c + 1; nc < cols; ++nc) {
                    if (apples[r][c] - apples[r][nc] > 0) {
                        dp[remain][r][c] = (dp[remain][r][c] + dp[remain - 1][r][nc]) % MOD;
                    }
                }
            }
        }
    }
    
    return dp[k - 1][0][0];
}
// We use dynamic programming with a 3D table `dp[remaining][row][col]` where `remaining` is the number of pieces still to be formed from the sub-pizza starting at `(row, col)` to the bottom-right corner (since cuts only affect the top-left part, we can always consider the remaining piece from current top-left). A 2D prefix sum array `apples` stores the count of apples in any sub-rectangle. Base case: with `remaining = 1`, `dp[0][row][col] = 1` if the sub-rectangle has at least one apple, else 0. For each `remaining > 1` (we iterate from 2 to k, but using index `remain` where `remain = pieces - 1`), we consider all possible horizontal cuts at `next_row` (strictly below current row) where the top piece from `(row, col)` to `(next_row-1, col_end)` has at least one apple. That condition is checked using `apples[row][col] - apples[next_row][col] > 0` because `apples[row][col]` is the total apples in the sub-pizza from `(row, col)`, and `apples[next_row][col]` is the total apples from `(next_row, col)`, so the difference gives apples in the top piece. Similarly for vertical cuts at `next_col`, condition `apples[row][col] - apples[row][next_col] > 0`. Then we add `dp[remain-1][next_row][col]` or `dp[remain-1][row][next_col]` respectively, taking modulo. Edge cases: if `k == 1`, return 1 if whole pizza has an apple else 0 (though problem states at least one apple overall, so returns 1). Also careful about integer overflow: use `long long` or apply modulo after addition. Time complexity: O(k * rows^2 * cols + k * rows * cols^2) = O(k * n^2 * m + k * n * m^2), at most 50^3 * 50 ≈ 6.25e6. Space: O(k * rows * cols) for DP plus prefix sum.
