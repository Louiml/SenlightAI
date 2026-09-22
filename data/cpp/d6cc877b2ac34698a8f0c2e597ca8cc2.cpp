Write a standalone C++ function that computes the operator (spectral) norm of a real square matrix given as a dynamically sized 2D vector of doubles. The operator norm is defined as the largest singular value of the matrix, which equals the square root of the largest eigenvalue of the matrix multiplied by its transpose. If the matrix is empty (0x0), return 0.0. Ensure the function handles any square size, including 1x1 and larger, with reasonable numerical precision. Do not rely on external linear algebra libraries; implement the singular value computation using a simple iterative method such as power iteration on the matrix A^T A to find the largest eigenvalue.
#include <cassert>
#include <cmath>
#include <vector>

// Function declaration from solution
double operatorNorm(const std::vector<std::vector<double>>& A);

int main() {
    // Test 1: empty matrix
    assert(operatorNorm({}) == 0.0);

    // Test 2: 1x1 matrix
    assert(std::fabs(operatorNorm({{5.0}}) - 5.0) < 1e-9);
    assert(std::fabs(operatorNorm({{-3.0}}) - 3.0) < 1e-9);

    // Test 3: identity matrix (norm = 1)
    std::vector<std::vector<double>> I = {{1,0,0},{0,1,0},{0,0,1}};
    assert(std::fabs(operatorNorm(I) - 1.0) < 1e-6);

    // Test 4: diagonal matrix with known norm (max abs diagonal)
    std::vector<std::vector<double>> D = {{4,0,0},{0,2,0},{0,0,6}};
    assert(std::fabs(operatorNorm(D) - 6.0) < 1e-6);

    // Test 5: matrix of ones (3x3) norm = 3 (since max singular value = sum of ones = 3)
    std::vector<std::vector<double>> ones3(3, std::vector<double>(3, 1.0));
    assert(std::fabs(operatorNorm(ones3) - 3.0) < 1e-6);

    // Test 6: rotation matrix (orthogonal) norm = 1
    std::vector<std::vector<double>> R = {{0,-1},{1,0}}; // 90-degree rotation
    assert(std::fabs(operatorNorm(R) - 1.0) < 1e-6);

    // Test 7: rank-1 matrix with large value
    std::vector<std::vector<double>> M = {{1,2,3},{4,5,6},{7,8,9}};
    // Exact largest singular value approx 16.8481 (from known) - we just ensure it's positive and reasonable
    double normM = operatorNorm(M);
    assert(std::fabs(normM - 16.8481) < 0.001);

    // Test 8: 2x2 with known norm: A = [[1,2],[3,4]]; largest singular value = sqrt( ( (1+4) + sqrt( (1-4)^2 + (2+3)^2 ) ) /2 )? Actually compute exactly.
    // B = A^T A = [[10,14],[14,20]]; eigenvalues: (30 ± sqrt( (30)^2 - 4*(200-196) ) )/2 = (30 ± sqrt(900-16))/2 = (30 ± sqrt(884))/2 = (30 ± 29.732)/2 -> largest ≈ 29.866, sqrt ≈ 5.46499
    std::vector<std::vector<double>> A2 = {{1,2},{3,4}};
    assert(std::fabs(operatorNorm(A2) - 5.46499) < 1e-3);

    return 0;
}
#include <vector>
#include <cmath>
#include <stdexcept>

// Compute the operator (spectral) norm of a square matrix.
// Uses power iteration to find the largest eigenvalue of A^T A.
// Empty matrix returns 0.0.
double operatorNorm(const std::vector<std::vector<double>>& A) {
    const size_t n = A.size();
    if (n == 0) return 0.0;
    // Ensure square
    for (const auto& row : A) {
        if (row.size() != n) throw std::invalid_argument("Matrix must be square");
    }
    if (n == 1) return std::fabs(A[0][0]);

    // Compute B = A^T A (symmetric)
    std::vector<std::vector<double>> B(n, std::vector<double>(n, 0.0));
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            double sum = 0.0;
            for (size_t k = 0; k < n; ++k) {
                sum += A[k][i] * A[k][j]; // A^T[i][k] * A[k][j]
            }
            B[i][j] = sum;
        }
    }

    // Power iteration
    std::vector<double> v(n, 1.0); // initial non-zero vector
    // Normalize
    double norm_v = 0.0;
    for (double x : v) norm_v += x * x;
    norm_v = std::sqrt(norm_v);
    if (norm_v == 0.0) norm_v = 1.0; // avoid division by zero
    for (double& x : v) x /= norm_v;

    double lambda = 0.0;
    const int max_iter = 1000;
    const double tol = 1e-12;

    for (int iter = 0; iter < max_iter; ++iter) {
        // v_new = B * v
        std::vector<double> v_new(n, 0.0);
        for (size_t i = 0; i < n; ++i) {
            double sum = 0.0;
            for (size_t j = 0; j < n; ++j) {
                sum += B[i][j] * v[j];
            }
            v_new[i] = sum;
        }

        // Rayleigh quotient: lambda = v^T B v / (v^T v) = v^T v_new / (v^T v) since v normalized
        double v_dot_v_new = 0.0;
        for (size_t i = 0; i < n; ++i) {
            v_dot_v_new += v[i] * v_new[i];
        }

        // Check convergence
        if (iter > 0 && std::fabs(v_dot_v_new - lambda) < tol * std::fabs(lambda + 1e-20)) {
            lambda = v_dot_v_new;
            break;
        }
        lambda = v_dot_v_new;

        // Normalize v_new
        double nrm = 0.0;
        for (double x : v_new) nrm += x * x;
        nrm = std::sqrt(nrm);
        if (nrm < 1e-15) break; // zero eigenvalue? avoid division by zero
        for (size_t i = 0; i < n; ++i) v[i] = v_new[i] / nrm;
    }

    // Guard against numerical issues
    if (lambda < 0.0) lambda = 0.0;
    return std::sqrt(lambda);
}
// The solution computes the operator norm by finding the largest singular value. For a matrix A, the largest singular value σ_max = sqrt(λ_max), where λ_max is the largest eigenvalue of the symmetric positive semidefinite matrix B = A^T A. We can use power iteration on B to approximate λ_max: start with a random non-zero vector (e.g., all ones), then repeatedly compute v_new = B * v, normalize v_new, and track the Rayleigh quotient. Convergence is linear with rate depending on the ratio of the two largest eigenvalues, but for typical matrices a fixed number of iterations (e.g., 100) and a tolerance (e.g., 1e-10) suffice. Edge cases: empty matrix returns 0; 1x1 matrix has norm = |a|; handle non-finite values gracefully (though not needed in basic tests). Complexity: each iteration computes B*v, which is O(n^2) (since B is n x n, and multiplying by vector is O(n^2)), with roughly O(iterations) * O(n^2) time. Space is O(n^2) for B (or avoid storing B explicitly by computing A^T (A v) to reduce space to O(n) for vectors). Use the latter for efficiency. Time complexity: O(iterations * n^2). For simplicity, we store B.
