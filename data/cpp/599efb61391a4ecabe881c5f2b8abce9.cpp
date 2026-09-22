/*
Write a C++ function `colorBorder` that takes a 2D vector of integers `grid`, a starting row `r0`, a starting column `c0`, and a target `color`. The function must return a new 2D vector equal to the original grid except that the connected component of cells having the same value as `grid[r0][c0]` (using 4-directional adjacency) should have its "border" cells recolored to `color`. A border cell is defined as a cell in that component that lies on the outer boundary of the grid (first/last row or first/last column) or is adjacent (up/down/left/right) to a cell that is not in the component. Cells inside the component that are not border cells remain unchanged. Your function must not modify the input grid (i.e., it should work on a copy) and should be robust to grids of size at least 1x1. The main algorithm must use depth-first search (DFS) to identify the component and then apply the recoloring rule.
*/

#include <vector>

// Recolor the border of the connected component containing (r0, c0) in grid.
// Returns a new grid; the input grid is not modified.
std::vector<std::vector<int>> colorBorder(
    const std::vector<std::vector<int>>& grid,
    int r0, int c0, int color
) {
    int rows = static_cast<int>(grid.size());
    int cols = static_cast<int>(grid[0].size());
    int original_color = grid[r0][c0];

    // Work on a copy so the input remains unchanged.
    std::vector<std::vector<int>> record = grid;

    // Sentinel to mark visited cells of the component.
    const int VISITED = 1001;

    // Recursive DFS to mark the whole connected component.
    auto dfs = [&](auto&& self, int r, int c) -> void {
        if (r < 0 || r >= rows || c < 0 || c >= cols) return;
        if (record[r][c] != original_color) return;
        record[r][c] = VISITED;
        self(self, r, c - 1); // left
        self(self, r - 1, c); // up
        self(self, r, c + 1); // right
        self(self, r + 1, c); // down
    };
    dfs(dfs, r0, c0);

    // Recolor border cells of the component.
    std::vector<std::vector<int>> result = grid;
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            if (record[r][c] != VISITED) continue;

            bool is_border = false;
            // Check grid boundary.
            if (r == 0 || r == rows - 1 || c == 0 || c == cols - 1) {
                is_border = true;
            }
            // Check adjacency to cells outside the component.
            if (!is_border) {
                if (record[r][c - 1] != VISITED ||
                    record[r - 1][c] != VISITED ||
                    record[r][c + 1] != VISITED ||
                    record[r + 1][c] != VISITED) {
                    is_border = true;
                }
            }
            if (is_border) {
                result[r][c] = color;
            }
        }
    }
    return result;
}

#include <cassert>
#include <vector>

// The solution function is provided above; include it here.

int main() {
    // Case 1: Simple 3x3, component in center, only center is border (all neighbors outside).
    {
        std::vector<std::vector<int>> grid = {{1,1,1},{1,2,1},{1,1,1}};
        auto res = colorBorder(grid, 1, 1, 3);
        std::vector<std::vector<int>> expected = {{1,1,1},{1,3,1},{1,1,1}};
        assert(res == expected);
    }

    // Case 2: Component touches grid boundary.
    {
        std::vector<std::vector<int>> grid = {{1,1,1},{1,2,1},{1,1,1}};
        auto res = colorBorder(grid, 0, 0, 5);
        std::vector<std::vector<int>> expected = {{5,5,5},{5,2,5},{5,5,5}};
        assert(res == expected);
    }

    // Case 3: Single cell component.
    {
        std::vector<std::vector<int>> grid = {{42}};
        auto res = colorBorder(grid, 0, 0, 7);
        std::vector<std::vector<int>> expected = {{7}};
        assert(res == expected);
    }

    // Case 4: Large component with interior cells.
    {
        std::vector<std::vector<int>> grid = {{1,1,1,1,1},{1,1,1,1,1},{1,1,2,1,1},{1,1,1,1,1},{1,1,1,1,1}};
        auto res = colorBorder(grid, 0, 0, 9);
        std::vector<std::vector<int>> expected = {
            {9,9,9,9,9},
            {9,9,1,9,9},
            {9,9,2,9,9},
            {9,9,1,9,9},
            {9,9,9,9,9}
        };
        assert(res == expected);
    }

    // Case 5: Original color equals target color (no visible change).
    {
        std::vector<std::vector<int>> grid = {{2,2,2},{2,2,2},{2,2,2}};
        auto res = colorBorder(grid, 1, 1, 2);
        assert(res == grid);
    }

    // Case 6: Non‑square grid.
    {
        std::vector<std::vector<int>> grid = {{1,2,2},{1,2,2},{1,1,2}};
        auto res = colorBorder(grid, 0, 1, 8);
        std::vector<std::vector<int>> expected = {{1,8,8},{1,8,8},{1,1,8}};
        assert(res == expected);
    }

    // Case 7: Input not modified.
    {
        std::vector<std::vector<int>> grid = {{1,1},{1,1}};
        std::vector<std::vector<int>> original = grid;
        colorBorder(grid, 0, 0, 3);
        assert(grid == original);
    }

    return 0;
}

// The solution uses a two‑phase approach. First, perform a DFS starting from `(r0, c0)` to mark every cell in the connected component that shares the initial value. During DFS, mark visited cells by changing their value in a copy of the grid to a sentinel (e.g., `1001`) to avoid re‑visiting. Second, iterate over every cell in the grid: if the cell’s recorded value (in the copy) is the sentinel, then it belongs to the component. Determine if it is on the grid boundary (row 0, row `rows‑1`, col 0, col `cols‑1`) or if any of its four orthogonal neighbors is *not* a sentinel (i.e., outside the component). If either condition holds, set the corresponding cell in the original grid copy to the target `color`; otherwise leave it unchanged. The important edge cases include a component that is a single cell (always a border cell because it is either on the boundary or all neighbors are outside), a component that touches the boundary but has interior cells (only boundary cells recolored), and a target color equal to the original component color (no visible change but still correct). Time complexity is O(rows × cols) for both the DFS and the final scan, and space complexity is O(rows × cols) for the copy and the recursion stack in the worst case (when the entire grid is one component).
