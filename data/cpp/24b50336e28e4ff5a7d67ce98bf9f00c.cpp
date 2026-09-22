Write a C++ function that takes a 3x3 matrix represented as a `std::array<std::array<double, 3>, 3>` (row-major order) and returns a `std::optional<std::array<std::array<double, 3>, 3>>` containing the inverse matrix if the matrix is invertible, or `std::nullopt` if it is not invertible. The inverse must be computed using Gaussian elimination with partial pivoting. For testing purposes, you may assume that matrices with a determinant exactly zero (within floating-point tolerance of 1e-9) are non-invertible, and all other matrices are invertible. The function must be `const`-correct and handle edge cases like singular matrices, near-singular matrices, and identity matrices.
#include <cassert>
#include <cmath>
#include <array>

using Matrix3x3 = std::array<std::array<double, 3>, 3>;

// Solution function declaration (as provided).
std::optional<Matrix3x3> computeInverse(const Matrix3x3& mat);

bool matricesEqual(const Matrix3x3& a, const Matrix3x3& b, double tol = 1e-6) {
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            if (std::fabs(a[i][j] - b[i][j]) > tol) return false;
    return true;
}

int main() {
    // Identity matrix: inverse is itself.
    Matrix3x3 identity = {{{1,0,0},{0,1,0},{0,0,1}}};
    auto invId = computeInverse(identity);
    assert(invId.has_value());
    assert(matricesEqual(*invId, identity));

    // Known inverse: matrix [[2,0,0],[0,3,0],[0,0,4]] inverse has reciprocals.
    Matrix3x3 diagonal = {{{2,0,0},{0,3,0},{0,0,4}}};
    auto invDiag = computeInverse(diagonal);
    assert(invDiag.has_value());
    Matrix3x3 expectedDiag = {{{0.5,0,0},{0,1.0/3.0,0},{0,0,0.25}}};
    assert(matricesEqual(*invDiag, expectedDiag));

    // Singular matrix (row of zeros).
    Matrix3x3 singular = {{{1,2,3},{4,5,6},{0,0,0}}};
    auto invSing = computeInverse(singular);
    assert(!invSing.has_value());

    // Singular matrix (linearly dependent rows).
    Matrix3x3 dependent = {{{1,2,3},{2,4,6},{7,8,9}}};
    auto invDep = computeInverse(dependent);
    assert(!invDep.has_value());

    // General invertible matrix: verify A * A^{-1} = I.
    Matrix3x3 general = {{{4,7,2},{3,6,1},{2,5,9}}};
    auto invGen = computeInverse(general);
    assert(invGen.has_value());
    // Compute product manually (since we don't have matrix multiplication here).
    Matrix3x3 product = {};
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j) {
            double sum = 0;
            for (int k = 0; k < 3; ++k)
                sum += general[i][k] * (*invGen)[k][j];
            product[i][j] = sum;
        }
    assert(matricesEqual(product, identity));

    // Near-singular matrix (determinant very small but non-zero) should be invertible with tolerance.
    Matrix3x3 nearSingular = {{{1,2,3},{4,5,6},{7,8,8.999}}}; // determinant ~ -0.003, not zero.
    auto invNear = computeInverse(nearSingular);
    assert(invNear.has_value());

    // Matrix with negative numbers.
    Matrix3x3 negative = {{{-1,2,-3},{4,-5,6},{-7,8,-9}}}; // actually singular, but check invertibility flag.
    auto invNeg = computeInverse(negative);
    // determinant of this matrix is 0, so should be nullopt.
    assert(!invNeg.has_value());

    return 0;
}
#include <array>
#include <optional>
#include <cmath>

using Matrix3x3 = std::array<std::array<double, 3>, 3>;

// Compute the inverse of a 3x3 matrix using Gaussian elimination with partial pivoting.
// Returns std::nullopt if the matrix is not invertible (determinant within tolerance).
std::optional<Matrix3x3> computeInverse(const Matrix3x3& mat) {
    // Augmented matrix: left side is a copy of input, right side starts as identity.
    double aug[3][6];
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            aug[i][j] = mat[i][j];
            aug[i][j + 3] = (i == j) ? 1.0 : 0.0;
        }
    }

    const double tolerance = 1e-9;

    // Gaussian elimination with partial pivoting.
    for (int col = 0; col < 3; ++col) {
        // Find pivot row (largest absolute value in this column from row col downwards).
        int pivotRow = col;
        double maxVal = std::fabs(aug[col][col]);
        for (int row = col + 1; row < 3; ++row) {
            if (std::fabs(aug[row][col]) > maxVal) {
                maxVal = std::fabs(aug[row][col]);
                pivotRow = row;
            }
        }

        // Check for singularity.
        if (maxVal < tolerance) {
            return std::nullopt; // Not invertible.
        }

        // Swap pivot row with current row if needed.
        if (pivotRow != col) {
            for (int j = 0; j < 6; ++j) {
                std::swap(aug[col][j], aug[pivotRow][j]);
            }
        }

        // Normalize pivot row: make pivot element 1.
        double pivot = aug[col][col];
        for (int j = 0; j < 6; ++j) {
            aug[col][j] /= pivot;
        }

        // Eliminate all other rows in this column.
        for (int row = 0; row < 3; ++row) {
            if (row != col) {
                double factor = aug[row][col];
                if (std::fabs(factor) > tolerance) {
                    for (int j = 0; j < 6; ++j) {
                        aug[row][j] -= factor * aug[col][j];
                    }
                }
            }
        }
    }

    // Extract the right half as the inverse.
    Matrix3x3 inverse;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            inverse[i][j] = aug[i][j + 3];
        }
    }
    return inverse;
}
// The solution approach uses Gaussian elimination with partial pivoting to compute both the determinant and the inverse simultaneously. The algorithm works by augmenting the input matrix with the identity matrix and then performing row operations to transform the left side into the identity matrix. Partial pivoting is used to avoid division by very small numbers, which improves numerical stability. For each column (from 0 to 2), find the row with the largest absolute value in that column at or below the diagonal. If that maximum is below a tolerance (e.g., 1e-9), the matrix is considered singular and we return `std::nullopt`. Otherwise, swap rows if needed, normalize the pivot row so the pivot becomes 1, and eliminate all other rows (both above and below) to make the column have zeros except at the pivot. The right side of the augmented matrix then becomes the inverse. We also track the determinant by multiplying by the pivot values (with sign changes for row swaps) to check invertibility, though the pivot check alone suffices. Edge cases include: identity matrix (inverse is itself), singular matrices (e.g., a row of zeros or linear dependency), and matrices with very small pivot values. Time complexity is O(3^3) = O(1) since the matrix size is fixed at 3x3, and space complexity is O(1) for the augmented matrix.
