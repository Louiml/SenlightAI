// Write a standalone C++ function named `computeGEMMBlocked` that performs blocked general matrix multiplication with an optional transpose on matrix B, mimicking the core computational strategy of the provided OpenCL GEMM kernel without hardware-specific code. Specifically, the function should compute `C = A * B` (where A is an `M x K` matrix and B is a `K x N` matrix) or `C = A * B^T` (where B is an `N x K` matrix and B is transposed to `K x N`), with result C of size `M x N`. The inputs are `std::vector<float>` in row-major order, along with dimensions `M`, `N`, `K`, and a boolean `transposeB`. Use fixed block sizes (e.g., 32 for both dimensions) to tile the computation, accumulate intermediate results in double precision for numerical stability, and write the output into a pre-allocated `std::vector<float>& C`. Handle partial edge blocks correctly by clamping to the actual matrix boundaries. Return `true` on success and `false` if any dimension is non-positive or if the input vectors have insufficient size based on the dimensions and transpose flag.

#include <cassert>
#include <vector>
#include <cmath>

int main() {
    // Test 1: Simple 2x2 multiply (no transpose)
    {
        std::vector<float> A = {1, 2, 3, 4}; // 2x2
        std::vector<float> B = {5, 6, 7, 8}; // 2x2
        std::vector<float> C(4, 0.0f);
        bool ok = computeGEMMBlocked(A, B, 2, 2, 2, false, C);
        assert(ok);
        assert(std::fabs(C[0] - 19.0f) < 0.0001f); // 1*5+2*7
        assert(std::fabs(C[1] - 22.0f) < 0.0001f); // 1*6+2*8
        assert(std::fabs(C[2] - 43.0f) < 0.0001f); // 3*5+4*7
        assert(std::fabs(C[3] - 50.0f) < 0.0001f); // 3*6+4*8
    }

    // Test 2: 2x3 * 3x2 = 2x2 (no transpose)
    {
        std::vector<float> A = {1, 2, 3, 4, 5, 6}; // 2x3
        std::vector<float> B = {7, 8, 9, 10, 11, 12}; // 3x2
        std::vector<float> C(4, 0.0f);
        bool ok = computeGEMMBlocked(A, B, 2, 2, 3, false, C);
        assert(ok);
        assert(std::fabs(C[0] - 58.0f) < 0.0001f); // 1*7+2*9+3*11
        assert(std::fabs(C[1] - 64.0f) < 0.0001f); // 1*8+2*10+3*12
        assert(std::fabs(C[2] - 139.0f) < 0.0001f); // 4*7+5*9+6*11
        assert(std::fabs(C[3] - 154.0f) < 0.0001f); // 4*8+5*10+6*12
    }

    // Test 3: 2x2 * 2x2^T (B is 2x2, transpose gives 2x2)
    {
        std::vector<float> A = {1, 2, 3, 4}; // 2x2
        std::vector<float> B = {5, 6, 7, 8}; // 2x2 (will be transposed)
        std::vector<float> C(4, 0.0f);
        bool ok = computeGEMMBlocked(A, B, 2, 2, 2, true, C);
        assert(ok);
        // B^T = [5 7; 6 8], so C = A * B^T
        assert(std::fabs(C[0] - 19.0f) < 0.0001f); // 1*5+2*6
        assert(std::fabs(C[1] - 23.0f) < 0.0001f); // 1*7+2*8
        assert(std::fabs(C[2] - 43.0f) < 0.0001f); // 3*5+4*6
        assert(std::fabs(C[3] - 53.0f) < 0.0001f); // 3*7+4*8
    }

    // Test 4: 3x2 * 2x3^T = 3x3 (B is 3x2)
    {
        std::vector<float> A = {1, 2, 3, 4, 5, 6}; // 3x2
        std::vector<float> B = {7, 8, 9, 10, 11, 12}; // 3x2 (transpose to 2x3)
        std::vector<float> C(9, 0.0f);
        bool ok = computeGEMMBlocked(A, B, 3, 3, 2, true, C);
        assert(ok);
        // B^T = [7 9 11; 8 10 12], so C = A * B^T
        // Row 0 of C: [1*7+2*8, 1*9+2*10, 1*11+2*12] = [23, 29, 35]
        assert(std::fabs(C[0] - 23.0f) < 0.0001f);
        assert(std::fabs(C[1] - 29.0f) < 0.0001f);
        assert(std::fabs(C[2] - 35.0f) < 0.0001f);
        // Row 1: [3*7+4*8, 3*9+4*10, 3*11+4*12] = [53, 67, 81]
        assert(std::fabs(C[3] - 53.0f) < 0.0001f);
        assert(std::fabs(C[4] - 67.0f) < 0.0001f);
        assert(std::fabs(C[5] - 81.0f) < 0.0001f);
        // Row 2: [5*7+6*8, 5*9+6*10, 5*11+6*12] = [83, 105, 127]
        assert(std::fabs(C[6] - 83.0f) < 0.0001f);
        assert(std::fabs(C[7] - 105.0f) < 0.0001f);
        assert(std::fabs(C[8] - 127.0f) < 0.0001f);
    }

    // Test 5: Edge case with dimensions not multiples of 32 (e.g., 1x1)
    {
        std::vector<float> A = {2.0f};
        std::vector<float> B = {3.0f};
        std::vector<float> C(1, 0.0f);
        bool ok = computeGEMMBlocked(A, B, 1, 1, 1, false, C);
        assert(ok);
        assert(std::fabs(C[0] - 6.0f) < 0.0001f);
    }

    // Test 6: Edge case with invalid dimensions
    {
        std::vector<float> A(4, 1.0f);
        std::vector<float> B(4, 1.0f);
        std::vector<float> C(4, 0.0f);
        bool ok = computeGEMMBlocked(A, B, 0, 2, 2, false, C);
        assert(!ok);
    }

    // Test 7: Edge case with insufficient B size
    {
        std::vector<float> A(4, 1.0f); // 2x2
        std::vector<float> B(2, 1.0f); // too small for 2x2
        std::vector<float> C(4, 0.0f);
        bool ok = computeGEMMBlocked(A, B, 2, 2, 2, false, C);
        assert(!ok);
    }

    // Test 8: Larger blocked multiply with M=35, N=40, K=33 (non-multiples)
    {
        int M = 35, N = 40, K = 33;
        std::vector<float> A(M * K);
        std::vector<float> B(K * N);
        std::vector<float> C(M * N, 0.0f);
        for (int i = 0; i < M * K; ++i) A[i] = static_cast<float>(i % 7 + 1);
        for (int i = 0; i < K * N; ++i) B[i] = static_cast<float>(i % 5 + 1);
        bool ok = computeGEMMBlocked(A, B, M, N, K, false, C);
        assert(ok);
        // Verify a few random entries manually
        int row = 3, col = 7;
        double expected = 0.0;
        for (int k = 0; k < K; ++k) {
            expected += A[row * K + k] * B[k * N + col];
        }
        assert(std::fabs(C[row * N + col] - static_cast<float>(expected)) < 0.001f);
    }

    // Test 9: Ensure C is not overwritten incorrectly when beta=0 (initialization)
    {
        std::vector<float> A = {1, 0, 0, 1}; // identity
        std::vector<float> B = {2, 3, 4, 5}; // 2x2
        std::vector<float> C = {100, 100, 100, 100}; // pre-filled
        bool ok = computeGEMMBlocked(A, B, 2, 2, 2, false, C);
        assert(ok);
        assert(std::fabs(C[0] - 2.0f) < 0.0001f);
        assert(std::fabs(C[1] - 3.0f) < 0.0001f);
        assert(std::fabs(C[2] - 4.0f) < 0.0001f);
        assert(std::fabs(C[3] - 5.0f) < 0.0001f);
    }

    return 0;
}

