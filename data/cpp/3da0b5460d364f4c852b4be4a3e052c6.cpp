/*
Write a C++ function that takes a 2x2 matrix of real numbers (represented as a fixed-size 2x2 array of `double`, e.g., `std::array<std::array<double,2>,2>` or a custom struct) and applies one step of the Jacobi eigenvalue algorithm to zero out the off-diagonal element at position (0,1). Specifically, given a symmetric matrix `m`, compute a Jacobi rotation (a 2x2 orthogonal matrix `J` of the form `[[c, s], [-s, c]]` with `c = cos(theta)`, `s = sin(theta)`) that makes the off-diagonal entry of `J^T * m * J` zero (or nearly zero within a small epsilon). Your function should return the rotated matrix `m' = J^T * m * J` (also a 2x2 matrix) while also outputting the rotation parameters `c` and `s` via reference parameters. The input matrix may not be exactly symmetric; you should symmetrize it first by replacing it with `(m + m^T)/2` before computing the rotation. Handle edge cases where the off-diagonal element is already zero (return the matrix unchanged with `c=1, s=0`), and avoid division by zero by using a safe formula for the rotation angle.
*/

#include <array>
#include <cmath>
#include <algorithm>

// Represents a 2x2 matrix with double entries
using Matrix2 = std::array<std::array<double, 2>, 2>;

// Apply one Jacobi rotation to zero the (0,1) off-diagonal element.
// Input 'm' is modified in place to become the rotated matrix J^T * m * J.
// On output, 'c' and 's' are the cosine and sine of the rotation angle.
// If the off-diagonal is already zero, the matrix is returned unchanged with c=1, s=0.
void jacobiRotation(Matrix2& m, double& c, double& s) {
    // Symmetrize the input matrix: replace m with (m + m^T)/2
    double sym01 = (m[0][1] + m[1][0]) / 2.0;
    double sym10 = sym01;
    // Keep diagonal as is (average of itself with itself)
    m[0][1] = sym01;
    m[1][0] = sym10;

    double a = m[0][0];
    double b = sym01;
    double d = m[1][1];

    // Handle zero off-diagonal
    if (std::abs(b) < 1e-15) {
        c = 1.0;
        s = 0.0;
        return;
    }

    // Compute rotation using stable formulas
    double tau = (d - a) / (2.0 * b);
    double t;
    if (tau >= 0.0) {
        t = 1.0 / (tau + std::sqrt(1.0 + tau * tau));
    } else {
        t = -1.0 / (-tau + std::sqrt(1.0 + tau * tau));
    }
    c = 1.0 / std::sqrt(1.0 + t * t);
    s = t * c;

    // Apply J^T * m * J where J = [[c, s], [-s, c]]
    // J^T = [[c, -s], [s, c]]
    // Compute intermediate: temp = m * J
    double temp00 = m[0][0] * c + m[0][1] * (-s);
    double temp01 = m[0][0] * s + m[0][1] * c;
    double temp10 = m[1][0] * c + m[1][1] * (-s);
    double temp11 = m[1][0] * s + m[1][1] * c;

    // Now compute J^T * temp
    double new00 = c * temp00 + (-s) * temp10;
    double new01 = c * temp01 + (-s) * temp11;
    double new10 = s * temp00 + c * temp10;
    double new11 = s * temp01 + c * temp11;

    m[0][0] = new00;
    m[0][1] = new01;
    m[1][0] = new10;
    m[1][1] = new11;
}

#include <cassert>
#include <cmath>
#include <array>
#include <iostream>

using Matrix2 = std::array<std::array<double, 2>, 2>;

// Forward declaration of the solution function (to be defined above in actual code)
void jacobiRotation(Matrix2& m, double& c, double& s);

