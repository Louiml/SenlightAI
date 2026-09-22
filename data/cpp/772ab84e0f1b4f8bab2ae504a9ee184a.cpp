Write a C++ function named `setZeroes` that takes a non-empty 2D vector of integers (representing a matrix) and modifies it in place so that if any cell contains the value `0`, the entire row and the entire column containing that cell are set to `0`. The function should handle matrices of arbitrary size (at least 1 row and 1 column), including cases with multiple zeros, zeros in the same row or column, and no zeros at all. The original positions of the zeros themselves do not need special preservation; after the operation, any row or column that originally contained at least one zero must be entirely zeros. The function must not use any additional matrix-sized storage; only O(rows + cols) auxiliary space is allowed.
// The main idea is to avoid altering the matrix while scanning for zeros, because changing rows/columns during the first pass would create new zeros that incorrectly trigger more rows/columns to be zeroed. Instead, use two auxiliary boolean arrays (or integer flags) of sizes equal to the number of rows and columns to record which rows and which columns contain at least one zero. In the first pass, iterate through every cell; whenever a `0` is found, mark its row index and column index in the corresponding arrays. In a second pass, iterate through each row; if that row is marked, set all its entries to `0`. In a third pass, iterate through each column; if that column is marked, set all its entries to `0`. This order ensures that setting a row to zero does not interfere with the column pass, because the column markings were already determined from the original matrix. Edge cases include a matrix with a single row or single column, multiple zeros sharing the same row/column (no duplication issues since arrays are boolean), and a matrix with no zeros (the auxiliary arrays remain all false, and the matrix is unchanged). The time complexity is O(rows × cols) because each cell is visited a constant number of times (first pass plus row/column zeroing passes sum to at most 3×cells). The space complexity is O(rows + cols) for the two auxiliary arrays.
#include <vector>

// Modify the matrix in place: if any cell is 0, set its entire row and column to 0.
// Uses O(rows + cols) extra space, linear in the number of rows and columns.
void setZeroes(std::vector<std::vector<int>>& matrix) {
    const int rows = static_cast<int>(matrix.size());
    const int cols = static_cast<int>(matrix[0].size());

    std::vector<bool> zeroRows(rows, false);
    std::vector<bool> zeroCols(cols, false);

    // First pass: mark rows and columns that contain a zero.
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (matrix[i][j] == 0) {
                zeroRows[i] = true;
                zeroCols[j] = true;
            }
        }
    }

    // Second pass: zero out marked rows.
    for (int i = 0; i < rows; ++i) {
        if (zeroRows[i]) {
            for (int j = 0; j < cols; ++j) {
                matrix[i][j] = 0;
            }
        }
    }

    // Third pass: zero out marked columns.
    for (int j = 0; j < cols; ++j) {
        if (zeroCols[j]) {
            for (int i = 0; i < rows; ++i) {
                matrix[i][j] = 0;
            }
        }
    }
}
#include <cassert>
#include <vector>

int main() {
    // Test 1: basic case with one zero in the middle
    std::vector<std::vector<int>> m1 = {{1, 2, 3}, {4, 0, 6}, {7, 8, 9}};
    setZeroes(m1);
    assert((m1 == std::vector<std::vector<int>>{{1, 0, 3}, {0, 0, 0}, {7, 0, 9}}));

    // Test 2: multiple zeros (including same row and column)
    std::vector<std::vector<int>> m2 = {{0, 1, 2}, {3, 4, 5}, {6, 7, 0}};
    setZeroes(m2);
    assert((m2 == std::vector<std::vector<int>>{{0, 0, 0}, {0, 4, 0}, {0, 0, 0}}));

    // Test 3: no zeros; matrix should remain unchanged
    std::vector<std::vector<int>> m3 = {{1, 2}, {3, 4}};
    setZeroes(m3);
    assert((m3 == std::vector<std::vector<int>>{{1, 2}, {3, 4}}));

    // Test 4: single row with a zero
    std::vector<std::vector<int>> m4 = {{1, 0, 3}};
    setZeroes(m4);
    assert((m4 == std::vector<std::vector<int>>{{0, 0, 0}}));

    // Test 5: single column with a zero
    std::vector<std::vector<int>> m5 = {{1}, {0}, {3}};
    setZeroes(m5);
    assert((m5 == std::vector<std::vector<int>>{{0}, {0}, {0}}));

    // Test 6: zeros already filling entire row/column (extra pass harmless)
    std::vector<std::vector<int>> m6 = {{0, 0}, {1, 2}};
    setZeroes(m6);
    assert((m6 == std::vector<std::vector<int>>{{0, 0}, {0, 0}}));

    // Test 7: large matrix with multiple non-adjacent zeros
    std::vector<std::vector<int>> m7 = {{1, 0, 1, 1}, {1, 1, 1, 1}, {1, 1, 0, 1}};
    setZeroes(m7);
    assert((m7 == std::vector<std::vector<int>>{{0, 0, 0, 0}, {1, 0, 0, 1}, {0, 0, 0, 0}}));

    // Test 8: 2x2 matrix where all cells become zero
    std::vector<std::vector<int>> m8 = {{0, 1}, {2, 0}};
    setZeroes(m8);
    assert((m8 == std::vector<std::vector<int>>{{0, 0}, {0, 0}}));

    // Test 9: 1x1 matrix with zero
    std::vector<std::vector<int>> m9 = {{0}};
    setZeroes(m9);
    assert((m9 == std::vector<std::vector<int>>{{0}}));

    // Test 10: 1x1 matrix without zero
    std::vector<std::vector<int>> m10 = {{5}};
    setZeroes(m10);
    assert((m10 == std::vector<std::vector<int>>{{5}}));

    return 0;
}
