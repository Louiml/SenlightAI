// Write a standalone C++ function `int solveLinearSystem(double* A, int n, double* b, int m)` that solves the linear system \(A \mathbf{x} = B\) for an \(n \times n\) matrix \(A\) (stored row-major in a flat array) and an \(n \times m\) right-hand side matrix \(B\) (stored row-major in a flat array) using LU decomposition with partial pivoting, in-place overwriting `A` with its LU factors and `b` with the solution. The function must return `1` on success (matrix non-singular) and `0` if the matrix is singular (i.e., a pivot is numerically too small, use tolerance `1e-10`). The solution must be robust for \(n \geq 1\), \(m \geq 0\) (if `m == 0`, no right-hand-side matrix is provided and `b` may be `nullptr`; in that case, the function should still perform the decomposition and return success/failure). Edge cases: if `n == 0`, return `1` immediately; if `A` is `nullptr` and `n > 0`, return `0`. The function must not use any external libraries beyond the C++ standard library, and must not allocate dynamic memory in the main algorithm (use only fixed-size local storage, though for simplicity you may use a `std::vector<double>` for the temporary row-swap buffer if needed, but it's preferable to avoid it).
The solution implements LU decomposition with partial pivoting directly from the provided snippet but simplified to a fixed double precision and a single flat array layout. The algorithm proceeds column by column: for each pivot column `i`, find the row `k` with the largest absolute value in that column from row `i` downward. If the maximum absolute value is below `eps = 1e-10`, the matrix is singular, return `0`. If `k != i`, swap rows `i` and `k` in both the matrix `A` and, if `b` is non-null and `m > 0`, in the right-hand side rows. Then compute the multiplier for each row below `i` as `alpha = A[j*n + i] / A[i*n + i]`, store it in `A[j*n + i]` (overwriting the original value, since it won’t be needed later), and update the trailing submatrix elements `A[j*n + k]` for `k > i` and the right-hand side rows `b[j*m + col]` for all `col`. After forward elimination, perform back substitution: for each row `i` from bottom to top, compute the solution by subtracting known contributions from rows below and dividing by the pivot `A[i*n + i]`. The function returns `1` if decomposition completes. Time complexity: \(O(n^3 + n^2 m)\) due to the nested loops for elimination and back substitution. Space complexity: \(O(1)\) auxiliary (no extra storage beyond a few scalars), as all operations are in-place.
#include <cmath>
#include <cstddef>
#include <utility>

/**
 * Solves A * X = B using in-place LU decomposition with partial pivoting.
 *
 * @param A  Row-major n x n matrix. On output, contains LU factors in-place (the
 *           upper triangular part is the U matrix, and the lower triangular part
 *           (including the implicit diagonal of 1s) is the L matrix).
 * @param n  Size of the square matrix (n >= 0).
 * @param b  Row-major n x m right-hand side. On output, contains the solution X.
 *           If m == 0, this may be nullptr and is ignored.
 * @param m  Number of columns in the right-hand side (m >= 0).
 * @return   1 on success, 0 if the matrix is singular (pivot below tolerance).
 */
int solveLinearSystem(double* A, int n, double* b, int m) {
    if (n == 0) return 1;
    if (A == nullptr) return 0;

    const double eps = 1e-10;

    // LU decomposition with partial pivoting
    for (int i = 0; i < n; ++i) {
        // Find pivot row
        int k = i;
        double max_abs = std::fabs(A[i * n + i]);
        for (int j = i + 1; j < n; ++j) {
            double val = std::fabs(A[j * n + i]);
            if (val > max_abs) {
                max_abs = val;
                k = j;
            }
        }

        if (max_abs < eps) {
            return 0; // singular matrix
        }

        if (k != i) {
            // Swap rows in A
            for (int j = 0; j < n; ++j) {
                std::swap(A[i * n + j], A[k * n + j]);
            }
            // Swap rows in b if present
            if (b != nullptr && m > 0) {
                for (int j = 0; j < m; ++j) {
                    std::swap(b[i * m + j], b[k * m + j]);
                }
            }
        }

        double pivot = A[i * n + i];
        // Eliminate below pivot
        for (int j = i + 1; j < n; ++j) {
            double alpha = A[j * n + i] / pivot;
            A[j * n + i] = alpha; // store multiplier in L
            for (int col = i + 1; col < n; ++col) {
                A[j * n + col] -= alpha * A[i * n + col];
            }
            if (b != nullptr && m > 0) {
                for (int col = 0; col < m; ++col) {
                    b[j * m + col] -= alpha * b[i * m + col];
                }
            }
        }
    }

    // Back substitution
    if (b != nullptr && m > 0) {
        for (int i = n - 1; i >= 0; --i) {
            for (int col = 0; col < m; ++col) {
                double sum = b[i * m + col];
                for (int j = i + 1; j < n; ++j) {
                    sum -= A[i * n + j] * b[j * m + col];
                }
                b[i * m + col] = sum / A[i * n + i];
            }
        }
    }

    return 1;
}
#include <cassert>
#include <cmath>

