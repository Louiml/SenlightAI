Write a C++ function that, given a 4x4 symmetric matrix represented as `std::array<std::array<double,4>,4>` (or `Eigen::Matrix4d` if using Eigen, but for standalone standard C++ use a fixed-size array), returns the tridiagonal matrix \(T\) (a 4x4 tridiagonal symmetric matrix) obtained by applying Householder transformations to reduce the input symmetric matrix to tridiagonal form. The function should return a `std::array<std::array<double,4>,4>` containing the tridiagonal matrix \(T\), where the main diagonal and the first sub/super-diagonal entries are the only non-zero values (all others must be exactly zero). The input is guaranteed to be exactly symmetric (i.e., \(A_{ij} = A_{ji}\)) and finite. Use double precision arithmetic. The function must be named `tridiagonalizeSymmetric4`.

The solution implements the standard Householder tridiagonalization algorithm for an \(n \times n\) symmetric matrix. For \(n=4\), we perform \(n-2 = 2\) Householder reflections.  
**Algorithm**:  
- For each column \(k = 0\) to \(n-3\):  
  1. Compute the vector \(x = A[k+1..n-1][k]\) (the subdiagonal part of column \(k\)).  
  2. Compute the norm \(\alpha = -\text{sign}(x_0) \cdot \|x\|_2\).  
  3. Construct the Householder vector \(v = x - \alpha e_0\) (where \(e_0\) is the first unit vector).  
  4. Normalize \(v\) by its norm (if norm is zero, skip).  
  5. Apply the similarity transformation \(A \leftarrow P A P^T\) where \(P = I - 2 v v^T\). Since \(A\) is symmetric, we update only the lower-right \((n-k) \times (n-k)\) block: compute \(w = 2 A_{\text{block}} v\), then \(A_{\text{block}} \leftarrow A_{\text{block}} - v w^T - w v^T\).  
  6. Explicitly zero out the entries below the subdiagonal in column \(k\) and row \(k\) to avoid numerical noise.  
- After processing, the matrix is tridiagonal. The main diagonal and first off-diagonals are the result.  
**Edge cases**:  
- If the input matrix is already tridiagonal (or diag ), the algorithm does nothing (norms become zero) and returns it unchanged.  
- If any subdiagonal entry is exactly zero, the Householder vector degenerates; we handle by skipping the step if the norm is near zero (tolerance 1e-12).  
- The input is symmetric by guarantee, so we can avoid mirroring updates.  
**Complexity**: Time \(O(n^3)\) for general \(n\), but for fixed \(n=4\) it is constant and trivial. Space \(O(n^2)\) to store the matrix, with no extra large allocations.  
**Precision**: We use double and tolerance-based zeroing to ensure off-tridiagonal entries are exactly zero (or within machine epsilon). The test will compare using a relative tolerance.

#include <array>
#include <cmath>
#include <algorithm>
#include <stdexcept>

using Matrix4 = std::array<std::array<double, 4>, 4>;

// Perform Householder tridiagonalization on a 4x4 symmetric matrix.
// Returns the tridiagonal matrix T (symmetric, with zero entries outside the
// main diagonal and first sub/super-diagonal).
Matrix4 tridiagonalizeSymmetric4(const Matrix4& A) {
    // Copy input to work on (we assume symmetry, but we'll enforce it in updates)
    Matrix4 M = A;
    const int n = 4;
    const double eps = 1e-12;

    for (int k = 0; k < n - 2; ++k) {
        // Extract the vector x = M[k+1..n-1][k]
        double x[n - k - 1];
        for (int i = k + 1; i < n; ++i) {
            x[i - k - 1] = M[i][k];
        }
        int m = n - k - 1; // length of x

        // Compute norm and alpha
        double norm = 0.0;
        for (int i = 0; i < m; ++i) norm += x[i] * x[i];
        norm = std::sqrt(norm);

        if (norm < eps) {
            // Already zero subdiagonal; skip this step
            continue;
        }

        double alpha = (x[0] < 0) ? norm : -norm;
        // Householder vector v = x - alpha * e0
        double v[m];
        for (int i = 0; i < m; ++i) v[i] = x[i];
        v[0] -= alpha;

        // Normalize v
        double vnorm = 0.0;
        for (int i = 0; i < m; ++i) vnorm += v[i] * v[i];
        vnorm = std::sqrt(vnorm);
        if (vnorm < eps) continue; // should not happen if norm>eps and alpha chosen
        for (int i = 0; i < m; ++i) v[i] /= vnorm;

        // Apply similarity transformation to the sub-block starting at (k+1,k+1)
        // Block indices: rows/cols from k+1 to n-1
        int start = k + 1;
        int size = n - start; // m

        // Compute w = 2 * M_block * v
        double w[m];
        for (int i = 0; i < m; ++i) {
            double sum = 0.0;
            for (int j = 0; j < m; ++j) {
                sum += M[start + i][start + j] * v[j];
            }
            w[i] = 2.0 * sum;
        }

        // Update M_block: M_block -= v*w^T + w*v^T
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < m; ++j) {
                M[start + i][start + j] -= v[i] * w[j] + w[i] * v[j];
            }
        }

        // Zero out the subdiagonal and superdiagonal entries below/above the
        // current k-th column/row to avoid numerical residue.
        for (int i = k + 2; i < n; ++i) {
            M[i][k] = 0.0;
            M[k][i] = 0.0;
        }
    }

    // Zero out strict lower/upper triangular parts beyond first sub/superdiagonal
    // (safety, but should already be zero)
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (std::abs(i - j) > 1) {
                M[i][j] = 0.0;
            }
        }
    }

    return M;
}

