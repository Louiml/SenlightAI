// Write a C++ function `std::vector<double> multiplyMatrixVector(const std::vector<std::vector<double>>& matrix, const std::vector<double>& vec)` that takes a 2D vector representing a matrix and a 1D vector representing a column vector, and returns the resulting vector after matrix-vector multiplication. The matrix has dimensions `rows x cols` and the vector has size `cols`. Validate that the number of columns in the matrix matches the vector size; if not, throw `std::invalid_argument`. The function must handle empty matrices (0 rows) and empty vectors (size 0) gracefully by returning an empty vector when appropriate. All arithmetic is real-valued doubles. Do not modify the inputs.
#include <cassert>
#include <cmath>
#include <vector>

// Include the solution function here (or assume it's above).
// For completeness, this main tests the function.

int main() {
    // Basic 2x3 matrix times 3-vector.
    std::vector<std::vector<double>> m1 = {{1, 2, 3}, {4, 5, 6}};
    std::vector<double> v1 = {1, 1, 1};
    auto r1 = multiplyMatrixVector(m1, v1);
    assert(r1.size() == 2);
    assert(std::fabs(r1[0] - 6.0) < 1e-9);
    assert(std::fabs(r1[1] - 15.0) < 1e-9);

    // 1x1 matrix with a scalar vector.
    std::vector<std::vector<double>> m2 = {{2.5}};
    std::vector<double> v2 = {4.0};
    auto r2 = multiplyMatrixVector(m2, v2);
    assert(r2.size() == 1);
    assert(std::fabs(r2[0] - 10.0) < 1e-9);

    // 3x1 matrix (column vector) times 1-vector.
    std::vector<std::vector<double>> m3 = {{1.0}, {2.0}, {3.0}};
    std::vector<double> v3 = {5.0};
    auto r3 = multiplyMatrixVector(m3, v3);
    assert(r3.size() == 3);
    assert(std::fabs(r3[0] - 5.0) < 1e-9);
    assert(std::fabs(r3[1] - 10.0) < 1e-9);
    assert(std::fabs(r3[2] - 15.0) < 1e-9);

    // Identity pattern with zeros.
    std::vector<std::vector<double>> m4 = {{0, 0}, {0, 0}};
    std::vector<double> v4 = {3.0, -1.0};
    auto r4 = multiplyMatrixVector(m4, v4);
    assert(r4.size() == 2);
    assert(std::fabs(r4[0]) < 1e-9);
    assert(std::fabs(r4[1]) < 1e-9);

    // Negative values and non-trivial result.
    std::vector<std::vector<double>> m5 = {{-1, 2}, {3, -4}};
    std::vector<double> v5 = {2, -3};
    auto r5 = multiplyMatrixVector(m5, v5);
    assert(std::fabs(r5[0] - (-8.0)) < 1e-9); // -1*2 + 2*(-3) = -2 -6 = -8
    assert(std::fabs(r5[1] - 18.0) < 1e-9);   // 3*2 + (-4)*(-3) = 6 + 12 = 18

    // Empty matrix and empty vector.
    std::vector<std::vector<double>> m6 = {};
    std::vector<double> v6 = {};
    auto r6 = multiplyMatrixVector(m6, v6);
    assert(r6.empty());

    // Mismatched dimensions should throw.
    bool threw = false;
    try {
        std::vector<std::vector<double>> m7 = {{1, 2}, {3, 4}};
        std::vector<double> v7 = {1, 2, 3};
        multiplyMatrixVector(m7, v7);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);

    // Empty matrix with non-empty vector should throw.
    threw = false;
    try {
        std::vector<std::vector<double>> m8 = {};
        std::vector<double> v8 = {1.0};
        multiplyMatrixVector(m8, v8);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);

    return 0;
}
#include <vector>
#include <stdexcept>

// Multiply a matrix (rows x cols) by a column vector (size cols).
// Returns the resulting column vector of size rows.
// Throws std::invalid_argument if matrix columns do not match vector size.
std::vector<double> multiplyMatrixVector(
    const std::vector<std::vector<double>>& matrix, 
    const std::vector<double>& vec) 
{
    const std::size_t rows = matrix.size();
    const std::size_t cols = vec.size();

    // Handle empty matrix case: valid only if vector is also empty.
    if (rows == 0) {
        if (cols != 0) {
            throw std::invalid_argument("Empty matrix requires empty vector");
        }
        return {};
    }

    // Validate each row's size matches vector size.
    for (std::size_t i = 0; i < rows; ++i) {
        if (matrix[i].size() != cols) {
            throw std::invalid_argument("Matrix column count does not match vector size");
        }
    }

    // Perform multiplication.
    std::vector<double> result(rows, 0.0);
    for (std::size_t i = 0; i < rows; ++i) {
        double sum = 0.0;
        for (std::size_t j = 0; j < cols; ++j) {
            sum += matrix[i][j] * vec[j];
        }
        result[i] = sum;
    }
    return result;
}
// The core algorithm is straightforward: the result vector must have length equal to the number of rows in the matrix. For each row `i`, compute the dot product of that row with the input vector: sum over `j` of `matrix[i][j] * vec[j]`. Edge cases: (1) If the matrix has 0 rows, the result is empty regardless of vector size, but validation still applies—if vector size is not 0, this is a mismatch (cols unknown per row), but we can treat it as mismatch if vector is non-empty and matrix has 0 rows? Actually, if matrix has 0 rows and 0 columns (empty matrix), a non-empty vector is invalid (cols mismatch vs vector size). We define validation: check each row's size, ensure all rows have same size, and ensure that size equals `vec.size()`. If matrix is empty (`rows == 0`), we check if `vec.size() == 0`? For simplicity, we require: if matrix is empty, then vector must also be empty, else throw. If matrix is non-empty, verify each row size equals `vec.size()`. Time complexity is O(rows * cols) for the multiplication, with an additional O(rows) to validate row sizes (if we check each row). Space complexity is O(rows) for the output vector, not counting input storage.
