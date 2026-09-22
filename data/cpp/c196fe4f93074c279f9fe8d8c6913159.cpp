// Write a C++ function `bool isSymmetric(const double matrix[][4], int side)` that checks whether a square matrix of doubles (with a fixed column count of 4, but a variable `side` rows/columns where `side <= 4`) is symmetric along its main diagonal (i.e., `matrix[i][j] == matrix[j][i]` for all valid `i, j`). The function must handle matrices of any size up to 4×4, return `true` if symmetric, `false` otherwise, and use `const` correctness. Do not modify the input matrix. Your implementation should not rely on global variables or external libraries beyond standard headers.

#include <cassert>

int main() {
    double m1[][4] = {{1, 2, 3, 4}, {2, 2, 3, 1}, {3, 3, 3, 2}, {4, 1, 2, 4}};
    assert(isSymmetric(m1, 4) == false);

    double m2[][4] = {{1, 2, 3, 4}, {2, 2, 3, 1}, {3, 3, 3, 2}, {4, 1, 2, 4}};
    assert(isSymmetric(m2, 1) == true); // 1x1 always symmetric

    double m3[][4] = {{1, 2, 3}, {2, 4, 5}, {3, 5, 6}};
    assert(isSymmetric(m3, 3) == true);

    double m4[][4] = {{1, 2, 3}, {2, 4, 5}, {3, 5, 6}};
    assert(isSymmetric(m4, 2) == true); // top-left 2x2 submatrix is symmetric

    double m5[][4] = {{1, 2}, {3, 4}};
    assert(isSymmetric(m5, 2) == false);

    double m6[][4] = {{0}};
    assert(isSymmetric(m6, 1) == true);

    double m7[][4] = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};
    assert(isSymmetric(m7, 3) == true);
}

#include <cstddef> // for size_t

// Check whether the top-left 'side' x 'side' submatrix of a 4-column matrix is symmetric.
bool isSymmetric(const double matrix[][4], int side) {
    for (int i = 0; i < side; ++i) {
        for (int j = 0; j < side; ++j) {
            if (matrix[i][j] != matrix[j][i]) {
                return false;
            }
        }
    }
    return true;
}

// The main algorithm is a double loop over all row/column indices from `0` to `side-1`. For each pair `(i, j)`, compare `matrix[i][j]` with `matrix[j][i]`. If any pair differs, return `false` immediately because symmetry is violated; otherwise, after all pairs are checked, return `true`. Edge cases: `side = 1` (any 1×1 matrix is symmetric), `side = 0` (empty matrix, typically treat as symmetric), and non-square input (the function assumes `side` is the number of rows and columns, and since the fixed column count is 4, it’s up to the caller to pass correct `side`). Floating-point equality is exact; for this task we assume exact representation or values that compare equal exactly. The time complexity is O(side²) because we visit every element of the square submatrix; auxiliary space is O(1) because we only use a few loop variables.
