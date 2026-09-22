Write a C++ function `std::string gameWinner(int n, int m, const std::vector<std::vector<int>>& grid)` that determines the winner of a two-player game played on an `n x m` binary matrix (each cell is either 0 or 1). Players alternate turns starting with the first player. On each turn, a player must choose a cell that currently contains 0 and whose entire row and entire column both contain only zeros (i.e., no other 1 in that row or column). The player sets that cell to 1. A player who cannot make a move loses the game. The function should return the string `"Ashish"` if the first player wins under optimal play, or `"Vivek"` if the second player wins. Assume `n`, `m` are positive integers, and `grid` has exactly `n` rows and `m` columns. The game is impartial with normal play (last player to move wins). Each move removes at least one row and one column from future consideration (since that row and column now contain a 1, making any zero in them invalid for future moves). The function must simulate the game deterministically: on each turn, the player scans the grid in row-major order and picks the first valid zero cell found, then marks it as 1. If no valid cell exists, the player loses. Return the winner based on that deterministic play.
#include <cassert>
#include <string>
#include <vector>

// Include the solution function here (e.g., copy above).

int main() {
    // Test 1: No initial 1s, 1x1 grid -> first player must place a 1, then second has no move -> Ashish wins.
    {
        std::vector<std::vector<int>> grid = {{0}};
        assert(gameWinner(1, 1, grid) == "Ashish");
    }
    // Test 2: 2x2 all zeros -> moves: (0,0), then (1,1), then no moves -> 2 moves, second player (Vivek) wins.
    {
        std::vector<std::vector<int>> grid = {{0,0},{0,0}};
        assert(gameWinner(2, 2, grid) == "Vivek");
    }
    // Test 3: 1x2 all zeros -> first takes (0,0), second has no move because column 1 is still zero? Wait: after blocking row 0 and col 0, cell (0,1) is blocked by row 0, so no moves -> first player wins.
    {
        std::vector<std::vector<int>> grid = {{0,0}};
        assert(gameWinner(1, 2, grid) == "Ashish");
    }
    // Test 4: Already has a 1 at (0,0), so no zero cells with free row and col -> first player loses.
    {
        std::vector<std::vector<int>> grid = {{1,0},{0,0}};
        assert(gameWinner(2, 2, grid) == "Vivek");
    }
    // Test 5: 3x3 with a 1 at (1,1) -> rows 1 and col 1 blocked, so zero cells available: (0,0), (0,2), (2,0), (2,2). First takes (0,0), blocking row0, col0. Then second takes (2,2) (first free in row-major after row0 blocked? Let's simulate: after first move, blocked rows = {0,1}, cols={0,1}. Scan row2: col2 zero, free -> second takes (2,2), blocking row2,col2. Then no moves -> third player? Actually after second move, all rows blocked, so third (Ashish again) can't move, so Vivek wins. Let's see: moves = Ashish at (0,0), Vivek at (2,2), then Ashish can't move -> Vivek wins.
    {
        std::vector<std::vector<int>> grid = {{0,0,0},{0,1,0},{0,0,0}};
        assert(gameWinner(3, 3, grid) == "Vivek");
    }
    // Test 6: 2x3 all zeros -> first takes (0,0), blocking row0 and col0. Then second takes (1,1) (because row1, col1 zero; row1 not blocked, col1 not blocked). After that, row1 and col1 blocked, so no moves remain (row0 blocked, row1 blocked). So second player (Vivek) made the last move -> Ashish loses, so Vivek wins.
    {
        std::vector<std::vector<int>> grid = {{0,0,0},{0,0,0}};
        assert(gameWinner(2, 3, grid) == "Vivek");
    }
    // Test 7: 3x2 all zeros -> first takes (0,0), blocks row0,col0. Second takes (1,1), blocks row1,col1. Now no cells: row2 free? Wait row2 is not blocked, but col0 and col1 are blocked, so cells in row2 have either col0 or col1, both blocked, so no moves. So after 2 moves, third player (Ashish) can't move -> Vivek wins.
    {
        std::vector<std::vector<int>> grid = {{0,0},{0,0},{0,0}};
        assert(gameWinner(3, 2, grid) == "Vivek");
    }
    // Test 8: Single row with multiple zeros and a 1 at the end -> row is blocked, so no moves at all -> first loses.
    {
        std::vector<std::vector<int>> grid = {{0,0,1}};
        assert(gameWinner(1, 3, grid) == "Vivek");
    }
    // Test 9: Single column with zeros -> first takes (0,0), blocks col0, so no moves -> Ashish wins.
    {
        std::vector<std::vector<int>> grid = {{0},{0},{0}};
        assert(gameWinner(3, 1, grid) == "Ashish");
    }
    // Test 10: 4x4 with a few 1s that do not fully block everything -> simulate carefully.
    {
        // setup: 1 at (0,1) and (1,0). Row0 and col1 blocked; row1 and col0 blocked.
        // Free rows: 2,3; free cols: 2,3. Cell (2,2) zero -> Ashish takes it, blocks row2,col2.
        // Then free rows: 3; free cols: 3. Cell (3,3) zero -> Vivek takes it, blocks row3,col3.
        // Then no moves -> Ashish loses -> Vivek wins.
        std::vector<std::vector<int>> grid = {
            {0,1,0,0},
            {1,0,0,0},
            {0,0,0,0},
            {0,0,0,0}
        };
        assert(gameWinner(4, 4, grid) == "Vivek");
    }
}
#include <string>
#include <vector>

