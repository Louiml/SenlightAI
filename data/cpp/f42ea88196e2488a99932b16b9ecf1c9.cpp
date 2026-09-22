Write a C++ function that takes a square matrix of integers (represented as an `Eigen::MatrixXi`) and a row index as parameters, and returns a new `Eigen::MatrixXi` containing exactly three consecutive rows starting from the given index (i.e., rows `index`, `index+1`, and `index+2`). The function must handle edge cases where the requested range would exceed matrix bounds by returning an empty matrix (0×0). The input matrix will always have at least 3 rows, and the row index will be a non-negative integer. Use Eigen's block operation `middleRows<3>` for the extraction, and ensure the returned matrix is a deep copy, not a view, so that modifying the result does not affect the original.

The solution leverages Eigen's `middleRows<3>(index)` method, which extracts a block of exactly 3 consecutive rows starting at `index`. This method requires that `index + 3 <= matrix.rows()`, otherwise it triggers an assertion failure in debug mode (and undefined behavior in release). Therefore, the main algorithmic step is to validate the bounds before performing the extraction: if `index + 3 > matrix.rows()`, return a default-constructed `MatrixXi` (which is 0×0). Otherwise, use `matrix.middleRows<3>(index)` to obtain a block, and then convert it to a dense `MatrixXi` by assigning it to a new matrix (e.g., `return MatrixXi(matrix.middleRows<3>(index));`), ensuring a deep copy. Time complexity is O(3 * cols) because copying three rows of the input matrix is linear in the number of columns; space complexity is likewise O(3 * cols) for the returned copy. Edge cases: index exactly equal to `rows-3` is valid; index at `rows-2` or `rows-1` should return empty; negative index is not considered per constraints.

#include <Eigen/Core>

// Extract three consecutive rows starting at 'rowIndex' as a deep copy.
// Returns an empty 0x0 matrix if the range exceeds the matrix dimensions.
Eigen::MatrixXi extractThreeRows(const Eigen::MatrixXi& matrix, int rowIndex) {
    // Check bounds: need at least rows rowIndex, rowIndex+1, rowIndex+2.
    if (rowIndex < 0 || rowIndex + 3 > matrix.rows()) {
        return Eigen::MatrixXi(); // default-constructed is 0x0
    }
    // Use middleRows<3> to get a block, then copy into a new matrix.
    return Eigen::MatrixXi(matrix.middleRows<3>(rowIndex));
}

#include <Eigen/Core>
#include <cassert>

int main() {
    Eigen::MatrixXi A(5, 4);
    A << 1, 2, 3, 4,
         5, 6, 7, 8,
         9, 10, 11, 12,
         13, 14, 15, 16,
         17, 18, 19, 20;

    // Normal extraction (rows 1,2,3)
    Eigen::MatrixXi result1 = extractThreeRows(A, 1);
    assert(result1.rows() == 3 && result1.cols() == 4);
    assert(result1(0,0) == 5 && result1(0,3) == 8);
    assert(result1(1,0) == 9 && result1(1,3) == 12);
    assert(result1(2,0) == 13 && result1(2,3) == 16);

    // Extraction from the last valid starting row (index=2, rows 2,3,4)
    Eigen::MatrixXi result2 = extractThreeRows(A, 2);
    assert(result2.rows() == 3 && result2.cols() == 4);
    assert(result2(0,0) == 9 && result2(2,3) == 20);

    // Out-of-bounds index (index=3 would need rows 3,4,5 but only 0..4 exist)
    Eigen::MatrixXi result3 = extractThreeRows(A, 3);
    assert(result3.rows() == 0 && result3.cols() == 0);

    // Out-of-bounds index (index=4)
    Eigen::MatrixXi result4 = extractThreeRows(A, 4);
    assert(result4.rows() == 0 && result4.cols() == 0);

    // Negative index (should be invalid per constraints, but safe)
    Eigen::MatrixXi result5 = extractThreeRows(A, -1);
    assert(result5.rows() == 0 && result5.cols() == 0);

    // Verify deep copy: modifying result doesn't affect original
    result1(0,0) = 999;
    assert(A(1,0) == 5);

    return 0;
}
