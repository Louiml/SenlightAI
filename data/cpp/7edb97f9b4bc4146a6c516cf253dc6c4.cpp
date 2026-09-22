/*
Given an initially filled N x M grid of integers, a starting position at the top-left cell (0,0), and a string S consisting only of the characters 'D' and 'R', write a C++ function `solveGrid` that processes the grid according to the following rules. The function takes as parameters: the number of rows N, the number of columns M, the string S, and a 2D vector `grid` representing the initial integer grid. The function must modify the grid in-place and return the final grid as a 2D vector of long long integers. At each step, when the current position is at (x, y) and there are remaining characters in S: if the current character is 'D', set `grid[x][y]` to the negative of the sum of all numbers in column y (including the current value), then move down to (x+1, y); if the current character is 'R', set `grid[x][y]` to the negative of the sum of all numbers in row x (including the current value), then move right to (x, y+1). When S is exhausted (i.e., when the index equals S.size()), set the current cell to the negative of the sum of its entire row, and stop. The path is guaranteed to stay within grid bounds, and N, M, and the length of S will be such that the path respects the rules. The grid values can be large, so use 64-bit integers. You must implement this logic exactly as described, preserving the grid's original state when calculating sums for each step (i.e., do not modify other cells before they are processed; sums are computed from the current unmodified grid, except that previously assigned negative values from earlier steps remain in the grid and may affect sums of rows/columns that include those cells).
*/
#include <vector>
#include <string>
#include <numeric>

// Process the grid according to the described path traversal.
// Parameters: N rows, M columns, move string S, and the grid (modified in-place).
// Returns the final grid as a 2D vector of long long.
std::vector<std::vector<long long>> solveGrid(
    int N, int M, const std::string& S, std::vector<std::vector<long long>> grid) {
    
    // Helper lambda for recursive processing.
    // Captures grid, N, M, S by reference.
    std::function<void(int, int, int)> calc = [&](int x, int y, int idx) {
        // Compute sums from the current grid state.
        long long rsum = 0, csum = 0;
        for (int i = 0; i < M; ++i) rsum += grid[x][i];       // row sum
        for (int i = 0; i < N; ++i) csum += grid[i][y];       // column sum

        if (idx == (int)S.size()) {
            // Terminal: set to negative row sum.
            grid[x][y] = -rsum;
            return;
        }

        if (S[idx] == 'D') {
            grid[x][y] = -rsum;
            calc(x + 1, y, idx + 1);
        } else { // 'R'
            grid[x][y] = -csum;
            calc(x, y + 1, idx + 1);
        }
    };

    calc(0, 0, 0);
    return grid;
}
#include <cassert>
#include <vector>
#include <string>

// The solution function is assumed to be defined above.
// Include the function here for completeness (or include the header).

