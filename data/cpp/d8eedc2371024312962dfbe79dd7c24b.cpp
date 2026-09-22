/*
Write a C++ function that simulates the construction of a tridiagonal matrix using a row-oriented filling strategy, without relying on any external matrix or parallel-for libraries. The function should accept an integer `matrixSize` (assumed to be at least 2) and return a `std::vector<std::vector<double>>` representing the square matrix of that size. The matrix must have `2.0` on the main diagonal, `-1.0` on the first subdiagonal (below the diagonal, i.e., row index > column index) and first superdiagonal (above the diagonal, i.e., column index > row index), and `0.0` everywhere else. To mimic the row-filling pattern from the snippet, you must fill the matrix row by row: for each row, if the row index is greater than 0, set the element at column `rowIdx-1` to `-1.0`; always set the element at column `rowIdx` to `2.0`; and if the row index is less than `matrixSize-1`, set the element at column `rowIdx+1` to `-1.0`. Do not modify any other entries. The function should be `const`-correct in the sense that it does not modify any external state and returns the matrix by value.
*/

#include <vector>

// Builds a tridiagonal matrix of given size with 2.0 on the diagonal
// and -1.0 on the first off-diagonals, filled row by row.
std::vector<std::vector<double>> buildTridiagonalMatrix(const int matrixSize) {
    // Initialize an matrixSize x matrixSize matrix filled with zeros.
    std::vector<std::vector<double>> matrix(matrixSize, std::vector<double>(matrixSize, 0.0));

    // Fill each row according to the required pattern.
    for (int rowIdx = 0; rowIdx < matrixSize; ++rowIdx) {
        // Element below the diagonal (left neighbor) if exists.
        if (rowIdx > 0) {
            matrix[rowIdx][rowIdx - 1] = -1.0;
        }
        // Diagonal element.
        matrix[rowIdx][rowIdx] = 2.0;
        // Element above the diagonal (right neighbor) if exists.
        if (rowIdx < matrixSize - 1) {
            matrix[rowIdx][rowIdx + 1] = -1.0;
        }
    }

    return matrix;
}

#include <cassert>
#include <vector>

// Include the solution header (if separate) or assume the function is defined above.
// Here we reproduce the function for completeness in the test context.
std::vector<std::vector<double>> buildTridiagonalMatrix(const int matrixSize);

int main() {
    // Test size 2.
    auto m2 = buildTridiagonalMatrix(2);
    assert(m2.size() == 2);
    assert(m2[0].size() == 2);
    assert(m2[0][0] == 2.0);
    assert(m2[0][1] == -1.0);
    assert(m2[1][0] == -1.0);
    assert(m2[1][1] == 2.0);

    // Test size 3.
    auto m3 = buildTridiagonalMatrix(3);
    assert(m3[0][0] == 2.0);
    assert(m3[0][1] == -1.0);
    assert(m3[0][2] == 0.0);
    assert(m3[1][0] == -1.0);
    assert(m3[1][1] == 2.0);
    assert(m3[1][2] == -1.0);
    assert(m3[2][0] == 0.0);
    assert(m3[2][1] == -1.0);
    assert(m3[2][2] == 2.0);

    // Test size 5: check that all zeros are correct in a non-adjacent cell.
    auto m5 = buildTridiagonalMatrix(5);
    assert(m5[0][3] == 0.0);
    assert(m5[4][0] == 0.0);
    assert(m5[2][4] == 0.0);
    // Check the main diagonal values.
    for (int i = 0; i < 5; ++i) {
        assert(m5[i][i] == 2.0);
    }
    // Check subdiagonal and superdiagonal.
    for (int i = 0; i < 4; ++i) {
        assert(m5[i + 1][i] == -1.0);
        assert(m5[i][i + 1] == -1.0);
    }

    return 0;
}

// The task mirrors the behavior of the original snippet but strips away the TNL dependency. The key is to allocate a 2D vector of size `matrixSize x matrixSize` initialized to `0.0`. Then iterate over each row index from `0` to `matrixSize-1`. For each row: if the row index is positive, assign `-1.0` to the element at column `rowIdx-1` (the left neighbor, which is below the diagonal in row-major terms); assign `2.0` to the diagonal element at column `rowIdx`; and if the row index is less than `matrixSize-1`, assign `-1.0` to the element at column `rowIdx+1` (the right neighbor, above the diagonal). This matches the original logic exactly. Edge cases: for `matrixSize = 2`, the first row (index 0) has only diagonal and superdiagonal; the last row (index 1) has only subdiagonal and diagonal; no out-of-bounds access occurs because the conditions prevent writing beyond the matrix. Time complexity is \(O(n^2)\) because the matrix has \(n^2\) entries, and we initialize all of them to zero and then modify \(3n-2\) entries for the tridiagonal structure. Space complexity is also \(O(n^2)\) for storing the returned matrix. The approach is straightforward and uses no extra data structures beyond the matrix itself.
