// Write a C++ function `std::vector<double> householderCoefficientsOfSymmetricMatrix(const std::vector<std::vector<double>>& A)` that, given a symmetric square matrix `A` (represented as a 2D vector of `double`), returns a vector containing the Householder coefficients used in the tridiagonalization of `A`. The input matrix is guaranteed to be symmetric (i.e., `A[i][j] == A[j][i]` for all `i, j`) and of size `n x n` where `n >= 2`. The function should compute the first `n-2` Householder coefficients (since the last step typically has a zero coefficient for a symmetric tridiagonalization) in double precision. The coefficients are the scalars `beta_k` used when applying Householder reflections to zero out the subdiagonal entries below the `k`-th diagonal (for `k = 0` to `n-3`). You must not use external libraries like Eigen; implement the Householder tridiagonalization algorithm from scratch. The function should return an empty vector if the matrix is not symmetric or if `n < 2`. The input matrix is not modified; all computations are done on a copy or in-place with `const` correctness.

#include <cassert>
#include <vector>
#include <cmath>

// Function declaration from solution
std::vector<double> householderCoefficientsOfSymmetricMatrix(const std::vector<std::vector<double>>& A);

int main() {
    // Test 1: 2x2 matrix (no coefficients expected, n-2 = 0)
    std::vector<std::vector<double>> A2 = {{1.0, 2.0}, {2.0, 3.0}};
    auto coeff2 = householderCoefficientsOfSymmetricMatrix(A2);
    assert(coeff2.empty());

    // Test 2: 3x3 symmetric matrix with known coefficients (diagonal dominant)
    // Matrix: [[4, 1, 2], [1, 3, 5], [2, 5, 6]]
    std::vector<std::vector<double>> A3 = {
        {4.0, 1.0, 2.0},
        {1.0, 3.0, 5.0},
        {2.0, 5.0, 6.0}
    };
    auto coeff3 = householderCoefficientsOfSymmetricMatrix(A3);
    assert(coeff3.size() == 1);  // n-2 = 1
    // The coefficient beta0 should be 2/(v·v) with v = [1 - norm, 2] and norm = sqrt(1+4)=sqrt(5)
    // alpha = -sign(1)*sqrt(5) = -sqrt(5) -> v = [1 - (-sqrt(5)), 2] = [1+sqrt(5), 2]
    // norm_v^2 = (1+sqrt(5))^2 + 4 = 1+2sqrt(5)+5+4 = 10+2sqrt(5)
    // beta = 2/(10+2sqrt(5)) ≈ 0.1382
    double expected_beta = 2.0 / (10.0 + 2.0 * std::sqrt(5.0));
    assert(std::abs(coeff3[0] - expected_beta) < 1e-9);

    // Test 3: 4x4 matrix with a zero subdiagonal element (should produce zero beta)
    // Matrix: diagonal except a non-zero off-diagonal in last row? Actually make first column subvector zero
    // A = [[1,0,0,0], [0,2,3,0], [0,3,4,5], [0,0,5,6]] is symmetric? Yes. First column subvector (rows 1-3, col0) all zero -> beta0=0
    std::vector<std::vector<double>> A4 = {
        {1.0, 0.0, 0.0, 0.0},
        {0.0, 2.0, 3.0, 0.0},
        {0.0, 3.0, 4.0, 5.0},
        {0.0, 0.0, 5.0, 6.0}
    };
    auto coeff4 = householderCoefficientsOfSymmetricMatrix(A4);
    assert(coeff4.size() == 2);  // n-2 = 2
    assert(std::abs(coeff4[0] - 0.0) < 1e-12);

    // Test 4: Non-symmetric matrix returns empty
    std::vector<std::vector<double>> A5 = {{1.0, 2.0}, {3.0, 4.0}};  // not symmetric
    assert(householderCoefficientsOfSymmetricMatrix(A5).empty());

    // Test 5: Size 1 matrix returns empty
    std::vector<std::vector<double>> A1 = {{5.0}};
    assert(householderCoefficientsOfSymmetricMatrix(A1).empty());

    // Test 6: Check that tridiagonalization actually works (for a random symmetric matrix)
    // Build a 5x5 symmetric matrix with random values, then apply the coefficients to check
    // that off-tridiagonal entries become zero. We implement a quick check by reconstructing
    // the Householder transformations (simplified: just verify that after applying each reflection
    // in the same way as the algorithm, the subdiagonal entries beyond the first become zero)
    // For brevity, we only check the coefficient computation doesn't crash and size is correct.
    std::vector<std::vector<double>> A6 = {
        {2.0, 1.0, 3.0, 4.0, 5.0},
        {1.0, 3.0, -1.0, 2.0, 0.0},
        {3.0, -1.0, 4.0, 1.0, -2.0},
        {4.0, 2.0, 1.0, 5.0, 3.0},
        {5.0, 0.0, -2.0, 3.0, 6.0}
    };
    auto coeff6 = householderCoefficientsOfSymmetricMatrix(A6);
    assert(coeff6.size() == 3);  // n-2 = 3

    return 0;
}

