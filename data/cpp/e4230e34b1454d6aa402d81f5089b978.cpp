/*
Write a standalone C++ function that takes four parameters: a `size_t` integer `N` (the number of grid points along one axis), a `double` `delta` (the grid spacing), and two boolean flags `periodic_x` and `periodic_y` indicating whether boundary conditions are periodic or Neumann in the x and y directions respectively. The function must simulate the eigenvalue precomputation performed in the given snippet for a 2D Poisson solver: it should construct a 2D array (represented as a `std::vector<std::vector<double>>`) of size `Nx × Ny` where `Nx = (periodic_x ? N : N)` and `Ny = (periodic_y ? N : N)` (for this task, assume both dimensions use the same `N` and the flags independently determine the boundary type). For each grid point `(i,j)` with `0 ≤ i < Nx` and `0 ≤ j < Ny`, compute the eigenvalue reciprocal as `1 / (lambda_x + lambda_y)`, where `lambda_x` is computed using `compute_eigenvalue_periodic(i, delta, N)` if `periodic_x` is true, otherwise `compute_eigenvalue_neumann(i, delta, N)`, and similarly for `lambda_y` using index `j` and flag `periodic_y`. The eigenvalue formulas are: for Neumann, `2 * (cos(M_PI * index / (N - 1)) - 1) / (delta * delta)`; for periodic, `2 * (cos(2 * M_PI * index / N) - 1) / (delta * delta)`. Ensure the function is `const`-correct, uses proper namespaces, and returns the 2D vector. Handle edge cases: if `N < 2` for Neumann, the denominator `N - 1` becomes zero or negative; in such cases, you may assume `N ≥ 2` and assert this precondition inside the function. The function must be self-contained (include all necessary headers) and not rely on any external libraries other than the standard library.
*/
#include <vector>
#include <cmath>
#include <cassert>

// Compute eigenvalue for Neumann boundary conditions.
inline double compute_eigenvalue_neumann_2d(size_t index, double delta, size_t N) {
    assert(N >= 2 && "Neumann requires at least 2 grid points");
    return 2.0 * (std::cos(M_PI * static_cast<double>(index) / static_cast<double>(N - 1)) - 1.0) / (delta * delta);
}

// Compute eigenvalue for periodic boundary conditions.
inline double compute_eigenvalue_periodic_2d(size_t index, double delta, size_t N) {
    assert(N >= 1 && "Periodic requires at least 1 grid point");
    return 2.0 * (std::cos(2.0 * M_PI * static_cast<double>(index) / static_cast<double>(N)) - 1.0) / (delta * delta);
}

// Precompute reciprocal eigenvalues for a 2D Poisson solver with mixed boundary conditions.
// Returns a N x N matrix where element [i][j] = 1 / (lambda_x(i) + lambda_y(j)).
// The boundary condition type for each dimension is controlled by periodic_x and periodic_y.
std::vector<std::vector<double>> compute_eigenvalue_matrix(size_t N, double delta, bool periodic_x, bool periodic_y) {
    assert(N >= 2 && "Grid size must be at least 2 to support Neumann boundaries");
    std::vector<std::vector<double>> result(N, std::vector<double>(N, 0.0));

    for (size_t i = 0; i < N; ++i) {
        const double lambda_x = periodic_x
            ? compute_eigenvalue_periodic_2d(i, delta, N)
            : compute_eigenvalue_neumann_2d(i, delta, N);
        for (size_t j = 0; j < N; ++j) {
            const double lambda_y = periodic_y
                ? compute_eigenvalue_periodic_2d(j, delta, N)
                : compute_eigenvalue_neumann_2d(j, delta, N);
            result[i][j] = 1.0 / (lambda_x + lambda_y);
        }
    }
    return result;
}
#include <cassert>
#include <cmath>
#include <vector>

// Include the solution function here (or via #include "solution.h").

