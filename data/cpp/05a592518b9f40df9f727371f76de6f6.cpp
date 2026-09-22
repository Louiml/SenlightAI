Write a standalone C++ function `compareMatrixSubset` that compares two column-major matrices `A` and `B`, where `B` may optionally be transposed, and only considers elements in either the upper triangle (`uplo == 'U'`), lower triangle (`uplo == 'L'`), or the full matrix (`uplo == 'N'`). The function must compute two metrics: (1) the maximum normalized absolute error, defined for each compared element as `|1 - B_value / A_value|` if `A_value != 0`, and `|B_value|` if `A_value == 0`; and (2) the Frobenius norm of the difference over the same compared elements, with off-diagonal entries counted twice (to account for the symmetric nature when only one triangle is stored) and diagonal entries counted once. The function signature must be `void compareMatrixSubset(const double *A, const double *B, long nrows, long ncols, char uplo, char trans, double &maxErr, double &frobeniusN)`. Assume both matrices are stored in column-major order (Fortran-style) with leading dimension equal to the number of rows. Handle invalid `uplo` by treating it as `'N'`. Only iterate over allowed indices, and ensure no division by zero.

The function needs to iterate over all rows `i` and columns `j` but only process elements that fall within the specified triangular or full region. For a column-major matrix with `nrows` rows and `ncols` columns, element `(i,j)` is stored at flat index `i + j * nrows`. If `trans == 'T'`, then `B` is logically the transpose of the expected shape, meaning `B` actually has `nrows` columns and `ncols` rows, and element `(i,j)` from `A` corresponds to `B` at index `j + i * ncols` (since `B` is column-major with leading dimension `ncols`). For `uplo == 'U'`, we only consider `j >= i`; for `uplo == 'L'`, we only consider `j <= i`; otherwise all elements. For each considered element, compute the normalized error: if `A_value == 0`, error is `abs(B_value)`, else `abs(1 - B_value / A_value)`. Track the maximum. For the Frobenius norm, accumulate `(B - A)^2`, and if the element is off-diagonal (i != j) and `uplo` is `'U'` or `'L'`, multiply by 2 (because the symmetric counterpart would also contribute). For `uplo == 'N'`, each element is counted once regardless of diagonal/off-diagonal. Finally, take the square root of the accumulated sum. Time complexity is `O(nrows * ncols)` in the worst case (full matrix), space complexity `O(1)`.

#include <cmath>
#include <algorithm>

/**
 * Compare two column-major matrices A and B, optionally transposing B,
 * and compute max normalized error and Frobenius norm of difference
 * over a specified triangular or full region.
 * 
 * @param A         Pointer to column-major matrix of size nrows x ncols.
 * @param B         Pointer to column-major matrix. If trans=='T', B is ncols x nrows.
 * @param nrows     Number of rows of A.
 * @param ncols     Number of columns of A.
 * @param uplo      'U' for upper triangle, 'L' for lower triangle, 'N' for full.
 * @param trans     'T' means B is stored transposed (ncols x nrows).
 * @param maxErr    Output: maximum normalized absolute error.
 * @param frobeniusN Output: Frobenius norm of difference over compared region.
 */
void compareMatrixSubset(const double *A, const double *B, long nrows, long ncols, char uplo, char trans, double &maxErr, double &frobeniusN) {
    // Normalize uplo
    if (uplo != 'U' && uplo != 'L') {
        uplo = 'N';
    }

    maxErr = 0.0;
    double sumSq = 0.0;

    for (long i = 0; i < nrows; ++i) {
        long start = 0;
        long end = ncols;
        if (uplo == 'U') start = i;
        else if (uplo == 'L') end = i + 1; // include diagonal

        for (long j = start; j < end; ++j) {
            // Index in A (column-major)
            long nA = i + j * nrows;
            // Index in B, possibly transposed
            long nB;
            if (trans == 'T') {
                // B has ncols rows and nrows columns, column-major
                nB = j + i * ncols;
            } else {
                nB = i + j * nrows;
            }

            double aVal = A[nA];
            double bVal = B[nB];

            // Maximum normalized error
            double diffNorm;
            if (aVal == 0.0) {
                diffNorm = std::abs(bVal);
            } else {
                diffNorm = std::abs(1.0 - bVal / aVal);
            }
            maxErr = std::max(maxErr, diffNorm);

            // Frobenius accumulation
            double diff = bVal - aVal;
            double term = diff * diff;
            // Double-count off-diagonal entries when only one triangle is stored
            if ((uplo == 'U' || uplo == 'L') && i != j) {
                term *= 2.0;
            }
            sumSq += term;
        }
    }

    frobeniusN = std::sqrt(sumSq);
}

#include <cassert>
#include <cmath>

// Declaration of the function (assume it's in the same translation unit)
void compareMatrixSubset(const double *A, const double *B, long nrows, long ncols, char uplo, char trans, double &maxErr, double &frobeniusN);

