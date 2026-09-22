Given two square sparse matrices `A` and `B` of equal size `n` (with `n` up to 10,000) stored in compressed sparse row (CSR) format, write a C++ function `sparseMultiply` that computes their product `C = A * B` and returns it in CSR format. The input matrices are provided as three vectors each: `values` (non-zero entries in row-major order), `rowPtr` (length `n+1`, where `rowPtr[i]` is the start index of row `i` in `values` and `colIndices`), and `colIndices` (column indices for each non-zero). The function must handle matrices with up to 6 non-zero entries per row and must correctly compute the product even when intermediate sums are zero (i.e., drop zeros in the result). The output must be a valid CSR representation with sorted column indices within each row. Assume square matrices, but the algorithm should generalize if stated. Your solution must be self-contained with all necessary headers and should use efficient sparse multiplication techniques.
The core algorithm for sparse matrix multiplication in CSR format is based on three phases: (1) symbol calculation, (2) numerical computation, and (3) compaction. We first compute the sparsity pattern of the result `C` without actual values, using a temporary array `next` of size `n` initialized to -1, and a `processed` array to mark which rows of `B` have been merged for the current row of `A`. For each row `i` of `A`, we iterate over its non-zero entries `(k, a_ik)`. For each column `j` that is non-zero in row `k` of `B`, we push `j` into the pattern list for row `i` if it hasn’t been seen yet, and accumulate the contribution `a_ik * b_kj` into a workspace array `work[j]`. After processing all entries of row `i`, we scan through the collected column indices, harvest the values from `work`, reset `work` to zero, and append the non-zero results (dropping any that are exactly zero) into the output CSR vectors. This ensures each row of `C` is built with sorted columns because we iterate through `A` rows in increasing column order and `B` rows in increasing column order, leading to sorted order in the result. The time complexity is `O(nnz(A) * avg_nnz_per_row_of_B + nnz(C) + n)` for the accumulation and compaction, which in the worst case is `O(n * k^2)` where `k` is the average non-zeros per row (here up to 6, so nearly linear). Space complexity is `O(n + nnz(C))` for the workspace and output. Edge cases include zero rows in `A` (producing empty rows in `C`), rows in `B` with no entries (no contribution), and intermediate sums that cancel to zero requiring removal to maintain a valid CSR format.
#include <vector>
#include <algorithm>

/**
 * Multiply two sparse matrices in CSR format.
 * @param A_values    Non-zero values of A, row-major.
 * @param A_rowPtr    Row pointers for A (size n+1).
 * @param A_colIdx    Column indices for A.
 * @param B_values    Non-zero values of B, row-major.
 * @param B_rowPtr    Row pointers for B (size n+1).
 * @param B_colIdx    Column indices for B.
 * @param n           Dimension of square matrices.
 * @return A struct containing the CSR representation of A*B.
 */
struct CSRMatrix {
    std::vector<double> values;
    std::vector<int> colIndices;
    std::vector<int> rowPtr;
};

CSRMatrix sparseMultiply(const std::vector<double>& A_values,
                         const std::vector<int>& A_rowPtr,
                         const std::vector<int>& A_colIdx,
                         const std::vector<double>& B_values,
                         const std::vector<int>& B_rowPtr,
                         const std::vector<int>& B_colIdx,
                         int n) {
    // Workspace for accumulating products for a single row.
    std::vector<double> work(n, 0.0);
    // Tracks which columns already have been added to the current row's pattern.
    std::vector<int> next(n, -1);
    // Linked list of columns for the current row.
    std::vector<int> patternList;

    CSRMatrix C;
    C.rowPtr.reserve(n + 1);
    C.rowPtr.push_back(0);

    for (int i = 0; i < n; ++i) {
        patternList.clear();
        // Process all non-zero entries in row i of A.
        for (int idx = A_rowPtr[i]; idx < A_rowPtr[i + 1]; ++idx) {
            int k = A_colIdx[idx];
            double a_ik = A_values[idx];
            // Iterate over non-zero entries in row k of B.
            for (int jdx = B_rowPtr[k]; jdx < B_rowPtr[k + 1]; ++jdx) {
                int j = B_colIdx[jdx];
                // If column j is not yet in the current pattern.
                if (next[j] == -1) {
                    next[j] = 1;  // Mark as seen (could use a boolean array, but this is fine)
                    patternList.push_back(j);
                }
                work[j] += a_ik * B_values[jdx];
            }
        }
        // Harvest results for row i.
        std::sort(patternList.begin(), patternList.end()); // Ensure sorted columns
        for (int j : patternList) {
            double value = work[j];
            if (value != 0.0) {
                C.values.push_back(value);
                C.colIndices.push_back(j);
            }
            // Reset for next row.
            work[j] = 0.0;
            next[j] = -1;
        }
        C.rowPtr.push_back(static_cast<int>(C.values.size()));
    }
    return C;
}
#include <cassert>
#include <vector>
#include <cmath>