// Declare the solution function
int solveLinearSystem(double* A, int n, double* b, int m);

int main() {
    // Test 1: 2x2 system with b having one column
    {
        double A[4] = {4.0, 3.0, 6.0, 3.0};
        double b[2] = {10.0, 12.0};
        int res = solveLinearSystem(A, 2, b, 1);
        assert(res == 1);
        // Solution: x = [1, 2]
        assert(std::fabs(b[0] - 1.0) < 1e-9);
        assert(std::fabs(b[1] - 2.0) < 1e-9);
    }

    // Test 2: 3x3 system with multiple right-hand sides
    {
        double A[9] = {2.0, 1.0, -1.0, -3.0, -1.0, 2.0, -2.0, 1.0, 2.0};
        double b[6] = {8.0, 1.0, -11.0, -2.0, -3.0, 5.0}; // two RHS columns
        int res = solveLinearSystem(A, 3, b, 2);
        assert(res == 1);
        // Solution for first column: [2, 3, -1]
        // Solution for second column: [2, 1, 2]
        assert(std::fabs(b[0] - 2.0) < 1e-9);
        assert(std::fabs(b[1] - 3.0) < 1e-9);
        assert(std::fabs(b[2] + 1.0) < 1e-9);
        assert(std::fabs(b[3] - 2.0) < 1e-9);
        assert(std::fabs(b[4] - 1.0) < 1e-9);
        assert(std::fabs(b[5] - 2.0) < 1e-9);
    }

    // Test 3: Singular matrix should return 0
    {
        double A[4] = {1.0, 2.0, 2.0, 4.0};
        double b[2] = {1.0, 2.0};
        int res = solveLinearSystem(A, 2, b, 1);
        assert(res == 0);
    }

    // Test 4: Identity matrix, n=1
    {
        double A[1] = {5.0};
        double b[1] = {15.0};
        int res = solveLinearSystem(A, 1, b, 1);
        assert(res == 1);
        assert(std::fabs(b[0] - 3.0) < 1e-9);
    }

    // Test 5: n=0 returns 1
    {
        int res = solveLinearSystem(nullptr, 0, nullptr, 0);
        assert(res == 1);
    }

    // Test 6: m=0 (no right-hand side), matrix non-singular
    {
        double A[4] = {2.0, 0.0, 0.0, 3.0};
        int res = solveLinearSystem(A, 2, nullptr, 0);
        assert(res == 1);
        // A should still have the LU factors (diagonal pivots)
        assert(std::fabs(A[0] - 2.0) < 1e-9);
        assert(std::fabs(A[3] - 3.0) < 1e-9);
    }

    // Test 7: Need pivoting (row swap)
    {
        double A[4] = {0.0, 1.0, 1.0, 0.0};
        double b[2] = {1.0, 2.0};
        int res = solveLinearSystem(A, 2, b, 1);
        assert(res == 1);
        // Solution: x = [2, 1]
        assert(std::fabs(b[0] - 2.0) < 1e-9);
        assert(std::fabs(b[1] - 1.0) < 1e-9);
    }

    // Test 8: Large diagonal values
    {
        double A[9] = {1e10, 0.0, 0.0, 0.0, 1e-10, 0.0, 0.0, 0.0, 1.0};
        double b[3] = {1e10, 1e-10, 1.0};
        int res = solveLinearSystem(A, 3, b, 1);
        assert(res == 1);
        // Solution should be [1, 1, 1]
        assert(std::fabs(b[0] - 1.0) < 1e-6);
        assert(std::fabs(b[1] - 1.0) < 1e-6);
        assert(std::fabs(b[2] - 1.0) < 1e-6);
    }

    return 0;
}
