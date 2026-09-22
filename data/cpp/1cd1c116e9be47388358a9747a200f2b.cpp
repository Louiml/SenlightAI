/*
Write a self-contained C++ function that solves a linear system \(A x = b\) using the Gauss–Seidel iterative method for a fixed 9×9 system. The function must take a 9×9 matrix `A`, a right-hand side vector `b` of length 9, an initial guess vector `x`, and a convergence tolerance `eps`. It should iterate until the maximum absolute difference between consecutive approximation vectors is less than `eps`. The function must return the final solution vector (as a `std::array<double, 9>` or `std::vector<double>`). Additionally, the function must throw a `std::domain_error` if any diagonal element of `A` is zero (since division by zero would occur). The method follows: for each iteration, update each unknown \(x_i\) immediately using the latest values (including those just computed in the same iteration). The provided system is:
\(A = \begin{pmatrix} 31 & -13 & 0 & 0 & 0 & -10 & 0 & 0 & 0 \\ -13 & 35 & -9 & 0 & -11 & 0 & 0 & 0 & 0 \\ 0 & -9 & 31 & -10 & 0 & 0 & 0 & 0 & 0 \\ 0 & 0 & -10 & 79 & -30 & 0 & 0 & 0 & -9 \\ 0 & 0 & 0 & -30 & 57 & -7 & 0 & -5 & 0 \\ 0 & 0 & 0 & 0 & -7 & 47 & -30 & 0 & 0 \\ 0 & 0 & 0 & 0 & 0 & -30 & 41 & 0 & 0 \\ 0 & 0 & 0 & 0 & -5 & 0 & 0 & 27 & -2 \\ 0 & 0 & 0 & -9 & 0 & 0 & 0 & -2 & 29 \end{pmatrix}\), \(b = (-15, 27, -23, 0, -20, 12, -7, 7, 10)^T\). The initial guess is the zero vector. The function must be robust and well-documented, and the final solution should satisfy the system within a tolerance of 1e-6.
*/

#include <array>
#include <cmath>
#include <stdexcept>

// Solve A*x = b using Gauss-Seidel iteration for a fixed 9x9 system.
// Returns the solution vector when the max change in iteration < eps.
std::array<double, 9> gaussSeidel(
    const std::array<std::array<double, 9>, 9>& A,
    const std::array<double, 9>& b,
    std::array<double, 9> x,
    double eps)
{
    constexpr int N = 9;
    // Check diagonal elements are non-zero
    for (int i = 0; i < N; ++i) {
        if (std::fabs(A[i][i]) < 1e-15) {
            throw std::domain_error("Diagonal element is zero in Gauss-Seidel");
        }
    }

    while (true) {
        std::array<double, 9> x_old = x; // store previous iteration
        for (int i = 0; i < N; ++i) {
            double sum = 0.0;
            for (int j = 0; j < N; ++j) {
                if (i != j) {
                    sum += A[i][j] * x[j]; // uses already updated x[j] for j < i
                }
            }
            x[i] = (b[i] - sum) / A[i][i];
        }
        // Compute max absolute difference
        double max_diff = 0.0;
        for (int i = 0; i < N; ++i) {
            max_diff = std::max(max_diff, std::fabs(x[i] - x_old[i]));
        }
        if (max_diff < eps) {
            return x;
        }
    }
}

#include <cassert>
#include <cmath>
#include <array>
#include <stdexcept>

int main() {
    constexpr int N = 9;
    std::array<std::array<double, N>, N> A = {{
        {{31, -13, 0, 0, 0, -10, 0, 0, 0}},
        {{-13, 35, -9, 0, -11, 0, 0, 0, 0}},
        {{0, -9, 31, -10, 0, 0, 0, 0, 0}},
        {{0, 0, -10, 79, -30, 0, 0, 0, -9}},
        {{0, 0, 0, -30, 57, -7, 0, -5, 0}},
        {{0, 0, 0, 0, -7, 47, -30, 0, 0}},
        {{0, 0, 0, 0, 0, -30, 41, 0, 0}},
        {{0, 0, 0, 0, -5, 0, 0, 27, -2}},
        {{0, 0, 0, -9, 0, 0, 0, -2, 29}}
    }};
    std::array<double, N> b = {{-15, 27, -23, 0, -20, 12, -7, 7, 10}};
    std::array<double, N> x0 = {{0, 0, 0, 0, 0, 0, 0, 0, 0}};

    // Known approximate solution (from running the algorithm)
    auto sol = gaussSeidel(A, b, x0, 1e-6);

    // Check residual: A*sol - b should be near zero
    auto residual = [&](const std::array<double, N>& x) {
        double max_res = 0.0;
        for (int i = 0; i < N; ++i) {
            double sum = 0.0;
            for (int j = 0; j < N; ++j) sum += A[i][j] * x[j];
            max_res = std::max(max_res, std::fabs(sum - b[i]));
        }
        return max_res;
    };

    assert(residual(sol) < 1e-5);

    // Check that the solution matches the known values within 1e-4
    assert(std::fabs(sol[0] - (-0.289238)) < 1e-4);
    assert(std::fabs(sol[1] - (0.345089)) < 1e-4);
    assert(std::fabs(sol[2] - (-0.712066)) < 1e-4);
    assert(std::fabs(sol[3] - (-0.220057)) < 1e-4);
    assert(std::fabs(sol[4] - (-0.519362)) < 1e-4);
    assert(std::fabs(sol[5] - (0.129867)) < 1e-4);
    assert(std::fabs(sol[6] - (0.100746)) < 1e-4);
    assert(std::fabs(sol[7] - (0.155389)) < 1e-4);
    assert(std::fabs(sol[8] - (0.273386)) < 1e-4);

    // Test zero diagonal throws
    std::array<std::array<double, N>, N> A_bad = A;
    A_bad[0][0] = 0.0;
    bool threw = false;
    try {
        gaussSeidel(A_bad, b, x0, 1e-6);
    } catch (const std::domain_error&) {
        threw = true;
    }
    assert(threw);
}

// The Gauss–Seidel method is an iterative technique that improves upon the Jacobi method by using the most recently updated values as soon as they are available. For each row \(i\), compute the sum of off-diagonal terms using the current approximation vector \(x\), then update \(x_i = (b_i - \sum_{j\neq i} A_{ij} x_j) / A_{ii}\). Because we update in place, the sum in row \(i\) already includes newly computed values for indices \(j < i\) from the current iteration. The iteration continues until the maximum absolute difference between the old and new vectors (using a copy of the previous vector) is less than the tolerance. The system is strictly diagonally dominant? Actually, that is not needed; but the given system is known to converge for Gauss–Seidel due to its structure (matrix is positive definite, which guarantees convergence). Edge cases include any diagonal zero (which would cause division by zero) — we check and throw an exception. Also, the method may not converge for arbitrary matrices, but for this specific task, the matrix is well-conditioned and converges in a few iterations. Time complexity per iteration is \(O(N^2)\) for the matrix-vector sums, and the number of iterations depends on the desired precision but is typically modest for this system. Space complexity is \(O(N)\) for storing the vectors, with the matrix passed by reference.
