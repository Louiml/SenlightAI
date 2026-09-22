/*
Write a C++ function that, given a 2D grid of integers representing a terrain map, returns a `std::pair<int,int>` containing the 1-based row and column indices of the "special" cell, where a special cell is defined as one whose value equals 42 and all eight surrounding neighboring cells (up, down, left, right, and four diagonals) have a value exactly equal to 7. The function takes as parameters a `const std::vector<std::vector<int>>&` (the terrain) and returns `{0,0}` if no such cell exists. The grid is guaranteed to have at least 3 rows and 3 columns, but it may contain multiple special cells; in that case, return the first one found when scanning from top-left to bottom-right (row-major order). The function must avoid accessing out-of-bounds elements and must work for any rectangular grid where all rows have equal length. For example, given the grid:
```
7 7 7
7 42 7
7 7 7
```
the function should return `{2,2}`.
*/

#include <vector>
#include <utility>

// Returns a pair of 1-based indices of the special cell (value 42 with all 8 neighbors equal to 7),
// or {0,0} if no such cell exists. Scans row-major from top-left to bottom-right.
std::pair<int,int> findSpecialCell(const std::vector<std::vector<int>>& terrain) {
    // Validate grid dimensions.
    if (terrain.empty() || terrain[0].empty()) return {0,0};
    const int rows = static_cast<int>(terrain.size());
    const int cols = static_cast<int>(terrain[0].size());
    
    // Need at least 3x3 for a cell with all eight neighbors inside.
    if (rows < 3 || cols < 3) return {0,0};
    
    // Iterate over interior cells only.
    for (int i = 1; i < rows - 1; ++i) {
        for (int j = 1; j < cols - 1; ++j) {
            // Check center value.
            if (terrain[i][j] != 42) continue;
            
            // Check all eight neighbors for the value 7.
            if (terrain[i-1][j-1] == 7 &&
                terrain[i-1][j]   == 7 &&
                terrain[i-1][j+1] == 7 &&
                terrain[i][j-1]   == 7 &&
                terrain[i][j+1]   == 7 &&
                terrain[i+1][j-1] == 7 &&
                terrain[i+1][j]   == 7 &&
                terrain[i+1][j+1] == 7) {
                return {i+1, j+1}; // Return 1-based coordinates.
            }
        }
    }
    return {0,0}; // Not found.
}

#include <cassert>
#include <vector>
#include <utility>

// The solution function is defined elsewhere (above). This main tests it.
int main() {
    // Basic 3x3 perfect match.
    std::vector<std::vector<int>> g1 = {
        {7,7,7},
        {7,42,7},
        {7,7,7}
    };
    assert(findSpecialCell(g1) == std::make_pair(2,2));

    // No center 42.
    std::vector<std::vector<int>> g2 = {
        {7,7,7},
        {7,7,7},
        {7,7,7}
    };
    assert(findSpecialCell(g2) == std::make_pair(0,0));

    // 3x5 with match at (2,3).
    std::vector<std::vector<int>> g3 = {
        {7,7,7,7,7},
        {7,42,7,7,7},
        {7,7,7,7,7}
    };
    assert(findSpecialCell(g3) == std::make_pair(2,2));

    // 5x3 with match at (3,2).
    std::vector<std::vector<int>> g4 = {
        {7,7,7},
        {7,7,7},
        {7,42,7},
        {7,7,7},
        {7,7,7}
    };
    assert(findSpecialCell(g4) == std::make_pair(3,2));

    // Multiple matches, first in row-major is (2,2).
    std::vector<std::vector<int>> g5 = {
        {7,7,7,7,7},
        {7,42,7,7,7},
        {7,7,7,7,7},
        {7,42,7,7,7},
        {7,7,7,7,7}
    };
    assert(findSpecialCell(g5) == std::make_pair(2,2));

    // One neighbor not 7.
    std::vector<std::vector<int>> g6 = {
        {7,7,7},
        {7,42,8},
        {7,7,7}
    };
    assert(findSpecialCell(g6) == std::make_pair(0,0));

    // Wrong center value but neighbors correct.
    std::vector<std::vector<int>> g7 = {
        {7,7,7},
        {7,0,7},
        {7,7,7}
    };
    assert(findSpecialCell(g7) == std::make_pair(0,0));

    // Small grid (2x2) returns no match.
    std::vector<std::vector<int>> g8 = {{7,7},{7,7}};
    assert(findSpecialCell(g8) == std::make_pair(0,0));

    // Empty grid returns no match.
    std::vector<std::vector<int>> g9;
    assert(findSpecialCell(g9) == std::make_pair(0,0));

    // Single row (1x3) returns no match.
    std::vector<std::vector<int>> g10 = {{7,42,7}};
    assert(findSpecialCell(g10) == std::make_pair(0,0));

    return 0;
}

// The core algorithm is a nested loop over all interior cells (skipping the first and last row and column because those cannot have all eight neighbors inside the grid). For each candidate cell at `(i,j)`, check the value at `terrain[i][j]` equals 42, and then check all eight neighbors: `(i-1,j-1)`, `(i-1,j)`, `(i-1,j+1)`, `(i,j-1)`, `(i,j+1)`, `(i+1,j-1)`, `(i+1,j)`, `(i+1,j+1)` — each must equal 7. If all conditions hold, immediately return `{i+1, j+1}` (converting from 0-based to 1-based). Since the first match from top-left to bottom-right is required, returning at the first success is correct. If no match is found after the loops, return `{0,0}`. Edge cases: if the grid has fewer than 3 rows or columns, return `{0,0}` early. Time complexity is \(O(R \times C)\) for a grid of \(R\) rows and \(C\) columns, because each cell is visited a constant number of times; auxiliary space is \(O(1)\) (ignoring the input vector). The solution uses `const` references to avoid copying and `const` correctness for the input.
