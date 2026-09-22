Write a standalone C++ function that solves the linear system \( A x = b \) for a 2×2 coefficient matrix \( A \) and a 2×2 right-hand side matrix \( b \), using the LDLT (Cholesky with pivoting) decomposition method. The function should take two `Eigen::Matrix2f` objects (the matrix \( A \) and the matrix \( b \)) as parameters and return the 2×2 solution matrix \( x \). The function must handle symmetric positive definite (SPD) matrices, but it should work for any symmetric matrix where the LDLT decomposition succeeds. Additionally, the function must be `const`-correct, meaning it should accept the input matrices by `const` reference and return the result by value. The function should not print anything; it should only perform the computation and return the solution. Ensure that the solution is computed using Eigen's `LDLT` solver, and that the function is self-contained with all necessary headers included.
#include <cassert>
#include <Eigen/Dense>

// Free function from the solution (declared here for completeness).
Eigen::Matrix2f solveLinearSystem(const Eigen::Matrix2f& A, const Eigen::Matrix2f& b);

int main() {
    // Test case 1: Simple SPD matrix
    Eigen::Matrix2f A1, b1, expected1;
    A1 << 2, -1, -1, 3;
    b1 << 1, 2, 3, 1;
    expected1 << 1.2, 1.4, 0.6, 0.8; // computed manually or via solver
    Eigen::Matrix2f result1 = solveLinearSystem(A1, b1);
    assert(result1.isApprox(expected1, 1e-5));

    // Test case 2: Identity matrix
    Eigen::Matrix2f A2 = Eigen::Matrix2f::Identity();
    Eigen::Matrix2f b2;
    b2 << 5, 6, 7, 8;
    Eigen::Matrix2f result2 = solveLinearSystem(A2, b2);
    assert(result2.isApprox(b2, 1e-5));

    // Test case 3: Diagonal matrix with non-unity entries
    Eigen::Matrix2f A3;
    A3 << 4, 0, 0, -2; // symmetric but indefinite (LDLT works)
    Eigen::Matrix2f b3;
    b3 << 8, 2, -6, 4;
    Eigen::Matrix2f expected3;
    expected3 << 2, 0.5, 3, -2; // solving x = A^{-1} * b
    Eigen::Matrix2f result3 = solveLinearSystem(A3, b3);
    assert(result3.isApprox(expected3, 1e-5));

    // Test case 4: Verify that A * x equals b (residual check)
    Eigen::Matrix2f A4, b4;
    A4 << 1, 2, 2, 5;
    b4 << 3, 4, 5, 6;
    Eigen::Matrix2f result4 = solveLinearSystem(A4, b4);
    Eigen::Matrix2f residual = A4 * result4 - b4;
    assert(residual.norm() < 1e-5);

    // Test case 5: Symmetric matrix with negative diagonal (still SPD if positive definite, but this is indefinite)
    Eigen::Matrix2f A5;
    A5 << 3, 1, 1, 2;
    Eigen::Matrix2f b5;
    b5 << 1, 1, 2, 2;
    Eigen::Matrix2f result5 = solveLinearSystem(A5, b5);
    assert((A5 * result5).isApprox(b5, 1e-5));
}
#include <Eigen/Dense>

// Solve the 2x2 linear system A * x = b using Eigen's LDLT decomposition.
// A must be a symmetric matrix; b is the right-hand side matrix.
// Returns the solution matrix x (same dimensions as b).
Eigen::Matrix2f solveLinearSystem(const Eigen::Matrix2f& A, const Eigen::Matrix2f& b) {
    return A.ldlt().solve(b);
}
// The main algorithm is straightforward: given the symmetric matrix \( A \) and right-hand side \( b \), we compute the LDLT decomposition of \( A \) and then solve the system using Eigen's `ldlt().solve(b)` interface. This decomposition is numerically stable for symmetric matrices, including SPD and indefinite ones, as it uses pivoting to improve stability. The key steps are: (1) include the `<Eigen/Dense>` header to access matrix classes and solvers, (2) define the function signature taking `const Matrix2f&` for both inputs and returning `Matrix2f`, (3) call `A.ldlt().solve(b)` and return the result. Important edge cases: if \( A \) is singular or nearly singular, the LDLT solver may produce `NaN` or `Inf` values; the function does not check for this, which is acceptable for a self-contained exercise but should be noted. The complexity is \( O(1) \) time and space because the matrices are fixed at 2×2, but conceptually the LDLT decomposition for an \( n \times n \) matrix takes \( O(n^3) \) time and \( O(n^2) \) space; for the fixed-size case here, it is constant.
