Write a C++ function `int kthSpiralElement(int matrix[][100], int rows, int cols, int k)` that returns the `k`-th element (1-indexed) when traversing a rectangular integer matrix in clockwise spiral order starting from the top-left corner (row 0, column 0). The matrix has dimensions `rows` × `cols` with both dimensions at least 1 and at most 100. Assume `k` is always valid (1 ≤ k ≤ rows*cols). The spiral traversal moves right across the top row, then down the right column, then left across the bottom row, then up the left column, then repeats inward. The function must handle matrices where rows and columns may be equal or different, including single-row or single-column matrices, and must operate directly on the given 2D array (use fixed-size second dimension `100` as in the signature). Return the integer value found at the `k`-th position in that order.
#include <cassert>

int main() {
    // Test 1: 3x3 matrix
    int m1[3][100] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    assert(kthSpiralElement(m1, 3, 3, 1) == 1);
    assert(kthSpiralElement(m1, 3, 3, 2) == 2);
    assert(kthSpiralElement(m1, 3, 3, 3) == 3);
    assert(kthSpiralElement(m1, 3, 3, 4) == 6);
    assert(kthSpiralElement(m1, 3, 3, 5) == 9);
    assert(kthSpiralElement(m1, 3, 3, 6) == 8);
    assert(kthSpiralElement(m1, 3, 3, 7) == 7);
    assert(kthSpiralElement(m1, 3, 3, 8) == 4);
    assert(kthSpiralElement(m1, 3, 3, 9) == 5);

    // Test 2: single row
    int m2[1][100] = {{10, 20, 30}};
    assert(kthSpiralElement(m2, 1, 3, 1) == 10);
    assert(kthSpiralElement(m2, 1, 3, 2) == 20);
    assert(kthSpiralElement(m2, 1, 3, 3) == 30);

    // Test 3: single column
    int m3[3][100] = {{5}, {6}, {7}};
    assert(kthSpiralElement(m3, 3, 1, 1) == 5);
    assert(kthSpiralElement(m3, 3, 1, 2) == 6);
    assert(kthSpiralElement(m3, 3, 1, 3) == 7);

    // Test 4: 2x4 matrix (row-major)
    int m4[2][100] = {{1, 2, 3, 4}, {5, 6, 7, 8}};
    // Spiral: 1,2,3,4,8,7,6,5
    assert(kthSpiralElement(m4, 2, 4, 5) == 8);
    assert(kthSpiralElement(m4, 2, 4, 6) == 7);
    assert(kthSpiralElement(m4, 2, 4, 8) == 5);

    // Test 5: 4x2 matrix
    int m5[4][100] = {{1, 2}, {3, 4}, {5, 6}, {7, 8}};
    // Spiral: 1,2,4,6,8,7,5,3
    assert(kthSpiralElement(m5, 4, 2, 1) == 1);
    assert(kthSpiralElement(m5, 4, 2, 3) == 4);
    assert(kthSpiralElement(m5, 4, 2, 5) == 8);
    assert(kthSpiralElement(m5, 4, 2, 7) == 5);

    // Test 6: 1x1 matrix
    int m6[1][100] = {{42}};
    assert(kthSpiralElement(m6, 1, 1, 1) == 42);

    // Test 7: larger 4x4 matrix
    int m7[4][100] = {{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}, {13, 14, 15, 16}};
    // Spiral: 1,2,3,4,8,12,16,15,14,13,9,5,6,7,11,10
    assert(kthSpiralElement(m7, 4, 4, 10) == 13);
    assert(kthSpiralElement(m7, 4, 4, 13) == 6);
    assert(kthSpiralElement(m7, 4, 4, 16) == 10);

    return 0;
}
#include <assert.h>
#include <cstddef>

// Return the k-th element (1-indexed) in clockwise spiral order from matrix[0][0].
// matrix is a rows x cols array with cols <= 100 fixed dimension.
int kthSpiralElement(int matrix[][100], int rows, int cols, int k) {
    int startRow = 0;
    int endRow = rows - 1;
    int startCol = 0;
    int endCol = cols - 1;

    while (k > 0) {
        // Traverse top row left -> right
        for (int j = startCol; j <= endCol; ++j) {
            --k;
            if (k == 0) return matrix[startRow][j];
        }
        ++startRow;

        // Traverse right column top -> bottom
        for (int i = startRow; i <= endRow; ++i) {
            --k;
            if (k == 0) return matrix[i][endCol];
        }
        --endCol;

        // Traverse bottom row right -> left (only if still valid)
        if (startRow <= endRow) {
            for (int j = endCol; j >= startCol; --j) {
                --k;
                if (k == 0) return matrix[endRow][j];
            }
            --endRow;
        }

        // Traverse left column bottom -> top (only if still valid)
        if (startCol <= endCol) {
            for (int i = endRow; i >= startRow; --i) {
                --k;
                if (k == 0) return matrix[i][startCol];
            }
            ++startCol;
        }
    }
    // Should never reach here for valid k, but return a sentinel.
    return -1;
}
// The algorithm uses four boundary variables: `startRow = 0`, `endRow = rows-1`, `startCol = 0`, `endCol = cols-1`. While `k` is positive, we traverse each of the four sides of the current boundary rectangle in order. For each side, iterate through the cells, decrementing `k` each time; if `k` becomes 0, return that cell’s value. After completing a side, shrink the corresponding boundary (e.g., `startRow++` after top row). The loop continues until `k` is exhausted, which always happens because `k` is valid. Edge cases: single row (only the top row is traversed; the left/right/bottom loops will have conditions that immediately fail because `startRow > endRow` or `startCol > endCol`), single column (only the top and right sides produce valid cells, the bottom/left loops fail). Also need to handle when `k` exactly equals the last element of a side; the check `if (k == 0)` after decrement handles that. Time complexity is O(rows*cols) in the worst case because we might traverse all cells, but actually we stop early at `k`, so average is O(k) ≤ O(rows*cols). Space complexity is O(1) beyond the input matrix.
