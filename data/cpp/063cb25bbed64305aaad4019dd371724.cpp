// Write a C++ function named `matrixSum` that accepts two matrices as 2D arrays of fixed maximum size 100x100, along with their respective row and column counts. The function must return the sum of the two matrices as a new 2D array (also of size 100x100) but delivered via a result pointer parameter. If the matrices have incompatible dimensions (i.e., row counts differ or column counts differ), the function must set the result matrix to a zero matrix and also return a boolean `false`; otherwise it performs element-wise addition and returns `true`. The function must be `const`-correct for the input matrices (i.e., they are read-only) and must not modify them. The result is always a valid 100x100 array, but only the top-left `row1` by `col1` entries are meaningful when addition is successful; the rest can be left uninitialized. You may assume the input matrices are already filled with integer values and their dimensions are positive and ≤100.

#include <cassert>

int main() {
    // Test 1: Normal addition of 2x2 matrices.
    int a1[100][100] = {{1, 2}, {3, 4}};
    int a2[100][100] = {{5, 6}, {7, 8}};
    int res1[100][100];
    assert(matrixSum(a1, 2, 2, a2, 2, 2, res1) == true);
    assert(res1[0][0] == 6);
    assert(res1[0][1] == 8);
    assert(res1[1][0] == 10);
    assert(res1[1][1] == 12);

    // Test 2: Dimension mismatch (row count differs) returns false and zeroes result.
    int b1[100][100] = {{1, 2}};
    int b2[100][100] = {{3, 4}, {5, 6}};
    int res2[100][100];
    assert(matrixSum(b1, 1, 2, b2, 2, 2, res2) == false);
    // Check that at least first few entries are zeroed.
    assert(res2[0][0] == 0);
    assert(res2[0][1] == 0);
    assert(res2[1][0] == 0);

    // Test 3: Dimension mismatch (column count differs) returns false and zeroes result.
    int c1[100][100] = {{1, 2, 3}};
    int c2[100][100] = {{4, 5}};
    int res3[100][100];
    assert(matrixSum(c1, 1, 3, c2, 1, 2, res3) == false);
    assert(res3[0][0] == 0);
    assert(res3[0][1] == 0);
    assert(res3[0][2] == 0);

    // Test 4: 1x1 matrices.
    int d1[100][100] = {{42}};
    int d2[100][100] = {{-17}};
    int res4[100][100];
    assert(matrixSum(d1, 1, 1, d2, 1, 1, res4) == true);
    assert(res4[0][0] == 25);

    // Test 5: Non-square dimensions 2x3 addition.
    int e1[100][100] = {{1, 2, 3}, {4, 5, 6}};
    int e2[100][100] = {{-1, 0, 1}, {2, 3, 4}};
    int res5[100][100];
    assert(matrixSum(e1, 2, 3, e2, 2, 3, res5) == true);
    assert(res5[0][0] == 0);
    assert(res5[0][1] == 2);
    assert(res5[0][2] == 4);
    assert(res5[1][0] == 6);
    assert(res5[1][1] == 8);
    assert(res5[1][2] == 10);

    // Test 6: Same dimensions but both zero.
    int f1[100][100] = {{0, 0}, {0, 0}};
    int f2[100][100] = {{0, 0}, {0, 0}};
    int res6[100][100];
    assert(matrixSum(f1, 2, 2, f2, 2, 2, res6) == true);
    assert(res6[0][0] == 0 && res6[1][1] == 0);

    // Test 7: Maximum dimensions 100x100 (just check returns true and a few values).
    int g1[100][100] = {};
    int g2[100][100] = {};
    // Fill with simple pattern to avoid uninitialized reads.
    for (int i = 0; i < 100; ++i) {
        for (int j = 0; j < 100; ++j) {
            g1[i][j] = i + j;
            g2[i][j] = 1;
        }
    }
    int res7[100][100];
    assert(matrixSum(g1, 100, 100, g2, 100, 100, res7) == true);
    assert(res7[0][0] == 1);
    assert(res7[99][99] == 199);

    return 0;
}

#include <cstddef>

// Computes the element-wise sum of two matrices.
// Preconditions: `mat1` and `mat2` are 100x100 arrays with valid leading dimensions `row1`, `col1`, `row2`, `col2`.
//                All dimensions are positive and ≤ 100.
// Output: `result` is a 100x100 array; when addition is possible, the top-left `row1` by `col1` block holds the sum.
// Returns: `true` on success, `false` on dimension mismatch (and zeroes the whole result).
bool matrixSum(const int mat1[100][100], int row1, int col1,
               const int mat2[100][100], int row2, int col2,
               int result[100][100]) {
    if (row1 != row2 || col1 != col2) {
        // Clear the entire result matrix to zero.
        for (int i = 0; i < 100; ++i) {
            for (int j = 0; j < 100; ++j) {
                result[i][j] = 0;
            }
        }
        return false;
    }

    for (int i = 0; i < row1; ++i) {
        for (int j = 0; j < col1; ++j) {
            result[i][j] = mat1[i][j] + mat2[i][j];
        }
    }
    return true;
}

// The core algorithm is trivial: if `row1 != row2` or `col1 != col2`, we should not compute a sum; instead set the entire result matrix to zero (all 100x100 entries) and return `false`. Otherwise, for each `i` from 0 to row1-1 and each `j` from 0 to col1-1, set `result[i][j] = mat1[i][j] + mat2[i][j]`. No other entries in the result need initialization. The main edge case is dimension mismatch, which must be handled gracefully without accessing out-of-bounds indices. Time complexity is O(rows × columns) because we iterate over every element once. Space complexity is O(1) auxiliary (the result matrix is provided by the caller, so we only modify it in place; we do not allocate extra storage). Since the matrix dimensions are at most 100, the worst-case operations are 10,000, which is trivial.
