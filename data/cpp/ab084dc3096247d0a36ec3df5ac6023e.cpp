// Write a C++ function named `columnMaxima` that accepts a constant reference to an `Eigen::MatrixXf` and returns a `std::vector<float>` containing the maximum value of each column, in column order. The function must work for matrices of any dimensions, including empty matrices (0×0, 0×n, or m×0), returning an empty vector in those cases. Use only Eigen's dense matrix operations and standard library containers. The solution must not modify the input matrix.
// The solution iterates over each column index from 0 to `cols()-1` and extracts the column using `.col(i)`. For each column, the maximum is computed via `maxCoeff()`. For non‑empty columns, this returns the largest element. Edge cases: if the matrix has zero columns, the loop never executes and an empty vector is returned. If a column is empty (e.g., matrix with 0 rows but positive columns), `.col(i).maxCoeff()` would be undefined behavior — therefore, we must guard against matrices with zero rows by returning an empty vector early. Time complexity is O(m·n) because each element is visited once across all columns (each `maxCoeff()` on a column of length m takes O(m) time). Space complexity is O(n) for the output vector, plus no extra significant storage.
#include <vector>
#include <Eigen/Dense>

// Return the maximum element of each column of the input matrix.
// If the matrix has zero rows or zero columns, return an empty vector.
std::vector<float> columnMaxima(const Eigen::MatrixXf& mat) {
    std::vector<float> result;
    // Guard against matrices with zero rows (columns would be empty) or zero columns.
    if (mat.rows() == 0 || mat.cols() == 0) {
        return result;
    }
    result.reserve(mat.cols());
    for (int i = 0; i < mat.cols(); ++i) {
        result.push_back(mat.col(i).maxCoeff());
    }
    return result;
}
#include <cassert>
#include <vector>
#include <Eigen/Dense>

// Declare the function being tested (already defined in solution)
std::vector<float> columnMaxima(const Eigen::MatrixXf& mat);

int main() {
    // Example from the snippet
    Eigen::MatrixXf mat(2, 4);
    mat << 1, 2, 6, 9,
           3, 1, 7, 2;
    std::vector<float> expected = {3, 2, 7, 9};
    assert(columnMaxima(mat) == expected);

    // Single row
    Eigen::MatrixXf row(1, 3);
    row << -5, 0, 4;
    assert(columnMaxima(row) == (std::vector<float>{-5, 0, 4}));

    // Single column
    Eigen::MatrixXf col(3, 1);
    col << 10, -2, 7;
    assert(columnMaxima(col) == (std::vector<float>{10}));

    // Negative values
    Eigen::MatrixXf neg(2, 2);
    neg << -1, -4,
           -3, -2;
    assert(columnMaxima(neg) == (std::vector<float>{-1, -2}));

    // Zero rows → empty
    Eigen::MatrixXf zeroRows(0, 5);
    assert(columnMaxima(zeroRows).empty());

    // Zero columns → empty
    Eigen::MatrixXf zeroCols(5, 0);
    assert(columnMaxima(zeroCols).empty());

    // 1x1 matrix
    Eigen::MatrixXf one(1, 1);
    one << 42;
    assert(columnMaxima(one) == (std::vector<float>{42}));

    return 0;
}
