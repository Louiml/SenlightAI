Write a C++ function that takes a positive integer `n` and returns the first eigenvector of the `n x n` matrix of all ones, using the Eigen library. The function should compute the eigendecomposition with `Eigen::EigenSolver<Eigen::MatrixXd>` and extract the eigenvector corresponding to the largest eigenvalue (which is always `n` for this matrix). The function must handle `n >= 1`, and for `n = 1` the eigenvector should be `[1]` after normalization. The return type should be `Eigen::VectorXd` representing the normalized eigenvector (unit length). If the computation fails (e.g., matrix is not diagonalizable, which it is not the case for this symmetric matrix), you may assume it succeeds, but still check `es.info()` and throw `std::runtime_error` on failure.
// The matrix of all ones is symmetric and has rank 1. Its eigenvalues are `n` (with multiplicity 1) and `0` (with multiplicity `n-1`). The eigenvector corresponding to eigenvalue `n` is the all-ones vector `[1,1,...,1]^T`, which after normalization becomes `(1/√n, 1/√n, ..., 1/√n)`. However, the task asks to compute it via the EigenSolver, not analytically. The `EigenSolver` for a symmetric matrix returns real eigenvalues and orthogonal eigenvectors. The eigenvectors are stored as columns of the matrix returned by `es.eigenvectors()`. The eigenvalue vector `es.eigenvalues()` contains the eigenvalues in descending order (for symmetric matrices, Eigen sorts them in increasing order? Actually for `EigenSolver`, eigenvalues are not guaranteed sorted, but for symmetric matrices they are sorted in increasing order? We must find the index of the eigenvalue equal to `n`. We can loop through the eigenvalues, find the index where the real part is approximately `n` (within a tolerance), then return that column. For safety, use `es.eigenvalues().imag().norm()` to ensure all imaginary parts are zero (they are for symmetric matrices). Time complexity is O(n^3) due to eigendecomposition; space O(n^2). Edge cases: n=1 works (eigenvalue 1, eigenvector [1]). If eigenvalue not found, throw.
#include <Eigen/Dense>
#include <stdexcept>
#include <cmath>

// Returns the eigenvector corresponding to the largest eigenvalue of an n x n all-ones matrix.
// The input n must be a positive integer. The returned vector is normalized to unit length.
Eigen::VectorXd allOnesEigenvector(int n) {
    if (n <= 0) {
        throw std::invalid_argument("n must be positive");
    }

    Eigen::MatrixXd ones = Eigen::MatrixXd::Ones(n, n);
    Eigen::EigenSolver<Eigen::MatrixXd> es(ones);
    if (es.info() != Eigen::Success) {
        throw std::runtime_error("Eigendecomposition failed");
    }

    // For a symmetric matrix, eigenvalues are real, but check and find the largest one.
    // The all-ones matrix has eigenvalue n (largest) and 0 (multiplicity n-1).
    const double tolerance = 1e-12;
    double largestEigenvalue = -std::numeric_limits<double>::infinity();
    int largestIndex = -1;
    for (int i = 0; i < n; ++i) {
        // Real part is the eigenvalue for symmetric matrices.
        double lambda = es.eigenvalues()[i].real();
        // Ensure imaginary part is negligible for symmetric input.
        if (std::abs(es.eigenvalues()[i].imag()) > tolerance) {
            throw std::runtime_error("Unexpected complex eigenvalue");
        }
        if (lambda > largestEigenvalue) {
            largestEigenvalue = lambda;
            largestIndex = i;
        }
    }

    if (largestIndex == -1) {
        throw std::runtime_error("No eigenvalues found");
    }

    Eigen::VectorXd vec = es.eigenvectors().col(largestIndex).real();
    // Normalize to unit length (though Eigen already returns orthonormal eigenvectors).
    vec.normalize();
    return vec;
}
#include <cassert>
#include <cmath>
#include <Eigen/Dense>

// The tested function is declared above (or included here).
int main() {
    // n=1: matrix [1], eigenvalue 1, eigenvector [1] (normalized is still [1]).
    Eigen::VectorXd v1 = allOnesEigenvector(1);
    assert(v1.size() == 1);
    assert(std::abs(v1[0] - 1.0) < 1e-12);

    // n=2: eigenvalue 2, eigenvector [1/√2, 1/√2] (sign may be opposite, so compare absolute).
    Eigen::VectorXd v2 = allOnesEigenvector(2);
    assert(v2.size() == 2);
    double expected2 = 1.0 / std::sqrt(2.0);
    assert(std::abs(std::abs(v2[0]) - expected2) < 1e-12);
    assert(std::abs(std::abs(v2[1]) - expected2) < 1e-12);
    // Check unit norm
    assert(std::abs(v2.norm() - 1.0) < 1e-12);

    // n=3: all components have absolute value 1/√3.
    Eigen::VectorXd v3 = allOnesEigenvector(3);
    assert(v3.size() == 3);
    double expected3 = 1.0 / std::sqrt(3.0);
    for (int i = 0; i < 3; ++i) {
        assert(std::abs(std::abs(v3[i]) - expected3) < 1e-12);
    }

    // n=5: verify it's an eigenvector: A*v = n*v.
    int n = 5;
    Eigen::VectorXd v5 = allOnesEigenvector(n);
    Eigen::MatrixXd ones5 = Eigen::MatrixXd::Ones(n, n);
    Eigen::VectorXd left = ones5 * v5;
    Eigen::VectorXd right = double(n) * v5;
    assert((left - right).norm() < 1e-10);

    // n=10: check unit norm and that it satisfies A*v = n*v.
    n = 10;
    Eigen::VectorXd v10 = allOnesEigenvector(n);
    assert(std::abs(v10.norm() - 1.0) < 1e-12);
    Eigen::MatrixXd ones10 = Eigen::MatrixXd::Ones(n, n);
    assert((ones10 * v10 - double(n) * v10).norm() < 1e-8);

    // n=4: verify all components equal in absolute value.
    n = 4;
    Eigen::VectorXd v4 = allOnesEigenvector(n);
    double absVal = std::abs(v4[0]);
    for (int i = 1; i < n; ++i) {
        assert(std::abs(std::abs(v4[i]) - absVal) < 1e-12);
    }
    assert(std::abs(absVal - 0.5) < 1e-12); // 1/√4 = 0.5
}
