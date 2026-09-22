Write a C++ function that takes a square matrix of real numbers of dimension `n` (where `n` is at least 1 and at most 6) and returns a `std::complex<double>` value representing the eigenvalue with the smallest real part (choose the one with the smallest real part; if there is a tie, choose the one with the smallest imaginary part among those tied). The function must use the Eigen library's `EigenSolver` to compute all eigenvalues of the matrix. The function should be named `smallestRealPartEigenvalue` and should accept a constant reference to an `Eigen::MatrixXd`. The function must handle the case where `n = 1` correctly (the only eigenvalue is the matrix's single entry). The matrix is guaranteed to be square but not necessarily symmetric, so eigenvalues may be complex numbers. The function should return the selected eigenvalue as a `std::complex<double>`.

#include <cassert>
#include <complex>
#include <Eigen/Dense>

// Declaration of the function being tested (assume it's in the same translation unit).
std::complex<double> smallestRealPartEigenvalue(const Eigen::MatrixXd& A);

int main() {
    // Test 1: 1x1 matrix
    Eigen::MatrixXd A1(1,1);
    A1(0,0) = 3.5;
    std::complex<double> result1 = smallestRealPartEigenvalue(A1);
    assert(result1 == std::complex<double>(3.5, 0.0));

    // Test 2: 2x2 matrix with real eigenvalues (symmetric)
    Eigen::MatrixXd A2(2,2);
    A2 << 2.0, 1.0,
          1.0, 2.0;
    // Eigenvalues are 1 and 3. Smallest real part is 1.
    std::complex<double> result2 = smallestRealPartEigenvalue(A2);
    assert(std::abs(result2 - std::complex<double>(1.0, 0.0)) < 1e-9);

    // Test 3: 2x2 matrix with complex eigenvalues
    Eigen::MatrixXd A3(2,2);
    A3 << 0.0, -1.0,
          1.0,  0.0;
    // Eigenvalues are i and -i. Real parts both 0; smallest imaginary part is -1.
    std::complex<double> result3 = smallestRealPartEigenvalue(A3);
    assert(std::abs(result3.real() - 0.0) < 1e-9);
    assert(std::abs(result3.imag() - (-1.0)) < 1e-9);

    // Test 4: 3x3 diagonal matrix
    Eigen::MatrixXd A4 = Eigen::MatrixXd::Zero(3,3);
    A4(0,0) = -2.0;
    A4(1,1) = 5.0;
    A4(2,2) = -2.0;
    // Eigenvalues: -2, -2, 5. Smallest real part is -2 (both -2 tie; pick first, imag 0).
    std::complex<double> result4 = smallestRealPartEigenvalue(A4);
    assert(std::abs(result4 - std::complex<double>(-2.0, 0.0)) < 1e-9);

    // Test 5: 4x4 random symmetric matrix (guaranteed real eigenvalues)
    Eigen::MatrixXd A5(4,4);
    A5 << 1.0, 0.5, 0.2, 0.1,
          0.5, 2.0, 0.3, 0.4,
          0.2, 0.3, 3.0, 0.6,
          0.1, 0.4, 0.6, 4.0;
    std::complex<double> result5 = smallestRealPartEigenvalue(A5);
    // By construction, smallest eigenvalue is less than or equal to 1 (since first diag is 1
    // and off-diagonal positive, but we can just check it's real and less than or equal to 1).
    assert(std::abs(result5.imag()) < 1e-9);
    assert(result5.real() <= 1.0 + 1e-9);

    // Test 6: 6x6 matrix from the snippet (fixed values to make deterministic)
    Eigen::MatrixXd A6(6,6);
    A6 << 1, 2, 3, 4, 5, 6,
          6, 5, 4, 3, 2, 1,
          1, 0, 0, 0, 0, 0,
          0, 1, 0, 0, 0, 0,
          0, 0, 1, 0, 0, 0,
          0, 0, 0, 1, 0, 0;
    // Just ensure the function runs and returns a complex number.
    std::complex<double> result6 = smallestRealPartEigenvalue(A6);
    (void)result6; // place to assert if we had known eigenvalues; here just checks no crash

    // Test 7: Negative diagonal matrix
    Eigen::MatrixXd A7(2,2);
    A7 << -10.0, 0.0,
          0.0, -5.0;
    std::complex<double> result7 = smallestRealPartEigenvalue(A7);
    assert(std::abs(result7 - std::complex<double>(-10.0, 0.0)) < 1e-9);

    // Test 8: Matrices with repeated eigenvalues
    Eigen::MatrixXd A8 = Eigen::MatrixXd::Identity(3,3);
    // All eigenvalues are 1; smallest real part is 1.
    std::complex<double> result8 = smallestRealPartEigenvalue(A8);
    assert(std::abs(result8 - std::complex<double>(1.0, 0.0)) < 1e-9);

    // Test 9: Matrix with a zero eigenvalue
    Eigen::MatrixXd A9(2,2);
    A9 << 0.0, 0.0,
          0.0, 3.0;
    std::complex<double> result9 = smallestRealPartEigenvalue(A9);
    assert(std::abs(result9 - std::complex<double>(0.0, 0.0)) < 1e-9);

    // Test 10: Non-symmetric with known complex pair
    Eigen::MatrixXd A10(2,2);
    A10 << 1.0, -4.0,
          1.0,  1.0;
    // Eigenvalues: 1 ± 2i. Smallest real part both 1, pick imaginary -2.
    std::complex<double> result10 = smallestRealPartEigenvalue(A10);
    assert(std::abs(result10.real() - 1.0) < 1e-9);
    assert(std::abs(result10.imag() - (-2.0)) < 1e-9);

    return 0;
}

