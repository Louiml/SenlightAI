// Write a C++ function `solveSymmetricEigenproblem` that takes a symmetric matrix `A` (stored as a `std::vector<double>` in column-major order, with leading dimension `lda`), its size `n`, and a boolean `computeVectors`. The function must compute the eigenvalues of `A` (and optionally the eigenvectors) using the `Eigen` library's `SelfAdjointEigenSolver`. It must handle three special cases: (1) invalid input (negative `n`, `lda` less than `max(1,n)`, or matrix not symmetric) by returning an error code `-1` and leaving outputs unchanged, (2) empty matrix (`n==0`) by returning `0` and leaving outputs unchanged, (3) non-convergence by setting eigenvalues to zero and (if requested) eigenvectors to the identity matrix, returning `0`. On success, it must write the eigenvalues into a provided `std::vector<double>& eigenvalues` (size at least `n`) in ascending order, and if `computeVectors` is true, write the eigenvectors (each as a column) into a provided `std::vector<double>& eigenvectors` (size `n*n`, column-major) and return `0`. The function must be `const`-correct with respect to the input matrix and must not allocate more than `O(n^2)` extra space.
The solution uses the `Eigen::SelfAdjointEigenSolver` on an `Eigen::Map<const Eigen::MatrixXd>` to avoid copying the input matrix. First, we validate the inputs: `n>=0`, `lda>=max(1,n)`, and that the matrix is symmetric (check `A[i*lda+j] == A[j*lda+i]` for all `i>j`, using a small tolerance like `1e-12`). If any check fails, return `-1`. If `n==0`, return `0` without modifying outputs. Then we construct the solver, passing `computeVectors ? Eigen::ComputeEigenvectors : Eigen::EigenvaluesOnly`. If `eig.info() != Eigen::Success` (non-convergence), set all eigenvalues to zero, and if `computeVectors`, set the eigenvector matrix to the identity (by filling `eigenvectors` with zeros and setting diagonal entries to 1), then return `0`. Otherwise, copy `eig.eigenvalues()` into `eigenvalues`, and if `computeVectors`, copy `eig.eigenvectors()` (which is a column-major `n x n` matrix) into `eigenvectors`. Time complexity is \(O(n^3)\) for the eigen decomposition (standard for symmetric matrices), and space complexity is \(O(n^2)\) for the solver's internal storage and the output eigenvector matrix (if requested). The input matrix is not modified.
#include <Eigen/Eigenvalues>
#include <vector>
#include <cmath>

/**
 * Computes eigenvalues (and optionally eigenvectors) of a symmetric matrix.
 *
 * @param A          Input symmetric matrix stored in column-major order (size n*n with stride lda).
 * @param n          Matrix dimension (must be >= 0).
 * @param lda        Leading dimension of A (must be >= max(1,n)).
 * @param computeVectors If true, compute eigenvectors; otherwise only eigenvalues.
 * @param eigenvalues Output eigenvalues in ascending order (size at least n).
 * @param eigenvectors Output eigenvectors as columns in column-major order (size n*n, only if computeVectors).
 * @return 0 on success, -1 on invalid input or non-convergence (non-convergence still writes zeros/identity).
 */
int solveSymmetricEigenproblem(const std::vector<double>& A, int n, int lda,
                               bool computeVectors,
                               std::vector<double>& eigenvalues,
                               std::vector<double>& eigenvectors) {
    // Input validation
    if (n < 0 || lda < std::max(1, n) || static_cast<int>(A.size()) < lda * n) {
        return -1;
    }
    // Check symmetry
    const double tol = 1e-12;
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            if (std::abs(A[i + j * lda] - A[j + i * lda]) > tol) {
                return -1;
            }
        }
    }

    // Empty matrix
    if (n == 0) {
        return 0;
    }

    // Map input to an Eigen matrix (column-major, no copy)
    Eigen::Map<const Eigen::MatrixXd> mat(A.data(), n, n, Eigen::Stride<Eigen::Dynamic, Eigen::Dynamic>(lda, 1));

    // Solve eigenproblem
    Eigen::SelfAdjointEigenSolver<Eigen::MatrixXd> eig(mat, computeVectors ? Eigen::ComputeEigenvectors : Eigen::EigenvaluesOnly);

    // Handle non-convergence
    if (eig.info() != Eigen::Success) {
        std::fill(eigenvalues.begin(), eigenvalues.begin() + n, 0.0);
        if (computeVectors) {
            std::fill(eigenvectors.begin(), eigenvectors.begin() + n * n, 0.0);
            for (int i = 0; i < n; ++i) {
                eigenvectors[i + i * n] = 1.0;
            }
        }
        return 0;  // Per task, non-convergence returns 0 but with fallback outputs
    }

    // Copy eigenvalues
    for (int i = 0; i < n; ++i) {
        eigenvalues[i] = eig.eigenvalues()[i];
    }

    // Copy eigenvectors if requested
    if (computeVectors) {
        Eigen::Map<Eigen::MatrixXd> eigvec_out(eigenvectors.data(), n, n);
        eigvec_out = eig.eigenvectors();
    }

    return 0;
}
#include <cassert>
#include <cmath>
#include <vector>
#include <iostream>

