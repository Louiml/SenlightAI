// Write a C++ function named `setBooleanMatrix` that takes a reference to a non-empty rectangular matrix of integers where each entry is either 0 or 1, and modifies it in place so that if any cell originally contains a 1, then every cell in that cell's entire row and entire column becomes 1. The function must preserve the original dimensions and only change cell values from 0 to 1 (never from 1 to 0). The input matrix may have any number of rows and columns (both at least 1), may contain multiple 1s, and may have all zeros or all ones. The function should be efficient and avoid using extra O(rows*columns) space beyond the original matrix.

// The main idea is to first identify all rows and columns that contain at least one 1, then set all cells in those rows and columns to 1. We can achieve O(1) auxiliary space beyond the input matrix by using the first row and first column as markers. Specifically, we scan the matrix and for every cell (i,j) that has value 1, we set `matrix[i][0] = 1` (mark the row) and `matrix[0][j] = 1` (mark the column). However, because the first row and first column are used as markers, we need separate flags to remember if the original first row or first column contained any 1. We first record those two flags, then process the rest of the matrix (starting from row 1, column 1) to set markers. After marking, we iterate over rows 1..n-1 and columns 1..m-1 and set cells to 1 if either its row marker or column marker is 1. Finally, we apply the flags to the first row and first column: if the row flag is true, set the entire first row to 1; if the column flag is true, set the entire first column to 1. Edge cases include single-row or single-column matrices, where the first row and first column are the same; the flags must be handled carefully so that a single cell with 1 correctly propagates. The algorithm runs in O(rows × columns) time and uses O(1) extra space beyond the input matrix.

#include <vector>

// Modify the matrix in place so that if any cell is 1, its entire row and column become 1.
void setBooleanMatrix(std::vector<std::vector<int>>& matrix) {
    int rows = matrix.size();
    int cols = matrix[0].size();
    
    // Flags for the first row and first column.
    bool firstRowHasOne = false;
    bool firstColHasOne = false;
    
    // Check the first row for any 1.
    for (int j = 0; j < cols; ++j) {
        if (matrix[0][j] == 1) {
            firstRowHasOne = true;
            break;
        }
    }
    // Check the first column for any 1.
    for (int i = 0; i < rows; ++i) {
        if (matrix[i][0] == 1) {
            firstColHasOne = true;
            break;
        }
    }
    
    // Use first row and first column as markers.
    for (int i = 1; i < rows; ++i) {
        for (int j = 1; j < cols; ++j) {
            if (matrix[i][j] == 1) {
                matrix[i][0] = 1; // mark row i
                matrix[0][j] = 1; // mark column j
            }
        }
    }
    
    // Set cells to 1 based on markers.
    for (int i = 1; i < rows; ++i) {
        for (int j = 1; j < cols; ++j) {
            if (matrix[i][0] == 1 || matrix[0][j] == 1) {
                matrix[i][j] = 1;
            }
        }
    }
    
    // Handle the first row and first column.
    if (firstRowHasOne) {
        for (int j = 0; j < cols; ++j) {
            matrix[0][j] = 1;
        }
    }
    if (firstColHasOne) {
        for (int i = 0; i < rows; ++i) {
            matrix[i][0] = 1;
        }
    }
}

#include <cassert>
#include <vector>

// Declaration of the function to test.
void setBooleanMatrix(std::vector<std::vector<int>>& matrix);

int main() {
    // Test 1: Simple 2x2 with one 1.
    std::vector<std::vector<int>> m1 = {{0,0},{0,1}};
    setBooleanMatrix(m1);
    assert(m1 == std::vector<std::vector<int>>({{0,1},{1,1}}));
    
    // Test 2: All zeros remain unchanged.
    std::vector<std::vector<int>> m2 = {{0,0,0},{0,0,0}};
    setBooleanMatrix(m2);
    assert(m2 == std::vector<std::vector<int>>({{0,0,0},{0,0,0}}));
    
    // Test 3: All ones become all ones.
    std::vector<std::vector<int>> m3 = {{1,1},{1,1}};
    setBooleanMatrix(m3);
    assert(m3 == std::vector<std::vector<int>>({{1,1},{1,1}}));
    
    // Test 4: Single row with a 1 in the middle.
    std::vector<std::vector<int>> m4 = {{0,1,0}};
    setBooleanMatrix(m4);
    assert(m4 == std::vector<std::vector<int>>({{1,1,1}}));
    
    // Test 5: Single column with a 1 at the bottom.
    std::vector<std::vector<int>> m5 = {{0},{0},{1}};
    setBooleanMatrix(m5);
    assert(m5 == std::vector<std::vector<int>>({{1},{1},{1}}));
    
    // Test 6: 1 in first row and first column separately.
    std::vector<std::vector<int>> m6 = {{1,0,0},{0,0,0}};
    setBooleanMatrix(m6);
    assert(m6 == std::vector<std::vector<int>>({{1,1,1},{1,0,0}}));
    
    // Test 7: 3x3 with two distant 1s.
    std::vector<std::vector<int>> m7 = {{0,0,0},{0,0,0},{0,1,0}};
    setBooleanMatrix(m7);
    assert(m7 == std::vector<std::vector<int>>({{0,1,0},{0,1,0},{1,1,1}}));
    
    // Test 8: 1 in first column and first row.
    std::vector<std::vector<int>> m8 = {{0,0,1},{0,0,0},{1,0,0}};
    setBooleanMatrix(m8);
    assert(m8 == std::vector<std::vector<int>>({{1,0,1},{1,0,1},{1,1,1}}));
    
    // Test 9: Larger matrix, check no unintended 0→1.
    std::vector<std::vector<int>> m9 = {{0,0,0,0},{0,0,1,0},{0,0,0,0}};
    setBooleanMatrix(m9);
    assert(m9 == std::vector<std::vector<int>>({{0,0,1,0},{1,1,1,1},{0,0,1,0}}));
    
    // Test 10: Single element 1.
    std::vector<std::vector<int>> m10 = {{1}};
    setBooleanMatrix(m10);
    assert(m10 == std::vector<std::vector<int>>({{1}}));
    
    return 0;
}
