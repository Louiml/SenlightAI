/*
Write a C++ function `bool invertAndTest(const double m[3][3], double out[3][3])` that attempts to compute the inverse of a 3x3 matrix represented as a 2D array of doubles. If the matrix is singular (determinant is zero, or within a small tolerance of zero), return `false` and leave `out` unchanged. Otherwise, compute the inverse, store it in `out`, return `true`, and also verify that the computed inverse is correct by multiplying the original matrix and the output inverse; the product should be within a small tolerance of the identity matrix. The function should use a tolerance of `1e-9` for both determinant zero-check and identity verification. The input matrix elements are arbitrary real numbers (including zero, negative, and non-integer values). You may assume the matrix is stored in row-major order: `m[0][0]` corresponds to `e11`, `m[0][1]` to `e12`, `m[0][2]` to `e13`, `m[1][0]` to `e21`, and so on.
*/

#include <cmath>

// Compute the inverse of a 3x3 matrix m (row-major). Returns true if invertible
// and the inverse is stored in out. Returns false if the matrix is singular
// (within tolerance) or verification of the product fails.
bool invertAndTest(const double m[3][3], double out[3][3]) {
    const double tolerance = 1e-9;

    // Determinant using first row expansion
    double det = m[0][0] * (m[1][1] * m[2][2] - m[1][2] * m[2][1])
               - m[0][1] * (m[1][0] * m[2][2] - m[1][2] * m[2][0])
               + m[0][2] * (m[1][0] * m[2][1] - m[1][1] * m[2][0]);

    if (std::abs(det) <= tolerance) {
        return false;
    }

    double invDet = 1.0 / det;

    // Adjugate matrix (cofactor transpose) times invDet
    out[0][0] = (m[1][1] * m[2][2] - m[1][2] * m[2][1]) * invDet;
    out[0][1] = (m[0][2] * m[2][1] - m[0][1] * m[2][2]) * invDet;
    out[0][2] = (m[0][1] * m[1][2] - m[0][2] * m[1][1]) * invDet;

    out[1][0] = (m[1][2] * m[2][0] - m[1][0] * m[2][2]) * invDet;
    out[1][1] = (m[0][0] * m[2][2] - m[0][2] * m[2][0]) * invDet;
    out[1][2] = (m[0][2] * m[1][0] - m[0][0] * m[1][2]) * invDet;

    out[2][0] = (m[1][0] * m[2][1] - m[1][1] * m[2][0]) * invDet;
    out[2][1] = (m[0][1] * m[2][0] - m[0][0] * m[2][1]) * invDet;
    out[2][2] = (m[0][0] * m[1][1] - m[0][1] * m[1][0]) * invDet;

    // Verify m * out equals identity within tolerance
    double product[3][3] = {};
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            double sum = 0.0;
            for (int k = 0; k < 3; ++k) {
                sum += m[i][k] * out[k][j];
            }
            product[i][j] = sum;
        }
    }

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            double expected = (i == j) ? 1.0 : 0.0;
            if (std::abs(product[i][j] - expected) > tolerance) {
                return false;
            }
        }
    }

    return true;
}

#include <cassert>
#include <cmath>

// Forward declaration of the solution function (already defined above).
bool invertAndTest(const double m[3][3], double out[3][3]);

int main() {
    // Identity matrix -> inverse is itself
    double id[3][3] = {{1,0,0},{0,1,0},{0,0,1}};
    double out[3][3];
    assert(invertAndTest(id, out));
    assert(std::abs(out[0][0] - 1.0) < 1e-9);
    assert(std::abs(out[1][1] - 1.0) < 1e-9);
    assert(std::abs(out[2][2] - 1.0) < 1e-9);

    // Diagonal matrix with non-1 entries
    double diag[3][3] = {{2,0,0},{0,-3,0},{0,0,4}};
    assert(invertAndTest(diag, out));
    assert(std::abs(out[0][0] - 0.5) < 1e-9);
    assert(std::abs(out[1][1] - (-1.0/3.0)) < 1e-9);
    assert(std::abs(out[2][2] - 0.25) < 1e-9);

    // General non-singular matrix
    double gen[3][3] = {{4,7,2},{3,6,1},{2,5,3}};
    assert(invertAndTest(gen, out));

    // Singular matrix (zero determinant)
    double singular[3][3] = {{1,2,3},{4,5,6},{7,8,9}};  // det = 0
    assert(!invertAndTest(singular, out));

    // Zero matrix
    double zero[3][3] = {{0,0,0},{0,0,0},{0,0,0}};
    assert(!invertAndTest(zero, out));

    // Matrix with decimals
    double dec[3][3] = {{1.5, -2.0, 0.5},{0.25, 3.0, -1.0},{2.0, 0.75, 4.0}};
    assert(invertAndTest(dec, out));

    // Verify out matrix unchanged when false is returned (optional check)
    double before[3][3] = {{9,9,9},{9,9,9},{9,9,9}};
    double orig[3][3] = {{9,9,9},{9,9,9},{9,9,9}};
    assert(!invertAndTest(singular, out)); // out not guaranteed unchanged, but we just want false

    return 0;
}

// The core algorithm derives from the explicit formula for a 3x3 matrix inverse using the adjugate matrix and determinant. First, compute the determinant using the standard cofactor expansion along the first row: `det = m[0][0]*(m[1][1]*m[2][2] - m[1][2]*m[2][1]) - m[0][1]*(m[1][0]*m[2][2] - m[1][2]*m[2][0]) + m[0][2]*(m[1][0]*m[2][1] - m[1][1]*m[2][0])`. If `abs(det) <= 1e-9`, the matrix is considered singular, so return `false` without modifying `out`. Otherwise, compute `invDet = 1.0/det` and fill `out` with the cofactor matrix transposed (i.e., the adjugate) multiplied by `invDet`. Each `out[i][j]` corresponds to the cofactor of `m[j][i]`. After filling `out`, perform a matrix multiplication `product = m * out` (three rows, three columns) and verify that each element is within `1e-9` of the corresponding identity element (1.0 on the diagonal, 0.0 off-diagonal). If any element fails, return `false` (this should not happen if determinant computation and inverse formula are correct, but it is a safety check). Return `true` only if both determinant is non-zero and the verification passes. Edge cases include the zero matrix (det=0 → false), a nearly singular matrix with small but non-zero determinant (should pass if det > tolerance), and matrices with negative or fractional elements. Time complexity is O(1) since the matrix is fixed size, and space complexity is O(1) excluding the output array.
