/*
Write a C++ function that takes an integer matrix `A` as input and returns the eigenvector corresponding to the smallest eigenvalue of `A`. The matrix is guaranteed to be symmetric and positive semi-definite (so all eigenvalues are real and non-negative), and its size will be at least 2×2. Use the `Eigen` library’s `SelfAdjointEigenSolver` to compute the eigenvalues and eigenvectors, then extract the column index corresponding to the minimum eigenvalue. Return that eigenvector as an `Eigen::VectorXd`. The function must be `const`-correct and handle degenerate cases where the smallest eigenvalue is repeated (in that case, return the first occurrence from the left). Do not modify the input matrix.
*/
#include <Eigen/Dense>

// Return the eigenvector corresponding to the smallest eigenvalue of the symmetric matrix A.
// The matrix A must be symmetric and positive semi-definite. The result is normalized to unit length.
Eigen::VectorXd smallestEigenvector(const Eigen::MatrixXd& A) {
    // SelfAdjointEigenSolver computes eigenvalues in ascending order.
    Eigen::SelfAdjointEigenSolver<Eigen::MatrixXd> solver(A);
    // The smallest eigenvalue is at index 0; its eigenvector is the first column.
    return solver.eigenvectors().col(0);
}
#include <cassert>
#include <cmath>
#include <Eigen/Dense>

// The solution function is declared here for completeness.
Eigen::VectorXd smallestEigenvector(const Eigen::MatrixXd& A);

int main() {
    // 2x2 matrix: eigenvalues are 0 and 4. Smallest eigenvector should be [ -0.707, 0.707 ] or [0.707,-0.707] (sign arbitrary).
    Eigen::MatrixXd A1(2,2);
    A1 << 2, -2, -2, 2;
    Eigen::VectorXd ev1 = smallestEigenvector(A1);
    // Test that it's a unit vector and satisfies A*v = lambda_min*v with lambda_min=0.
    assert(std::abs(ev1.norm() - 1.0) < 1e-9);
    Eigen::VectorXd Av1 = A1 * ev1;
    // Since lambda_min≈0, Av should be nearly zero.
    assert(Av1.norm() < 1e-9);

    // 3x3 matrix of ones: eigenvalues are 0,0,3. Smallest eigenvector is any vector in plane orthogonal to (1,1,1).
    Eigen::MatrixXd A2 = Eigen::MatrixXd::Ones(3,3);
    Eigen::VectorXd ev2 = smallestEigenvector(A2);
    assert(std::abs(ev2.norm() - 1.0) < 1e-9);
    assert(std::abs(ev2.sum()) < 1e-9); // orthogonal to (1,1,1)
    Eigen::VectorXd Av2 = A2 * ev2;
    assert(Av2.norm() < 1e-9); // eigenvalue 0

    // 2x2 diagonal matrix: eigenvalues 2 and 5. Smallest eigenvector = [1,0].
    Eigen::MatrixXd A3(2,2);
    A3 << 2, 0, 0, 5;
    Eigen::VectorXd ev3 = smallestEigenvector(A3);
    assert(std::abs(std::abs(ev3(0)) - 1.0) < 1e-9);
    assert(std::abs(ev3(1)) < 1e-9);

    // 4x4 identity matrix: all eigenvalues 1. Smallest eigenvector is column 0 = [1,0,0,0].
    Eigen::MatrixXd A4 = Eigen::MatrixXd::Identity(4,4);
    Eigen::VectorXd ev4 = smallestEigenvector(A4);
    assert(std::abs(std::abs(ev4(0)) - 1.0) < 1e-9);
    assert(ev4.norm() > 1e-9); // just sanity

    // 2x2 non-diagonal with eigenvalues 1 and 2: test known eigen vector.
    Eigen::MatrixXd A5(2,2);
    A5 << 2, 1, 1, 2; // eigenvalues 1 and 3; smallest eigenvector = [ -0.707, 0.707 ] or opposite.
    Eigen::VectorXd ev5 = smallestEigenvector(A5);
    assert(std::abs(std::abs(ev5(0)) - std::sqrt(0.5)) < 1e-9);
    assert(std::abs(std::abs(ev5(1)) - std::sqrt(0.5)) < 1e-9);

    return 0;
}
// The problem reduces to performing a spectral decomposition of a symmetric matrix. The `SelfAdjointEigenSolver` computes all eigenvalues in ascending order (for symmetric matrices) and stores the corresponding eigenvectors as columns of its `eigenvectors()` matrix. Therefore the smallest eigenvalue is always at index 0 of the eigenvalue vector, and the corresponding eigenvector is the first column of the eigenvector matrix. However, since the order is guaranteed sorted ascending, we can simply return `es.eigenvectors().col(0)`. Edge cases: if eigenvalues are repeated, the solver still provides a valid orthogonal basis for the eigenspace; we only need the first column. The matrix size is at least 2, so no degenerate 1×1 case. Time complexity is \(O(n^3)\) for the eigen decomposition (typical for dense symmetric eigensolvers), and we extract one column in \(O(n)\). Memory usage is \(O(n^2)\) for the internal matrices of the solver.
