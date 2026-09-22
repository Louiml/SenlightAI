/*
Write a C++ function that takes a symmetric positive-definite 3x3 matrix (represented as a `double` array of length 9 in row-major order) and returns a `std::array<double, 9>` containing the lower-triangular Cholesky factor `L` (also row-major), such that `L * L.transpose()` reconstructs the original matrix. Your function must verify positive-definiteness (no zero or negative pivots during decomposition) and return a zero-filled array if the matrix is not positive-definite. Do not use external linear algebra libraries; implement the Cholesky–Banachiewicz algorithm manually.
*/

#include <array>
#include <cmath>

// Compute the lower-triangular Cholesky factor L (row-major) of a 3x3 symmetric positive-definite matrix.
// If the matrix is not positive-definite, return a zero-filled array.
std::array<double, 9> cholesky3x3(const std::array<double, 9>& A) {
    constexpr int n = 3;
    constexpr double eps = 1e-12;
    std::array<double, 9> L = {};  // all zeros initially

    for (int j = 0; j < n; ++j) {
        // Compute diagonal element L[j][j]
        double sum = 0.0;
        for (int k = 0; k < j; ++k) {
            sum += L[j*n + k] * L[j*n + k];
        }
        double diag_val = A[j*n + j] - sum;
        if (diag_val <= eps) {
            return {};  // Not positive-definite
        }
        L[j*n + j] = std::sqrt(diag_val);

        // Compute off-diagonal elements in column j
        for (int i = j + 1; i < n; ++i) {
            sum = 0.0;
            for (int k = 0; k < j; ++k) {
                sum += L[i*n + k] * L[j*n + k];
            }
            L[i*n + j] = (A[i*n + j] - sum) / L[j*n + j];
        }
    }
    return L;
}

#include <cassert>
#include <cmath>
#include <array>

int main() {
    // Test 1: Known 3x3 positive-definite matrix from the prompt
    std::array<double,9> A = {4, -1, 2, -1, 6, 0, 2, 0, 5};
    auto L = cholesky3x3(A);
    // Expected L (computed manually): [[2,0,0], [-0.5, 2.3979, 0], [1, 0.2085, 1.989]]
    assert(std::abs(L[0] - 2.0) < 1e-9);
    assert(std::abs(L[1]) < 1e-9);
    assert(std::abs(L[2]) < 1e-9);
    assert(std::abs(L[3] + 0.5) < 1e-9);
    assert(std::abs(L[4] - std::sqrt(5.75)) < 1e-9);
    assert(std::abs(L[5]) < 1e-9);
    assert(std::abs(L[6] - 1.0) < 1e-9);
    assert(std::abs(L[7] - (0.5/2.3979)) < 1e-9);
    // Verify reconstruction: L*L^T = A
    std::array<double,9> recon = {};
    for (int i=0;i<3;i++) for (int j=0;j<3;j++) {
        double s=0;
        for (int k=0;k<3;k++) s += L[i*3+k]*L[j*3+k];
        recon[i*3+j]=s;
    }
    for (int i=0;i<9;i++) assert(std::abs(recon[i]-A[i]) < 1e-9);

    // Test 2: Identity matrix
    std::array<double,9> I = {1,0,0,0,1,0,0,0,1};
    auto LI = cholesky3x3(I);
    assert(std::abs(LI[0]-1)<1e-9 && std::abs(LI[4]-1)<1e-9 && std::abs(LI[8]-1)<1e-9);
    for (int i=0;i<9;i++) if (i!=0 && i!=4 && i!=8) assert(std::abs(LI[i])<1e-9);

    // Test 3: Non-positive-definite matrix (zero pivot)
    std::array<double,9> bad = {1,0,0,0,0,0,0,0,0};
    auto Lbad = cholesky3x3(bad);
    for (double v : Lbad) assert(v == 0.0);

    // Test 4: Negative diagonal
    std::array<double,9> neg = {-1,0,0,0,1,0,0,0,1};
    auto Lneg = cholesky3x3(neg);
    for (double v : Lneg) assert(v == 0.0);

    return 0;
}

// The Cholesky decomposition for a symmetric positive-definite matrix `A` computes a lower-triangular matrix `L` where `A = L * L^T`. The algorithm processes each column `j` from 0 to n-1: first compute the diagonal element `L[j][j] = sqrt(A[j][j] - sum_{k=0}^{j-1} L[j][k]^2)`. If the argument under the square root is not positive (or zero, which would make the matrix singular but not positive-definite), the decomposition fails — return a zero array. Then for each row `i` from j+1 to n-1, compute `L[i][j] = (A[i][j] - sum_{k=0}^{j-1} L[i][k]*L[j][k]) / L[j][j]`. Because the matrix is symmetric, we only need the lower triangle of the input. Edge cases: numerical tolerance — if the diagonal argument is less than a small epsilon (e.g., 1e-12), treat as non-positive-definite. Time complexity is O(n^3) due to the triple loop (here n=3, constant), space O(n^2) for the output matrix. The manual implementation avoids memory allocations beyond the fixed-size output.
