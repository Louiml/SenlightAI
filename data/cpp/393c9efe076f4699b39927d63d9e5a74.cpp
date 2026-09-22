// Write a C++ function `int maxTetrominoSum(const std::vector<std::vector<int>>& grid)` that takes a rectangular grid of non-negative integers (dimensions at least 1×1 and at most 500×500) and returns the maximum possible sum of four adjacent cells that form any of the 19 distinct tetromino shapes (the seven free tetrominoes: I, O, T, L, J, S, Z, including all their rotations and reflections). Each tetromino must fit entirely within the grid. The function should consider all possible placements of every shape. The grid is indexed as `grid[row][col]` with row 0 at the top, and the function must handle grids as small as 1×1 or 2×2 where most placements are invalid (returning the sum of all cells if they fit, or 0 if no tetromino fits—though with dimensions at least 1×1, at least the O-tetromino may not fit, so handle that case by returning 0 if no shape can be placed). You may assume all values are non-negative, so the maximum is well-defined even with zeros.

The solution enumerates all possible placements of the 19 tetromino orientations. For each orientation, we define a list of four coordinate offsets (relative to a reference cell). Then for every cell in the grid that can serve as an anchor (top-left-most cell of the shape), we check whether all four offsets stay within grid bounds; if so, we sum the values at those positions and update the global maximum. Because the grid dimensions are at most 500×500, brute-force checking all 19 shapes at every cell is efficient: the total number of cells is up to 250,000, and for each cell we test up to 19 shapes, each requiring 4 coordinate checks/sums, giving about 19 million operations—well within time limits. The time complexity is O(R*C*19*4) = O(R*C), and the space complexity is O(1) beyond the input grid (since we only store the shapes' offsets as static data). An important edge case is when the grid is very small (e.g., 1×1 or 2×2), where most shapes cannot be placed; the algorithm naturally handles that because the bounds checks fail. For a 1×1 grid, no tetromino of 4 cells fits, so the function should return 0—this is handled by initializing the result to 0 and only updating if a valid placement is found. Also note that all values are non-negative, so we do not need to worry about negative contributions.

#include <vector>
#include <array>
#include <algorithm>

// Return the maximum sum of four cells forming any tetromino shape in the grid.
// The grid is indexed as grid[row][col], with row 0 at the top.
int maxTetrominoSum(const std::vector<std::vector<int>>& grid) {
    if (grid.empty() || grid[0].empty()) return 0;
    const int rows = static_cast<int>(grid.size());
    const int cols = static_cast<int>(grid[0].size());

    // All 19 tetromino orientations as coordinate offsets from a reference cell.
    // Each offset is (row_delta, col_delta).
    const std::array<std::array<std::pair<int,int>,4>,19> shapes = {{
        // I (horizontal, vertical)
        {{{0,0},{0,1},{0,2},{0,3}}},
        {{{0,0},{1,0},{2,0},{3,0}}},
        // O
        {{{0,0},{0,1},{1,0},{1,1}}},
        // L (4 rotations) and J (4 rotations) combined as 8 orientations
        {{{0,0},{1,0},{2,0},{2,1}}},
        {{{0,0},{0,1},{0,2},{1,0}}},
        {{{0,0},{0,1},{1,1},{2,1}}},
        {{{0,0},{1,0},{1,1},{1,2}}},
        {{{0,0},{1,0},{2,0},{2,-1}}},
        {{{0,0},{0,-1},{0,-2},{1,0}}},
        {{{0,0},{0,-1},{1,-1},{2,-1}}},
        {{{0,0},{1,0},{1,-1},{1,-2}}},
        // S and Z orientations
        {{{0,0},{0,1},{1,1},{1,2}}},
        {{{0,0},{1,0},{1,1},{2,1}}},
        {{{0,0},{0,1},{1,0},{1,-1}}},
        {{{0,0},{1,0},{1,-1},{2,-1}}},
        // T orientations (4)
        {{{0,0},{0,1},{0,2},{1,1}}},
        {{{0,0},{1,0},{2,0},{1,1}}},
        {{{0,0},{0,1},{0,2},{-1,1}}},
        {{{0,0},{1,0},{2,0},{1,-1}}}
    }};

    int best = 0;
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            for (const auto& shape : shapes) {
                int sum = 0;
                bool valid = true;
                for (int k = 0; k < 4; ++k) {
                    int nr = r + shape[k].first;
                    int nc = c + shape[k].second;
                    if (nr < 0 || nr >= rows || nc < 0 || nc >= cols) {
                        valid = false;
                        break;
                    }
                    sum += grid[nr][nc];
                }
                if (valid) {
                    best = std::max(best, sum);
                }
            }
        }
    }
    return best;
}

