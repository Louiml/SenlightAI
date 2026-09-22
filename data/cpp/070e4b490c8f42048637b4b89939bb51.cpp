/*
Given a 2D grid of non-negative integers where `1` represents a house and `0` represents empty land, write a C++ function `int minTotalDistance(const vector<vector<int>>& grid)` that computes the minimum total Manhattan distance required to meet at any point (not necessarily on a house or within the grid—meeting point can be any cell in the infinite plane, but the optimal meeting point will always be on a grid coordinate) such that the sum of distances from all houses to that point is minimized. The grid may be empty or have rows of length zero—return `0` in those cases. The grid dimensions can be up to, say, 200×200, and the number of houses can range from 0 to all cells. Handle duplicates of houses (multiple `1`s in the same cell) correctly—each `1` counts as a separate house. The meeting point can be chosen as any integer-coordinate cell (including outside the grid bounds, but the median property guarantees an optimal point inside or on grid boundaries). Ensure your solution works for grids where the number of houses is even (the median is typically taken as the upper middle index) and for odd counts. Provide a self-contained function with proper `const` correctness, including necessary headers.
*/
#include <vector>
#include <algorithm>
#include <cstdlib>

// Compute the minimum total Manhattan distance to meet all houses.
int minTotalDistance(const std::vector<std::vector<int>>& grid) {
    if (grid.empty() || grid[0].empty()) {
        return 0;
    }

    const size_t rows = grid.size();
    const size_t cols = grid[0].size();

    std::vector<int> rowCoords;
    std::vector<int> colCoords;

    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            if (grid[i][j] == 1) {
                rowCoords.push_back(static_cast<int>(i));
                colCoords.push_back(static_cast<int>(j));
            }
        }
    }

    if (rowCoords.empty()) {
        return 0;
    }

    // Row coordinates are already sorted because we scan row-major.
    const int meetRow = rowCoords[rowCoords.size() / 2];

    // Sort column coordinates to find the median.
    std::sort(colCoords.begin(), colCoords.end());
    const int meetCol = colCoords[colCoords.size() / 2];

    int totalDist = 0;
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            if (grid[i][j] == 1) {
                totalDist += std::abs(static_cast<int>(i) - meetRow)
                           + std::abs(static_cast<int>(j) - meetCol);
            }
        }
    }

    return totalDist;
}
#include <cassert>
#include <vector>

// The solution function is declared above.
// Test cases for minTotalDistance.
int main() {
    // Single house
    std::vector<std::vector<int>> grid1 = {{0, 0, 0},
                                           {0, 1, 0},
                                           {0, 0, 0}};
    assert(minTotalDistance(grid1) == 0);

    // Two houses on same row
    std::vector<std::vector<int>> grid2 = {{1, 0, 1}};
    assert(minTotalDistance(grid2) == 2); // meet at index 1

    // Two houses on same column
    std::vector<std::vector<int>> grid3 = {{1},
                                           {0},
                                           {1}};
    assert(minTotalDistance(grid3) == 2); // meet at row 1

    // Classic example from LeetCode
    std::vector<std::vector<int>> grid4 = {{1, 0, 0, 0, 1},
                                           {0, 0, 0, 0, 0},
                                           {0, 0, 1, 0, 0}};
    // Houses at (0,0), (0,4), (2,2) -> meet at (0,2) => 0+2 + 0+2 + 2+0 = 6
    assert(minTotalDistance(grid4) == 6);

    // Empty grid
    std::vector<std::vector<int>> grid5;
    assert(minTotalDistance(grid5) == 0);

    // Grid with first row empty
    std::vector<std::vector<int>> grid6 = {{}};
    assert(minTotalDistance(grid6) == 0);

    // All zeros
    std::vector<std::vector<int>> grid7 = {{0, 0},
                                           {0, 0}};
    assert(minTotalDistance(grid7) == 0);

    // Even number of houses: two in middle columns
    std::vector<std::vector<int>> grid8 = {{1, 0, 0, 1}};
    // Houses at col 0 and 3, median col = 3/2=1 => dist = 1+2=3
    assert(minTotalDistance(grid8) == 3);

    // Larger grid with multiple houses
    std::vector<std::vector<int>> grid9 = {{1, 0, 1},
                                           {0, 1, 0},
                                           {1, 0, 1}};
    // Houses at (0,0),(0,2),(1,1),(2,0),(2,2)
    // Sorted rows: [0,0,1,2,2] median=1
    // Sorted cols: [0,0,1,2,2] median=1
    // Distances: (0,0):2, (0,2):2, (1,1):0, (2,0):2, (2,2):2 => total=8
    assert(minTotalDistance(grid9) == 8);

    return 0;
}
// The problem is a classic 1D median minimization extended to two dimensions separately because Manhattan distance separates into independent x and y components. For a set of points, the sum of absolute deviations is minimized at any median of the coordinates. Thus, collect all row indices of houses into one vector and all column indices into another vector (or collect column indices separately and sort). Since the grid is traversed row-major, the row vector is already sorted; the column vector must be sorted. The optimal meeting row is the median of the row indices, and the optimal meeting column is the median of the column indices. For an even number of houses, either of the two middle values works; using index `size()/2` (upper median) is standard and yields the same minimum distance. After determining the median coordinates, iterate over all grid cells again and sum the Manhattan distance from each house to the median. Edge cases: empty grid or empty first row → return 0; no houses → return 0 (since median vectors are empty, but we check before indexing—in implementation, handle by returning 0 if house count is zero). Time complexity: O(R*C) to scan the grid twice (once to collect indices, once to compute distances) plus O(H log H) for sorting the column vector, where H is the number of houses, but since H ≤ R*C, overall O(R*C log(R*C)) worst-case. Space complexity: O(H) for storing indices, which is O(R*C) worst-case.