int main() {
    // Test 1: Symmetric matrix with off-diagonal 1
    Matrix2 m1 = {{{2.0, 1.0}, {1.0, 2.0}}};
    double c1, s1;
    jacobiRotation(m1, c1, s1);
    assert(std::abs(m1[0][1]) < 1e-10);
    assert(std::abs(m1[1][0]) < 1e-10);
    // Eigenvalues should be 1 and 3, diagonal after rotation
    assert(std::abs(m1[0][0] - 1.0) < 1e-10 || std::abs(m1[0][0] - 3.0) < 1e-10);
    assert(std::abs(m1[1][1] - 1.0) < 1e-10 || std::abs(m1[1][1] - 3.0) < 1e-10);

    // Test 2: Matrix with zero off-diagonal
    Matrix2 m2 = {{{5.0, 0.0}, {0.0, -3.0}}};
    double c2, s2;
    jacobiRotation(m2, c2, s2);
    assert(c2 == 1.0 && s2 == 0.0);
    assert(m2[0][0] == 5.0 && m2[0][1] == 0.0);
    assert(m2[1][0] == 0.0 && m2[1][1] == -3.0);

    // Test 3: Non-symmetric input gets symmetrized
    Matrix2 m3 = {{{4.0, 2.0}, {0.0, 1.0}}};
    double c3, s3;
    jacobiRotation(m3, c3, s3);
    // After symmetrization, off-diagonal becomes 1.0
    assert(std::abs(m3[0][1] - m3[1][0]) < 1e-10);
    assert(std::abs(m3[0][1]) < 1e-10); // zeroed after rotation

    // Test 4: Large off-diagonal case (stress test for stability)
    Matrix2 m4 = {{{1e6, 1.0}, {1.0, 1e6}}};
    double c4, s4;
    jacobiRotation(m4, c4, s4);
    assert(std::abs(m4[0][1]) < 1e-5);
    // Diagonal should be close to eigenvalues 1e6 +/- 1
    assert(std::abs((m4[0][0] + m4[1][1]) - 2e6) < 1e-3);

    // Test 5: Identity matrix (off-diagonal zero, unchanged)
    Matrix2 m5 = {{{1.0, 0.0}, {0.0, 1.0}}};
    double c5, s5;
    jacobiRotation(m5, c5, s5);
    assert(c5 == 1.0 && s5 == 0.0);
    assert(m5[0][0] == 1.0 && m5[1][1] == 1.0 && m5[0][1] == 0.0 && m5[1][0] == 0.0);

    // Test 6: Negative off-diagonal
    Matrix2 m6 = {{{3.0, -2.0}, {-2.0, 3.0}}};
    double c6, s6;
    jacobiRotation(m6, c6, s6);
    assert(std::abs(m6[0][1]) < 1e-10);
    assert(std::abs(m6[1][0]) < 1e-10);
    // Trace preserved
    assert(std::abs((m6[0][0] + m6[1][1]) - 6.0) < 1e-10);

    // Test 7: Off-diagonal much larger than diagonal
    Matrix2 m7 = {{{0.0, 5.0}, {5.0, 0.0}}};
    double c7, s7;
    jacobiRotation(m7, c7, s7);
    assert(std::abs(m7[0][1]) < 1e-10);
    assert(std::abs(m7[1][0]) < 1e-10);
    // Eigenvalues are 5 and -5
    assert(std::abs(m7[0][0] - 5.0) < 1e-10 || std::abs(m7[0][0] + 5.0) < 1e-10);
    assert(std::abs(m7[1][1] - 5.0) < 1e-10 || std::abs(m7[1][1] + 5.0) < 1e-10);

    return 0;
}

// The Jacobi rotation zeroes a symmetric matrix's off-diagonal entry by applying an orthogonal similarity transformation `J^T * m * J`. For a 2x2 symmetric matrix `[[a, b], [b, d]]`, the rotation angle `theta` satisfies `tan(2*theta) = 2b/(d-a)`. We compute `c = cos(theta)` and `s = sin(theta)` using the standard stable formulas: if `b == 0`, return `c=1, s=0`. Otherwise, compute `tau = (d - a)/(2*b)`, then `t = sign(tau)/(abs(tau) + sqrt(1 + tau^2))` (or `t = 1/(tau + sign(tau)*sqrt(1+tau^2))` for safety when `tau` is large), then `c = 1/sqrt(1+t^2)`, `s = t*c`. After constructing `J = [[c, s], [-s, c]]`, we compute `m' = J^T * m * J` explicitly (since 2x2, we can do arithmetic directly). Edge cases: if `b` is extremely small relative to `a` and `d`, `tau` may overflow; the formula with `t = 1/(tau + sign(tau)*sqrt(1+tau^2))` handles large `tau` by yielding tiny `t`. Also, if input is not symmetric, we symmetrize first. Time complexity is O(1) and space O(1) for a 2x2 matrix. The main algorithm is straightforward: symmetrize, compute rotation, apply similarity transform.
