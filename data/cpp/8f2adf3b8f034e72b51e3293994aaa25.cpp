/*
Write a C++ function that, given a symmetric matrix `A` (represented as an `Eigen::MatrixXf`), returns the tridiagonal matrix `T` obtained from the tridiagonalization decomposition of `A` (i.e., the result of `Eigen::Tridiagonalization<MatrixXf>::matrixT()`). The function must be generic in the sense that it works for any square symmetric matrix size (≥1), and it must be reusable: if called multiple times with different matrices, it should return the correct `T` for each call without retaining state from previous calls. The function should accept the input matrix by const reference, return `Eigen::MatrixXf` by value, and handle the edge case where the input is not perfectly symmetric by symmetrizing it (using `(A + A.transpose())/2`) before decomposition. Do not include a `main` function in your solution code.
*/
#include <Eigen/Dense>
#include <stdexcept>

// Returns the tridiagonal matrix T from the tridiagonalization decomposition of a symmetric matrix.
// If the input is not symmetric, it is symmetrized as (A + A^T)/2.
// The function is reentrant and safe for repeated calls with different matrices.
Eigen::MatrixXf computeTridiagonalT(const Eigen::MatrixXf& A) {
    if (A.rows() != A.cols()) {
        throw std::invalid_argument("Input matrix must be square.");
    }
    if (A.rows() == 0) {
        return Eigen::MatrixXf(0, 0);
    }
    
    // Symmetrize to handle near-symmetric or non-symmetric inputs.
    Eigen::MatrixXf symmetricA = (A + A.transpose()) * 0.5f;
    
    Eigen::Tridiagonalization<Eigen::MatrixXf> tri;
    tri.compute(symmetricA);
    return tri.matrixT();
}
#include <cassert>
#include <cmath>
#include <Eigen/Dense>

int main() {
    // 1x1 case
    Eigen::MatrixXf A1(1,1);
    A1 << 5.0f;
    Eigen::MatrixXf T1 = computeTridiagonalT(A1);
    assert(T1.rows() == 1 && T1.cols() == 1);
    assert(std::abs(T1(0,0) - 5.0f) < 1e-5);

    // Diagonal matrix: tridiagonal is itself
    Eigen::MatrixXf A2(3,3);
    A2 << 1.0f, 0.0f, 0.0f,
          0.0f, 2.0f, 0.0f,
          0.0f, 0.0f, 3.0f;
    Eigen::MatrixXf T2 = computeTridiagonalT(A2);
    assert(T2.isApprox(A2, 1e-5));

    // Random symmetric matrix: verify T is tridiagonal and has same eigenvalues
    Eigen::MatrixXf R = Eigen::MatrixXf::Random(4,4);
    Eigen::MatrixXf A3 = R + R.transpose();
    Eigen::MatrixXf T3 = computeTridiagonalT(A3);
    // Check tridiagonal structure: only main, super, and sub diagonals non-zero
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (std::abs(i - j) > 1) {
                assert(std::abs(T3(i,j)) < 1e-5);
            }
        }
    }
    // Eigenvalues of A3 and T3 should be the same (up to sign for eigenvectors)
    Eigen::SelfAdjointEigenSolver<Eigen::MatrixXf> eigA(A3);
    Eigen::SelfAdjointEigenSolver<Eigen::MatrixXf> eigT(T3);
    assert(eigA.eigenvalues().isApprox(eigT.eigenvalues(), 1e-4));

    // Non-symmetric input: symmetrize internally
    Eigen::MatrixXf A4(2,2);
    A4 << 1.0f, 2.0f,
          3.0f, 4.0f;
    // Symmetrized = (A + A^T)/2 = [[1, 2.5], [2.5, 4]]
    Eigen::MatrixXf T4 = computeTridiagonalT(A4);
    Eigen::MatrixXf expected(2,2);
    expected << 1.0f, 2.5f,
                2.5f, 4.0f;
    assert(T4.isApprox(expected, 1e-5));

    // Reusability: call with different matrices, results independent
    Eigen::MatrixXf T_again = computeTridiagonalT(A1);
    assert(T_again.isApprox(T1, 1e-6));

    // 2x2 random symmetric: tridiagonal is full 2x2 (since off-diagonals are adjacent)
    Eigen::MatrixXf A5(2,2);
    A5 << 1.0f, -2.0f,
          -2.0f, 3.0f;
    Eigen::MatrixXf T5 = computeTridiagonalT(A5);
    assert(T5.isApprox(A5, 1e-5)); // For 2x2, tridiagonal is the same as original symmetric matrix

    return 0;
}
// The core task is to wrap the Eigen library's tridiagonalization functionality. The main algorithm is straightforward: construct an `Eigen::Tridiagonalization<MatrixXf>` object, call `compute()` on the symmetrized input, and return `matrixT()`. Edge cases include: (1) a 1x1 matrix, where the tridiagonal form is just the single element; (2) non-symmetric input, which must be explicitly symmetrized because `Tridiagonalization::compute` expects a symmetric matrix (in debug mode, Eigen asserts symmetry; in release mode it may produce garbage). The time complexity is dominated by the Householder-based tridiagonalization, which is \(O(n^3)\) for an \(n \times n\) matrix, with \(O(n)\) auxiliary storage beyond the input. The symmetrization step is \(O(n^2)\). The function is stateless, so calling it repeatedly with different matrices is safe and independent.
