// Write a standalone C++ function `computeSymmetricEigen` that accepts a symmetric matrix supplied in a flattened column-major array (LAPACK-style), along with its leading dimension and size `n`, and a boolean flag `wantVectors`. The function must compute the eigenvalues (in ascending order) and, if requested, the orthonormal eigenvectors, writing eigenvalues into a caller-provided output array `eigenvalues` and eigenvectors back into the input matrix array `matrixData` in column-major order. The function must handle the special case `n=0` by returning immediately, and must validate that `lda >= max(1, n)` and that the matrix is symmetric (within a tolerance). Use Eigen's `SelfAdjointEigenSolver` internally, and if convergence fails, set all eigenvalues to zero and (if vectors requested) set the matrix to the identity. The function should not allocate any dynamic memory internally beyond what Eigen itself uses (you may use Eigen types on the stack).
The solution uses Eigen's `SelfAdjointEigenSolver`, which is the standard efficient algorithm for symmetric eigenproblems (typically a tridiagonalization via Householder reflections followed by the QR algorithm or divide-and-conquer). The main steps are:

1. **Input validation**: Check that `lda >= max(1, n)` and that the matrix is symmetric within a tolerance (e.g., `1e-12 * max(1, maxAbs)`). If not, throw a `std::invalid_argument` (or return an error code, but the task likely expects throwing).
2. **Copy input into an Eigen matrix**: Since the input is column-major, we can use `Eigen::Map<const Eigen::Matrix<Scalar, Dynamic, Dynamic, ColMajor>>` to avoid copying, but for simplicity we can copy into a local `Eigen::MatrixXd`.
3. **Symmetry check**: Compute the absolute difference between the matrix and its transpose; if the maximum exceeds tolerance, throw.
4. **Eigensolver**: Create `SelfAdjointEigenSolver<MatrixXd>` with options `EigenvaluesOnly` or `ComputeEigenvectors`. The eigenvalues are naturally sorted in ascending order.
5. **Handle non-convergence**: If `eig.info() != Success`, set eigenvalues to zero and, if vectors requested, set the matrix to identity (this matches the LAPACK behavior in the snippet, which does not report failure).
6. **Write outputs**: Copy eigenvalues to the output array, and if vectors requested, copy the eigenvector matrix (in column-major order) back to the input array.
7. **Edge cases**: `n=0` returns immediately; `n=1` works naturally (eigenvalue equals the single entry, eigenvector is [1]).
8. **Complexity**: For an `n x n` matrix, time is `O(n^3)` and space is `O(n^2)` (for the copy and the solver's internal workspaces). The function itself does not allocate additional dynamic memory beyond Eigen's internal allocations.
#include <Eigen/Eigenvalues>
#include <stdexcept>
#include <cmath>
#include <algorithm>

// Compute eigenvalues (and optionally eigenvectors) of a symmetric matrix.
// matrixData: column-major array of size lda*n (only first n rows/cols used)
// lda: leading dimension (>= max(1,n))
// n: matrix size
// wantVectors: if true, eigenvectors are written back into matrixData
// eigenvalues: output array of length n, eigenvalues in ascending order
// Throws std::invalid_argument on invalid lda or non-symmetric matrix.
void computeSymmetricEigen(double* matrixData, int lda, int n,
                           bool wantVectors, double* eigenvalues) {
    if (n == 0) return;
    if (lda < std::max(1, n)) {
        throw std::invalid_argument("lda must be >= max(1,n)");
    }

    // Map input to Eigen column-major matrix
    Eigen::Map<Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::ColMajor>> mat(matrixData, n, lda);
    // Take only the top-left n x n block (since lda may be > n)
    Eigen::MatrixXd A = mat.block(0, 0, n, n);

    // Symmetry check
    double maxAbs = A.cwiseAbs().maxCoeff();
    double tol = 1e-12 * std::max(1.0, maxAbs);
    if ((A - A.transpose()).cwiseAbs().maxCoeff() > tol) {
        throw std::invalid_argument("Matrix is not symmetric");
    }

    // Compute eigen decomposition
    Eigen::SelfAdjointEigenSolver<Eigen::MatrixXd> eig(
        A, wantVectors ? Eigen::ComputeEigenvectors : Eigen::EigenvaluesOnly);

    if (eig.info() != Eigen::Success) {
        // Set eigenvalues to zero and matrix to identity (LAPACK-like fallback)
        std::fill(eigenvalues, eigenvalues + n, 0.0);
        if (wantVectors) {
            mat.block(0, 0, n, n).setIdentity();
        }
        return;
    }

    // Copy eigenvalues (ascending order)
    std::copy(eig.eigenvalues().data(), eig.eigenvalues().data() + n, eigenvalues);

    // Copy eigenvectors if requested (column-major)
    if (wantVectors) {
        mat.block(0, 0, n, n) = eig.eigenvectors();
    }
}
#include <cassert>
#include <cmath>
#include <vector>
#include <algorithm>

// Helper to build a symmetric matrix in column-major
void makeSymMatrix(std::vector<double>& data, int lda, int n,
                   const std::vector<double>& lowerTri) {
    data.assign(lda * n, 0.0);
    int idx = 0;
    for (int j = 0; j < n; ++j) {
        for (int i = 0; i < n; ++i) {
            if (i >= j) {
                data[j * lda + i] = lowerTri[idx++];
                data[i * lda + j] = lowerTri[idx-1];
            }
        }
    }
}

int main() {
    // Test 1: 2x2 matrix [[2,1],[1,2]] -> eigenvalues 1,3
    {
        int n = 2, lda = 2;
        std::vector<double> mat(lda * n, 0.0);
        mat[0] = 2.0; mat[1] = 1.0; // col0
        mat[2] = 1.0; mat[3] = 2.0; // col1
        std::vector<double> eig(n);
        computeSymmetricEigen(mat.data(), lda, n, false, eig.data());
        assert(std::abs(eig[0] - 1.0) < 1e-12);
        assert(std::abs(eig[1] - 3.0) < 1e-12);
    }

    // Test 2: 3x3 identity -> eigenvalues all 1, vectors identity
    {
        int n = 3, lda = 3;
        std::vector<double> mat(lda * n, 0.0);
        for (int i = 0; i < n; ++i) mat[i * lda + i] = 1.0;
        std::vector<double> eig(n);
        computeSymmetricEigen(mat.data(), lda, n, true, eig.data());
        for (int i = 0; i < n; ++i) assert(std::abs(eig[i] - 1.0) < 1e-12);
        for (int i = 0; i < n; ++i)
            for (int j = 0; j < n; ++j)
                assert(std::abs(mat[j * lda + i] - (i == j ? 1.0 : 0.0)) < 1e-12);
    }

    // Test 3: n=1 with value 5
    {
        int n = 1, lda = 1;
        std::vector<double> mat = {5.0};
        std::vector<double> eig(1);
        computeSymmetricEigen(mat.data(), lda, n, false, eig.data());
        assert(std::abs(eig[0] - 5.0) < 1e-12);
    }

    // Test 4: 4x4 diagonal with values 4,3,2,1 -> eigenvalues sorted 1,2,3,4
    {
        int n = 4, lda = 4;
        std::vector<double> mat(lda * n, 0.0);
        std::vector<double> diag = {4.0, 3.0, 2.0, 1.0};
        for (int i = 0; i < n; ++i) mat[i * lda + i] = diag[i];
        std::vector<double> eig(n);
        computeSymmetricEigen(mat.data(), lda, n, false, eig.data());
        for (int i = 0; i < n; ++i) assert(std::abs(eig[i] - (i + 1.0)) < 1e-12);
    }

    // Test 5: lda > n (padded matrix) and vectors
    {
        int n = 2, lda = 5;
        std::vector<double> mat(lda * n, 0.0);
        // Set [[2,1],[1,2]] in top-left, other entries arbitrary
        mat[0] = 2.0; mat[1] = 1.0;
        mat[5] = 1.0; mat[6] = 2.0;
        // Fill rest with garbage
        for (int i = 2; i < lda * n; ++i) mat[i] = 123.0;
        std::vector<double> eig(n);
        computeSymmetricEigen(mat.data(), lda, n, true, eig.data());
        assert(std::abs(eig[0] - 1.0) < 1e-12);
        assert(std::abs(eig[1] - 3.0) < 1e-12);
        // Verify eigenvectors orthonormal
        double v0[2] = {mat[0], mat[1]}; // col0
        double v1[2] = {mat[lda], mat[lda+1]}; // col1
        double dot = v0[0]*v1[0] + v0[1]*v1[1];
        assert(std::abs(dot) < 1e-12);
        assert(std::abs(v0[0]*v0[0] + v0[1]*v0[1] - 1.0) < 1e-12);
        assert(std::abs(v1[0]*v1[0] + v1[1]*v1[1] - 1.0) < 1e-12);
    }

    // Test 6: Non-symmetric matrix throws
    {
        int n = 2, lda = 2;
        std::vector<double> mat = {1.0, 2.0, 3.0, 4.0};
        std::vector<double> eig(n);
        bool threw = false;
        try {
            computeSymmetricEigen(mat.data(), lda, n, false, eig.data());
        } catch (const std::invalid_argument&) {
            threw = true;
        }
        assert(threw);
    }

    // Test 7: Invalid lda throws
    {
        int n = 3, lda = 2; // invalid
        std::vector<double> mat(lda * n, 0.0);
        std::vector<double> eig(n);
        bool threw = false;
        try {
            computeSymmetricEigen(mat.data(), lda, n, false, eig.data());
        } catch (const std::invalid_argument&) {
            threw = true;
        }
        assert(threw);
    }

    return 0;
}
