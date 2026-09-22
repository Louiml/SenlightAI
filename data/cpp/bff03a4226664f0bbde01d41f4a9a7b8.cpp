Write a C++ function named `transformMatrixColumns` that accepts a square `Eigen::MatrixXf` by const reference and an integer column index `targetCol` (0-based). The function must return a new `Eigen::MatrixXf` where the target column has been replaced by the sum of the original target column and 3 times the value of the first column (column index 0) of the original matrix. All other columns remain unchanged. The function must not modify the input matrix. You may assume the matrix is non-empty (at least 1 row and 1 column) and that `targetCol` is a valid column index (0 ≤ targetCol < number of columns). For example, if the input matrix is `[[1,2,3],[4,5,6],[7,8,9]]` and `targetCol = 2`, the result should be `[[1,2,6],[4,5,18],[7,8,30]]` because each element in column 2 gets 3 times the corresponding element in column 0 added (e.g., 3 + 3*1 = 6, 6 + 3*4 = 18, 9 + 3*7 = 30).
#include <Eigen/Dense>
#include <cassert>

// Declaration of the function to test (assumed to be defined above)
Eigen::MatrixXf transformMatrixColumns(const Eigen::MatrixXf& mat, int targetCol);

int main() {
    // Example from the problem statement
    Eigen::MatrixXf m(3,3);
    m << 1,2,3,
         4,5,6,
         7,8,9;
    Eigen::MatrixXf result = transformMatrixColumns(m, 2);
    // Expected: column 2 becomes [3+3*1, 6+3*4, 9+3*7] = [6,18,30]
    assert(result(0,0) == 1);
    assert(result(0,1) == 2);
    assert(result(0,2) == 6);
    assert(result(1,0) == 4);
    assert(result(1,1) == 5);
    assert(result(1,2) == 18);
    assert(result(2,0) == 7);
    assert(result(2,1) == 8);
    assert(result(2,2) == 30);
    // Ensure the original matrix is unchanged
    assert(m(0,2) == 3);
    assert(m(2,2) == 9);

    // Test targetCol = 0 (modifying first column using itself)
    Eigen::MatrixXf m2(2,2);
    m2 << 1, 5,
          2, 6;
    Eigen::MatrixXf result2 = transformMatrixColumns(m2, 0);
    // Expected: column 0 becomes [1+3*1, 2+3*2] = [4, 8]
    assert(result2(0,0) == 4);
    assert(result2(0,1) == 5);
    assert(result2(1,0) == 8);
    assert(result2(1,1) == 6);

    // Test 1x1 matrix
    Eigen::MatrixXf m3(1,1);
    m3 << 10;
    Eigen::MatrixXf result3 = transformMatrixColumns(m3, 0);
    assert(result3(0,0) == 40); // 10 + 3*10 = 40

    // Test a matrix with negative values
    Eigen::MatrixXf m4(2,3);
    m4 << -1, 2, -3,
           4, -5, 6;
    Eigen::MatrixXf result4 = transformMatrixColumns(m4, 2);
    // Expected: column 2 becomes [-3 + 3*(-1), 6 + 3*4] = [-6, 18]
    assert(result4(0,2) == -6);
    assert(result4(1,2) == 18);
    // Check first column remains -1, 4
    assert(result4(0,0) == -1);
    assert(result4(1,0) == 4);

    // Test preserving other columns for a larger matrix
    Eigen::MatrixXf m5 = Eigen::MatrixXf::Zero(3,3);
    m5.col(0) << 2, 3, 4;
    m5.col(1) << 10, 20, 30;
    m5.col(2) << 5, 6, 7;
    Eigen::MatrixXf result5 = transformMatrixColumns(m5, 1);
    // Column 1 becomes [10+3*2, 20+3*3, 30+3*4] = [16, 29, 42]
    assert(result5(0,1) == 16);
    assert(result5(1,1) == 29);
    assert(result5(2,1) == 42);
    // Column 0 and 2 unchanged
    assert(result5(0,0) == 2);
    assert(result5(2,0) == 4);
    assert(result5(0,2) == 5);
    assert(result5(2,2) == 7);

    return 0;
}
#include <Eigen/Dense>

// Return a copy of 'mat' where the column at index 'targetCol' is replaced by
// the original column plus 3 times the original first column (column 0).
// The input matrix is not modified.
Eigen::MatrixXf transformMatrixColumns(const Eigen::MatrixXf& mat, int targetCol) {
    // Create a copy of the input matrix
    Eigen::MatrixXf result = mat;
    // Modify the target column in the copy using the original matrix's first column
    result.col(targetCol) += 3.0f * mat.col(0);
    return result;
}
// The problem is straightforward: we need to create a copy of the input matrix and modify only the specified column. The main algorithm: 
// 1. Copy the input matrix into a new local `Eigen::MatrixXf` (since `Eigen::MatrixXf` supports copy construction).
// 2. Using Eigen’s block operations, compute the expression `3 * original.col(0)` and add it to `result.col(targetCol)`. Because we are working on a copy, the original remains untouched.
// 3. Return the result by value.
//
// Edge cases: 
// - If `targetCol == 0`, we are modifying the first column using itself. Since we use the expression `3 * original.col(0)`, and original is a const reference, the expression is evaluated from the original, not from the result, so the result will be `original.col(0) + 3*original.col(0) = 4*original.col(0)` — that is correct.
// - The matrix could be 1x1, then the only column is both first and target; result is `m(0,0) + 3*m(0,0) = 4*m(0,0)`.
// - Input is non-empty, so no empty matrix handling needed.
// - Floating-point precision: Eigen uses float by default for MatrixXf; adding and multiplying floats is fine but may introduce tiny rounding errors; for test purposes we compare with tolerance.
//
// Time complexity: Copying an \(n \times n\) matrix takes \(O(n^2)\) time, and the column operation takes \(O(n)\) time (since each column has n rows). So overall \(O(n^2)\). Space complexity: We allocate one new matrix, so \(O(n^2)\) auxiliary space.
