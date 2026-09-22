/*
Write a standalone C++ function named `matrixTranspose` that takes a two-dimensional integer array (represented as a fixed-size 2D array) along with its row and column counts, computes its transpose into a separate output 2D array, and returns the transposed matrix by filling the provided output array. The function should work for any valid nonzero input dimensions up to the fixed array sizes (e.g., max 10x10), and the output array must contain the transposed values such that `output[j][i] == input[i][j]` for all valid indices. The function should not print anything; the caller will handle output.
*/

#include <cstddef>   // for size_t if needed, but not required

// Compute the transpose of a fixed-size 2D integer array.
// input:    rows x cols matrix (max MAX_ROWS rows, MAX_COLS columns)
// output:   cols x rows matrix (must have dimensions [MAX_COLS][MAX_ROWS])
// rows, cols: number of rows and columns in the input matrix (nonzero up to limits)
void matrixTranspose(const int input[][10], int rows, int cols, int output[][10]) {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            output[j][i] = input[i][j];
        }
    }
}

#include <cassert>

int main() {
    const int MAX = 10;
    
    // Test case 1: 2x3 matrix
    int mat1[2][3] = {{1, 2, 3}, {4, 5, 6}};
    int trans1[3][2] = {};
    matrixTranspose(mat1, 2, 3, trans1);
    assert(trans1[0][0] == 1 && trans1[0][1] == 4);
    assert(trans1[1][0] == 2 && trans1[1][1] == 5);
    assert(trans1[2][0] == 3 && trans1[2][1] == 6);

    // Test case 2: 3x2 matrix
    int mat2[3][2] = {{7, 8}, {9, 10}, {11, 12}};
    int trans2[2][3] = {};
    matrixTranspose(mat2, 3, 2, trans2);
    assert(trans2[0][0] == 7 && trans2[0][1] == 9 && trans2[0][2] == 11);
    assert(trans2[1][0] == 8 && trans2[1][1] == 10 && trans2[1][2] == 12);

    // Test case 3: 1x1
    int mat3[1][1] = {{42}};
    int trans3[1][1] = {};
    matrixTranspose(mat3, 1, 1, trans3);
    assert(trans3[0][0] == 42);

    // Test case 4: 1x4
    int mat4[1][4] = {{5, 6, 7, 8}};
    int trans4[4][1] = {};
    matrixTranspose(mat4, 1, 4, trans4);
    assert(trans4[0][0] == 5 && trans4[1][0] == 6 && trans4[2][0] == 7 && trans4[3][0] == 8);

    // Test case 5: 4x1
    int mat5[4][1] = {{1}, {2}, {3}, {4}};
    int trans5[1][4] = {};
    matrixTranspose(mat5, 4, 1, trans5);
    assert(trans5[0][0] == 1 && trans5[0][1] == 2 && trans5[0][2] == 3 && trans5[0][3] == 4);

    // Test case 6: Square 3x3
    int mat6[3][3] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    int trans6[3][3] = {};
    matrixTranspose(mat6, 3, 3, trans6);
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            assert(trans6[i][j] == mat6[j][i]);

    // Edge case: 0 rows / cols (should not crash)
    int mat7[1][1] = {{0}};
    int trans7[1][1] = {{99}};
    matrixTranspose(mat7, 0, 1, trans7);
    assert(trans7[0][0] == 99); // unchanged

    return 0;
}

// The main algorithm is straightforward nested loops: for every row index `i` from 0 to `rows-1` and for every column index `j` from 0 to `cols-1`, assign `transpose[j][i] = matrix[i][j]`. Because the output dimensions are swapped (the transpose of an `r x c` matrix is `c x r`), ensure the output array is declared with dimensions `[maxCols][maxRows]` to accommodate the swap. Edge cases: if rows or cols are zero, the function should simply do nothing (the loops won't execute), and the output array remains untouched; if rows and cols are equal, the operation is still valid and produces the expected result. Since the input is read-only, pass it as `const` (e.g., `const int input[][MAX_COLS]`) to guarantee no modification. Time complexity is `O(rows * cols)`, which is optimal because every element must be visited once. Space complexity is `O(rows * cols)` for the output array, but note the function itself does not allocate extra memory beyond the output array provided by the caller; auxiliary space is `O(1)`.
