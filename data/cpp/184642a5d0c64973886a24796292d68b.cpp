// Write a C++ function `int countConnectedIslands(const std::vector<std::vector<char>>& grid)` that takes a rectangular grid of characters where `'1'` represents land and `'0'` represents water, and returns the number of distinct islands. An island is defined as a group of adjacent land cells connected horizontally or vertically (not diagonally). The grid is guaranteed to be non-empty (at least 1 row and 1 column). The function must not modify the input grid. Handle edge cases such as a single-cell grid, an all-water grid, an all-land grid, and grids with multiple separate islands.
// The solution uses depth-first search (DFS) to traverse and mark every land cell belonging to an island. The main algorithm iterates through every cell of the grid. When an unvisited land cell (`'1'`) is found, it increments the island counter and starts a DFS from that cell. The DFS recursively explores all four orthogonal neighbors (up, down, left, right). To avoid revisiting cells and to prevent counting the same island multiple times, a separate `visited` boolean matrix of the same dimensions is used. The DFS marks visited cells as `true` as soon as it enters them. Since the input grid is `const`, we cannot alter it to mark visited cells, so the separate `visited` matrix is necessary. Edge cases: (1) If the grid is all `'0'`, the loop never finds a `'1'`, and the count remains 0. (2) A single-cell grid with `'1'` returns 1; with `'0'` returns 0. (3) Large grids with many islands still work because DFS is bounded by the grid size. Time complexity: O(rows * columns) because every cell is visited at most once (each cell is either checked in the main loop or visited in DFS, but not repeatedly). Space complexity: O(rows * columns) for the `visited` matrix plus O(rows * columns) in the worst case for the recursion stack (if the entire grid is one island, the DFS recursion depth can be as large as the number of cells). For a very large grid, this recursion depth could cause stack overflow, but for typical competitive programming constraints (e.g., up to 300x300) it is acceptable. To be safe, one could use an iterative stack, but the problem specification does not forbid recursion.
#include <vector>

// Count the number of connected components of '1's in a grid.
// DFS traversal marks all visited land cells to avoid recounting.
int countConnectedIslands(const std::vector<std::vector<char>>& grid) {
    if (grid.empty() || grid[0].empty()) {
        return 0;
    }
    const int rows = static_cast<int>(grid.size());
    const int cols = static_cast<int>(grid[0].size());

    // Visited matrix to track already processed cells.
    std::vector<std::vector<bool>> visited(rows, std::vector<bool>(cols, false));

    // Recursive DFS helper (lambda for encapsulation).
    // Captures grid, visited, rows, cols by reference.
    std::function<void(int, int)> dfs = [&](int r, int c) {
        // Check bounds and if water or already visited.
        if (r < 0 || r >= rows || c < 0 || c >= cols) return;
        if (grid[r][c] == '0' || visited[r][c]) return;
        visited[r][c] = true;
        dfs(r + 1, c);
        dfs(r - 1, c);
        dfs(r, c + 1);
        dfs(r, c - 1);
    };

    int islandCount = 0;
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            if (!visited[r][c] && grid[r][c] == '1') {
                ++islandCount;
                dfs(r, c);
            }
        }
    }
    return islandCount;
}
#include <cassert>
#include <vector>

// Provided solution function is assumed to be available above.

int main() {
    // Single-cell water.
    std::vector<std::vector<char>> grid1 = {{'0'}};
    assert(countConnectedIslands(grid1) == 0);

    // Single-cell land.
    std::vector<std::vector<char>> grid2 = {{'1'}};
    assert(countConnectedIslands(grid2) == 1);

    // All water 2x2.
    std::vector<std::vector<char>> grid3 = {{'0','0'},{'0','0'}};
    assert(countConnectedIslands(grid3) == 0);

    // All land 2x2 (one island).
    std::vector<std::vector<char>> grid4 = {{'1','1'},{'1','1'}};
    assert(countConnectedIslands(grid4) == 1);

    // Two separate islands.
    std::vector<std::vector<char>> grid5 = {
        {'1','0','1'},
        {'0','0','0'},
        {'1','0','1'}
    };
    assert(countConnectedIslands(grid5) == 4);

    // L-shaped island (one island).
    std::vector<std::vector<char>> grid6 = {
        {'1','1','0'},
        {'0','1','0'},
        {'0','0','0'}
    };
    assert(countConnectedIslands(grid6) == 1);

    // Classic example from problem statement style.
    std::vector<std::vector<char>> grid7 = {
        {'1','1','0','0','0'},
        {'1','1','0','0','0'},
        {'0','0','1','0','0'},
        {'0','0','0','1','1'}
    };
    assert(countConnectedIslands(grid7) == 3);

    // Empty grid (edge case) – but problem says non-empty, so skip.

    // Grid with one large island that touches all borders.
    std::vector<std::vector<char>> grid8 = {
        {'1','1','1'},
        {'1','0','1'},
        {'1','1','1'}
    };
    assert(countConnectedIslands(grid8) == 1);

    // Diagonal cells are not connected.
    std::vector<std::vector<char>> grid9 = {
        {'1','0'},
        {'0','1'}
    };
    assert(countConnectedIslands(grid9) == 2);

    // Row of alternating land and water.
    std::vector<std::vector<char>> grid10 = {{'1','0','1','0','1'}};
    assert(countConnectedIslands(grid10) == 3);
}
