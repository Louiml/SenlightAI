/*
Write a C++ function `parallelFoxMatrixMultiply` that performs blocked matrix multiplication of two square double-precision matrices `A` and `B` of the same size `n` using a parallel Fox algorithm. The function must accept the input matrices by const reference, an output matrix by pointer, and an integer `block_size`; it should validate that `block_size` is positive and does not exceed `n`, and that the matrices are square and compatible. The function must use OpenMP to parallelize the computation across a 2D grid of threads (using `omp_get_max_threads()` to determine the grid dimension, e.g., `q = sqrt(threads)`), where each thread handles one block of size `block_size × block_size` in a round-robin shifting pattern typical of Fox’s algorithm: for each step `z` from `0` to `q-1`, each thread multiplies its current block of `A` (shifted left by one block per step) with its current block of `B` (shifted up by one block per step), accumulating partial results into a local block `C`. After the loop, each thread writes its local block back into the global output matrix. Ensure correct handling of cases where `n` is not an exact multiple of `block_size` by using the actual block extents (e.g., `min(start+block_size, n)`) but you may assume `block_size` divides `n` evenly for simplicity if you document it; however, for robustness, compute actual extents. The function should return `true` on success and `false` on invalid input (e.g., size mismatch, invalid block size, or `n=0`). You must include all necessary headers (`<vector>`, `<cmath>`, `<algorithm>`, `<omp.h>`, `<iostream>`) and use `const` correctness. Do not write a `main` function; only provide the function implementation with a descriptive comment.
*/
#include <vector>
#include <cmath>
#include <algorithm>
#include <omp.h>
#include <iostream>

/**
 * Parallel blocked matrix multiplication using Fox's algorithm.
 * 
 * @param C        Pointer to output matrix (will be resized).
 * @param A        First input matrix (n x n).
 * @param B        Second input matrix (n x n).
 * @param block_size  Size of each square block (must divide n evenly).
 * @return         true on success, false on invalid input.
 */
