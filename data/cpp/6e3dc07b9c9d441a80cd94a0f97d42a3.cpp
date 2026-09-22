Write a C++ function `int maxCollectedFruits(vector<vector<int>>& fruits)` that, given an `n x n` grid `fruits` where each cell contains a non-negative integer, returns the maximum total fruits that can be collected under the following rules. Three players start simultaneously: Player A starts at the top-left cell `(0,0)`, Player B starts at the top-right cell `(0,n-1)`, and Player C starts at the bottom-left cell `(n-1,0)`. Each player moves one cell down per turn (row index increases by 1 each step). Player A must stay on the main diagonal (i.e., can only move from `(i,i)` to `(i+1,i+1)`). Player B can move from `(i,j)` to `(i+1,j-1)`, `(i+1,j)`, or `(i+1,j+1)` (as long as within bounds) and must end at the bottom-right cell `(n-1,n-1)`. Player C can move from `(i,j)` to `(i+1,j-1)`, `(i+1,j)`, or `(i+1,j+1)` and must also end at `(n-1,n-1)`. The players collect fruits from every cell they step on, but if two or more players collect from the same cell, the fruits are counted only once (i.e., do not double-count). All players move exactly `n-1` steps and reach the bottom-right cell simultaneously. The function should return the maximum possible total fruits collected. The grid size `n` satisfies `1 <= n <= 1000`, and each fruit count satisfies `0 <= fruits[i][j] <= 10^6`. For `n=1`, all three start and end at the same cell, so the answer is simply `fruits[0][0]`. Note that the paths of B and C are symmetric via the main diagonal, and the two paths cannot overlap on any intermediate cell (excluding start and end) because that would cause double-counting issues; however, the problem reduces to two independent path maximizations from top-right to bottom-left (after transposing) and from bottom-left to top-right. To avoid double-counting, the algorithm fixes the main diagonal fruits (collected by A) separately, then transforms the grid by transposing it and computes the maximum path for two players moving from the top-right to the bottom-right (or equivalently from top-right to bottom-left after rotation) while never sharing a cell, which can be decoupled due to symmetry: we compute the maximum path from `(0,n-1)` to `(n-1,n-1)` using a dynamic programming approach that moves downward, and then we transpose the grid and compute the maximum path from the new top-right to the new bottom-right; the sum of these two path values (excluding the already-counted diagonal) gives the answer. Your implementation must be efficient for `n` up to 1000, so use `O(n^2)` time and `O(n)` auxiliary space (per path computation). Include necessary headers and use `const` correctness where appropriate.
// The key insights: 
// 1. Player A is forced to move along the main diagonal, so its contribution is simply the sum of all diagonal elements `fruits[i][i]`.
// 2. Players B and C must each move from their respective corners to the bottom-right cell. Their movement rules are identical (down, left/right/down). Notice that if we transpose the grid (swap `fruits[i][j]` with `fruits[j][i]`), then the path for Player C becomes equivalent to a path from the top-right corner of the transposed grid to the bottom-right corner. Thus both players’ problems become: given a grid, find the maximum sum path from top-right `(0,n-1)` to bottom-right `(n-1,n-1)` moving only downward (row+1) and horizontally within the same row (col-1, col, or col+1) per step. However, since the two players would overlap on the diagonal and other cells if not careful, but the problem statement implicitly ensures that the optimal paths for B and C do not conflict because the fruits are counted only once; nevertheless, the standard solution (as in the provided snippet) decouples them: it first adds the diagonal, then computes the maximum path from top-right to bottom-right in the original grid (which corresponds to Player B), then transposes the grid and computes the same maximum path again (which corresponds to Player C). The reason this is correct is that for a symmetric grid, the maximum path for B and the maximum path for C (after transposition) will not share any off-diagonal cell in the optimal solution, because if they did, one could modify one path to avoid the shared cell without decreasing the total sum (the shared cell would be counted once anyway, so the sum is simply the sum of both paths minus the shared cells; but since both paths are independently maximized, and the grid is symmetric, the optimal solution will have B and C paths that are mirror images across the diagonal, thus only intersecting on the diagonal, which is already counted). The provided snippet's algorithm does exactly that: it computes the maximum path for B using dynamic programming with a rolling array (prev, curr) of size n, updating each row. The DP state `prev[i]` stores the maximum sum to reach cell `(row-1, i)`. For each new row, we compute `curr[i]` as the maximum of `prev[i-1]`, `prev[i]`, `prev[i+1]` plus the fruit at `(row, i)`, handling boundaries. The starting state is `prev[n-1] = fruits[0][n-1]` (top-right). The final answer after processing rows 1 to n-1 is `prev[n-1]` (bottom-right). Then we transpose the grid (swap upper triangle with lower triangle) and repeat the same DP to get the second player's contribution. The total answer is diagonal sum + first DP result + second DP result. Edge cases: n=1 (diagonal sum is the only cell, and both DP returns 0 because there are no steps; the loop for rows runs from 1 to n-1, which is none, so `prev[n-1]` remains `fruits[0][n-1]`? Actually careful: for n=1, the initial `prev[n-1] = fruits[0][0]`, but the loop doesn't run, and then `ans += prev[n-1]` would add it again, leading to double count. So the implementation must handle n=1 specially or initialize differently. In the given snippet, for n=1, the loop from row=1 to row<0 doesn't run, so `prev` is size 1 with `prev[0]=fruits[0][0]`, then `ans += prev[0]` would add fruits[0][0] again, wrong. So we must add a special case: if n==1, return fruits[0][0]. Alternatively, set initial `prev` to 0 for the first DP and add fruits[0][n-1] separately? The provided snippet appears flawed for n=1, so our solution must handle it. Also, the DP uses -1 to indicate unreachable cells, which works because fruits are non-negative. Time complexity: Two DP passes, each O(n^2) time and O(n) space per pass (rolling array). Transposing the grid is O(n^2). Overall O(n^2) time and O(n) auxiliary space (excluding input grid). 
// Important edge cases: n=1, n=2 (small grids), and grids where some paths are forced.
// The reference solution will: if n==1 return fruits[0][0]; else compute diagonal sum; then compute max path from top-right to bottom-right using a helper function that takes the grid and returns the maximum sum; then transpose the grid; compute again; add all three; return total.
#include <vector>
#include <algorithm>

