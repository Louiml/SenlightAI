// Write a standalone C++ function that implements one symmetric Gauss-Seidel (SYMGS) sweep for a sparse matrix stored in compressed sparse row (CSR) format. The function must take a `SparseMatrix` structure (with rows, columns, values, and diagonal pointers), a right-hand side vector `r`, and an input/output vector `x`. On entry, `x` may contain arbitrary values, but it will be treated as an initial guess of zero for the forward sweep (i.e., the forward sweep only uses the diagonal and lower-triangular part of `A`). After the forward sweep, the function performs a backward sweep that also includes the diagonal correction. The function must modify `x` in place and return `0` on success. The matrix is assumed square, each row has a diagonal entry (whose address is stored in `matrixDiagonal`), and the row entries are ordered so that lower-triangular elements come before the diagonal, and upper-triangular elements come after the diagonal. The function should apply `const` correctly and be self-contained (include all necessary headers).

The symmetric Gauss-Seidel sweep is split into a forward and a backward pass. In the forward pass, we process rows from the first to the last. For each row `i`, we compute the sum of contributions from the lower-triangular entries (which appear before the diagonal in the row’s column list) multiplied by the already-updated values of `x`. Since the initial guess for `x` is treated as zero, we do not need to subtract contributions from upper-triangular entries during the forward pass. After computing the lower contribution, we update `x[i]` as `(r[i] - lower_sum) / diag[i]`, where `diag[i]` is the value of the diagonal element in that row. In the backward pass, we process rows in reverse order. For each row `i`, we compute the sum of contributions from the diagonal and all off-diagonal entries (both lower and upper triangular) using the current values of `x`. However, to avoid double-counting the diagonal, we first sum the contributions of all off-diagonal entries, then add the diagonal contribution separately, and finally compute the new `x[i]` as `(r[i] - off_diag_sum) / diag[i]`. This is the “include diagonal in the loop, then correct after” approach mentioned in the code snippet. Edge cases include rows where the diagonal pointer might be invalid (should not happen with valid input), and the function must handle matrices with arbitrary sparsity patterns as long as the ordering assumption holds. Time complexity is `O(nnz)` where `nnz` is the number of nonzeros in the matrix, because we visit each nonzero once in the forward pass and once in the backward pass. Space complexity is `O(1)` auxiliary, excluding the input data structures.

#include <vector>
#include <cstddef>

// Forward declaration of matrix structure (simplified for task)
struct SparseMatrix {
    int nrow;                     // number of rows
    std::vector<int> row_ptr;     // CSR row pointer (size nrow+1)
    std::vector<int> col_idx;     // column indices
    std::vector<double> values;   // nonzero values
    std::vector<int> diag_ptr;    // pointer in col_idx/values to diagonal of each row
};

// Vector structure
struct Vector {
    int n;                       // length
    std::vector<double> values;   // data
};

/**
 * Performs one symmetric Gauss-Seidel sweep: forward pass with zero initial guess,
 * then backward pass including diagonal correction.
 * Assumes matrix has diagonal in each row, and row entries ordered as
 * lower-triangular, diagonal, upper-triangular.
 * @param A   matrix (CSR format)
 * @param r   right-hand side vector
 * @param x   on entry: may contain arbitrary values; on exit: result of one sweep
 * @return 0 on success
 */
int ComputeSYMGS(const SparseMatrix& A, const Vector& r, Vector& x) {
    const int n = A.nrow;
    if (x.n != n || r.n != n) return -1; // size mismatch

    // Forward sweep (lower triangular only, assuming x initial guess zero)
    for (int i = 0; i < n; ++i) {
        double sum = 0.0;
        int start = A.row_ptr[i];
        int end = A.diag_ptr[i]; // diagonal is at diag_ptr; lower entries are before
        for (int j = start; j < end; ++j) {
            sum += A.values[j] * x.values[A.col_idx[j]];
        }
        double diag = A.values[end]; // diagonal value at index end
        x.values[i] = (r.values[i] - sum) / diag;
    }

    // Backward sweep (include diagonal, then correct)
    for (int i = n - 1; i >= 0; --i) {
        double off_diag_sum = 0.0;
        int start = A.row_ptr[i];
        int end = A.row_ptr[i + 1];
        int diag_pos = A.diag_ptr[i];
        // Sum all off-diagonal contributions (lower and upper)
        for (int j = start; j < end; ++j) {
            if (j != diag_pos) {
                off_diag_sum += A.values[j] * x.values[A.col_idx[j]];
            }
        }
        double diag = A.values[diag_pos];
        x.values[i] = (r.values[i] - off_diag_sum) / diag;
    }

    return 0;
}