// The solution function is assumed to be defined above.

int main() {
    // Test 1: Simple 2x2 symmetric matrix (eigenvalues 3 and 7)
    {
        std::vector<double> A = {3.0, 1.0, 1.0, 3.0};  // column-major, lda=2
        int n = 2, lda = 2;
        std::vector<double> eigenvalues(2), eigenvectors;
        int ret = solveSymmetricEigenproblem(A, n, lda, false, eigenvalues, eigenvectors);
        assert(ret == 0);
        assert(std::abs(eigenvalues[0] - 2.0) < 1e-10);
        assert(std::abs(eigenvalues[1] - 4.0) < 1e-10);
    }

    // Test 2: 3x3 with known eigenvalues (0, 1, 2) diagonal matrix
    {
        std::vector<double> A = {0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 2.0}; // lda=3
        int n = 3, lda = 3;
        std::vector<double> eigenvalues(3), eigenvectors;
        int ret = solveSymmetricEigenproblem(A, n, lda, false, eigenvalues, eigenvectors);
        assert(ret == 0);
        assert(std::abs(eigenvalues[0]) < 1e-10);
        assert(std::abs(eigenvalues[1] - 1.0) < 1e-10);
        assert(std::abs(eigenvalues[2] - 2.0) < 1e-10);
    }

    // Test 3: Compute eigenvectors for 2x2, verify orthonormality and reconstruction
    {
        std::vector<double> A = {2.0, -1.0, -1.0, 2.0}; // eigenvalues 1 and 3
        int n = 2, lda = 2;
        std::vector<double> eigenvalues(2), eigenvectors(4);
        int ret = solveSymmetricEigenproblem(A, n, lda, true, eigenvalues, eigenvectors);
        assert(ret == 0);
        assert(std::abs(eigenvalues[0] - 1.0) < 1e-10);
        assert(std::abs(eigenvalues[1] - 3.0) < 1e-10);
        // Check that A * v = lambda * v for first eigenvector
        double v0x = eigenvectors[0], v0y = eigenvectors[2]; // column 0
        double Av0x = 2.0 * v0x - 1.0 * v0y;
        double Av0y = -1.0 * v0x + 2.0 * v0y;
        assert(std::abs(Av0x - eigenvalues[0] * v0x) < 1e-10);
        assert(std::abs(Av0y - eigenvalues[0] * v0y) < 1e-10);
        // Check orthonormality
        double dot = v0x * eigenvectors[1] + v0y * eigenvectors[3];
        assert(std::abs(dot) < 1e-10);
        assert(std::abs(v0x * v0x + v0y * v0y - 1.0) < 1e-10);
    }

    // Test 4: Empty matrix n=0 returns 0
    {
        std::vector<double> A; // empty
        int n = 0, lda = 1; // lda min is 1 per spec
        std::vector<double> eigenvalues, eigenvectors;
        int ret = solveSymmetricEigenproblem(A, n, lda, false, eigenvalues, eigenvectors);
        assert(ret == 0);
    }

    // Test 5: Invalid input (negative n) returns -1
    {
        std::vector<double> A(4, 0.0);
        int n = -2, lda = 2;
        std::vector<double> eigenvalues, eigenvectors;
        int ret = solveSymmetricEigenproblem(A, n, lda, false, eigenvalues, eigenvectors);
        assert(ret == -1);
    }

    // Test 6: Invalid lda returns -1
    {
        std::vector<double> A = {1.0, 0.0, 0.0, 1.0};
        int n = 2, lda = 1; // invalid
        std::vector<double> eigenvalues(2), eigenvectors;
        int ret = solveSymmetricEigenproblem(A, n, lda, false, eigenvalues, eigenvectors);
        assert(ret == -1);
    }

    // Test 7: Non-symmetric matrix returns -1
    {
        std::vector<double> A = {1.0, 2.0, 3.0, 4.0}; // A[0,1]=2, A[1,0]=3 -> not symmetric
        int n = 2, lda = 2;
        std::vector<double> eigenvalues(2), eigenvectors;
        int ret = solveSymmetricEigenproblem(A, n, lda, false, eigenvalues, eigenvectors);
        assert(ret == -1);
    }

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
