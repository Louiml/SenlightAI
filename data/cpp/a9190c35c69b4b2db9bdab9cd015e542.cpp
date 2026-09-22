/*
Write a C++ function named `matrixDeterminant` that accepts a square matrix represented as a `std::vector<std::vector<int>>` and returns its determinant as an integer. The determinant must be computed using recursive cofactor expansion (Laplace expansion) along the first row. The function should handle matrices of size 1×1, 2×2, and larger. If the matrix is not square (i.e., the number of rows does not equal the number of columns, or any row has a different length), the function must return 0. The function must be `const`-correct, taking the matrix by `const` reference and not modifying it. Assume the input matrix is non-empty and contains at least one row and one column. Do not use built‑in determinant functions or libraries; implement the expansion manually.
*/
#include <vector>
#include <cstddef>

// Compute the determinant of a square matrix using recursive cofactor expansion.
// Returns 0 for non-square matrices or inconsistent row lengths.
int matrixDeterminant(const std::vector<std::vector<int>>& mat) {
    const std::size_t n = mat.size();
    if (n == 0) return 0;

    // Check square and consistent row lengths
    for (const auto& row : mat) {
        if (row.size() != n) return 0;
    }

    // Base cases
    if (n == 1) return mat[0][0];
    if (n == 2) return mat[0][0] * mat[1][1] - mat[0][1] * mat[1][0];

    int det = 0;
    for (std::size_t j = 0; j < n; ++j) {
        // Build submatrix by removing first row and column j
        std::vector<std::vector<int>> sub;
        sub.reserve(n - 1);
        for (std::size_t r = 1; r < n; ++r) {
            std::vector<int> row;
            row.reserve(n - 1);
            for (std::size_t c = 0; c < n; ++c) {
                if (c != j) row.push_back(mat[r][c]);
            }
            sub.push_back(std::move(row));
        }
        // Sign alternates starting with + for j=0
        int sign = (j % 2 == 0) ? 1 : -1;
        det += sign * mat[0][j] * matrixDeterminant(sub);
    }
    return det;
}
#include <cassert>
#include <vector>

// Declaration for testing
int matrixDeterminant(const std::vector<std::vector<int>>& mat);

int main() {
    // 1x1 matrix
    assert(matrixDeterminant({{5}}) == 5);
    assert(matrixDeterminant({{-3}}) == -3);

    // 2x2 matrix
    assert(matrixDeterminant({{1, 2}, {3, 4}}) == -2);
    assert(matrixDeterminant({{2, 0}, {0, 5}}) == 10);
    assert(matrixDeterminant({{7, 7}, {7, 7}}) == 0);

    // 3x3 matrices
    assert(matrixDeterminant({{1, 2, 3}, {4, 5, 6}, {7, 8, 9}}) == 0);
    assert(matrixDeterminant({{6, 1, 1}, {4, -2, 5}, {2, 8, 7}}) == -306);
    assert(matrixDeterminant({{0, 0, 0}, {1, 2, 3}, {4, 5, 6}}) == 0);

    // 4x4 matrix
    assert(matrixDeterminant({{1, 0, 0, 0}, {0, 2, 0, 0}, {0, 0, 3, 0}, {0, 0, 0, 4}}) == 24);

    // Non-square or inconsistent matrices
    assert(matrixDeterminant({{1, 2}, {3, 4}, {5, 6}}) == 0);
    assert(matrixDeterminant({{1, 2, 3}, {4, 5, 6}}) == 0);
    assert(matrixDeterminant({{1, 2}, {3, 4, 5}}) == 0); // inconsistent row length
}
// The solution recursively computes the determinant using cofactor expansion along the first row. Base cases: for a 1×1 matrix, the determinant is its single element; for a 2×2 matrix, use the formula `ad - bc`. For larger matrices, iterate over each column `j` of the first row, compute the sign using `(j % 2 == 0 ? 1 : -1)` (since first row index is 0), build the submatrix by removing the first row and the `j`-th column, and add `sign * mat[0][j] * determinant(submatrix)` to the total. Edge cases: non‑square matrices or inconsistent row lengths → return 0; very large determinants might overflow `int`, but the task specifies returning `int`, so we assume inputs remain within range. Time complexity: for an n×n matrix, the recurrence is T(n) = n·T(n-1) + O(n²) for submatrix construction, yielding O(n!) time and O(n²) auxiliary space for the recursive call stack and submatrices. For typical small matrices (n ≤ 8), this is acceptable.
