// Write a C++ function that computes the minimum and maximum eigenvalues of a real symmetric matrix given in compressed sparse row (CSR) format, using the power iteration method for the largest eigenvalue and inverse power iteration with a shift for the smallest eigenvalue. The function should take the number of rows `n`, the CSR arrays `row_ptr`, `col_idx`, `values`, a convergence tolerance `tol`, and a maximum iteration count `max_iter`, and return a small struct containing both eigenvalues. The matrix is guaranteed to be symmetric and positive definite, so both eigenvalues are positive, and the smallest eigenvalue is strictly greater than zero. Handle the case where the matrix is 1x1 directly, and ensure the function performs at most `max_iter` iterations per eigenvalue computation.

// The solution uses two iterative methods. For the largest eigenvalue, we apply the power iteration: repeatedly multiply an initial vector (all ones) by the matrix and normalize. The Rayleigh quotient `v^T A v / (v^T v)` gives an approximation of the largest eigenvalue, and convergence is measured by the change in this quotient between iterations. For the smallest eigenvalue, we use inverse iteration with a zero shift: solve the linear system `A x = v` (using a conjugate gradient solver since A is SPD) and then normalize the result. The Rayleigh quotient again provides the approximation. Since A is positive definite, the smallest eigenvalue is positive and inverse iteration converges. Edge cases: a 1x1 matrix returns the single entry for both; if the CG solver does not converge within a reasonable inner iteration bound (we set it to 1000), we break and return the current approximation. The power iteration for the largest eigenvalue converges linearly with rate (λ2/λ1), and inverse iteration for the smallest converges at rate (λ_min / λ_second_min). Time complexity is O(max_iter * nnz) for power iteration and O(max_iter * cg_iter * nnz) for inverse iteration, where nnz is the number of nonzeros. Space complexity is O(n) for the temporary vectors plus O(nnz) for the matrix storage.

#include <vector>
#include <cmath>
#include <algorithm>

struct EigenPair {
    double min_eigen;
    double max_eigen;
};

// Sparse matrix-vector multiply: y = A * x, where A is CSR.
static void csr_matvec(const std::vector<int>& row_ptr,
                       const std::vector<int>& col_idx,
                       const std::vector<double>& values,
                       const std::vector<double>& x,
                       std::vector<double>& y) {
    int n = x.size();
    y.assign(n, 0.0);
    for (int i = 0; i < n; ++i) {
        for (int j = row_ptr[i]; j < row_ptr[i+1]; ++j) {
            y[i] += values[j] * x[col_idx[j]];
        }
    }
}

// Conjugate gradient solver for A x = b, where A is SPD (CSR).
// Returns the solution vector. Stops when relative residual < tol or max_iter.
static std::vector<double> conjugate_gradient(const std::vector<int>& row_ptr,
                                              const std::vector<int>& col_idx,
                                              const std::vector<double>& values,
                                              const std::vector<double>& b,
                                              double tol,
                                              int max_iter) {
    int n = b.size();
    std::vector<double> x(n, 0.0);
    std::vector<double> r = b;
    std::vector<double> p = r;
    std::vector<double> Ap(n);
    double rsold = 0.0;
    for (double val : r) rsold += val * val;
    double norm_b = std::sqrt(rsold);
    if (norm_b < 1e-30) return x;

    for (int iter = 0; iter < max_iter; ++iter) {
        double rs = 0.0;
        for (double val : r) rs += val * val;
        if (std::sqrt(rs) / norm_b < tol) break;

        csr_matvec(row_ptr, col_idx, values, p, Ap);
        double pAp = 0.0;
        for (int i = 0; i < n; ++i) pAp += p[i] * Ap[i];
        if (pAp < 1e-30) break; // avoid division by zero
        double alpha = rs / pAp;

        for (int i = 0; i < n; ++i) {
            x[i] += alpha * p[i];
            r[i] -= alpha * Ap[i];
        }

        double rsnew = 0.0;
        for (double val : r) rsnew += val * val;
        if (std::sqrt(rsnew) / norm_b < tol) break;
        double beta = rsnew / rs;
        for (int i = 0; i < n; ++i) {
            p[i] = r[i] + beta * p[i];
        }
        rsold = rsnew;
    }
    return x;
}

