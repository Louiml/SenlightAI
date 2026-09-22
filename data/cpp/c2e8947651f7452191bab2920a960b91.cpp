Given a 2D matrix of floating-point values stored as an Eigen `MatrixXf`, write a C++ function `findColumnWithMaxSum` that returns the index of the column whose entries have the largest sum. If multiple columns tie for the maximum sum, return the smallest such index. The function must accept the matrix by const reference, handle non-empty input (at least one row and one column), and internally use Eigen's block/colwise operations without manually looping over every element. The function should preserve the original matrix and not modify it.

#include <cassert>
#include <Eigen/Dense>

Eigen::Index findColumnWithMaxSum(const Eigen::MatrixXf& mat);

int main() {
    // Test 1: Example from the snippet.
    Eigen::MatrixXf mat1(2, 4);
    mat1 << 1, 2, 6, 9,
            3, 1, 7, 2;
    assert(findColumnWithMaxSum(mat1) == 2); // column 2 (0-based) has sum 13.

    // Test 2: Single column.
    Eigen::MatrixXf mat2(3, 1);
    mat2 << 1, -5, 3;
    assert(findColumnWithMaxSum(mat2) == 0);

    // Test 3: Tie between columns, must pick smallest index.
    Eigen::MatrixXf mat3(2, 4);
    mat3 << 1, 1, 5, 5,
            2, 2, -1, -1;
    // Column sums: 3, 3, 4, 4 -> tie at 0 and 1? Actually col0=3, col1=3, col2=4, col3=4.
    // But to test tie: use all equal sums.
    Eigen::MatrixXf mat4(2, 3);
    mat4 << 1, 2, 3,
            4, 5, 6;
    // Sums: 5, 7, 9? That's not tie. Use a clear tie:
    Eigen::MatrixXf mat5(2, 3);
    mat5 << 1, 2, 3,
            1, 2, 3;
    // Sums: 2, 4, 6? Not tie either. Let's do:
    Eigen::MatrixXf mat6(2, 3);
    mat6 << 1, 1, 1,
            1, 1, 1;
    // All sums = 2, tie at index 0.
    assert(findColumnWithMaxSum(mat6) == 0);

    // Test 4: Negative values.
    Eigen::MatrixXf mat7(2, 2);
    mat7 << -1, -2,
            -3, -4;
    // Sums: -4, -6 -> max is -4 at index 0.
    assert(findColumnWithMaxSum(mat7) == 0);

    // Test 5: Larger matrix, ensure correct index.
    Eigen::MatrixXf mat8 = Eigen::MatrixXf::Zero(4, 5);
    mat8(0, 2) = 10;
    mat8(3, 2) = 5;
    assert(findColumnWithMaxSum(mat8) == 2); // column 2 sum = 15, others 0.

    return 0;
}

#include <Eigen/Dense>

// Return the column index (0-based) with the largest sum of entries.
// If there are ties, return the smallest index. The matrix must not be empty.
Eigen::Index findColumnWithMaxSum(const Eigen::MatrixXf& mat) {
    // Compute sums of all columns in one vectorized operation.
    auto colSums = mat.colwise().sum();
    
    Eigen::Index maxIndex;
    colSums.maxCoeff(&maxIndex);
    
    return maxIndex;
}

// The core algorithm uses Eigen's `colwise().sum()` to compute a row vector containing the sum of each column in one vectorized operation. Then `maxCoeff(&index)` returns the largest value and stores the position of the first occurrence of that maximum (Eigen's `maxCoeff` returns the index of the first maximum when ties exist), which satisfies the requirement for the smallest index. The matrix is accessed via `const MatrixXf&`, ensuring no modification. Edge cases: the matrix may have one column (trivial), negative values (still works), and floating-point equality ties (Eigen's `maxCoeff` returns the first index, so tie-breaking is automatic). Time complexity is O(rows * columns) because `colwise().sum()` scans each element once, and `maxCoeff` adds O(columns). Space complexity is O(columns) for the temporary column-sum vector.
