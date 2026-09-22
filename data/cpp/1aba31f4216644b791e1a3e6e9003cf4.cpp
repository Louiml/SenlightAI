/*
Write a C++ function `transposeMatrix` that accepts a non-empty rectangular matrix represented as a `std::vector<std::vector<int>>` and returns a new matrix which is its transpose (rows become columns, columns become rows). The function must preserve the exact element order correctly, handle both square and non-square matrices, and work for matrices with dimensions up to a reasonable size (e.g., 1000×1000). The input matrix is guaranteed to have at least one row and one column, and all rows will have the same number of columns. Return the transposed matrix as a `std::vector<std::vector<int>>`.
*/
#include <vector>

// Return the transpose of a non-empty rectangular matrix.
std::vector<std::vector<int>> transposeMatrix(const std::vector<std::vector<int>>& matrix) {
    const size_t rows = matrix.size();
    const size_t cols = matrix[0].size();

    // Initialize result with cols rows and rows columns.
    std::vector<std::vector<int>> result(cols, std::vector<int>(rows));

    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            result[j][i] = matrix[i][j];
        }
    }

    return result;
}
#include <cassert>
#include <vector>

// Function under test
std::vector<std::vector<int>> transposeMatrix(const std::vector<std::vector<int>>& matrix);

int main() {
    // 2x3 matrix
    std::vector<std::vector<int>> m1 = {{1,2,3},{4,5,6}};
    std::vector<std::vector<int>> t1 = transposeMatrix(m1);
    assert(t1.size() == 3);
    assert(t1[0] == std::vector<int>({1,4}));
    assert(t1[1] == std::vector<int>({2,5}));
    assert(t1[2] == std::vector<int>({3,6}));

    // 3x2 matrix (transpose of t1)
    std::vector<std::vector<int>> m2 = {{1,4},{2,5},{3,6}};
    std::vector<std::vector<int>> t2 = transposeMatrix(m2);
    assert(t2.size() == 2);
    assert(t2[0] == std::vector<int>({1,2,3}));
    assert(t2[1] == std::vector<int>({4,5,6}));

    // 1x4 matrix (single row)
    std::vector<std::vector<int>> m3 = {{7,8,9,10}};
    std::vector<std::vector<int>> t3 = transposeMatrix(m3);
    assert(t3.size() == 4);
    for (int i = 0; i < 4; ++i) {
        assert(t3[i].size() == 1);
        assert(t3[i][0] == 7 + i);
    }

    // 4x1 matrix (single column)
    std::vector<std::vector<int>> m4 = {{1},{2},{3},{4}};
    std::vector<std::vector<int>> t4 = transposeMatrix(m4);
    assert(t4.size() == 1);
    assert(t4[0] == std::vector<int>({1,2,3,4}));

    // Square matrix
    std::vector<std::vector<int>> m5 = {{1,2},{3,4}};
    std::vector<std::vector<int>> t5 = transposeMatrix(m5);
    assert(t5[0] == std::vector<int>({1,3}));
    assert(t5[1] == std::vector<int>({2,4}));

    // Check that input is not modified
    assert(m1[0][0] == 1 && m1[2].empty() == false); // just ensure original intact

    return 0;
}
// The core operation is to swap the indices: for an input matrix `A` of size `n` rows and `m` columns, the output matrix `B` will have `m` rows and `n` columns, and element `B[i][j]` equals `A[j][i]`. We first determine `n = A.size()` and `m = A[0].size()`. We allocate `B` with `m` rows, each containing `n` integers initialized to zero. Then, using nested loops over `i` from 0 to `n-1` and `j` from 0 to `m-1`, we assign `B[j][i] = A[i][j]`. Edge cases include when `n == m` (square), where transposition does not change dimensions; when `n == 1` (single row) or `m == 1` (single column), the transpose becomes a single column or single row respectively. Ensure no out-of-bounds access by using the loop bounds derived from the original dimensions. Time complexity is O(n*m), as every element is visited exactly once. Space complexity is O(n*m) for the output matrix, excluding the input which is passed by const reference.
