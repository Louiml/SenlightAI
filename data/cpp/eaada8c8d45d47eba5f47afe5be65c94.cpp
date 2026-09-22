// Write a C++ function named `mergeSparseMatrices` that takes two sparse matrices stored in compressed sparse row (CSR) format with zero-based indexing, along with two scalar coefficients, and returns a new CSR matrix representing the linear combination `C = alpha * A + beta * B`. The function must handle matrices of potentially different sizes, and if the dimensions do not match, it should return an empty matrix (all vectors empty, nnz = 0). Each input matrix is represented by four vectors: `rowPtr` (size `numRows + 1`), `colIndex` (size `nnz`), `values` (size `nnz`), and the number of rows `numRows` and columns `numCols`. The function should assume that within each matrix the column indices per row are sorted in ascending order, but it must correctly merge rows even if the same column index appears in both A and B (summing values) and may produce unsorted output if needed (though sorted output is preferred). The output matrix should use the same zero-based CSR representation. The function must be self-contained (no external sparse libraries) and no dynamic exceptions—use standard vectors only. Time complexity should be O(nnzA + nnzB + numRows) in the worst case, with auxiliary space O(numRows + nnzC).

The solution merges two CSR matrices by iterating row by row, keeping two indices (one for A's current row's entries, one for B's current row's entries). For each row, we repeatedly compare the current column indices from both matrices. If they are equal, we add the values and advance both pointers; if one is less, we copy that entry (scaled by its coefficient) and advance that pointer. After one matrix's row entries are exhausted, we copy the remaining entries from the other. This is essentially the merge step of a merge sort applied to each row. Edge cases: (1) matrices have different numbers of rows or columns—return empty matrix immediately; (2) a row may have no entries in either matrix (empty row in output); (3) if alpha or beta is zero, entries from the corresponding matrix should still be included but with zero value—we can just include them (or skip them, but including them with zero values is simpler and consistent). We must preserve sorted order in each output row by construction. The output's `rowPtr` is built incrementally. The total number of output nonzeros is at most nnzA + nnzB, so `push_back` operations amortize to O(1) each. The algorithm iterates exactly once through each row's entries across both matrices, so total time is O(nnzA + nnzB + numRows). Space is O(numRows + 1) for rowPtr plus the two output vectors, which are O(nnzC).

#include <vector>
#include <cstddef>

// Structure to represent a sparse matrix in CSR format with zero-based indexing.
struct CSRMatrix {
    std::size_t numRows;
    std::size_t numCols;
    std::vector<std::size_t> rowPtr;      // size numRows + 1
    std::vector<std::size_t> colIndex;    // size nnz
    std::vector<double> values;           // size nnz

    CSRMatrix(std::size_t rows = 0, std::size_t cols = 0)
        : numRows(rows), numCols(cols), rowPtr(rows + 1, 0) {}
};

// Compute C = alpha * A + beta * B, where A and B are CSR matrices with sorted columns per row.
// Returns an empty matrix (numRows=0) if dimensions do not match.
CSRMatrix mergeSparseMatrices(
    double alpha, const CSRMatrix& A,
    double beta,  const CSRMatrix& B)
{
    // Dimension mismatch: return empty matrix.
    if (A.numRows != B.numRows || A.numCols != B.numCols) {
        return CSRMatrix(0, 0);
    }

    CSRMatrix C(A.numRows, A.numCols);
    C.rowPtr.resize(A.numRows + 1, 0);

    std::size_t nnzA = A.colIndex.size();
    std::size_t nnzB = B.colIndex.size();
    std::size_t ptrA = 0;  // current index into A's colIndex/values
    std::size_t ptrB = 0;  // current index into B's colIndex/values

    // Pre-allocate capacity to avoid repeated reallocations.
    // At most nnzA + nnzB entries.
    C.colIndex.reserve(nnzA + nnzB);
    C.values.reserve(nnzA + nnzB);

    for (std::size_t row = 0; row < A.numRows; ++row) {
        std::size_t startA = A.rowPtr[row];
        std::size_t endA   = A.rowPtr[row + 1];
        std::size_t startB = B.rowPtr[row];
        std::size_t endB   = B.rowPtr[row + 1];

        // Merge the two sorted lists of column indices for this row.
        while (ptrA < endA && ptrB < endB) {
            std::size_t colA = A.colIndex[ptrA];
            std::size_t colB = B.colIndex[ptrB];

            if (colA < colB) {
                C.colIndex.push_back(colA);
                C.values.push_back(alpha * A.values[ptrA]);
                ++ptrA;
            } else if (colA > colB) {
                C.colIndex.push_back(colB);
                C.values.push_back(beta * B.values[ptrB]);
                ++ptrB;
            } else {  // same column: sum the contributions
                C.colIndex.push_back(colA);
                C.values.push_back(alpha * A.values[ptrA] + beta * B.values[ptrB]);
                ++ptrA;
                ++ptrB;
            }
        }

        // Copy any remaining entries from A for this row.
        while (ptrA < endA) {
            C.colIndex.push_back(A.colIndex[ptrA]);
            C.values.push_back(alpha * A.values[ptrA]);
            ++ptrA;
        }

        // Copy any remaining entries from B for this row.
        while (ptrB < endB) {
            C.colIndex.push_back(B.colIndex[ptrB]);
            C.values.push_back(beta * B.values[ptrB]);
            ++ptrB;
        }

        // Advance rowPtr for the next row (we know current row is done).
        // The next index equals the current size of C.colIndex.
        C.rowPtr[row + 1] = C.colIndex.size();
    }

    return C;
}