// Helper: compute maximum sum path from top-right (0,n-1) to bottom-right (n-1,n-1)
// moving down each step, with horizontal moves within the same row.
// Grid is n x n, fruits are non-negative.
int maxDownwardPath(const std::vector<std::vector<int>>& grid) {
    int n = grid.size();
    if (n == 1) return grid[0][0]; // edge case, but caller handles n=1 separately.

    // prev[i] = max sum to reach cell (row-1, i)
    std::vector<int> prev(n, -1);
    prev[n-1] = grid[0][n-1];

    for (int row = 1; row < n; ++row) {
        std::vector<int> curr(n, -1);
        for (int col = 0; col < n; ++col) {
            if (prev[col] < 0) continue;
            // Move to same column
            if (prev[col] >= 0) {
                curr[col] = std::max(curr[col], prev[col] + grid[row][col]);
            }
            // Move left (col-1)
            if (col > 0) {
                curr[col-1] = std::max(curr[col-1], prev[col] + grid[row][col-1]);
            }
            // Move right (col+1)
            if (col < n-1) {
                curr[col+1] = std::max(curr[col+1], prev[col] + grid[row][col+1]);
            }
        }
        prev = std::move(curr);
    }
    return prev[n-1];
}

// Main function: returns maximum total fruits collected by three players.
int maxCollectedFruits(std::vector<std::vector<int>>& fruits) {
    int n = fruits.size();
    if (n == 1) return fruits[0][0];

    int ans = 0;
    // Player A: main diagonal sum
    for (int i = 0; i < n; ++i) ans += fruits[i][i];

    // Player B: from top-right to bottom-right
    ans += maxDownwardPath(fruits);

    // Transpose the grid for Player C
    for (int i = 0; i < n; ++i) {
        for (int j = i+1; j < n; ++j) {
            std::swap(fruits[i][j], fruits[j][i]);
        }
    }

    // Player C: now from top-right to bottom-right of transposed grid
    ans += maxDownwardPath(fruits);

    return ans;
}
#include <cassert>
#include <vector>

