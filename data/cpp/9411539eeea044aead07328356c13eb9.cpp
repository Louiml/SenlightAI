Write a C++ function that takes a square matrix of `std::complex<float>` (represented as `Eigen::MatrixXcf`) and returns the eigenvector corresponding to the eigenvalue with the largest real part (if there is a tie, choose the one with the largest imaginary part). If the matrix is empty (0×0), return an empty vector. Use `Eigen::ComplexEigenSolver` to compute eigenvectors, and ensure the returned eigenvector is normalized to unit norm (Eigen already does this). The function should be named `dominantEigenvector` and accept a const reference to the matrix. Handle edge cases: non-square input, singular matrices, and matrices with repeated eigenvalues.

#include <Eigen/Dense>
#include <cassert>
#include <complex>

// Free function declaration (usually in a header).
Eigen::VectorXcf dominantEigenvector(const Eigen::MatrixXcf& A);

int main() {
    // Test 1: Known matrix with distinct eigenvalues (two eigenvectors).
    // Matrix [[1, 0], [0, 2]] -> eigenvalues 1 and 2, eigenvector for 2 is [0,1].
    Eigen::MatrixXcf M1(2, 2);
    M1 << std::complex<float>(1,0), std::complex<float>(0,0),
          std::complex<float>(0,0), std::complex<float>(2,0);
    Eigen::VectorXcf v1 = dominantEigenvector(M1);
    assert(v1.size() == 2);
    // Check it's close to [0,1] (allowing numerical tolerance).
    assert(std::abs(v1(0) - std::complex<float>(0,0)) < 1e-6f);
    assert(std::abs(v1(1) - std::complex<float>(1,0)) < 1e-6f);

    // Test 2: 3x3 matrix of ones (as in snippet). Eigenvalues: 3, 0, 0.
    // Eigenvector for eigenvalue 3 is normalized [1/√3, 1/√3, 1/√3].
    Eigen::MatrixXcf M2 = Eigen::MatrixXcf::Ones(3, 3);
    Eigen::VectorXcf v2 = dominantEigenvector(M2);
    assert(v2.size() == 3);
    // All components should be equal (approximately) and sum of squares ≈ 1.
    float sum_sq = (v2(0).real() * v2(0).real() + v2(0).imag() * v2(0).imag()) +
                   (v2(1).real() * v2(1).real() + v2(1).imag() * v2(1).imag()) +
                   (v2(2).real() * v2(2).real() + v2(2).imag() * v2(2).imag());
    assert(std::abs(sum_sq - 1.0f) < 1e-4f);
    assert(std::abs(v2(0) - v2(1)) < 1e-4f);
    assert(std::abs(v2(1) - v2(2)) < 1e-4f);
    assert(v2(0).real() > 0); // positive direction (Eigen picks a sign, we check consistent)

    // Test 3: Empty matrix returns empty vector.
    Eigen::MatrixXcf M3(0, 0);
    Eigen::VectorXcf v3 = dominantEigenvector(M3);
    assert(v3.size() == 0);

    // Test 4: Non-square matrix returns empty.
    Eigen::MatrixXcf M4(2, 3);
    M4.setZero();
    Eigen::VectorXcf v4 = dominantEigenvector(M4);
    assert(v4.size() == 0);

    // Test 5: Diagonal matrix with complex eigenvalues.
    // Eigenvalues: 1+2i, 2-1i => dominant real part is 2 (eigenvalue 2-1i, eigenvector [0,1]).
    Eigen::MatrixXcf M5(2, 2);
    M5 << std::complex<float>(1,2), std::complex<float>(0,0),
          std::complex<float>(0,0), std::complex<float>(2,-1);
    Eigen::VectorXcf v5 = dominantEigenvector(M5);
    assert(std::abs(v5(0) - std::complex<float>(0,0)) < 1e-6f);
    assert(std::abs(v5(1) - std::complex<float>(1,0)) < 1e-6f);

    // Test 6: Tie on real part, larger imaginary part wins.
    // Matrix diag(1+5i, 1+3i) -> both real=1, first imag=5 > 3, choose index 0.
    Eigen::MatrixXcf M6(2, 2);
    M6 << std::complex<float>(1,5), std::complex<float>(0,0),
          std::complex<float>(0,0), std::complex<float>(1,3);
    Eigen::VectorXcf v6 = dominantEigenvector(M6);
    assert(std::abs(v6(0) - std::complex<float>(1,0)) < 1e-6f);
    assert(std::abs(v6(1) - std::complex<float>(0,0)) < 1e-6f);

    // Test 7: Zero matrix – all eigenvalues 0, tie → but our rule picks first (imag=0 all).
    // Eigenvectors are identity columns; any is fine, but we expect first column [1,0].
    Eigen::MatrixXcf M7(2, 2);
    M7.setZero();
    Eigen::VectorXcf v7 = dominantEigenvector(M7);
    // Since all eigenvalues equal, best_idx=0, so eigenvector is first column (normalized to [1,0]).
    assert(std::abs(v7(0) - std::complex<float>(1,0)) < 1e-6f);
    assert(std::abs(v7(1) - std::complex<float>(0,0)) < 1e-6f);

    return 0;
}