#include <Eigen/Dense>
#include <complex>
#include <algorithm>

// Return the eigenvalue with the smallest real part; if tie, smallest imaginary part.
// Uses Eigen's EigenSolver for general real matrices.
std::complex<double> smallestRealPartEigenvalue(const Eigen::MatrixXd& A) {
    // EigenSolver computes all eigenvalues of a general real matrix.
    Eigen::EigenSolver<Eigen::MatrixXd> es(A);
    const Eigen::VectorXcd& eigenvalues = es.eigenvalues();

    // Initialize with the first eigenvalue.
    std::complex<double> best = eigenvalues[0];
    for (int i = 1; i < eigenvalues.size(); ++i) {
        const std::complex<double>& current = eigenvalues[i];
        // Compare real parts; if equal, compare imaginary parts.
        if (current.real() < best.real() ||
            (current.real() == best.real() && current.imag() < best.imag())) {
            best = current;
        }
    }
    return best;
}

// The solution uses Eigen's `EigenSolver<MatrixXd>` which computes all eigenvalues (and optionally eigenvectors) of a general real square matrix. Since the matrix can have complex eigenvalues, the solver returns them as `std::complex<double>` values. The main algorithm is:
// 1. Construct an `EigenSolver<MatrixXd>` object with the input matrix.
// 2. Access the eigenvalues via `eigenvalues()` which returns a `VectorXcd`.
// 3. Iterate over all eigenvalues, tracking the one with the smallest real part; if two eigenvalues have the same real part (within a tolerance, which for our task is exact equality since they come from the same solver), choose the one with the smaller imaginary part.
// 4. Return the chosen eigenvalue as `std::complex<double>`.
//
// Edge cases: 
// - If the matrix is 1x1, there is exactly one eigenvalue, which is the matrix element itself (as a complex with zero imaginary part). The loop handles this naturally.
// - The matrix may be singular or have repeated eigenvalues; the selection rule decides deterministically among ties.
// - The eigenvalues are computed numerically, but for our comparisons we use exact `std::complex<double>` equality as returned by the solver, which is consistent.
//
// Time complexity: `EigenSolver` uses the QR algorithm (or QZ for generalized) which is \(O(n^3)\) for an \(n \times n\) matrix. For \(n \le 6\), this is constant time in practical terms. Space complexity is \(O(n^2)\) for storing the matrix and eigen-decomposition results, but since we only need eigenvalues, we don't store eigenvectors. However, the solver itself allocates \(O(n^2)\) memory internally.
//
// The function should be `const`-correct: the input matrix is passed by `const&`, and the function does not modify it.
