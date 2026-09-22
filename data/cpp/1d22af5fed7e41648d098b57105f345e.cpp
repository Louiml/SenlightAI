/*
Write a C++ function named `transformGrid` that takes a non-empty 2D vector of integers (`grid`), where every entry is either `0` or `1`, and returns a new 2D vector of the same dimensions. For each cell `(r, c)`, the output value must be computed as: `onesRow[r] + onesCol[c] - zerosRow[r] - zerosCol[c]`, where `onesRow[r]` is the count of `1`s in row `r`, `onesCol[c]` is the count of `1`s in column `c`, `zerosRow[r]` is the count of `0`s in row `r`, and `zerosCol[c]` is the count of `0`s in column `c`. The function must not modify the input vector; instead, it must create and return a new vector. Handle edge cases such as a single row, a single column, or a grid consisting entirely of zeros or ones. The function should be efficient and use only constant extra space beyond the input and output vectors.
*/

#include <vector>

// Given a grid of 0s and 1s, return a new grid where each cell (r,c) is:
// onesRow[r] + onesCol[c] - zerosRow[r] - zerosCol[c].
std::vector<std::vector<int>> transformGrid(const std::vector<std::vector<int>>& grid) {
    const int rows = static_cast<int>(grid.size());
    const int cols = static_cast<int>(grid[0].size());

    std::vector<int> onesRow(rows, 0);
    std::vector<int> onesCol(cols, 0);

    // Count ones in each row and column.
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            onesRow[r] += grid[r][c];
            onesCol[c] += grid[r][c];
        }
    }

    std::vector<std::vector<int>> result(rows, std::vector<int>(cols));

    // Fill the result using the simplified formula.
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            result[r][c] = 2 * onesRow[r] + 2 * onesCol[c] - rows - cols;
        }
    }

    return result;
}

#include <cassert>
#include <vector>

// Function declaration (assume provided above).
std::vector<std::vector<int>> transformGrid(const std::vector<std::vector<int>>& grid);

int main() {
    // 2x2 mixed grid
    std::vector<std::vector<int>> g1 = {{1, 0}, {0, 1}};
    std::vector<std::vector<int>> r1 = transformGrid(g1);
    assert(r1 == std::vector<std::vector<int>>({{0, 0}, {0, 0}}));

    // 1x3 grid
    std::vector<std::vector<int>> g2 = {{1, 1, 0}};
    std::vector<std::vector<int>> r2 = transformGrid(g2);
    assert(r2 == std::vector<std::vector<int>>({{1, 1, -1}}));

    // 3x1 grid
    std::vector<std::vector<int>> g3 = {{0}, {1}, {0}};
    std::vector<std::vector<int>> r3 = transformGrid(g3);
    assert(r3 == std::vector<std::vector<int>>({{-1}, {1}, {-1}}));

    // All ones
    std::vector<std::vector<int>> g4 = {{1, 1}, {1, 1}};
    std::vector<std::vector<int>> r4 = transformGrid(g4);
    assert(r4 == std::vector<std::vector<int>>({{0, 0}, {0, 0}}));

    // All zeros
    std::vector<std::vector<int>> g5 = {{0, 0, 0}};
    std::vector<std::vector<int>> r5 = transformGrid(g5);
    assert(r5 == std::vector<std::vector<int>>({{-2, -2, -2}}));

    // Larger grid with varying values
    std::vector<std::vector<int>> g6 = {{1, 0, 1}, {0, 1, 0}, {1, 1, 0}};
    std::vector<std::vector<int>> r6 = transformGrid(g6);
    assert(r6 == std::vector<std::vector<int>>({{2, 0, 0}, {-2, 0, -2}, {0, 0, -2}}));

    // Ensure the input is not modified
    std::vector<std::vector<int>> original = {{1, 0}, {0, 1}};
    auto copy = original;
    transformGrid(original);
    assert(original == copy);

    return 0;
}

// The solution first computes the number of `1`s in each row and each column by iterating over the entire grid once. Let `rows` = number of rows, `cols` = number of columns. For row `r`, the number of zeros is simply `cols - onesRow[r]` because every cell is either 0 or 1. Similarly, for column `c`, the number of zeros is `rows - onesCol[c]`. Then for each cell `(r, c)`, the required value simplifies to:  
// `onesRow[r] + onesCol[c] - (cols - onesRow[r]) - (rows - onesCol[c])`  
// = `2 * onesRow[r] + 2 * onesCol[c] - rows - cols`.  
//
// This avoids computing zero counts separately. The algorithm uses two vectors of size `rows` and `cols` to store the one-counts, which is `O(rows + cols)` auxiliary space. The time complexity is `O(rows * cols)` because we traverse the grid twice: once to compute counts and once to fill the result. Edge cases: if the grid has one row, `cols - onesRow[r]` correctly gives zeros in that row; if all cells are `1`, the formula yields `rows + cols - rows - cols = 0` for each cell; if all cells are `0`, the formula yields `0 + 0 - rows - cols` which is negative, correct as per the definition. The function returns a new vector, leaving the input unchanged.
