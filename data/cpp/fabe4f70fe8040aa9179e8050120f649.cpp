// Given a 2D matrix of integers, write a C++ function `setZeroes` that modifies the matrix in-place by setting every element in any row or column that contains at least one zero to zero. The function must handle non-empty rectangular matrices (all rows have the same number of columns). The algorithm must use **O(1)** auxiliary space (excluding the input matrix itself) — using additional boolean vectors, arrays, or sets of any kind is not allowed. The original matrix may contain negative numbers, positive numbers, and any number of zeros. Edge cases include the first row or first column already containing a zero, a matrix with a single row or single column, and a matrix entirely filled with non-zero values. The function must be `const`-correct on the input type (i.e., take `std::vector<std::vector<int>>&` but not mark it `const` because it modifies it).

// The key is to avoid storing separate sets of rows and columns that contain zeros, which would require O(m+n) space. Instead, we can use the first row and first column of the matrix itself as markers. First, we scan the entire matrix to determine if the first row and first column originally contain any zeros; we store these two booleans separately because the first row/column will be overwritten with markers. Then, for every element `matrix[r][c]` where `r>=1` and `c>=1`, if it is zero, we set `matrix[r][0]=0` and `matrix[0][c]=0` — these act as flags indicating that row `r` and column `c` should be zeroed. After that, we traverse the submatrix (rows 1..m-1, columns 1..n-1) again, and for each cell, if either its row marker `matrix[r][0]` or its column marker `matrix[0][c]` is zero, we set that cell to zero. Finally, we handle the first row and first column separately: if either was originally containing a zero, we fill the entire first row or first column with zeros. This approach uses only two boolean variables and the matrix itself, achieving O(1) auxiliary space. Time complexity is O(m*n) because we scan the matrix a constant number of times (three passes). Edge cases: a 1×1 matrix with a zero should become all zeros (it already is); a 1×n or m×1 matrix must be handled by the separate first-row/first-column logic, not the inner loop; if the first row contains a zero originally, we must not use it to store markers until after we've recorded the original status.

#include <vector>

// Set entire rows and columns to zero if they contain at least one zero.
// Uses O(1) auxiliary space by storing markers in the first row/column.
void setZeroes(std::vector<std::vector<int>>& matrix) {
    if (matrix.empty() || matrix[0].empty()) return;

    const int m = static_cast<int>(matrix.size());
    const int n = static_cast<int>(matrix[0].size());

    bool firstRowHasZero = false;
    bool firstColHasZero = false;

    // Check if original first row contains a zero.
    for (int c = 0; c < n; ++c) {
        if (matrix[0][c] == 0) {
            firstRowHasZero = true;
            break;
        }
    }

    // Check if original first column contains a zero.
    for (int r = 0; r < m; ++r) {
        if (matrix[r][0] == 0) {
            firstColHasZero = true;
            break;
        }
    }

    // Use first row and first column as markers for zeros found in the rest.
    for (int r = 1; r < m; ++r) {
        for (int c = 1; c < n; ++c) {
            if (matrix[r][c] == 0) {
                matrix[r][0] = 0;
                matrix[0][c] = 0;
            }
        }
    }

    // Zero out cells based on the markers.
    for (int r = 1; r < m; ++r) {
        for (int c = 1; c < n; ++c) {
            if (matrix[r][0] == 0 || matrix[0][c] == 0) {
                matrix[r][c] = 0;
            }
        }
    }

    // Handle first row if it originally contained a zero.
    if (firstRowHasZero) {
        for (int c = 0; c < n; ++c) {
            matrix[0][c] = 0;
        }
    }

    // Handle first column if it originally contained a zero.
    if (firstColHasZero) {
        for (int r = 0; r < m; ++r) {
            matrix[r][0] = 0;
        }
    }
}

#include <cassert>
#include <vector>

// Function declaration (or include the solution file).
void setZeroes(std::vector<std::vector<int>>& matrix);

int main() {
    // Test case 1: Ordinary case.
    std::vector<std::vector<int>> m1 = {{1,1,1},{1,0,1},{1,1,1}};
    setZeroes(m1);
    assert(m1 == std::vector<std::vector<int>>({{1,0,1},{0,0,0},{1,0,1}}));

    // Test case 2: First row contains zero.
    std::vector<std::vector<int>> m2 = {{0,1,2},{3,4,5}};
    setZeroes(m2);
    assert(m2 == std::vector<std::vector<int>>({{0,0,0},{0,4,5}}));

    // Test case 3: First column contains zero.
    std::vector<std::vector<int>> m3 = {{0,1},{2,3}};
    setZeroes(m3);
    assert(m3 == std::vector<std::vector<int>>({{0,0},{0,3}}));

    // Test case 4: Single element zero.
    std::vector<std::vector<int>> m4 = {{0}};
    setZeroes(m4);
    assert(m4 == std::vector<std::vector<int>>({{0}}));

    // Test case 5: Single row with a zero.
    std::vector<std::vector<int>> m5 = {{1,0,2}};
    setZeroes(m5);
    assert(m5 == std::vector<std::vector<int>>({{0,0,0}}));

    // Test case 6: Single column with a zero.
    std::vector<std::vector<int>> m6 = {{1},{0},{2}};
    setZeroes(m6);
    assert(m6 == std::vector<std::vector<int>>({{0},{0},{0}}));

    // Test case 7: No zeros.
    std::vector<std::vector<int>> m7 = {{1,2},{3,4}};
    setZeroes(m7);
    assert(m7 == std::vector<std::vector<int>>({{1,2},{3,4}}));

    // Test case 8: Multiple zeros spanning overlapping rows/columns.
    std::vector<std::vector<int>> m8 = {{1,2,3,4},{5,0,7,8},{9,10,11,12},{13,14,0,16}};
    setZeroes(m8);
    std::vector<std::vector<int>> expected8 = {{1,0,0,4},{0,0,0,0},{9,0,0,12},{0,0,0,0}};
    assert(m8 == expected8);

    // Test case 9: First row and first column both have zeros.
    std::vector<std::vector<int>> m9 = {{0,1},{2,3}};
    setZeroes(m9);
    assert(m9 == std::vector<std::vector<int>>({{0,0},{0,0}}));

    // Test case 10: 2x2 all zeros.
    std::vector<std::vector<int>> m10 = {{0,0},{0,0}};
    setZeroes(m10);
    assert(m10 == std::vector<std::vector<int>>({{0,0},{0,0}}));

    return 0;
}
