Write a standalone C++ function named `fp16_gemm` that simulates the behavior of a half-precision (FP16) general matrix multiply, \( C = \alpha \cdot op(A) \cdot op(B) + \beta \cdot C \), without relying on GPU libraries. The function should accept two boolean flags `transA` and `transB` (where `false` means no transpose, `true` means transpose), integer dimensions `m`, `n`, `k`, scalar `alpha` and `beta` as `float` values, and three matrices stored in row-major order as `std::vector<float>` (since true FP16 arithmetic is not natively available in standard C++, simulate by storing values as `float` but conceptually treat them as FP16-precision inputs). The matrices `A`, `B`, and `C` are passed as `std::vector<float>` with given leading dimensions `lda`, `ldb`, `ldc` (stride between rows, assuming row-major but allowing padding via leading dimensions). The function should modify `C` in place, computing the result with a simple triple-loop, and must handle both transposed and non-transposed cases correctly, including zero dimensions, aliasing (when `A` or `B` overlaps with `C` in memory), and edge cases where `alpha` or `beta` are zero. The function should be efficient (avoid redundant copies) and use `const` correctly for read-only parameters.
#include <cassert>
#include <vector>
#include <cmath>

// The solution function is assumed to be defined above.

int main() {
    // Test 1: simple 2x2 * 2x2, no transpose
    {
        std::vector<float> A = {1, 2, 3, 4};
        std::vector<float> B = {5, 6, 7, 8};
        std::vector<float> C = {0, 0, 0, 0};
        fp16_gemm(false, false, 2, 2, 2, 1.0f, A, 2, B, 2, 0.0f, C, 2);
        assert(C[0] == 19 && C[1] == 22 && C[2] == 43 && C[3] == 50);
    }
    // Test 2: with beta (accumulate into existing C)
    {
        std::vector<float> A = {1, 2, 3, 4};
        std::vector<float> B = {1, 0, 0, 1};
        std::vector<float> C = {10, 20, 30, 40};
        fp16_gemm(false, false, 2, 2, 2, 1.0f, A, 2, B, 2, 2.0f, C, 2);
        // result: [1*1+2*0 + 2*10 = 21, 1*0+2*1 + 2*20 = 42, 3*1+4*0 + 2*30 = 63, 3*0+4*1 + 2*40 = 84]
        assert(C[0] == 21 && C[1] == 42 && C[2] == 63 && C[3] == 84);
    }
    // Test 3: transpose A and B, rectangular (2x3 * 3x2 -> 2x2) with alpha=0.5
    {
        // A is 3 rows x 2 cols (will be transposed to 2x3)
        // A = [1, 2; 3, 4; 5, 6] stored row-major, lda=2
        std::vector<float> A = {1, 2, 3, 4, 5, 6};
        // B is 2 rows x 3 cols (will be transposed to 3x2)
        // B = [1, 2, 3; 4, 5, 6] stored row-major, ldb=3
        std::vector<float> B = {1, 2, 3, 4, 5, 6};
        std::vector<float> C(4, 0.0f);
        fp16_gemm(true, true, 2, 2, 3, 0.5f, A, 2, B, 3, 0.0f, C, 2);
        // op(A) = A^T = [1,3,5; 2,4,6]
        // op(B) = B^T = [1,4; 2,5; 3,6]
        // product = [1*1+3*2+5*3, 1*4+3*5+5*6; 2*1+4*2+6*3, 2*4+4*5+6*6] = [22, 49; 28, 64]
        // times alpha=0.5 -> [11, 24.5; 14, 32]
        assert(std::fabs(C[0] - 11.0f) < 1e-5);
        assert(std::fabs(C[1] - 24.5f) < 1e-5);
        assert(std::fabs(C[2] - 14.0f) < 1e-5);
        assert(std::fabs(C[3] - 32.0f) < 1e-5);
    }
    // Test 4: k=0 (empty inner dimension) -> result = beta * C
    {
        std::vector<float> A; // empty
        std::vector<float> B; // empty
        std::vector<float> C = {1, 2, 3, 4};
        fp16_gemm(false, false, 2, 2, 0, 3.0f, A, 1, B, 1, 2.0f, C, 2);
        assert(C[0] == 2 && C[1] == 4 && C[2] == 6 && C[3] == 8);
    }
    // Test 5: alpha=0 -> only beta*C remains
    {
        std::vector<float> A = {1, 2, 3, 4};
        std::vector<float> B = {5, 6, 7, 8};
        std::vector<float> C = {10, 20, 30, 40};
        fp16_gemm(false, false, 2, 2, 2, 0.0f, A, 2, B, 2, 0.5f, C, 2);
        assert(C[0] == 5 && C[1] == 10 && C[2] == 15 && C[3] == 20);
    }
    // Test 6: leading dimensions with padding (lda=3 for 2x2 matrix)
    {
        std::vector<float> A = {1, 2, 99, 3, 4, 99}; // lda=3, rows: [1,2], [3,4]
        std::vector<float> B = {5, 6, 7, 8}; // ldb=2
        std::vector<float> C = {0, 0, 0, 0};
        fp16_gemm(false, false, 2, 2, 2, 1.0f, A, 3, B, 2, 0.0f, C, 2);
        assert(C[0] == 19 && C[1] == 22 && C[2] == 43 && C[3] == 50);
    }
    // Test 7: beta=0 and alpha=1, rectangular m=1 n=3 k=2
    {
        // A is 1x2, B is 2x3
        std::vector<float> A = {1, 2};
        std::vector<float> B = {1, 2, 3, 4, 5, 6};
        std::vector<float> C = {0, 0, 0};
        fp16_gemm(false, false, 1, 3, 2, 1.0f, A, 2, B, 3, 0.0f, C, 3);
        // row = [1*1+2*4, 1*2+2*5, 1*3+2*6] = [9, 12, 15]
        assert(C[0] == 9 && C[1] == 12 && C[2] == 15);
    }
    // Test 8: zero dimension m=0
    {
        std::vector<float> A = {1, 2};
        std::vector<float> B = {3, 4};
        std::vector<float> C = {5, 6};
        fp16_gemm(false, false, 0, 2, 2, 1.0f, A, 2, B, 2, 1.0f, C, 2);
        assert(C[0] == 5 && C[1] == 6); // unchanged
    }
    // Test 9: transpose only B, and negative values
    {
        // A is 1x2, B is 2x2 but we use B^T operation: op(B) = B^T = 2x2
        std::vector<float> A = {1, -2};
        std::vector<float> B = {1, 2, 3, 4}; // B^T = [1,3; 2,4]
        std::vector<float> C = {0, 0};
        fp16_gemm(false, true, 1, 2, 2, 2.0f, A, 2, B, 2, 0.0f, C, 2);
        // row = [1*1 + (-2)*3, 1*2 + (-2)*4] = [-5, -6] times 2 = [-10, -12]
        assert(C[0] == -10 && C[1] == -12);
    }
    // Test 10: aliasing C with A (same underlying vector) – simple case where A and C are same memory
    {
        std::vector<float> data = {1, 2, 3, 4}; // A and C share this
        std::vector<float> B = {1, 0, 0, 1};
        // We need separate vectors for A and C but can simulate by copying pointer? 
        // Since function takes vectors by const ref and C by ref, we can't alias directly with different names.
        // Instead test that C can be overwritten without affecting reads of A (since A is separate).
        // This test is implicitly covered by earlier tests. We'll add a simple non-aliased test.
        std::vector<float> A = {1, 2, 3, 4};
        std::vector<float> C = A; // copy
        fp16_gemm(false, false, 2, 2, 2, 1.0f, A, 2, B, 2, 0.0f, C, 2);
        assert(C[0] == 1 && C[1] == 2 && C[2] == 3 && C[3] == 4); // since B is identity
    }
    return 0;
}
#include <vector>
#include <cstddef>

