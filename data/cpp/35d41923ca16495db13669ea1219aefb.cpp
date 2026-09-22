// Write a C++ function that takes a matrix represented as a 2D `std::vector<std::vector<double>>` and returns a `std::vector<double>` containing the Frobenius norm (Euclidean norm), the 1-norm (maximum absolute column sum), and the infinity norm (maximum absolute row sum) of the matrix, in that order. The matrix dimensions can be any positive integers, and the values can be negative, zero, or positive. The function must not modify the input matrix, must handle empty matrices by returning `{0.0, 0.0, 0.0}` (although typical inputs are non-empty, this edge case should be covered), and must compute all norms using their mathematical definitions without relying on external libraries.
#include <cassert>
#include <cmath>
#include <vector>

// The solution function is assumed to be available above.

int main() {
    // Test 1: Example matrix from the snippet
    std::vector<std::vector<double>> m1 = {{1.0, -2.0}, {-3.0, 4.0}};
    auto n1 = matrixNorms(m1);
    assert(std::fabs(n1[0] - std::sqrt(30.0)) < 1e-9); // 1+4+9+16=30
    assert(std::fabs(n1[1] - 6.0) < 1e-9); // col sums: 4,6 -> max=6
    assert(std::fabs(n1[2] - 7.0) < 1e-9); // row sums: 3,7 -> max=7

    // Test 2: Single element matrix
    std::vector<std::vector<double>> m2 = {{-5.0}};
    auto n2 = matrixNorms(m2);
    assert(std::fabs(n2[0] - 5.0) < 1e-9);
    assert(std::fabs(n2[1] - 5.0) < 1e-9);
    assert(std::fabs(n2[2] - 5.0) < 1e-9);

    // Test 3: Rectangular matrix with zeros
    std::vector<std::vector<double>> m3 = {{0.0, 3.0, -4.0}, {-2.0, 0.0, 0.0}};
    auto n3 = matrixNorms(m3);
    // Frobenius: 0+9+16+4+0+0=29 -> sqrt(29)
    assert(std::fabs(n3[0] - std::sqrt(29.0)) < 1e-9);
    // Column sums: |0|+|-2|=2, |3|+|0|=3, |-4|+|0|=4 -> max=4
    assert(std::fabs(n3[1] - 4.0) < 1e-9);
    // Row sums: |0|+|3|+|-4|=7, |-2|+|0|+|0|=2 -> max=7
    assert(std::fabs(n3[2] - 7.0) < 1e-9);

    // Test 4: Empty matrix
    std::vector<std::vector<double>> m4;
    auto n4 = matrixNorms(m4);
    assert(n4[0] == 0.0 && n4[1] == 0.0 && n4[2] == 0.0);

    // Test 5: Matrix with all identical positive values
    std::vector<std::vector<double>> m5 = {{2.0, 2.0}, {2.0, 2.0}};
    auto n5 = matrixNorms(m5);
    assert(std::fabs(n5[0] - std::sqrt(16.0)) < 1e-9); // 4
    assert(std::fabs(n5[1] - 4.0) < 1e-9); // each col sum=4
    assert(std::fabs(n5[2] - 4.0) < 1e-9); // each row sum=4

    return 0;
}
#include <vector>
#include <cmath>
#include <algorithm>

// Compute Frobenius, 1-norm, and infinity norm of a matrix.
// Returns {frobenius, one_norm, infinity_norm}. Empty matrix -> {0,0,0}.
std::vector<double> matrixNorms(const std::vector<std::vector<double>>& matrix) {
    if (matrix.empty() || matrix[0].empty()) {
        return {0.0, 0.0, 0.0};
    }
    const size_t rows = matrix.size();
    const size_t cols = matrix[0].size();

    // Frobenius norm
    double frobenius = 0.0;
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            frobenius += matrix[i][j] * matrix[i][j];
        }
    }
    frobenius = std::sqrt(frobenius);

    // 1-norm: max absolute column sum
    double one_norm = 0.0;
    for (size_t j = 0; j < cols; ++j) {
        double col_sum = 0.0;
        for (size_t i = 0; i < rows; ++i) {
            col_sum += std::abs(matrix[i][j]);
        }
        one_norm = std::max(one_norm, col_sum);
    }

    // Infinity norm: max absolute row sum
    double infinity_norm = 0.0;
    for (size_t i = 0; i < rows; ++i) {
        double row_sum = 0.0;
        for (size_t j = 0; j < cols; ++j) {
            row_sum += std::abs(matrix[i][j]);
        }
        infinity_norm = std::max(infinity_norm, row_sum);
    }

    return {frobenius, one_norm, infinity_norm};
}
// The solution involves three separate passes over the matrix to compute each norm. First, the Frobenius norm: sum the squares of every element, then take the square root. This requires iterating through all rows and columns, accumulating the squared sum in a `double` variable (using `long double` internally for precision if values are large, but `double` is acceptable for typical inputs). Second, the 1-norm: for each column, sum the absolute values of all elements in that column, and take the maximum such column sum. This requires iterating column by column, which can be done by first iterating over columns, then over rows for that column (or by maintaining a vector of column sums and then taking the maximum). Third, the infinity norm: for each row, sum the absolute values of all elements, and take the maximum such row sum. Edge cases include an empty matrix (return zeros), a matrix with a single element (all norms equal the absolute value of that element), and negative values (must apply `std::abs`). Time complexity is \(O(R \times C)\) for each norm, totaling \(O(R \times C)\) since we do constant work per element across all norms. Space complexity is \(O(1)\) auxiliary if we compute column sums on the fly (e.g., using a vector of size C), or \(O(C)\) if storing column sums; we can avoid extra space by iterating with nested loops correctly.
