/*
Write a C++ function `computeUpperTriangularProduct` that takes two integers `M` (rows) and `N` (columns) and returns a `std::vector<double>` of length `M`, where each element `A[i]` is computed as the dot product of row `i` of an upper-triangular matrix `B` (size `M x N`) with a vector `C` (size `N`). The matrix `B` and vector `C` are generated deterministically using a seeded random number generator (seed = 0) as in the snippet: for each position, if `i <= j` then the value is `(rand() % 1000000)/1e6`, otherwise it is `0.0`. For `C[j]`, it is always that random value. The computation must only multiply `B[i][j] * C[j]` for `j` from `max(0,i)` to `N-1` (since entries below the diagonal are zero). The function should not print anything; it only computes and returns the result. Use `std::mt19937` with a fixed seed (e.g., 0) for reproducibility, and return a `std::vector<double>` of size `M` with all values initialized to `0.0` before accumulation.
*/
#include <vector>
#include <random>
#include <cstddef>

// Compute A[i] = sum_{j=max(0,i)}^{N-1} B[i][j] * C[j] where B is upper-triangular
// with entries generated deterministically using seed 0.
std::vector<double> computeUpperTriangularProduct(int M, int N) {
    std::vector<double> A(M, 0.0);
    if (M == 0 || N == 0) return A;

    // Use fixed seed for reproducibility
    std::mt19937 gen(0);
    // Generate random values in [0,1) as in the snippet (rand()%1000000 / 1e6)
    auto randomValue = [&gen]() -> double {
        return static_cast<double>(gen() % 1000000) / 1e6;
    };

    // Generate upper-triangular matrix B (M x N) and vector C
    std::vector<std::vector<double>> B(M, std::vector<double>(N, 0.0));
    for (int i = 0; i < M; ++i) {
        for (int j = 0; j < N; ++j) {
            if (i <= j) {
                B[i][j] = randomValue();
            }
        }
    }

    std::vector<double> C(N, 0.0);
    for (int j = 0; j < N; ++j) {
        C[j] = randomValue();
    }

    // Compute the product, only using upper-triangular entries
    for (int i = 0; i < M; ++i) {
        for (int j = std::max(0, i); j < N; ++j) {
            A[i] += B[i][j] * C[j];
        }
    }

    return A;
}
#include <cassert>
#include <cmath>
#include <vector>
#include <iostream>

// Include the solution function (e.g., from above)
std::vector<double> computeUpperTriangularProduct(int M, int N);

int main() {
    // Case 1: Basic 3x3
    auto result1 = computeUpperTriangularProduct(3, 3);
    // Because the result depends on the fixed random sequence, we cannot hardcode exact values.
    // Instead, verify that the last row (i=2) has only one element (j=2) and is equal to B[2][2]*C[2].
    // But to test determinism, compute twice and compare.
    auto result1_again = computeUpperTriangularProduct(3, 3);
    for (size_t i = 0; i < result1.size(); ++i) {
        assert(result1[i] == result1_again[i]);
    }
    // For M=3,N=3, the first row (i=0) sums over j=0..2, which should be positive
    assert(result1[0] > 0.0);

    // Case 2: Rows >= N are zero (since i<=j never holds)
    auto result2 = computeUpperTriangularProduct(5, 2);
    assert(result2.size() == 5);
    // For i=2,3,4, all entries are zero because i>N-1
    for (int i = 2; i < 5; ++i) {
        assert(result2[i] == 0.0);
    }

    // Case 3: M=0 returns empty
    auto result3 = computeUpperTriangularProduct(0, 4);
    assert(result3.empty());

    // Case 4: N=0 returns zeros for all rows
    auto result4 = computeUpperTriangularProduct(4, 0);
    assert(result4.size() == 4);
    for (double val : result4) {
        assert(val == 0.0);
    }

    // Case 5: Single row, single column
    auto result5 = computeUpperTriangularProduct(1, 1);
    // Deterministic: compute twice and compare
    auto result5_again = computeUpperTriangularProduct(1, 1);
    assert(result5[0] == result5_again[0]);
    assert(result5[0] >= 0.0 && result5[0] < 1.0); // because both are <1, product <1

    // Case 6: Verify the sum for a specific case manually using the same generation logic
    // Reproduce B and C exactly for M=2,N=2 using the same random sequence
    std::mt19937 gen(0);
    auto randomValue = [&gen]() -> double {
        return static_cast<double>(gen() % 1000000) / 1e6;
    };
    // Generate B[0][0], B[0][1], B[1][1] (upper triangular) and C[0], C[1]
    double b00 = randomValue();
    double b01 = randomValue();
    double b11 = randomValue();
    double c0 = randomValue();
    double c1 = randomValue();
    // Note: B[1][0] is zero (i>j)
    double expected0 = b00*c0 + b01*c1;
    double expected1 = b11*c1;  // only j=1
    auto result6 = computeUpperTriangularProduct(2, 2);
    assert(std::abs(result6[0] - expected0) < 1e-9);
    assert(std::abs(result6[1] - expected1) < 1e-9);

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
// The core algorithm follows the exact loop structure from the snippet: generate the upper-triangular matrix `B` (only store/access when `i <= j`), generate vector `C`, then for each row `i`, loop `j` from `max(0,i)` to `N-1` and accumulate `A[i] += B[i][j] * C[j]`. Since the matrix is upper-triangular, rows where `i >= N` will have no nonzero entries (because `i <= j` is impossible when `i > N-1`), so those rows remain zero. Edge cases: if `N` is 0, the matrix has no columns and all `A[i]` are zero; if `M` is 0, return an empty vector. Use `std::mt19937` with seed `0` and generate uniform doubles in `[0,1)` by dividing `rand()` by `1e6` as in the snippet—but for reproducibility, use `std::uniform_real_distribution` or emulate the exact `rand() % 1000000 / 1e6` with `std::mt19937` by using `gen() % 1000000` and cast to double, ensuring same sequence as the snippet if needed. Time complexity is `O(M*N)` for generation and `O(M*N)` for the product loop, so overall `O(M*N)`. Space complexity is `O(M*N)` for the matrix (or `O(N)` if we generate on the fly, but we store it for clarity), plus `O(N)` for `C` and `O(M)` for the result.