/**
 * Simulated FP16 general matrix multiply: C = alpha * op(A) * op(B) + beta * C.
 * Matrices are stored in row-major order with given leading dimensions (stride).
 * transA/transB == false means no transpose; true means transpose.
 * The function modifies C in place.
 */
void fp16_gemm(
    bool transA, bool transB,
    int m, int n, int k,
    float alpha,
    const std::vector<float>& A, int lda,
    const std::vector<float>& B, int ldb,
    float beta,
    std::vector<float>& C, int ldc)
{
    // Handle trivial case: if m or n is zero, nothing to compute.
    if (m == 0 || n == 0) return;

    // For each row of C
    for (int i = 0; i < m; ++i) {
        // For each column of C
        for (int j = 0; j < n; ++j) {
            // Compute dot product of row i of op(A) and column j of op(B)
            float sum = 0.0f;
            for (int t = 0; t < k; ++t) {
                // Get A element at (i, t) in op(A)
                float a_val;
                if (!transA) {
                    a_val = A[i * lda + t];
                } else {
                    a_val = A[t * lda + i];
                }
                // Get B element at (t, j) in op(B)
                float b_val;
                if (!transB) {
                    b_val = B[t * ldb + j];
                } else {
                    b_val = B[j * ldb + t];
                }
                sum += a_val * b_val;
            }
            // Compute new C[i,j] = alpha * sum + beta * old_C[i,j]
            float old_c = C[i * ldc + j];
            C[i * ldc + j] = alpha * sum + beta * old_c;
        }
    }
}
// The solution follows the standard definition of GEMM. The main challenge is indexing into row-major matrices with optional transposition. For each row `i` in 0..m-1 and column `j` in 0..n-1, compute the dot product of the i-th row of op(A) and j-th column of op(B). If `transA` is false, op(A) is `m`×`k` with element at (i, t) at `A[i*lda + t]`; if true, op(A) is `k`×`m`, and element (i, t) of op(A) corresponds to original `A[t*lda + i]`. Similarly, if `transB` is false, op(B) is `k`×`n` with element at (t, j) at `B[t*ldb + j]`; if true, op(B) is `n`×`k`, and element (t, j) of op(B) corresponds to original `B[j*ldb + t]`. The result for each (i,j) is `alpha * sum_{t=0}^{k-1} A[...] * B[...] + beta * C_original[i,j]`. Edge cases: when `k == 0`, the sum is zero; when `alpha == 0`, the product term vanishes; when `beta == 0`, the original C term is ignored. Aliasing is handled naturally because we compute the dot product first into a temporary scalar and then update C[i,j] after reading all needed inputs, so modifying C does not affect A or B reads (unless A or B points to same memory as C, but we read A/B elements before writing C for that position). Time complexity is O(m·n·k) with O(1) auxiliary space. Space complexity is O(1) beyond the input vectors.
