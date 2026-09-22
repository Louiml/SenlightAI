// Write a C++ function `bool invertMatrix(const double* A, double* inv, uint16_t n)` that computes the inverse of a general square matrix using LU decomposition with partial pivoting. The function should return `false` if the matrix is singular (determinant effectively zero or any resulting entry is `NaN` or infinite), and `true` and populate the output array `inv` with the inverse otherwise. The matrix is stored in row-major order (i.e., element `(i,j)` is at `A[i*n + j]`). You may use helper functions for matrix multiplication, identity, pivoting, LU decomposition, and forward/backward substitution, but all code must be self-contained within your solution. The function should support any dimension `n >= 1`, including 1x1 matrices. Use `double` precision throughout.
The approach is based on LU decomposition with partial row pivoting. The algorithm computes a permutation matrix `P` such that `P*A = L*U`, where `L` is lower triangular with unit diagonal and `U` is upper triangular. Then the inverse is computed as `A^{-1} = U^{-1} * L^{-1} * P`. Steps: (1) Create pivot matrix by selecting the largest absolute value in each column and swapping rows; (2) Form `APrime = P*A`; (3) Perform Doolittle LU decomposition on `APrime` to produce `L` and `U`; (4) Invert `L` using forward substitution and `U` using backward substitution; (5) Multiply `U^{-1} * L^{-1}` to get an unpivoted inverse, then multiply by `P` to get the final inverse. Edge cases: zero or near-zero diagonal elements cause division by zero or overflow; check for `NaN`/`inf` in the result. For a 1x1 matrix, the inverse is simply the reciprocal of the single element. Time complexity is O(n^3) for LU decomposition and O(n^3) for the two matrix multiplications, and space complexity is O(n^2). The algorithm is numerically stable for general matrices due to partial pivoting, but still fails for singular matrices.
#include <cstdint>
#include <cstring>
#include <cmath>

// Helper: multiply two square matrices (row-major) into C.
static void mat_mul(const double* A, const double* B, double* C, uint16_t n) {
    memset(C, 0, sizeof(double)*n*n);
    for (uint16_t i = 0; i < n; ++i) {
        for (uint16_t j = 0; j < n; ++j) {
            double sum = 0.0;
            for (uint16_t k = 0; k < n; ++k) {
                sum += A[i*n + k] * B[k*n + j];
            }
            C[i*n + j] = sum;
        }
    }
}

// Helper: create identity matrix.
static void mat_identity(double* A, uint16_t n) {
    memset(A, 0, sizeof(double)*n*n);
    for (uint16_t i = 0; i < n; ++i) {
        A[i*n + i] = 1.0;
    }
}

// Helper: swap two doubles.
static void swap(double& a, double& b) {
    double temp = a;
    a = b;
    b = temp;
}

// Helper: compute pivot matrix P such that P*A has largest magnitude elements on diagonal.
static void mat_pivot(const double* A, double* pivot, uint16_t n) {
    mat_identity(pivot, n);
    for (uint16_t i = 0; i < n; ++i) {
        uint16_t maxRow = i;
        double maxVal = fabs(A[i*n + i]);
        for (uint16_t j = i+1; j < n; ++j) {
            double val = fabs(A[j*n + i]);
            if (val > maxVal) {
                maxVal = val;
                maxRow = j;
            }
        }
        if (maxRow != i) {
            for (uint16_t k = 0; k < n; ++k) {
                swap(pivot[i*n + k], pivot[maxRow*n + k]);
            }
        }
    }
}

// Helper: invert lower triangular L (unit diagonal implied) using forward substitution.
// Result stored in out (initially zero).
static void mat_forward_sub(const double* L, double* out, uint16_t n) {
    // Solve L * out = I (column by column).
    for (uint16_t col = 0; col < n; ++col) {
        out[col*n + col] = 1.0 / L[col*n + col];
        for (uint16_t row = col+1; row < n; ++row) {
            double sum = 0.0;
            for (uint16_t k = col; k < row; ++k) {
                sum += L[row*n + k] * out[k*n + col];
            }
            out[row*n + col] = -sum / L[row*n + row];
        }
    }
}

// Helper: invert upper triangular U using backward substitution.
static void mat_back_sub(const double* U, double* out, uint16_t n) {
    for (int col = n-1; col >= 0; --col) {
        out[col*n + col] = 1.0 / U[col*n + col];
        for (int row = col-1; row >= 0; --row) {
            double sum = 0.0;
            for (int k = col; k > row; --k) {
                sum += U[row*n + k] * out[k*n + col];
            }
            out[row*n + col] = -sum / U[row*n + row];
        }
    }
}

// Helper: LU decomposition with partial pivoting.
// A is original, L and U are outputs (L unit diagonal), P is pivot matrix.
static void mat_LU_decompose(const double* A, double* L, double* U, double* P, uint16_t n) {
    memset(L, 0, sizeof(double)*n*n);
    memset(U, 0, sizeof(double)*n*n);
    mat_pivot(A, P, n);
    // Compute APrime = P * A.
    double* APrime = new double[n*n];
    mat_mul(P, A, APrime, n);
    for (uint16_t i = 0; i < n; ++i) {
        L[i*n + i] = 1.0;
    }
    for (uint16_t i = 0; i < n; ++i) {
        for (uint16_t j = 0; j < n; ++j) {
            if (j <= i) {
                // U[j][i]
                double sum = 0.0;
                for (uint16_t k = 0; k < j; ++k) {
                    sum += L[j*n + k] * U[k*n + i];
                }
                U[j*n + i] = APrime[j*n + i] - sum;
            }
            if (j >= i) {
                // L[j][i]
                double sum = 0.0;
                for (uint16_t k = 0; k < i; ++k) {
                    sum += L[j*n + k] * U[k*n + i];
                }
                if (fabs(U[i*n + i]) < 1e-15) {
                    // Avoid division by zero; set to a very large value to cause failure later.
                    L[j*n + i] = 0.0;
                } else {
                    L[j*n + i] = (APrime[j*n + i] - sum) / U[i*n + i];
                }
            }
        }
    }
    delete[] APrime;
}

