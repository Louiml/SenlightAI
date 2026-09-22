// Write a C++ function named `computeIterationCount` that, given a square matrix `A` (as a `std::vector<std::vector<double>>`), a right-hand side vector `b`, an initial guess vector `x0`, a maximum iteration limit `maxIter`, and a tolerance `tol`, returns the number of Jacobi iterations (including the first iteration) required to make the 2-norm (Euclidean norm) of the difference between consecutive iterates less than `tol`. The function must stop early if `maxIter` iterations have been performed (returning exactly `maxIter`), and it must handle the case where the input matrix has a zero on the diagonal by immediately returning 0 (since division by zero is undefined). The function should use a standard Jacobi update: for each row `i`, compute `new_x[i] = (b[i] - sum_{j != i} A[i][j] * old_x[j]) / A[i][i]`, then replace `old_x` with `new_x` and compute the 2-norm of their difference. The initial difference is considered infinite (so at least one iteration is always performed). The function must not modify its inputs and should work for any positive dimension `n`.

#include <cassert>
#include <vector>

// The solution function is declared above; tests are below.

int main() {
    // Test 1: Simple 2x2 system with known solution.
    // A = [[4, 1], [1, 3]], b = [1, 2], x0=[0,0]
    // True solution is [0.0909, 0.6364]? Not needed; just check iteration count.
    std::vector<std::vector<double>> A1 = {{4.0, 1.0}, {1.0, 3.0}};
    std::vector<double> b1 = {1.0, 2.0};
    std::vector<double> x0_1 = {0.0, 0.0};
    // Very loose tolerance should converge in one iteration.
    int iter1 = computeIterationCount(A1, b1, x0_1, 100, 1e-1);
    assert(iter1 == 1 || iter1 == 2); // Should stop after 1 or 2 iterations.

    // Test 2: Zero diagonal returns 0.
    std::vector<std::vector<double>> A2 = {{0.0, 2.0}, {1.0, 3.0}};
    std::vector<double> b2 = {1.0, 1.0};
    std::vector<double> x0_2 = {0.0, 0.0};
    assert(computeIterationCount(A2, b2, x0_2, 10, 1e-6) == 0);

    // Test 3: MaxIter cap.
    // Use a system that converges slowly; ensure we don't exceed maxIter.
    std::vector<std::vector<double>> A3 = {{2.0, 1.0}, {1.0, 2.0}};
    std::vector<double> b3 = {1.0, 1.0};
    std::vector<double> x0_3 = {0.0, 0.0};
    int iter3 = computeIterationCount(A3, b3, x0_3, 5, 1e-15);
    assert(iter3 <= 5);
    assert(iter3 == 5); // With tight tolerance, should hit the limit.

    // Test 4: Already converged initial guess? We force at least one iteration.
    std::vector<std::vector<double>> A4 = {{2.0, 0.0}, {0.0, 2.0}};
    std::vector<double> b4 = {4.0, 6.0};
    std::vector<double> x0_4 = {2.0, 3.0};
    // After one iteration, new_x = (4/2, 6/2) = (2,3), diff=0, so returns 1.
    int iter4 = computeIterationCount(A4, b4, x0_4, 100, 1e-10);
    assert(iter4 == 1);

    // Test 5: Larger system with exact convergence after one iteration.
    std::vector<std::vector<double>> A5 = {{1.0, 0.0}, {0.0, 1.0}};
    std::vector<double> b5 = {5.0, -2.0};
    std::vector<double> x0_5 = {0.0, 0.0};
    int iter5 = computeIterationCount(A5, b5, x0_5, 100, 1e-12);
    assert(iter5 == 1);

    // Test 6: n=0 returns 0 (safety).
    std::vector<std::vector<double>> A6 = {};
    std::vector<double> b6 = {};
    std::vector<double> x0_6 = {};
    assert(computeIterationCount(A6, b6, x0_6, 10, 1e-6) == 0);

    // Test 7: Tolerance very large, should stop after one iteration.
    std::vector<std::vector<double>> A7 = {{3.0, 0.0}, {0.0, 3.0}};
    std::vector<double> b7 = {3.0, 3.0};
    std::vector<double> x0_7 = {0.0, 0.0};
    int iter7 = computeIterationCount(A7, b7, x0_7, 100, 100.0);
    assert(iter7 == 1);

    // Test 8: Non-convergent (or slowly) with small maxIter, check limit.
    std::vector<std::vector<double>> A8 = {{1.0, 0.99}, {0.99, 1.0}};
    std::vector<double> b8 = {1.0, -1.0};
    std::vector<double> x0_8 = {0.0, 0.0};
    int iter8 = computeIterationCount(A8, b8, x0_8, 3, 1e-10);
    assert(iter8 == 3);

    return 0;
}