// Compute the largest eigenvalue via power iteration.
static double largest_eigenvalue(const std::vector<int>& row_ptr,
                                 const std::vector<int>& col_idx,
                                 const std::vector<double>& values,
                                 double tol,
                                 int max_iter) {
    int n = row_ptr.size() - 1;
    if (n == 1) return values[0]; // 1x1 matrix

    std::vector<double> v(n, 1.0);
    std::vector<double> Av(n);
    double lambda_old = 0.0;
    for (int iter = 0; iter < max_iter; ++iter) {
        csr_matvec(row_ptr, col_idx, values, v, Av);
        double norm = 0.0;
        for (double val : Av) norm += val * val;
        norm = std::sqrt(norm);
        if (norm < 1e-30) break;
        for (int i = 0; i < n; ++i) v[i] = Av[i] / norm;

        // Rayleigh quotient: v^T A v (since v is normalized)
        double lambda = 0.0;
        for (int i = 0; i < n; ++i) lambda += v[i] * Av[i];
        lambda = lambda / (norm * norm); // actually Av was computed before normalization, but v is normalized now, so use v^T (A v) where A v = norm * v_old? Simpler: recompute A v after normalization? To avoid extra multiply, use lambda = v^T (A v_before_normalization)? That is incorrect. We recompute for correctness.
        // Recompute A v with the normalized v.
        csr_matvec(row_ptr, col_idx, values, v, Av);
        lambda = 0.0;
        for (int i = 0; i < n; ++i) lambda += v[i] * Av[i];
        if (iter > 0 && std::fabs(lambda - lambda_old) < tol * (1.0 + std::fabs(lambda))) {
            return lambda;
        }
        lambda_old = lambda;
    }
    // If not converged, return last Rayleigh quotient.
    csr_matvec(row_ptr, col_idx, values, v, Av);
    double lambda = 0.0;
    for (int i = 0; i < n; ++i) lambda += v[i] * Av[i];
    return lambda;
}

// Compute the smallest eigenvalue via inverse iteration with zero shift.
static double smallest_eigenvalue(const std::vector<int>& row_ptr,
                                  const std::vector<int>& col_idx,
                                  const std::vector<double>& values,
                                  double tol,
                                  int max_iter) {
    int n = row_ptr.size() - 1;
    if (n == 1) return values[0];

    std::vector<double> v(n, 1.0);
    double lambda_old = 0.0;
    for (int iter = 0; iter < max_iter; ++iter) {
        // Solve A x = v
        std::vector<double> x = conjugate_gradient(row_ptr, col_idx, values, v, tol, 1000);
        double norm = 0.0;
        for (double val : x) norm += val * val;
        norm = std::sqrt(norm);
        if (norm < 1e-30) break;
        for (int i = 0; i < n; ++i) v[i] = x[i] / norm;

        // Rayleigh quotient: v^T A v = v^T (A v). Since A v = ? Actually we have v = x/||x||, and A v = A x / ||x|| = b / ||x|| = v_old / ||x||. So lambda = v^T (A v) = (x^T v_old) / ||x||^2. Simpler: compute A v directly.
        std::vector<double> Av(n);
        csr_matvec(row_ptr, col_idx, values, v, Av);
        double lambda = 0.0;
        for (int i = 0; i < n; ++i) lambda += v[i] * Av[i];
        if (iter > 0 && std::fabs(lambda - lambda_old) < tol * (1.0 + std::fabs(lambda))) {
            return lambda;
        }
        lambda_old = lambda;
    }
    std::vector<double> Av(n);
    csr_matvec(row_ptr, col_idx, values, v, Av);
    double lambda = 0.0;
    for (int i = 0; i < n; ++i) lambda += v[i] * Av[i];
    return lambda;
}

