/*
Write a C++ function that takes a fixed-size 2x2 matrix of double-precision floating-point values (represented as a `std::array<std::array<double,2>,2>` or a simple struct with four doubles) and returns a new matrix that is the result of performing one Jacobi rotation to zero out the off-diagonal entry at position (0,1). Specifically, compute the Jacobi rotation matrix \( J \) such that \( J^T M J \) has its (0,1) entry equal to zero (or as close as numerical precision allows). The rotation should be applied symmetrically on the left and right as in the snippet: compute \( J' \) (adjoint, which for real matrices is just transpose), apply `M' = J' * M * J`, and return the resulting matrix. The function must handle the case where the off-diagonal element is already zero (in which case \( J \) is the identity). Do not use any external linear algebra libraries; implement the Jacobi rotation manually using the standard formula for symmetric matrices: \( \tau = (a_{11} - a_{00}) / (2 a_{01}) \), \( t = \text{sign}(\tau) / (|\tau| + \sqrt{1+\tau^2}) \), \( c = 1/\sqrt{1+t^2} \), \( s = c t \), and form \( J = [[c, s],[-s, c]] \). The input matrix is assumed symmetric (i.e., a01 == a10), and if not, take the average of the two off-diagonal entries for computation. Return the rotated matrix.
*/

#include <array>
#include <cmath>
#include <algorithm>

// A 2x2 matrix with double precision.
using Matrix2d = std::array<std::array<double, 2>, 2>;

// Perform one Jacobi rotation to zero out the (0,1) entry of a symmetric 2x2 matrix.
// Returns the rotated matrix J^T * M * J. If off-diagonal is already zero, returns input.
Matrix2d jacobiRotate(const Matrix2d& m) {
    // Ensure symmetry by averaging off-diagonal entries.
    double a = m[0][0];
    double d = m[1][1];
    double b = 0.5 * (m[0][1] + m[1][0]);

    // If off-diagonal is effectively zero, no rotation needed.
    if (std::abs(b) < 1e-12) {
        return m;
    }

    // Compute rotation parameters.
    double tau = (d - a) / (2.0 * b);
    double t = (tau >= 0.0 ? 1.0 : -1.0) / (std::abs(tau) + std::sqrt(1.0 + tau * tau));
    double c = 1.0 / std::sqrt(1.0 + t * t);
    double s = c * t;

    // Rotation matrix J = [[c, s], [-s, c]].
    // Compute J^T * M * J manually.
    // First compute J^T * M.
    // J^T = [[c, -s], [s, c]]
    // Let R = J^T * M.
    // R[0][0] = c*a + (-s)*b = c*a - s*b
    // R[0][1] = c*b + (-s)*d = c*b - s*d
    // R[1][0] = s*a + c*b
    // R[1][1] = s*b + c*d
    double r00 = c * a - s * b;
    double r01 = c * b - s * d;
    double r10 = s * a + c * b;
    double r11 = s * b + c * d;

    // Then compute (J^T * M) * J.
    // Result[0][0] = r00*c + r01*(-s) = r00*c - r01*s
    // Result[0][1] = r00*s + r01*c
    // Result[1][0] = r10*c + r11*(-s) = r10*c - r11*s
    // Result[1][1] = r10*s + r11*c
    Matrix2d result;
    result[0][0] = r00 * c - r01 * s;
    result[0][1] = r00 * s + r01 * c;
    result[1][0] = r10 * c - r11 * s;
    result[1][1] = r10 * s + r11 * c;

    return result;
}

#include <cassert>
#include <cmath>
#include <array>

using Matrix2d = std::array<std::array<double, 2>, 2>;

Matrix2d jacobiRotate(const Matrix2d& m); // declaration

bool isClose(double x, double y, double eps = 1e-9) {
    return std::abs(x - y) < eps;
}

bool isSymmetricAndZeroOffDiag(const Matrix2d& m) {
    return isClose(m[0][1], 0.0) && isClose(m[1][0], 0.0) && isClose(m[0][1], m[1][0]);
}