#include <cassert>
#include <cmath>

// Forward declarations (include the solution code above)
// The following main function tests the ComputeSYMGS function.

int main() {
    // Test 1: 1x1 identity matrix
    {
        SparseMatrix A;
        A.nrow = 1;
        A.row_ptr = {0, 1};
        A.col_idx = {0};
        A.values = {2.0};
        A.diag_ptr = {0};
        Vector r, x;
        r.n = 1; r.values = {4.0};
        x.n = 1; x.values = {0.0};
        assert(ComputeSYMGS(A, r, x) == 0);
        assert(std::fabs(x.values[0] - 2.0) < 1e-9);
    }

    // Test 2: 2x2 lower triangular matrix (no upper entries)
    {
        SparseMatrix A;
        A.nrow = 2;
        A.row_ptr = {0, 2, 3};
        A.col_idx = {0, 1, 1}; // row0: col0 (diag), col1 (lower? actually col1>0 but we treat as upper? To make it purely lower, we put only diag and a lower entry? Let's design: row0: diag at col0, no lower; row1: col0 (lower), col1 (diag)
        // Fix: row0: {0 (diag)}; row1: {0 (lower), 1 (diag)}
        A.row_ptr = {0, 1, 3};
        A.col_idx = {0, 0, 1};
        A.values = {3.0, -1.0, 4.0};
        A.diag_ptr = {0, 2};
        Vector r, x;
        r.n = 2; r.values = {6.0, 5.0};
        x.n = 2; x.values = {0.0, 0.0};
        assert(ComputeSYMGS(A, r, x) == 0);
        // Forward: x0 = 6/3=2; x1 = (5 - (-1)*2)/4 = (5+2)/4=1.75
        // Backward: i=1: off_diag = -1*2 = -2, x1 = (5 - (-2))/4 = 7/4=1.75
        // i=0: off_diag=0, x0=6/3=2
        assert(std::fabs(x.values[0] - 2.0) < 1e-9);
        assert(std::fabs(x.values[1] - 1.75) < 1e-9);
    }

    // Test 3: 3x3 tridiagonal matrix (diag 2, off diag -1)
    {
        SparseMatrix A;
        A.nrow = 3;
        // Row0: diag only (since no lower, upper is after diag but we don't care for forward)
        // Row1: lower (col0) then diag (col1) then upper (col2)
        // Row2: lower (col1) then diag (col2)
        A.row_ptr = {0, 1, 4, 6};
        A.col_idx = {0, 0, 1, 2, 1, 2};
        A.values = {2.0, -1.0, 2.0, -1.0, -1.0, 2.0};
        A.diag_ptr = {0, 2, 5};
        Vector r, x;
        r.n = 3; r.values = {1.0, 2.0, 3.0};
        x.n = 3; x.values = {0.0, 0.0, 0.0};
        assert(ComputeSYMGS(A, r, x) == 0);
        // Forward: x0=1/2=0.5; x1=(2 - (-1)*0.5)/2 = (2+0.5)/2=1.25; x2=(3 - (-1)*1.25)/2=(3+1.25)/2=2.125
        // Backward: i=2: off_diag = -1 * x1 = -1.25, x2 = (3 - (-1.25))/2 = 4.25/2=2.125
        // i=1: off_diag = -1*0.5 + -1*2.125 = -2.625, x1 = (2 - (-2.625))/2 = 4.625/2=2.3125
        // i=0: off_diag=0, x0 = 1/2=0.5
        assert(std::fabs(x.values[0] - 0.5) < 1e-9);
        assert(std::fabs(x.values[1] - 2.3125) < 1e-9);
        assert(std::fabs(x.values[2] - 2.125) < 1e-9);
    }

    // Test 4: size mismatch returns -1
    {
        SparseMatrix A;
        A.nrow = 1;
        A.row_ptr = {0, 1};
        A.col_idx = {0};
        A.values = {1.0};
        A.diag_ptr = {0};
        Vector r, x;
        r.n = 1; r.values = {1.0};
        x.n = 2; x.values = {0.0, 0.0};
        assert(ComputeSYMGS(A, r, x) == -1);
    }

    return 0;
}
