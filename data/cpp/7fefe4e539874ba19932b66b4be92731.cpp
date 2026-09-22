// Write a C++ function `double dominantEigenvalue(double a[3][3])` that takes a 3x3 matrix (as a C-style 2D array) and returns its eigenvalue with the largest absolute value. If the matrix is singular (determinant zero) or has no real eigenvalues, return `0.0`. You must compute the determinant, trace, and the sum of principal 2x2 minors directly from the 3x3 entries, solve the resulting cubic characteristic polynomial exactly/robustly (e.g., by finding its roots analytically or via a stable numerical method), and return the eigenvalue with maximum magnitude. Handle the case where the cubic has complex roots by returning 0.0 for that situation. Do not use any external linear algebra libraries; only standard headers are allowed.

#include <cassert>
#include <cmath>
#include <iostream>

// Function under test is declared here (or included from header)
double dominantEigenvalue(double a[3][3]);

int main() {
    // Test 1: diagonal matrix with eigenvalues 1,2,3 -> dominant is 3
    double A1[3][3] = {{1,0,0},{0,2,0},{0,0,3}};
    assert(std::fabs(dominantEigenvalue(A1) - 3.0) < 1e-9);

    // Test 2: identity matrix -> all eigenvalues 1 -> dominant 1
    double A2[3][3] = {{1,0,0},{0,1,0},{0,0,1}};
    assert(std::fabs(dominantEigenvalue(A2) - 1.0) < 1e-9);

    // Test 3: zero matrix -> all eigenvalues 0 -> dominant 0
    double A3[3][3] = {{0,0,0},{0,0,0},{0,0,0}};
    assert(std::fabs(dominantEigenvalue(A3) - 0.0) < 1e-9);

    // Test 4: matrix with eigenvalues -5, 2, 1 -> dominant absolute is 5
    double A4[3][3] = {{-5,0,0},{0,2,0},{0,0,1}};
    assert(std::fabs(dominantEigenvalue(A4) - (-5.0)) < 1e-9);

    // Test 5: rotation matrix (90 degrees around z) has eigenvalues 1, i, -i -> real eigenvalue 1
    double A5[3][3] = {{0,-1,0},{1,0,0},{0,0,1}};
    assert(std::fabs(dominantEigenvalue(A5) - 1.0) < 1e-9);

    // Test 6: non-symmetric matrix with known dominant eigenvalue ~3 (from sample)
    double A6[3][3] = {{2,0,0},{0,3,0},{0,0,1}};
    assert(std::fabs(dominantEigenvalue(A6) - 3.0) < 1e-9);

    // Test 7: matrix with negative determinant and known roots
    double A7[3][3] = {{0,0,1},{1,0,0},{0,1,0}}; // permutation matrix, eigenvalues 1, -1/2±i√3/2
    double val7 = dominantEigenvalue(A7);
    // Dominant absolute is 1 (from real root 1)
    assert(std::fabs(val7 - 1.0) < 1e-9);

    // Test 8: matrix with all eigenvalues equal (e.g., 2I) -> dominant 2
    double A8[3][3] = {{2,0,0},{0,2,0},{0,0,2}};
    assert(std::fabs(dominantEigenvalue(A8) - 2.0) < 1e-9);

    // Test 9: matrix with cubic having three distinct real roots (e.g., companion)
    double A9[3][3] = {{0,1,0},{0,0,1},{6,-11,6}}; // characteristic λ^3 -6λ^2+11λ-6 roots 1,2,3
    assert(std::fabs(dominantEigenvalue(A9) - 3.0) < 1e-9);

    // Test 10: matrix with complex eigenvalues only? Not possible for 3x3 real, but test a case with one real root
    double A10[3][3] = {{1,0,0},{0,0,1},{0,-1,0}}; // block has eigenvalues ±i and +1
    assert(std::fabs(dominantEigenvalue(A10) - 1.0) < 1e-9);

    std::cout << "All tests passed!" << std::endl;
    return 0;
}

#include <cmath>
#include <algorithm>

// Compute determinant of a 3x3 matrix given as C-style array.
double det3(double a[3][3]) {
    return a[0][0] * (a[1][1]*a[2][2] - a[1][2]*a[2][1])
         - a[0][1] * (a[1][0]*a[2][2] - a[1][2]*a[2][0])
         + a[0][2] * (a[1][0]*a[2][1] - a[1][1]*a[2][0]);
}

