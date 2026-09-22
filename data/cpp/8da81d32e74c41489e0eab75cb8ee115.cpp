// Write a C++ function that takes a square matrix represented as a `std::vector<std::vector<double>>` and returns its transpose as a new matrix. The input matrix is guaranteed to be square (same number of rows and columns). The function must preserve the original matrix unchanged and return the transposed matrix. Handle edge cases like an empty matrix (return an empty matrix) and a 1×1 matrix (return the same single element). The solution should use appropriate `const` qualifiers for the input and be robust for any square size.
// The solution simply iterates over the rows and columns of the input matrix. For each element at position `(i, j)`, it places it in the output at position `(j, i)`. Since the matrix is square, the output has the same dimensions. Edge cases: an empty matrix (size 0×0) results in an empty output; a 1×1 matrix trivially returns its single element. Time complexity is \(O(n^2)\) where \(n\) is the number of rows/columns, because we visit each element exactly once. Space complexity is \(O(n^2)\) for the output matrix, plus \(O(1)\) additional auxiliary space. The input is passed by const reference to avoid copying and to prevent modification.
#include <vector>

// Return the transpose of a square matrix.
// The input matrix is preserved unchanged.
std::vector<std::vector<double>> transposeMatrix(const std::vector<std::vector<double>>& matrix) {
    // Handle empty matrix case.
    if (matrix.empty()) {
        return {};
    }

    const size_t n = matrix.size();  // square, so rows == columns

    // Initialize result with n rows and n columns, all zeros.
    std::vector<std::vector<double>> result(n, std::vector<double>(n, 0.0));

    for (size_t i = 0; i < n; ++i) {
        // Note: matrix is guaranteed square, so matrix[i].size() == n.
        for (size_t j = 0; j < n; ++j) {
            result[j][i] = matrix[i][j];
        }
    }

    return result;
}
#include <cassert>
#include <vector>

// The solution function is already defined above; we just test it here.

int main() {
    // Empty matrix
    std::vector<std::vector<double>> empty;
    assert(transposeMatrix(empty).empty());

    // 1x1 matrix
    std::vector<std::vector<double>> one = {{42.0}};
    auto oneT = transposeMatrix(one);
    assert(oneT.size() == 1);
    assert(oneT[0].size() == 1);
    assert(oneT[0][0] == 42.0);

    // 2x2 matrix
    std::vector<std::vector<double>> mat2 = {{1.0, 2.0}, {3.0, 4.0}};
    auto t2 = transposeMatrix(mat2);
    assert(t2[0][0] == 1.0);
    assert(t2[0][1] == 3.0);
    assert(t2[1][0] == 2.0);
    assert(t2[1][1] == 4.0);

    // 3x3 matrix
    std::vector<std::vector<double>> mat3 = {
        {1.0, 2.0, 3.0},
        {4.0, 5.0, 6.0},
        {7.0, 8.0, 9.0}
    };
    auto t3 = transposeMatrix(mat3);
    assert(t3[0][0] == 1.0); assert(t3[0][1] == 4.0); assert(t3[0][2] == 7.0);
    assert(t3[1][0] == 2.0); assert(t3[1][1] == 5.0); assert(t3[1][2] == 8.0);
    assert(t3[2][0] == 3.0); assert(t3[2][1] == 6.0); assert(t3[2][2] == 9.0);

    // Symmetric matrix remains unchanged
    std::vector<std::vector<double>> sym = {{1.0, 2.0}, {2.0, 1.0}};
    auto symT = transposeMatrix(sym);
    assert(symT[0][0] == 1.0); assert(symT[0][1] == 2.0);
    assert(symT[1][0] == 2.0); assert(symT[1][1] == 1.0);

    // Larger matrix (4x4) with random values
    std::vector<std::vector<double>> big = {
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 10, 11, 12},
        {13, 14, 15, 16}
    };
    auto bigT = transposeMatrix(big);
    for (size_t i = 0; i < 4; ++i) {
        for (size_t j = 0; j < 4; ++j) {
            assert(bigT[i][j] == big[j][i]);
        }
    }

    // Original matrices are unchanged
    assert(mat2[0][0] == 1.0); assert(mat2[0][1] == 2.0);
    assert(mat2[1][0] == 3.0); assert(mat2[1][1] == 4.0);

    return 0;
}