int main() {
    // Test 1: Simple 2x2 matrices
    // A = [1 2; 3 4], B = [5 0; 0 6] => C = [5 12; 15 24]
    {
        std::vector<double> A_vals = {1,2,3,4};
        std::vector<int> A_row = {0,2,4};
        std::vector<int> A_col = {0,1,0,1};
        std::vector<double> B_vals = {5,6};
        std::vector<int> B_row = {0,1,2};
        std::vector<int> B_col = {0,1};
        auto C = sparseMultiply(A_vals, A_row, A_col, B_vals, B_row, B_col, 2);
        assert(C.values == std::vector<double>({5,12,15,24}));
        assert(C.colIndices == std::vector<int>({0,1,0,1}));
        assert(C.rowPtr == std::vector<int>({0,2,4}));
    }

    // Test 2: Matrices with zero row in A producing zero row in C
    // A = [0 0; 1 0], B = [2 0; 0 3] => C = [0 0; 2 0]
    {
        std::vector<double> A_vals = {1};
        std::vector<int> A_row = {0,0,1};
        std::vector<int> A_col = {0};
        std::vector<double> B_vals = {2,3};
        std::vector<int> B_row = {0,1,2};
        std::vector<int> B_col = {0,1};
        auto C = sparseMultiply(A_vals, A_row, A_col, B_vals, B_row, B_col, 2);
        assert(C.values == std::vector<double>({2}));
        assert(C.colIndices == std::vector<int>({0}));
        assert(C.rowPtr == std::vector<int>({0,0,1}));
    }

    // Test 3: Identity times identity yields identity (3x3)
    {
        std::vector<double> A_vals = {1,1,1};
        std::vector<int> A_row = {0,1,2,3};
        std::vector<int> A_col = {0,1,2};
        auto I = A_vals;
        auto Ir = A_row;
        auto Ic = A_col;
        auto C = sparseMultiply(I, Ir, Ic, I, Ir, Ic, 3);
        assert(C.values == I);
        assert(C.colIndices == Ic);
        assert(C.rowPtr == Ir);
    }

    // Test 4: Sparse matrices with cancellation (e.g., (1 -1) times (1; 1) gives 0)
    // A = [1 -1], B = [1; 1] => C = [0]
    {
        std::vector<double> A_vals = {1, -1};
        std::vector<int> A_row = {0,2};
        std::vector<int> A_col = {0,1};
        std::vector<double> B_vals = {1, 1};
        std::vector<int> B_row = {0,1,2};
        std::vector<int> B_col = {0,0};
        auto C = sparseMultiply(A_vals, A_row, A_col, B_vals, B_row, B_col, 2);
        assert(C.values.empty());
        assert(C.colIndices.empty());
        assert(C.rowPtr == std::vector<int>({0,0}));
    }

    // Test 5: Larger 4x4 with sparse data
    // A has ones at (0,0),(0,2),(1,1),(2,2),(3,0)
    // B has ones at (0,1),(1,2),(2,3),(3,0)
    // C[i][j] = sum over k A[i][k]*B[k][j]
    {
        std::vector<double> A_vals = {1,1,1,1,1};
        std::vector<int> A_row = {0,2,3,4,5};
        std::vector<int> A_col = {0,2,1,2,0};
        std::vector<double> B_vals = {1,1,1,1};
        std::vector<int> B_row = {0,1,2,3,4};
        std::vector<int> B_col = {1,2,3,0};
        auto C = sparseMultiply(A_vals, A_row, A_col, B_vals, B_row, B_col, 4);
        // Expected: 
        // Row0: A[0][0]*B[0][1]=1, A[0][2]*B[2][3]=1 => cols 1,3
        // Row1: A[1][1]*B[1][2]=1 => col 2
        // Row2: A[2][2]*B[2][3]=1 => col 3
        // Row3: A[3][0]*B[0][1]=1 => col 1
        assert(C.values == std::vector<double>({1,1,1,1,1}));
        assert(C.colIndices == std::vector<int>({1,3,2,3,1}));
        assert(C.rowPtr == std::vector<int>({0,2,3,4,5}));
    }

    return 0;
}
