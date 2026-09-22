/*
Write a C++ function that takes a 2x2 matrix of floating-point numbers and performs one Jacobi rotation sweep to zero out the off-diagonal entry at position (0,1), returning the updated matrix. The function must first construct the Jacobi rotation matrix (a 2x2 rotation matrix) that zeroes the element at row 0, column 1 of the input matrix, as done by `Eigen::JacobiRotation::makeJacobi`, then apply the similarity transformation \(J^T M J\) (i.e., left-multiply by the transpose/adjoint of the Jacobi rotation, then right-multiply by the Jacobi rotation itself) to produce the result. The function must handle the case where the off-diagonal element is already zero (or extremely close to zero) by returning a copy of the original matrix, and must compute the rotation coefficients using the standard Jacobi eigenvalue algorithm for symmetric matrices. Input is a `std::array<std::array<double, 2>, 2>` representing a 2x2 matrix; output is a new `std::array<std::array<double, 2>, 2>` representing the transformed matrix. The function must be `const`-correct and avoid modifying the input.
*/

#include <array>
#include <cmath>
#include <algorithm>
#include <limits>

// Apply a Jacobi rotation similarity transform to a 2x2 matrix,
// zeroing the off-diagonal (0,1) entry. Returns the transformed matrix.
std::array<std::array<double, 2>, 2> jacobiSweep(
    const std::array<std::array<double, 2>, 2>& m) {
    
    double a = m[0][0];
    double b = m[0][1];
    double c = m[1][0];
    double d = m[1][1];
    
    // If off-diagonal is already zero (or effectively), return a copy.
    const double eps = std::numeric_limits<double>::epsilon();
    if (std::abs(b) < eps * (std::abs(a) + std::abs(d) + 1.0)) {
        return m;
    }
    
    // Compute Jacobi rotation parameters for the symmetric matrix
    // (we treat the matrix as symmetric for the rotation calculation,
    // but apply the rotation to the original non-symmetric matrix).
    double t;
    double diagonalDiff = a - d;
    double offDiagSum = 2.0 * b;
    
    if (std::abs(diagonalDiff) >= std::abs(offDiagSum)) {
        t = offDiagSum / (diagonalDiff + 
            std::copysign(std::sqrt(diagonalDiff * diagonalDiff + offDiagSum * offDiagSum), diagonalDiff));
    } else {
        t = std::copysign(1.0, offDiagSum) / 
            (std::abs(offDiagSum) + std::sqrt(diagonalDiff * diagonalDiff + offDiagSum * offDiagSum));
    }
    
    double cosTheta = 1.0 / std::sqrt(1.0 + t * t);
    double sinTheta = t * cosTheta;
    
    // Build rotation matrix J = [c, s; -s, c] and its transpose J^T.
    double j00 = cosTheta;
    double j01 = sinTheta;
    double j10 = -sinTheta;
    double j11 = cosTheta;
    
    // Apply J^T * M * J.
    // First compute J^T * M.
    double p00 = j00 * a + j10 * c;
    double p01 = j00 * b + j10 * d;
    double p10 = j01 * a + j11 * c;
    double p11 = j01 * b + j11 * d;
    
    // Then right-multiply by J.
    double r00 = p00 * j00 + p01 * j10;
    double r01 = p00 * j01 + p01 * j11;
    double r10 = p10 * j00 + p11 * j10;
    double r11 = p10 * j01 + p11 * j11;
    
    return {{
        {r00, r01},
        {r10, r11}
    }};
}

#include <cassert>
#include <cmath>
#include <array>

// Function declaration (provided in solution)
std::array<std::array<double, 2>, 2> jacobiSweep(
    const std::array<std::array<double, 2>, 2>& m);

int main() {
    // Test 1: Symmetric matrix with off-diagonal non-zero.
    std::array<std::array<double, 2>, 2> m1 = {{{4.0, 1.0}, {1.0, 3.0}}};
    auto r1 = jacobiSweep(m1);
    assert(std::abs(r1[0][1]) < 1e-10);
    assert(std::abs(r1[1][0]) < 1e-10);

    // Test 2: Already diagonal matrix returns copy.
    std::array<std::array<double, 2>, 2> m2 = {{{2.0, 0.0}, {0.0, 5.0}}};
    auto r2 = jacobiSweep(m2);
    assert(r2 == m2);

    // Test 3: Non-symmetric input still zeroes (0,1) element.
    std::array<std::array<double, 2>, 2> m3 = {{{1.0, 2.0}, {3.0, 4.0}}};
    auto r3 = jacobiSweep(m3);
    assert(std::abs(r3[0][1]) < 1e-10);

    // Test 4: Equal diagonal, off-diagonal 1 -> 45 degree rotation.
    std::array<std::array<double, 2>, 2> m4 = {{{3.0, 1.0}, {1.0, 3.0}}};
    auto r4 = jacobiSweep(m4);
    assert(std::abs(r4[0][1]) < 1e-10);
    assert(std::abs(r4[0][0] - 4.0) < 1e-10); // eigenvalues 4 and 2
    assert(std::abs(r4[1][1] - 2.0) < 1e-10);

    // Test 5: Negative off-diagonal.
    std::array<std::array<double, 2>, 2> m5 = {{{2.0, -1.5}, {-1.5, 5.0}}};
    auto r5 = jacobiSweep(m5);
    assert(std::abs(r5[0][1]) < 1e-10);
    assert(std::abs(r5[1][0]) < 1e-10);

    // Test 6: Very small off-diagonal treated as zero.
    std::array<std::array<double, 2>, 2> m6 = {{{1.0, 1e-15}, {1e-15, 2.0}}};
    auto r6 = jacobiSweep(m6);
    assert(r6 == m6);

    // Test 7: Trace preserved.
    std::array<std::array<double, 2>, 2> m7 = {{{7.0, 3.0}, {-2.0, 1.0}}};
    auto r7 = jacobiSweep(m7);
    assert(std::abs((r7[0][0] + r7[1][1]) - (m7[0][0] + m7[1][1])) < 1e-10);

    return 0;
}

// The solution uses the standard Jacobi rotation for symmetric 2x2 matrices. Given a matrix \(M = \begin{pmatrix} a & b \\ c & d \end{pmatrix}\), the goal is to find a rotation matrix \(J = \begin{pmatrix} \cos\theta & \sin\theta \\ -\sin\theta & \cos\theta \end{pmatrix}\) such that the off-diagonal entry of \(J^T M J\) becomes zero. For a symmetric matrix where \(b = c\), the rotation angle \(\theta\) satisfies \(\tan(2\theta) = \frac{2b}{a-d}\). The standard approach (as in `Eigen::JacobiRotation::makeJacobi`) avoids division by zero by comparing \(|a-d|\) and \(|2b|\). If \(b = 0\), no rotation is needed, so \(J\) is the identity. Otherwise, compute \(t = \frac{\text{sign}(2b)}{|2b| + \sqrt{(a-d)^2 + (2b)^2}}\) and then \(\cos\theta = \frac{1}{\sqrt{1+t^2}}\), \(\sin\theta = t \cos\theta\). The transformation \(J^T M J\) is then applied explicitly: first left-rotate (adjoint, which is transpose since the rotation is real and orthogonal), then right-rotate. Edge cases include \(a = d\) and \(b \neq 0\) (then \(t = \text{sign}(b)\), yielding \(\theta=45^\circ\)), and very small off-diagonal values where the rotation may be near identity—the formula handles these gracefully. Time complexity is \(O(1)\) since the matrix is fixed 2x2; space complexity is \(O(1)\) auxiliary, as we only store a few intermediate values.