#include <Eigen/Dense>
#include <complex>
#include <vector>

/**
 * @brief Returns the eigenvector corresponding to the eigenvalue with the largest
 *        real part (and if tie, largest imaginary part) of a complex square matrix.
 *
 * The matrix must be square and non-empty (except 0×0 is allowed, returns empty vector).
 * Uses Eigen's ComplexEigenSolver. The returned vector is normalized to unit norm
 * (Eigen's eigenvectors are already normalized). If the matrix is not square or empty,
 * returns an empty vector.
 *
 * @param A Square matrix of complex floats.
 * @return Eigen::VectorXcf The dominant eigenvector, or empty if invalid.
 */
Eigen::VectorXcf dominantEigenvector(const Eigen::MatrixXcf& A) {
    // Validate input: must be square, but allow 0×0 to return empty.
    if (A.rows() != A.cols()) {
        return Eigen::VectorXcf();
    }
    if (A.rows() == 0) {
        return Eigen::VectorXcf();
    }

    // Compute eigenvalues and eigenvectors.
    Eigen::ComplexEigenSolver<Eigen::MatrixXcf> solver(A);
    // solver.info() could be checked, but for simplicity assume success.

    const auto& eigenvalues = solver.eigenvalues();
    const auto& eigenvectors = solver.eigenvectors();

    // Find index of eigenvalue with largest (real, then imaginary) lexicographically.
    int best_idx = 0;
    for (int i = 1; i < eigenvalues.size(); ++i) {
        const auto& best = eigenvalues[best_idx];
        const auto& current = eigenvalues[i];
        // Compare real parts first, then imaginary parts.
        if (current.real() > best.real() ||
            (current.real() == best.real() && current.imag() > best.imag())) {
            best_idx = i;
        }
    }

    // Return the corresponding eigenvector.
    return eigenvectors.col(best_idx);
}

// The solution uses `Eigen::ComplexEigenSolver<MatrixXcf>` to compute eigenvalues and eigenvectors. For a given matrix `A`, the solver provides `eigenvalues()` (a column vector of complex values) and `eigenvectors()` (a matrix whose columns correspond to eigenvalues). We iterate over all eigenvalues, comparing by lexicographic order: first by real part, then by imaginary part (to break ties). We track the index of the largest. After finding the index, we return `eigenvectors().col(index)`. If the matrix is empty (0×0), we return an empty vector (size 0). If the input is not square, we can either assert or return an empty vector; the task says "square matrix", but to be robust, we can check and return empty if `rows() != cols()`. Also, for a 1×1 matrix, the eigenvalue is just the single element, and eigenvector is [1] (normalized). Time complexity: Eigen's complex eigensolver for general matrices runs in O(n³) time. Space complexity: O(n²) for storing eigenvectors and eigenvalues. Edge cases: zero matrix – all eigenvalues are 0, the eigenvectors are the standard basis; we choose the first one per our tie-break rule. Repeated eigenvalues – any eigenvector in the corresponding eigenspace is valid; Eigen returns an orthonormal basis, so we simply pick the one at the index of the chosen eigenvalue.
