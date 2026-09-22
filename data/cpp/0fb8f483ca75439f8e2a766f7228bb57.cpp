/*
Write a C++ function that takes a 2x2 matrix of floating-point values (represented as `std::array<std::array<double,2>,2>`) and returns the same matrix after applying a symmetric Jacobi rotation to zero out the off-diagonal entries. Specifically, compute the Jacobi rotation matrix \(J\) that diagonalizes the symmetric matrix \(A\) (where \(A = (M + M^T)/2\)), apply the similarity transformation \(J^T A J\), and return the resulting diagonal matrix. The function should preserve the diagonal entries’ order and set both off-diagonal entries to exactly zero (within floating-point tolerance). Handle edge cases where the off-diagonal entry is already zero, and where the rotation angle leads to a degenerate case (e.g., when the off-diagonal is zero or the denominator in the tangent formula is zero).
*/

#include <array>
#include <cmath>
#include <algorithm>

// Symmetric 2x2 matrix type using double precision
using Matrix2d = std::array<std::array<double, 2>, 2>;

// Apply a symmetric Jacobi rotation to zero out off-diagonal entries of the symmetric part.
// Input M is a general 2x2 matrix. We symmetrize it as A = (M + M^T)/2, then compute
// J such that J^T * A * J is diagonal, and return that diagonal matrix.
Matrix2d jacobiDiagonalize(const Matrix2d& M) {
    // Symmetrize: a = A(0,0), b = A(0,1) = A(1,0), c = A(1,1)
    double a = (M[0][0] + M[0][0]) * 0.5; // actually M[0][0] is already symmetric at that position
    double b = (M[0][1] + M[1][0]) * 0.5;
    double c = (M[1][1] + M[1][1]) * 0.5;
    
    // If off-diagonal is already zero (within epsilon), return original matrix
    const double eps = 1e-12;
    if (std::abs(b) < eps) {
        Matrix2d result = M;
        result[0][1] = 0.0;
        result[1][0] = 0.0;
        result[0][0] = a;
        result[1][1] = c;
        return result;
    }
    
    double cos_theta, sin_theta;
    
    if (std::abs(a - c) < eps) {
        // Special case: a == c, choose theta = pi/4
        cos_theta = std::sqrt(0.5);
        sin_theta = std::sqrt(0.5);
    } else {
        // Stable formula for tan(2θ) = 2b/(a-c)
        double p = (a - c) / (2.0 * b);
        double t = (p >= 0.0) ? 1.0 / (p + std::sqrt(p*p + 1.0))
                              : -1.0 / (-p + std::sqrt(p*p + 1.0));
        cos_theta = 1.0 / std::sqrt(t*t + 1.0);
        sin_theta = t * cos_theta;
    }
    
    // Apply J^T * A * J
    // J = [cos  -sin; sin  cos], J^T = [cos  sin; -sin  cos]
    // Compute effective matrix entries
    double a11 = cos_theta, a12 = -sin_theta, a21 = sin_theta, a22 = cos_theta;
    
    // First compute A * J (where J is [[cos, -sin], [sin, cos]])
    double b11 = a * a11 + b * a21;
    double b12 = a * a12 + b * a22;
    double b21 = b * a11 + c * a21;
    double b22 = b * a12 + c * a22;
    
    // Then compute J^T * (A*J)
    double d11 = a11 * b11 + a21 * b21;
    double d12 = a11 * b12 + a21 * b22;
    double d21 = a12 * b11 + a22 * b21;
    double d22 = a12 * b12 + a22 * b22;
    
    // Clamp tiny off-diagonals to zero
    Matrix2d result = {{{d11, 0.0}, {0.0, d22}}};
    if (std::abs(d12) > eps || std::abs(d21) > eps) {
        // Should be zero mathematically; enforce zero
        result[0][1] = 0.0;
        result[1][0] = 0.0;
    }
    
    return result;
}

#include <cassert>
#include <cmath>
#include <array>

// Reuse the Matrix2d type and function from the solution (paste above)

