/*
Implement a C++ function `solveDiagonalDominant` that, given the matrix size `n` and the number of threads `numThreads` (which is unused in the actual computation but provided for interface consistency), solves the linear system `A x = b` where `A` is the `n x n` matrix with 2 on the diagonal and 1 everywhere else, and `b` is a vector of all `n+1`. Use the conjugate gradient method exactly as presented in the snippet: start with a random initial guess from a seeded PRNG (e.g., use `std::mt19937` seeded with a fixed value, and uniform real distribution in [-2, 2]), perform the iterations until the relative residual norm (computed as `norm(r) / norm(b)`) is below `1e-5` or a maximum of 1000 iterations, then return the final solution vector. The function must be self-contained, not rely on MPI or any external matrix libraries, and be suitable for running on a single thread. Assume `n` is at least 1 and that the system is well-posed; the returned vector should contain the approximate solution of the linear system.
*/
#include <vector>
#include <cmath>
#include <random>
#include <stdexcept>

// Solve A x = b where A is n x n with diagonal 2, off-diagonal 1, and b = (n+1) for all entries.
// Uses conjugate gradient with random initial guess in [-2, 2], relative residual tolerance 1e-5, max 1000 iterations.
// Returns the approximate solution vector of size n.
std::vector<double> solveDiagonalDominant(int n, int /*numThreads*/) {
    if (n <= 0) {
        throw std::invalid_argument("Matrix size must be positive");
    }

    // Build matrix A (n x n) and right-hand side b
    std::vector<std::vector<double>> A(n, std::vector<double>(n, 1.0));
    for (int i = 0; i < n; ++i) {
        A[i][i] = 2.0;
    }
    std::vector<double> b(n, static_cast<double>(n + 1));

    // Random initial guess x in [-2, 2]
    std::mt19937 rng(42);  // fixed seed for reproducibility
    std::uniform_real_distribution<double> dist(-2.0, 2.0);
    std::vector<double> x(n);
    for (int i = 0; i < n; ++i) {
        x[i] = dist(rng);
    }

    // Helper: compute A * v (n x n matrix times vector)
    auto matVecMul = [&](const std::vector<double>& v) {
        std::vector<double> result(n, 0.0);
        for (int i = 0; i < n; ++i) {
            double sum = 0.0;
            for (int j = 0; j < n; ++j) {
                sum += A[i][j] * v[j];
            }
            result[i] = sum;
        }
        return result;
    };

    // Helper: dot product
    auto dot = [&](const std::vector<double>& a, const std::vector<double>& c) {
        double sum = 0.0;
        for (int i = 0; i < n; ++i) {
            sum += a[i] * c[i];
        }
        return sum;
    };

    // Helper: vector norm (L2)
    auto norm = [&](const std::vector<double>& v) {
        return std::sqrt(dot(v, v));
    };

    // Initial residual and search direction
    std::vector<double> r = b;
    auto Ax = matVecMul(x);
    for (int i = 0; i < n; ++i) {
        r[i] -= Ax[i];
    }
    std::vector<double> z = r;

    double eps = 1e-5;
    int maxIter = 1000;
    double bNorm = norm(b);
    if (bNorm < 1e-12) {
        // Degenerate case: b is zero, solution is zero
        return std::vector<double>(n, 0.0);
    }

    for (int iter = 0; iter < maxIter; ++iter) {
        double rDotR = dot(r, r);
        auto Az = matVecMul(z);
        double zDotAz = dot(z, Az);
        if (std::abs(zDotAz) < 1e-12) {
            break; // avoid division by zero, though should not happen
        }
        double alpha = rDotR / zDotAz;
        for (int i = 0; i < n; ++i) {
            x[i] += alpha * z[i];
        }
        std::vector<double> r1(n);
        for (int i = 0; i < n; ++i) {
            r1[i] = r[i] - alpha * Az[i];
        }
        double r1DotR1 = dot(r1, r1);
        double beta = r1DotR1 / rDotR;
        for (int i = 0; i < n; ++i) {
            z[i] = r1[i] + beta * z[i];
        }
        r = r1;

        // Check stopping criterion: ||r|| / ||b|| < eps
        if (norm(r) / bNorm < eps) {
            break;
        }
    }
    return x;
}
#include <cassert>
#include <cmath>
#include <vector>