#include <vector>
#include <cmath>
#include <limits>

// Compute the Euclidean norm of the difference between two vectors.
static double differenceNorm(const std::vector<double>& a, const std::vector<double>& b) {
    double sum = 0.0;
    for (size_t i = 0; i < a.size(); ++i) {
        double diff = a[i] - b[i];
        sum += diff * diff;
    }
    return std::sqrt(sum);
}

/**
 * Perform Jacobi iterations to solve A*x = b starting from x0.
 * Returns the number of iterations performed, or 0 if the matrix
 * has a zero diagonal entry (making the method undefined).
 * The loop stops when the 2-norm of successive iterates is < tol,
 * or when maxIter iterations have been performed (returns maxIter).
 * Inputs are not modified.
 */
int computeIterationCount(const std::vector<std::vector<double>>& A,
                          const std::vector<double>& b,
                          const std::vector<double>& x0,
                          int maxIter,
                          double tol) {
    const int n = static_cast<int>(A.size());
    if (n == 0) return 0;

    // Check for zero diagonal entries.
    for (int i = 0; i < n; ++i) {
        if (A[i][i] == 0.0) return 0;
    }

    std::vector<double> old_x = x0;
    std::vector<double> new_x(n, 0.0);

    // Force at least one iteration; initial diff is "infinite".
    double diff = std::numeric_limits<double>::infinity();
    int iter = 0;

    while (iter < maxIter && diff > tol) {
        ++iter;

        // Jacobi update using old_x values.
        for (int i = 0; i < n; ++i) {
            double sum = 0.0;
            for (int j = 0; j < n; ++j) {
                if (j != i) {
                    sum += A[i][j] * old_x[j];
                }
            }
            new_x[i] = (b[i] - sum) / A[i][i];
        }

        // Compute 2-norm of difference between new and old.
        diff = differenceNorm(new_x, old_x);

        // Prepare for next iteration.
        old_x = new_x;
    }

    return iter;
}

// The algorithm is a straightforward implementation of the Jacobi iterative method for solving linear systems. The main steps are: (1) define a helper to compute the Euclidean norm of a vector difference; (2) initialize the current solution to `x0` and set the difference norm to `INFINITY` to force at least one iteration; (3) loop while iterations performed < `maxIter` and the current difference norm > `tol`. In each iteration, compute a new vector `new_x` using the Jacobi update formula. For each row, if the diagonal element `A[i][i]` is zero (or essentially zero, here we check exact zero to keep it simple), return 0 immediately because the method is invalid. Otherwise, accumulate the sum of off-diagonal products using the old solution, then compute the new value. After updating all entries, compute the 2-norm of `new_x - old_x`, then copy `new_x` into `old_x` for the next iteration. Increment the iteration counter. The function returns the iteration count at which convergence was achieved or `maxIter` if the limit was reached. Edge cases: empty matrix (n=0) – but we assume positive dimension, still we can guard and return 0; matrix with zero diagonal – return 0; tolerance too large (e.g., tol=1e10) – the loop will stop after one iteration because the difference is finite (unless it’s already smaller, but since we initialize diff=INF, we still do one iteration). Time complexity: each iteration is O(n^2), and if we have K iterations (K ≤ maxIter), total is O(K * n^2). Space complexity: O(n) for the two solution vectors.
