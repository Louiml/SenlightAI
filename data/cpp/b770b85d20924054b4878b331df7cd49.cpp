Write a C++ function named `next_generation` that takes a square 2D vector of booleans representing a cell grid (true = alive, false = dead) and returns a new grid after applying one step of Conway's Game of Life. The rules are: a live cell survives if it has 2 or 3 live neighbors; a dead cell becomes alive if it has exactly 3 live neighbors; all other cells die or stay dead. Neighbors are the 8 cells surrounding a cell (horizontally, vertically, diagonally), but cells outside the grid are always dead (no wrapping). The input grid is guaranteed to be non-empty and square (all rows have the same length equal to the number of rows). The function must not modify the input; return a new grid.

The solution involves iterating over each cell and counting its live neighbors. Since the grid is square and non-empty, we can determine the size `n` from `grid.size()`. For each cell at row `i` and column `j`, we examine all neighbor offsets `(dr, dc)` where `dr` and `dc` are in `{-1, 0, 1}`, excluding the case where both are zero. For each offset, we compute the neighbor coordinates `ni = i + dr` and `nj = j + dc`, and if they are within bounds (`0 <= ni < n` and `0 <= nj < n`), we add 1 to the neighbor count if `grid[ni][nj]` is true. After counting, we apply the rules: if the current cell is alive and the neighbor count is 2 or 3, it survives; if the current cell is dead and the count is exactly 3, it becomes alive; otherwise, the cell is dead. Edge cases include corners and edges where many neighbor positions are out of bounds; the bounds check handles these. The time complexity is O(n^2) for the grid of n×n cells, and the space complexity is O(n^2) for the new grid we build. We also need to be careful with const correctness: the input parameter should be `const std::vector<std::vector<bool>>&` and the function returns by value.

#include <vector>

// Apply one step of Conway's Game of Life to a square boolean grid.
std::vector<std::vector<bool>> next_generation(const std::vector<std::vector<bool>>& grid) {
    int n = grid.size();
    std::vector<std::vector<bool>> result(n, std::vector<bool>(n, false));

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            int live_neighbors = 0;

            // Check all 8 neighbor positions.
            for (int dr = -1; dr <= 1; ++dr) {
                for (int dc = -1; dc <= 1; ++dc) {
                    if (dr == 0 && dc == 0) continue; // Skip the cell itself.

                    int ni = i + dr;
                    int nj = j + dc;

                    // Only count if the neighbor is inside the grid.
                    if (ni >= 0 && ni < n && nj >= 0 && nj < n) {
                        if (grid[ni][nj]) {
                            ++live_neighbors;
                        }
                    }
                }
            }

            // Apply the rules.
            if (grid[i][j]) {
                // Live cell survives with 2 or 3 neighbors.
                result[i][j] = (live_neighbors == 2 || live_neighbors == 3);
            } else {
                // Dead cell becomes alive with exactly 3 neighbors.
                result[i][j] = (live_neighbors == 3);
            }
        }
    }

    return result;
}

#include <cassert>
#include <vector>

int main() {
    // Test 1: Single dead cell stays dead.
    std::vector<std::vector<bool>> grid1 = {{false}};
    assert(next_generation(grid1) == std::vector<std::vector<bool>>{{false}});

    // Test 2: Single live cell dies (no neighbors).
    std::vector<std::vector<bool>> grid2 = {{true}};
    assert(next_generation(grid2) == std::vector<std::vector<bool>>{{false}});

    // Test 3: Block (2x2) is stable.
    std::vector<std::vector<bool>> grid3 = {{true, true}, {true, true}};
    assert(next_generation(grid3) == grid3);

    // Test 4: Blinker (horizontal line) becomes vertical.
    std::vector<std::vector<bool>> grid4 = {{false, true, false}, {false, true, false}, {false, true, false}};
    std::vector<std::vector<bool>> expected4 = {{false, false, false}, {true, true, true}, {false, false, false}};
    assert(next_generation(grid4) == expected4);

    // Test 5: Empty grid stays empty (2x2).
    std::vector<std::vector<bool>> grid5 = {{false, false}, {false, false}};
    assert(next_generation(grid5) == grid5);

    // Test 6: Edge cell with two live neighbors (one out of bounds) dies.
    std::vector<std::vector<bool>> grid6 = {{true, true, false}, {false, false, false}, {false, false, false}};
    // Cell (0,0) has neighbors: (0,1)=true, (1,0)=false, (1,1)=false → 1 neighbor → dies.
    // Cell (0,1) has neighbors: (0,0)=true, (1,0)=false, (1,1)=false, (1,2)=false → 1 neighbor → dies.
    // All others have 0 or 1 neighbors → stay dead.
    std::vector<std::vector<bool>> expected6(3, std::vector<bool>(3, false));
    assert(next_generation(grid6) == expected6);

    // Test 7: Input is not modified (original grid unchanged).
    std::vector<std::vector<bool>> grid7 = {{true, false}, {false, true}};
    auto original7 = grid7;
    next_generation(grid7);
    assert(grid7 == original7);
}
