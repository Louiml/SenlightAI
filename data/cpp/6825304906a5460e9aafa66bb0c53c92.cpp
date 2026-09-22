Write a standalone C++ function `luSolve` that accepts a square matrix `A` (as a `std::vector<std::vector<double>>`) and a right-hand side vector `b` (as a `std::vector<double>`) of the same size, and returns the solution vector `x` to the linear system `A x = b`. The function must perform an LU decomposition of `A` using partial pivoting with row interchanges, then solve the resulting triangular systems. The input matrix `A` is passed by const reference and must not be modified; the decomposition is done on an internal copy. Assume the matrix is non-singular (no need to check for singularity). The function must handle any positive integer size `n`. For simplicity, do not handle the transpose or adjoint cases—only solve `A x = b`.
#include <cassert>
#include <cmath>
#include <vector>

// The solution function is assumed to be declared elsewhere.
std::vector<double> luSolve(const std::vector<std::vector<double>>& A,
                            const std::vector<double>& b);

int main() {
    // Test 1: 2x2 system
    {
        std::vector<std::vector<double>> A = {{2, 1}, {1, 3}};
        std::vector<double> b = {4, 5};
        std::vector<double> x = luSolve(A, b);
        assert(std::fabs(x[0] - 1.4) < 1e-9);
        assert(std::fabs(x[1] - 1.2) < 1e-9);
    }

    // Test 2: 3x3 system with a known solution
    {
        std::vector<std::vector<double>> A = {{1, 2, 3}, {0, 1, 4}, {5, 6, 0}};
        std::vector<double> b = {14, 9, 11};
        std::vector<double> x = luSolve(A, b);
        assert(std::fabs(x[0] - 1.0) < 1e-9);
        assert(std::fabs(x[1] - 1.0) < 1e-9);
        assert(std::fabs(x[2] - 2.0) < 1e-9);
    }

    // Test 3: 1x1 system
    {
        std::vector<std::vector<double>> A = {{7}};
        std::vector<double> b = {21};
        std::vector<double> x = luSolve(A, b);
        assert(std::fabs(x[0] - 3.0) < 1e-9);
    }

    // Test 4: 4x4 system requiring row swaps (matrix originally badly ordered)
    {
        std::vector<std::vector<double>> A = {{0, 1, 2, 3},
                                              {1, 0, 1, 0},
                                              {2, 1, 0, 0},
                                              {3, 2, 1, 0}};
        std::vector<double> b = {14, 2, 3, 4};
        std::vector<double> x = luSolve(A, b);
        // Verify by multiplying A * x and comparing to b.
        std::vector<double> result(4, 0.0);
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                result[i] += A[i][j] * x[j];
            }
            assert(std::fabs(result[i] - b[i]) < 1e-9);
        }
    }

    // Test 5: 2x2 with a negative pivot
    {
        std::vector<std::vector<double>> A = {{-3, 2}, {1, -1}};
        std::vector<double> b = {1, 0};
        std::vector<double> x = luSolve(A, b);
        assert(std::fabs(x[0] - (-1.0)) < 1e-9);
        assert(std::fabs(x[1] - (-1.0)) < 1e-9);
    }

    return 0;
}
#include <vector>
#include <cmath>
#include <algorithm>

// Solve A x = b for a square matrix A using LU decomposition with partial pivoting.
// A is passed by const reference and is not modified internally; the matrix is copied.
// Returns the solution vector x.
std::vector<double> luSolve(const std::vector<std::vector<double>>& A,
                            const std::vector<double>& b) {
    const std::size_t n = A.size();
    // Copy the matrix to allow modification during factorization.
    std::vector<std::vector<double>> LU = A;
    // Vector to record row swaps: ipiv[i] = original row index that ended up at row i.
    std::vector<std::size_t> ipiv(n);
    for (std::size_t i = 0; i < n; ++i) ipiv[i] = i;

    // LU factorization with partial pivoting.
    for (std::size_t k = 0; k < n; ++k) {
        // Find pivot row: largest absolute value in column k from row k down.
        std::size_t p = k;
        double max_abs = std::abs(LU[k][k]);
        for (std::size_t i = k + 1; i < n; ++i) {
            if (std::abs(LU[i][k]) > max_abs) {
                max_abs = std::abs(LU[i][k]);
                p = i;
            }
        }
        // Swap rows k and p in the matrix and record in ipiv.
        if (p != k) {
            std::swap(LU[k], LU[p]);
            std::swap(ipiv[k], ipiv[p]);
        }
        // Compute multipliers and update the Schur complement.
        for (std::size_t i = k + 1; i < n; ++i) {
            LU[i][k] /= LU[k][k];
            for (std::size_t j = k + 1; j < n; ++j) {
                LU[i][j] -= LU[i][k] * LU[k][j];
            }
        }
    }

    // Apply the row permutation to the right-hand side vector b.
    std::vector<double> y(n);
    for (std::size_t i = 0; i < n; ++i) {
        y[i] = b[ipiv[i]];
    }

    // Forward substitution: solve L y = b (L has unit diagonal).
    // The multipliers are stored below the diagonal in LU.
    for (std::size_t i = 0; i < n; ++i) {
        for (std::size_t j = 0; j < i; ++j) {
            y[i] -= LU[i][j] * y[j];
        }
    }

    // Backward substitution: solve U x = y (U is the upper part of LU).
    std::vector<double> x(n);
    for (std::size_t i = n; i-- > 0; ) {
        double sum = y[i];
        for (std::size_t j = i + 1; j < n; ++j) {
            sum -= LU[i][j] * x[j];
        }
        x[i] = sum / LU[i][i];
    }

    return x;
}
// The solution follows the standard LU decomposition with partial pivoting. We copy the input matrix `A` into a local mutable matrix `LU`. For each column `k` from 0 to n-1, we find the pivot row `p` with the largest absolute value in `LU[k..n-1][k]`. If `p != k`, swap rows `k` and `p` in the matrix and record the pivot index in a permutation vector `ipiv`. Then, for each row `i > k`, compute the multiplier `LU[i][k] /= LU[k][k]` and update the remaining columns `j > k` by `LU[i][j] -= LU[i][k] * LU[k][j]`. After the factorization, we apply the same row permutations to the right-hand side vector `b` (by creating a new vector `y` and copying from `b` according to `ipiv`), then solve the lower triangular system `L y = b` (where `L` has unit diagonal and `LU[i][j]` for `i > j`) via forward substitution, and finally solve the upper triangular system `U x = y` via backward substitution. Edge case: n = 1 works trivially. We assume no zero pivots occur (non-singular matrix). Time complexity is O(n^3) for the decomposition and O(n^2) for the two solves, total O(n^3). Space complexity is O(n^2) for the matrix copy and O(n) for the pivot and vector storage.
