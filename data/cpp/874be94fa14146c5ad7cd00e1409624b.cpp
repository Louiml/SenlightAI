// Write a C++ function `reverseRows` that takes a 2D integer matrix represented as a fixed-size 2D array with 10 columns and 10 rows (matching the style of the original snippet), along with the actual number of rows and columns, and reverses the order of the rows in-place. For example, given a 4×4 matrix, the first row becomes the last, the second becomes the second-to-last, and so on. The function must modify the original matrix directly and not return anything. The matrix dimensions (iRow and iCol) will always be between 1 and 10 inclusive, and you may assume the input is valid. You are to provide only the free function (no `main`), with appropriate `const` correctness where applicable for helper functions if needed, but the main `reverseRows` function must be non-const because it modifies the array.
The approach is straightforward: to reverse the rows of a matrix, swap the first row with the last, the second with the second-to-last, and so on, for the first half of the rows (i.e., iterate `i` from 0 to `iRow/2`). For each row index `i`, swap the entire row at index `i` with the row at index `iRow-1-i`. Swapping entire rows can be done element-by-element using a nested loop over columns. Since the matrix is stored as a 2D array with fixed column size 10, we only need to process the first `iCol` columns for each row. Edge cases: if `iRow` is odd, the middle row stays unchanged; if `iRow` is 1, nothing is swapped. The time complexity is O(iRow × iCol) because for each pair of rows we swap up to iCol elements, and there are about iRow/2 pairs. The space complexity is O(1) as we only use a temporary variable for swapping.
#include <utility>  // for std::swap (optional, can use manual temp)

// Reverses the order of rows in a fixed-size 2D array (10 columns).
// The matrix has iRow actual rows and iCol actual columns.
// The function modifies the array in-place.
void reverseRows(int Arr[][10], int iRow, int iCol) {
    for (int i = 0; i < iRow / 2; ++i) {
        for (int j = 0; j < iCol; ++j) {
            // Swap element at (i,j) with element at (iRow-1-i, j)
            int temp = Arr[i][j];
            Arr[i][j] = Arr[iRow - 1 - i][j];
            Arr[iRow - 1 - i][j] = temp;
        }
    }
}
#include <cassert>

int main() {
    // Test 1: 4x4 matrix from the problem statement
    int mat1[10][10] = {
        {3, 2, 9, 7},
        {4, 3, 2, 2},
        {8, 4, 1, 5},
        {6, 9, 7, 5}
    };
    reverseRows(mat1, 4, 4);
    int expected1[10][10] = {
        {6, 9, 7, 5},
        {8, 4, 1, 5},
        {4, 3, 2, 2},
        {3, 2, 9, 7}
    };
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            assert(mat1[i][j] == expected1[i][j]);

    // Test 2: Odd number of rows (3x2)
    int mat2[10][10] = {{1, 2}, {3, 4}, {5, 6}};
    reverseRows(mat2, 3, 2);
    int expected2[10][10] = {{5, 6}, {3, 4}, {1, 2}};
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 2; ++j)
            assert(mat2[i][j] == expected2[i][j]);

    // Test 3: Single row (1x5) – no change
    int mat3[10][10] = {{7, 8, 9, 10, 11}};
    reverseRows(mat3, 1, 5);
    assert(mat3[0][0] == 7 && mat3[0][4] == 11);

    // Test 4: Two rows (2x3)
    int mat4[10][10] = {{1, 2, 3}, {4, 5, 6}};
    reverseRows(mat4, 2, 3);
    assert(mat4[0][0] == 4 && mat4[0][2] == 6 && mat4[1][0] == 1 && mat4[1][2] == 3);

    // Test 5: All zeros (4x4) – no change but verify all remain zero
    int mat5[10][10] = {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}};
    reverseRows(mat5, 4, 4);
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            assert(mat5[i][j] == 0);

    return 0;
}
