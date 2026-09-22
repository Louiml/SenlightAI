// Write a standalone C++ function named `padMatrix` that takes a 2D matrix (represented as `Eigen::MatrixXi`) and four integers: `padRows`, `padCols`, `inRows`, and `inCols`. The function must return a new matrix of size `padRows × padCols` where the original `inRows × inCols` matrix is placed at the bottom‑right corner, and all other cells (top and left padding areas) are filled with zeros. The padding should replicate the behavior of the provided snippet: for an output index `i`, it maps to input index `max(0, i - (out_size - in_size))`. This means positions outside the original matrix (before the bottom‑right region) are clamped to index 0. The input matrix must be assumed non‑empty, and the output dimensions must be at least as large as the input dimensions in both axes. The function must be `const`‑correct and use `Eigen` types.
#include <cassert>
#include <Eigen/Dense>

// The function under test is declared here (or included from the solution above).
Eigen::MatrixXi padMatrix(const Eigen::MatrixXi& input, int padRows, int padCols);

int main() {
    // 1x1 input padded to 3x3
    Eigen::MatrixXi in1(1,1);
    in1 << 5;
    Eigen::MatrixXi out1 = padMatrix(in1, 3, 3);
    assert(out1.rows() == 3 && out1.cols() == 3);
    assert(out1 == Eigen::MatrixXi::Constant(3,3,5));

    // 2x2 input padded to 3x3
    Eigen::MatrixXi in2(2,2);
    in2 << 1, 2,
           3, 4;
    Eigen::MatrixXi out2 = padMatrix(in2, 3, 3);
    // Expected: top-left 2x2 area filled with 1 (the top-left corner), and bottom-right 2x2 copied.
    Eigen::MatrixXi expected2(3,3);
    expected2 << 1, 1, 2,
                 1, 1, 2,
                 3, 3, 4;
    assert(out2 == expected2);

    // 3x3 input padded to 5x5
    Eigen::MatrixXi in3(3,3);
    in3 << 1,2,3,
           4,5,6,
           7,8,9;
    Eigen::MatrixXi out3 = padMatrix(in3, 5, 5);
    Eigen::MatrixXi expected3(5,5);
    expected3 << 1,1,1,2,3,
                 1,1,1,2,3,
                 1,1,1,2,3,
                 4,4,4,5,6,
                 7,7,7,8,9;
    assert(out3 == expected3);

    // Same dimensions (no padding)
    Eigen::MatrixXi in4(2,2);
    in4 << 9,8,
           7,6;
    Eigen::MatrixXi out4 = padMatrix(in4, 2, 2);
    assert(out4 == in4);

    // Non-square input and output
    Eigen::MatrixXi in5(1,3);
    in5 << 10,20,30;
    Eigen::MatrixXi out5 = padMatrix(in5, 2, 4);
    Eigen::MatrixXi expected5(2,4);
    expected5 << 10,10,10,20,
                 10,10,10,20;
    assert(out5 == expected5);

    return 0;
}
#include <Eigen/Dense>
#include <algorithm>
#include <cassert>

// Place the input matrix at the bottom-right corner of a larger matrix,
// filling the top-left padding area with the value of the input's top-left corner.
Eigen::MatrixXi padMatrix(const Eigen::MatrixXi& input, int padRows, int padCols) {
    const int inRows = input.rows();
    const int inCols = input.cols();
    assert(padRows >= inRows && padCols >= inCols);

    Eigen::MatrixXi result(padRows, padCols);
    const int rowOffset = padRows - inRows;
    const int colOffset = padCols - inCols;

    for (int r = 0; r < padRows; ++r) {
        const int srcRow = std::max(0, r - rowOffset);
        for (int c = 0; c < padCols; ++c) {
            const int srcCol = std::max(0, c - colOffset);
            result(r, c) = input(srcRow, srcCol);
        }
    }
    return result;
}
// The core idea is to directly compute the value for each output cell using the index mapping from the snippet. For a given output row `r` (0‑based) and column `c`, the corresponding input row is `max(0, r - (padRows - inRows))` and input column is `max(0, c - (padCols - inCols))`. For rows/columns in the top‑left padding region (where the expression inside `max` is negative), the result is 0. For rows/columns in the valid bottom‑right region, the result is the original matrix entry at that clamped index. Because the mapping clamps to 0, any output cell whose mapped row or column is 0 but that lies outside the original matrix will reference row/column 0 of the input – but note that when `r < (padRows - inRows)`, the mapped row is 0, but the original matrix has no rows above it, yet the mapping intentionally yields 0. For cells truly outside (i.e., `r < padRows - inRows` or `c < padCols - inCols`), the clamp yields 0, and the corresponding input element at `(0,0)` is copied. However, per the snippet’s semantics, this is actually the correct behavior: it places the original matrix at the bottom‑right, and all cells "above" or "to the left" of the original matrix are filled with the value of the top‑left corner of the original matrix (since they map to index 0). This matches the snippet’s output: the padded matrix is a replication of the top‑left element into the padding area. Edge cases: if the output dimensions equal the input dimensions, no padding occurs, and the function returns a copy. If either output dimension is smaller than the input, the behavior is undefined; we can assert that `padRows >= inRows` and `padCols >= inCols`. Time complexity is \(O(padRows \cdot padCols)\), as each output cell is computed in constant time. Space complexity is \(O(padRows \cdot padCols)\) for the result matrix.
