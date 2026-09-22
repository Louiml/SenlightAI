// Write a C++ function that takes a non-empty matrix represented as a `std::vector<std::vector<int>>` along with its row count `n` and column count `m`, and rotates each boundary layer of the matrix clockwise by one position. For example, the top-left element moves to the second column in the top row, the top row elements shift right, the right column elements shift down, the bottom row elements shift left, and the left column elements shift up. Only the outermost "ring" (if more than one row and column) is shifted by one step, then the next inner ring (if any) is likewise shifted, and so on. The matrix dimensions are fixed (do not transpose or resize); only the values within each layer are permuted. The function should modify the input matrix in-place and return `void`. You may assume `n >= 1` and `m >= 1`, and that all vectors have exactly `m` elements. If `n == 1` or `m == 1`, the matrix remains unchanged (since a single row or column cannot have a ring). Edge cases include rectangular matrices where the number of rows and columns differ, and matrices where the inner ring has only one row or one column after trimming.

// The algorithm processes the matrix layer by layer. Define four boundaries: `top` (starting at 0), `bottom` (starting at `n-1`), `left` (starting at 0), and `right` (starting at `m-1`). While `top < bottom` and `left < right`, perform a clockwise rotation of the current ring by one step. To do this, store the value at `mat[top][left]` in a temporary variable. Then, shift the elements along the top edge (from left+1 to right) to the left by one, using a sequence of `swap` operations: for each column `c` from `left+1` to `right`, swap the current temporary with `mat[top][c]`. This moves the top row elements right, because the swapped value progressively moves into the temporary. Next, shift the right edge downward: for each row `r` from `top+1` to `bottom`, swap temporary with `mat[r][right]`. Then shift the bottom edge leftward: for each column `c` from `right-1` down to `left`, swap temporary with `mat[bottom][c]`. Finally, shift the left edge upward: for each row `r` from `bottom-1` down to `top`, swap temporary with `mat[r][left]`. After completing the ring, shrink the boundaries: `top++`, `left++`, `bottom--`, `right--`, and repeat for the next inner layer.  
// Edge cases: If `n == 1` or `m == 1`, the loop condition `top < bottom && left < right` is false immediately, so no changes occur. For rectangular matrices, e.g., 2×3, the outer ring is rotated, and after shrinking, `top == bottom` or `left == right` stops further processing. The algorithm uses only a constant amount of extra space (one temporary variable) and visits each element a constant number of times (each element in a ring is swapped exactly once per layer, and there are `min(n,m)/2` layers). Time complexity is `O(n*m)`, and space complexity is `O(1)`.

#include <vector>
#include <utility> // for std::swap

// Rotate each boundary layer of a non-empty matrix clockwise by one position.
// The matrix is modified in-place. Assumes mat has n rows and m columns.
void rotateMatrixLayerOneStep(std::vector<std::vector<int>>& mat, int n, int m) {
    int top = 0;
    int bottom = n - 1;
    int left = 0;
    int right = m - 1;

    while (top < bottom && left < right) {
        // Store the value that will wrap around the ring.
        int tmp = mat[top][left];

        // Shift top row elements right (from left+1 to right).
        for (int c = left + 1; c <= right; ++c) {
            std::swap(mat[top][c], tmp);
        }

        // Shift right column elements down (from top+1 to bottom).
        for (int r = top + 1; r <= bottom; ++r) {
            std::swap(mat[r][right], tmp);
        }

        // Shift bottom row elements left (from right-1 down to left).
        for (int c = right - 1; c >= left; --c) {
            std::swap(mat[bottom][c], tmp);
        }

        // Shift left column elements up (from bottom-1 down to top).
        for (int r = bottom - 1; r >= top; --r) {
            std::swap(mat[r][left], tmp);
        }

        // Move to the next inner layer.
        ++top;
        ++left;
        --bottom;
        --right;
    }
}

#include <cassert>
#include <vector>

// (Solution function declaration is assumed from the solution section above.)

int main() {
    // Test 1: 3x3 matrix, outer ring rotates clockwise.
    std::vector<std::vector<int>> m1 = {{1,2,3},{4,5,6},{7,8,9}};
    rotateMatrixLayerOneStep(m1, 3, 3);
    std::vector<std::vector<int>> expected1 = {{4,1,2},{7,5,3},{8,9,6}};
    assert(m1 == expected1);

    // Test 2: 2x2 matrix, all elements rotate one step.
    std::vector<std::vector<int>> m2 = {{1,2},{3,4}};
    rotateMatrixLayerOneStep(m2, 2, 2);
    std::vector<std::vector<int>> expected2 = {{3,1},{4,2}};
    assert(m2 == expected2);

    // Test 3: 1x5 matrix (single row) — unchanged.
    std::vector<std::vector<int>> m3 = {{10,20,30,40,50}};
    rotateMatrixLayerOneStep(m3, 1, 5);
    std::vector<std::vector<int>> expected3 = {{10,20,30,40,50}};
    assert(m3 == expected3);

    // Test 4: 4x1 matrix (single column) — unchanged.
    std::vector<std::vector<int>> m4 = {{1},{2},{3},{4}};
    rotateMatrixLayerOneStep(m4, 4, 1);
    std::vector<std::vector<int>> expected4 = {{1},{2},{3},{4}};
    assert(m4 == expected4);

    // Test 5: 3x4 rectangular matrix, two layers (outer and inner 1x2).
    std::vector<std::vector<int>> m5 = {
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9,10,11,12}
    };
    rotateMatrixLayerOneStep(m5, 3, 4);
    std::vector<std::vector<int>> expected5 = {
        {5, 1, 2, 3},
        {9, 7, 6, 4},
        {10,11,12,8}
    };
    assert(m5 == expected5);

    // Test 6: 2x3 matrix (only one layer).
    std::vector<std::vector<int>> m6 = {{1,2,3},{4,5,6}};
    rotateMatrixLayerOneStep(m6, 2, 3);
    std::vector<std::vector<int>> expected6 = {{4,1,2},{6,5,3}};
    assert(m6 == expected6);

    // Test 7: 5x5 matrix with distinct values, check inner ring also shifts.
    std::vector<std::vector<int>> m7 = {
        {1, 2, 3, 4, 5},
        {6, 7, 8, 9, 10},
        {11,12,13,14,15},
        {16,17,18,19,20},
        {21,22,23,24,25}
    };
    rotateMatrixLayerOneStep(m7, 5, 5);
    std::vector<std::vector<int>> expected7 = {
        {6, 1, 2, 3, 4},
        {11,12,7, 8, 5},
        {16,17,13,9, 10},
        {21,18,19,14,15},
        {22,23,24,25,20}
    };
    assert(m7 == expected7);

    // Test 8: 1x1 matrix (single element) — unchanged.
    std::vector<std::vector<int>> m8 = {{42}};
    rotateMatrixLayerOneStep(m8, 1, 1);
    std::vector<std::vector<int>> expected8 = {{42}};
    assert(m8 == expected8);

    return 0;
}
