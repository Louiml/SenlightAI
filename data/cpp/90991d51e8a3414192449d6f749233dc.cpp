Write a C++ function that takes a square matrix represented as a `std::vector<std::vector<double>>`, a right-hand side vector `std::vector<double>`, and a tolerance `eps`, and returns the minimum-norm least-squares solution to the linear system `A * x = b` using the truncated singular value decomposition (SVD) method. The function should use a simple iterative Jacobi eigenvalue algorithm to compute the SVD of `A` (decomposing it into `U * S * V^T`), truncate singular values below `eps` (set their inverse to zero), and compute `x = V * S_pinv * U^T * b`. If the matrix is not square, throw `std::invalid_argument`. If the SVD iteration fails to converge within a maximum of 1000 iterations, throw `std::runtime_error`. The function must return the solution vector and must not modify its inputs.

#include <cassert>
#include <cmath>
#include <vector>

// Solution function declared above (include its definition here or header)

int main() {
    // Test 1: Identity matrix
    std::vector<std::vector<double>> A1 = {{1,0},{0,1}};
    std::vector<double> b1 = {3,4};
    auto x1 = solveTruncatedSVD(A1, b1, 1e-9);
    assert(std::abs(x1[0]-3) < 1e-9 && std::abs(x1[1]-4) < 1e-9);

    // Test 2: Diagonal matrix
    A1 = {{2,0},{0,5}};
    b1 = {4,10};
    x1 = solveTruncatedSVD(A1, b1, 1e-9);
    assert(std::abs(x1[0]-2) < 1e-9 && std::abs(x1[1]-2) < 1e-9);

    // Test 3: Singular matrix with consistent b (one singular value zero)
    // A = [[1,1],[1,1]] rank 1, b = [2,2], solution should have x[0]+x[1]=2, min norm gives x=[1,1]
    A1 = {{1,1},{1,1}};
    b1 = {2,2};
    x1 = solveTruncatedSVD(A1, b1, 1e-9);
    assert(std::abs(x1[0]-1) < 1e-9 && std::abs(x1[1]-1) < 1e-9);

    // Test 4: Inconsistent system (least squares)
    // A = [[1,0],[0,0]], b = [1,1], least squares solution x = [1,0] (since second equation ignored)
    A1 = {{1,0},{0,0}};
    b1 = {1,1};
    x1 = solveTruncatedSVD(A1, b1, 1e-9);
    assert(std::abs(x1[0]-1) < 1e-9 && std::abs(x1[1]-0) < 1e-9);

    // Test 5: 3x3 matrix
    A1 = {{4,1,2},{1,3,0},{2,0,5}};
    b1 = {7,4,7};
    x1 = solveTruncatedSVD(A1, b1, 1e-9);
    // Exact solution is [1,1,1] (verify: 4+1+2=7, 1+3=4, 2+5=7)
    for (int i=0; i<3; ++i) assert(std::abs(x1[i]-1) < 1e-9);

    // Test 6: Truncation with eps larger than smallest singular value
    // A = [[1,0],[0,1e-8]], eps = 1e-6 -> singular value truncated, no solution but least squares
    A1 = {{1,0},{0,1e-8}};
    b1 = {1,1};
    x1 = solveTruncatedSVD(A1, b1, 1e-6);
    assert(std::abs(x1[0]-1) < 1e-9 && std::abs(x1[1]) < 1e-9);

    // Test 7: Non-square throws
    bool threw = false;
    try { solveTruncatedSVD({{1,2},{3,4},{5,6}}, {1,2}, 1e-9); } catch (std::invalid_argument&) { threw = true; }
    assert(threw);

    // Test 8: Size mismatch throws
    threw = false;
    try { solveTruncatedSVD({{1,2},{3,4}}, {1}, 1e-9); } catch (std::invalid_argument&) { threw = true; }
    assert(threw);

    // Test 9: Zero matrix
    A1 = {{0,0},{0,0}};
    b1 = {0,0};
    x1 = solveTruncatedSVD(A1, b1, 1e-9);
    assert(std::abs(x1[0]) < 1e-9 && std::abs(x1[1]) < 1e-9);

    // Test 10: Large diagonal with different scales
    A1 = {{1e6,0},{0,1e-6}};
    b1 = {1e6,1};
    x1 = solveTruncatedSVD(A1, b1, 1e-12);
    assert(std::abs(x1[0]-1) < 1e-9 && std::abs(x1[1]-1) < 1e-9);

    return 0;
}

#include <vector>
#include <cmath>
#include <stdexcept>
#include <algorithm>

/**
 * Compute the minimum-norm least-squares solution to A*x = b using truncated SVD.
 * A must be square. Singular values below eps are treated as zero.
 * Throws std::invalid_argument if A is not square or sizes mismatch.
 * Throws std::runtime_error if SVD does not converge within 1000 iterations.
 */
