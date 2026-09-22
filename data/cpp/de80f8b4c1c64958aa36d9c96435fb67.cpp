Write a C++ function `int** fillSpiral(int rows, int cols)` that dynamically allocates a 2D array with the given number of rows and columns, fills it with consecutive positive integers starting from 1 in a clockwise spiral order (starting from the top-left corner and moving right along the top row, then down the right column, then left along the bottom row, then up the left column, repeating inward), and returns the pointer to the array. The function must handle any positive `rows` and `cols` values, including non-square (rectangular) matrices where the spiral may end prematurely on one side. The caller is responsible for freeing the allocated memory. The function should not print anything.
The solution simulates spiral traversal using four directional loops inside a `while` loop that continues until all `rows * cols` cells are filled. We maintain a counter `value` starting at 1, and layer index `layer` starting at 0. Each layer consists of: (1) fill the top row from left to right for columns `layer` to `cols-layer-1`; (2) fill the right column from top+1 down to `rows-layer-1` (the row below the top row down to the bottom row of the layer); (3) fill the bottom row from right-1 down to `layer` (only if there is a bottom row distinct from the top row, i.e., when `rows-layer-1 != layer`); (4) fill the left column from bottom-1 up to `layer+1` (only if there is a left column distinct from the right column, i.e., when `cols-layer-1 != layer`). After each placement, increment the value counter and a total placed counter. After finishing the four sides, increment `layer` and continue. Edge cases: single-row matrix (only the top row is filled, then the while loop ends because all cells are placed); single-column matrix (only the top and bottom loops execute, but the bottom loop may be skipped if the row is the same; the left column loop then fills upward); matrices that are not square will terminate earlier on one dimension; the algorithm naturally handles this by checking the total placed count and by guarding against overlapping rows or columns in the middle steps. Time complexity is O(rows * cols) because each cell is filled exactly once. Space complexity is O(rows * cols) for the matrix itself, plus O(1) auxiliary for the counters.
#include <cstddef>

// Fills a dynamically allocated 2D array with numbers 1 to rows*cols in clockwise spiral order.
// The caller must free the memory with delete[] for each row and then delete[] the array of pointers.
int** fillSpiral(int rows, int cols) {
    int** matrix = new int*[rows];
    for (int i = 0; i < rows; ++i) {
        matrix[i] = new int[cols];
    }

    int value = 1;
    int total = rows * cols;
    int layer = 0;

    while (value <= total) {
        // Top row: left to right
        for (int j = layer; j < cols - layer && value <= total; ++j) {
            matrix[layer][j] = value++;
        }

        // Right column: top+1 to bottom
        for (int i = layer + 1; i < rows - layer && value <= total; ++i) {
            matrix[i][cols - layer - 1] = value++;
        }

        // Bottom row: right-1 to left (only if distinct from top row)
        if (rows - layer - 1 > layer) {
            for (int j = cols - layer - 2; j >= layer && value <= total; --j) {
                matrix[rows - layer - 1][j] = value++;
            }
        }

        // Left column: bottom-1 up to top+1 (only if distinct from right column)
        if (cols - layer - 1 > layer) {
            for (int i = rows - layer - 2; i > layer && value <= total; --i) {
                matrix[i][layer] = value++;
            }
        }

        ++layer;
    }

    return matrix;
}
#include <cassert>

int main() {
    // 1x1 matrix
    int** m1 = fillSpiral(1, 1);
    assert(m1[0][0] == 1);
    delete[] m1[0];
    delete[] m1;

    // 1x4 matrix (single row)
    int** m2 = fillSpiral(1, 4);
    assert(m2[0][0] == 1 && m2[0][1] == 2 && m2[0][2] == 3 && m2[0][3] == 4);
    delete[] m2[0];
    delete[] m2;

    // 4x1 matrix (single column)
    int** m3 = fillSpiral(4, 1);
    assert(m3[0][0] == 1 && m3[1][0] == 2 && m3[2][0] == 3 && m3[3][0] == 4);
    for (int i = 0; i < 4; ++i) delete[] m3[i];
    delete[] m3;

    // 3x3 matrix
    int** m4 = fillSpiral(3, 3);
    assert(m4[0][0] == 1 && m4[0][1] == 2 && m4[0][2] == 3);
    assert(m4[1][2] == 4 && m4[2][2] == 5 && m4[2][1] == 6);
    assert(m4[2][0] == 7 && m4[1][0] == 8 && m4[1][1] == 9);
    for (int i = 0; i < 3; ++i) delete[] m4[i];
    delete[] m4;

    // 3x4 matrix (rectangular)
    int** m5 = fillSpiral(3, 4);
    assert(m5[0][0] == 1 && m5[0][1] == 2 && m5[0][2] == 3 && m5[0][3] == 4);
    assert(m5[1][3] == 5 && m5[2][3] == 6 && m5[2][2] == 7 && m5[2][1] == 8);
    assert(m5[2][0] == 9 && m5[1][0] == 10 && m5[1][1] == 11 && m5[1][2] == 12);
    for (int i = 0; i < 3; ++i) delete[] m5[i];
    delete[] m5;

    // 4x3 matrix (rectangular, more rows than cols)
    int** m6 = fillSpiral(4, 3);
    assert(m6[0][0] == 1 && m6[0][1] == 2 && m6[0][2] == 3);
    assert(m6[1][2] == 4 && m6[2][2] == 5 && m6[3][2] == 6);
    assert(m6[3][1] == 7 && m6[3][0] == 8 && m6[2][0] == 9);
    assert(m6[1][0] == 10 && m6[1][1] == 11 && m6[2][1] == 12);
    for (int i = 0; i < 4; ++i) delete[] m6[i];
    delete[] m6;

    return 0;
}
