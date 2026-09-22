// Write a C++ function `bool canReachBottomOrRight(const std::vector<std::string>& grid)` that takes an `n x n` grid of characters `'0'` and `'1'`, where `'1'` represents a black cell and `'0'` represents a white cell. The grid is valid if and only if every black cell can reach either the bottom row or the rightmost column by moving only down or right through black cells (staying within the grid). Return `true` if the grid is valid, and `false` otherwise. The function should handle grids of any size from 1×1 up to 50×50, and it should not modify the input grid.

#include <cassert>
#include <vector>
#include <string>

// The solution function (declared here for the test)
bool canReachBottomOrRight(const std::vector<std::string>& grid);

int main() {
    // Valid: all ones are on the boundary
    assert(canReachBottomOrRight({"1"}) == true);
    assert(canReachBottomOrRight({"0"}) == true);
    
    // Valid: 2x2 with bottom row and right column all ones
    assert(canReachBottomOrRight({"11", "11"}) == true);
    assert(canReachBottomOrRight({"10", "11"}) == true);
    assert(canReachBottomOrRight({"01", "11"}) == true);
    
    // Invalid: a 1 at top-left cannot reach boundary
    assert(canReachBottomOrRight({"10", "00"}) == false);
    assert(canReachBottomOrRight({"01", "00"}) == false);
    
    // Valid: a 1 at top-left can reach via right or down
    assert(canReachBottomOrRight({"11", "01"}) == true);
    assert(canReachBottomOrRight({"10", "11"}) == true);
    
    // Valid: larger grid with a connected path
    std::vector<std::string> grid3 = {
        "100",
        "110",
        "111"
    };
    assert(canReachBottomOrRight(grid3) == true);
    
    // Invalid: isolated black cell not connected
    std::vector<std::string> grid4 = {
        "100",
        "000",
        "001"
    };
    assert(canReachBottomOrRight(grid4) == false);
    
    // Valid: all zeros trivially valid
    std::vector<std::string> grid5 = {
        "000",
        "000",
        "000"
    };
    assert(canReachBottomOrRight(grid5) == true);
    
    // Invalid: a black cell in the middle that cannot go anywhere
    std::vector<std::string> grid6 = {
        "010",
        "010",
        "000"
    };
    assert(canReachBottomOrRight(grid6) == false);
    
    return 0;
}

#include <vector>
#include <string>

// Returns true if every '1' cell can reach the bottom row or rightmost column
// by moving only down or right through '1' cells.
bool canReachBottomOrRight(const std::vector<std::string>& grid) {
    int n = static_cast<int>(grid.size());
    if (n == 0) return true; // empty grid, trivially valid

    // used[i][j] = true if cell (i,j) is a '1' that can reach the boundary
    std::vector<std::vector<bool>> used(n, std::vector<bool>(n, false));

    // Process from bottom-right to top-left
    for (int i = n - 1; i >= 0; --i) {
        for (int j = n - 1; j >= 0; --j) {
            if (grid[i][j] != '1') continue; // white cell: not relevant

            // If in the last row or last column, it can reach the boundary directly
            if (i == n - 1 || j == n - 1) {
                used[i][j] = true;
            } else {
                // Otherwise, it can reach if either the cell below or to the right can
                used[i][j] = used[i + 1][j] || used[i][j + 1];
            }
        }
    }

    // Now check that every '1' cell is marked as usable
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (grid[i][j] == '1' && !used[i][j]) {
                return false;
            }
        }
    }
    return true;
}

// The solution processes the grid from the bottom-right corner upward and leftward (i.e., from the last row/column back to the first). For each cell, we define a boolean `used[i][j]` indicating whether a black cell at position (i,j) can reach the boundary (bottom row or rightmost column) using only down/right moves through black cells. Base cases: if a cell is in the last row or last column and is black, it is immediately valid (`used = true`). For any other black cell, it is valid if and only if either the cell below it or the cell to its right is also valid (i.e., `used[i+1][j]` or `used[i][j+1]` is true). White cells are always considered irrelevant (they don't need to reach anywhere). After computing `used` for all cells, we check if every black cell has `used = true`. If any black cell has `used = false`, the grid is invalid. This greedy/dynamic programming approach works because moving only right or down means all possible paths from a cell go through its immediate right or lower neighbor. The order of processing from bottom-right to top-left ensures that when evaluating a cell, its right and lower neighbors have already been evaluated. Edge cases include a 1×1 grid (either `'0'` → valid, or `'1'` → valid because it's in the bottom row and rightmost column), and grids where a black cell is completely isolated from both the bottom and right boundaries. The time complexity is O(n²) to scan the grid twice, and space complexity is O(n²) for the `used` array.
