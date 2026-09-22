Write a C++ function that takes a 2D vector of integers (with at least one row and one column) and rotates the elements of each rectangular "ring" (or boundary layer) of the matrix clockwise by one position. The rotation applies to every layer from the outermost ring inward, and the inner elements that do not form a full ring (when the remaining submatrix is a single row or column) remain unchanged. The function should modify the matrix in place and return void. For example, a 3×4 matrix with values 1–12 arranged row‑wise becomes a matrix where each element moves one step along its ring in the clockwise direction: the top row shifts right, the rightmost column shifts down, the bottom row shifts left, and the leftmost column shifts up, with the element that "falls off" the top‑right corner wrapping to the bottom‑left corner of that ring.
The solution processes the matrix ring by ring from the outermost layer inward. For each ring, we identify the current boundaries: starting row (`rowStart`), starting column (`colStart`), ending row (`n-1`), and ending column (`m-1`). If the ring is a single row or single column (i.e., `rowStart >= n-1` or `colStart >= m-1`), no rotation is possible and we break out of the loop. Otherwise, we store the element that will move into the first cell of the ring: the element just below the top‑left corner (`mat[rowStart+1][colStart]`). Then we traverse the four sides of the ring in clockwise order: first the top row from left to right, then the rightmost column from top to bottom, then the bottom row from right to left, and finally the leftmost column from bottom to top. During each traversal, we swap the stored `previous` value with the current cell's value, effectively shifting each element one step clockwise. After completing a ring, we shrink the boundaries by incrementing `rowStart` and `colStart` and decrementing `n` and `m`. The loop continues until no full ring remains. Time complexity is O(N*M) because each element is visited exactly once, and space complexity is O(1) auxiliary, aside from the input matrix itself. Edge cases include matrices with only one row or one column (no rotation), and matrices with an odd number of rows/columns where the innermost single cell is left unchanged.
#include <vector>
#include <cstddef>

// Rotate each rectangular ring of the matrix clockwise by one position.
void rotateMatrixClockwise(std::vector<std::vector<int>>& mat) {
    if (mat.empty() || mat[0].empty()) return;

    std::size_t rowStart = 0;
    std::size_t colStart = 0;
    std::size_t rowEnd = mat.size() - 1;      // inclusive
    std::size_t colEnd = mat[0].size() - 1;   // inclusive

    while (rowStart < rowEnd && colStart < colEnd) {
        int previous = mat[rowStart + 1][colStart];
        int current;

        // Move top row from left to right (excluding the last cell of the ring).
        for (std::size_t i = colStart; i < colEnd; ++i) {
            current = mat[rowStart][i];
            mat[rowStart][i] = previous;
            previous = current;
        }

        // Move right column from top to bottom (excluding the last cell of the ring).
        for (std::size_t i = rowStart; i < rowEnd; ++i) {
            current = mat[i][colEnd];
            mat[i][colEnd] = previous;
            previous = current;
        }

        // Move bottom row from right to left (excluding the last cell of the ring).
        for (std::size_t i = colEnd; i > colStart; --i) {
            current = mat[rowEnd][i];
            mat[rowEnd][i] = previous;
            previous = current;
        }

        // Move left column from bottom to top (excluding the last cell of the ring).
        for (std::size_t i = rowEnd; i > rowStart; --i) {
            current = mat[i][colStart];
            mat[i][colStart] = previous;
            previous = current;
        }

        ++rowStart;
        ++colStart;
        --rowEnd;
        --colEnd;
    }
}
#include <cassert>
#include <vector>

// The solution function is declared above (or included here).
int main() {
    // Example: 3x4 matrix
    std::vector<std::vector<int>> mat1 = {
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 10, 11, 12}
    };
    rotateMatrixClockwise(mat1);
    std::vector<std::vector<int>> expected1 = {
        {5, 1, 2, 3},
        {9, 6, 7, 4},
        {10, 11, 12, 8}
    };
    assert(mat1 == expected1);

    // Single row: no change
    std::vector<std::vector<int>> mat2 = {{1, 2, 3}};
    rotateMatrixClockwise(mat2);
    assert(mat2 == std::vector<std::vector<int>>{{1, 2, 3}});

    // Single column: no change
    std::vector<std::vector<int>> mat3 = {{1}, {2}, {3}};
    rotateMatrixClockwise(mat3);
    assert(mat3 == std::vector<std::vector<int>>{{1}, {2}, {3}});

    // 2x2 matrix
    std::vector<std::vector<int>> mat4 = {{1, 2}, {3, 4}};
    rotateMatrixClockwise(mat4);
    assert(mat4 == std::vector<std::vector<int>>{{3, 1}, {4, 2}});

    // 3x3 matrix (single ring then inner single cell unchanged)
    std::vector<std::vector<int>> mat5 = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };
    rotateMatrixClockwise(mat5);
    std::vector<std::vector<int>> expected5 = {
        {4, 1, 2},
        {7, 5, 3},
        {8, 9, 6}
    };
    assert(mat5 == expected5);

    // 4x4 matrix with two rings
    std::vector<std::vector<int>> mat6 = {
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 10, 11, 12},
        {13, 14, 15, 16}
    };
    rotateMatrixClockwise(mat6);
    std::vector<std::vector<int>> expected6 = {
        {5, 1, 2, 3},
        {9, 10, 6, 4},
        {13, 11, 7, 8},
        {14, 15, 16, 12}
    };
    assert(mat6 == expected6);

    // Empty matrix: no crash
    std::vector<std::vector<int>> mat7;
    rotateMatrixClockwise(mat7);
    assert(mat7.empty());

    // Single element
    std::vector<std::vector<int>> mat8 = {{42}};
    rotateMatrixClockwise(mat8);
    assert(mat8 == std::vector<std::vector<int>>{{42}});

    return 0;
}