int main() {
    const double delta = 0.5;
    const size_t N = 4;

    // Test 1: Periodic in both directions.
    auto m1 = compute_eigenvalue_matrix(N, delta, true, true);
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < N; ++j) {
            double lx = 2.0 * (std::cos(2.0 * M_PI * static_cast<double>(i) / N) - 1.0) / (delta * delta);
            double ly = 2.0 * (std::cos(2.0 * M_PI * static_cast<double>(j) / N) - 1.0) / (delta * delta);
            assert(std::fabs(m1[i][j] - 1.0 / (lx + ly)) < 1e-12);
        }
    }

    // Test 2: Neumann in both directions.
    auto m2 = compute_eigenvalue_matrix(N, delta, false, false);
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < N; ++j) {
            double lx = 2.0 * (std::cos(M_PI * static_cast<double>(i) / (N - 1)) - 1.0) / (delta * delta);
            double ly = 2.0 * (std::cos(M_PI * static_cast<double>(j) / (N - 1)) - 1.0) / (delta * delta);
            assert(std::fabs(m2[i][j] - 1.0 / (lx + ly)) < 1e-12);
        }
    }

    // Test 3: Mixed periodic-x, Neumann-y.
    auto m3 = compute_eigenvalue_matrix(N, delta, true, false);
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < N; ++j) {
            double lx = 2.0 * (std::cos(2.0 * M_PI * static_cast<double>(i) / N) - 1.0) / (delta * delta);
            double ly = 2.0 * (std::cos(M_PI * static_cast<double>(j) / (N - 1)) - 1.0) / (delta * delta);
            assert(std::fabs(m3[i][j] - 1.0 / (lx + ly)) < 1e-12);
        }
    }

    // Test 4: Mixed Neumann-x, periodic-y.
    auto m4 = compute_eigenvalue_matrix(N, delta, false, true);
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < N; ++j) {
            double lx = 2.0 * (std::cos(M_PI * static_cast<double>(i) / (N - 1)) - 1.0) / (delta * delta);
            double ly = 2.0 * (std::cos(2.0 * M_PI * static_cast<double>(j) / N) - 1.0) / (delta * delta);
            assert(std::fabs(m4[i][j] - 1.0 / (lx + ly)) < 1e-12);
        }
    }

    // Test 5: Check matrix dimensions.
    assert(m1.size() == N && m1[0].size() == N);
    assert(m2.size() == N && m2[0].size() == N);

    // Test 6: Symmetry? For periodic-periodic, matrix is symmetric because lambda_x(i)=lambda_y(i).
    for (size_t i = 0; i < N; ++i)
        for (size_t j = 0; j < N; ++j)
            assert(std::fabs(m1[i][j] - m1[j][i]) < 1e-12);

    // Test 7: Boundary index 0 for Neumann yields denominator zero (infinite), but skip if not applicable.
    // For periodic-periodic, no division by zero; check that all values are finite.
    for (size_t i = 0; i < N; ++i)
        for (size_t j = 0; j < N; ++j)
            assert(std::isfinite(m1[i][j]));

    return 0;
}
// The solution computes eigenvalues for each dimension separately and then combines them additively, as in the original code where the reciprocal of the sum of three eigenvalues is stored. For this 2D task, we replicate the same logic but for two dimensions. The main algorithm iterates over all `Nx * Ny` grid points, and for each point calculates `lambda_x` and `lambda_y` using the appropriate boundary condition formula based on the flags. A key detail: for Neumann boundary conditions, the indices range from `0` to `N-1` inclusive, and the denominator in the cosine argument uses `N - 1`, which is why we assert `N ≥ 2`. For periodic boundary conditions, the denominator is `N`. The eigenvalue values are always non-positive (cosine values between -1 and 1, minus 1 gives ≤ 0), so the sum `lambda_x + lambda_y` is ≤ 0, and its reciprocal is well-defined unless both are zero (which only happens at index 0 for Neumann, giving `lambda = 0`; if both are zero then division by zero occurs — but in practice this won’t happen unless both dimensions are Neumann and index is (0,0), which would yield infinity; the original code doesn't protect against this either, so we leave it as-is). Time complexity is O(Nx * Ny) = O(N^2), and space complexity is O(N^2) for the result vector. Edge cases include periodic with `N = 1` (which is valid, cosine of 0), and Neumann with `N = 2` (denominator 1, valid). Assertions are used to enforce `N ≥ 2` for Neumann.