#include <cassert>
#include <vector>

// The CSRMatrix struct and mergeSparseMatrices function are assumed to be defined above.

int main() {
    // Test 1: simple addition with same columns
    CSRMatrix A(3, 3);
    A.rowPtr = {0, 2, 3, 4};
    A.colIndex = {0, 2, 1, 2};
    A.values = {1.0, 2.0, 3.0, 4.0};

    CSRMatrix B(3, 3);
    B.rowPtr = {0, 1, 3, 4};
    B.colIndex = {0, 0, 2, 1};
    B.values = {5.0, 6.0, 7.0, 8.0};

    CSRMatrix C = mergeSparseMatrices(1.0, A, 1.0, B);
    assert(C.numRows == 3);
    assert(C.numCols == 3);
    // Expected C:
    // Row 0: (0, 6.0), (2, 2.0)
    // Row 1: (0, 6.0), (1, 3.0), (2, 7.0)
    // Row 2: (1, 8.0), (2, 4.0)
    assert(C.rowPtr.size() == 4);
    assert(C.rowPtr[0] == 0);
    assert(C.rowPtr[1] == 2);
    assert(C.rowPtr[2] == 5);
    assert(C.rowPtr[3] == 7);
    assert(C.colIndex.size() == 7);
    assert(C.values.size() == 7);

    std::vector<std::size_t> expCols = {0, 2, 0, 1, 2, 1, 2};
    std::vector<double> expVals = {6.0, 2.0, 6.0, 3.0, 7.0, 8.0, 4.0};
    for (std::size_t i = 0; i < 7; ++i) {
        assert(C.colIndex[i] == expCols[i]);
        assert(C.values[i] == expVals[i]);
    }

    // Test 2: zero coefficients
    CSRMatrix D = mergeSparseMatrices(0.0, A, 0.0, B);
    assert(D.numRows == 3);
    assert(D.rowPtr[3] == 7);  // still includes entries with zero values
    assert(D.values[0] == 0.0);

    // Test 3: dimension mismatch produces empty matrix
    CSRMatrix E(2, 2);
    E.rowPtr = {0, 1, 2};
    E.colIndex = {0, 1};
    E.values = {1.0, 2.0};
    CSRMatrix F = mergeSparseMatrices(1.0, A, 1.0, E);
    assert(F.numRows == 0);
    assert(F.rowPtr.empty());
    assert(F.colIndex.empty());
    assert(F.values.empty());

    // Test 4: one matrix empty (no nonzeros)
    CSRMatrix G(2, 2);
    G.rowPtr = {0, 0, 0};  // all rows empty
    G.colIndex = {};
    G.values = {};
    CSRMatrix H(2, 2);
    H.rowPtr = {0, 1, 2};
    H.colIndex = {0, 1};
    H.values = {10.0, 20.0};
    CSRMatrix I = mergeSparseMatrices(2.0, G, 3.0, H);
    assert(I.rowPtr.size() == 3);
    assert(I.rowPtr[0] == 0);
    assert(I.rowPtr[1] == 1);
    assert(I.rowPtr[2] == 2);
    assert(I.colIndex[0] == 0);
    assert(I.values[0] == 30.0);
    assert(I.colIndex[1] == 1);
    assert(I.values[1] == 60.0);

    // Test 5: negative coefficients
    CSRMatrix J = mergeSparseMatrices(-1.0, A, 2.0, B);
    assert(J.values[0] == (-1.0 * 1.0 + 2.0 * 5.0));  // row 0, col 0
    assert(J.values[1] == (-1.0 * 2.0));               // row 0, col 2

    // Test 6: single-row matrices
    CSRMatrix K(1, 4);
    K.rowPtr = {0, 3};
    K.colIndex = {0, 2, 3};
    K.values = {1.0, 2.0, 3.0};
    CSRMatrix L(1, 4);
    L.rowPtr = {0, 2};
    L.colIndex = {1, 3};
    L.values = {4.0, 5.0};
    CSRMatrix M = mergeSparseMatrices(1.0, K, 1.0, L);
    assert(M.rowPtr.size() == 2);
    assert(M.rowPtr[0] == 0);
    assert(M.rowPtr[1] == 4);
    assert(M.colIndex == std::vector<std::size_t>({0, 1, 2, 3}));
    assert(M.values == std::vector<double>({1.0, 4.0, 2.0, 8.0}));

    return 0;
}
