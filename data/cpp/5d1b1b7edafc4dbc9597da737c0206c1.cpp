/*
Write a C++ function `tridiagonalSymmetricDecomposition` that takes a square, symmetric real matrix (as `std::vector<std::vector<double>>`) and returns a `std::pair<std::vector<double>, std::vector<double>>` containing the main diagonal and subdiagonal of its symmetric tridiagonal form using Householder reflections. The input matrix is guaranteed to be real and symmetric (i.e., `A[i][j] == A[j][i]`). The function must handle any size `n >= 1`. For `n == 1`, return the single diagonal element and an empty subdiagonal. The algorithm should preserve the eigenvalues (i.e., the tridiagonal matrix is orthogonally similar to the original). The function should not modify the input matrix and should apply `const` correctly.
*/
#include <vector>
#include <cmath>
#include <utility>
#include <algorithm>

// Compute the symmetric tridiagonal decomposition of a real symmetric matrix
// using Householder reflections. Returns (diagonal, subdiagonal).
std::pair<std::vector<double>, std::vector<double>>
tridiagonalSymmetricDecomposition(const std::vector<std::vector<double>>& A) {
    const std::size_t n = A.size();
    // Work on a copy to preserve the input
    std::vector<std::vector<double>> M = A;

    for (std::size_t k = 0; k + 1 < n; ++k) {
        // Compute the norm of the subdiagonal part of column k (rows k+1..n-1)
        double norm = 0.0;
        for (std::size_t i = k + 1; i < n; ++i) {
            norm += M[i][k] * M[i][k];
        }
        if (norm < 1e-15) {
            continue; // already zero below subdiagonal
        }
        norm = std::sqrt(norm);

        // Choose sign to avoid cancellation: use negative of the first subdiagonal element
        double alpha = (M[k+1][k] >= 0.0) ? -norm : norm;
        // Householder vector v (length n-k-1), first component modified
        std::vector<double> v(n - k - 1, 0.0);
        v[0] = M[k+1][k] - alpha;
        for (std::size_t i = 1; i < v.size(); ++i) {
            v[i] = M[k+1+i][k];
        }
        double vv = 0.0;
        for (double val : v) {
            vv += val * val;
        }
        if (vv < 1e-30) {
            continue; // no reflection needed
        }

        // Apply H * M * H where H = I - 2*v*v^T / vv
        // Compute w = (2/vv) * M * v, then M = M - v*w^T - w*v^T
        std::vector<double> w(n, 0.0);
        for (std::size_t i = 0; i < n; ++i) {
            double sum = 0.0;
            for (std::size_t j = k + 1; j < n; ++j) {
                sum += M[i][j] * v[j - (k+1)];
            }
            w[i] = (2.0 / vv) * sum;
        }

        for (std::size_t i = 0; i < n; ++i) {
            for (std::size_t j = 0; j < n; ++j) {
                M[i][j] -= v[i >= (k+1) ? i - (k+1) : 0] * w[j] +
                           w[i] * v[j >= (k+1) ? j - (k+1) : 0];
            }
        }
    }

    // Extract diagonal and subdiagonal
    std::vector<double> diag(n);
    std::vector<double> subdiag(n > 0 ? n - 1 : 0);
    for (std::size_t i = 0; i < n; ++i) {
        diag[i] = M[i][i];
        if (i + 1 < n) {
            subdiag[i] = M[i+1][i];
        }
    }
    return {diag, subdiag};
}
#include <cassert>
#include <cmath>
#include <vector>
#include <utility>

// The solution function is assumed to be defined above.
// Tests below verify the tridiagonal structure and symmetry preservation.

int main() {
    // Test 1: 1x1 matrix
    {
        std::vector<std::vector<double>> A = {{5.0}};
        auto [diag, sub] = tridiagonalSymmetricDecomposition(A);
        assert(diag.size() == 1 && sub.size() == 0);
        assert(std::abs(diag[0] - 5.0) < 1e-12);
    }

    // Test 2: 2x2 symmetric matrix
    {
        std::vector<std::vector<double>> A = {{2.0, 1.0}, {1.0, 3.0}};
        auto [diag, sub] = tridiagonalSymmetricDecomposition(A);
        assert(diag.size() == 2 && sub.size() == 1);
        // Trace preserved
        assert(std::abs((diag[0] + diag[1]) - 5.0) < 1e-12);
        assert(std::abs(sub[0] - std::sqrt(2.0)) < 1e-12); // eigenvalues 4 and 1, off-diag magnitude sqrt(2)
    }

    // Test 3: 3x3 matrix, check tridiagonal structure
    {
        std::vector<std::vector<double>> A = {{4.0, 1.0, 2.0},
                                              {1.0, 3.0, 0.5},
                                              {2.0, 0.5, 5.0}};
        auto [diag, sub] = tridiagonalSymmetricDecomposition(A);
        assert(diag.size() == 3 && sub.size() == 2);
        // Check that the resulting tridiagonal has trace equal to original trace
        double trace = 0.0;
        for (double d : diag) trace += d;
        assert(std::abs(trace - 12.0) < 1e-10);
        // Check off-diagonal symmetry: sub[0] and sub[1] should be approximately equal
        assert(std::abs(sub[0] - sub[1]) < 1e-10);
    }

    // Test 4: already tridiagonal matrix
    {
        std::vector<std::vector<double>> A = {{1.0, 2.0, 0.0},
                                              {2.0, 3.0, 4.0},
                                              {0.0, 4.0, 5.0}};
        auto [diag, sub] = tridiagonalSymmetricDecomposition(A);
        assert(diag.size() == 3 && sub.size() == 2);
        assert(std::abs(diag[0] - 1.0) < 1e-12);
        assert(std::abs(diag[1] - 3.0) < 1e-12);
        assert(std::abs(diag[2] - 5.0) < 1e-12);
        assert(std::abs(sub[0] - 2.0) < 1e-12);
        assert(std::abs(sub[1] - 4.0) < 1e-12);
    }

    // Test 5: zero matrix of size 4
    {
        std::vector<std::vector<double>> A(4, std::vector<double>(4, 0.0));
        auto [diag, sub] = tridiagonalSymmetricDecomposition(A);
        assert(diag.size() == 4 && sub.size() == 3);
        for (double d : diag) assert(std::abs(d) < 1e-12);
        for (double s : sub) assert(std::abs(s) < 1e-12);
    }

    return 0;
}
// The solution uses Householder reflections to reduce a symmetric matrix to tridiagonal form. For each column `k` from 0 to `n-2`, we form a Householder vector `v` that zeroes out the elements below the first subdiagonal in that column. Specifically, we compute the norm of the subdiagonal part of column `k` (rows `k+1` to `n-1`), choose a sign to avoid cancellation, and construct the reflection `H = I - 2*v*v^T / (v^T v)`. We then apply this similarity transformation to the matrix: `A = H * A * H` (since `H` is symmetric and orthogonal). This preserves symmetry and zeros out the desired elements. After processing all columns, the matrix becomes tridiagonal. We extract the diagonal (rows 0..n-1) and subdiagonal (rows 1..n-1, columns row-1). Care must be taken to handle the case where the subdiagonal part is already zero (skip that column). The transformation should be done in-place on a copy of the input to avoid modifying the original. Time complexity is `O(n^3)` due to the matrix multiplications for each column. Space complexity is `O(n^2)` for the matrix copy plus `O(n)` for the returned vectors.
