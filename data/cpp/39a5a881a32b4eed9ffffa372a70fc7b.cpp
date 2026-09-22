Write a C++ function `bool isValidSudoku(const int board[9][9])` that determines whether a given 9x9 Sudoku board (filled with digits 1-9, with 0 representing empty cells) is a valid partially filled Sudoku puzzle, meaning that no row, column, or 3x3 subgrid contains duplicate nonzero digits. The function should return `true` if the board satisfies all Sudoku constraints (ignoring zeros) and `false` otherwise. The input board is a fixed 9x9 array; the function must not modify it and must handle any valid integer entries, including zeros, negative values, or digits outside 1-9 (which should also be treated as invalid since Sudoku only uses 1-9).
#include <cassert>

int main() {
    // Valid fully solved Sudoku
    int solved[9][9] = {
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
    assert(isValidSudoku(solved) == true);

    // All zeros (empty) is valid
    int empty[9][9] = {0};
    assert(isValidSudoku(empty) == true);

    // Duplicate in a row
    int dupRow[9][9] = {0};
    dupRow[0][0] = 1; dupRow[0][1] = 1;
    assert(isValidSudoku(dupRow) == false);

    // Duplicate in a column
    int dupCol[9][9] = {0};
    dupCol[0][0] = 5; dupCol[1][0] = 5;
    assert(isValidSudoku(dupCol) == false);

    // Duplicate in a 3x3 block
    int dupBlock[9][9] = {0};
    dupBlock[0][0] = 7; dupBlock[1][1] = 7;
    assert(isValidSudoku(dupBlock) == false);

    // Value outside 1-9
    int invalidVal[9][9] = {0};
    invalidVal[4][4] = 10;
    assert(isValidSudoku(invalidVal) == false);

    // Valid partial puzzle (no duplicates)
    int partial[9][9] = {0};
    partial[0][0] = 1; partial[1][1] = 2; partial[2][2] = 3;
    partial[3][3] = 4; partial[4][4] = 5; partial[5][5] = 6;
    partial[6][6] = 7; partial[7][7] = 8; partial[8][8] = 9;
    assert(isValidSudoku(partial) == true);

    // Same digit in different blocks but different rows/cols is fine (e.g., spread out)
    int spread[9][9] = {0};
    spread[0][0] = 1; spread[1][4] = 1; spread[2][8] = 1;
    assert(isValidSudoku(spread) == true);

    return 0;
}
#include <array>
#include <cstddef>

// Check if a 9x9 Sudoku board (0 = empty) is valid (no duplicate nonzero digits in any row, column, or 3x3 block).
bool isValidSudoku(const int board[9][9]) {
    std::array<std::array<bool, 10>, 9> rowSeen{};
    std::array<std::array<bool, 10>, 9> colSeen{};
    std::array<std::array<bool, 10>, 9> blockSeen{};

    for (std::size_t i = 0; i < 9; ++i) {
        for (std::size_t j = 0; j < 9; ++j) {
            int val = board[i][j];
            if (val == 0) continue; // empty cell

            // Sudoku digits must be 1 through 9
            if (val < 1 || val > 9) return false;

            int blockIndex = static_cast<int>((i / 3) * 3 + (j / 3));

            if (rowSeen[i][val] || colSeen[j][val] || blockSeen[blockIndex][val])
                return false;

            rowSeen[i][val] = true;
            colSeen[j][val] = true;
            blockSeen[blockIndex][val] = true;
        }
    }
    return true;
}
// The solution must check three independent conditions for every nonzero cell: (1) no duplicate digit in the same row, (2) no duplicate digit in the same column, and (3) no duplicate digit in the same 3x3 block. The simplest correct approach is brute-force with three nested loops: for each cell (i,j), if board[i][j] is nonzero, compare it with all later cells in the same row (i, k) for k>j, all later cells in the same column (k, j) for k>i, and all later cells in the same 3x3 block. However, a more efficient and readable approach uses three boolean arrays of size 10 (for digits 1-9) to mark seen digits per row, column, and block as you iterate once. Initialize all to false. For each cell, if the value is 0, skip. If the value is outside 1-9, return false immediately. Otherwise, compute block index `bi = (i/3)*3 + (j/3)` and check `row[i][val]`, `col[j][val]`, and `block[bi][val]`; if any is already true, return false, else set them true. This handles duplicate detection efficiently. Edge cases: empty board (all zeros) is valid; a board with a single 0 is valid; a board with any value outside 1-9 is invalid; duplicate in any region returns false. Time complexity is O(81) = O(1) constant because the board size is fixed, and space complexity is O(9*10) = O(1) for the boolean arrays. The brute-force triple-loop approach also works but is O(81^2) worst-case, still constant but less elegant.