std::vector<double> solveTruncatedSVD(const std::vector<std::vector<double>>& A,
                                      const std::vector<double>& b,
                                      double eps) {
    const size_t n = A.size();
    if (n == 0) throw std::invalid_argument("Matrix cannot be empty");
    for (const auto& row : A) {
        if (row.size() != n) throw std::invalid_argument("Matrix must be square");
    }
    if (b.size() != n) throw std::invalid_argument("Right-hand side size mismatch");

    // Working copy of A
    std::vector<std::vector<double>> U = A;
    // V is identity matrix of size n
    std::vector<std::vector<double>> V(n, std::vector<double>(n, 0.0));
    for (size_t i = 0; i < n; ++i) V[i][i] = 1.0;

    const double tolerance = 1e-12;  // convergence threshold for off-diagonal orthogonality
    const int maxIter = 1000;

    // One-sided Jacobi SVD
    bool converged = false;
    for (int iter = 0; iter < maxIter; ++iter) {
        double off = 0.0;  // measure of non-orthogonality
        // Sweep over all column pairs (i,j) with i < j
        for (size_t i = 0; i < n; ++i) {
            for (size_t j = i + 1; j < n; ++j) {
                // Compute the 2x2 Gram matrix of columns i and j
                double alpha = 0.0, beta = 0.0, gamma = 0.0;
                for (size_t k = 0; k < n; ++k) {
                    double ui = U[k][i], uj = U[k][j];
                    alpha += ui * ui;
                    beta += uj * uj;
                    gamma += ui * uj;
                }
                off += gamma * gamma;
                // If columns are already orthogonal, skip
                if (std::abs(gamma) <= tolerance * std::sqrt(alpha * beta)) continue;

                // Compute rotation angle to zero out gamma
                double zeta = (beta - alpha) / (2.0 * gamma);
                double t = (zeta >= 0 ? 1.0 : -1.0) / (std::abs(zeta) + std::sqrt(1.0 + zeta * zeta));
                double c = 1.0 / std::sqrt(1.0 + t * t);
                double s = c * t;

                // Apply rotation to columns i and j of U
                for (size_t k = 0; k < n; ++k) {
                    double uki = U[k][i], ukj = U[k][j];
                    U[k][i] = c * uki - s * ukj;
                    U[k][j] = s * uki + c * ukj;
                }
                // Accumulate rotation into V (right after: V = V * J, where J posts)
                for (size_t k = 0; k < n; ++k) {
                    double vki = V[k][i], vkj = V[k][j];
                    V[k][i] = c * vki - s * vkj;
                    V[k][j] = s * vki + c * vkj;
                }
            }
        }
        // Check convergence: all off-diagonal Gram entries are near zero
        if (off <= tolerance * tolerance * n * n) {
            converged = true;
            break;
        }
    }
    if (!converged) throw std::runtime_error("SVD did not converge");

    // Extract singular values as norms of columns of U, and normalize U columns
    std::vector<double> S(n);
    for (size_t j = 0; j < n; ++j) {
        double norm = 0.0;
        for (size_t i = 0; i < n; ++i) norm += U[i][j] * U[i][j];
        norm = std::sqrt(norm);
        S[j] = norm;
        if (norm > 0) {
            for (size_t i = 0; i < n; ++i) U[i][j] /= norm;
        }
    }

    // Compute U^T * b
    std::vector<double> Utb(n, 0.0);
    for (size_t i = 0; i < n; ++i) {
        double sum = 0.0;
        for (size_t j = 0; j < n; ++j) {
            sum += U[j][i] * b[j];  // U[j][i] is element (i,j) of U^T
        }
        Utb[i] = sum;
    }

    // Multiply by S_pinv (truncated) and then by V
    std::vector<double> result(n, 0.0);
    for (size_t i = 0; i < n; ++i) {
        double invS = (std::abs(S[i]) < eps) ? 0.0 : 1.0 / S[i];
        double coeff = invS * Utb[i];
        if (coeff != 0.0) {
            for (size_t j = 0; j < n; ++j) {
                result[j] += V[j][i] * coeff;  // V[j][i] is element (i,j) of V
            }
        }
    }
    return result;
}

// The core approach is to compute the SVD of the square matrix `A` and then use it to solve the linear system in the least-squares sense. The SVD is computed using the one-sided Jacobi algorithm: repeatedly apply Jacobi rotations to the columns of `A` to make them orthogonal, accumulating the rotations into `V`. After convergence, the columns of `U` are the normalized rotated columns of `A`, the diagonal entries of `S` are the norms of those columns, and `V` is the accumulated rotation matrix. This yields `A = U * S * V^T⋅`.
//
// Once the SVD is obtained, compute the pseudo-inverse solution: For each singular value `s_i`, if `|s_i| < eps`, set its reciprocal to zero (truncated SVD), otherwise use `1/s_i`. Then compute `x = V * S_pinv * (U^T * b)`. This gives the minimum-norm least-squares solution. Edge cases: if a singular value is exactly zero but above tolerance (tolerance should be small positive), it could cause division by zero; use `fabs(s) <= eps` to truncate. The Jacobi algorithm may not converge if the matrix is pathological; the maximum iteration guard throws an error. Time complexity is \(O(n^3 \cdot \text{iterations})\), typically \(O(n^3)\) for practical matrices, and space complexity is \(O(n^2)\) for storing `U`, `V`, and temporary matrices.
