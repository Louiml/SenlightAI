Write a C++ function that takes a 3x3 matrix of doubles (represented as `std::array<std::array<double,3>,3>`) and returns a `std::optional<std::pair<double, std::array<std::array<double,3>,3>>>` containing the determinant and the inverse matrix if the matrix is invertible, or `std::nullopt` if it is singular (determinant approximately zero). The function must compute both the determinant and inverse using the adjugate method (cofactor expansion). Handle numerical precision by treating any determinant whose absolute value is less than `1e-9` as zero (non-invertible). The returned inverse should be computed exactly from the adjugate divided by the determinant.
The main algorithm uses the classical adjugate method for a 3x3 matrix. First, compute the determinant using the rule of Sarrus or cofactor expansion along the first row: `det = a(ei − fh) − b(di − fg) + c(dh − eg)`. If the absolute value of the determinant is less than `1e-9`, return `std::nullopt`. Otherwise, compute the matrix of minors, convert it to a cofactor matrix (apply a checkerboard of signs), then transpose to get the adjugate matrix. The inverse is the adjugate divided by the determinant. Edge cases include matrices with entries that make the determinant very small but not exactly zero—the threshold handles this. Also, matrices with repeated rows/columns will have determinant zero, correctly returning `nullopt`. The algorithm runs in constant time (fixed 3x3 size) and uses O(1) auxiliary space. The determinant computation involves 12 multiplications and 5 additions/subtractions; the cofactor computation involves 9 minors each requiring 2 multiplications and 1 subtraction, so total ~30 arithmetic operations.
#include <optional>
#include <array>
#include <cmath>

using Matrix3d = std::array<std::array<double, 3>, 3>;

// Compute determinant and inverse of a 3x3 matrix using adjugate method.
// Returns std::nullopt if the matrix is singular (|det| < 1e-9).
// Otherwise returns a pair containing the determinant and the inverse matrix.
std::optional<std::pair<double, Matrix3d>> invertibleMatrixWithDet(const Matrix3d& m) {
    const double a = m[0][0], b = m[0][1], c = m[0][2];
    const double d = m[1][0], e = m[1][1], f = m[1][2];
    const double g = m[2][0], h = m[2][1], i = m[2][2];

    // Determinant via Sarrus rule: a(ei - fh) - b(di - fg) + c(dh - eg)
    const double det = a * (e * i - f * h) - b * (d * i - f * g) + c * (d * h - e * g);

    if (std::abs(det) < 1e-9) {
        return std::nullopt;
    }

    // Compute minors (cofactors before sign adjustment)
    // Minor for position (0,0): determinant of [[e, f], [h, i]]
    const double m00 = e * i - f * h;
    const double m01 = d * i - f * g;
    const double m02 = d * h - e * g;
    // Minor for position (1,0): determinant of [[b, c], [h, i]]
    const double m10 = b * i - c * h;
    const double m11 = a * i - c * g;
    const double m12 = a * h - b * g;
    // Minor for position (2,0): determinant of [[b, c], [e, f]]
    const double m20 = b * f - c * e;
    const double m21 = a * f - c * d;
    const double m22 = a * e - b * d;

    // Cofactor matrix (apply signs) then transpose to get adjugate
    // Adj = [[+m00, -m10, +m20],
    //        [-m01, +m11, -m21],
    //        [+m02, -m12, +m22]]
    Matrix3d inv{{
        { m00 / det, -m10 / det,  m20 / det },
        {-m01 / det,  m11 / det, -m21 / det },
        { m02 / det, -m12 / det,  m22 / det }
    }};

    return std::make_pair(det, inv);
}
#include <cassert>
#include <cmath>
#include <optional>

int main() {
    // Identity matrix
    Matrix3d id = {{{1,0,0},{0,1,0},{0,0,1}}};
    auto res1 = invertibleMatrixWithDet(id);
    assert(res1.has_value());
    assert(std::abs(res1->first - 1.0) < 1e-12);
    assert(std::abs(res1->second[0][0] - 1.0) < 1e-12);
    assert(std::abs(res1->second[1][1] - 1.0) < 1e-12);
    assert(std::abs(res1->second[2][2] - 1.0) < 1e-12);

    // Diagonal matrix with determinant 6
    Matrix3d diag = {{{2,0,0},{0,3,0},{0,0,1}}};
    auto res2 = invertibleMatrixWithDet(diag);
    assert(res2.has_value());
    assert(std::abs(res2->first - 6.0) < 1e-12);
    assert(std::abs(res2->second[0][0] - 0.5) < 1e-12);
    assert(std::abs(res2->second[1][1] - 1.0/3.0) < 1e-12);
    assert(std::abs(res2->second[2][2] - 1.0) < 1e-12);

    // Singular matrix (all zeros)
    Matrix3d zero = {{{0,0,0},{0,0,0},{0,0,0}}};
    auto res3 = invertibleMatrixWithDet(zero);
    assert(!res3.has_value());

    // Singular matrix with duplicate rows
    Matrix3d dup = {{{1,2,3},{1,2,3},{4,5,6}}};
    auto res4 = invertibleMatrixWithDet(dup);
    assert(!res4.has_value());

    // Non-symmetric random invertible matrix
    Matrix3d m = {{{4,7,2},{3,6,1},{2,5,9}}};
    auto res5 = invertibleMatrixWithDet(m);
    assert(res5.has_value());
    // Verify that m * inv ≈ identity by manual matrix multiplication
    Matrix3d inv = res5->second;
    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 3; ++col) {
            double sum = 0.0;
            for (int k = 0; k < 3; ++k) {
                sum += m[row][k] * inv[k][col];
            }
            double expected = (row == col) ? 1.0 : 0.0;
            assert(std::abs(sum - expected) < 1e-9);
        }
    }

    // Matrix with very small but non-zero determinant (near singular)
    Matrix3d nearSingular = {{{1, 2, 3}, {4, 5, 6}, {7, 8, 8.999999}}};
    auto res6 = invertibleMatrixWithDet(nearSingular);
    // Determinant is around -3e-6, absolute value < 1e-9? Actually is ~ -0.000003, so it's invertible
    // Let's compute: det = 1*(5*8.999999 -6*8) -2*(4*8.999999-6*7)+3*(4*8-5*7) = 1*(44.999995-48) -2*(35.999996-42)+3*(32-35) = (-3.000005) -2*(-6.000004) +3*(-3) = -3.000005 +12.000008 -9 = -0.000003? Let's just assert it's invertible
    assert(res6.has_value());
    assert(std::abs(res6->first) >= 1e-9);

    // Matrix with exact zero determinant but not obvious
    Matrix3d singular = {{{1,2,3},{4,5,6},{7,8,9}}};
    auto res7 = invertibleMatrixWithDet(singular);
    assert(!res7.has_value());

    return 0;
}