int main() {
    // Test 1: Diagonal matrix -> unchanged
    Matrix2d m1 = {{{3.0, 0.0}, {0.0, 5.0}}};
    Matrix2d r1 = jacobiRotate(m1);
    assert(isClose(r1[0][0], 3.0) && isClose(r1[1][1], 5.0));
    assert(isSymmetricAndZeroOffDiag(r1));

    // Test 2: Symmetric with off-diagonal
    Matrix2d m2 = {{{2.0, 1.0}, {1.0, 2.0}}};
    Matrix2d r2 = jacobiRotate(m2);
    assert(isSymmetricAndZeroOffDiag(r2));
    // Trace is preserved: sum of diagonal remains 4
    assert(isClose(r2[0][0] + r2[1][1], 4.0));

    // Test 3: Non-symmetric input (averages off-diagonal)
    Matrix2d m3 = {{{4.0, 0.0}, {2.0, 1.0}}};
    Matrix2d r3 = jacobiRotate(m3);
    // Should treat b = 1.0, so tau = (1-4)/(2*1) = -1.5, t = -1/(1.5+sqrt(3.25)) ≈ -0.3029
    assert(isSymmetricAndZeroOffDiag(r3));
    // Trace preserved
    assert(isClose(r3[0][0] + r3[1][1], 5.0));

    // Test 4: Negative off-diagonal
    Matrix2d m4 = {{{1.0, -2.0}, {-2.0, 1.0}}};
    Matrix2d r4 = jacobiRotate(m4);
    assert(isSymmetricAndZeroOffDiag(r4));
    assert(isClose(r4[0][0] + r4[1][1], 2.0));

    // Test 5: Already zero off-diagonal but asymmetric diagonal
    Matrix2d m5 = {{{7.0, 0.0}, {0.0, -3.0}}};
    Matrix2d r5 = jacobiRotate(m5);
    assert(r5 == m5); // exact copy

    // Test 6: Large off-diagonal (degenerate case)
    Matrix2d m6 = {{{1.0, 1000.0}, {1000.0, 1.0}}};
    Matrix2d r6 = jacobiRotate(m6);
    assert(isSymmetricAndZeroOffDiag(r6));
    assert(isClose(r6[0][0] + r6[1][1], 2.0));

    return 0;
}

// The core algorithm follows the classic Jacobi eigenvalue algorithm for a symmetric 2x2 matrix. Given a symmetric matrix \( M = \begin{bmatrix} a & b \\ b & d \end{bmatrix} \) (where \( b = m_{01} = m_{10} \)), we seek an orthogonal rotation \( J \) that diagonalizes \( M \). The rotation angle \( \theta \) satisfies \( \tan(2\theta) = 2b/(a-d) \). To avoid numerical issues when \( b \) is zero, check if \( b \) is exactly zero (or very small, say < 1e-12) and return the input unchanged. Otherwise, compute \( \tau = (d - a)/(2b) \) (note the snippet uses a00-a01 but the standard formula uses d-a; both lead to the same rotation up to sign). Then \( t = \text{sign}(\tau) / (|\tau| + \sqrt{1+\tau^2}) \), \( c = 1/\sqrt{1+t^2} \), \( s = c t \). The rotation matrix \( J \) is \( \begin{bmatrix} c & s \\ -s & c \end{bmatrix} \) (this is the orthogonal matrix that zeros out the (0,1) entry when applied as \( J^T M J \)). Then compute \( M' = J^T M J \). Since \( J \) is orthogonal, the result is symmetric and the off-diagonal becomes zero up to floating point error. Edge cases: if \( b \) is very close to zero, we might still get a large \( \tau \); but using the standard `sign` and the robust formula avoids overflow. If the matrix is not exactly symmetric, average the off-diagonal entries. Time complexity: O(1) as it's fixed-size. Space complexity: O(1) for local variables.
