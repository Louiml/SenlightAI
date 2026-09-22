Write a standalone C++ function named `sparseMatrixVectorProduct` that takes a sparse matrix represented in compressed sparse row (CSR) format (using `std::vector<double>` for values, `std::vector<int>` for column indices, and `std::vector<int>` for row pointers) and a dense vector as a `std::vector<double>`. The function must return the result of the matrix-vector product (i.e., `result[i] = sum_j matrix[i][j] * vector[j]`) as a `std::vector<double>`. The input CSR matrix is assumed to be valid (size-consistent, row pointers sorted, column indices in each row sorted ascending, no duplicate entries in a row). Handle the case of an empty matrix (zero rows or zero columns) gracefully, returning an empty vector or a zero-vector of appropriate size. Also handle the case where the input vector is empty when there are zero columns. The function should be `const`-correct and not modify the inputs.

// The solution uses the standard CSR matrix-vector multiplication algorithm. For each row `i` (from 0 to number-of-rows-1), iterate over the non-zero entries in that row using the row pointer range `[rowPtr[i], rowPtr[i+1])`. For each entry at column `j` with value `val`, add `val * vector[j]` to `result[i]`. Key edge cases: (1) If the matrix has zero rows or zero columns, we must still return a vector of size equal to the number of rows (which may be zero or positive). (2) If there are zero columns, the input vector might be empty, so we should not index into it; the loop over columns uses the column index from the matrix, and if the matrix has no entries (since columns are zero), the row pointers must all be zero, so the inner loop never executes—this is safe even if the vector is empty. (3) We initialize result to size `numRows` with all zeros to avoid out-of-bounds if a row has no non-zeros. Time complexity is `O(nnz + numRows)` where `nnz` is the number of non-zero entries, because we visit each non-zero once and each row once. Space complexity is `O(numRows)` for the result vector, plus constant extra space.

#include <vector>
#include <cstddef>

// Computes the product of a sparse matrix in CSR format and a dense vector.
// matrix is defined by values (non-zero values), colIndices (column index per value),
// and rowPtr (size numRows+1, where rowPtr[i] and rowPtr[i+1] bound the i-th row's entries).
// The input vector must have at least as many elements as the number of columns.
// Returns a vector of size numRows containing the product.
std::vector<double> sparseMatrixVectorProduct(
    const std::vector<double>& values,
    const std::vector<int>& colIndices,
    const std::vector<int>& rowPtr,
    const std::vector<double>& vector) 
{
    // Number of rows = rowPtr.size() - 1 (assuming rowPtr is non-empty and consistent)
    if (rowPtr.empty()) {
        return {};
    }
    const std::size_t numRows = rowPtr.size() - 1;
    std::vector<double> result(numRows, 0.0);

    for (std::size_t i = 0; i < numRows; ++i) {
        const int rowStart = rowPtr[i];
        const int rowEnd = rowPtr[i + 1];
        double sum = 0.0;
        for (int k = rowStart; k < rowEnd; ++k) {
            const int col = colIndices[k];
            sum += values[k] * vector[col];
        }
        result[i] = sum;
    }
    return result;
}

#include <cassert>
#include <vector>
#include <cmath>

int main() {
    // Test 1: Simple 2x3 matrix
    // [1 0 2]
    // [0 3 4]
    std::vector<double> values   = {1.0, 2.0, 3.0, 4.0};
    std::vector<int>    colIdx   = {0, 2, 1, 2};
    std::vector<int>    rowPtr   = {0, 2, 4};
    std::vector<double> vec      = {1.0, 1.0, 1.0};
    std::vector<double> result   = sparseMatrixVectorProduct(values, colIdx, rowPtr, vec);
    assert(result.size() == 2);
    assert(std::fabs(result[0] - 3.0) < 1e-12); // 1*1 + 2*1 = 3
    assert(std::fabs(result[1] - 7.0) < 1e-12); // 3*1 + 4*1 = 7

    // Test 2: Empty matrix (0 rows, 0 columns)
    std::vector<double> emptyVals;
    std::vector<int>    emptyCols;
    std::vector<int>    emptyRows = {0};
    std::vector<double> emptyVec;
    result = sparseMatrixVectorProduct(emptyVals, emptyCols, emptyRows, emptyVec);
    assert(result.empty());

    // Test 3: Matrix with zero rows but nonzero columns (should return empty)
    std::vector<int> rows3 = {0,0,0}; // two rows? Actually rowPtr for 0 rows should be {0}? Let's use 0 rows: rowPtr.size()-1=0 -> rowPtr={0}
    // But here we test with rowPtr={0,0,0}? That means 2 rows with no nonzeros. Use that scenario.
    std::vector<int> rowsNonZero = {0,0,0}; // 2 rows, each with no non-zeros
    result = sparseMatrixVectorProduct({}, {}, rowsNonZero, {1.0, 2.0});
    assert(result.size() == 2);
    assert(result[0] == 0.0 && result[1] == 0.0);

    // Test 4: Diagonal matrix 3x3
    std::vector<double> diagVals = {2.0, 5.0, -1.0};
    std::vector<int>    diagCols = {0, 1, 2};
    std::vector<int>    diagRows = {0,1,2,3};
    std::vector<double> diagVec  = {1.0, 2.0, 3.0};
    result = sparseMatrixVectorProduct(diagVals, diagCols, diagRows, diagVec);
    assert(result.size() == 3);
    assert(result[0] == 2.0); // 2*1
    assert(result[1] == 10.0); // 5*2
    assert(result[2] == -3.0); // -1*3

    // Test 5: Matrix with a row having no non-zeros
    // [1 0 0]
    // [0 0 0]
    // [0 0 4]
    std::vector<double> vals5 = {1.0, 4.0};
    std::vector<int>    cols5 = {0, 2};
    std::vector<int>    rows5 = {0,1,1,2};
    std::vector<double> vec5  = {5.0, 6.0, 7.0};
    result = sparseMatrixVectorProduct(vals5, cols5, rows5, vec5);
    assert(result.size() == 3);
    assert(result[0] == 5.0); // 1*5
    assert(result[1] == 0.0); // no non-zeros
    assert(result[2] == 28.0); // 4*7

    // Test 6: Consistency with dense product (random small)
    // Use a 4x3 matrix
    std::vector<double> vals6 = {1.0,2.0,3.0,4.0,5.0,6.0,7.0};
    std::vector<int>    cols6 = {0,1,2,0,2,1,2};
    std::vector<int>    rows6 = {0,3,5,6,7};
    std::vector<double> vec6  = {2.0, -1.0, 0.5};
    // Expected:
    // row0: 1*2 +2*(-1)+3*0.5 = 2 -2 +1.5 =1.5
    // row1: 4*2 +5*0.5 =8+2.5=10.5
    // row2: 6*(-1)+7*0.5 = -6+3.5=-2.5
    // row3: 0
    result = sparseMatrixVectorProduct(vals6, cols6, rows6, vec6);
    assert(std::fabs(result[0] - 1.5) < 1e-12);
    assert(std::fabs(result[1] - 10.5) < 1e-12);
    assert(std::fabs(result[2] - (-2.5)) < 1e-12);
    assert(result[3] == 0.0);

    return 0;
}