// Main function: compute inverse of general square matrix.
bool invertMatrix(const double* A, double* inv, uint16_t n) {
    if (n == 0) return false;
    if (n == 1) {
        if (fabs(A[0]) < 1e-15) return false;
        inv[0] = 1.0 / A[0];
        return true;
    }
    double* L = new double[n*n];
    double* U = new double[n*n];
    double* P = new double[n*n];
    mat_LU_decompose(A, L, U, P, n);

    double* L_inv = new double[n*n]();
    double* U_inv = new double[n*n]();
    mat_forward_sub(L, L_inv, n);
    mat_back_sub(U, U_inv, n);

    // Compute U_inv * L_inv
    double* inv_unpivoted = new double[n*n];
    mat_mul(U_inv, L_inv, inv_unpivoted, n);
    // Multiply by P to get final inverse: inv = inv_unpivoted * P
    double* inv_pivoted = new double[n*n];
    mat_mul(inv_unpivoted, P, inv_pivoted, n);

    // Check for NaN or infinities.
    bool success = true;
    for (uint16_t i = 0; i < n*n; ++i) {
        if (std::isnan(inv_pivoted[i]) || std::isinf(inv_pivoted[i])) {
            success = false;
            break;
        }
    }
    if (success) {
        memcpy(inv, inv_pivoted, sizeof(double)*n*n);
    }

    delete[] L;
    delete[] U;
    delete[] P;
    delete[] L_inv;
    delete[] U_inv;
    delete[] inv_unpivoted;
    delete[] inv_pivoted;
    return success;
}
#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstring>

// Declare the function (assume it's provided in the solution).
bool invertMatrix(const double* A, double* inv, uint16_t n);

// Helper to compare matrices with tolerance.
static bool matricesEqual(const double* a, const double* b, uint16_t n, double tol = 1e-9) {
    for (uint16_t i = 0; i < n*n; ++i) {
        if (fabs(a[i] - b[i]) > tol) return false;
    }
    return true;
}

int main() {
    // Test 1: 2x2 non-singular
    double A2[4] = {4.0, 7.0, 2.0, 6.0};
    double inv2[4];
    assert(invertMatrix(A2, inv2, 2) == true);
    double expected2[4] = {0.6, -0.7, -0.2, 0.4};
    assert(matricesEqual(inv2, expected2, 2));

    // Test 2: Verify A * inv = I for 3x3
    double A3[9] = {2.0, -1.0, 0.0, -1.0, 2.0, -1.0, 0.0, -1.0, 2.0};
    double inv3[9];
    assert(invertMatrix(A3, inv3, 3) == true);
    double product[9];
    for (uint16_t i = 0; i < 3; ++i) {
        for (uint16_t j = 0; j < 3; ++j) {
            product[i*3+j] = 0.0;
            for (uint16_t k = 0; k < 3; ++k) {
                product[i*3+j] += A3[i*3+k] * inv3[k*3+j];
            }
        }
    }
    double I3[9] = {1,0,0, 0,1,0, 0,0,1};
    assert(matricesEqual(product, I3, 3, 1e-9));

    // Test 3: 1x1 matrix
    double A1[1] = {5.0};
    double inv1[1];
    assert(invertMatrix(A1, inv1, 1) == true);
    assert(fabs(inv1[0] - 0.2) < 1e-12);

    // Test 4: Singular matrix
    double Asing[2] = {1.0, 2.0, 2.0, 4.0}; // rank 1
    double invsing[2];
    assert(invertMatrix(Asing, invsing, 2) == false);

    // Test 5: 4x4 random-ish non-singular (with known inverse from direct computation)
    double A4[16] = {
        1, 2, 3, 4,
        0, 1, 0, 2,
        5, 0, 0, 1,
        2, 3, 1, 0
    };
    double inv4[16];
    assert(invertMatrix(A4, inv4, 4) == true);
    double prod4[16];
    for (uint16_t i = 0; i < 4; ++i) {
        for (uint16_t j = 0; j < 4; ++j) {
            prod4[i*4+j] = 0.0;
            for (uint16_t k = 0; k < 4; ++k) {
                prod4[i*4+j] += A4[i*4+k] * inv4[k*4+j];
            }
        }
    }
    double I4[16] = {1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1};
    assert(matricesEqual(prod4, I4, 4, 1e-8));

    // Test 6: Identity matrix
    double I2[4] = {1,0,0,1};
    double invI2[4];
    assert(invertMatrix(I2, invI2, 2) == true);
    assert(matricesEqual(invI2, I2, 2));

    // Test 7: Diagonal matrix
    double D3[9] = {2,0,0, 0,4,0, 0,0,8};
    double invD3[9];
    assert(invertMatrix(D3, invD3, 3) == true);
    double expectedD[9] = {0.5,0,0, 0,0.25,0, 0,0,0.125};
    assert(matricesEqual(invD3, expectedD, 3));

    return 0;
}