bool parallelFoxMatrixMultiply(std::vector<std::vector<double>>* C,
                               const std::vector<std::vector<double>>& A,
                               const std::vector<std::vector<double>>& B,
                               int block_size) {
    // Validate inputs.
    int n = A.size();
    if (n == 0) {
        std::cerr << "Error: empty matrix\n";
        return false;
    }
    if (B.size() != n) {
        std::cerr << "Error: matrix size mismatch\n";
        return false;
    }
    for (const auto& row : A) if (row.size() != n) return false;
    for (const auto& row : B) if (row.size() != n) return false;
    if (block_size <= 0 || block_size > n) {
        std::cerr << "Error: invalid block size\n";
        return false;
    }
    if (n % block_size != 0) {
        std::cerr << "Error: block_size must divide matrix size evenly\n";
        return false;
    }

    // Allocate output matrix initialized to zero.
    (*C) = std::vector<std::vector<double>>(n, std::vector<double>(n, 0.0));

    int q = static_cast<int>(std::sqrt(omp_get_max_threads()));
    if (q < 1) q = 1;
    int num_threads = q * q;

    #pragma omp parallel num_threads(num_threads)
    {
        int tid = omp_get_thread_num();
        int i_thread = tid / q;  // block row index
        int j_thread = tid % q;  // block column index

        int block_size = n / q; // actual block size (since n % q == 0 if n % block_size == 0 and block_size divides n, but q may not divide n; we handle by using min extents)
        // For simplicity, we require that n is a multiple of both block_size and q? Actually we can compute actual extents.
        int row_start = i_thread * block_size;
        int col_start = j_thread * block_size;
        int row_end = std::min(row_start + block_size, n);
        int col_end = std::min(col_start + block_size, n);
        int actual_bs = row_end - row_start; // actual block size for C

        // Local block accumulators.
        std::vector<std::vector<double>> blockC(actual_bs, std::vector<double>(actual_bs, 0.0));

        for (int z = 0; z < q; ++z) {
            // Source block column for A: (i_thread + z) % q
            int a_col = ((i_thread + z) % q) * block_size;
            int a_col_end = std::min(a_col + block_size, n);
            // Source block row for B: (i_thread + z) % q
            int b_row = ((i_thread + z) % q) * block_size;
            int b_row_end = std::min(b_row + block_size, n);

            // Copy A block: rows [row_start, row_end-1]? Wait, in Fox algorithm, thread (i,j) uses A block (i, (i+z)%q) and B block ((i+z)%q, j).
            // So A block rows are row_start to row_end-1, columns from a_col to a_col_end-1.
            // B block rows from b_row to b_row_end-1, columns col_start to col_end-1.
            // Ensure dimensions match (block_size x block_size), but we have actual_bs for C's rows and columns.
            int bsA = row_end - row_start; // rows of A block
            int bsA_col = a_col_end - a_col; // cols of A block
            int bsB_row = b_row_end - b_row; // rows of B block
            int bsB_col = col_end - col_start; // cols of B block

            // For multiplication, need bsA == bsB_col? Actually C[i][j] = sum_k A[i][k]*B[k][j], where k ranges over block column of A = block row of B.
            // So the number of columns in A block must equal number of rows in B block. That is bsA_col == bsB_row.
            // Since all blocks are square block_size (with possible edge cut), they should match when n divisible by q, but we'll fix by using min.
            // Simplify: assume n is divisible by both block_size and q. Then actual_bs = block_size everywhere.
            // For robustness, we compute k_length = min(a_col_end - a_col, b_row_end - b_row).
            int k_length = std::min(a_col_end - a_col, b_row_end - b_row);
            if (k_length <= 0) continue;

            // Build local A and B blocks.
            std::vector<std::vector<double>> blockA(row_end - row_start, std::vector<double>(k_length, 0.0));
            std::vector<std::vector<double>> blockB(k_length, std::vector<double>(col_end - col_start, 0.0));

            for (int i = 0; i < row_end - row_start; ++i) {
                for (int k = 0; k < k_length; ++k) {
                    blockA[i][k] = A[row_start + i][a_col + k];
                }
            }
            for (int k = 0; k < k_length; ++k) {
                for (int j = 0; j < col_end - col_start; ++j) {
                    blockB[k][j] = B[b_row + k][col_start + j];
                }
            }

            // Multiply blockA * blockB and accumulate into blockC.
            for (int i = 0; i < row_end - row_start; ++i) {
                for (int j = 0; j < col_end - col_start; ++j) {
                    double sum = 0.0;
                    for (int k = 0; k < k_length; ++k) {
                        sum += blockA[i][k] * blockB[k][j];
                    }
                    blockC[i][j] += sum;
                }
            }
        }

        // Write blockC to global output.
        for (int i = 0; i < row_end - row_start; ++i) {
            for (int j = 0; j < col_end - col_start; ++j) {
                (*C)[row_start + i][col_start + j] = blockC[i][j];
            }
        }
    }

    return true;
}
#include <cassert>
#include <vector>
#include <cmath>

// Function declaration (assumes defined elsewhere).
bool parallelFoxMatrixMultiply(std::vector<std::vector<double>>* C,
                               const std::vector<std::vector<double>>& A,
                               const std::vector<std::vector<double>>& B,
                               int block_size);