int main() {
    // Test 1: 1x1 grid, empty S (but typically S length=0? Actually path length may be 0)
    {
        std::vector<std::vector<long long>> grid = {{5}};
        auto res = solveGrid(1, 1, "", grid);
        assert(res.size() == 1 && res[0].size() == 1);
        assert(res[0][0] == -5); // row sum of 5 = 5, negative = -5
    }

    // Test 2: 2x2 grid, S="D" (move down once)
    {
        std::vector<std::vector<long long>> grid = {{1,2},{3,4}};
        auto res = solveGrid(2, 2, "D", grid);
        // Step 0: (0,0), S[0]='D', row sum of row 0 = 1+2=3 => set grid[0][0]=-3, move to (1,0)
        // Step 1: idx=1==S.size(), terminal at (1,0), row sum of row 1 = 3+4=7 => set grid[1][0]=-7
        assert(res[0][0] == -3);
        assert(res[0][1] == 2);
        assert(res[1][0] == -7);
        assert(res[1][1] == 4);
    }

    // Test 3: 2x2 grid, S="R" (move right once)
    {
        std::vector<std::vector<long long>> grid = {{1,2},{3,4}};
        auto res = solveGrid(2, 2, "R", grid);
        // (0,0): S[0]='R', column sum of col 0 = 1+3=4 => set grid[0][0]=-4, move to (0,1)
        // (0,1): terminal, row sum of row 0 = 1+2? Wait grid[0][0] is now -4, so row sum = -4+2 = -2 => set grid[0][1] = 2
        assert(res[0][0] == -4);
        assert(res[0][1] == 2);
        assert(res[1][0] == 3);
        assert(res[1][1] == 4);
    }

    // Test 4: 2x3 grid, S="RD" (right then down)
    {
        std::vector<std::vector<long long>> grid = {{1,2,3},{4,5,6}};
        auto res = solveGrid(2, 3, "RD", grid);
        // (0,0): R, col0 sum = 1+4=5 => grid[0][0]=-5, move (0,1)
        // (0,1): D, row0 sum = -5+2+3=0 => grid[0][1]=0, move (1,1)
        // (1,1): terminal, row1 sum = 4+5+6=15 => grid[1][1]=-15
        assert(res[0][0] == -5);
        assert(res[0][1] == 0);
        assert(res[0][2] == 3);
        assert(res[1][0] == 4);
        assert(res[1][1] == -15);
        assert(res[1][2] == 6);
    }

    // Test 5: 3x3 grid, S="DR" (down then right)
    {
        std::vector<std::vector<long long>> grid = {{1,2,3},{4,5,6},{7,8,9}};
        auto res = solveGrid(3, 3, "DR", grid);
        // (0,0): D, row0 sum = 1+2+3=6 => grid[0][0]=-6, move (1,0)
        // (1,0): R, col0 sum = -6+4+7=5 => grid[1][0]=-5, move (1,1)
        // (1,1): terminal, row1 sum = -5+5+6=6 => grid[1][1]=-6
        assert(res[0][0] == -6);
        assert(res[1][0] == -5);
        assert(res[1][1] == -6);
        assert(res[1][2] == 6);
        assert(res[0][1] == 2);
        assert(res[0][2] == 3);
        assert(res[2][0] == 7);
        assert(res[2][1] == 8);
        assert(res[2][2] == 9);
    }

    return 0;
}
// The algorithm simulates the described path traversal. We maintain two indices: the current row `x` and column `y`, both starting at 0, and an index `idx` into S starting at 0. At each call to the recursive helper (or iterative loop), we compute the sum of the current column (if the next move is 'R' or if we're at the end? Actually, the logic from the original snippet: when `idx < S.size()`, if S[idx]=='D', use row sum (i.e., negative of row sum) and move down; if S[idx]=='R', use column sum and move right. When `idx == S.size()`, use row sum and stop. So we need to understand: In the given code, `if(S[idx]=='D')` uses `-rsum` (row sum) and moves down; else uses `-csum` (column sum) and moves right. At the end (`idx==S.size()`), uses `-rsum`. So essentially, for a 'D' move, we set the current cell to negative row sum; for an 'R' move, we set to negative column sum; when finished, we set to negative row sum. After setting, we recursively process the next cell if not finished. The sums are computed on the current state of the board, which may have been partially modified by earlier steps (those earlier cells are already changed to negative values). This is important because those negative values affect subsequent row/column sums if they lie in the same row or column as the current position. The path always moves either down or right, so it never revisits a cell. The grid dimensions and S length ensure that the path never goes out of bounds. Time complexity is O(N*M) for the initial sum calculations, but note that we recompute row/column sums for each step, leading to O((N+M)*L) where L is the length of S, but since we only visit L+1 cells (the last is the terminal), and each sum calculation takes O(N) or O(M), worst-case is O((N+M)*L). In practice, N and M are at most 1000 and L is at most N+M-2, so O((N+M)^2) which is fine. Space complexity is O(1) auxiliary (excluding the grid storage). Edge cases: when N=1 or M=1, the path must be consistent with the allowed moves; the terminal step uses row sum regardless of whether the last character was 'D' or 'R'. Also ensure we use long long to avoid overflow.