#include <vector>
#include <cstddef>
#include <algorithm>

// Compute C = A * B (or A * B^T) using blocked GEMM.
// A: M x K row-major
// B: K x N row-major if transposeB == false, else N x K row-major
// C: M x N row-major (pre-allocated)
// Returns false if dimensions are invalid or vector sizes insufficient.
bool computeGEMMBlocked(const std::vector<float>& A,
                        const std::vector<float>& B,
                        int M, int N, int K,
                        bool transposeB,
                        std::vector<float>& C) {
    if (M <= 0 || N <= 0 || K <= 0) return false;
    if (A.size() < static_cast<size_t>(M) * K) return false;
    size_t neededB = transposeB ? static_cast<size_t>(N) * K
                                : static_cast<size_t>(K) * N;
    if (B.size() < neededB) return false;
    if (C.size() < static_cast<size_t>(M) * N) return false;

    const int blockSize = 32; // fixed block dimension

    // Initialize C to zero (since we accumulate with beta=0).
    std::fill(C.begin(), C.begin() + M * N, 0.0f);

    // Iterate over output blocks of C.
    for (int rowBlock = 0; rowBlock < M; rowBlock += blockSize) {
        int rowEnd = std::min(rowBlock + blockSize, M);
        int blockH = rowEnd - rowBlock;

        for (int colBlock = 0; colBlock < N; colBlock += blockSize) {
            int colEnd = std::min(colBlock + blockSize, N);
            int blockW = colEnd - colBlock;

            // Iterate over K dimension in chunks.
            for (int kBlock = 0; kBlock < K; kBlock += blockSize) {
                int kEnd = std::min(kBlock + blockSize, K);
                int blockK = kEnd - kBlock;

                // Compute partial product for this block.
                // Use double for accumulation to improve numerical stability.
                for (int i = 0; i < blockH; ++i) {
                    int row = rowBlock + i;
                    for (int j = 0; j < blockW; ++j) {
                        int col = colBlock + j;
                        double sum = 0.0;
                        for (int k = 0; k < blockK; ++k) {
                            int kIdx = kBlock + k;
                            float aVal = A[row * K + kIdx];
                            float bVal;
                            if (transposeB) {
                                // B is N x K, so B[col][kIdx]
                                bVal = B[col * K + kIdx];
                            } else {
                                // B is K x N, so B[kIdx][col]
                                bVal = B[kIdx * N + col];
                            }
                            sum += static_cast<double>(aVal) * static_cast<double>(bVal);
                        }
                        C[row * N + col] += static_cast<float>(sum);
                    }
                }
            }
        }
    }
    return true;
}

