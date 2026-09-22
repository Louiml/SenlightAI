/*
Write a C++ function that takes a constant reference to an `Eigen::MatrixXi` and returns a new `Eigen::MatrixXi` that is the reverse of the input matrix along both rows and columns (i.e., the element at position (i,j) in the original appears at position (rows-1-i, cols-1-j) in the output). The function must not modify the input matrix. Additionally, provide a second free function that, given a constant reference to a matrix, modifies a copy of it by setting the coefficient at the position corresponding to `(1,0)` of the reversed matrix to the value `4`, and returns that modified copy. The task requires direct use of Eigen’s `.reverse()` and coefficient assignment, but you must ensure that the original input remains unchanged. The solution must include appropriate `#include <Eigen/Core>` and use `Eigen::MatrixXi`.
*/
#include <Eigen/Core>

// Return a new matrix that is the reverse of the input along both rows and columns.
// The original matrix is not modified.
Eigen::MatrixXi reverseMatrix(const Eigen::MatrixXi& input) {
    return input.reverse();
}

// Return a copy of the input matrix where the coefficient at position (1,0)
// in the reversed matrix has been set to 4. The original input is not modified.
Eigen::MatrixXi reverseAndSet(const Eigen::MatrixXi& input) {
    Eigen::MatrixXi result = input;  // copy so we don't modify original
    if (result.rows() >= 2 && result.cols() >= 1) {
        result.reverse()(1, 0) = 4;
    }
    return result;
}
#include <cassert>
#include <Eigen/Core>

// Solution functions declared above (place them here or include header)
Eigen::MatrixXi reverseMatrix(const Eigen::MatrixXi& input);
Eigen::MatrixXi reverseAndSet(const Eigen::MatrixXi& input);

int main() {
    // Test 1: Basic 3x4 reversal
    Eigen::MatrixXi m(3, 4);
    m << 1, 2, 3, 4,
         5, 6, 7, 8,
         9,10,11,12;
    Eigen::MatrixXi rev = reverseMatrix(m);
    assert(rev.rows() == 3 && rev.cols() == 4);
    assert(rev(0,0) == 12);
    assert(rev(2,3) == 1);
    assert(rev(1,1) == 7);

    // Test 2: Original not modified by reverseMatrix
    assert(m(0,0) == 1 && m(2,3) == 12);

    // Test 3: reverseAndSet modifies a copy only
    Eigen::MatrixXi modified = reverseAndSet(m);
    assert(modified.rows() == 3 && modified.cols() == 4);
    // In reversed matrix, (1,0) corresponds to original (rows-2, cols-1) = (1,3)
    // But we set through reverse, so check that reversed position has 4
    Eigen::MatrixXi revMod = modified.reverse();
    assert(revMod(1,0) == 4);
    // Original unchanged
    assert(m(1,3) == 8);

    // Test 4: Single row matrix
    Eigen::MatrixXi singleRow(1, 3);
    singleRow << 7, 8, 9;
    Eigen::MatrixXi revSingle = reverseMatrix(singleRow);
    assert(revSingle(0,0) == 9 && revSingle(0,2) == 7);
    // reverseAndSet with only 1 row should not change anything (guard)
    Eigen::MatrixXi modSingle = reverseAndSet(singleRow);
    assert(modSingle == singleRow);

    // Test 5: Empty matrix (0x0) works
    Eigen::MatrixXi empty(0,0);
    Eigen::MatrixXi revEmpty = reverseMatrix(empty);
    assert(revEmpty.rows() == 0 && revEmpty.cols() == 0);

    // Test 6: 2x2 matrix with negative numbers
    Eigen::MatrixXi neg(2,2);
    neg << -1, -2,
           -3, -4;
    Eigen::MatrixXi revNeg = reverseMatrix(neg);
    assert(revNeg(0,0) == -4 && revNeg(1,1) == -1);

    // Test 7: reverseAndSet on 2x2: reversed (1,0) corresponds to original (0,1)
    Eigen::MatrixXi modNeg = reverseAndSet(neg);
    Eigen::MatrixXi revModNeg = modNeg.reverse();
    assert(revModNeg(1,0) == 4);
    assert(neg(0,1) == -2); // original unchanged

    return 0;
}
// The core operation is Eigen’s `reverse()` method, which returns an expression object that can be used to read or write coefficients in reversed order. For the first function, simply return `input.reverse()` by value — Eigen will evaluate the expression into a new matrix. For the second function, we must avoid modifying the original; so we make a local copy `MatrixXi result = input;`, then assign `result.reverse()(1,0) = 4;`. Note that `(1,0)` in the reversed matrix corresponds to the original coefficient at `(rows-2, cols-1)` — but since we are writing through the reverse expression, Eigen correctly maps it. Time complexity is O(rows*cols) for copying/reversing, and space complexity for the returned matrix is also O(rows*cols). Edge cases: empty matrix (0x0) — reverse returns empty; single row/column — reversal works element-wise; we must not assume square dimensions. The second function must handle matrices with fewer than 2 rows or 1 column? Eigen’s `(1,0)` would be out of bounds for a 1-row matrix — but the task implies the input will be at least 2x1 for meaningful testing; still, we can guard by checking `rows >= 2 && cols >= 1` before assignment, but for the task specification we assume valid input.
