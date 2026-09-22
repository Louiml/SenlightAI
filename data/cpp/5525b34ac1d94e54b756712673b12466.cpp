Write a C++ function named `solveLinearSystem3` that accepts two parameters: a 3x3 matrix `A` (as `std::array<std::array<double,3>,3>`) and a 3-element vector `b` (as `std::array<double,3>`). The function must solve the linear system \(A \mathbf{x} = \mathbf{b}\) using Gaussian elimination with partial pivoting. It should return the solution vector \(\mathbf{x}\) as a `std::array<double,3>`. If the matrix is singular (i.e., no unique solution exists), the function should throw a `std::invalid_argument` exception with the message "Matrix is singular". The implementation must be self-contained (no external linear algebra libraries) and handle floating-point values robustly, using a tolerance (e.g., 1e-12) to detect near-zero pivots. The free function must be `const`-correct and operate on copies of the input arrays.
// The core algorithm is Gaussian elimination with partial pivoting, applied to a 3x3 system for efficiency and numerical stability. The approach involves three main steps:
// 1. **Augmented matrix formation**: Combine the 3x3 coefficient matrix `A` and the right-hand side vector `b` into a single 3x4 augmented matrix (3 rows, 4 columns), where the last column holds the `b` values.
// 2. **Forward elimination with partial pivoting**: For each column `col` from 0 to 2, find the row `maxRow` (among rows `col` to 2) that has the largest absolute value in that column. If this maximum absolute value is less than a tolerance (e.g., 1e-12), the matrix is considered singular, and we throw `std::invalid_argument`. Otherwise, swap the current row `col` with `maxRow` (if different) to bring the largest pivot to the diagonal. Then, for every row `i` below `col`, compute the factor `factor = augmented[i][col] / augmented[col][col]` and subtract `factor` times the pivot row from row `i`, eliminating the column `col` entry in all rows below. This transforms the matrix into upper triangular form.
// 3. **Back substitution**: Starting from the last row (row 2) down to row 0, solve for each variable. For row `i`, the variable `x[i]` is computed as `(augmented[i][3] - sum_{j=i+1}^{2} augmented[i][j] * x[j]) / augmented[i][i]`. This yields the unique solution if the matrix is non-singular.
//
// **Edge cases**: 
// - The matrix may be singular (e.g., rows linearly dependent), detected during pivoting when no non-zero pivot is found.
// - Floating-point precision: use a tolerance for pivot magnitude; values below this are treated as zero.
// - The matrix entries and vector entries may be negative, zero, or very small; these are handled by partial pivoting.
//
// **Complexity**: For a fixed size 3x3, the time complexity is \(O(3^3) = O(27)\), effectively constant. The space complexity is \(O(1)\) extra space beyond the input copies (the augmented matrix is a fixed 3x4 array). The algorithm is deterministic and robust for 3x3 systems.
#include <array>
#include <cmath>
#include <stdexcept>

// Solve the 3x3 linear system A * x = b using Gaussian elimination with partial pivoting.
// Throws std::invalid_argument if the matrix is singular (no unique solution).
std::array<double, 3> solveLinearSystem3(
    const std::array<std::array<double, 3>, 3>& A,
    const std::array<double, 3>& b) {
    
    // Build augmented matrix [A | b] as a 3x4 array.
    std::array<std::array<double, 4>, 3> aug{};
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            aug[i][j] = A[i][j];
        }
        aug[i][3] = b[i];
    }

    const double tolerance = 1e-12;

    // Forward elimination with partial pivoting.
    for (int col = 0; col < 3; ++col) {
        // Find pivot row (largest absolute value in this column from col down).
        int maxRow = col;
        for (int row = col + 1; row < 3; ++row) {
            if (std::abs(aug[row][col]) > std::abs(aug[maxRow][col])) {
                maxRow = row;
            }
        }

        // Check for singular matrix.
        if (std::abs(aug[maxRow][col]) < tolerance) {
            throw std::invalid_argument("Matrix is singular");
        }

        // Swap current row with pivot row if needed.
        if (maxRow != col) {
            std::swap(aug[maxRow], aug[col]);
        }

        // Eliminate entries below the pivot.
        for (int row = col + 1; row < 3; ++row) {
            double factor = aug[row][col] / aug[col][col];
            for (int j = col; j < 4; ++j) {
                aug[row][j] -= factor * aug[col][j];
            }
        }
    }

    // Back substitution to find solution.
    std::array<double, 3> x{};
    for (int i = 2; i >= 0; --i) {
        double sum = aug[i][3];
        for (int j = i + 1; j < 3; ++j) {
            sum -= aug[i][j] * x[j];
        }
        x[i] = sum / aug[i][i];
    }

    return x;
}
#include <cassert>
#include <cmath>
#include <array>

// The solution function is assumed to be defined above.

int main() {
    // Test 1: Simple non-singular system
    std::array<std::array<double, 3>, 3> A1 = {{{1, 2, 3}, {4, 5, 6}, {7, 8, 10}}};
    std::array<double, 3> b1 = {3, 3, 4};
    auto x1 = solveLinearSystem3(A1, b1);
    assert(std::abs(x1[0] - 2.0) < 1e-9);
    assert(std::abs(x1[1] - 1.0) < 1e-9);
    assert(std::abs(x1[2] - (-1.0)) < 1e-9);

    // Test 2: Identity matrix
    std::array<std::array<double, 3>, 3> A2 = {{{1, 0, 0}, {0, 1, 0}, {0, 0, 1}}};
    std::array<double, 3> b2 = {5, -2, 7};
    auto x2 = solveLinearSystem3(A2, b2);
    assert(x2 == (std::array<double, 3>{5, -2, 7}));

    // Test 3: Diagonal matrix
    std::array<std::array<double, 3>, 3> A3 = {{{2, 0, 0}, {0, -3, 0}, {0, 0, 4}}};
    std::array<double, 3> b3 = {8, 9, -8};
    auto x3 = solveLinearSystem3(A3, b3);
    assert(std::abs(x3[0] - 4.0) < 1e-9);
    assert(std::abs(x3[1] - (-3.0)) < 1e-9);
    assert(std::abs(x3[2] - (-2.0)) < 1e-9);

    // Test 4: Singular matrix should throw
    std::array<std::array<double, 3>, 3> A4 = {{{1, 2, 3}, {2, 4, 6}, {1, 1, 1}}};
    std::array<double, 3> b4 = {1, 2, 3};
    bool threw = false;
    try {
        auto x4 = solveLinearSystem3(A4, b4);
        (void)x4;
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);

    // Test 5: System with a near-zero pivot that requires pivoting
    std::array<std::array<double, 3>, 3> A5 = {{{0, 1, 1}, {1, 1, 1}, {1, 0, 2}}};
    std::array<double, 3> b5 = {1, 2, 3};
    auto x5 = solveLinearSystem3(A5, b5);
    // Manually verify: solve gives x = (1, 0, 1)
    assert(std::abs(x5[0] - 1.0) < 1e-9);
    assert(std::abs(x5[1] - 0.0) < 1e-9);
    assert(std::abs(x5[2] - 1.0) < 1e-9);

    // Test 6: All-zero RHS (homogeneous) with non-singular A -> all zero solution
    std::array<std::array<double, 3>, 3> A6 = {{{1, 2, 3}, {0, 5, 6}, {0, 0, 7}}};
    std::array<double, 3> b6 = {0, 0, 0};
    auto x6 = solveLinearSystem3(A6, b6);
    assert(x6 == (std::array<double, 3>{0, 0, 0}));

    return 0;
}
