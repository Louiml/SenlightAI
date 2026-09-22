Write a standalone C++ function that computes the coefficients of a piecewise cubic Hermite spline in 1D, given the initial and final parameter values (`sInit`, `sGoal`), the initial and final positions (`PosInit`, `PosGoal`), and the initial and final velocities (`VelInit`, `VelGoal`). The function must solve the cubic polynomial \( p(s) = a s^3 + b s^2 + c s + d \) subject to the four constraints: \( p(sInit)=PosInit \), \( p'(sInit)=VelInit \), \( p(sGoal)=PosGoal \), and \( p'(sGoal)=VelGoal \). The output must be the four coefficients \( a, b, c, d \) passed by reference. The solution must handle the general case where \( sInit \neq sGoal \), and must be numerically stable (avoid division by zero). The function should be self-contained, using only standard C++ headers, and must not rely on any external libraries or classes.
#include <cassert>
#include <cmath>

// Test function is declared above
void cubicHermiteCoefficients1D(double, double, double, double, double, double, double&, double&, double&, double&);

int main() {
    // Test 1: Simple case, s from 0 to 1, position 0->1, velocity 0->0
    double a, b, c, d;
    cubicHermiteCoefficients1D(0.0, 1.0, 0.0, 0.0, 1.0, 0.0, a, b, c, d);
    // p(s) = 0*s^3 + 0*s^2 + 0*s + 0? Actually solution: p(s)=3s^2-2s^3? Let's verify:
    // p(0)=0, p'(0)=0, p(1)=1, p'(1)=0 => a=-2, b=3, c=0, d=0
    assert(std::fabs(a - (-2.0)) < 1e-9);
    assert(std::fabs(b - 3.0) < 1e-9);
    assert(std::fabs(c - 0.0) < 1e-9);
    assert(std::fabs(d - 0.0) < 1e-9);

    // Test 2: Symmetric case, s from 0 to 1, position 1->0, velocity 0->0
    cubicHermiteCoefficients1D(0.0, 1.0, 1.0, 0.0, 0.0, 0.0, a, b, c, d);
    // p(s)=? Should be a=2, b=-3, c=0, d=1
    assert(std::fabs(a - 2.0) < 1e-9);
    assert(std::fabs(b - (-3.0)) < 1e-9);
    assert(std::fabs(c - 0.0) < 1e-9);
    assert(std::fabs(d - 1.0) < 1e-9);

    // Test 3: Non-zero velocities, arbitrary s range
    cubicHermiteCoefficients1D(1.0, 3.0, 4.0, 2.0, 10.0, -1.0, a, b, c, d);
    // Verify by directly evaluating at endpoints and derivatives
    double s = 1.0;
    double p_init = a*s*s*s + b*s*s + c*s + d;
    double p_prime_init = 3*a*s*s + 2*b*s + c;
    assert(std::fabs(p_init - 4.0) < 1e-9);
    assert(std::fabs(p_prime_init - 2.0) < 1e-9);
    s = 3.0;
    double p_goal = a*s*s*s + b*s*s + c*s + d;
    double p_prime_goal = 3*a*s*s + 2*b*s + c;
    assert(std::fabs(p_goal - 10.0) < 1e-9);
    assert(std::fabs(p_prime_goal - (-1.0)) < 1e-9);

    // Test 4: Negative s range
    cubicHermiteCoefficients1D(-2.0, -1.0, -5.0, 1.0, 2.0, 0.5, a, b, c, d);
    // Verify endpoints
    s = -2.0;
    double p_init2 = a*s*s*s + b*s*s + c*s + d;
    double p_prime_init2 = 3*a*s*s + 2*b*s + c;
    assert(std::fabs(p_init2 + 5.0) < 1e-9);
    assert(std::fabs(p_prime_init2 - 1.0) < 1e-9);
    s = -1.0;
    double p_goal2 = a*s*s*s + b*s*s + c*s + d;
    double p_prime_goal2 = 3*a*s*s + 2*b*s + c;
    assert(std::fabs(p_goal2 - 2.0) < 1e-9);
    assert(std::fabs(p_prime_goal2 - 0.5) < 1e-9);

    // Test 5: Zero-length interval should trigger assertion (commented out because assert aborts)
    // Uncomment to test:
    // cubicHermiteCoefficients1D(0.0, 0.0, 1.0, 2.0, 3.0, 4.0, a, b, c, d);

    // Test 6: Check symmetry property? No, but just another random case
    cubicHermiteCoefficients1D(0.0, 2.0, 0.0, 10.0, 0.0, -10.0, a, b, c, d);
    // Verify at s=0 and s=2
    s = 0.0;
    assert(std::fabs(a*s*s*s + b*s*s + c*s + d - 0.0) < 1e-9);
    assert(std::fabs(3*a*s*s + 2*b*s + c - 10.0) < 1e-9);
    s = 2.0;
    assert(std::fabs(a*s*s*s + b*s*s + c*s + d - 0.0) < 1e-9);
    assert(std::fabs(3*a*s*s + 2*b*s + c + 10.0) < 1e-9);

    return 0;
}
#include <cmath>
#include <cassert>

