// Write a standalone C++ function named `matrixMultiplySimd` that multiplies two 2D matrices `A` (size `m × n`) and `B` (size `n × k`) in row-major order, storing the result in matrix `C` (size `m × k`). The function must use SIMD intrinsics (e.g., AVX2) to accelerate the inner loop, but must also correctly handle cases where the inner dimension `n` is not a multiple of the SIMD vector width (e.g., 8 floats for AVX). Additionally, the function should take `float` pointers for A, B, C and integer dimensions `m`, `n`, `k`, and it must not modify the input matrices. The output must be numerically correct to within a floating-point tolerance of 1e-4 compared to a naive scalar multiplication. Your function should be self-contained, include the necessary headers, and use appropriate `const` correctness. Do not write a `main` function—only the function definition.
#include <cassert>
#include <cmath>
#include <cstddef>
#include <vector>

// Declare the function being tested (normally comes from header).
void matrixMultiplySimd(const float* A, const float* B, float* C,
                        int m, int n, int k);

// Scalar reference multiplication for testing.
void matrixMultiplyScalar(const float* A, const float* B, float* C,
                          int m, int n, int k) {
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < k; ++j) {
            float sum = 0.0f;
            for (int t = 0; t < n; ++t) {
                sum += A[static_cast<std::size_t>(i) * n + t] *
                       B[static_cast<std::size_t>(t) * k + j];
            }
            C[static_cast<std::size_t>(i) * k + j] = sum;
        }
    }
}

bool matricesClose(const float* A, const float* B, std::size_t size, float eps) {
    for (std::size_t i = 0; i < size; ++i) {
        if (std::fabs(A[i] - B[i]) > eps) return false;
    }
    return true;
}

int main() {
    // Test 1: 2x3 * 3x4 (k=4, not a multiple of 8)
    {
        const int m = 2, n = 3, k = 4;
        std::vector<float> A = {1, 2, 3, 4, 5, 6};
        std::vector<float> B = {7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18};
        std::vector<float> C_simd(m * k, 0.0f), C_ref(m * k, 0.0f);
        matrixMultiplySimd(A.data(), B.data(), C_simd.data(), m, n, k);
        matrixMultiplyScalar(A.data(), B.data(), C_ref.data(), m, n, k);
        assert(matricesClose(C_simd.data(), C_ref.data(), m * k, 1e-4));
    }

    // Test 2: 3x3 * 3x3 (k=3, less than 8)
    {
        const int m = 3, n = 3, k = 3;
        std::vector<float> A = {1, 0, 0, 0, 1, 0, 0, 0, 1};
        std::vector<float> B = {1, 2, 3, 4, 5, 6, 7, 8, 9};
        std::vector<float> C_simd(m * k, 0.0f), C_ref(m * k, 0.0f);
        matrixMultiplySimd(A.data(), B.data(), C_simd.data(), m, n, k);
        matrixMultiplyScalar(A.data(), B.data(), C_ref.data(), m, n, k);
        assert(matricesClose(C_simd.data(), C_ref.data(), m * k, 1e-4));
    }

    // Test 3: 1x1 * 1x1 (edge case single element)
    {
        const int m = 1, n = 1, k = 1;
        std::vector<float> A = {2.5f};
        std::vector<float> B = {4.0f};
        std::vector<float> C_simd(1, 0.0f), C_ref(1, 0.0f);
        matrixMultiplySimd(A.data(), B.data(), C_simd.data(), m, n, k);
        matrixMultiplyScalar(A.data(), B.data(), C_ref.data(), m, n, k);
        assert(matricesClose(C_simd.data(), C_ref.data(), 1, 1e-4));
    }

    // Test 4: 4x8 * 8x16 (k=16, multiple of 8)
    {
        const int m = 4, n = 8, k = 16;
        std::vector<float> A(m * n), B(n * k), C_simd(m * k), C_ref(m * k);
        float val = 0.5f;
        for (auto& x : A) x = val += 0.1f;
        for (auto& x : B) x = val += 0.1f;
        matrixMultiplySimd(A.data(), B.data(), C_simd.data(), m, n, k);
        matrixMultiplyScalar(A.data(), B.data(), C_ref.data(), m, n, k);
        assert(matricesClose(C_simd.data(), C_ref.data(), m * k, 1e-3));
    }

    // Test 5: Zero dimension (should not crash, C remains zero)
    {
        const int m = 2, n = 0, k = 3;
        std::vector<float> A = {1, 2};
        std::vector<float> B = {}; // empty
        std::vector<float> C_simd(m * k, 1.0f);
        matrixMultiplySimd(A.data(), B.data(), C_simd.data(), m, n, k);
        for (auto x : C_simd) assert(x == 0.0f);
    }

    // Test 6: Large random matrix with k=12 (not multiple of 8, >8)
    {
        const int m = 5, n = 6, k = 12;
        std::vector<float> A(m * n), B(n * k), C_simd(m * k), C_ref(m * k);
        float seed = 1.0f;
        for (auto& x : A) x = seed = seed * 1.3f + 0.2f;
        for (auto& x : B) x = seed = seed * 1.7f - 0.5f;
        matrixMultiplySimd(A.data(), B.data(), C_simd.data(), m, n, k);
        matrixMultiplyScalar(A.data(), B.data(), C_ref.data(), m, n, k);
        assert(matricesClose(C_simd.data(), C_ref.data(), m * k, 1e-3));
    }

    return 0;
}
#include <cstddef>
#include <immintrin.h>

