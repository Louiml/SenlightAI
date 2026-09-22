// Write a C++ function `minimumCostPath` that takes a square matrix of non-negative integers (represented as a `std::vector<std::vector<int>>`) and returns the minimum cost to travel from the top-left cell `(0,0)` to the bottom-right cell `(n-1,n-1)`, where movement is allowed only right, down, or diagonally down-right. The cost of a path is the sum of the values of all cells visited. The matrix will have dimensions at least 1×1 and may contain zeros. The function should work for any square size, not just a fixed 3×3. Do not modify the input matrix. If the matrix is empty, return 0.
The problem is a classic dynamic programming (DP) task. Define `dp[i][j]` as the minimum cost to reach cell `(i,j)` from `(0,0)`. Base case: `dp[0][0] = matrix[0][0]`. For the first row (`i=0`), cells can only be reached from the left: `dp[0][j] = dp[0][j-1] + matrix[0][j]`. For the first column (`j=0`), cells can only be reached from above: `dp[i][0] = dp[i-1][0] + matrix[i][0]`. For every other cell `(i,j)`, the minimum cost is the minimum of the three possible predecessors (`dp[i-1][j-1]`, `dp[i-1][j]`, `dp[i][j-1]`) plus `matrix[i][j]`. The answer is `dp[n-1][n-1]`. Edge cases: a 1×1 matrix returns the single cell’s value; a matrix with zeros is fine because the DP still accumulates correctly; no negative values ensures no overflow concerns, but `int` is sufficient for typical inputs. Time complexity is `O(n^2)` for filling the DP table, and space complexity is `O(n^2)` as well. The implementation uses `const` references and iterates rows and columns in order, which is correct because each cell depends only on previously computed cells.
#include <vector>
#include <algorithm>
#include <limits>

// Returns the minimum cost to travel from top-left to bottom-right of a square matrix,
// moving only right, down, or diagonally down-right. The input is not modified.
int minimumCostPath(const std::vector<std::vector<int>>& matrix) {
    if (matrix.empty() || matrix[0].empty()) {
        return 0;
    }
    int n = static_cast<int>(matrix.size());
    std::vector<std::vector<int>> dp(n, std::vector<int>(n, 0));
    
    dp[0][0] = matrix[0][0];
    // Fill first row
    for (int j = 1; j < n; ++j) {
        dp[0][j] = dp[0][j-1] + matrix[0][j];
    }
    // Fill first column
    for (int i = 1; i < n; ++i) {
        dp[i][0] = dp[i-1][0] + matrix[i][0];
    }
    // Fill rest of the table
    for (int i = 1; i < n; ++i) {
        for (int j = 1; j < n; ++j) {
            int minPrev = std::min({dp[i-1][j-1], dp[i-1][j], dp[i][j-1]});
            dp[i][j] = minPrev + matrix[i][j];
        }
    }
    return dp[n-1][n-1];
}
#include <cassert>
#include <vector>

int main() {
    // Test 1: Provided 3x3 example, expected cost = 8
    std::vector<std::vector<int>> m1 = {{1,2,3},{4,8,2},{1,5,3}};
    assert(minimumCostPath(m1) == 8);

    // Test 2: Single cell
    std::vector<std::vector<int>> m2 = {{5}};
    assert(minimumCostPath(m2) == 5);

    // Test 3: Simple 2x2
    std::vector<std::vector<int>> m3 = {{1,2},{3,4}};
    // Paths: 1->2->4 = 7, 1->3->4 = 8, 1->4 (diag) = 5
    assert(minimumCostPath(m3) == 5);

    // Test 4: All zeros
    std::vector<std::vector<int>> m4 = {{0,0,0},{0,0,0},{0,0,0}};
    assert(minimumCostPath(m4) == 0);

    // Test 5: Larger matrix with zeros mixed
    std::vector<std::vector<int>> m5 = {{1,2,3,4},{0,5,6,7},{8,9,0,1},{2,3,4,5}};
    // Manually compute expected via DP: dp[0][0]=1, dp row0: 1,3,6,10
    // col0: 1,1,9,11
    // dp[1][1]=min(1,1,3)+5=6 ; dp[1][2]=min(3,6,6)+6=12 ; dp[1][3]=min(6,12,10)+7=17
    // dp[2][1]=min(6,1,9)+9=15 ; dp[2][2]=min(12,6,15)+0=6 ; dp[2][3]=min(17,12,17)+1=13
    // dp[3][1]=min(15,11,9)+3=14 ; dp[3][2]=min(6,15,14)+4=10 ; dp[3][3]=min(13,10,13)+5=15
    assert(minimumCostPath(m5) == 15);

    // Test 6: Empty matrix
    std::vector<std::vector<int>> m6;
    assert(minimumCostPath(m6) == 0);
}