// Declaration of the solution function (assume it's in the same translation unit or included)
std::vector<double> solveDiagonalDominant(int n, int numThreads);

int main() {
    // Test 1: n=1, A=[2], b=[2], solution should be [1]
    {
        auto sol = solveDiagonalDominant(1, 1);
        assert(sol.size() == 1);
        assert(std::abs(sol[0] - 1.0) < 1e-4);
    }

    // Test 2: n=2, A = [[2,1],[1,2]], b = [3,3], solution should be [1,1]
    {
        auto sol = solveDiagonalDominant(2, 1);
        assert(sol.size() == 2);
        assert(std::abs(sol[0] - 1.0) < 1e-4);
        assert(std::abs(sol[1] - 1.0) < 1e-4);
    }

    // Test 3: n=5, solution should be all ones
    {
        auto sol = solveDiagonalDominant(5, 2);
        assert(sol.size() == 5);
        for (int i = 0; i < 5; ++i) {
            assert(std::abs(sol[i] - 1.0) < 1e-4);
        }
    }

    // Test 4: n=10, check residual norm is small (rel err < 1e-4)
    {
        int n = 10;
        auto sol = solveDiagonalDominant(n, 4);
        assert(sol.size() == n);
        // Compute residual b - A*x
        std::vector<double> residual(n, 0.0);
        for (int i = 0; i < n; ++i) {
            double Ax = 0.0;
            for (int j = 0; j < n; ++j) {
                Ax += (i == j ? 2.0 : 1.0) * sol[j];
            }
            residual[i] = (n + 1.0) - Ax;
        }
        double normRes = 0.0;
        for (double v : residual) normRes += v * v;
        normRes = std::sqrt(normRes);
        double normB = std::sqrt(static_cast<double>(n) * (n + 1.0) * (n + 1.0));
        assert(normRes / normB < 1e-4);
    }

    // Test 5: n=50 (larger), solution should still converge to near 1
    {
        int n = 50;
        auto sol = solveDiagonalDominant(n, 8);
        assert(sol.size() == n);
        double maxDiff = 0.0;
        for (double v : sol) {
            maxDiff = std::max(maxDiff, std::abs(v - 1.0));
        }
        assert(maxDiff < 1e-3);
    }

    // Test 6: Throw on invalid n
    {
        bool threw = false;
        try {
            solveDiagonalDominant(0, 1);
        } catch (...) {
            threw = true;
        }
        assert(threw);
    }

    return 0;
}
// The conjugate gradient (CG) method is an iterative algorithm for solving symmetric positive definite linear systems. The matrix here is symmetric and diagonally dominant (diagonal 2, off-diagonal 1), so it is positive definite. The algorithm begins with an arbitrary initial guess `x` (here random in [-2,2]). The residual `r = b - A x` and search direction `z = r` are initialized. Each iteration computes alpha = (r·r)/((A·z)·z), updates x = x + alpha*z, computes new residual r1 = r - alpha*(A·z), computes beta = (r1·r1)/(r·r), updates z = r1 + beta*z, and sets r = r1. The stopping criterion checks whether the ratio of the L2 norm of the residual to the L2 norm of b is below epsilon (1e-5). Key operations include matrix-vector multiplication and vector dot products. Complexity: each iteration requires one matrix-vector product (O(n^2)) and several O(n) vector operations. The number of iterations in exact arithmetic is at most n, but in practice due to floating-point and stopping tolerance, it may stop earlier. For this specific matrix, the solution is known: all entries are 1 (since A * 1 = (2+(n-1)) = n+1 = b), so the algorithm should converge to exactly that after one or a few iterations, but the random start and tolerance will cause it to converge in a few iterations. Edge cases: n=1 works (matrix [2], b=[2]), solution [1]. The function must avoid division by zero: ensure the denominator (A·z)·z is nonzero; for this positive definite matrix it is. Time complexity O(iter * n^2), typically few iterations, space O(n^2) for matrix storage (or we could compute A*z without storing full matrix, but for clarity we store). The task requires returning the solution vector; we use `std::vector<double>`.
