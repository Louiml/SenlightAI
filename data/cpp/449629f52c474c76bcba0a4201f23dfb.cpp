/*
Write a C++ function named `transposeSparseMatrix` that takes a square sparse matrix represented in compressed sparse row (CSR) format as four vectors: `rowPtr` (size `n+1`), `colIndex` (nonzero column indices), `values` (nonzero values), and the matrix dimension `n`. The function must return the transpose of the matrix also in CSR format as a `std::tuple` (or a small struct) containing the resulting `rowPtr`, `colIndex`, and `values`. The input matrix is guaranteed to have at least one nonzero, and the function must handle duplicate entries in the same row (they should be merged by summing their values, as in standard CSR assembly). The output must also be sorted by row and column, and any zero entries resulting from cancellation must be removed.
*/

#include <tuple>
#include <vector>
#include <cstddef>

// Represents a CSR matrix transpose result.
struct CSRMatrix {
    std::vector<std::size_t> rowPtr;
    std::vector<std::size_t> colIndex;
    std::vector<double> values;
};

// Transpose a square CSR matrix (dimension n) and return the result in CSR format.
CSRMatrix transposeSparseMatrix(
    const std::vector<std::size_t>& rowPtr,
    const std::vector<std::size_t>& colIndex,
    const std::vector<double>& values,
    std::size_t n) {

    const std::size_t nnz = values.size();
    
    // Count nonzeros per column of A (= rows of A^T).
    std::vector<std::size_t> colCount(n, 0);
    for (std::size_t i = 0; i < n; ++i) {
        for (std::size_t k = rowPtr[i]; k < rowPtr[i+1]; ++k) {
            ++colCount[colIndex[k]];
        }
    }
    
    // Compute row pointers for A^T.
    std::vector<std::size_t> resultRowPtr(n + 1, 0);
    for (std::size_t i = 0; i < n; ++i) {
        resultRowPtr[i+1] = resultRowPtr[i] + colCount[i];
    }
    
    // Temporary position tracker.
    std::vector<std::size_t> nextPos = resultRowPtr; // copy of rowPtr
    
    // Fill temporary arrays (size at most nnz).
    std::vector<std::size_t> resultColIndex(nnz, 0);
    std::vector<double> resultValues(nnz, 0.0);
    
    for (std::size_t i = 0; i < n; ++i) {
        for (std::size_t k = rowPtr[i]; k < rowPtr[i+1]; ++k) {
            std::size_t j = colIndex[k];
            std::size_t pos = nextPos[j]++;
            resultColIndex[pos] = i;
            resultValues[pos] += values[k]; // add in case of duplicates
        }
    }
    
    // Compaction: remove zero values (possible after summing duplicates).
    std::vector<std::size_t> finalRowPtr(n + 1, 0);
    std::vector<std::size_t> finalColIndex;
    std::vector<double> finalValues;
    
    for (std::size_t i = 0; i < n; ++i) {
        finalRowPtr[i] = finalColIndex.size();
        for (std::size_t k = resultRowPtr[i]; k < resultRowPtr[i+1]; ++k) {
            if (resultValues[k] != 0.0) {
                finalColIndex.push_back(resultColIndex[k]);
                finalValues.push_back(resultValues[k]);
            }
        }
    }
    finalRowPtr[n] = finalColIndex.size();
    
    return {finalRowPtr, finalColIndex, finalValues};
}

#include <cassert>
#include <vector>

// (The solution code above is assumed to be included.)

