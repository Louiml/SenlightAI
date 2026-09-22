Write a standalone C++ function that performs an in-place Cholesky decomposition of a square, symmetric positive-definite matrix stored in row-major order. The function must accept a flat `std::vector<double>` representing the matrix, its dimension `N`, and a boolean `is_upper` indicating whether the upper or lower triangular factor should be computed (and the other triangle zeroed). The function should return an integer status: `0` on success, a positive integer (the index of the first pivot that failed) if the matrix is not positive-definite, and `-1` if the matrix is not square or if the input vector size does not match `N*N`. The function must modify the input vector in-place so that after a successful call, the requested triangle contains the factor `L` (if lower) or `U` (if upper) such that `A = L * L^T` or `A = U^T * U`, and the opposite triangle is set to zero. The function must not use any external linear algebra libraries; implement the decomposition manually using standard loops and `<cmath>`.

// The core algorithm is the classic Cholesky–Banachiewicz or Cholesky–Crout method. For the lower-triangular case, we iterate `i` from `0` to `N-1`, and for each `j` from `0` to `i`, we compute `L[i][j]` using the formula: for diagonal elements (j==i), `L[i][i] = sqrt(A[i][i] - sum_{k=0}^{i-1} L[i][k]^2)`; for off-diagonal elements (j < i), `L[i][j] = (A[i][j] - sum_{k=0}^{j-1} L[i][k] * L[j][k]) / L[j][j]`. If any diagonal value under the square root becomes non-positive (or due to numerical tolerance, extremely small), we return the 1-based index of the failing row. The upper-triangular case is symmetric: we compute `U` such that `A = U^T * U`, which is essentially the same algorithm but applied to the transposed matrix, or we can compute with loops adapted to upper storage. To keep the code simple and avoid transposition, we can implement both branches explicitly. After computing the factor, we zero the opposite triangle (set all elements outside the requested triangle to zero). The time complexity is `O(N^3)` due to the inner sums, and the space complexity is `O(1)` auxiliary (only the input vector is modified). Edge cases: `N=0` returns `0` (empty matrix trivially valid); non-square matrices (but our input is just an `N` and a vector, so we check `vector.size() == N*N`); if any diagonal pivot is `<= 0`, the matrix is not positive-definite; due to floating point, we use a tolerance like `1e-12` to treat near-zero pivots as failure.

#include <vector>
#include <cmath>
#include <algorithm>

// Performs in-place Cholesky decomposition of a symmetric positive-definite matrix.
// Input: flat matrix A (row-major), dimension N, and is_upper flag.
// On success, A contains L (lower) or U (upper) and the opposite triangle is zeroed.
// Returns: 0 on success, positive 1-based index of failing pivot if not PD, -1 on size mismatch.
int cholesky_inplace(std::vector<double>& A, int N, bool is_upper) {
    if (N < 0) return -1;
    if (static_cast<int>(A.size()) != N * N) return -1;
    if (N == 0) return 0;

    const double tol = 1e-12; // numerical tolerance for positive definiteness

    if (!is_upper) {
        // Lower triangular decomposition: A = L * L^T
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j <= i; ++j) {
                double sum = A[i * N + j];
                for (int k = 0; k < j; ++k) {
                    sum -= A[i * N + k] * A[j * N + k];
                }
                if (i == j) {
                    if (sum <= tol) {
                        return i + 1; // not positive definite
                    }
                    A[i * N + j] = std::sqrt(sum);
                } else {
                    A[i * N + j] = sum / A[j * N + j];
                }
            }
        }
        // Zero upper triangle
        for (int i = 0; i < N; ++i) {
            for (int j = i + 1; j < N; ++j) {
                A[i * N + j] = 0.0;
            }
        }
    } else {
        // Upper triangular decomposition: A = U^T * U
        // We compute U by applying the same algorithm to the mirrored matrix.
        // To avoid copying, we compute U directly with a modified loop.
        for (int j = 0; j < N; ++j) { // column j
            for (int i = 0; i <= j; ++i) { // row i in upper triangle
                double sum = A[i * N + j]; // symmetric element A[i][j] = A[j][i]
                for (int k = 0; k < i; ++k) {
                    sum -= A[k * N + i] * A[k * N + j]; // U[k][i] * U[k][j]
                }
                if (i == j) {
                    if (sum <= tol) {
                        return j + 1;
                    }
                    A[i * N + j] = std::sqrt(sum);
                } else {
                    A[i * N + j] = sum / A[i * N + i];
                    A[j * N + i] = 0.0; // clear lower part later, but we set now
                }
            }
        }
        // Zero lower triangle (already zeroed in loop, but ensure all)
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < i; ++j) {
                A[i * N + j] = 0.0;
            }
        }
    }

    return 0;
}