#include <cassert>
#include <vector>
#include "solution.h" // assume the above function is in this header or included directly

int main() {
    // Basic cases
    std::vector<std::vector<int>> grid1 = {
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 10, 11, 12},
        {13, 14, 15, 16}
    };
    // The maximum tetromino sum is 15+16+11+12 = 54 (bottom-right 2x2, or others)
    assert(maxTetrominoSum(grid1) == 54);

    // All ones: any placement gives 4
    std::vector<std::vector<int>> grid2 = {
        {1, 1, 1},
        {1, 1, 1},
        {1, 1, 1}
    };
    assert(maxTetrominoSum(grid2) == 4);

    // 1x1 grid: no tetromino fits, return 0
    std::vector<std::vector<int>> grid3 = {{5}};
    assert(maxTetrominoSum(grid3) == 0);

    // 2x2 grid: only the O tetromino fits
    std::vector<std::vector<int>> grid4 = {
        {10, 20},
        {30, 40}
    };
    assert(maxTetrominoSum(grid4) == 100);

    // A shape where only an I-tetromino fits (3 rows, 1 column) but cannot place 4 cells horizontally/vertically -> 0
    std::vector<std::vector<int>> grid5 = {
        {1},
        {2},
        {3}
    };
    assert(maxTetrominoSum(grid5) == 0);

    // A 3x3 grid with a specific high-value cluster
    std::vector<std::vector<int>> grid6 = {
        {0, 0, 0},
        {0, 9, 9},
        {0, 9, 9}
    };
    // O tetromino gives 36, but other shapes might not fit, so max is 36
    assert(maxTetrominoSum(grid6) == 36);

    // Edge case: a row of 4 high values, horizontal I-tetromino
    std::vector<std::vector<int>> grid7 = {
        {1, 1, 1, 1},
        {2, 2, 2, 2},
        {3, 3, 3, 3}
    };
    // The best is 3+3+3+3 = 12 from the bottom row, or any row of 3s
    assert(maxTetrominoSum(grid7) == 12);

    // A grid with a single row of length 4: only horizontal I works
    std::vector<std::vector<int>> grid8 = {{4, 4, 4, 4}};
    assert(maxTetrominoSum(grid8) == 16);

    // A grid with a single column of length 4: only vertical I works
    std::vector<std::vector<int>> grid9 = {{4}, {4}, {4}, {4}};
    assert(maxTetrominoSum(grid9) == 16);

    // Mixed values with negative not allowed, but zeros are fine
    std::vector<std::vector<int>> grid10 = {
        {0, 5, 0},
        {5, 0, 5},
        {0, 5, 0}
    };
    // Best is a T-tetromino? Actually any shape yielding 4 cells, max sum is 20 (e.g., the O at center? but center is 0, so maybe 15 from cross shape? Let's compute: any 4-cell shape: top row [0,5,0] + center row [5,0,5] gives at most 5+5+5+0=15 if we pick a T with top-center and left/right, so 15
    assert(maxTetrominoSum(grid10) == 15);

    return 0;
}
