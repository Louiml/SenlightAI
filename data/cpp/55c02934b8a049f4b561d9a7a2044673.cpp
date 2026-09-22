Write a C++ function `countCoastCells(int rows, int cols, const std::vector<std::string>& grid)` that takes a rectangular grid of characters where `'#'` represents land and `'.'` represents water, and returns the number of land cells that are adjacent (up, down, left, or right) to at least one water cell **or** to the outside of the grid boundary (i.e., a land cell on the edge of the grid is always considered a coast cell). You may assume the grid is non-empty, but the number of rows and columns can be up to 1000. The function should not modify the input grid.
We need to count land cells (`'#'`) that touch either water (`'.'`) or the grid border. The simplest approach is to iterate over all cells. For each cell that contains land, check its four orthogonal neighbors. If any neighbor is out of bounds, that land cell is on the border and thus counts as coast. If a neighbor is inside the grid and is water, the land cell also counts. To avoid double‑counting, we can count each qualifying land cell once. The algorithm runs in O(rows × cols) time because we examine each cell and its four neighbors, which is a constant factor. Space complexity is O(1) beyond the input storage, as we only use a few loop variables and a constant‑sized move offset array. Edge cases: a grid with only land (all `'#'`) – only the border cells count; a grid with only water – answer is 0; a 1×1 grid with `'#'` counts as 1 because it touches the outside on all sides; a 1×1 grid with `'.'` counts as 0.
#include <string>
#include <vector>

// Count land cells that are adjacent to water or outside the grid.
int countCoastCells(int rows, int cols, const std::vector<std::string>& grid) {
    const int dr[4] = {-1, 1, 0, 0};
    const int dc[4] = {0, 0, -1, 1};
    int coastCount = 0;

    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            if (grid[r][c] != '#') continue; // only land cells

            bool isCoast = false;
            for (int d = 0; d < 4; ++d) {
                int nr = r + dr[d];
                int nc = c + dc[d];
                // Out of bounds means border → coast
                if (nr < 0 || nr >= rows || nc < 0 || nc >= cols) {
                    isCoast = true;
                    break;
                }
                // Adjacent to water → coast
                if (grid[nr][nc] == '.') {
                    isCoast = true;
                    break;
                }
            }
            if (isCoast) {
                ++coastCount;
            }
        }
    }
    return coastCount;
}
#include <cassert>
#include <string>
#include <vector>

// Include the solution function here (or copy above).

int main() {
    // Test 1: Mixed grid, corners and edges count.
    std::vector<std::string> grid1 = {
        "###",
        "#.#",
        "###"
    };
    assert(countCoastCells(3, 3, grid1) == 8); // all except center land

    // Test 2: All land, 3x3 → border cells (8) count.
    std::vector<std::string> grid2 = {
        "###",
        "###",
        "###"
    };
    assert(countCoastCells(3, 3, grid2) == 8);

    // Test 3: All water → 0.
    std::vector<std::string> grid3 = {
        "...",
        "...",
        "..."
    };
    assert(countCoastCells(3, 3, grid3) == 0);

    // Test 4: Single land cell 1x1 → counts because out of bounds on all sides.
    std::vector<std::string> grid4 = {"#"};
    assert(countCoastCells(1, 1, grid4) == 1);

    // Test 5: Single water cell 1x1 → 0.
    std::vector<std::string> grid5 = {"."};
    assert(countCoastCells(1, 1, grid5) == 0);

    // Test 6: Land row in middle of water, e.g., 1x5 "#.#.#" → each land touches water.
    std::vector<std::string> grid6 = {"#.#.#"};
    assert(countCoastCells(1, 5, grid6) == 3);

    // Test 7: No water but 1×n all land → both ends count, interior don't.
    std::vector<std::string> grid7 = {"#####"};
    assert(countCoastCells(1, 5, grid7) == 2); // first and last

    // Test 8: Large grid (1000x1000) all border land, interior water.
    int N = 1000;
    std::vector<std::string> grid8(N, std::string(N, '.'));
    for (int i = 0; i < N; ++i) {
        grid8[i][0] = '#';
        grid8[i][N-1] = '#';
    }
    for (int j = 0; j < N; ++j) {
        grid8[0][j] = '#';
        grid8[N-1][j] = '#';
    }
    // Border cells: 4*N - 4 (corners counted once)
    int expected = 4*N - 4;
    assert(countCoastCells(N, N, grid8) == expected);

    return 0;
}
