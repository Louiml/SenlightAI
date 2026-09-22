Write a C++ function `double solveJacobi(int n, const double* A, const double* b, double* x, int maxIter, double tol)` that solves the linear system `A*x = b` using the Jacobi iterative method for an `n`-by-`n` matrix `A` (stored in row-major order), with an initial guess of `x[i] = 0` for all `i`. The function must perform up to `maxIter` iterations, stopping early when the current residual's L2 norm (i.e., `||A*x - b||_2`) is less than `tol`. The final solution must be stored in the output array `x`, and the function returns the number of iterations actually performed. The matrix `A` must be diagonally dominant (i.e., `|A[i][i]| > sum_{j!=i} |A[i][j]|` for all rows) so that the method converges. The function should not modify the input matrix `A` or vector `b`; only the output `x` may be modified. You must implement the matrix-vector multiplication inline within the Jacobi update (no separate helper function is required). Handle the case where `n <= 0` by returning 0 without modifying `x`. Use double precision arithmetic.
The Jacobi method iteratively solves `A*x = b` by isolating each diagonal element. For each row `i`, the new value `x_new[i]` equals `(b[i] - sum_{j!=i} A[i][j] * x_old[j]) / A[i][i]`. The key is to compute all new values using the old vector `x` (from the previous iteration) before updating `x` in place, because the method requires simultaneous updates. We initialize `x` to all zeros as specified. At each iteration, we first compute the residual `r = A*x - b` using the current (old) `x`, calculate its squared L2 norm, and break if `sqrt(norm_sq) < tol`. Then compute the new `x` using the old `x` values. We must be careful not to read from `x` while writing to it during the same iteration; use a temporary vector `x_new`. The function returns the number of iterations completed, including the final iteration that triggered termination. Edge cases: if `n <= 0`, return 0 immediately. If `maxIter` is 0, return 0 (no iterations). If `tol` is negative or zero, we still perform at least one iteration unless `maxIter` is 0, because the condition `sqrt(...) < tol` will be false for any non-negative residual (and a residual of zero would be `< tol` for any positive `tol`; but with zero tolerance, we would stop only if residual is exactly zero, which is unlikely). For typical positive tolerance, the method works as expected. Time complexity is `O(maxIter * n^2)` because each iteration computes a matrix-vector product (O(n^2)) and does an O(n) update. Space complexity is `O(n)` for the temporary vector.
#include <cmath>
#include <vector>

/*
 * Solves A*x = b using Jacobi's iterative method.
 *
 * @param n       Number of rows/columns of A (and size of vectors).
 * @param A       Row-major n-by-n matrix (must be diagonally dominant).
 * @param b       Right-hand side vector of length n.
 * @param x       Output vector; on entry, its contents are ignored and it is
 *                overwritten with the solution. Initially set to all zeros.
 * @param maxIter Maximum number of iterations to perform.
 * @param tol     Termination tolerance on the L2 norm of the residual.
 * @return        Number of iterations actually performed (could be less than
 *                maxIter if convergence is reached early).
 */
double solveJacobi(int n, const double* A, const double* b, double* x,
                   int maxIter, double tol) {
    if (n <= 0 || maxIter <= 0) {
        return 0;
    }

    // Initialize x to zeros.
    for (int i = 0; i < n; ++i) {
        x[i] = 0.0;
    }

    std::vector<double> x_new(n);
    std::vector<double> residual(n);

    int iter = 0;
    while (iter < maxIter) {
        // Compute residual r = A*x - b using current x.
        double norm_sq = 0.0;
        for (int i = 0; i < n; ++i) {
            double sum = 0.0;
            for (int j = 0; j < n; ++j) {
                sum += A[i * n + j] * x[j];
            }
            residual[i] = sum - b[i];
            norm_sq += residual[i] * residual[i];
        }
        // Check convergence.
        if (std::sqrt(norm_sq) < tol) {
            break;
        }

        // Perform Jacobi update using old x values.
        for (int i = 0; i < n; ++i) {
            double sum_off_diag = 0.0;
            double diag = A[i * n + i];
            for (int j = 0; j < n; ++j) {
                if (j != i) {
                    sum_off_diag += A[i * n + j] * x[j];
                }
            }
            x_new[i] = (b[i] - sum_off_diag) / diag;
        }

        // Copy new values to x.
        for (int i = 0; i < n; ++i) {
            x[i] = x_new[i];
        }
        ++iter;
    }

    return static_cast<double>(iter);
}
#include <cassert>
#include <cmath>