int maxCollectedFruits(std::vector<std::vector<int>>& fruits);

int main() {
    // Test 1: n=1
    std::vector<std::vector<int>> f1 = {{5}};
    assert(maxCollectedFruits(f1) == 5);

    // Test 2: n=2
    std::vector<std::vector<int>> f2 = {{1,2},{3,4}};
    assert(maxCollectedFruits(f2) == 10); // 1+2+3+4 =10

    // Test 3: n=3, all ones
    std::vector<std::vector<int>> f3 = {{1,1,1},{1,1,1},{1,1,1}};
    // Diagonal: 1+1+1=3
    // B path from (0,2): can go to (1,1) or (1,2); to reach row 1 col 2, choose (0,2)->(1,2) sum=2
    // Transpose same, C path sum=2, total=7
    assert(maxCollectedFruits(f3) == 7);

    // Test 4: n=3 with higher values off-diagonal
    std::vector<std::vector<int>> f4 = {{0,10,0},{0,0,20},{0,0,0}};
    // Diagonal: 0+0+0=0
    // B: top-right (0,2)=0, then row1 can go to col1 (0) or col2 (20) or col3 (none). To maximize, go (0,2)->(1,2)=20, so prev[2]=20. Return 20.
    // Transpose: [[0,0,0],[10,0,0],[0,20,0]] -> top-right (0,2)=0, then row1 can go to col1 (0) or col2 (0) or col3? Actually from (0,2), col-1=1, col=2, col+1=3 out. So to row1 col1=0, col2=0, max=0, then to row n-2=1 col2? Actually row1 is the only intermediate (since n-1=2, so rows 1..n-2 = row1 only). So prev after row1: from (0,2) we can go to (1,1)=0 or (1,2)=0, so prev[2]=0? Wait, we need to reach column n-1=2 at row n-2=1. From (0,2) with fruit 0, we can move down to (1,2) adding fruit[1][2]=0, so max 0. So second path=0. Total=0.
    assert(maxCollectedFruits(f4) == 20); // Actually B collects 20, A collects 0, C collects 0 => 20.

    // Test 5: n=4, simple symmetric grid with some values
    std::vector<std::vector<int>> f5 = {{1,2,3,4},
                                        {5,6,7,8},
                                        {9,10,11,12},
                                        {13,14,15,16}};
    // Compute manually? We'll trust the algorithm and just check it runs and gives plausible result.
    // Diagonal sum: 1+6+11+16 = 34
    // B path: start (0,3)=4, then row1 (index1) can go to (1,2)=7, (1,3)=8, (1,4?) none. To reach row n-2=2, col n-1=3. 
    // DP: row1: from (0,3) -> (1,2): +7 =11, (1,3):+8=12, (1,4) none. So prev after row1: [_, _, 11, 12]
    // row2 (since n-1=3, loop row=1 to row<3 => row=1,2). For row2 (index2): from (1,2)=11 -> (2,1):+10=21, (2,2):+11=22, (2,3):+12=23. From (1,3)=12 -> (2,2):+11=23, (2,3):+12=24, (2,4) none. So prev[3] becomes max(23,24)=24. So first path returns 24.
    // Transpose grid: f5_T = [[1,5,9,13],[2,6,10,14],[3,7,11,15],[4,8,12,16]]
    // B' path from top-right (0,3)=13, row1: to (1,2)=10 ->23, (1,3)=14->27. row2: from (1,3)=27 -> (2,2)=11->38, (2,3)=15->42. So prev[3]=42. Total =34+24+42=100.
    assert(maxCollectedFruits(f5) == 100);

    return 0;
}