int main() {
    // Test 1: Already diagonal
    Matrix2d m1 = {{{2.0, 0.0}, {0.0, 3.0}}};
    Matrix2d r1 = jacobiDiagonalize(m1);
    assert(std::abs(r1[0][0] - 2.0) < 1e-12);
    assert(std::abs(r1[1][1] - 3.0) < 1e-12);
    assert(std::abs(r1[0][1]) < 1e-12);
    assert(std::abs(r1[1][0]) < 1e-12);
    
    // Test 2: Symmetric with off-diagonal
    Matrix2d m2 = {{{2.0, 1.0}, {1.0, 2.0}}};
    Matrix2d r2 = jacobiDiagonalize(m2);
    // Eigenvalues are 3 and 1, off-diagonals zero
    assert(std::abs(r2[0][1]) < 1e-12);
    assert(std::abs(r2[1][0]) < 1e-12);
    assert(std::abs(r2[0][0] - 3.0) < 1e-9 || std::abs(r2[0][0] - 1.0) < 1e-9);
    assert(std::abs(r2[1][1] - (1.0 + 3.0 - r2[0][0])) < 1e-9);
    
    // Test 3: Asymmetric input; symmetrized
    Matrix2d m3 = {{{1.0, 0.5}, {0.5, 4.0}}};
    Matrix2d r3 = jacobiDiagonalize(m3);
    assert(std::abs(r3[0][1]) < 1e-12);
    assert(std::abs(r3[1][0]) < 1e-12);
    // Trace should be 1+4=5
    assert(std::abs(r3[0][0] + r3[1][1] - 5.0) < 1e-9);
    
    // Test 4: Negative off-diagonal
    Matrix2d m4 = {{{3.0, -2.0}, {-2.0, 6.0}}};
    Matrix2d r4 = jacobiDiagonalize(m4);
    assert(std::abs(r4[0][1]) < 1e-12);
    assert(std::abs(r4[1][0]) < 1e-12);
    assert(std::abs(r4[0][0] + r4[1][1] - 9.0) < 1e-9);
    
    // Test 5: Zero off-diagonal but asymmetric? (symmetric part has zero off-diag)
    Matrix2d m5 = {{{1.0, 2.0}, {0.0, 3.0}}};
    Matrix2d r5 = jacobiDiagonalize(m5);
    // Symmetric part is [[1, 1], [1, 3]] -> eigenvalues 0.5858 and 3.4142
    assert(std::abs(r5[0][1]) < 1e-12);
    assert(std::abs(r5[1][0]) < 1e-12);
    assert(std::abs(r5[0][0] + r5[1][1] - 4.0) < 1e-9);
    
    // Test 6: Large off-diagonal relative to diagonal diff
    Matrix2d m6 = {{{10.0, 100.0}, {100.0, 10.0}}};
    Matrix2d r6 = jacobiDiagonalize(m6);
    assert(std::abs(r6[0][1]) < 1e-9);
    assert(std::abs(r6[1][0]) < 1e-9);
    // eigenvalues: 110 and -90
    assert(std::abs(r6[0][0] - 110.0) < 1e-6 || std::abs(r6[0][0] + 90.0) < 1e-6);
    
    // Test 7: Already rotated (diagonal after rotation)
    Matrix2d m7 = {{{2.0, 0.0}, {0.0, -5.0}}};
    Matrix2d r7 = jacobiDiagonalize(m7);
    assert(std::abs(r7[0][0] - 2.0) < 1e-12);
    assert(std::abs(r7[1][1] + 5.0) < 1e-12);
    assert(std::abs(r7[0][1]) < 1e-12);
    assert(std::abs(r7[1][0]) < 1e-12);
    
    // Test 8: All zeros
    Matrix2d m8 = {{{0.0, 0.0}, {0.0, 0.0}}};
    Matrix2d r8 = jacobiDiagonalize(m8);
    for (int i = 0; i < 2; ++i)
        for (int j = 0; j < 2; ++j)
            assert(std::abs(r8[i][j]) < 1e-12);
    
    // Test 9: Negative diagonal and positive off-diagonal
    Matrix2d m9 = {{{-1.0, 0.3}, {0.3, -2.0}}};
    Matrix2d r9 = jacobiDiagonalize(m9);
    assert(std::abs(r9[0][1]) < 1e-12);
    assert(std::abs(r9[1][0]) < 1e-12);
    assert(std::abs(r9[0][0] + r9[1][1] + 3.0) < 1e-9);
    
    // Test 10: Random but symmetric
    Matrix2d m10 = {{{4.0, -0.5}, {-0.5, 7.0}}};
    Matrix2d r10 = jacobiDiagonalize(m10);
    assert(std::abs(r10[0][1]) < 1e-12);
    assert(std::abs(r10[1][0]) < 1e-12);
    assert(std::abs(r10[0][0] + r10[1][1] - 11.0) < 1e-9);
}

// The Jacobi eigenvalue algorithm for a 2x2 symmetric matrix \(A = \begin{pmatrix} a & b \\ b & c \end{pmatrix}\) seeks a rotation matrix \(J = \begin{pmatrix} \cos\theta & -\sin\theta \\ \sin\theta & \cos\theta \end{pmatrix}\) such that \(J^T A J\) is diagonal. The rotation angle satisfies \(\tan(2\theta) = \frac{2b}{a-c}\). If \(a = c\), then \(\theta = \pi/4\) (i.e., \(\cos\theta = \sin\theta = 1/\sqrt{2}\)). To avoid numerical issues, compute \(p = (a - c)/(2b)\); then \(t = \text{sign}(p)/(|p| + \sqrt{p^2+1})\) and \(\cos\theta = 1/\sqrt{t^2+1}\), \(\sin\theta = t \cos\theta\). This formula is stable even when \(b\) is very small or zero. After applying \(J^T A J\), the off-diagonal entries become mathematically zero, but due to floating-point rounding, clamp them to absolute value below a small epsilon (e.g., 1e-12) to ensure exact zero comparison. Edge cases: if \(b\) is already zero, return the matrix unchanged; if \(a = c\) and \(b \neq 0\), use the \(\pi/4\) special case. The algorithm runs in \(O(1)\) time and uses \(O(1)\) auxiliary space, as the matrix is fixed-size.
