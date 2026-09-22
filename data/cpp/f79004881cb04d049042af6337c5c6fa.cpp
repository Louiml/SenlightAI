// Write a C++ function named `solveLinearSystem` that takes two constant references to Eigen matrices, `A` (size \(n \times n\), square) and `B` (size \(n \times k\)), and returns the \(n \times k\) matrix `X` that is the solution to the matrix equation \(A X = B\). The function must use LU decomposition with partial pivoting to solve the system, and it must assume that the matrix `A` is invertible (i.e., square, non-singular). The function should compute the solution numerically and return it as an `Eigen::MatrixXd`. Do not include a `main` function in the solution code. The test code will verify correctness by checking that the relative residual \(\|A X - B\|_2 / \|B\|_2\) is smaller than a small tolerance (e.g., \(10^{-12}\)) for random invertible matrices of moderate size.

#include <Eigen/Dense>
#include <cassert>
#include <cmath>

// Disable the global solution function name to avoid conflict? No need, we'll just include the solution code above in the same file.

int main() {
    // Test 1: 2x2 system with known solution
    Eigen::MatrixXd A1(2,2);
    A1 << 4.0, 7.0,
          2.0, 6.0;
    Eigen::MatrixXd B1(2,2);
    B1 << 1.0, 2.0,
          3.0, 4.0;
    Eigen::MatrixXd X1 = solveLinearSystem(A1, B1);
    double relErr1 = (A1*X1 - B1).norm() / B1.norm();
    assert(relErr1 < 1e-12);

    // Test 2: 3x3 random invertible matrix (identity plus small random)
    Eigen::MatrixXd A2 = Eigen::MatrixXd::Identity(3,3);
    A2(0,1) = 0.1;
    A2(1,2) = -0.2;
    A2(2,0) = 0.3;
    Eigen::MatrixXd B2 = Eigen::MatrixXd::Random(3,2);
    Eigen::MatrixXd X2 = solveLinearSystem(A2, B2);
    double relErr2 = (A2*X2 - B2).norm() / B2.norm();
    assert(relErr2 < 1e-12);

    // Test 3: 1x1 system (scalar)
    Eigen::MatrixXd A3(1,1);
    A3 << 5.0;
    Eigen::MatrixXd B3(1,1);
    B3 << 20.0;
    Eigen::MatrixXd X3 = solveLinearSystem(A3, B3);
    assert(std::abs(X3(0,0) - 4.0) < 1e-12);

    // Test 4: 4x4 diagonal system
    Eigen::MatrixXd A4 = Eigen::MatrixXd::Zero(4,4);
    A4(0,0) = 2.0;
    A4(1,1) = -3.0;
    A4(2,2) = 5.0;
    A4(3,3) = 7.0;
    Eigen::MatrixXd B4 = Eigen::MatrixXd::Ones(4,1);
    Eigen::MatrixXd X4 = solveLinearSystem(A4, B4);
    Eigen::MatrixXd expected4(4,1);
    expected4 << 0.5, -1.0/3.0, 0.2, 1.0/7.0;
    assert((X4 - expected4).norm() < 1e-12);

    // Test 5: Larger 5x5 random invertible matrix (ensuring numerical stability)
    Eigen::MatrixXd A5 = Eigen::MatrixXd::Random(5,5);
    // Add a large diagonal to make it well-conditioned
    A5 += 10.0 * Eigen::MatrixXd::Identity(5,5);
    Eigen::MatrixXd B5 = Eigen::MatrixXd::Random(5,3);
    Eigen::MatrixXd X5 = solveLinearSystem(A5, B5);
    double relErr5 = (A5*X5 - B5).norm() / B5.norm();
    assert(relErr5 < 1e-12);
}

#include <Eigen/Dense>

// Solve the linear system A * X = B using LU decomposition with partial pivoting.
// Precondition: A must be square and invertible (non-singular).
// Returns the solution matrix X of the same size as B.
Eigen::MatrixXd solveLinearSystem(const Eigen::MatrixXd& A, const Eigen::MatrixXd& B) {
    // Use Eigen's built-in LU decomposition with partial pivoting (ColumnPivotingLU)
    // which is numerically stable for invertible matrices.
    Eigen::MatrixXd X = A.lu().solve(B);
    return X;
}

// The main algorithm uses Eigen's `FullPivLU` or the simpler `.lu()` partial-pivoting LU decomposition to solve the linear system. For a square invertible matrix `A`, the LU factorization decomposes `A` as \(A = P L U\) (with permutation matrix `P`, lower triangular `L`, and upper triangular `U`) in \(O(n^3)\) time. Then solving for `X` involves two back-substitution steps: first solve \(L Y = P^T B\), then solve \(U X = Y\), each \(O(n^2 k)\) time. The overall complexity is \(O(n^3 + n^2 k)\), with \(O(n^2)\) auxiliary space for the matrix decomposition (plus the returned matrix). Edge cases: the input must be a valid square matrix; if `A` is singular or nearly singular, the numerical solution may be inaccurate, but the task assumes invertibility. The function should be `const`-correct by taking `const Eigen::MatrixXd&` parameters and returning by value. Use `.lu().solve(B)` which internally uses column-pivoting LU and is robust for most invertible matrices. For extreme ill-conditioning, full-pivoting could be used, but the task specification does not require it.
