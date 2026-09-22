Write a C++ function that accepts an `Eigen::MatrixXi` (an integer matrix from the Eigen library) and returns a new `Eigen::MatrixXi` consisting of the middle two rows of the input matrix. If the matrix has fewer than 2 rows, the function should return an empty matrix (size 0×0). The function must not modify the input matrix. For example, given a 5×5 matrix, the returned matrix should contain rows 2 and 3 (0-indexed) of the original, i.e., rows with indices \[2, 3). If the input has 2 or 3 rows, return all rows (since the middle two rows are the entire matrix for 2 rows, and rows 1 and 2 for 3 rows). The function should handle any positive number of columns, including zero columns.

// The solution uses Eigen's `middleRows(start, count)` method, which returns a block view of the matrix. However, to return a new independent matrix, we must copy the block into a fresh `Eigen::MatrixXi`. The algorithm: first, check if the number of rows `rows = A.rows()` is less than 2. If so, return a `MatrixXi(0,0)` (default-constructed, which is 0×0). Otherwise, compute the starting index as `(rows - 2) / 2` (integer division). For example, with 5 rows, `(5-2)/2 = 1`, but we want start at 2? Wait, re-check: middle two rows of 5 rows are indices 1 and 2? Actually, "middle two" is ambiguous—the task defines it as rows with indices \[2,3) for a 5-row matrix, meaning the third and fourth rows (0-indexed 2 and 3). That is, we take two rows starting at index `rows/2`? For 5 rows, `rows/2 = 2` (integer division), so start=2, count=2 → rows 2 and 3. For 4 rows, `rows/2 = 2`, start=2, count=2 → rows 2 and 3 (the last two). For 3 rows, `rows/2 = 1`, start=1, count=2 → rows 1 and 2 (last two). For 2 rows, `rows/2 = 1`, start=1, count=2 → but that would go out of bounds? Actually, `middleRows(1,2)` on a 2-row matrix would be invalid because start+count > rows. So we must handle: if rows == 2, return the whole matrix (copy). If rows > 2, start = (rows - 2) / 2? Let's see: for 5, (5-2)/2=1 → start=1 gives rows 1,2 not matching spec. The spec says for 5 rows, take rows 2 and 3 (indices 2,3). So we need start = (rows - 2) / 2? That gives 1 for 5. Not right. Instead, we want the two rows centered around the middle. For 5 rows, middle index is 2 (0-indexed), so take rows 2 and 3? That's not symmetric. Actually the spec explicitly says: "given a 5×5 matrix, the returned matrix should contain rows 2 and 3 (0-indexed)". So start = 2 for 5. That is start = rows/2? For 5, rows/2=2 → correct. For 4, rows/2=2 → start=2, count=2 → rows 2,3 (last two). For 3, rows/2=1 → start=1, count=2 → rows 1,2 (last two). For 2, rows/2=1 → start=1, count=2 → out of bounds. So we need special case for rows==2: return a copy of the whole matrix. For rows>2, start = rows/2, count=2. For rows<2, return empty. This matches the spec: for 5 rows, start=2, rows 2 and 3. For 4 rows, start=2, rows 2 and 3 (middle two? actually middle two of 4 are rows 1 and 2, but spec doesn't specify; it only specifies 5 rows example). But the analysis must state the logic: For rows >= 2, if rows == 2, return copy; else start = rows/2 (integer division) and count=2, which for even rows gives the last two, for odd rows gives the two centered around the middle. The time complexity is O(rows*cols) to copy the block, and space O(rows*cols) for the result. Edge case: rows < 2 returns empty. Also handle zero columns gracefully—copying an empty block is fine. Use `const Eigen::MatrixXi&` parameter and return by value.

#include <Eigen/Core>

// Return a new matrix containing the middle two rows of the input matrix.
// For matrices with exactly 2 rows, returns a copy of the whole matrix.
// For matrices with fewer than 2 rows, returns a 0x0 matrix.
// The middle two rows are defined as starting at row index rows/2 (integer division).
Eigen::MatrixXi middleTwoRows(const Eigen::MatrixXi& A) {
    const int rows = A.rows();
    const int cols = A.cols();
    
    if (rows < 2) {
        return Eigen::MatrixXi(0, 0);
    }
    if (rows == 2) {
        return A;  // implicit copy
    }
    
    const int start = rows / 2;
    return A.middleRows(start, 2);  // returns a block expression, but assignment to MatrixXi copies
}

#include <Eigen/Core>
#include <cassert>

int main() {
    // Test 5x5 matrix: middle rows are indices 2 and 3
    Eigen::MatrixXi A(5,5);
    A << 1, 2, 3, 4, 5,
         6, 7, 8, 9, 10,
         11,12,13,14,15,
         16,17,18,19,20,
         21,22,23,24,25;
    Eigen::MatrixXi result = middleTwoRows(A);
    assert(result.rows() == 2 && result.cols() == 5);
    assert(result(0,0) == 11 && result(0,4) == 15);
    assert(result(1,0) == 16 && result(1,4) == 20);

    // Test 2x3 matrix: returns copy of whole matrix
    Eigen::MatrixXi B(2,3);
    B << 1, 2, 3,
         4, 5, 6;
    result = middleTwoRows(B);
    assert(result.rows() == 2 && result.cols() == 3);
    assert(result(0,0) == 1 && result(1,2) == 6);

    // Test 3x2 matrix: middle rows are indices 1 and 2 (rows/2 = 1)
    Eigen::MatrixXi C(3,2);
    C << 1, 2,
         3, 4,
         5, 6;
    result = middleTwoRows(C);
    assert(result.rows() == 2 && result.cols() == 2);
    assert(result(0,0) == 3 && result(1,1) == 6);

    // Test 1x5 matrix: returns empty
    Eigen::MatrixXi D(1,5);
    D << 1,2,3,4,5;
    result = middleTwoRows(D);
    assert(result.rows() == 0 && result.cols() == 0);

    // Test 0x0 matrix: returns empty
    Eigen::MatrixXi E(0,0);
    result = middleTwoRows(E);
    assert(result.rows() == 0 && result.cols() == 0);

    // Test 4x4 matrix: start = 2, rows 2 and 3
    Eigen::MatrixXi F(4,4);
    F << 1,2,3,4,
         5,6,7,8,
         9,10,11,12,
         13,14,15,16;
    result = middleTwoRows(F);
    assert(result.rows() == 2 && result.cols() == 4);
    assert(result(0,0) == 9 && result(1,3) == 16);

    return 0;
}