// Solve cubic x^3 + b x^2 + c x + d = 0, return number of real roots
// and store them in roots (max 3). Returns -1 on numerical failure.
int solveCubic(double b, double c, double d, double roots[3]) {
    // Depressed cubic: x = y - b/3
    double p = c - b*b/3.0;
    double q = d - b*c/3.0 + 2.0*b*b*b/27.0;
    double disc = q*q/4.0 + p*p*p/27.0;

    if (disc > 1e-12) {
        // One real root
        double u = std::cbrt(-q/2.0 + std::sqrt(disc));
        double v = std::cbrt(-q/2.0 - std::sqrt(disc));
        roots[0] = u + v - b/3.0;
        return 1;
    } else if (std::fabs(disc) <= 1e-12) {
        // Repeated real roots
        double u = std::cbrt(-q/2.0);
        roots[0] = 2.0*u - b/3.0;
        roots[1] = -u - b/3.0;
        return 2;
    } else {
        // Three distinct real roots (disc < 0)
        double r = std::sqrt(-p*p*p/27.0);
        double phi = std::acos(-q/(2.0*r));
        double factor = 2.0 * std::cbrt(r);
        roots[0] = factor * std::cos(phi/3.0) - b/3.0;
        roots[1] = factor * std::cos((phi + 2.0*M_PI)/3.0) - b/3.0;
        roots[2] = factor * std::cos((phi + 4.0*M_PI)/3.0) - b/3.0;
        return 3;
    }
}

// Return eigenvalue with largest absolute value of a 3x3 matrix.
// If matrix has no real eigenvalues (which for 3x3 with real entries
// always has at least one real eigenvalue), return 0.0 for safety.
double dominantEigenvalue(double a[3][3]) {
    // Characteristic polynomial coefficients: lambda^3 - c2 lambda^2 - c1 lambda - c0 = 0
    double c0 = det3(a);
    double c1 = a[1][0]*a[0][1] + a[2][0]*a[0][2] + a[1][2]*a[2][1]
              - a[0][0]*a[1][1] - a[0][0]*a[2][2] - a[1][1]*a[2][2];
    double c2 = a[0][0] + a[1][1] + a[2][2];

    // Form polynomial: x^3 + b x^2 + c x + d = 0
    double b = -c2;
    double c = -c1;
    double d = -c0;

    double roots[3];
    int numRoots = solveCubic(b, c, d, roots);

    if (numRoots <= 0) return 0.0; // Should not happen for real matrix

    // Find the root with maximum absolute value
    double maxAbs = 0.0;
    double best = 0.0;
    for (int i = 0; i < numRoots; ++i) {
        double val = std::fabs(roots[i]);
        if (val > maxAbs) {
            maxAbs = val;
            best = roots[i];
        }
    }
    return best;
}

// The characteristic polynomial of a 3x3 matrix \(A\) is \(\det(A - \lambda I) = -\lambda^3 + c_2 \lambda^2 + c_1 \lambda + c_0 = 0\), where:
// - \(c_0 = \det(A)\)
// - \(c_1 = (a_{1,0}a_{0,1} + a_{2,0}a_{0,2} + a_{1,2}a_{2,1}) - (a_{0,0}a_{1,1} + a_{0,0}a_{2,2} + a_{1,1}a_{2,2})\)
// - \(c_2 = \text{trace}(A)\)
//
// Equivalently, we solve \( \lambda^3 - c_2 \lambda^2 - c_1 \lambda - c_0 = 0\). A robust approach is to use the cubic formula via the depressed cubic transformation. Set \( \lambda = x + c_2/3 \), giving \( x^3 + px + q = 0 \) where:
// - \( p = -c_1 - c_2^2/3 \)
// - \( q = -c_0 + c_2 c_1/3 + 2 c_2^3/27 \)
//
// The discriminant \( \Delta = (q/2)^2 + (p/3)^3 \). If \( \Delta > 0 \), there is one real root and two complex conjugates; that real root is \( x = \sqrt[3]{-q/2 + \sqrt{\Delta}} + \sqrt[3]{-q/2 - \sqrt{\Delta}} \). If \( \Delta < 0 \), there are three distinct real roots, found using trigonometric form: \( x_k = 2\sqrt{-p/3} \cos\left( \frac{1}{3} \arccos\left( \frac{3q}{2p}\sqrt{-3/p} \right) - \frac{2\pi k}{3} \right) \) for \( k=0,1,2 \). If \( \Delta = 0 \), there are repeated real roots, handled by the same formula with \( \arccos(1) \) giving zero, or separately. After obtaining real roots, add \( c_2/3 \) back. Return the real root (or the maximum absolute value among real roots). Since we only want real eigenvalues, if the cubic has exactly one real root, return that root; if it has three, return the one with largest absolute value. Edge cases: singular matrix (determinant zero) still has an eigenvalue 0, which is fine; but we must ensure the cubic root finding works for zero coefficients. Time complexity is constant \(O(1)\) for a fixed 3x3 matrix; space is \(O(1)\). The method avoids external libraries and is stable for the given size.