// Multiply matrices A (m x n) and B (n x k) into C (m x k) using AVX2 SIMD.
// Handles any value of k, including non-multiples of 8.
void matrixMultiplySimd(const float* A, const float* B, float* C,
                        int m, int n, int k) {
    if (m <= 0 || n <= 0 || k <= 0) return;

    // Zero out C first.
    const std::size_t c_size = static_cast<std::size_t>(m) * k;
    for (std::size_t idx = 0; idx < c_size; ++idx) {
        C[idx] = 0.0f;
    }

    const int simd_width = 8; // AVX2 floats per vector

    // Loop over rows of A.
    for (int i = 0; i < m; ++i) {
        float* c_row = C + static_cast<std::size_t>(i) * k;
        const float* a_row = A + static_cast<std::size_t>(i) * n;

        // For each column block of size simd_width in C.
        int j_block = 0;
        while (j_block + simd_width <= k) {
            // Accumulator for this block.
            __m256 acc = _mm256_setzero_ps();

            // Iterate over shared dimension.
            for (int t = 0; t < n; ++t) {
                float a_val = a_row[t];
                const float* b_row = B + static_cast<std::size_t>(t) * k + j_block;
                __m256 b_vec = _mm256_loadu_ps(b_row);
                __m256 a_vec = _mm256_set1_ps(a_val);
                acc = _mm256_add_ps(acc, _mm256_mul_ps(a_vec, b_vec));
            }
            _mm256_storeu_ps(c_row + j_block, acc);
            j_block += simd_width;
        }

        // Handle remaining columns (0 to 7) with scalar code.
        while (j_block < k) {
            float sum = 0.0f;
            for (int t = 0; t < n; ++t) {
                sum += a_row[t] * B[static_cast<std::size_t>(t) * k + j_block];
            }
            c_row[j_block] = sum;
            ++j_block;
        }
    }
}
// The core algorithm is standard matrix multiplication: for each row `i` of A and column `j` of B, compute the dot product of A's row with B's column. However, to use SIMD efficiently, we restructure the loops: for each row `i` of A, we accumulate into an entire row of C. The inner loop iterates over the shared dimension `t` (0 to n-1), and for each `t`, we broadcast `A[i*n + t]` into a SIMD vector and multiply it with a vector loaded from `B[t*k + (0..k-1)]`, adding to the accumulator vector for C's row. This is the standard "outer product" formulation. To handle `k` not being a multiple of 8, we process blocks of 8 using AVX, then handle the remainder (0 to 7 elements) with scalar code. Edge cases include: empty matrices (m, n, or k zero—then just return, no work), and when `k < 8` we skip SIMD entirely. Time complexity is O(m·n·k), same as naive, but SIMD provides a constant-factor speedup. Space complexity is O(m·k) for the output matrix (already allocated by caller). We use `#include <immintrin.h>` for AVX intrinsics, and we must align our loads/accumulator appropriately; we use unaligned loads (`_mm256_loadu_ps`) to be safe. `const` correctness: A and B are `const float*`, and the function returns void but fills C (which is non-const). We should also use `restrict` or `__restrict` if compiler allows, but we keep it simple.