// The solution replicates the tiled GEMM approach seen in the original code, which divides the output matrix C into disjoint blocks of size `blockSize × blockSize` (e.g., 32×32) and processes each block independently. For each output block, the algorithm iterates over the K dimension in chunks (also of size `blockSize`), loading sub-blocks of A (size `blockSize × blockChunkK`) and B (either `blockChunkK × blockSize` if no transpose, or `blockSize × blockChunkK` if transposed) into local storage on a CPU (simulated by nested loops). For each such chunk, a triple-nested loop computes the partial product accumulation using double precision to reduce floating-point error. Since the blocking is purely logical (no actual cache optimization in this standalone exercise), the main benefit is clarity of the tiling pattern and proper handling of edges. Important edge cases include: (a) when `M`, `N`, or `K` are not multiples of the block size, the last block in each dimension is smaller; (b) when `transposeB` is true, the input B is stored as an `N × K` matrix (row-major) and must be interpreted with swapped indices; (c) the function must validate that the input vector sizes are at least `M*K` for A and either `K*N` (no transpose) or `N*K` (transpose) for B, and `M*N` for C. The time complexity is \(O(M \cdot N \cdot K)\) for both cases, as every element of A and B is used in the multiplication. Space complexity is \(O(1)\) extra beyond input/output.