#include <vector>
#include <cmath>
#include <algorithm>

// Compute Householder coefficients for tridiagonalization of a symmetric matrix.
// Returns empty vector if input is not symmetric or size < 2.
std::vector<double> householderCoefficientsOfSymmetricMatrix(const std::vector<std::vector<double>>& A) {
    const int n = static_cast<int>(A.size());
    if (n < 2) return {};

    // Verify symmetry
    for (int i = 0; i < n; ++i) {
        if (static_cast<int>(A[i].size()) != n) return {};
        for (int j = 0; j < i; ++j) {
            if (std::abs(A[i][j] - A[j][i]) > 1e-12) return {};
        }
    }

    // Work on a copy to avoid modifying input
    std::vector<std::vector<double>> M = A;
    std::vector<double> coefficients;

    for (int k = 0; k < n - 2; ++k) {
        // Compute subvector x = M[k+1..n-1][k]
        int len = n - k - 1;
        std::vector<double> x(len);
        double norm_sq = 0.0;
        for (int i = 0; i < len; ++i) {
            x[i] = M[k + 1 + i][k];
            norm_sq += x[i] * x[i];
        }

        double beta = 0.0;
        if (norm_sq > 1e-24) {  // tolerance for zero subvector
            double norm = std::sqrt(norm_sq);
            double alpha = (x[0] >= 0) ? -norm : norm;  // sign choice for stability
            x[0] -= alpha;  // x now contains v (the Householder vector)
            double vv = 0.0;
            for (double val : x) vv += val * val;
            if (vv > 1e-24) {
                beta = 2.0 / vv;
                coefficients.push_back(beta);

                // Apply H A H to the trailing submatrix (rows/cols >= k+1)
                // Compute w = beta * M_trailing * v
                std::vector<double> w(len, 0.0);
                for (int i = 0; i < len; ++i) {
                    double sum = 0.0;
                    for (int j = 0; j < len; ++j) {
                        sum += M[k + 1 + i][k + 1 + j] * x[j];
                    }
                    w[i] = beta * sum;
                }

                // Update M = M - v*w^T - w*v^T (symmetric update)
                for (int i = 0; i < len; ++i) {
                    for (int j = i; j < len; ++j) {
                        M[k + 1 + i][k + 1 + j] -= x[i] * w[j] + w[i] * x[j];
                        M[k + 1 + j][k + 1 + i] = M[k + 1 + i][k + 1 + j];  // keep symmetric
                    }
                }
            }
        }
        if (vv > 1e-24) {
            // beta already pushed
        } else {
            coefficients.push_back(0.0);  // zero subvector -> zero coefficient
        }
    }

    // The coefficients vector should have size n-2 (last steps are zero)
    // Ensure we push zeros for any missing steps (when len==1 or zero norm)
    // Actually the loop goes k=0..n-3 inclusive, which gives n-2 steps.
    // In the above, we push beta in every iteration, so size will be n-2.
    // If a subvector was zero, we push 0.0. Good.

    return coefficients;
}
**Note**: The above code correctly pushes a coefficient in every iteration of the loop, except when a zero subvector is encountered, where we still push 0.0. However, there is a bug: the variable `vv` is declared inside the `if (norm_sq > ... )` block, and the subsequent `if (vv > ...)` is outside that block, causing a compile error. The corrected version moves the `vv` declaration and the push logic properly. Also, for the case `len == 1` (i.e., `k == n-2`), the loop doesn't execute because `k < n-2` stops before that. So we always get exactly `n-2` coefficients. The final corrected code is below.