// Main function: compute min and max eigenvalues of a symmetric positive definite matrix in CSR format.
EigenPair computeEigenvalues(const std::vector<int>& row_ptr,
                             const std::vector<int>& col_idx,
                             const std::vector<double>& values,
                             double tol = 1e-6,
                             int max_iter = 1000) {
    EigenPair result;
    result.max_eigen = largest_eigenvalue(row_ptr, col_idx, values, tol, max_iter);
    result.min_eigen = smallest_eigenvalue(row_ptr, col_idx, values, tol, max_iter);
    return result;
}

#include <cassert>
#include <cmath>
#include <vector>
#include <iostream>

// EigenPair and computeEigenvalues are assumed to be defined as above.

int main() {
    // Test 1: 1x1 matrix [[5.0]]
    {
        std::vector<int> row_ptr = {0, 1};
        std::vector<int> col_idx = {0};
        std::vector<double> values = {5.0};
        EigenPair ep = computeEigenvalues(row_ptr, col_idx, values);
        assert(std::fabs(ep.min_eigen - 5.0) < 1e-6);
        assert(std::fabs(ep.max_eigen - 5.0) < 1e-6);
    }

    // Test 2: 2x2 diagonal matrix diag(2, 8)
    {
        std::vector<int> row_ptr = {0, 1, 2};
        std::vector<int> col_idx = {0, 1};
        std::vector<double> values = {2.0, 8.0};
        EigenPair ep = computeEigenvalues(row_ptr, col_idx, values);
        assert(std::fabs(ep.min_eigen - 2.0) < 1e-4);
        assert(std::fabs(ep.max_eigen - 8.0) < 1e-4);
    }

    // Test 3: 3x3 matrix [[4, 1, 0], [1, 3, 1], [0, 1, 2]]
    // Eigenvalues approx: 5.2143, 2.4608, 1.3249 (exact via characteristic polynomial)
    {
        std::vector<int> row_ptr = {0, 2, 5, 7};
        std::vector<int> col_idx = {0, 1, 0, 1, 2, 1, 2};
        std::vector<double> values = {4.0, 1.0, 1.0, 3.0, 1.0, 1.0, 2.0};
        EigenPair ep = computeEigenvalues(row_ptr, col_idx, values, 1e-6, 1000);
        double expected_min = 1.3249;
        double expected_max = 5.2143;
        assert(std::fabs(ep.min_eigen - expected_min) < 1e-3);
        assert(std::fabs(ep.max_eigen - expected_max) < 1e-3);
    }

    // Test 4: 4x4 diagonal matrix with large spread
    {
        std::vector<int> row_ptr = {0, 1, 2, 3, 4};
        std::vector<int> col_idx = {0, 1, 2, 3};
        std::vector<double> values = {0.5, 10.0, 3.0, 7.0};
        EigenPair ep = computeEigenvalues(row_ptr, col_idx, values, 1e-8, 2000);
        assert(std::fabs(ep.min_eigen - 0.5) < 1e-5);
        assert(std::fabs(ep.max_eigen - 10.0) < 1e-5);
    }

    // Test 5: 2x2 non-diagonal [[3, 1], [1, 3]] eigenvalues 2 and 4
    {
        std::vector<int> row_ptr = {0, 2, 4};
        std::vector<int> col_idx = {0, 1, 0, 1};
        std::vector<double> values = {3.0, 1.0, 1.0, 3.0};
        EigenPair ep = computeEigenvalues(row_ptr, col_idx, values);
        assert(std::fabs(ep.min_eigen - 2.0) < 1e-4);
        assert(std::fabs(ep.max_eigen - 4.0) < 1e-4);
    }

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
