Write a C++ function `invertMatrixNormalized` that takes a 3×3 matrix of integers (represented as `std::array<std::array<int,3>,3>` by value) and returns a 3×3 matrix of doubles (represented as `std::array<std::array<double,3>,3>`) that is the inverse of the input matrix. The function must handle matrices with a zero determinant by returning a zero matrix (all entries equal to 0.0). Use the algebraic cofactor method to compute the inverse. The function must be const-correct: take the input as a const reference and not modify it. Provide necessary headers and a concise comment explaining the method.

#include <cassert>
#include <cmath>

int main() {
    // Identity matrix inverse is itself
    std::array<std::array<int,3>,3> ident = {{ {{1,0,0}}, {{0,1,0}}, {{0,0,1}} }};
    auto inv = invertMatrixNormalized(ident);
    assert(std::fabs(inv[0][0] - 1.0) < 1e-9);
    assert(std::fabs(inv[1][1] - 1.0) < 1e-9);
    assert(std::fabs(inv[2][2] - 1.0) < 1e-9);
    assert(std::fabs(inv[0][1]) < 1e-9);

    // Simple diagonal matrix: diag(1,2,3) inverse = diag(1,1/2,1/3)
    std::array<std::array<int,3>,3> diag = {{ {{1,0,0}}, {{0,2,0}}, {{0,0,3}} }};
    inv = invertMatrixNormalized(diag);
    assert(std::fabs(inv[1][1] - 0.5) < 1e-9);
    assert(std::fabs(inv[2][2] - (1.0/3.0)) < 1e-9);

    // Singular matrix with zero determinant returns zero matrix
    std::array<std::array<int,3>,3> singular = {{ {{1,2,3}}, {{2,4,6}}, {{0,1,1}} }};
    inv = invertMatrixNormalized(singular);
    for (auto& row : inv) {
        for (double val : row) assert(val == 0.0);
    }

    // Known invertible matrix: [1 2 0; 0 1 0; 0 0 1] inverse is same
    std::array<std::array<int,3>,3> shear = {{ {{1,2,0}}, {{0,1,0}}, {{0,0,1}} }};
    inv = invertMatrixNormalized(shear);
    assert(std::fabs(inv[0][1] + 2.0) < 1e-9); // inverse has -2
    assert(std::fabs(inv[1][1] - 1.0) < 1e-9);

    // General matrix, check A * A_inv ≈ I
    std::array<std::array<int,3>,3> A = {{ {{2,0,1}}, {{0,3,0}}, {{1,0,4}} }};
    inv = invertMatrixNormalized(A);
    auto I = std::array<std::array<double,3>,3>{{ {{1,0,0}}, {{0,1,0}}, {{0,0,1}} }};
    // multiplication result
    double prod[3][3];
    for (int i=0;i<3;i++) for (int j=0;j<3;j++) {
        prod[i][j] = 0;
        for (int k=0;k<3;k++) prod[i][j] += A[i][k] * inv[k][j];
        assert(std::fabs(prod[i][j] - I[i][j]) < 1e-9);
    }
}

#include <array>
#include <cstddef>

// Compute the inverse of a 3x3 integer matrix. Returns a zero matrix if det == 0.
std::array<std::array<double, 3>, 3> invertMatrixNormalized(
    const std::array<std::array<int, 3>, 3>& a) {
    // Compute determinant
    double det = 
        a[0][0] * (a[1][1] * a[2][2] - a[2][1] * a[1][2])
      - a[0][1] * (a[1][0] * a[2][2] - a[2][0] * a[1][2])
      + a[0][2] * (a[1][0] * a[2][1] - a[2][0] * a[1][1]);

    std::array<std::array<double, 3>, 3> result;
    if (det == 0.0) {
        for (auto& row : result) row = {0.0, 0.0, 0.0};
        return result;
    }

    // Cofactor matrix
    double cof[3][3];
    cof[0][0] = a[1][1] * a[2][2] - a[2][1] * a[1][2];
    cof[0][1] = -(a[1][0] * a[2][2] - a[2][0] * a[1][2]);
    cof[0][2] = a[1][0] * a[2][1] - a[2][0] * a[1][1];

    cof[1][0] = -(a[0][1] * a[2][2] - a[2][1] * a[0][2]);
    cof[1][1] = a[0][0] * a[2][2] - a[2][0] * a[0][2];
    cof[1][2] = -(a[0][0] * a[2][1] - a[2][0] * a[0][1]);

    cof[2][0] = a[0][1] * a[1][2] - a[1][1] * a[0][2];
    cof[2][1] = -(a[0][0] * a[1][2] - a[1][0] * a[0][2]);
    cof[2][2] = a[0][0] * a[1][1] - a[1][0] * a[0][1];

    // Adjugate is transpose of cofactor matrix, then divide by det
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            result[i][j] = cof[j][i] / det;
        }
    }
    return result;
}

// The inverse of a 3×3 matrix A is computed using the formula A⁻¹ = (1/det(A)) * adj(A), where adj(A) is the transpose of the cofactor matrix. First, compute the determinant using the standard expansion: det = a(ei − fh) − b(di − fg) + c(dh − eg), where elements are indexed as a[0][0], a[0][1], a[0][2], a[1][0], etc. If the determinant is exactly zero (as a double), return a 3×3 matrix filled with 0.0. Otherwise, compute each cofactor C_ij, transpose the cofactor matrix to get the adjugate, and divide each element by the determinant. Edge cases: integer overflow in intermediate multiplications is possible but acceptable for typical small integers; the determinant must be computed as a double to avoid integer division; the function must handle a matrix with one or more zero rows/columns properly. Time complexity is O(1) because the matrix size is fixed at 3×3; space complexity is O(1) for the output.