/**
 * Solves a 3x3 linear system A * x = b using Gaussian elimination with partial pivoting.
 * Assumes A is invertible. Modifies A and b.
 */
bool solve3x3(double A[3][3], double b[3], double x[3]) {
    // Gaussian elimination with partial pivoting
    for (int col = 0; col < 3; ++col) {
        // Find pivot row
        int pivotRow = col;
        double maxVal = std::fabs(A[col][col]);
        for (int row = col + 1; row < 3; ++row) {
            double val = std::fabs(A[row][col]);
            if (val > maxVal) {
                maxVal = val;
                pivotRow = row;
            }
        }
        if (maxVal < 1e-12) {
            return false; // singular matrix
        }
        // Swap rows
        if (pivotRow != col) {
            for (int j = col; j < 3; ++j) {
                std::swap(A[col][j], A[pivotRow][j]);
            }
            std::swap(b[col], b[pivotRow]);
        }
        // Eliminate below
        for (int row = col + 1; row < 3; ++row) {
            double factor = A[row][col] / A[col][col];
            for (int j = col; j < 3; ++j) {
                A[row][j] -= factor * A[col][j];
            }
            b[row] -= factor * b[col];
        }
    }
    // Back substitution
    for (int row = 2; row >= 0; --row) {
        double sum = b[row];
        for (int j = row + 1; j < 3; ++j) {
            sum -= A[row][j] * x[j];
        }
        x[row] = sum / A[row][row];
    }
    return true;
}

/**
 * Computes coefficients a, b, c, d of cubic polynomial p(s) = a*s^3 + b*s^2 + c*s + d
 * that satisfies p(sInit)=PosInit, p'(sInit)=VelInit, p(sGoal)=PosGoal, p'(sGoal)=VelGoal.
 */
void cubicHermiteCoefficients1D(
    double sInit, double sGoal,
    double PosInit, double VelInit,
    double PosGoal, double VelGoal,
    double &a, double &b, double &c, double &d)
{
    // Ensure distinct parameter values
    assert(std::fabs(sInit - sGoal) > 1e-12);

    // Set up 3x3 system for (a, b, c)
    double A[3][3] = {
        {3 * sInit * sInit, 2 * sInit, 1.0},
        {3 * sGoal * sGoal, 2 * sGoal, 1.0},
        {sGoal * sGoal * sGoal - sInit * sInit * sInit,
         sGoal * sGoal - sInit * sInit,
         sGoal - sInit}
    };
    double rhs[3] = {VelInit, VelGoal, PosGoal - PosInit};
    double abc[3];

    bool solvable = solve3x3(A, rhs, abc);
    assert(solvable); // guaranteed if sInit != sGoal

    a = abc[0];
    b = abc[1];
    c = abc[2];
    d = PosInit - a * sInit * sInit * sInit - b * sInit * sInit - c * sInit;
}
// The task is to determine the unique cubic polynomial that matches position and velocity (first derivative) at two distinct parameter values. The approach is to set up a linear system of four equations in four unknowns (the coefficients \( a, b, c, d \)). The constraints are:
//
// 1. \( p(sInit) = a sInit^3 + b sInit^2 + c sInit + d = PosInit \)
// 2. \( p'(sInit) = 3 a sInit^2 + 2 b sInit + c = VelInit \)
// 3. \( p(sGoal) = a sGoal^3 + b sGoal^2 + c sGoal + d = PosGoal \)
// 4. \( p'(sGoal) = 3 a sGoal^2 + 2 b sGoal + c = VelGoal \)
//
// We can eliminate \( d \) by subtracting the first equation from the third equation: \( a (sGoal^3 - sInit^3) + b (sGoal^2 - sInit^2) + c (sGoal - sInit) = PosGoal - PosInit \). This gives a system of three equations in \( a, b, c \), with the following matrix form:
//
// \[
// \begin{bmatrix}
// 3 sInit^2 & 2 sInit & 1 \\
// 3 sGoal^2 & 2 sGoal & 1 \\
// sGoal^3 - sInit^3 & sGoal^2 - sInit^2 & sGoal - sInit
// \end{bmatrix}
// \begin{bmatrix}
// a \\ b \\ c
// \end{bmatrix}
// =
// \begin{bmatrix}
// VelInit \\ VelGoal \\ PosGoal - PosInit
// \end{bmatrix}
// \]
//
// The matrix is invertible as long as \( sInit \neq sGoal \). We can solve this 3x3 linear system using Cramer’s rule or a direct Gaussian elimination. After obtaining \( a, b, c \), we compute \( d = PosInit - a sInit^3 - b sInit^2 - c sInit \).
//
// Edge cases: If \( sInit == sGoal \), the problem is ill-posed (no unique cubic). The function should assert or handle gracefully (e.g., set coefficients to zero or throw). Also, very close values of \( sInit \) and \( sGoal \) may cause numerical instability; using a stable linear solver (e.g., Gaussian elimination with partial pivoting) is recommended.
//
// Time complexity is O(1) since it is a fixed 3x3 solve. Space complexity is O(1).