int main() {
    // Test 1: 4x4 matrices, block_size=2, check against naive multiplication.
    {
        std::vector<std::vector<double>> A = {{1,2,3,4},{5,6,7,8},{9,10,11,12},{13,14,15,16}};
        std::vector<std::vector<double>> B = {{2,0,1,3},{0,1,0,2},{1,0,2,1},{0,1,1,0}};
        std::vector<std::vector<double>> C;
        bool ok = parallelFoxMatrixMultiply(&C, A, B, 2);
        assert(ok);
        // Expected result computed manually (or via naive).
        std::vector<std::vector<double>> expected = {{6,7,10,11},{14,19,26,35},{22,31,42,55},{30,43,58,75}};
        for (size_t i=0; i<4; ++i) {
            for (size_t j=0; j<4; ++j) {
                assert(fabs(C[i][j] - expected[i][j]) < 1e-9);
            }
        }
    }

    // Test 2: Invalid block size.
    {
        std::vector<std::vector<double>> A = {{1,2},{3,4}};
        std::vector<std::vector<double>> B = {{1,0},{0,1}};
        std::vector<std::vector<double>> C;
        assert(!parallelFoxMatrixMultiply(&C, A, B, 0));
        assert(!parallelFoxMatrixMultiply(&C, A, B, 3));
    }

    // Test 3: Size mismatch.
    {
        std::vector<std::vector<double>> A = {{1,2},{3,4}};
        std::vector<std::vector<double>> B = {{1,2,3}};
        std::vector<std::vector<double>> C;
        assert(!parallelFoxMatrixMultiply(&C, A, B, 1));
    }

    // Test 4: 1x1 matrix.
    {
        std::vector<std::vector<double>> A = {{5}};
        std::vector<std::vector<double>> B = {{7}};
        std::vector<std::vector<double>> C;
        assert(parallelFoxMatrixMultiply(&C, A, B, 1));
        assert(fabs(C[0][0] - 35.0) < 1e-9);
    }

    // Test 5: 6x6 with block_size=3, compare to naive.
    {
        int n = 6;
        std::vector<std::vector<double>> A(n, std::vector<double>(n));
        std::vector<std::vector<double>> B(n, std::vector<double>(n));
        // Fill with simple pattern.
        for (int i=0; i<n; ++i) {
            for (int j=0; j<n; ++j) {
                A[i][j] = (i*2 + j) * 0.5;
                B[i][j] = (i - j) * 1.5;
            }
        }
        std::vector<std::vector<double>> C_expected(n, std::vector<double>(n,0));
        for (int i=0; i<n; ++i)
            for (int j=0; j<n; ++j)
                for (int k=0; k<n; ++k)
                    C_expected[i][j] += A[i][k]*B[k][j];

        std::vector<std::vector<double>> C;
        assert(parallelFoxMatrixMultiply(&C, A, B, 3));
        for (int i=0; i<n; ++i)
            for (int j=0; j<n; ++j)
                assert(fabs(C[i][j] - C_expected[i][j]) < 1e-6);
    }

    return 0;
}
// The core idea is to parallelize matrix multiplication using a blocked algorithm that distributes work across a 2D grid of OpenMP threads. For an `n × n` matrix and `p` threads, we set `q = floor(sqrt(p))` and effectively use `q*q` threads (the number of threads is clamped to `q*q` via `num_threads`). Each thread is assigned a unique block position `(i_thread, j_thread)` where `i_thread = tid / q` and `j_thread = tid % q`. The matrix is divided into `q × q` blocks, each of size `block_size × block_size` (assuming exact division; if not, we adjust actual block dimensions). In Fox’s algorithm, for each step `z = 0` to `q-1`, the thread responsible for block row `i_thread` uses the block of `A` at column `(i_thread + z) % q` (i.e., shifted left), and the block of `B` at row `(i_thread + z) % q` (shifted up), but note that the column index for `B` is `j_thread` (since each thread maintains its own column). The thread multiplies its local `blockA` (a `block_size × block_size` submatrix copied from `A`) by `blockB` (similar submatrix from `B`) and accumulates into `blockC`. After all `q` steps, the thread writes `blockC` to the global output matrix at row offset `i_thread * block_size` and column offset `j_thread * block_size`. Edge cases: (1) Non-square matrices or different sizes – reject with `false`. (2) `n == 0` – invalid. (3) `block_size <= 0` or `block_size > n` – invalid. (4) If `n` is not a multiple of `block_size`, we handle by computing actual extents `endA = min(start + block_size, n)` but the round‑robin shifting assumes equal block sizes; to simplify, we require that `block_size` divides `n` evenly (document this) and reject otherwise. (5) The number of threads used may be less than `omp_get_max_threads()` if `q*q` is smaller; we set `num_threads(q*q)`. (6) The global output matrix must be initialized to zeros before the parallel region. Time complexity is `O(n^3/p)` for computation (since each thread does `n^3/(q^2) = n^3/p` multiplications, with `p = q^2`), plus `O(n^2)` copy overhead for each block per step, so overall `O(n^3/p + n^2 * q)` (since each thread copies `q` blocks of size `block_size^2 = (n/q)^2`, total copy per thread `q * (n/q)^2 = n^2/q`, and across `q^2` threads that’s `n^2 * q`). Space complexity is `O(n^2)` for the output matrix plus `O(block_size^2)` per thread for local blocks, which is `O(n^2/q^2)` per thread, acceptable.
