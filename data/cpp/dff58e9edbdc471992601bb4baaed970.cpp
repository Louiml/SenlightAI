/*
Write a C++ function that takes a 2x2 matrix of floating-point values (represented as `std::array<std::array<double,2>,2>`) and applies one step of the Jacobi eigenvalue algorithm to zero out the off-diagonal element at position (1,0) using a sequence of similarity transformations. Specifically, compute a rotation matrix \(J\) (with \(\cos\theta\) and \(\sin\theta\) entries) such that \((J^T M J)_{1,0} = 0\), then return the transformed matrix \(J^T M J\). The function must handle symmetric matrices (where \(M_{1,0} = M_{0,1}\)) and must preserve the symmetry of the result. If the off-diagonal element is already zero, return the input unchanged. The returned matrix should have the same precision as the input (use `double`).
*/

#include <array>
#include <cmath>

// Return the result of applying one Jacobi rotation to a symmetric 2x2 matrix.
// The input is assumed symmetric (m[0][1] == m[1][0]).
// The returned matrix is also symmetric and has zero off-diagonal entries (within FP precision).
std::array<std::array<double,2>,2> applyJacobiRotation(const std::array<std::array<double,2>,2>& m) {
    // If off-diagonal is already zero, nothing to do.
    if (m[0][1] == 0.0 && m[1][0] == 0.0) {
        return m;
    }

    double a = m[0][0];
    double b = m[0][1]; // or m[1][0]; symmetric
    double d = m[1][1];

    double c, s;
    if (a == d) {
        // Equal diagonal entries: rotation angle = pi/4
        c = std::sqrt(0.5);
        s = std::sqrt(0.5);
    } else {
        // Use tan(2theta) = 2b/(a-d) and half-angle formulas.
        double t = (2.0 * b) / (a - d);
        double theta = 0.5 * std::atan(t);
        c = std::cos(theta);
        s = std::sin(theta);
    }

    // Compute transformed entries directly to avoid extra matrix multiplications.
    double cs = c * s;
    double c2 = c * c;
    double s2 = s * s;

    double newA = c2 * a - 2.0 * cs * b + s2 * d;
    double newD = s2 * a + 2.0 * cs * b + c2 * d;
    // The new off-diagonal should be zero mathematically, but we compute it to be explicit.
    double newB = cs * (a - d) + (c2 - s2) * b;

    // Force exact symmetry (and essentially zero off-diagonal).
    std::array<std::array<double,2>,2> result;
    result[0][0] = newA;
    result[1][1] = newD;
    result[0][1] = newB;
    result[1][0] = newB;
    return result;
}

#include <cassert>
#include <cmath>
#include <array>

// Function prototype (from solution).
std::array<std::array<double,2>,2> applyJacobiRotation(const std::array<std::array<double,2>,2>& m);

int main() {
    // Test 1: Simple symmetric matrix with nonzero off-diagonal.
    std::array<std::array<double,2>,2> m1 = {{{2.0, 1.0}, {1.0, 2.0}}};
    auto r1 = applyJacobiRotation(m1);
    assert(std::fabs(r1[0][1]) < 1e-12);
    assert(std::fabs(r1[1][0]) < 1e-12);
    assert(std::fabs(r1[0][0] - 3.0) < 1e-12); // eigenvalues are 3 and 1
    assert(std::fabs(r1[1][1] - 1.0) < 1e-12);

    // Test 2: Already diagonal matrix unchanged.
    std::array<std::array<double,2>,2> m2 = {{{5.0, 0.0}, {0.0, -3.0}}};
    auto r2 = applyJacobiRotation(m2);
    assert(r2 == m2);

    // Test 3: Equal diagonal entries.
    std::array<std::array<double,2>,2> m3 = {{{0.0, 2.0}, {2.0, 0.0}}};
    auto r3 = applyJacobiRotation(m3);
    assert(std::fabs(r3[0][1]) < 1e-12);
    assert(std::fabs(r3[0][0] - 2.0) < 1e-12);
    assert(std::fabs(r3[1][1] + 2.0) < 1e-12);

    // Test 4: Negative off-diagonal.
    std::array<std::array<double,2>,2> m4 = {{{4.0, -1.0}, {-1.0, 4.0}}};
    auto r4 = applyJacobiRotation(m4);
    assert(std::fabs(r4[0][1]) < 1e-12);
    assert(std::fabs(r4[1][0]) < 1e-12);
    assert(std::fabs(r4[0][0] - 5.0) < 1e-12);
    assert(std::fabs(r4[1][1] - 3.0) < 1e-12);

    // Test 5: Symmetry of output.
    std::array<std::array<double,2>,2> m5 = {{{1.0, 3.0}, {3.0, -2.0}}};
    auto r5 = applyJacobiRotation(m5);
    assert(r5[0][1] == r5[1][0]);

    // Test 6: Zero matrix remains zero.
    std::array<std::array<double,2>,2> m6 = {{{0.0, 0.0}, {0.0, 0.0}}};
    auto r6 = applyJacobiRotation(m6);
    assert(r6[0][0] == 0.0 && r6[0][1] == 0.0 && r6[1][0] == 0.0 && r6[1][1] == 0.0);

    return 0;
}

// The Jacobi rotation for a 2x2 symmetric matrix \(M = \begin{pmatrix} a & b \\ b & d \end{pmatrix}\) zeroes the off-diagonal entry \(b\) by choosing an angle \(\theta\) such that \(\tan 2\theta = \frac{2b}{a-d}\). The rotation matrix is \(J = \begin{pmatrix} c & s \\ -s & c \end{pmatrix}\) where \(c = \cos\theta\), \(s = \sin\theta\). We compute \(c\) and \(s\) using a numerically stable formulation: if \(a = d\), set \(\theta = \pi/4\) so \(c = s = \sqrt{0.5}\). Otherwise, let \(t = \frac{2b}{a-d}\), and use \(\theta = \frac{1}{2}\arctan(t)\); then \(c = \cos\theta\), \(s = \sin\theta\). The transformed matrix \(M' = J^T M J\) is computed by directly applying the formulas: \(M'_{0,0} = c^2 a - 2cs b + s^2 d\), \(M'_{1,1} = s^2 a + 2cs b + c^2 d\), and \(M'_{0,1} = M'_{1,0} = cs(a-d) + (c^2 - s^2)b\). This ensures the off-diagonal becomes exactly zero (within floating-point error). Edge cases: when \(b=0\) exactly, return the original matrix (to avoid unnecessary computation and potential precision loss). Symmetry is preserved because we compute both off-diagonals to the same value; we can set both to the same computed value. Time complexity is O(1); space complexity is O(1) (only a few temporaries).
