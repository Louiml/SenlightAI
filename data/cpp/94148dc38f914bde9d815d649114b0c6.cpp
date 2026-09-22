/*
Write a C++ function named `extractMiddleCols` that takes a square matrix of integers (using `Eigen::MatrixXi`) and an integer column index `col`, and returns a new `Eigen::MatrixXi` containing 3 consecutive columns starting from `col` (i.e., columns `col`, `col+1`, `col+2`). The function must handle the case where the matrix size is at least 4 and the requested starting column is valid so that the 3 columns exist. Assume the input is always valid (size ≥ 4 and 0 ≤ col ≤ size-3). The function should return a matrix of size `rows × 3`. Use `middleCols` or equivalent slicing. Provide only the function definition and necessary includes, with proper `const` correctness.
*/

#include <Eigen/Core>

// Return a new matrix containing 3 consecutive columns starting at 'col'.
// Assumes 'matrix' has at least 4 columns and 'col' is such that col+2 < cols.
Eigen::MatrixXi extractMiddleCols(const Eigen::MatrixXi& matrix, int col) {
    // Validate input (optional but good practice)
    assert(col >= 0 && col + 2 < matrix.cols());
    // Extract columns [col, col+1, col+2] and copy into a new matrix.
    return matrix.middleCols(col, 3);
}

#include <Eigen/Core>
#include <cassert>

// Declare the function (since we are not including the solution file)
Eigen::MatrixXi extractMiddleCols(const Eigen::MatrixXi& matrix, int col);

int main() {
    // Test 1: Basic 5x5 matrix
    Eigen::MatrixXi A(5, 5);
    A << 1, 2, 3, 4, 5,
         6, 7, 8, 9, 10,
         11, 12, 13, 14, 15,
         16, 17, 18, 19, 20,
         21, 22, 23, 24, 25;
    Eigen::MatrixXi result = extractMiddleCols(A, 1);
    Eigen::MatrixXi expected(5, 3);
    expected << 2, 3, 4,
                7, 8, 9,
                12, 13, 14,
                17, 18, 19,
                22, 23, 24;
    assert(result == expected);

    // Test 2: Start at column 0
    result = extractMiddleCols(A, 0);
    expected << 1, 2, 3,
                6, 7, 8,
                11, 12, 13,
                16, 17, 18,
                21, 22, 23;
    assert(result == expected);

    // Test 3: Start at last possible column (col = N-3)
    result = extractMiddleCols(A, 2);
    expected << 3, 4, 5,
                8, 9, 10,
                13, 14, 15,
                18, 19, 20,
                23, 24, 25;
    assert(result == expected);

    // Test 4: 4x4 matrix, start at col 1
    Eigen::MatrixXi B(4, 4);
    B << 1, 2, 3, 4,
         5, 6, 7, 8,
         9, 10, 11, 12,
         13, 14, 15, 16;
    result = extractMiddleCols(B, 1);
    expected.resize(4, 3);
    expected << 2, 3, 4,
                6, 7, 8,
                10, 11, 12,
                14, 15, 16;
    assert(result == expected);

    // Test 5: Negative numbers
    Eigen::MatrixXi C(4, 4);
    C << -1, -2, -3, -4,
         -5, -6, -7, -8,
         -9, -10, -11, -12,
         -13, -14, -15, -16;
    result = extractMiddleCols(C, 0);
    expected << -1, -2, -3,
                -5, -6, -7,
                -9, -10, -11,
                -13, -14, -15;
    assert(result == expected);

    // All tests passed
    return 0;
}

// The task focuses on Eigen’s block operations. The solution uses the `middleCols` method, which extracts a set of consecutive columns from a matrix. For a matrix `A` of size `N × N`, calling `A.middleCols(col, 3)` returns a block view of size `N × 3` starting at column `col` and spanning 3 columns. Since we need to return a new independent matrix (not a view that depends on the original), we can copy the block into a new `MatrixXi` by assigning it to the return value. The main algorithm is trivial: validate that the matrix has at least 4 columns (though the problem guarantees this) and that `col` is within `[0, N-3]`, then return the block. Edge cases: if the matrix is empty or has fewer than 4 columns, we could assert or return an empty matrix; but per task, we assume valid input. Time complexity is O(N × 3) = O(N) because copying 3 columns from an N-row matrix takes linear time in N. Space complexity is O(N) for the returned matrix, plus constant auxiliary space.
