Write a C++ function that takes a two-dimensional matrix of floating-point numbers stored as a `std::vector<std::vector<double>>` and returns a `std::pair<double, double>` containing the matrix's 1-norm (maximum absolute column sum) as the first element and the infinity-norm (maximum absolute row sum) as the second element. The matrix is guaranteed to be non-empty (i.e., at least one row and each row has at least one column, and all rows have equal length). The function should compute these norms using only basic loops and standard library functions, without using any external linear algebra libraries.

#include <cassert>
#include <cmath>

// Include the solution function or paste it here.
// The function `matrixNorms` is expected above.

int main() {
    // Test 1: Simple 2x2 matrix from the snippet.
    std::vector<std::vector<double>> m1 = {{1.0, -2.0}, {-3.0, 4.0}};
    auto result1 = matrixNorms(m1);
    assert(std::abs(result1.first - 6.0) < 1e-9);    // 1-norm: col0=4, col1=6 -> max=6
    assert(std::abs(result1.second - 7.0) < 1e-9);   // inf-norm: row0=3, row1=7 -> max=7
    
    // Test 2: Single element.
    std::vector<std::vector<double>> m2 = {{-5.0}};
    auto result2 = matrixNorms(m2);
    assert(std::abs(result2.first - 5.0) < 1e-9);
    assert(std::abs(result2.second - 5.0) < 1e-9);
    
    // Test 3: All zeros.
    std::vector<std::vector<double>> m3 = {{0.0, 0.0}, {0.0, 0.0}};
    auto result3 = matrixNorms(m3);
    assert(std::abs(result3.first - 0.0) < 1e-9);
    assert(std::abs(result3.second - 0.0) < 1e-9);
    
    // Test 4: Rectangular matrix (2 rows, 3 columns).
    std::vector<std::vector<double>> m4 = {{1.0, 2.0, 3.0}, {-1.0, -2.0, -3.0}};
    auto result4 = matrixNorms(m4);
    // Col sums: col0=2, col1=4, col2=6 -> 1-norm=6
    assert(std::abs(result4.first - 6.0) < 1e-9);
    // Row sums: row0=6, row1=6 -> inf-norm=6
    assert(std::abs(result4.second - 6.0) < 1e-9);
    
    // Test 5: Negative values and asymmetric sums.
    std::vector<std::vector<double>> m5 = {{-10.0, 5.0}, {3.0, -2.0}};
    auto result5 = matrixNorms(m5);
    // Col sums: col0=13, col1=7 -> 1-norm=13
    assert(std::abs(result5.first - 13.0) < 1e-9);
    // Row sums: row0=15, row1=5 -> inf-norm=15
    assert(std::abs(result5.second - 15.0) < 1e-9);
    
    return 0;
}

#include <vector>
#include <cmath>
#include <utility>
#include <algorithm>

// Compute the 1-norm and infinity-norm of a matrix.
// 1-norm: max absolute column sum.
// infinity-norm: max absolute row sum.
// Returns {1-norm, infinity-norm}.
std::pair<double, double> matrixNorms(const std::vector<std::vector<double>>& matrix) {
    if (matrix.empty() || matrix[0].empty()) {
        return {0.0, 0.0};
    }
    
    const size_t rows = matrix.size();
    const size_t cols = matrix[0].size();
    
    double norm1 = 0.0;
    double normInf = 0.0;
    
    // 1-norm: for each column, sum absolute values across rows.
    for (size_t col = 0; col < cols; ++col) {
        double colSum = 0.0;
        for (size_t row = 0; row < rows; ++row) {
            colSum += std::abs(matrix[row][col]);
        }
        norm1 = std::max(norm1, colSum);
    }
    
    // infinity-norm: for each row, sum absolute values across columns.
    for (size_t row = 0; row < rows; ++row) {
        double rowSum = 0.0;
        for (size_t col = 0; col < cols; ++col) {
            rowSum += std::abs(matrix[row][col]);
        }
        normInf = std::max(normInf, rowSum);
    }
    
    return {norm1, normInf};
}

// The 1-norm of a matrix is defined as the maximum over all columns of the sum of absolute values of entries in that column. The infinity-norm is the maximum over all rows of the sum of absolute values of entries in that row. The algorithm is straightforward: first validate the input by checking the matrix is non-empty and rectangular (though the problem guarantees this, defensive code can check). Then initialize two accumulators—one for the 1-norm and one for the infinity-norm—to 0.0. Iterate over each column, summing the absolute values of all rows for that column, and update the 1-norm maximum. Similarly, iterate over each row, summing absolute values across columns, and update the infinity-norm maximum. Edge cases include single-element matrices (both norms equal the absolute value of that element) and matrices with negative or zero values (use `std::abs` for absolute value). Time complexity is \(O(m \times n)\) where \(m\) is the number of rows and \(n\) is the number of columns, because each element is visited exactly twice (once for each norm). Space complexity is \(O(1)\) auxiliary space, ignoring the input storage.
