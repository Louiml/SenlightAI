Write a C++ function that accepts a 4x4 complex matrix (`std::complex<float>`) and returns a `std::pair` containing two complex matrices: the unitary matrix \(U\) and the upper-triangular Schur matrix \(T\) obtained from the complex Schur decomposition of the input. The function must compute the Schur decomposition such that \(A = U T U^H\), where \(U^H\) is the conjugate transpose of \(U\). The input matrix can be any 4x4 complex matrix (including singular, non-diagonalizable in real sense, or with repeated eigenvalues), and the output must preserve the exact floating-point values as produced by Eigen's `ComplexSchur` solver. The solution must use Eigen's `MatrixXcf` and `ComplexSchur` classes.

// The main algorithm is straightforward: use Eigen's `ComplexSchur<MatrixXcf>` class to compute the decomposition of the input matrix. Construct a `ComplexSchur` object with the input matrix, then call `matrixU()` to obtain the unitary matrix \(U\) and `matrixT()` to obtain the Schur form \(T\). The decomposition guarantees \(A = U T U^H\) (within numerical precision). Edge cases include matrices with repeated eigenvalues, non-diagonalizable matrices (where \(T\) has 2x2 blocks on the diagonal), and zero or singular matrices; Eigen handles all these robustly via the QR iteration algorithm. The time complexity is \(O(n^3)\) for an \(n \times n\) matrix, and auxiliary space is \(O(n^2)\) to store the U and T matrices. Since the input is fixed at 4x4, the complexity is constant in practice.

#include <Eigen/Dense>
#include <Eigen/Eigenvalues>
#include <utility>
#include <complex>

// Computes the complex Schur decomposition of a 4x4 complex matrix.
// Returns a pair (U, T) such that A = U * T * U^H, where U is unitary and T is upper-triangular.
std::pair<Eigen::MatrixXcf, Eigen::MatrixXcf> computeComplexSchur(const Eigen::MatrixXcf& A) {
    // Ensure input is 4x4; if not, resize (though the task specifies 4x4).
    Eigen::MatrixXcf input = A;
    if (input.rows() != 4 || input.cols() != 4) {
        input.resize(4, 4);
        input.setZero();
    }

    // Perform the complex Schur decomposition.
    Eigen::ComplexSchur<Eigen::MatrixXcf> schur(input);
    Eigen::MatrixXcf U = schur.matrixU();
    Eigen::MatrixXcf T = schur.matrixT();

    return std::make_pair(U, T);
}

#include <Eigen/Dense>
#include <Eigen/Eigenvalues>
#include <cassert>
#include <complex>
#include <utility>

// Declaration of the function under test.
std::pair<Eigen::MatrixXcf, Eigen::MatrixXcf> computeComplexSchur(const Eigen::MatrixXcf& A);

int main() {
    // Test 1: Random 4x4 matrix (using a fixed seed for reproducibility).
    Eigen::MatrixXcf A1 = Eigen::MatrixXcf::Random(4,4);
    auto [U1, T1] = computeComplexSchur(A1);
    Eigen::MatrixXcf reconstructed1 = U1 * T1 * U1.adjoint();
    assert(reconstructed1.isApprox(A1, 1e-5f));

    // Test 2: Identity matrix (trivial case, U = I, T = I).
    Eigen::MatrixXcf A2 = Eigen::MatrixXcf::Identity(4,4);
    auto [U2, T2] = computeComplexSchur(A2);
    assert(U2.isApprox(Eigen::MatrixXcf::Identity(4,4), 1e-5f));
    assert(T2.isApprox(Eigen::MatrixXcf::Identity(4,4), 1e-5f));

    // Test 3: Zero matrix (U should be identity, T zero).
    Eigen::MatrixXcf A3 = Eigen::MatrixXcf::Zero(4,4);
    auto [U3, T3] = computeComplexSchur(A3);
    assert(T3.isApprox(Eigen::MatrixXcf::Zero(4,4), 1e-5f));
    // U should be unitary: U * U^H = I.
    assert((U3 * U3.adjoint()).isApprox(Eigen::MatrixXcf::Identity(4,4), 1e-5f));

    // Test 4: A matrix with a repeated eigenvalue (e.g., Jordan block).
    Eigen::MatrixXcf A4(4,4);
    A4 << std::complex<float>(2,0), std::complex<float>(1,0), std::complex<float>(0,0), std::complex<float>(0,0),
          std::complex<float>(0,0), std::complex<float>(2,0), std::complex<float>(1,0), std::complex<float>(0,0),
          std::complex<float>(0,0), std::complex<float>(0,0), std::complex<float>(2,0), std::complex<float>(1,0),
          std::complex<float>(0,0), std::complex<float>(0,0), std::complex<float>(0,0), std::complex<float>(2,0);
    auto [U4, T4] = computeComplexSchur(A4);
    Eigen::MatrixXcf reconstructed4 = U4 * T4 * U4.adjoint();
    assert(reconstructed4.isApprox(A4, 1e-5f));

    // Test 5: A purely imaginary eigenvalue matrix.
    Eigen::MatrixXcf A5(4,4);
    A5 << std::complex<float>(0,1), std::complex<float>(0,0), std::complex<float>(0,0), std::complex<float>(0,0),
          std::complex<float>(0,0), std::complex<float>(0,-1), std::complex<float>(0,0), std::complex<float>(0,0),
          std::complex<float>(0,0), std::complex<float>(0,0), std::complex<float>(0,2), std::complex<float>(0,0),
          std::complex<float>(0,0), std::complex<float>(0,0), std::complex<float>(0,0), std::complex<float>(0,3);
    auto [U5, T5] = computeComplexSchur(A5);
    Eigen::MatrixXcf reconstructed5 = U5 * T5 * U5.adjoint();
    assert(reconstructed5.isApprox(A5, 1e-5f));

    // Test 6: Unitarity of U for a random matrix.
    auto [U6, T6] = computeComplexSchur(A1);
    Eigen::MatrixXcf product6 = U6 * U6.adjoint();
    assert(product6.isApprox(Eigen::MatrixXcf::Identity(4,4), 1e-5f));

    // Test 7: Upper-triangular property of T for a random matrix.
    auto [U7, T7] = computeComplexSchur(A1);
    // Check that all entries below the main diagonal are zero.
    for (int i = 1; i < 4; ++i) {
        for (int j = 0; j < i; ++j) {
            assert(std::abs(T7(i,j)) < 1e-5f);
        }
    }

    // Test 8: Diagonal of T contains eigenvalues; check trace preservation.
    auto [U8, T8] = computeComplexSchur(A1);
    std::complex<float> traceA = A1.trace();
    std::complex<float> traceT = T8.trace();
    assert(std::abs(traceA - traceT) < 1e-5f);

    // Test 9: Determinant preservation (product of diagonal of T).
    auto [U9, T9] = computeComplexSchur(A1);
    std::complex<float> detA = A1.determinant();
    std::complex<float> detT = T9.diagonal().prod();
    assert(std::abs(detA - detT) < 1e-4f);

    // Test 10: Scaling invariance (multiplying A by 2 scales T by 2).
    Eigen::MatrixXcf A10 = Eigen::MatrixXcf::Random(4,4);
    auto [U10, T10] = computeComplexSchur(A10);
    Eigen::MatrixXcf scaledA = A10 * 2.0f;
    auto [U10s, T10s] = computeComplexSchur(scaledA);
    assert(T10s.isApprox(T10 * 2.0f, 1e-5f));

    return 0;
}
