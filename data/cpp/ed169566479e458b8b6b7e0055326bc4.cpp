// Given a 2D matrix represented as an `Eigen::MatrixXf` with at least one column, write a C++ function named `columnWiseMaxima` that returns an `Eigen::VectorXf` containing the maximum value of each column. The function must accept the matrix by const reference and preserve the order of columns. The input matrix may have any number of rows and columns (including zero rows), and values can be positive, negative, or zero, including floating-point denormals (though not NaN or infinity). If a column has zero rows, the maximum for that column is undefined; in that case, return `0.0f` for that column as a placeholder. The function must not modify the input matrix.
// The solution uses Eigen's built-in `colwise()` reduction combined with `maxCoeff()` to compute per-column maxima. The key algorithm is straightforward: `mat.colwise().maxCoeff()` returns a row vector of size equal to the number of columns, where each entry is the maximum of that column. However, to match the required return type `Eigen::VectorXf` (a column vector), we must transpose the result or use a more explicit loop. For clarity and to handle the edge case of zero rows explicitly, we can iterate over each column using a for-loop and call `mat.col(j).maxCoeff()`, but `maxCoeff()` on an empty column (0 rows) is undefined behavior; we guard against that with `mat.rows() == 0`. If the matrix has zero rows, all columns are empty, so we return a zero vector of size equal to the column count. If there is at least one row, every column has at least one element, so `maxCoeff()` is safe. Time complexity is O(rows × columns) because we scan every element once. Space complexity is O(columns) for the output vector (plus constant overhead). We apply `const` correctness by accepting `const Eigen::MatrixXf&` and returning by value.
#include <Eigen/Dense>

// Return a column vector containing the maximum value of each column.
// If the matrix has zero rows, each column is empty; we return 0.0f for each.
Eigen::VectorXf columnWiseMaxima(const Eigen::MatrixXf& mat) {
    const Eigen::Index numCols = mat.cols();
    Eigen::VectorXf result(numCols);
    
    if (mat.rows() == 0) {
        result.setZero();
        return result;
    }
    
    for (Eigen::Index j = 0; j < numCols; ++j) {
        result[j] = mat.col(j).maxCoeff();
    }
    return result;
}
#include <cassert>
#include <Eigen/Dense>

Eigen::VectorXf columnWiseMaxima(const Eigen::MatrixXf& mat);

int main() {
    // Basic test from the original snippet
    Eigen::MatrixXf mat1(2, 4);
    mat1 << 1, 2, 6, 9,
            3, 1, 7, 2;
    Eigen::VectorXf expected1(4);
    expected1 << 3, 2, 7, 9;
    assert(columnWiseMaxima(mat1) == expected1);

    // Single row
    Eigen::MatrixXf mat2(1, 3);
    mat2 << -5.5f, 0.0f, 3.25f;
    Eigen::VectorXf expected2(3);
    expected2 << -5.5f, 0.0f, 3.25f;
    assert(columnWiseMaxima(mat2) == expected2);

    // Single column, multiple rows
    Eigen::MatrixXf mat3(3, 1);
    mat3 << 2.0f, -1.0f, 2.0f;
    Eigen::VectorXf expected3(1);
    expected3 << 2.0f;
    assert(columnWiseMaxima(mat3) == expected3);

    // All negative
    Eigen::MatrixXf mat4(2, 2);
    mat4 << -1.0f, -4.0f,
            -3.0f, -2.0f;
    Eigen::VectorXf expected4(2);
    expected4 << -1.0f, -2.0f;
    assert(columnWiseMaxima(mat4) == expected4);

    // Zero rows: every column skipped, must return zeros
    Eigen::MatrixXf mat5(0, 4);
    Eigen::VectorXf expected5(4);
    expected5.setZero();
    assert(columnWiseMaxima(mat5) == expected5);

    // Zero columns: empty result
    Eigen::MatrixXf mat6(3, 0);
    assert(columnWiseMaxima(mat6).size() == 0);
}