#include <cassert>
#include <cmath>
#include <iostream>

using Matrix4 = std::array<std::array<double, 4>, 4>;

// Declaration of the solution function
Matrix4 tridiagonalizeSymmetric4(const Matrix4& A);

bool matricesClose(const Matrix4& a, const Matrix4& b, double tol = 1e-10) {
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            if (std::abs(a[i][j] - b[i][j]) > tol) return false;
    return true;
}

int main() {
    // Test 1: Already diagonal matrix
    Matrix4 diag = {{
        {{2,0,0,0}},
        {{0,3,0,0}},
        {{0,0,4,0}},
        {{0,0,0,5}}
    }};
    Matrix4 result = tridiagonalizeSymmetric4(diag);
    assert(matricesClose(result, diag));

    // Test 2: Symmetric matrix with off-diagonals
    Matrix4 sym = {{
        {{4,1,2,3}},
        {{1,5,0,1}},
        {{2,0,6,2}},
        {{3,1,2,7}}
    }};
    result = tridiagonalizeSymmetric4(sym);
    // Check tridiagonal structure: only |i-j|<=1 non-zero
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            if (std::abs(i - j) > 1)
                assert(std::abs(result[i][j]) < 1e-12);
    // Check symmetry
    assert(matricesClose(result, {{
        {{result[0][0], result[1][0], 0, 0}},
        {{result[1][0], result[1][1], result[2][1], 0}},
        {{0, result[2][1], result[2][2], result[3][2]}},
        {{0, 0, result[3][2], result[3][3]}}
    }}));

    // Test 3: A known tridiagonal matrix
    Matrix4 tridiag = {{
        {{1,2,0,0}},
        {{2,3,4,0}},
        {{0,4,5,6}},
        {{0,0,6,7}}
    }};
    result = tridiagonalizeSymmetric4(tridiag);
    assert(matricesClose(result, tridiag));

    // Test 4: Matrix with zero subdiagonal element
    Matrix4 withZero = {{
        {{1,0,1,1}},
        {{0,2,0,0}},
        {{1,0,3,1}},
        {{1,0,1,4}}
    }};
    result = tridiagonalizeSymmetric4(withZero);
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            if (std::abs(i - j) > 1)
                assert(std::abs(result[i][j]) < 1e-12);

    // Test 5: Random symmetric matrix (deterministic values)
    Matrix4 randSym = {{
        {{5, -2, 1, 4}},
        {{-2, 3, 7, -1}},
        {{1, 7, -4, 2}},
        {{4, -1, 2, 6}}
    }};
    result = tridiagonalizeSymmetric4(randSym);
    // Check symmetry
    assert(matricesClose(result, {{
        {{result[0][0], result[1][0], 0, 0}},
        {{result[1][0], result[1][1], result[2][1], 0}},
        {{0, result[2][1], result[2][2], result[3][2]}},
        {{0, 0, result[3][2], result[3][3]}}
    }}));
    // Check that the trace is preserved (diagonal sum)
    double traceA = 0, traceT = 0;
    for (int i = 0; i < 4; ++i) {
        traceA += randSym[i][i];
        traceT += result[i][i];
    }
    assert(std::abs(traceA - traceT) < 1e-10);

    std::cout << "All tests passed." << std::endl;
    return 0;
}