#include <cassert>
#include <cmath>
#include <vector>

// The solution function is assumed to be defined above.

int main() {
    // Test 1: Simple 2x2 lower
    std::vector<double> A1 = {4.0, 2.0, 2.0, 3.0};
    int s1 = cholesky_inplace(A1, 2, false);
    assert(s1 == 0);
    // Expected L = [[2,0],[1,sqrt(2)]]
    assert(std::fabs(A1[0] - 2.0) < 1e-9);
    assert(std::fabs(A1[1] - 0.0) < 1e-9);
    assert(std::fabs(A1[2] - 1.0) < 1e-9);
    assert(std::fabs(A1[3] - std::sqrt(2.0)) < 1e-9);

    // Test 2: Simple 2x2 upper
    std::vector<double> A2 = {4.0, 2.0, 2.0, 3.0};
    int s2 = cholesky_inplace(A2, 2, true);
    assert(s2 == 0);
    // Expected U = [[2,1],[0,sqrt(2)]]
    assert(std::fabs(A2[0] - 2.0) < 1e-9);
    assert(std::fabs(A2[1] - 1.0) < 1e-9);
    assert(std::fabs(A2[2] - 0.0) < 1e-9);
    assert(std::fabs(A2[3] - std::sqrt(2.0)) < 1e-9);

    // Test 3: 3x3 identity
    std::vector<double> A3 = {1,0,0, 0,1,0, 0,0,1};
    int s3 = cholesky_inplace(A3, 3, false);
    assert(s3 == 0);
    for (double v : A3) {
        assert(std::fabs(v - (v == 0.0 ? 0.0 : 1.0)) < 1e-9); // identity stays identity
    }

    // Test 4: Not positive definite (zero pivot)
    std::vector<double> A4 = {1.0, 2.0, 2.0, 1.0};
    int s4 = cholesky_inplace(A4, 2, false);
    assert(s4 > 0); // should fail at row 2

    // Test 5: Size mismatch
    std::vector<double> A5 = {1,2,3};
    int s5 = cholesky_inplace(A5, 2, false);
    assert(s5 == -1);

    // Test 6: N=0
    std::vector<double> A6;
    int s6 = cholesky_inplace(A6, 0, false);
    assert(s6 == 0);

    // Test 7: 3x3 known matrix
    // A = [[4,12,-16],[12,37,-43],[-16,-43,98]]
    // L = [[2,0,0],[6,1,0],[-8,5,3]]
    std::vector<double> A7 = {4,12,-16, 12,37,-43, -16,-43,98};
    int s7 = cholesky_inplace(A7, 3, false);
    assert(s7 == 0);
    assert(std::fabs(A7[0] - 2.0) < 1e-9);
    assert(std::fabs(A7[3] - 6.0) < 1e-9);
    assert(std::fabs(A7[4] - 1.0) < 1e-9);
    assert(std::fabs(A7[6] + 8.0) < 1e-9);
    assert(std::fabs(A7[7] - 5.0) < 1e-9);
    assert(std::fabs(A7[8] - 3.0) < 1e-9);

    // Test 8: Upper version of previous matrix
    std::vector<double> A8 = {4,12,-16, 12,37,-43, -16,-43,98};
    int s8 = cholesky_inplace(A8, 3, true);
    assert(s8 == 0);
    // Expected U = [[2,6,-8],[0,1,5],[0,0,3]]
    assert(std::fabs(A8[0] - 2.0) < 1e-9);
    assert(std::fabs(A8[1] - 6.0) < 1e-9);
    assert(std::fabs(A8[2] + 8.0) < 1e-9);
    assert(std::fabs(A8[4] - 1.0) < 1e-9);
    assert(std::fabs(A8[5] - 5.0) < 1e-9);
    assert(std::fabs(A8[8] - 3.0) < 1e-9);

    return 0;
}