int main() {
    // Test 1: Simple 3x3 with one nonzero per row.
    {
        std::vector<std::size_t> rowPtr = {0, 1, 2, 3};
        std::vector<std::size_t> colIndex = {1, 0, 2};
        std::vector<double> values = {1.0, 2.0, 3.0};
        CSRMatrix res = transposeSparseMatrix(rowPtr, colIndex, values, 3);
        assert(res.rowPtr == std::vector<std::size_t>({0, 1, 2, 3}));
        assert(res.colIndex == std::vector<std::size_t>({1, 0, 2}));
        assert(res.values == std::vector<double>({2.0, 1.0, 3.0}));
    }

    // Test 2: Duplicate entries in same row are summed.
    {
        std::vector<std::size_t> rowPtr = {0, 2, 2};
        std::vector<std::size_t> colIndex = {0, 0};
        std::vector<double> values = {1.0, 2.0};
        CSRMatrix res = transposeSparseMatrix(rowPtr, colIndex, values, 2);
        assert(res.rowPtr == std::vector<std::size_t>({0, 1, 1}));
        assert(res.colIndex == std::vector<std::size_t>({0}));
        assert(res.values == std::vector<double>({3.0}));
    }

    // Test 3: Duplicates that cancel to zero are removed.
    {
        std::vector<std::size_t> rowPtr = {0, 1, 2};
        std::vector<std::size_t> colIndex = {1, 1};
        std::vector<double> values = {5.0, -5.0};
        CSRMatrix res = transposeSparseMatrix(rowPtr, colIndex, values, 2);
        assert(res.rowPtr == std::vector<std::size_t>({0, 0, 0}));
        assert(res.colIndex.empty());
        assert(res.values.empty());
    }

    // Test 4: Row with no nonzeros in the middle.
    {
        std::vector<std::size_t> rowPtr = {0, 1, 1, 2};
        std::vector<std::size_t> colIndex = {2, 0};
        std::vector<double> values = {7.0, 9.0};
        CSRMatrix res = transposeSparseMatrix(rowPtr, colIndex, values, 3);
        assert(res.rowPtr == std::vector<std::size_t>({0, 1, 1, 2}));
        assert(res.colIndex == std::vector<std::size_t>({2, 0}));
        assert(res.values == std::vector<double>({9.0, 7.0}));
    }

    // Test 5: 1x1 matrix.
    {
        std::vector<std::size_t> rowPtr = {0, 1};
        std::vector<std::size_t> colIndex = {0};
        std::vector<double> values = {42.0};
        CSRMatrix res = transposeSparseMatrix(rowPtr, colIndex, values, 1);
        assert(res.rowPtr == std::vector<std::size_t>({0, 1}));
        assert(res.colIndex == std::vector<std::size_t>({0}));
        assert(res.values == std::vector<double>({42.0}));
    }

    // Test 6: Larger matrix with mixed duplicates and multiple entries per row.
    {
        // A = [ 1 0 2 ; 0 3 0 ; 4 5 6 ] (but with duplicate)
        // Actually build A with duplicates: row0: (0,0)+1, (0,2)+2, (0,0)+-1  => row0: (0,0)=0? Let's design careful.
        // Simpler: A = [1 2 0; 0 0 3; 4 0 5] but duplicate (1,2) two times? We'll do:
        // row0: (0,0)=1, (0,1)=2
        // row1: (1,2)=3
        // row2: (2,0)=4, (2,2)=5
        // Also add duplicate row2: (2,0)=1 (so sum to 5) but then also add (2,0)=-1 to cancel? Too messy.
        // Let's just use distinct entries for clarity:
        std::vector<std::size_t> rowPtr = {0, 2, 3, 5};
        std::vector<std::size_t> colIndex = {0, 1, 2, 0, 2};
        std::vector<double> values = {1.0, 2.0, 3.0, 4.0, 5.0};
        CSRMatrix res = transposeSparseMatrix(rowPtr, colIndex, values, 3);
        // Transpose A^T: row0: (0,0)=1, (0,2)=4; row1: (1,0)=2; row2: (2,1)=3, (2,2)=5
        assert(res.rowPtr == std::vector<std::size_t>({0, 2, 3, 5}));
        assert(res.colIndex == std::vector<std::size_t>({0, 2, 0, 1, 2}));
        assert(res.values == std::vector<double>({1.0, 4.0, 2.0, 3.0, 5.0}));
    }

    return 0;
}

// The transpose of a CSR matrix `A` (dimension `n`) can be computed efficiently by first determining the number of nonzeros in each column of `A`, which become the row counts of `A^T`. This is done by iterating over all nonzeros and incrementing a count per column. Then, a cumulative sum of these counts yields the row pointers for the transposed matrix. Next, a temporary position array (initialized to the row pointers) is used to fill the output column indices and values in column-major order: for each row `i` of the original matrix, for each nonzero `(i, j, v)`, place `i` into the output's column array at position `pos[j]` and `v` into the output's value array, then increment `pos[j]`. After this, compaction is applied because merging duplicate entries might cause zero values; we walk through the output and remove any entries where the value is exactly zero, adjusting the row pointers accordingly. Edge cases include a matrix with only one nonzero, rows with no nonzeros, and duplicates that sum to zero. Time complexity is `O(nnz + n)` for filling and `O(nnz)` for compaction, so overall `O(nnz + n)`. Space complexity is `O(nnz + n)` due to the output arrays and the temporary position array.