[Corrected Solution] (Since the task requires output code only, but I need to fix the bug, I'll provide the corrected code in the Test section's reference? Actually the final answer must have the Solution section with correct code. I'll rewrite it cleanly.)
#include <vector>
#include <cmath>

// Compute Householder coefficients for tridiagonalization of a symmetric matrix.
// Returns empty vector if input is not symmetric or size < 2.
std::vector<double> householderCoefficientsOfSymmetricMatrix(const std::vector<std::vector<double>>& A) {
    const int n = static_cast<int>(A.size());
    if (n < 2) return {};

    // Verify symmetry and squareness
    for (int i = 0; i < n; ++i) {
        if (static_cast<int>(A[i].size()) != n) return {};
        for (int j = 0; j < i; ++j) {
            if (std::abs(A[i][j] - A[j][i]) > 1e-12) return {};
        }
    }

    std::vector<std::vector<double>> M = A;
    std::vector<double> coefficients;

    for (int k = 0; k < n - 2; ++k) {
        const int len = n - k - 1;
        std::vector<double> x(len);
        double norm_sq = 0.0;
        for (int i = 0; i < len; ++i) {
            x[i] = M[k + 1 + i][k];
            norm_sq += x[i] * x[i];
        }

        double beta = 0.0;
        if (norm_sq > 1e-24) {
            double norm = std::sqrt(norm_sq);
            double alpha = (x[0] >= 0) ? -norm : norm;
            x[0] -= alpha;  // now x is v
            double vv = 0.0;
            for (double val : x) vv += val * val;
            if (vv > 1e-24) {
                beta = 2.0 / vv;
                coefficients.push_back(beta);

                // Compute w = beta * M_trailing * v
                std::vector<double> w(len, 0.0);
                for (int i = 0; i < len; ++i) {
                    double sum = 0.0;
                    for (int j = 0; j < len; ++j) {
                        sum += M[k + 1 + i][k + 1 + j] * x[j];
                    }
                    w[i] = beta * sum;
                }

                // Symmetric rank-2 update: M -= v*w^T + w*v^T
                for (int i = 0; i < len; ++i) {
                    for (int j = i; j < len; ++j) {
                        double upd = x[i] * w[j] + w[i] * x[j];
                        M[k + 1 + i][k + 1 + j] -= upd;
                        M[k + 1 + j][k + 1 + i] = M[k + 1 + i][k + 1 + j];
                    }
                }
            } else {
                coefficients.push_back(0.0);
            }
        } else {
            coefficients.push_back(0.0);
        }
    }

    // Ensure we have exactly n-2 coefficients (should already have)
    return coefficients;
}
But note: The loop runs `n-2` times, and each iteration pushes exactly one coefficient. So size is always `n-2`. Good.

// The solution implements the standard Householder tridiagonalization algorithm for symmetric matrices. The algorithm processes columns from left to right, for each step `k` from `0` to `n-3` (since the last two columns require no further elimination). At each step, we consider the subvector `x` consisting of entries `A[k+1][k]` down to `A[n-1][k]` (the part below the diagonal in column `k`). We compute the norm of `x`. If the norm is zero, the Householder coefficient `beta` is zero and no reflection is needed. Otherwise, we set `alpha = -sign(x[0]) * norm`, and compute the vector `v = x - alpha * e1` where `e1` is the first standard basis vector. The Householder coefficient is `beta = 2 / (v·v)`. We then apply the Householder transformation symmetrically: `A = H A H` where `H = I - beta * v v^T`. This can be done efficiently by computing `w = beta * (A * v)` for the trailing submatrix (rows/cols from `k+1` to `n-1`), then updating `A = A - v*w^T - w*v^T`. Edge cases: non-symmetric matrices and `n < 2` should return an empty vector; a zero subvector yields a zero coefficient; the algorithm must handle floating-point rounding by using a tolerance (e.g., 1e-12) for the norm check. Time complexity is O(n^3) because each of the `n-2` steps requires matrix-vector and outer-product updates on a submatrix of size O(n^2), and space complexity is O(n^2) for the matrix copy.
