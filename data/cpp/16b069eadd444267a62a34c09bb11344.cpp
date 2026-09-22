/*
Write a C++ function `int countLandlockedCells(const std::vector<std::vector<int>>& grid)` that takes a binary grid (each cell is 0 for water and 1 for land) and returns the number of land cells that are not connected to any cell on the border of the grid through a path of adjacent (up, down, left, right) land cells. In other words, count the land cells that are completely surrounded by water or other land cells but cannot reach the outer boundary by moving only through land. The grid is rectangular with at least one row and one column. Cells on the border are considered connected to the outside, so any land cell that is on the border or reachable from a border land cell is not counted. Only land cells that are isolated from the border by water are counted.
*/

#include <vector>
#include <queue>
#include <utility>

// Returns the number of land cells that cannot reach the grid's border.
int countLandlockedCells(const std::vector<std::vector<int>>& grid) {
    if (grid.empty() || grid[0].empty()) {
        return 0;
    }

    int rows = static_cast<int>(grid.size());
    int cols = static_cast<int>(grid[0].size());

    // Work on a mutable copy so we can mark visited cells.
    std::vector<std::vector<int>> g = grid;

    std::queue<std::pair<int, int>> q;
    const int dirs[4][2] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};

    // Add all border land cells to the queue.
    for (int r = 0; r < rows; ++r) {
        if (g[r][0] == 1) {
            q.push({r, 0});
        }
        if (cols > 1 && g[r][cols - 1] == 1) {
            q.push({r, cols - 1});
        }
    }
    for (int c = 0; c < cols; ++c) {
        if (g[0][c] == 1) {
            q.push({0, c});
        }
        if (rows > 1 && g[rows - 1][c] == 1) {
            q.push({rows - 1, c});
        }
    }

    // BFS to mark all cells reachable from the border.
    while (!q.empty()) {
        auto [r, c] = q.front();
        q.pop();
        if (g[r][c] == 0) {
            continue;
        }
        g[r][c] = 0; // mark as visited (connected to border)

        for (int d = 0; d < 4; ++d) {
            int nr = r + dirs[d][0];
            int nc = c + dirs[d][1];
            if (nr >= 0 && nr < rows && nc >= 0 && nc < cols && g[nr][nc] == 1) {
                q.push({nr, nc});
            }
        }
    }

    // Count remaining land cells (landlocked).
    int count = 0;
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            if (g[r][c] == 1) {
                ++count;
            }
        }
    }
    return count;
}

#include <cassert>
#include <vector>

int main() {
    // 3x3 grid with center land cell completely surrounded.
    std::vector<std::vector<int>> grid1 = {
        {0, 0, 0},
        {0, 1, 0},
        {0, 0, 0}
    };
    assert(countLandlockedCells(grid1) == 1);

    // All land cells on border, none landlocked.
    std::vector<std::vector<int>> grid2 = {
        {1, 1, 1},
        {1, 0, 1},
        {1, 1, 1}
    };
    assert(countLandlockedCells(grid2) == 0);

    // A land cell connected to border via path, not landlocked.
    std::vector<std::vector<int>> grid3 = {
        {1, 0, 0},
        {1, 1, 0},
        {0, 0, 0}
    };
    assert(countLandlockedCells(grid3) == 0);

    // Landlocked cells isolated by water.
    std::vector<std::vector<int>> grid4 = {
        {0, 0, 0, 0, 0},
        {0, 1, 1, 0, 0},
        {0, 1, 1, 0, 0},
        {0, 0, 0, 0, 0}
    };
    assert(countLandlockedCells(grid4) == 4);

    // Single cell grid: border cell, so not landlocked.
    std::vector<std::vector<int>> grid5 = {{1}};
    assert(countLandlockedCells(grid5) == 0);

    // No land at all.
    std::vector<std::vector<int>> grid6 = {{0, 0}, {0, 0}};
    assert(countLandlockedCells(grid6) == 0);

    // 1xN grid: all cells are border, no landlocked.
    std::vector<std::vector<int>> grid7 = {{1, 0, 1, 1}};
    assert(countLandlockedCells(grid7) == 0);

    // Nx1 grid: all cells border.
    std::vector<std::vector<int>> grid8 = {{1}, {0}, {1}};
    assert(countLandlockedCells(grid8) == 0);

    // A larger grid with an interior pocket connected to border via a narrow path.
    std::vector<std::vector<int>> grid9 = {
        {1, 0, 0, 0},
        {1, 1, 0, 0},
        {0, 1, 1, 0},
        {0, 0, 1, 1}
    };
    // All land cells can reach border, so 0.
    assert(countLandlockedCells(grid9) == 0);

    // Mixed with a landlocked cell in the middle but not connected.
    std::vector<std::vector<int>> grid10 = {
        {0, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 1, 0},
        {0, 0, 0, 0}
    };
    // Two separate landlocked cells.
    assert(countLandlockedCells(grid10) == 2);

    return 0;
}

// The solution uses a breadth-first search (BFS) or depth-first search (DFS) approach starting from all land cells on the border. First, we initialize a queue with all border land cells (top row, bottom row, left column, right column). Then we perform a traversal (the provided snippet uses DFS recursively; an iterative BFS would avoid recursion depth issues) marking every visited land cell as 0 (or a separate visited flag) to indicate it is connected to the border. After the traversal, any remaining 1s in the grid are land cells that cannot reach the border and thus are landlocked. We count those and return the total. Edge cases include an empty grid (though spec says at least 1x1), a grid with no land (returns 0), a grid where all land is on the border (returns 0), and a grid where a land cell is surrounded by water but not connected to any border land. The algorithm processes each cell at most once during the BFS/DFS, so time complexity is O(rows × columns). Space complexity is O(rows × columns) in the worst case for the queue/recursion stack, though we can modify the input grid in-place to avoid extra visited arrays, but the given snippet modifies the input by setting visited cells to 0.
