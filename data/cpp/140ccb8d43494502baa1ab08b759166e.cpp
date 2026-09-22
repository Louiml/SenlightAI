// Write a C++ function that, given a vector of positive double values representing particle sizes and a user-supplied aggregation kernel function (a callable that takes two double sizes and returns a non-negative double rate), computes and returns a square matrix (as a `std::vector<std::vector<double>>`) where the entry at `[i][j]` equals `aggregation_kernel(sizes[i], sizes[j])`. The function must be templated on the kernel type, must respect `const` correctness (the input sizes vector and the kernel should not be modifiable), and must handle the edge case of an empty sizes vector by returning an empty matrix. The kernel should be invoked exactly once for each unordered pair (i, j) to avoid redundant computation, but the result must be stored symmetrically (i.e., `matrix[i][j] = matrix[j][i]`). For diagonal entries (i == j), compute the kernel once. Assume the kernel is deterministic and does not modify its arguments.

// The core idea is to precompute the aggregation kernel for all pairs of sizes. The main algorithm is straightforward: first, determine the number of sizes `n = sizes.size()`. Allocate an `n x n` matrix initialized to zero. Then iterate over all pairs where `i <= j` (or `i >= j`) to avoid duplicate calls to the kernel. For each such pair, compute `value = kernel(sizes[i], sizes[j])` and assign `matrix[i][j] = value` and `matrix[j][i] = value`. This ensures symmetry without recomputing the kernel twice for off-diagonal pairs. The function is templated so that any callable (function pointer, functor, lambda) that takes two `double` arguments and returns a `double` can be used. Edge cases: if `n == 0`, return an empty vector (no entries). If `n == 1`, the matrix is 1x1 with the diagonal set once. Negative values from the kernel are allowed by the specification? Actually, the task states the kernel returns a non-negative double, so we need not handle negative values specially. However, to be safe, we could assert or accept all values as given. But for robustness, we just store whatever the kernel returns. Time complexity: exactly `n*(n+1)/2` calls to the kernel, which is the minimum necessary to fill an `n x n` symmetric matrix. Space complexity: `O(n^2)` for the output matrix, plus `O(1)` auxiliary space. The use of `const` references for the sizes vector and the kernel (if it is a lambda, it is captured by value or reference as appropriate) ensures no accidental mutation.

#include <vector>
#include <cstddef>

// Compute a symmetric aggregation kernel matrix for given particle sizes.
// The kernel is a callable that takes two double sizes and returns a double rate.
// The result is a matrix where result[i][j] = kernel(sizes[i], sizes[j]).
// The kernel is called once per unordered pair (i, j) and the value is stored symmetrically.
template <typename Kernel>
std::vector<std::vector<double>> compute_kernel_matrix(
    const std::vector<double>& sizes,
    const Kernel& kernel) 
{
    const std::size_t n = sizes.size();
    // Edge case: empty input -> empty matrix.
    if (n == 0) {
        return {};
    }

    // Initialize an n x n matrix with zeros.
    std::vector<std::vector<double>> matrix(n, std::vector<double>(n, 0.0));

    // Iterate over all pairs i <= j to avoid redundant kernel calls.
    for (std::size_t i = 0; i < n; ++i) {
        for (std::size_t j = i; j < n; ++j) {
            // Compute the kernel value once.
            double value = kernel(sizes[i], sizes[j]);
            // Assign symmetrically.
            matrix[i][j] = value;
            matrix[j][i] = value;
        }
    }

    return matrix;
}

#include <cassert>
#include <vector>
#include <cmath>

// Example kernel: product of sizes.
double product_kernel(double a, double b) { return a * b; }

// Example kernel: sum of sizes.
double sum_kernel(double a, double b) { return a + b; }

// A functor kernel.
struct ConstantKernel {
    double operator()(double, double) const { return 42.0; }
};

int main() {
    // Test 1: Basic product kernel with three sizes.
    std::vector<double> sizes1 = {1.0, 2.0, 3.0};
    auto mat1 = compute_kernel_matrix(sizes1, product_kernel);
    assert(mat1.size() == 3);
    assert(mat1[0][0] == 1.0);
    assert(mat1[0][1] == 2.0);
    assert(mat1[0][2] == 3.0);
    assert(mat1[1][0] == 2.0); // symmetric
    assert(mat1[1][1] == 4.0);
    assert(mat1[1][2] == 6.0);
    assert(mat1[2][0] == 3.0);
    assert(mat1[2][1] == 6.0);
    assert(mat1[2][2] == 9.0);

    // Test 2: Sum kernel with two sizes.
    std::vector<double> sizes2 = {1.5, 2.5};
    auto mat2 = compute_kernel_matrix(sizes2, sum_kernel);
    assert(mat2.size() == 2);
    assert(mat2[0][0] == 3.0);
    assert(mat2[0][1] == 4.0);
    assert(mat2[1][0] == 4.0);
    assert(mat2[1][1] == 5.0);

    // Test 3: Single element, kernel called once.
    std::vector<double> sizes3 = {0.5};
    auto mat3 = compute_kernel_matrix(sizes3, product_kernel);
    assert(mat3.size() == 1);
    assert(mat3[0][0] == 0.25);

    // Test 4: Empty vector returns empty matrix.
    std::vector<double> sizes4;
    auto mat4 = compute_kernel_matrix(sizes4, sum_kernel);
    assert(mat4.empty());

    // Test 5: Constant functor kernel.
    std::vector<double> sizes5 = {1.0, 2.0, 3.0, 4.0};
    auto mat5 = compute_kernel_matrix(sizes5, ConstantKernel{});
    assert(mat5.size() == 4);
    for (std::size_t i = 0; i < 4; ++i) {
        for (std::size_t j = 0; j < 4; ++j) {
            assert(mat5[i][j] == 42.0);
        }
    }

    // Test 6: Symmetry property with a complex kernel.
    auto complex_kernel = [](double a, double b) { return std::sin(a) * std::cos(b); };
    std::vector<double> sizes6 = {0.1, 1.2, 2.3, 3.4};
    auto mat6 = compute_kernel_matrix(sizes6, complex_kernel);
    for (std::size_t i = 0; i < sizes6.size(); ++i) {
        for (std::size_t j = 0; j < sizes6.size(); ++j) {
            assert(std::fabs(mat6[i][j] - complex_kernel(sizes6[i], sizes6[j])) < 1e-12);
            assert(std::fabs(mat6[i][j] - mat6[j][i]) < 1e-12);
        }
    }

    return 0;
}
