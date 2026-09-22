// Write a C++ function `minimumRectangleArea` that takes a 2D vector of integers `grid` (where each cell is either 0 or 1, with at least one 1) and returns the area of the smallest axis-aligned rectangle that contains all the 1-cells. The rectangle's sides must be parallel to the grid's axes, and its area is computed as (number of rows spanned) × (number of columns spanned). For example, if the grid is `{{0,1,0},{1,0,0}}`, the minimum rectangle spans rows 0–1 and columns 0–1, giving area 4. The grid is non-empty, rectangular, and guaranteed to contain at least one 1.
#include <cassert>
#include <vector>

int main() {
    // Single 1
    assert(minimumRectangleArea({{0,0,0},{0,1,0},{0,0,0}}) == 1);
    
    // All 1s in a 2x3 grid
    assert(minimumRectangleArea({{1,1,1},{1,1,1}}) == 6);
    
    // Corners
    assert(minimumRectangleArea({{1,0},{0,1}}) == 4);
    assert(minimumRectangleArea({{1,0},{0,0}}) == 1);
    
    // Sparse across whole grid
    assert(minimumRectangleArea({{1,0,0},{0,0,0},{0,0,1}}) == 9);
    
    // Single row
    assert(minimumRectangleArea({{0,1,0,1}}) == 4);
    
    // Single column
    assert(minimumRectangleArea({{0},{1},{0},{1}}) == 4);
    
    // Larger example
    assert(minimumRectangleArea({
        {0,0,0,0},
        {0,1,0,0},
        {0,0,0,0},
        {0,0,1,0}
    }) == 6);

    // Multiple 1s in same row and column
    assert(minimumRectangleArea({
        {0,1,0},
        {1,1,1},
        {0,1,0}
    }) == 9);

    // 1 in first row, last column
    assert(minimumRectangleArea({{0,0,1},{0,0,0}}) == 3);

    return 0;
}
#include <vector>
#include <algorithm>
#include <cstddef>

// Returns the area of the smallest axis-aligned rectangle
// containing all cells equal to 1 in the given grid.
int minimumRectangleArea(const std::vector<std::vector<int>>& grid) {
    if (grid.empty() || grid[0].empty()) {
        return 0;
    }

    const std::size_t rowCount = grid.size();
    const std::size_t colCount = grid[0].size();

    std::size_t minRow = rowCount, maxRow = 0;
    std::size_t minCol = colCount, maxCol = 0;

    for (std::size_t i = 0; i < rowCount; ++i) {
        for (std::size_t j = 0; j < colCount; ++j) {
            if (grid[i][j] == 1) {
                minRow = std::min(minRow, i);
                maxRow = std::max(maxRow, i);
                minCol = std::min(minCol, j);
                maxCol = std::max(maxCol, j);
            }
        }
    }

    const std::size_t height = maxRow - minRow + 1;
    const std::size_t width  = maxCol - minCol + 1;
    return static_cast<int>(height * width);
}
// The solution scans the entire grid once, tracking the minimum row index, maximum row index, minimum column index, and maximum column index among all cells equal to 1. Initially, set `minRow = grid.size()`, `maxRow = 0`, `minCol = grid[0].size()`, `maxCol = 0` so that any found 1 will correctly update these bounds. After the scan, the rectangle's height is `maxRow - minRow + 1` and width is `maxCol - minCol + 1`, so the area is the product of those. Edge cases: if there is exactly one 1, all bounds collapse to that cell's row and column, yielding area 1. The problem guarantees at least one 1, so no need to handle an all-zero grid. Time complexity is O(n·m) where n and m are grid dimensions, and space complexity is O(1) beyond the input.
