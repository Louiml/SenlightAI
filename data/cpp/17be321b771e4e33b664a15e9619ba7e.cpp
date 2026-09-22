Write a C++ function named `isValidSudoku` that takes a 9×9 vector of vectors of integers representing a partially filled Sudoku board, where empty cells are denoted by `0` and filled cells contain values from `1` to `9`. The function must return `true` if the board is a valid partially filled Sudoku configuration, meaning that no row, column, or 3×3 sub-grid contains duplicate non-zero values, and `false` otherwise. The function should not modify the input board; use `const` references and work on a copy if needed. The board is guaranteed to be exactly 9×9, and values outside `0`–`9` will not appear, but the solution must handle any valid input within that range efficiently.
#include <cassert>
#include <vector>

// Function under test (declaration included for completeness)
bool isValidSudoku(const std::vector<std::vector<int>>& board);

int main() {
    // Valid empty board (all zeros)
    std::vector<std::vector<int>> empty(9, std::vector<int>(9, 0));
    assert(isValidSudoku(empty) == true);
    
    // Fully valid solved board (standard example)
    std::vector<std::vector<int>> valid = {
        {5,3,4,6,7,8,9,1,2},
        {6,7,2,1,9,5,3,4,8},
        {1,9,8,3,4,2,5,6,7},
        {8,5,9,7,6,1,4,2,3},
        {4,2,6,8,5,3,7,9,1},
        {7,1,3,9,2,4,8,5,6},
        {9,6,1,5,3,7,2,8,4},
        {2,8,7,4,1,9,6,3,5},
        {3,4,5,2,8,6,1,7,9}
    };
    assert(isValidSudoku(valid) == true);
    
    // Invalid: duplicate 5 in first row
    std::vector<std::vector<int>> dupRow = valid;
    dupRow[0][8] = 5;
    assert(isValidSudoku(dupRow) == false);
    
    // Invalid: duplicate 9 in first column
    std::vector<std::vector<int>> dupCol = valid;
    dupCol[8][0] = 5; // change to 5? Wait, need duplicate of an existing value e.g., column 0 has 5 at row 0, so set row 1 col 0 to 5
    dupCol[1][0] = 5;
    assert(isValidSudoku(dupCol) == false);
    
    // Invalid: duplicate 6 in top-left 3x3 block
    std::vector<std::vector<int>> dupBlock = valid;
    dupBlock[1][1] = 5; // top-left block already has 5 at (0,0), so set (1,1) to 5
    assert(isValidSudoku(dupBlock) == false);
    
    // Partially filled valid board (some zeros)
    std::vector<std::vector<int>> partial = {
        {5,3,0,0,7,0,0,0,0},
        {6,0,0,1,9,5,0,0,0},
        {0,9,8,0,0,0,0,6,0},
        {8,0,0,0,6,0,0,0,3},
        {4,0,0,8,0,3,0,0,1},
        {7,0,0,0,2,0,0,0,6},
        {0,6,0,0,0,0,2,8,0},
        {0,0,0,4,1,9,0,0,5},
        {0,0,0,0,8,0,0,7,9}
    };
    assert(isValidSudoku(partial) == true);
    
    // Invalid due to duplicate in a partially filled board's row
    std::vector<std::vector<int>> partialDup = partial;
    partialDup[0][4] = 3; // row 0 already has 3 at (0,1) -> duplicate
    assert(isValidSudoku(partialDup) == false);
    
    return 0;
}
#include <vector>

// Check if a 9x9 Sudoku board is valid (no duplicates in rows, columns, or 3x3 blocks).
// Empty cells are represented by 0. Input is not modified.
bool isValidSudoku(const std::vector<std::vector<int>>& board) {
    // rowSeen[r][d] = true if digit d (1-9) appears in row r
    // colSeen[c][d] = true if digit d appears in column c
    // blockSeen[b][d] = true if digit d appears in 3x3 block b
    bool rowSeen[9][10] = {false};
    bool colSeen[9][10] = {false};
    bool blockSeen[9][10] = {false};
    
    for (int r = 0; r < 9; ++r) {
        for (int c = 0; c < 9; ++c) {
            int val = board[r][c];
            if (val == 0) continue; // empty cell, skip
            
            // Block index: 0-8, grouping rows and columns into 3x3 regions
            int block = (r / 3) * 3 + (c / 3);
            
            // Check if digit already seen in any of the three contexts
            if (rowSeen[r][val] || colSeen[c][val] || blockSeen[block][val]) {
                return false;
            }
            
            // Mark digit as seen
            rowSeen[r][val] = true;
            colSeen[c][val] = true;
            blockSeen[block][val] = true;
        }
    }
    return true;
}
// The core idea is to check for duplicates in three independent dimensions: rows, columns, and 3×3 blocks. A straightforward approach uses three boolean arrays (or a boolean 2D map) to mark which digits have already been seen in each row, column, and block. Iterate over every cell: if the cell is non-zero, check the digit (1–9) and mark it in the corresponding row, column, and block tracker. If any of these trackers already contains that digit, return `false` immediately. This ensures that duplicates are detected as soon as they occur. Edge cases include an empty board (all zeros), which is trivially valid, and boards with all valid filled cells. The time complexity is \(O(9 \times 9) = O(81)\), which is constant, and space complexity is \(O(9 \times 9) = O(81)\) for the three tracking arrays, also constant. The solution avoids modifying the input by using `const` and internal copies of the tracking structures.