// The solution function is declared here (for reference).
double solveJacobi(int n, const double* A, const double* b, double* x,
                   int maxIter, double tol);

int main() {
    // Test 1: 2x2 diagonal system, exact convergence in 1 iteration.
    {
        int n = 2;
        double A[] = {4.0, 0.0, 0.0, 2.0};
        double b[] = {8.0, 6.0};
        double x[2];
        double iters = solveJacobi(n, A, b, x, 10, 1e-12);
        assert(std::abs(x[0] - 2.0) < 1e-9);
        assert(std::abs(x[1] - 3.0) < 1e-9);
        assert(iters == 1.0); // after one iteration residual is zero
    }

    // Test 2: 3x3 diagonally dominant system, converges.
    {
        int n = 3;
        double A[] = {
            10.0, 1.0, 1.0,
            1.0, 10.0, 1.0,
            1.0, 1.0, 10.0
        };
        double b[] = {12.0, 12.0, 12.0};
        double x[3];
        double iters = solveJacobi(n, A, b, x, 100, 1e-10);
        assert(std::abs(x[0] - 1.0) < 1e-8);
        assert(std::abs(x[1] - 1.0) < 1e-8);
        assert(std::abs(x[2] - 1.0) < 1e-8);
        assert(iters > 0 && iters <= 100);
    }

    // Test 3: maxIter = 0 returns 0 and leaves x unchanged (initialized? We set x to 0 before call).
    {
        int n = 2;
        double A[] = {1.0, 0.0, 0.0, 1.0};
        double b[] = {1.0, 2.0};
        double x[2] = {99.0, 99.0}; // x unchanged because n>0 but maxIter=0, function returns 0 without touching x.
        double iters = solveJacobi(n, A, b, x, 0, 1e-6);
        assert(iters == 0.0);
        assert(x[0] == 99.0 && x[1] == 99.0); // x not modified
    }

    // Test 4: n <= 0 returns 0 and does not touch x.
    {
        double x[1] = {42.0};
        double A[] = {1.0};
        double b[] = {1.0};
        assert(solveJacobi(0, A, b, x, 10, 1e-6) == 0.0);
        assert(x[0] == 42.0);
    }

    // Test 5: Larger problem, check residual norm after convergence.
    {
        int n = 4;
        double A[] = {
            5.0, 1.0, 0.5, 0.2,
            1.0, 6.0, 0.3, 0.1,
            0.5, 0.3, 7.0, 0.4,
            0.2, 0.1, 0.4, 8.0
        };
        double b[] = {1.0, 2.0, 3.0, 4.0};
        double x[4];
        double tol = 1e-8;
        solveJacobi(n, A, b, x, 1000, tol);
        // Verify actual residual norm < tol
        double norm_sq = 0.0;
        for (int i = 0; i < n; ++i) {
            double sum = 0.0;
            for (int j = 0; j < n; ++j) {
                sum += A[i * n + j] * x[j];
            }
            norm_sq += (sum - b[i]) * (sum - b[i]);
        }
        assert(std::sqrt(norm_sq) < tol);
    }

    // Test 6: Single equation (n=1) solves exactly.
    {
        int n = 1;
        double A[] = {7.0};
        double b[] = {21.0};
        double x[1];
        double iters = solveJacobi(n, A, b, x, 5, 1e-15);
        assert(std::abs(x[0] - 3.0) < 1e-12);
        assert(iters == 1.0); // one iteration gives exact solution
    }

    return 0;
}
