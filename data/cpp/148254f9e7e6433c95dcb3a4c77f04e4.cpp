// Write a C++ free function `matrixRowMaxima` that accepts a constant reference to an `Eigen::MatrixXf` and returns an `Eigen::VectorXf` containing the maximum value of each row of the input matrix. The function must preserve the row order (i.e., the i-th element of the result is the maximum of the i-th row of the input). The input matrix will have at least one row and one column, and may contain positive, negative, and zero values. The function should not modify the input matrix. Handle the general case where the number of rows may differ from the number of columns (including non-square and single-row/single-column matrices). Ensure the returned vector has the correct size (equal to the number of rows) and correctly computes row-wise maxima using Eigen's rowwise reduction with `maxCoeff()`. The solution must compile with Eigen 3 and C++11 or later.
The main algorithm leverages Eigen's built-in rowwise reduction. For a matrix `M` of size `rows × cols`, `M.rowwise().maxCoeff()` returns a `VectorXf` of size `rows` where each entry is the maximum of the corresponding row. This is a direct, efficient operation. Edge cases: (1) Single row: returns a 1-element vector with the max of that row. (2) Single column: each row has exactly one element, so the result equals the original column as a vector. (3) All negative or all positive values: `maxCoeff` correctly computes the maximum regardless of sign. (4) Duplicate maxima: returns the value, which is the same as any occurrence. No special handling is needed for these cases. Time complexity is O(rows × cols) because each element must be examined once to determine the row maximum, and Eigen's implementation does exactly this. Space complexity is O(rows) for the returned vector (Eigen may allocate temporary buffers internally, but we ignore those). The function should be marked `const`-correct (input is `const Eigen::MatrixXf&`), and the return type `Eigen::VectorXf` is appropriate. No input validation is required as the problem specifies a non-empty matrix.
#include <Eigen/Dense>

// Return a vector where each element is the maximum value of the corresponding row of the input matrix.
Eigen::VectorXf matrixRowMaxima(const Eigen::MatrixXf& mat) {
    return mat.rowwise().maxCoeff();
}
#include <Eigen/Dense>
#include <cassert>

int main() {
    // Test 1: 2x4 matrix from the original snippet
    Eigen::MatrixXf mat1(2, 4);
    mat1 << 1, 2, 6, 9,
            3, 1, 7, 2;
    Eigen::VectorXf result1 = matrixRowMaxima(mat1);
    assert(result1.size() == 2);
    assert(result1(0) == 9.0f);
    assert(result1(1) == 7.0f);

    // Test 2: Single row
    Eigen::MatrixXf mat2(1, 3);
    mat2 << -5, 0, 3;
    Eigen::VectorXf result2 = matrixRowMaxima(mat2);
    assert(result2.size() == 1);
    assert(result2(0) == 3.0f);

    // Test 3: Single column
    Eigen::MatrixXf mat3(3, 1);
    mat3 << 4, -1, 7;
    Eigen::VectorXf result3 = matrixRowMaxima(mat3);
    assert(result3.size() == 3);
    assert(result3(0) == 4.0f);
    assert(result3(1) == -1.0f);
    assert(result3(2) == 7.0f);

    // Test 4: All negative values
    Eigen::MatrixXf mat4(2, 2);
    mat4 << -3, -8,
            -2, -5;
    Eigen::VectorXf result4 = matrixRowMaxima(mat4);
    assert(result4.size() == 2);
    assert(result4(0) == -3.0f);
    assert(result4(1) == -2.0f);

    // Test 5: Duplicate maxima in a row
    Eigen::MatrixXf mat5(1, 4);
    mat5 << 2, 5, 5, 1;
    Eigen::VectorXf result5 = matrixRowMaxima(mat5);
    assert(result5.size() == 1);
    assert(result5(0) == 5.0f);

    // Test 6: Square matrix with zeros
    Eigen::MatrixXf mat6(3, 3);
    mat6 << 0, 1, 2,
            3, 0, -1,
            -4, -5, -6;
    Eigen::VectorXf result6 = matrixRowMaxima(mat6);
    assert(result6.size() == 3);
    assert(result6(0) == 2.0f);
    assert(result6(1) == 3.0f);
    assert(result6(2) == -4.0f);

    // Test 7: Non-square 3x2
    Eigen::MatrixXf mat7(3, 2);
    mat7 << 10, 20,
            30, 5,
            1, 0;
    Eigen::VectorXf result7 = matrixRowMaxima(mat7);
    assert(result7.size() == 3);
    assert(result7(0) == 20.0f);
    assert(result7(1) == 30.0f);
    assert(result7(2) == 1.0f);

    // Test 8: Ensure input is not modified (check original matrix)
    Eigen::MatrixXf mat8 = mat1;
    Eigen::VectorXf result8 = matrixRowMaxima(mat8);
    assert(mat8(0,0) == 1.0f && mat8(0,1) == 2.0f && mat8(0,2) == 6.0f && mat8(0,3) == 9.0f);
    assert(mat8(1,0) == 3.0f && mat8(1,1) == 1.0f && mat8(1,2) == 7.0f && mat8(1,3) == 2.0f);
    assert(result8.size() == 2);
}
