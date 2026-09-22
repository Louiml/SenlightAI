// Write a C++ function named `countUncoveredCells` that takes a 2D grid represented as a `std::vector<std::vector<int>>` where each cell contains either `0` (empty) or `1` (obstacle). The function must return the number of cells that are not covered by any obstacle "shadow" cast in four cardinal directions (up, down, left, right) from each obstacle. A cell is covered if there exists at least one obstacle in the same row to its left or right, or in the same column above or below it (i.e., any cell in the same row or column as an obstacle, excluding the obstacle itself, is covered). Empty cells that are not in any row or column containing an obstacle remain uncovered. The function should be `const`-correct and handle edge cases like empty grids, grids with no obstacles, and grids entirely covered.

#include <cassert>
#include <vector>

int main() {
    // Test 1: Empty grid returns 0.
    std::vector<std::vector<int>> emptyGrid;
    assert(countUncoveredCells(emptyGrid) == 0);

    // Test 2: Single empty cell returns 1.
    std::vector<std::vector<int>> singleEmpty = {{0}};
    assert(countUncoveredCells(singleEmpty) == 1);

    // Test 3: Single obstacle covers all cells (itself is excluded, but no other cells).
    // A 1x1 grid with an obstacle: the cell is an obstacle, so there are no empty cells to uncover.
    std::vector<std::vector<int>> singleObstacle = {{1}};
    assert(countUncoveredCells(singleObstacle) == 0);

    // Test 4: 2x2 grid with one obstacle at (0,0) covers row 0 and column 0.
    // Covered cells: (0,1), (1,0). Uncovered: (1,1) only.
    std::vector<std::vector<int>> grid4 = {{1,0}, {0,0}};
    assert(countUncoveredCells(grid4) == 1);

    // Test 5: No obstacles means all cells uncovered.
    std::vector<std::vector<int>> grid5 = {{0,0,0}, {0,0,0}};
    assert(countUncoveredCells(grid5) == 6);

    // Test 6: Obstacles in every row and column → no uncovered cells.
    std::vector<std::vector<int>> grid6 = {{1,0}, {0,1}};
    assert(countUncoveredCells(grid6) == 0);

    // Test 7: 3x3 grid with obstacles only in middle row and middle column.
    // Covered: all cells in row 1 and all cells in column 1. Uncovered: four corners.
    std::vector<std::vector<int>> grid7 = {{0,0,0}, {1,0,1}, {0,0,0}};
    assert(countUncoveredCells(grid7) == 4);

    return 0;
}

#include <vector>

// Count cells not covered by any obstacle in the same row or column.
// Obstacles are marked with 1, empty cells with 0.
int countUncoveredCells(const std::vector<std::vector<int>>& grid) {
    if (grid.empty() || grid[0].empty()) {
        return 0;
    }

    int rows = static_cast<int>(grid.size());
    int cols = static_cast<int>(grid[0].size());

    std::vector<bool> rowHasObstacle(rows, false);
    std::vector<bool> colHasObstacle(cols, false);

    // First pass: identify rows and columns that contain obstacles.
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            if (grid[r][c] == 1) {
                rowHasObstacle[r] = true;
                colHasObstacle[c] = true;
            }
        }
    }

    // Second pass: count cells that have no obstacle in their row or column.
    int uncoveredCount = 0;
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            if (!rowHasObstacle[r] && !colHasObstacle[c]) {
                ++uncoveredCount;
            }
        }
    }

    return uncoveredCount;
}

// The solution requires determining for each cell whether it shares a row or column with at least one obstacle. The main idea is to precompute two boolean arrays: `rowHasObstacle` for each row and `colHasObstacle` for each column. First, iterate over all cells to record which rows and columns contain obstacles. Then, iterate over all cells again; a cell is covered if `rowHasObstacle[row]` is true or `colHasObstacle[col]` is true. Count the number of cells that are NOT covered (i.e., both booleans are false). Edge cases: empty grid (return 0), grid with no obstacles (all rows and columns have false, so all cells are uncovered – count equals total cells), and grid fully covered (all cells have a row or column with an obstacle). The algorithm runs in O(R×C) time, where R is number of rows and C is number of columns, and uses O(R + C) auxiliary space for the boolean arrays. No extra data structures beyond these arrays are needed, and the solution is straightforward with two passes.
