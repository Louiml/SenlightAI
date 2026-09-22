/*
Write a C++ function that takes a square floating-point matrix of even dimension (size at least 2) and returns the upper Hessenberg matrix H of its real Schur-like Hessenberg decomposition (A = Q H Q^T, where Q is orthogonal and H is upper Hessenberg), using Eigen's `HessenbergDecomposition` class. The function must handle both real and complex matrices via a template, but for simplicity restrict the test to `float`. The function should accept a `const` reference to the matrix to avoid modification, and should return the matrix H by value. The input matrix is guaranteed to be square and have even dimension (2, 4, 6, ...). The function must be reusable: it should not modify the input matrix and should not rely on any global state. Edge cases: dimension 2 (the smallest even size) and matrices with repeated eigenvalues or zero entries must be handled correctly.
*/
#include <Eigen/Dense>
#include <stdexcept>

// Compute the upper Hessenberg matrix H of the Hessenberg decomposition of A.
// Precondition: A must be a square matrix with even dimension >= 2.
// Returns H such that A = Q H Q^T, where Q is orthogonal.
Eigen::MatrixXf computeHessenbergH(const Eigen::MatrixXf& A) {
    if (A.rows() != A.cols() || A.rows() < 2 || A.rows() % 2 != 0) {
        throw std::invalid_argument("Input must be square with even dimension >= 2");
    }
    Eigen::HessenbergDecomposition<Eigen::MatrixXf> hd(A.rows());
    hd.compute(A);
    return hd.matrixH();
}
#include <Eigen/Dense>
#include <cassert>
#include <cmath>

// Helper to compare matrices approximately.
bool matricesApproxEqual(const Eigen::MatrixXf& a, const Eigen::MatrixXf& b, float tol = 1e-5f) {
    return (a - b).cwiseAbs().maxCoeff() < tol;
}

int main() {
    // Test 1: dimension 2, zero matrix
    Eigen::MatrixXf A1 = Eigen::MatrixXf::Zero(2,2);
    Eigen::MatrixXf H1 = computeHessenbergH(A1);
    assert(matricesApproxEqual(H1, Eigen::MatrixXf::Zero(2,2)));

    // Test 2: dimension 2, random matrix
    Eigen::MatrixXf A2(2,2);
    A2 << 1.0f, 2.0f, 3.0f, 4.0f;
    Eigen::MatrixXf H2 = computeHessenbergH(A2);
    assert(H2.rows() == 2 && H2.cols() == 2);
    assert(std::abs(H2(1,0)) < 1e-5f); // upper Hessenberg: sub-diagonal zero for 2x2

    // Test 3: dimension 4, identity matrix (already Hessenberg)
    Eigen::MatrixXf A3 = Eigen::MatrixXf::Identity(4,4);
    Eigen::MatrixXf H3 = computeHessenbergH(A3);
    assert(matricesApproxEqual(H3, Eigen::MatrixXf::Identity(4,4)));

    // Test 4: dimension 4, random matrix, verify A ≈ Q H Q^T
    Eigen::MatrixXf A4 = Eigen::MatrixXf::Random(4,4);
    Eigen::HessenbergDecomposition<Eigen::MatrixXf> hd_check(A4);
    Eigen::MatrixXf Q = hd_check.matrixQ();
    Eigen::MatrixXf H4 = computeHessenbergH(A4);
    Eigen::MatrixXf reconstructed = Q * H4 * Q.transpose();
    assert(matricesApproxEqual(A4, reconstructed, 1e-4f));

    // Test 5: dimension 6, all ones matrix
    Eigen::MatrixXf A5 = Eigen::MatrixXf::Ones(6,6);
    Eigen::MatrixXf H5 = computeHessenbergH(A5);
    assert(H5.rows() == 6 && H5.cols() == 6);
    // Check upper Hessenberg: entries below first subdiagonal are zero
    for (int i = 2; i < 6; ++i) {
        for (int j = 0; j < i - 1; ++j) {
            assert(std::abs(H5(i,j)) < 1e-5f);
        }
    }

    // Test 6: dimension 6, random matrix, verify H is upper Hessenberg
    Eigen::MatrixXf A6 = Eigen::MatrixXf::Random(6,6);
    Eigen::MatrixXf H6 = computeHessenbergH(A6);
    for (int i = 2; i < 6; ++i) {
        for (int j = 0; j < i - 1; ++j) {
            assert(std::abs(H6(i,j)) < 1e-5f);
        }
    }

    // Test 7: dimension 2, matrix with repeated eigenvalues
    Eigen::MatrixXf A7(2,2);
    A7 << 5.0f, 0.0f, 0.0f, 5.0f;
    Eigen::MatrixXf H7 = computeHessenbergH(A7);
    assert(matricesApproxEqual(H7, A7));

    // Test 8: dimension 4, symmetric matrix (H is tridiagonal)
    Eigen::MatrixXf A8 = Eigen::MatrixXf::Random(4,4);
    A8 = (A8 + A8.transpose()) / 2.0f;
    Eigen::MatrixXf H8 = computeHessenbergH(A8);
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (j > i + 1 || i > j + 1) {
                assert(std::abs(H8(i,j)) < 1e-5f);
            }
        }
    }

    return 0;
}
// The solution leverages Eigen's `HessenbergDecomposition` class, which reduces a square matrix A to upper Hessenberg form H via an orthogonal similarity transformation: A = Q H Q^T (or Q H Q^H for complex, but we target real float). The main algorithm: create a `HessenbergDecomposition` object for the matrix type (e.g., `MatrixXf`), call `compute(A)` to perform the decomposition, then return `hd.matrixH()` (the upper Hessenberg matrix). Important edge cases: dimension 2 produces a 2x2 Hessenberg matrix that is already upper triangular (since a 2x2 matrix is trivially upper Hessenberg), so the decomposition should still work. Zero entries or repeated eigenvalues do not cause issues because Eigen's Householder-based algorithm is numerically stable and does not assume distinct eigenvalues. The function must be `const`-correct: take `const MatrixXf& A` and return `MatrixXf` by value. Time complexity: the standard Hessenberg reduction via Householder reflections is O(n^3) for an n×n matrix, and space complexity is O(n^2) for storing the matrix and the decomposition objects. The function is independent and has no side effects.
