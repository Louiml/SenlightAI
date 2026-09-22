/*
Write a C++ function that, given a non-empty square matrix of floating-point values (passed by const reference as an `Eigen::MatrixXd`), returns a new `Eigen::MatrixXd` where each entry `(i,j)` is replaced by the sum of that entry and the entry directly below it if it exists (i.e., row `i+1`), and if no such entry exists (i.e., for the last row), the entry remains unchanged. The original matrix must not be modified. The function should handle any square size (including 1×1), and the result must be correctly computed for both positive and negative values. For example, for a 2×2 matrix `[[3, -1], [2.5, 1.5]]`, the result would be `[[5.5, 0.5], [2.5, 1.5]]` because `(0,0)=3+2.5=5.5`, `(0,1)=-1+1.5=0.5`, and row 1 stays unchanged. Ensure your function is `const`-correct and does not rely on global state.
*/
#include <Eigen/Dense>

// Return a new matrix where each entry (i,j) is replaced by the sum of that entry
// and the entry directly below it (i+1,j). The last row is left unchanged.
// The input matrix is not modified.
Eigen::MatrixXd addBelowEntries(const Eigen::MatrixXd& input) {
    // Create a copy of the input to hold the result.
    Eigen::MatrixXd result = input;

    // Iterate over all rows except the last one.
    const int rows = input.rows();
    const int cols = input.cols();

    for (int i = 0; i < rows - 1; ++i) {
        for (int j = 0; j < cols; ++j) {
            // Replace entry (i,j) with the sum of the original (i,j) and (i+1,j).
            result(i, j) = input(i, j) + input(i + 1, j);
        }
    }

    return result;
}
#include <cassert>
#include <Eigen/Dense>
#include <cmath>

// The solution function is declared above.
Eigen::MatrixXd addBelowEntries(const Eigen::MatrixXd& input);

int main() {
    // Test case 1: 2x2 matrix from the prompt.
    Eigen::MatrixXd m1(2, 2);
    m1 << 3, -1,
           2.5, 1.5;
    Eigen::MatrixXd result1 = addBelowEntries(m1);
    assert(result1.rows() == 2 && result1.cols() == 2);
    assert(std::abs(result1(0, 0) - 5.5) < 1e-9);
    assert(std::abs(result1(0, 1) - 0.5) < 1e-9);
    assert(std::abs(result1(1, 0) - 2.5) < 1e-9);
    assert(std::abs(result1(1, 1) - 1.5) < 1e-9);
    // Ensure original matrix is unchanged.
    assert(std::abs(m1(0, 0) - 3) < 1e-9 && std::abs(m1(0, 1) - (-1)) < 1e-9);

    // Test case 2: 3x3 matrix with mixed signs and zeros.
    Eigen::MatrixXd m2(3, 3);
    m2 << 1, 2, 3,
          4, 5, 6,
          7, 8, 9;
    Eigen::MatrixXd result2 = addBelowEntries(m2);
    assert(std::abs(result2(0, 0) - 5) < 1e-9); // 1+4
    assert(std::abs(result2(0, 1) - 7) < 1e-9); // 2+5
    assert(std::abs(result2(0, 2) - 9) < 1e-9); // 3+6
    assert(std::abs(result2(1, 0) - 11) < 1e-9); // 4+7
    assert(std::abs(result2(1, 1) - 13) < 1e-9); // 5+8
    assert(std::abs(result2(1, 2) - 15) < 1e-9); // 6+9
    assert(std::abs(result2(2, 0) - 7) < 1e-9);  // last row unchanged
    assert(std::abs(result2(2, 1) - 8) < 1e-9);
    assert(std::abs(result2(2, 2) - 9) < 1e-9);

    // Test case 3: 1x1 matrix.
    Eigen::MatrixXd m3(1, 1);
    m3 << 42;
    Eigen::MatrixXd result3 = addBelowEntries(m3);
    assert(std::abs(result3(0, 0) - 42) < 1e-9);

    // Test case 4: 4x4 matrix with negative values.
    Eigen::MatrixXd m4(4, 4);
    m4 << -1, 0, 3, -2,
           2, -4, -1, 5,
           6, 1, -3, 0,
            -2, 7, 4, -5;
    Eigen::MatrixXd result4 = addBelowEntries(m4);
    assert(std::abs(result4(0, 0) - 1) < 1e-9);  // -1+2
    assert(std::abs(result4(0, 1) - (-4)) < 1e-9); // 0+(-4)
    assert(std::abs(result4(0, 2) - 2) < 1e-9);   // 3+(-1)
    assert(std::abs(result4(0, 3) - 3) < 1e-9);   // -2+5
    assert(std::abs(result4(1, 0) - 8) < 1e-9);   // 2+6
    assert(std::abs(result4(1, 1) - (-3)) < 1e-9); // -4+1
    assert(std::abs(result4(1, 2) - (-4)) < 1e-9); // -1+(-3)
    assert(std::abs(result4(1, 3) - 5) < 1e-9);   // 5+0
    assert(std::abs(result4(2, 0) - 4) < 1e-9);   // 6+(-2)
    assert(std::abs(result4(2, 1) - 8) < 1e-9);   // 1+7
    assert(std::abs(result4(2, 2) - 1) < 1e-9);   // -3+4
    assert(std::abs(result4(2, 3) - (-5)) < 1e-9); // 0+(-5)
    assert(std::abs(result4(3, 0) - (-2)) < 1e-9); // last row unchanged
    assert(std::abs(result4(3, 1) - 7) < 1e-9);
    assert(std::abs(result4(3, 2) - 4) < 1e-9);
    assert(std::abs(result4(3, 3) - (-5)) < 1e-9);

    return 0;
}
// The solution approach is straightforward: create a copy of the input matrix, then for every row index `i` from 0 to `rows-2` (i.e., all rows except the last), iterate over all columns `j`, and set the copy’s entry `(i,j)` to the sum of the original's `(i,j)` and `(i+1,j)`. For the last row, no change is needed because there is no row below. The function takes the matrix by `const Eigen::MatrixXd&` to avoid copying and to prevent modification of the input. The return value is a new `MatrixXd` initialized as a copy of the input (e.g., using `MatrixXd result = input;`), then modified. Edge cases: a 1×1 matrix has `rows-2 = -1`, so the loop simply does not run and the copy is returned unchanged; this is safe. Square matrix assumption is given, but if a non-square were provided, the function still works because it uses the actual number of rows (`rows()`) and all columns (`cols()`). Time complexity is O(n²) for an n×n matrix (since we iterate over (n-1)*n ≈ n² entries), and space complexity is O(n²) for the returned copy; the input is not modified. Const correctness is applied in the parameter and by not mutating the input.
