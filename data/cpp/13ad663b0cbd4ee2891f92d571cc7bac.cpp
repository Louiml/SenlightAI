/*
Write a standalone C++ function that, given a square matrix represented in compressed sparse row (CSR) format using four vectors—row offsets, column indices, values, and the number of rows—performs a sparse matrix-vector multiplication (SpMV) with a dense input vector and returns a dense output vector. The function must handle the general case where rows may have zero nonzeros, and the input and output vectors are represented as `std::vector<double>`. The function must be `const` correct, use only the C++ standard library (no external libraries), and must not modify the input matrix or vector. The operation computes `y = A * x`, where `A` is the matrix, `x` is the input vector, and `y` is the resulting output vector. Assume the matrix is square (row count equals column count) and the input vector size matches the column count. The function should return the output vector by value.
*/
#include <vector>

// Perform y = A * x where A is a square CSR matrix.
// Matrix is defined by:
//   rowOffsets: length n+1, rowOffsets[i] is start of row i, rowOffsets[i+1] is end.
//   colIndices: length nnz, column index for each nonzero.
//   values: length nnz, value for each nonzero.
//   n: number of rows and columns (assumed square).
// x: input vector of size n.
// Returns: output vector y of size n.
std::vector<double> sparseMatVec(
    const std::vector<size_t>& rowOffsets,
    const std::vector<size_t>& colIndices,
    const std::vector<double>& values,
    size_t n,
    const std::vector<double>& x
) {
    // Preallocate output vector with zeros.
    std::vector<double> y(n, 0.0);
    
    // Iterate over each row.
    for (size_t i = 0; i < n; ++i) {
        double sum = 0.0;
        // Iterate over nonzeros in the row.
        for (size_t k = rowOffsets[i]; k < rowOffsets[i + 1]; ++k) {
            size_t col = colIndices[k];
            sum += values[k] * x[col];
        }
        y[i] = sum;
    }
    
    return y;
}
#include <cassert>
#include <cmath>
#include <vector>

// The solution function is assumed to be included above or defined earlier.
// Test cases for sparseMatVec.

int main() {
    // Test 1: Simple 2x2 diagonal matrix.
    // A = [2 0; 0 3], x = [1, 4], expected y = [2, 12]
    {
        std::vector<size_t> offsets = {0, 1, 2};
        std::vector<size_t> cols = {0, 1};
        std::vector<double> vals = {2.0, 3.0};
        std::vector<double> x = {1.0, 4.0};
        auto y = sparseMatVec(offsets, cols, vals, 2, x);
        assert(y.size() == 2);
        assert(std::fabs(y[0] - 2.0) < 1e-12);
        assert(std::fabs(y[1] - 12.0) < 1e-12);
    }
    
    // Test 2: 3x3 with a zero row.
    // A = [1 2 0; 0 0 0; 3 0 4], x = [2, -1, 5], expected y = [0, 0, 26]
    {
        std::vector<size_t> offsets = {0, 2, 2, 4};
        std::vector<size_t> cols = {0, 1, 0, 2};
        std::vector<double> vals = {1.0, 2.0, 3.0, 4.0};
        std::vector<double> x = {2.0, -1.0, 5.0};
        auto y = sparseMatVec(offsets, cols, vals, 3, x);
        assert(y.size() == 3);
        assert(std::fabs(y[0] - (2.0 - 2.0)) < 1e-12); // 2*2 + 2*(-1) = 2 -2 = 0
        assert(std::fabs(y[1]) < 1e-12); // zero row
        assert(std::fabs(y[2] - (6.0 + 20.0)) < 1e-12); // 3*2 + 4*5 = 6+20=26
    }
    
    // Test 3: 1x1 matrix.
    // A = [5], x = [7], expected y = [35]
    {
        std::vector<size_t> offsets = {0, 1};
        std::vector<size_t> cols = {0};
        std::vector<double> vals = {5.0};
        std::vector<double> x = {7.0};
        auto y = sparseMatVec(offsets, cols, vals, 1, x);
        assert(y.size() == 1);
        assert(std::fabs(y[0] - 35.0) < 1e-12);
    }
    
    // Test 4: Empty matrix (zero rows and columns). Edge case.
    {
        std::vector<size_t> offsets = {0};
        std::vector<size_t> cols;
        std::vector<double> vals;
        std::vector<double> x;
        auto y = sparseMatVec(offsets, cols, vals, 0, x);
        assert(y.empty());
    }
    
    // Test 5: Sparse matrix with non-contiguous columns and negative values.
    // A = [0 -1; 2 0], x = [3, -4], expected y = [4, 6]
    {
        std::vector<size_t> offsets = {0, 1, 2};
        std::vector<size_t> cols = {1, 0};
        std::vector<double> vals = {-1.0, 2.0};
        std::vector<double> x = {3.0, -4.0};
        auto y = sparseMatVec(offsets, cols, vals, 2, x);
        assert(y.size() == 2);
        assert(std::fabs(y[0] - (-1.0 * -4.0)) < 1e-12); // 4
        assert(std::fabs(y[1] - (2.0 * 3.0)) < 1e-12); // 6
    }
    
    return 0;
}
// The solution iterates over each row of the matrix using the row offset array. For each row, it iterates through the nonzeros of that row—indexed from `rowOffsets[i]` to `rowOffsets[i+1]`—and accumulates the product of the matrix value and the corresponding input vector element into a running sum. After processing all nonzeros in a row, the sum is assigned to the output vector at that row index. The algorithm has a time complexity of O(nnz) where nnz is the total number of nonzeros, and a space complexity of O(n) for the output vector (excluding the input storage). Edge cases include rows with zero nonzeros (where the output element remains zero because the row contributes nothing) and input vectors with values at columns that have no corresponding matrix nonzero (which simply do not contribute). The implementation must properly handle the case where `rowOffsets` has length `n+1`, with the last offset being the total number of nonzeros. No special handling is needed for duplicate column indices within the same row (if the data is valid CSR, they should be summed, but the function will naturally sum them in the loop). The function should use `size_t` for indices to avoid signedness issues.
