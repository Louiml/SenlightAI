// Write a C++ function `int countComponents(const std::vector<std::string>& grid)` that takes a rectangular grid of characters where `'.'` represents open land and `'#'` represents an obstacle, and returns the number of connected components of open land. Two cells are connected if they are adjacent horizontally or vertically (not diagonally). The grid dimensions may be zero (empty grid) or any positive size. The grid is guaranteed to be rectangular (all strings have the same length). The function must handle grids of any size, including large ones, without recursion depth issues (so an iterative approach or explicit stack should be used). Only the four cardinal directions are considered.
// The problem is a classic connected components count on a 2D grid. The approach is to iterate over every cell in the grid; whenever we encounter an unvisited `'.'`, we increment the component counter and perform a graph traversal (BFS or DFS) from that cell, marking all reachable `'.'` cells as visited. Because the grid can be large (possibly up to 1000x1000 or more), recursion might cause stack overflow, so an iterative BFS using a queue (or an explicit stack) is safer. We maintain a `visited` boolean matrix of the same size. Time complexity is O(rows * cols) because each cell is visited at most once during traversal. Space complexity is O(rows * cols) for the visited matrix and potentially O(rows * cols) for the queue in the worst case (e.g., all cells are `'.'`). Edge cases include: an empty grid (return 0), a grid with no open land (return 0), a grid with only one cell (`"."` → 1, `"#"` → 0), and a grid with obstacles completely separating components.
#include <vector>
#include <queue>
#include <utility>

// Count the number of connected components of '.' cells in a rectangular grid.
// Cells are connected if they share an edge (up, down, left, right).
int countComponents(const std::vector<std::string>& grid) {
    if (grid.empty() || grid[0].empty()) {
        return 0;
    }

    const int rows = static_cast<int>(grid.size());
    const int cols = static_cast<int>(grid[0].size());
    std::vector<std::vector<bool>> visited(rows, std::vector<bool>(cols, false));

    // Direction vectors: up, down, left, right
    const int dr[4] = {-1, 1, 0, 0};
    const int dc[4] = {0, 0, -1, 1};

    int components = 0;

    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            if (grid[r][c] == '.' && !visited[r][c]) {
                ++components;
                // Iterative BFS
                std::queue<std::pair<int,int>> q;
                q.push({r, c});
                visited[r][c] = true;

                while (!q.empty()) {
                    auto [curR, curC] = q.front();
                    q.pop();
                    for (int dir = 0; dir < 4; ++dir) {
                        int nr = curR + dr[dir];
                        int nc = curC + dc[dir];
                        if (nr >= 0 && nr < rows && nc >= 0 && nc < cols &&
                            grid[nr][nc] == '.' && !visited[nr][nc]) {
                            visited[nr][nc] = true;
                            q.push({nr, nc});
                        }
                    }
                }
            }
        }
    }

    return components;
}
#include <cassert>
#include <vector>
#include <string>

// Assuming the solution function is defined above in the same translation unit.

int main() {
    // Test 1: Empty grid
    std::vector<std::string> grid1;
    assert(countComponents(grid1) == 0);

    // Test 2: Single '.' cell
    std::vector<std::string> grid2 = {"."};
    assert(countComponents(grid2) == 1);

    // Test 3: Single '#' cell
    std::vector<std::string> grid3 = {"#"};
    assert(countComponents(grid3) == 0);

    // Test 4: All open cells
    std::vector<std::string> grid4 = {"...", "...", "..."};
    assert(countComponents(grid4) == 1);

    // Test 5: Obstacles separate components
    std::vector<std::string> grid5 = {".#.", ".#.", ".#."};
    assert(countComponents(grid5) == 3);

    // Test 6: Diagonal adjacency is not considered
    std::vector<std::string> grid6 = {".#", "#."};
    assert(countComponents(grid6) == 2);

    // Test 7: Larger grid with multiple components
    std::vector<std::string> grid7 = {
        "...#.",
        "#..#.",
        ".#.#.",
        "..##.",
        "....."
    };
    assert(countComponents(grid7) == 4);

    // Test 8: Single row with mixed cells
    std::vector<std::string> grid8 = {"..#.."};
    assert(countComponents(grid8) == 2);

    // Test 9: Single column with mixed cells
    std::vector<std::string> grid9 = {".", "#", ".", ".", "#"};
    assert(countComponents(grid9) == 2);

    // Test 10: All obstacles
    std::vector<std::string> grid10 = {"###", "###", "###"};
    assert(countComponents(grid10) == 0);

    return 0;
}
