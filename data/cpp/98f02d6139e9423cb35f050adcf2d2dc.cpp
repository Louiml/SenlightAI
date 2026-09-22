// Write a C++ function `countOilDeposits(const vector<string>& grid)` that takes a rectangular grid of characters where `'@'` represents oil and `'*'` (or any other character) represents empty land. Two oil cells belong to the same deposit if they are adjacent horizontally, vertically, or diagonally (8 directions). The function must return the total number of distinct oil deposits in the grid. The grid will have at least 1 row and 1 column, and may contain multiple deposits. The function should not modify the input grid. For example, a 3×3 grid with three `'@'` in a diagonal line counts as one deposit because they are diagonally adjacent.

#include <cassert>
#include <vector>
#include <string>

int main() {
    // Single oil cell
    assert(countOilDeposits({"@"}) == 1);
    // No oil
    assert(countOilDeposits({"***", "***"}) == 0);
    // 1xN horizontal line
    assert(countOilDeposits({"@*@@"}) == 2);
    // Diagonal adjacency counts as one deposit
    assert(countOilDeposits({"@**", "*@*", "**@"}) == 1);
    // Two separate deposits: one horizontal, one vertical
    assert(countOilDeposits({"@@*", "***", "@*@"}) == 2);
    // Larger grid with multiple isolated deposits
    assert(countOilDeposits({"@*@", "*@*", "@*@"}) == 5);
    // Mixed with empty rows
    assert(countOilDeposits({"@*@*", "****", "*@@*", "@**@"}) == 3);
    // All oil in 3x3 fully connected (all neighbors) -> 1 deposit
    assert(countOilDeposits({"@@@", "@@@", "@@@"}) == 1);
    // Single column
    assert(countOilDeposits({"@", "*", "@"}) == 2);
    // Single row with two deposits separated
    assert(countOilDeposits({"@*@*@"}) == 3);
    return 0;
}

#include <vector>
#include <string>
#include <functional>

// Count the number of distinct oil deposits in the grid.
// Oil is represented by '@', empty land by any other character.
// 8-directional adjacency (horizontal, vertical, diagonal).
int countOilDeposits(const std::vector<std::string>& grid) {
    if (grid.empty() || grid[0].empty()) return 0;
    const int rows = static_cast<int>(grid.size());
    const int cols = static_cast<int>(grid[0].size());

    std::vector<std::vector<bool>> visited(rows, std::vector<bool>(cols, false));

    // Direction offsets for 8 neighbors
    const int dr[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
    const int dc[8] = {-1, 0, 1, -1, 1, -1, 0, 1};

    // Depth-first search to mark all connected '@' cells
    std::function<void(int, int)> dfs = [&](int r, int c) {
        // Mark as visited
        visited[r][c] = true;
        for (int k = 0; k < 8; ++k) {
            int nr = r + dr[k];
            int nc = c + dc[k];
            // Check bounds and unvisited oil cell
            if (nr >= 0 && nr < rows && nc >= 0 && nc < cols &&
                !visited[nr][nc] && grid[nr][nc] == '@') {
                dfs(nr, nc);
            }
        }
    };

    int count = 0;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (grid[i][j] == '@' && !visited[i][j]) {
                // Found a new deposit
                ++count;
                dfs(i, j);
            }
        }
    }
    return count;
}

// The problem is a classic connected-components count on a 2D grid with 8-directional adjacency. We can solve it using Depth-First Search (DFS) or Breadth-First Search (BFS). The approach: iterate over every cell. When we find an unvisited `'@'`, we start a DFS from that cell, marking all reachable `'@'` cells (via the 8 directions) as visited, and increment the deposit counter. This ensures each connected component is counted exactly once. Edge cases: the grid dimensions are at least 1, but we must handle single-cell grids, grids with no oil (return 0), and grids where deposits touch only diagonally (they count as one). Time complexity is O(R×C) because each cell is visited at most once. Space complexity is O(R×C) in the worst case for the visited matrix, plus recursion stack depth up to R×C in the worst case, but typically we can use an iterative BFS to avoid deep recursion. For safety, we can implement the DFS iteratively or use BFS with a queue. In the solution, we’ll use a `visited` vector of bools and a recursive DFS (or iterative) that respects boundaries and only moves to cells containing `'@'` and not yet visited.