int main() {
    // Test 1: Identity matrix A, B same, full, no trans
    {
        double A[4] = {1.0, 0.0, 0.0, 1.0};
        double B[4] = {1.0, 0.0, 0.0, 1.0};
        double maxErr, frob;
        compareMatrixSubset(A, B, 2, 2, 'N', 'N', maxErr, frob);
        assert(std::abs(maxErr) < 1e-12);
        assert(std::abs(frob) < 1e-12);
    }

    // Test 2: A = [1 2; 3 4], B = A, full, no trans
    {
        double A[4] = {1.0, 3.0, 2.0, 4.0};
        double B[4] = {1.0, 3.0, 2.0, 4.0};
        double maxErr, frob;
        compareMatrixSubset(A, B, 2, 2, 'N', 'N', maxErr, frob);
        assert(std::abs(maxErr) < 1e-12);
        assert(std::abs(frob) < 1e-12);
    }

    // Test 3: A = [1 2; 3 4], B = A*2, upper triangle only
    {
        double A[4] = {1.0, 3.0, 2.0, 4.0};
        double B[4] = {2.0, 6.0, 4.0, 8.0};
        double maxErr, frob;
        compareMatrixSubset(A, B, 2, 2, 'U', 'N', maxErr, frob);
        // Only elements (0,0)=1, (0,1)=2, (1,1)=4 compared
        // Normalized errors: 1, 1, 1 => maxErr = 1
        assert(std::abs(maxErr - 1.0) < 1e-12);
        // Frobenius: diffs: 1,2,4 squared =1+4+16=21, off-diag (0,1) doubled => +4 => total 25, sqrt=5
        assert(std::abs(frob - 5.0) < 1e-12);
    }

    // Test 4: A = [1 2; 3 4], B = transpose of A, full, trans
    {
        double A[4] = {1.0, 3.0, 2.0, 4.0}; // A = [1 2; 3 4]
        double B[4] = {1.0, 2.0, 3.0, 4.0}; // B = [1 3; 2 4] stored column-major => transpose of A
        double maxErr, frob;
        compareMatrixSubset(A, B, 2, 2, 'N', 'T', maxErr, frob);
        assert(std::abs(maxErr) < 1e-12);
        assert(std::abs(frob) < 1e-12);
    }

    // Test 5: A contains zero, B nonzero, check normalized error handles zero denominator
    {
        double A[4] = {0.0, 5.0, 0.0, 7.0};
        double B[4] = {3.0, 5.0, 0.0, 7.0};
        double maxErr, frob;
        compareMatrixSubset(A, B, 2, 2, 'N', 'N', maxErr, frob);
        // For element (0,0): A=0, B=3 => error=3
        // Others match, so maxErr=3
        assert(std::abs(maxErr - 3.0) < 1e-12);
    }

    // Test 6: Invalid uplo treated as full
    {
        double A[4] = {1.0, 2.0, 3.0, 4.0};
        double B[4] = {1.0, 2.0, 3.0, 4.0};
        double maxErr, frob;
        compareMatrixSubset(A, B, 2, 2, 'X', 'N', maxErr, frob);
        assert(std::abs(maxErr) < 1e-12);
        assert(std::abs(frob) < 1e-12);
    }

    // Test 7: Non-square matrix, upper triangle, B transposed
    {
        // A is 3x2: [1 2; 3 4; 5 6] stored column-major: [1,3,5,2,4,6]
        double A[6] = {1.0, 3.0, 5.0, 2.0, 4.0, 6.0};
        // B is transpose shape 2x3, but trans='T' means B is stored as 2x3 column-major
        // B should be A^T = [1 3 5; 2 4 6] stored column-major: [1,2,3,4,5,6]
        double B[6] = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0};
        double maxErr, frob;
        compareMatrixSubset(A, B, 3, 2, 'U', 'T', maxErr, frob);
        // A elements (i,j): (0,0)=1, (0,1)=2, (1,1)=4
        // B mapping: (0,0)->1, (0,1)->2, (1,1)->4, all match
        assert(std::abs(maxErr) < 1e-12);
        assert(std::abs(frob) < 1e-12);
    }

    // Test 8: Lower triangle with doubling, non-diagonal only
    {
        double A[4] = {1.0, 2.0, 3.0, 4.0};
        double B[4] = {1.0, 4.0, 3.0, 4.0}; // only (1,0) differs: 2 -> 4
        double maxErr, frob;
        compareMatrixSubset(A, B, 2, 2, 'L', 'N', maxErr, frob);
        // Compared elements: (0,0), (1,0), (1,1)
        // (1,0): A=2, B=4, diff=2, normalized err=1, frob contribution 4*2=8
        // others match, sumSq=8, sqrt=2*sqrt(2) ~2.828
        assert(std::abs(maxErr - 1.0) < 1e-12);
        assert(std::abs(frob - 2.0 * std::sqrt(2.0)) < 1e-12);
    }

    // Test 9: Full matrix with off-diagonal, no doubling
    {
        double A[4] = {1.0, 2.0, 3.0, 4.0};
        double B[4] = {1.0, 4.0, 3.0, 4.0};
        double maxErr, frob;
        compareMatrixSubset(A, B, 2, 2, 'N', 'N', maxErr, frob);
        // Element (1,0): diff=2, frob contribution 4 (no doubling)
        // maxErr = abs(1 - 4/2)=1
        assert(std::abs(maxErr - 1.0) < 1e-12);
        assert(std::abs(frob - 2.0) < 1e-12);
    }

    return 0;
}