std::string gameWinner(int n, int m, const std::vector<std::vector<int>>& grid) {
    std::vector<bool> blockedRow(n, false);
    std::vector<bool> blockedCol(m, false);
    // Mark rows and columns that already have a 1 from the initial grid.
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (grid[i][j] == 1) {
                blockedRow[i] = true;
                blockedCol[j] = true;
            }
        }
    }

    bool ashishTurn = true; // true = Ashish, false = Vivek

    while (true) {
        bool moved = false;
        // Find the first valid cell in row-major order.
        for (int i = 0; i < n && !moved; ++i) {
            if (blockedRow[i]) continue;
            for (int j = 0; j < m; ++j) {
                if (blockedCol[j]) continue;
                if (grid[i][j] == 0) {
                    // Make the move: block this row and column.
                    blockedRow[i] = true;
                    blockedCol[j] = true;
                    moved = true;
                    break;
                }
            }
        }
        if (!moved) {
            // Current player cannot move, so the other player wins.
            return ashishTurn ? "Vivek" : "Ashish";
        }
        ashishTurn = !ashishTurn;
    }
}
// The game is impartial with normal play, but the rule that on each move you must pick the *first* valid zero in row-major order makes the outcome deterministic—there is no choice. Therefore, we can simulate exactly: at each turn, recompute row sums and column sums. A cell `(i,j)` is playable if `rowSum[i] == 0` and `colSum[j] == 0` and `grid[i][j] == 0`. Since we always pick the first such cell in row-major order, we just scan rows from 0 to n-1 and columns from 0 to m-1, and the first cell satisfying the condition is chosen. If no such cell is found, the current player loses and the other player wins. After placing a 1, the row and column sums are updated efficiently. Because each move permanently eliminates at least that row and column from future play, the number of moves is at most `min(n, m)`. The simulation runs in `O(moves * (n*m))` time in the worst case if we recompute sums each turn, but we can optimize by maintaining running row and column sums and updating them in O(1) per move after initial O(n*m) computation. However, the simple recompute version is also fine for small constraints; but we can do better: initialize rowSums and colSums arrays with the initial sums. On each move, after placing a 1 at (i,j), decrement rowSums[i] by 1 and colSums[j] by 1. Note that since the cell was 0 and its row/col sums were 0, after setting to 1 the rowSums[i] becomes 1 and colSums[j] becomes 1, which correctly indicates they are no longer all-zero. So we don't even need to maintain sums for future check—just check the original grid for zeros and the sums arrays. However, we can simply use the original grid and a boolean array of "blocked" rows/columns. Because we only care if row i has any 1 in the *original* grid, but after we place a 1 in row i, that row is blocked forever. Similarly for columns. So we maintain `blockedRow[i]` and `blockedCol[j]` initially false. A cell (i,j) with grid[i][j] == 0 is playable iff `!blockedRow[i] && !blockedCol[j]`. After making a move, set both blockedRow[i] and blockedCol[j] to true. This is O(1) per check, and the scan is O(n*m) per move, with at most min(n,m) moves, so worst-case O(min(n,m)*n*m). Since the player picks the first in row-major order, the simulation is deterministic. Edge cases: there may already be initial 1s, blocking those rows/columns immediately. If no valid move at start, first player loses (return "Vivek"). Also, if one dimension is zero? But constraints say positive. The winner alternates each turn: start with `turn = 0` for Ashish, then flip after each successful move. If a player cannot move, the other wins. Complexity: O(min(n,m)*n*m) time and O(n+m) extra space for blocked arrays.
