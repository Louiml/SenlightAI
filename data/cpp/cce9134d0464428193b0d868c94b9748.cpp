Write a C++ function `countXPatterns` that takes a square grid of characters (size `n x n`) stored as a vector of strings, where each cell contains either `'X'` or another character, and returns the number of times the pattern of five `'X'` cells forming an "X" shape appears. Specifically, an occurrence is counted for every interior cell `(i, j)` (where `1 <= i <= n-2` and `1 <= j <= n-2`) such that the cell itself and its four diagonal neighbors (top-left, top-right, bottom-left, bottom-right) are all equal to `'X'`. The grid is guaranteed to be non-empty and square (`n >= 3`). The function should be efficient for large grids and must not modify the input.

// The solution iterates over all interior cells of the grid, i.e., rows from index 1 to `n-2` and columns from index 1 to `n-2`. For each interior cell, check if the cell and its four diagonal neighbors are all `'X'`. If so, increment a counter. The key edge case is that the grid must have at least size 3x3; if `n < 3`, no interior cells exist and the answer is 0. The algorithm runs in `O(n^2)` time because we visit each interior cell exactly once and perform constant-time checks for each. Space complexity is `O(1)` additional memory since we only use a counter and loop variables. No sorting or other pre-processing is required, and the input grid is read-only.

#include <vector>
#include <string>

// Count occurrences of the X pattern in a square grid of characters.
// The pattern consists of a center 'X' and its four diagonal neighbors all being 'X'.
long long countXPatterns(const std::vector<std::string>& grid) {
    long long count = 0;
    int n = static_cast<int>(grid.size());
    if (n < 3) {
        return 0; // No interior cells possible
    }

    for (int i = 1; i < n - 1; ++i) {
        for (int j = 1; j < n - 1; ++j) {
            if (grid[i][j] == 'X' &&
                grid[i-1][j-1] == 'X' &&
                grid[i-1][j+1] == 'X' &&
                grid[i+1][j-1] == 'X' &&
                grid[i+1][j+1] == 'X') {
                ++count;
            }
        }
    }
    return count;
}

#include <cassert>
int main() {
    // Test case 1: Single X pattern in 3x3 grid
    std::vector<std::string> grid1 = {
        "XXX",
        "XXX",
        "XXX"
    };
    assert(countXPatterns(grid1) == 1); // Center (1,1) qualifies

    // Test case 2: No pattern because center is not X
    std::vector<std::string> grid2 = {
        "XXX",
        "XOX",
        "XXX"
    };
    assert(countXPatterns(grid2) == 0);

    // Test case 3: Multiple patterns in 4x4 grid
    std::vector<std::string> grid3 = {
        "XXXX",
        "XXXX",
        "XXXX",
        "XXXX"
    };
    // All interior cells (1,1),(1,2),(2,1),(2,2) qualify = 4
    assert(countXPatterns(grid3) == 4);

    // Test case 4: Pattern with non-X diagonal breaks
    std::vector<std::string> grid4 = {
        "XXXX",
        "XXOX",
        "XXXX",
        "XXXX"
    };
    // Center (1,1): diag top-left=(0,0)X, top-right=(0,2)X, bottom-left=(2,0)X, bottom-right=(2,2)X -> all X, but center (1,1)='X'? yes, so counts.
    // Center (1,2): top-left=(0,1)X, top-right=(0,3)X, bottom-left=(2,1)X, bottom-right=(2,3)X -> all X, center='X'? yes, counts.
    // Center (2,1): top-left=(1,0)X, top-right=(1,2)O -> fails, no count.
    // Center (2,2): top-left=(1,1)X, top-right=(1,3)X, bottom-left=(3,1)X, bottom-right=(3,3)X -> all X, center='X'? yes, counts.
    // Total = 3
    assert(countXPatterns(grid4) == 3);

    // Test case 5: Small grid (n=2) should return 0
    std::vector<std::string> grid5 = {"XX", "XX"};
    assert(countXPatterns(grid5) == 0);

    // Test case 6: Empty grid (n=0) returns 0
    std::vector<std::string> grid6;
    assert(countXPatterns(grid6) == 0);

    // Test case 7: Diagonal pattern only on border cells not counted
    std::vector<std::string> grid7 = {
        "XXXX",
        "XXXX",
        "XXXX",
        "XXXX"
    };
    // Already tested, but ensure same result
    assert(countXPatterns(grid7) == 4);

    // Test case 8: Only one pattern with mixed non-X
    std::vector<std::string> grid8 = {
        "XXX",
        "XXX",
        "XXX"
    };
    // Exactly 1 pattern
    assert(countXPatterns(grid8) == 1);
}
