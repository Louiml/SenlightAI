/*
Given a 3x3 matrix `A` and a 3xN matrix `B` (where N is a positive integer), write a standalone C++ function that solves the linear system `A * X = B` for `X` using a numerically stable QR decomposition with column pivoting (specifically, `colPivHouseholderQr` from Eigen). The function should accept the matrices by const reference and return the solution matrix `X` by value. Ensure the solution satisfies `A * X ≈ B` (within a tight tolerance like 1e-6) for arbitrary real-valued matrices including near-singular or singular `A`. The function must be self-contained, include only necessary Eigen headers, and use `const` correctness. No `main` function is required in the solution; only the function definition.
*/
#include <Eigen/Dense>

// Solve A * X = B for a 3x3 matrix A and a 3xN matrix B.
// Uses column-pivoted Householder QR for numerical stability.
// Returns the 3xN matrix X such that A*X ≈ B.
Eigen::MatrixXf solveLinearSystem(const Eigen::Matrix3f& A, const Eigen::MatrixXf& B) {
    // Validate dimensions: B must have exactly 3 rows.
    assert(B.rows() == 3 && "B must have 3 rows to match A's dimensions");
    
    // Perform QR decomposition with column pivoting.
    Eigen::ColPivHouseholderQR<Eigen::Matrix3f> qr(A);
    
    // Solve for X; returns a 3xN matrix.
    return qr.solve(B);
}
#include <Eigen/Dense>
#include <cassert>

// The solution function is assumed to be declared above.
Eigen::MatrixXf solveLinearSystem(const Eigen::Matrix3f& A, const Eigen::MatrixXf& B);

int main() {
    // Test 1: Well-conditioned 3x3 system with 2 right-hand sides.
    {
        Eigen::Matrix3f A;
        A << 4, 1, 2,
             1, 3, 1,
             2, 1, 5;
        Eigen::MatrixXf B(3, 2);
        B << 9, 10,
             5, 6,
             8, 12;
        Eigen::MatrixXf X = solveLinearSystem(A, B);
        assert((A * X).isApprox(B, 1e-5));
    }
    
    // Test 2: Diagonal matrix (trivial case).
    {
        Eigen::Matrix3f A = Eigen::Matrix3f::Identity() * 2.0f;
        Eigen::MatrixXf B(3, 1);
        B << 4, 6, 8;
        Eigen::MatrixXf X = solveLinearSystem(A, B);
        assert((A * X).isApprox(B, 1e-5));
        assert(X.isApprox(Eigen::Vector3f(2, 3, 4)));
    }
    
    // Test 3: Near-singular matrix (small pivot) — still solves acceptably.
    {
        Eigen::Matrix3f A;
        A << 1, 2, 3,
             2, 4, 6,   // Row 2 is 2× row 1 (rank deficient)
             1, 1, 1;
        Eigen::MatrixXf B(3, 1);
        B << 6, 12, 3;
        Eigen::MatrixXf X = solveLinearSystem(A, B);
        // Check residual is small (least-squares solution).
        assert((A * X - B).norm() < 1e-3);
    }
    
    // Test 4: Single right-hand side (vector).
    {
        Eigen::Matrix3f A;
        A << 0, 1, 0,
             1, 0, 0,
             0, 0, 1;
        Eigen::Vector3f B(3, 4, 5);
        Eigen::Vector3f X = solveLinearSystem(A, B);
        assert(X.isApprox(Eigen::Vector3f(4, 3, 5), 1e-6));
    }
    
    // Test 5: Multiple right-hand sides (3 columns) with random matrices.
    {
        Eigen::Matrix3f A = Eigen::Matrix3f::Random();
        Eigen::MatrixXf B(3, 3);
        B << 1, 2, 3,
             4, 5, 6,
             7, 8, 9;
        Eigen::MatrixXf X = solveLinearSystem(A, B);
        assert((A * X).isApprox(B, 1e-5));
    }
    
    // Test 6: Zero matrix (A = 0) — solution is zero matrix (or any, but Eigen returns zero).
    {
        Eigen::Matrix3f A = Eigen::Matrix3f::Zero();
        Eigen::MatrixXf B(3, 2);
        B << 1, 2,
             3, 4,
             5, 6;
        Eigen::MatrixXf X = solveLinearSystem(A, B);
        // For zero A, the least-squares solution should be zero.
        assert(X.isZero(1e-6));
    }
    
    return 0;
}
// The core approach is to use Eigen’s `colPivHouseholderQr` decomposition, which is robust for both well-conditioned and ill-conditioned systems. The QR decomposition with column pivoting factorizes `A` into `A = Q * R * P^T` (where `P` is a permutation matrix). Solving `A*X = B` then reduces to `R * (P^T * X) = Q^T * B`, which is solved by back-substitution due to `R`’s upper-triangular form. Eigen’s `.solve()` method handles this internally and provides a least-squares solution if the system is underdetermined or inconsistent. The main edge case is near-singular matrices (e.g., rank-deficient), where the method still returns a meaningful minimum-norm solution, and the check `isApprox` with a tolerance accounts for numerical errors. Time complexity is dominated by the QR factorization: \(O(3^3)\) for the 3x3 matrix (constant for fixed size) plus \(O(3^2 * N)\) for solving multiple right-hand sides. Space complexity is \(O(3^2 + 3*N)\) for the decomposition and result matrix. The function uses `const` references, avoids copying inputs, and returns the result by value to allow move semantics.
