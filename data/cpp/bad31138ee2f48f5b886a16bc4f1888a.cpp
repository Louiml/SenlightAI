Write a standalone C++ function `generatePositiveDefiniteMatrix(double* A, int n)` that fills the column-major array `A` of size `n*n` with a symmetric positive definite matrix generated as follows: create a lower-triangular matrix `L` (n x n) where for each column `j`, the diagonal entry `L[j*n + j] = 1`, and for each row index `k < j`, set `L[k*n + j] = ( ( (j*k)/( (double)(j+1) ) / ( (double)(k+2) ) * 2.0 ) - 1.0 ) / ( (double)n )`. All other entries (above diagonal) are zero. Then compute `A = L * L^T` using a simple triple-nested loop (do not use external BLAS). The function must allocate no dynamic memory; it assumes `A` already points to `n*n` contiguous doubles. Ensure proper symmetry (A[i*n+j] == A[j*n+i]) and positive definiteness (all leading principal minors > 0, which holds because L has nonzero diagonal). Handle `n <= 0` by doing nothing (return early). The matrix entries must be exactly reproducible for the same `n`, so use only integer arithmetic for indices and `double` arithmetic for values, with no randomness.
// The core idea is to construct a lower triangular matrix `L` with unit diagonal and prescribed off-diagonal entries, then compute `A = L * L^T`. Because `L` has nonzero diagonal (all 1s), it is invertible, and for any nonzero vector `x`, `x^T A x = (L^T x)^T (L^T x) = ||L^T x||^2 > 0` since `L^T` is also invertible, thus `A` is positive definite. The off-diagonal entries are chosen as a deterministic rational function of indices to produce a well-conditioned matrix; the `/n` scaling keeps values small for larger `n`. Implementation details: initialize all entries of `A` to zero, fill `L` only in the lower triangle (including diagonal), then compute matrix product `A[i*n + j] = sum_{k=0}^{n-1} L[i*n + k] * L[j*n + k]` (which equals dot product of row i of L and row j of L, but since L is lower triangular, the sum effectively runs `k=0` to `min(i,j)` only, but we can just loop all k for simplicity). The symmetry arises automatically from the dot product formula: A[i][j] = A[j][i] because dot product is commutative. Edge case: if `n <= 0`, simply return. Time complexity: O(n^3) for the matrix multiplication; space complexity: O(1) extra (only a few double temporaries). The function should be `const`-correct: `A` is non-const pointer to output data.
#include <cstddef>

// Fill the column-major n*n array A with a symmetric positive definite matrix
// generated as L * L^T, where L is a lower triangular matrix with unit diagonal
// and deterministic off-diagonal values. Assumes A points to at least n*n doubles.
void generatePositiveDefiniteMatrix(double* A, int n) {
    if (n <= 0) return;

    // Temporary storage for L (n*n doubles) allocated on stack; but n could be large.
    // For safety, we use a simple vector-like approach with dynamic allocation to avoid stack overflow.
    // However, the task says "allocate no dynamic memory" — but that is impossible for large n without external storage.
    // To honor the spirit, we can use a local buffer with a fixed maximum? The problem statement says "no dynamic memory".
    // We'll interpret that as no malloc/calloc inside the function; we can use a temporary on stack if n is small,
    // but for generality we use a static thread_local buffer? That is not thread-safe.
    // Given the reference snippet uses calloc, we can use a local std::vector for clarity, but the task explicitly says "no dynamic memory".
    // To satisfy, we can compute A on the fly without storing L: for each i,j, we compute sum over k of L[i*N+k]*L[j*N+k].
    // But L is defined implicitly. We'll compute the product directly using formulas without storing L.
    // Since L is lower triangular, L[i*N+k] is zero for k>i, and for k<=i we have:
    // if k==i then L[i*N+i]=1; else if k<i then L[i*N+k] = ( ( (i*k)/( (double)(i+1) ) / ( (double)(k+2) ) * 2.0 ) - 1.0 ) / n;
    // For L[j*N+k], similarly. So we can compute each A[i][j] as sum_{k=0}^{min(i,j)} L_i_k * L_j_k.
    // But that is still O(n^3) but no extra memory. We'll do that.

    const double inv_n = 1.0 / static_cast<double>(n);

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            double sum = 0.0;
            int kmax = i < j ? i : j; // min(i,j)
            for (int k = 0; k <= kmax; ++k) {
                double l_i_k = 0.0;
                double l_j_k = 0.0;

                if (k == i) l_i_k = 1.0;
                else if (k < i) {
                    l_i_k = ( ( (static_cast<double>(i) * static_cast<double>(k)) /
                                (static_cast<double>(i + 1)) /
                                (static_cast<double>(k + 2)) * 2.0 ) - 1.0 ) * inv_n;
                }

                if (k == j) l_j_k = 1.0;
                else if (k < j) {
                    l_j_k = ( ( (static_cast<double>(j) * static_cast<double>(k)) /
                                (static_cast<double>(j + 1)) /
                                (static_cast<double>(k + 2)) * 2.0 ) - 1.0 ) * inv_n;
                }

                sum += l_i_k * l_j_k;
            }
            A[j * n + i] = sum; // column-major: A[i][j] stored at j*n+i
        }
    }
}
#include <cassert>
#include <cmath>
#include <cstdio>

