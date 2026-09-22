/*
Write a C++ function `int firstCompleteIndex(vector<int>& paintOrder, vector<vector<int>>& grid)` that simulates painting cells of a rectangular grid one by one in the order given by `paintOrder`. Each value in `paintOrder` appears exactly once in `grid`. When a value is painted, its corresponding cell is marked as painted. Return the zero-based index of the first value in `paintOrder` that, after painting, causes either an entire row or an entire column of the grid to be fully painted. The grid contains distinct positive integers, and you may assume that `paintOrder` is a permutation of all values in `grid`. If no row or column ever becomes fully painted (which cannot happen for a valid input, but for safety return -1), return -1. The function must be efficient for grids up to 1000x1000 and paintOrder of length up to 10^6.
*/
#include <unordered_map>
#include <vector>

// Given a grid of distinct positive integers and an order to paint all cells,
// returns the earliest index in paintOrder that completes any full row or column.
int firstCompleteIndex(const std::vector<int>& paintOrder,
                       const std::vector<std::vector<int>>& grid) {
    const int numRows = static_cast<int>(grid.size());
    if (numRows == 0) return -1;
    const int numCols = static_cast<int>(grid[0].size());

    // Map from value to its {row, col} position.
    std::unordered_map<int, std::pair<int, int>> valueToPos;
    valueToPos.reserve(static_cast<size_t>(numRows) * numCols);

    for (int row = 0; row < numRows; ++row) {
        for (int col = 0; col < numCols; ++col) {
            valueToPos[grid[row][col]] = {row, col};
        }
    }

    std::vector<int> rowPainted(numRows, 0);
    std::vector<int> colPainted(numCols, 0);

    for (int i = 0; i < static_cast<int>(paintOrder.size()); ++i) {
        const int value = paintOrder[i];
        const auto& pos = valueToPos.at(value);
        const int row = pos.first;
        const int col = pos.second;

        ++rowPainted[row];
        ++colPainted[col];

        if (rowPainted[row] == numCols || colPainted[col] == numRows) {
            return i;
        }
    }

    return -1;
}
#include <cassert>
#include <vector>

int main() {
    // Example from typical LeetCode problem: grid 2x3, order [2,1,3,4,5,6]
    std::vector<int> order1 = {2, 1, 3, 4, 5, 6};
    std::vector<std::vector<int>> grid1 = {{1, 3, 5}, {2, 4, 6}};
    assert(firstCompleteIndex(order1, grid1) == 2); // painting 3 completes first row

    // Single cell grid
    std::vector<int> order2 = {7};
    std::vector<std::vector<int>> grid2 = {{7}};
    assert(firstCompleteIndex(order2, grid2) == 0);

    // Single row grid
    std::vector<int> order3 = {2, 1, 3};
    std::vector<std::vector<int>> grid3 = {{1, 2, 3}};
    assert(firstCompleteIndex(order3, grid3) == 1); // second paint completes row

    // Single column grid
    std::vector<int> order4 = {5, 10, 15};
    std::vector<std::vector<int>> grid4 = {{5}, {10}, {15}};
    assert(firstCompleteIndex(order4, grid4) == 2);

    // Larger grid: 3x3, order paints all of first column first
    std::vector<int> order5 = {9, 6, 3, 1, 2, 4, 5, 7, 8};
    std::vector<std::vector<int>> grid5 = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    assert(firstCompleteIndex(order5, grid5) == 2); // after painting 3, first column complete

    // Edge: order completes a row and column simultaneously (2x2) at last cell
    std::vector<int> order6 = {1, 2, 3, 4};
    std::vector<std::vector<int>> grid6 = {{1, 2}, {3, 4}};
    assert(firstCompleteIndex(order6, grid6) == 3);

    return 0;
}
// The optimal approach avoids repeatedly checking rows/columns after each paint. Instead, maintain counters for how many cells have been painted in each row and each column. Preprocess the grid to build a hash map from each value to its (row, column) position. Then iterate through `paintOrder` in order. For each value, look up its position, increment the corresponding row counter and column counter. After each increment, if either the row counter equals the number of columns in the grid, or the column counter equals the number of rows, then that index is the answer. Key edge cases: a 1x1 grid, where the first paint completes both row and column; grids with one row or one column; duplicate values are not allowed as per problem statement; all values are positive, so no sign tricks needed. Complexity: building the map takes O(rows*cols) time and O(rows*cols) space. The iteration over `paintOrder` takes O(paintOrder.size()) time (which equals rows*cols). Total time O(n) where n = rows*cols, space O(n) for the map plus O(rows+cols) for counters. Use `unordered_map` for average O(1) lookups, but since values are up to 10^6, we could also use a vector if values are within a known bound—but the map is safer for arbitrary distinct integers.
