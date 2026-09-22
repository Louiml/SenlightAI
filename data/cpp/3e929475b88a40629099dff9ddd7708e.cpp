// Write a C++ function `void scaled_dgemv(int n, double* A, double* x, double* y, double alpha)` that performs the operation `Y := alpha * A * X + Y`, where `A` is an `n`-by-`n` matrix stored in row-major format, and `X` and `Y` are vectors of length `n`. The function must be parallelized using OpenMP so that each row of `A` is processed independently by a different thread, without causing data races on `y`. The scalar `alpha` is a runtime parameter and may be zero, negative, or any double value. The function should preserve the original contents of `A` and `X`. You may assume `n` is positive, but handle `n = 1` gracefully. You are not allowed to use any external libraries beyond the standard C++ headers and OpenMP.

// The operation is a matrix-vector multiplication with a scaling factor, adding the result to the existing vector `y`. The main challenge is avoiding race conditions when multiple threads write to `y`. The simplest safe approach is to parallelize the outer loop over rows (`i`) and compute a local dot product (`t_dot`) for that row, then perform `y[i] += alpha * t_dot`. Since each thread handles a distinct `i`, no two threads write to the same `y[i]`, so no critical section is needed. The inner loop over `j` is not parallelized to avoid redundant overhead; each thread serially computes its own row's dot product. Edge cases include `n = 1` (single row, works fine) and `alpha = 0` (then `y` remains unchanged; the loop still runs but adds zero). The function uses `#pragma omp parallel for` with default `static` scheduling; for load balancing with large `n`, one could add `schedule(static)` but it is not required. Time complexity is `O(n^2)` due to the double loop; space complexity is `O(1)` extra (only a scalar `t_dot` per thread). The implementation must include `#include <omp.h>` and be compiled with `-fopenmp` (or equivalent) to enable parallelism. The function should use `const double*` for `A` and `x` to indicate they are read-only, and `double*` for `y` because it is modified in place.

#include <omp.h>

/**
 * Perform Y := alpha * A * X + Y.
 * A is n-by-n row-major, x and y are length n vectors.
 * Parallelized over rows using OpenMP.
 */
void scaled_dgemv(int n, const double* A, const double* x, double* y, double alpha) {
    #pragma omp parallel for
    for (int i = 0; i < n; ++i) {
        double t_dot = 0.0;
        const double* row = A + i * n;
        for (int j = 0; j < n; ++j) {
            t_dot += row[j] * x[j];
        }
        y[i] += alpha * t_dot;
    }
}

#include <cassert>
#include <cmath>
#include <vector>

// Placeholder for the solution function (include the header/implementation above)
// void scaled_dgemv(int n, const double* A, const double* x, double* y, double alpha);

int main() {
    // Test 1: Simple 2x2 case with alpha = 2
    {
        int n = 2;
        std::vector<double> A = {1.0, 2.0, 3.0, 4.0};
        std::vector<double> x = {1.0, 2.0};
        std::vector<double> y = {0.0, 0.0};
        double alpha = 2.0;
        scaled_dgemv(n, A.data(), x.data(), y.data(), alpha);
        assert(y[0] == 2.0 * (1.0*1.0 + 2.0*2.0) == 10.0);
        assert(y[1] == 2.0 * (3.0*1.0 + 4.0*2.0) == 22.0);
    }

    // Test 2: alpha = 0, y remains unchanged
    {
        int n = 3;
        std::vector<double> A = {1,2,3,4,5,6,7,8,9};
        std::vector<double> x = {1,1,1};
        std::vector<double> y = {5,6,7};
        std::vector<double> original_y = y;
        double alpha = 0.0;
        scaled_dgemv(n, A.data(), x.data(), y.data(), alpha);
        assert(y == original_y);
    }

    // Test 3: n = 1
    {
        int n = 1;
        std::vector<double> A = {42.0};
        std::vector<double> x = {2.0};
        std::vector<double> y = {0.5};
        double alpha = 0.5;
        scaled_dgemv(n, A.data(), x.data(), y.data(), alpha);
        double expected = 0.5 + 0.5 * (42.0 * 2.0); // 42.5
        assert(std::fabs(y[0] - expected) < 1e-12);
    }

    // Test 4: negative alpha and negative values
    {
        int n = 2;
        std::vector<double> A = {-1, -2, 3, 4};
        std::vector<double> x = {-2, 5};
        std::vector<double> y = {1, 2};
        double alpha = -1.0;
        scaled_dgemv(n, A.data(), x.data(), y.data(), alpha);
        // Row0: alpha*dot = -1 * ((-1)*(-2) + (-2)*5) = -1 * (2-10) = 8 → y[0] = 1+8 = 9
        // Row1: alpha*dot = -1 * (3*(-2) + 4*5) = -1 * (-6+20) = -14 → y[1] = 2-14 = -12
        assert(y[0] == 9.0);
        assert(y[1] == -12.0);
    }

    // Test 5: Larger n (e.g., 100) verify against serial computation
    {
        int n = 100;
        std::vector<double> A(n * n);
        std::vector<double> x(n);
        std::vector<double> y(n);
        std::vector<double> y_expected(n);
        for (int i = 0; i < n; ++i) {
            x[i] = (i % 7) * 0.5 - 3.0;
            y[i] = (i % 3) * 1.2;
            y_expected[i] = y[i];
            for (int j = 0; j < n; ++j) {
                A[i * n + j] = (i + j) % 5 - 2.0; // small values
            }
        }
        double alpha = 1.7;
        // Compute expected serially
        for (int i = 0; i < n; ++i) {
            double dot = 0.0;
            for (int j = 0; j < n; ++j) {
                dot += A[i * n + j] * x[j];
            }
            y_expected[i] += alpha * dot;
        }
        scaled_dgemv(n, A.data(), x.data(), y.data(), alpha);
        for (int i = 0; i < n; ++i) {
            assert(std::fabs(y[i] - y_expected[i]) < 1e-9);
        }
    }

    return 0;
}