// forward declaration
void generatePositiveDefiniteMatrix(double* A, int n);

int main() {
    // Test n=1
    double A1[1];
    generatePositiveDefiniteMatrix(A1, 1);
    assert(std::fabs(A1[0] - 1.0) < 1e-12);

    // Test n=2, check symmetry and positive definiteness (determinant > 0)
    double A2[4];
    generatePositiveDefiniteMatrix(A2, 2);
    assert(std::fabs(A2[0] - A2[0]) < 1e-12); // trivially
    assert(std::fabs(A2[1] - A2[2]) < 1e-12); // A[0][1] == A[1][0]
    double det = A2[0]*A2[3] - A2[1]*A2[2];
    assert(det > 0.0);

    // Test n=3, explicitly compute from known formulas
    double A3[9];
    generatePositiveDefiniteMatrix(A3, 3);
    // L for n=3: 
    // L = [1 0 0; a 1 0; b c 1]
    // a = ((1*0)/(2)/(2)*2 -1)/3 = (-1)/3 = -0.3333333333333333
    // b = ((2*0)/(3)/(2)*2 -1)/3 = (-1)/3 = -0.3333333333333333
    // c = ((2*1)/(3)/(3)*2 -1)/3 = ( (2/9)*2 -1 )/3 = (4/9 -1)/3 = (-5/9)/3 = -5/27 ≈ -0.18518518518518517
    // A = L*L^T:
    // A[0][0] = 1
    // A[0][1] = a
    // A[0][2] = b
    // A[1][1] = a^2 + 1
    // A[1][2] = a*b + c
    // A[2][2] = b^2 + c^2 + 1
    double a = -1.0/3.0;
    double b = -1.0/3.0;
    double c = -5.0/27.0;
    assert(std::fabs(A3[0] - 1.0) < 1e-12); // A[0][0]
    assert(std::fabs(A3[1] - a) < 1e-12); // A[0][1] column-major index 1
    assert(std::fabs(A3[2] - b) < 1e-12); // A[0][2] column-major index 2
    assert(std::fabs(A3[3] - a) < 1e-12); // A[1][0]
    assert(std::fabs(A3[4] - (a*a + 1.0)) < 1e-12);
    assert(std::fabs(A3[5] - (a*b + c)) < 1e-12);
    assert(std::fabs(A3[6] - b) < 1e-12);
    assert(std::fabs(A3[7] - (a*b + c)) < 1e-12);
    assert(std::fabs(A3[8] - (b*b + c*c + 1.0)) < 1e-12);

    // Test n=0 does nothing (no crash)
    double A0[1];
    generatePositiveDefiniteMatrix(A0, 0);

    // Test n=4 symmetry
    double A4[16];
    generatePositiveDefiniteMatrix(A4, 4);
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            assert(std::fabs(A4[j*4+i] - A4[i*4+j]) < 1e-12);

    // Test positive definiteness for n=4 via leading principal minors
    // Compute minors: det of top-left kxk
    for (int k = 1; k <= 4; ++k) {
        double minor[16];
        for (int i = 0; i < k; ++i)
            for (int j = 0; j < k; ++j)
                minor[j*k+i] = A4[j*4+i];
        // compute determinant via Gaussian elimination (simple)
        double det = 1.0;
        double temp[16];
        for (int i = 0; i < k*k; ++i) temp[i] = minor[i];
        for (int i = 0; i < k; ++i) {
            // find pivot
            int pivot = i;
            for (int r = i+1; r < k; ++r)
                if (std::fabs(temp[r*k+i]) > std::fabs(temp[pivot*k+i])) pivot = r;
            if (std::fabs(temp[pivot*k+i]) < 1e-12) { det = 0; break; }
            if (pivot != i) {
                for (int c = 0; c < k; ++c) {
                    double t = temp[i*k+c];
                    temp[i*k+c] = temp[pivot*k+c];
                    temp[pivot*k+c] = t;
                }
                det = -det;
            }
            double piv = temp[i*k+i];
            det *= piv;
            for (int r = i+1; r < k; ++r) {
                double factor = temp[r*k+i] / piv;
                for (int c = i; c < k; ++c)
                    temp[r*k+c] -= factor * temp[i*k+c];
            }
        }
        assert(det > 0.0);
    }

    // Test reproducibility: same n yields same matrix
    double B4[16];
    generatePositiveDefiniteMatrix(B4, 4);
    for (int i = 0; i < 16; ++i) assert(std::fabs(A4[i] - B4[i]) < 1e-12);

    printf("All tests passed.\n");
    return 0;
}
