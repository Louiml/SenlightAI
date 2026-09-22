/*
Write a C++ function named `addScaledColumnToVector` that takes a matrix represented as a `std::vector<std::vector<double>>` (with all rows having the same length), a column index `col`, a scalar multiplier `scalar`, and a vector `result` (also a `std::vector<double>`) whose size equals the number of rows of the matrix. The function must compute `result[i] += scalar * matrix[i][col]` for all valid row indices `i`. The function must handle empty matrices (zero rows) gracefully by doing nothing, and must also handle the edge case where `col` is equal to the number of columns minus one (the last column). The function must not modify the input matrix or the scalar, and must only modify the `result` vector. It must be `const`-correct, use descriptive variable names, and include necessary headers. Assume the input matrix is non-empty in terms of columns (i.e., each row has at least one element) but may have zero rows. The function should return `void`.
*/

#include <vector>

/**
 * Adds scalar * matrix[row][col] to result[row] for each row.
 * The matrix must have the same number of columns in every row.
 * If the matrix has zero rows, the function does nothing.
 *
 * @param matrix   Read-only matrix (vector of vector of doubles).
 * @param col      Column index to use from each row.
 * @param scalar   Scalar multiplier.
 * @param result   Vector to be updated in-place; must have size equal to matrix.size().
 */
void addScaledColumnToVector(const std::vector<std::vector<double>>& matrix,
                             size_t col,
                             double scalar,
                             std::vector<double>& result)
{
    const size_t numRows = matrix.size();
    for (size_t i = 0; i < numRows; ++i) {
        // Access the (i, col) element. Assume col is valid.
        result[i] += scalar * matrix[i][col];
    }
}

#include <cassert>
#include <vector>

// Declaration of the solution function (placed here for the test).
void addScaledColumnToVector(const std::vector<std::vector<double>>& matrix,
                             size_t col,
                             double scalar,
                             std::vector<double>& result);

int main() {
    // Basic test with a 3x3 matrix, adding column 1 scaled by 2.
    {
        std::vector<std::vector<double>> matrix = {
            {1.0, 2.0, 3.0},
            {4.0, 5.0, 6.0},
            {7.0, 8.0, 9.0}
        };
        std::vector<double> result = {0.0, 0.0, 0.0};
        addScaledColumnToVector(matrix, 1, 2.0, result);
        assert(result[0] == 4.0);
        assert(result[1] == 10.0);
        assert(result[2] == 16.0);
    }

    // Test with last column and scalar 0.5.
    {
        std::vector<std::vector<double>> matrix = {
            {1.0, 2.0},
            {3.0, 4.0}
        };
        std::vector<double> result = {10.0, 20.0};
        addScaledColumnToVector(matrix, 1, 0.5, result);
        assert(result[0] == 11.0);
        assert(result[1] == 22.0);
    }

    // Edge case: empty matrix (zero rows) should not crash or modify.
    {
        std::vector<std::vector<double>> matrix;
        std::vector<double> result;  // empty
        addScaledColumnToVector(matrix, 0, 3.0, result);
        assert(result.empty());
    }

    // Edge case: single row, single column.
    {
        std::vector<std::vector<double>> matrix = {{5.0}};
        std::vector<double> result = {1.0};
        addScaledColumnToVector(matrix, 0, -2.0, result);
        assert(result[0] == -9.0);
    }

    // Test with negative scalar and initial nonzero result.
    {
        std::vector<std::vector<double>> matrix = {
            {1.0, 2.0},
            {3.0, 4.0},
            {5.0, 6.0}
        };
        std::vector<double> result = {100.0, 200.0, 300.0};
        addScaledColumnToVector(matrix, 0, -1.0, result);
        assert(result[0] == 99.0);
        assert(result[1] == 197.0);
        assert(result[2] == 295.0);
    }

    return 0;
}

// The solution is straightforward: iterate over each row index `i` from 0 to `matrix.size()-1`, and for each row, access the element at `matrix[i][col]`, multiply it by `scalar`, and add the product to `result[i]`. The main edge case is when the matrix has zero rows, in which case the loop does nothing, so no error occurs. Another edge case is a `col` index that is out of bounds – the task specification implies valid input, but for robustness, we can optionally assert or assume the column index is valid. The algorithm runs in O(R) time where R is the number of rows, and uses O(1) extra space beyond the input vectors. No special handling is needed for the last column because the loop iterates over all rows regardless of column position. The function should be declared as `void addScaledColumnToVector(const std::vector<std::vector<double>>& matrix, size_t col, double scalar, std::vector<double>& result)` to respect const correctness for the matrix and scalar, and pass result by non-const reference for modification.
