// Write a C++ function `solveLinearSystem` that accepts two square `Eigen::MatrixXd` matrices, `A` and `B`, both of the same dimension `n x n`, where `A` is guaranteed to be invertible. The function must return the unique solution matrix `X` (also `n x n`) to the matrix equation `A * X = B`. Use LU decomposition with partial pivoting (via `Eigen::FullPivLU` or `Eigen::PartialPivLU`) to compute the solution. In addition to returning the solution, the function must also verify the accuracy by computing the relative residual norm `||A*X - B||_F / ||B||_F` (Frobenius norm) and assert that it is below a small tolerance (e.g., `1e-12`) for well-conditioned matrices. The function should apply `const` correctness on input parameters and return the solution by value. You may assume `Eigen` is available via `#include <Eigen/Dense>`.

#include <Eigen/Dense>
#include <cassert>
#include <cmath>

int main() {
    // Test 1: Identity matrix, B = arbitrary 3x3.
    Eigen::MatrixXd A1 = Eigen::MatrixXd::Identity(3, 3);
    Eigen::MatrixXd B1(3, 3);
    B1 << 1, 2, 3,
          4, 5, 6,
          7, 8, 9;
    Eigen::MatrixXd X1 = solveLinearSystem(A1, B1);
    assert(X1.isApprox(B1, 1e-12));

    // Test 2: 2x2 simple system.
    Eigen::MatrixXd A2(2, 2);
    A2 << 2, 1,
          1, 3;
    Eigen::MatrixXd B2(2, 2);
    B2 << 1, 0,
          0, 1;
    Eigen::MatrixXd X2 = solveLinearSystem(A2, B2);
    Eigen::MatrixXd expected2(2, 2);
    expected2 << 0.6, -0.2,
                -0.2, 0.4;
    assert(X2.isApprox(expected2, 1e-12));

    // Test 3: 4x4 random invertible matrix (deterministic seed).
    srand(1234);
    Eigen::MatrixXd A3 = Eigen::MatrixXd::Random(4, 4);
    // Ensure invertible by adding a large identity scaling.
    A3 += 4.0 * Eigen::MatrixXd::Identity(4, 4);
    Eigen::MatrixXd B3 = Eigen::MatrixXd::Random(4, 4);
    Eigen::MatrixXd X3 = solveLinearSystem(A3, B3);
    assert((A3 * X3 - B3).norm() / B3.norm() < 1e-12);

    // Test 4: Zero B matrix yields zero X.
    Eigen::MatrixXd A4(2, 2);
    A4 << 1, 2,
          3, 4;
    Eigen::MatrixXd B4 = Eigen::MatrixXd::Zero(2, 2);
    Eigen::MatrixXd X4 = solveLinearSystem(A4, B4);
    assert(X4.isApprox(Eigen::MatrixXd::Zero(2, 2), 1e-12));

    // Test 5: 1x1 system.
    Eigen::MatrixXd A5(1, 1);
    A5 << 5.0;
    Eigen::MatrixXd B5(1, 1);
    B5 << 15.0;
    Eigen::MatrixXd X5 = solveLinearSystem(A5, B5);
    assert(std::abs(X5(0,0) - 3.0) < 1e-12);

    return 0;
}

#include <Eigen/Dense>
#include <cassert>
#include <cmath>

// Solves A * X = B for square invertible matrices A and B.
// Returns the solution matrix X.
// Asserts that the relative residual norm is below 1e-12.
Eigen::MatrixXd solveLinearSystem(const Eigen::MatrixXd& A, const Eigen::MatrixXd& B) {
    // Validate dimensions: both must be square and same size.
    const Eigen::Index n = A.rows();
    assert(A.cols() == n && B.rows() == n && B.cols() == n && "Matrices must be square and same dimension");

    // Use LU decomposition with partial pivoting for efficiency.
    Eigen::PartialPivLU<Eigen::MatrixXd> lu(A);

    // Compute solution.
    Eigen::MatrixXd X = lu.solve(B);

    // Verify accuracy via relative residual norm.
    const double residual_norm = (A * X - B).norm();
    const double b_norm = B.norm();
    if (b_norm > 0.0) {
        double relative_error = residual_norm / b_norm;
        assert(relative_error < 1e-12 && "Relative residual norm exceeds tolerance");
    } else {
        // If B is zero, the solution should be zero as well.
        assert(residual_norm < 1e-12 && "Zero B should yield zero residual");
    }

    return X;
}

// The core algorithm is to solve `A X = B` using LU decomposition. For a square invertible matrix `A`, we decompose `A` into a lower triangular matrix `L` and an upper triangular matrix `U` (with permutation matrices due to pivoting). Then we solve `L Y = P B` (forward substitution) and `U X = Y` (back substitution). Eigen’s `FullPivLU` or `PartialPivLU` handles this internally and provides a `.solve()` method. The `FullPivLU` is more robust (handles rank deficiency detection) but slightly slower; for guaranteed invertible matrices, `PartialPivLU` is sufficient and faster. Edge cases: if `A` is singular, the decomposition might produce a solution but with poor accuracy; we guard against this by checking the relative residual norm and asserting it is below a tolerance. If `B` is zero, the solution is zero and the residual is zero (avoid division by zero by checking `B.norm()`). The time complexity is dominated by the LU decomposition, which is \(O(n^3)\) for an `n x n` matrix. Space complexity is \(O(n^2)\) for storing the decomposition internally, plus the output matrix.
