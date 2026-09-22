// Write a C++ function named `zeroOutRowsAndColumns` that takes a non-empty matrix represented as `std::vector<std::vector<int>>` and returns a new matrix of the same dimensions where every element in any row or column that originally contained at least one `0` is set to `0`. All other elements retain their original values. The function must not modify the input matrix; it should return the result by value. The input matrix is guaranteed to be rectangular (all rows have the same length) and contain at least one row and one column. The function should handle arbitrary matrix sizes and any integer values (including negatives).
// The core idea is to first identify which rows and columns contain at least one zero, then construct the output matrix by setting every cell to zero if its row or column is marked. A direct approach would scan the entire matrix once to record zero positions in two separate boolean vectors (or integer vectors, using 0/1 markers): one for rows, one for columns. After this first pass, we build the result matrix (since the input is passed by `const` reference, we start by copying it) and then perform a second pass over all cells: if `rowMarked[i] == true` or `colMarked[j] == true`, set `result[i][j] = 0`. This ensures that any cell sharing a row or column with a zero becomes zero, which matches the specification. Edge cases include: a matrix with no zeros (result equals input), a matrix with a single zero (that row and column become all zeros), and matrices with multiple zeros (their rows/columns may overlap). Time complexity is \(O(m \cdot n)\) for the first pass plus \(O(m \cdot n)\) for the second pass, totaling \(O(m \cdot n)\). Space complexity is \(O(m + n)\) for the markers, plus the output matrix copy which is \(O(m \cdot n)\) (but the problem focuses on the auxiliary space beyond the input/output). The function should be `const` correct, taking the input as a `const` reference and returning by value, so the caller can compare with expected results.
#include <vector>
#include <cstddef>

// Given a rectangular matrix, return a new matrix where every element in any row
// or column that originally contained a zero is set to zero. The input is not modified.
std::vector<std::vector<int>> zeroOutRowsAndColumns(const std::vector<std::vector<int>>& matrix) {
    if (matrix.empty() || matrix[0].empty()) {
        return {};
    }

    const std::size_t rows = matrix.size();
    const std::size_t cols = matrix[0].size();

    // Mark rows and columns that contain at least one zero.
    std::vector<bool> rowHasZero(rows, false);
    std::vector<bool> colHasZero(cols, false);

    for (std::size_t i = 0; i < rows; ++i) {
        for (std::size_t j = 0; j < cols; ++j) {
            if (matrix[i][j] == 0) {
                rowHasZero[i] = true;
                colHasZero[j] = true;
            }
        }
    }

    // Build the output matrix by copying the input, then zeroing marked cells.
    std::vector<std::vector<int>> result = matrix;
    for (std::size_t i = 0; i < rows; ++i) {
        if (rowHasZero[i]) {
            std::fill(result[i].begin(), result[i].end(), 0);
        } else {
            for (std::size_t j = 0; j < cols; ++j) {
                if (colHasZero[j]) {
                    result[i][j] = 0;
                }
            }
        }
    }

    return result;
}
#include <cassert>
#include <vector>

// Declare the function from the solution (include the solution code above in a single file).
std::vector<std::vector<int>> zeroOutRowsAndColumns(const std::vector<std::vector<int>>& matrix);

int main() {
    // Test 1: Simple 2x2 with one zero.
    std::vector<std::vector<int>> m1 = {{1, 0}, {2, 3}};
    std::vector<std::vector<int>> expected1 = {{0, 0}, {2, 0}};
    assert(zeroOutRowsAndColumns(m1) == expected1);

    // Test 2: 3x3 with no zeros – unchanged.
    std::vector<std::vector<int>> m2 = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    assert(zeroOutRowsAndColumns(m2) == m2);

    // Test 3: Single row with a zero.
    std::vector<std::vector<int>> m3 = {{0, 5, 6}};
    std::vector<std::vector<int>> expected3 = {{0, 0, 0}};
    assert(zeroOutRowsAndColumns(m3) == expected3);

    // Test 4: Single column with a zero.
    std::vector<std::vector<int>> m4 = {{0}, {1}, {2}};
    std::vector<std::vector<int>> expected4 = {{0}, {0}, {0}};
    assert(zeroOutRowsAndColumns(m4) == expected4);

    // Test 5: 4x4 with multiple zeros including overlapping rows/columns.
    std::vector<std::vector<int>> m5 = {{1, 1, 1, 1},
                                        {1, 0, 1, 1},
                                        {1, 1, 0, 0},
                                        {0, 0, 0, 1}};
    std::vector<std::vector<int>> expected5 = {{0, 0, 0, 0},
                                               {0, 0, 0, 0},
                                               {0, 0, 0, 0},
                                               {0, 0, 0, 0}};
    assert(zeroOutRowsAndColumns(m5) == expected5);

    // Test 6: Non-zero elements remain where their row/col have no zeros.
    std::vector<std::vector<int>> m6 = {{1, 2, 3}, {4, 0, 6}, {7, 8, 9}};
    std::vector<std::vector<int>> expected6 = {{1, 0, 3}, {0, 0, 0}, {7, 0, 9}};
    assert(zeroOutRowsAndColumns(m6) == expected6);

    // Test 7: Negative values are preserved if not in zero rows/cols.
    std::vector<std::vector<int>> m7 = {{-1, -2}, {0, 4}, {5, 6}};
    std::vector<std::vector<int>> expected7 = {{0, -2}, {0, 0}, {5, 0}};
    assert(zeroOutRowsAndColumns(m7) == expected7);

    // Test 8: Single element zero.
    std::vector<std::vector<int>> m8 = {{0}};
    assert(zeroOutRowsAndColumns(m8) == m8);

    return 0;
}
