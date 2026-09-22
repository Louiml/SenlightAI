/*
Write a C++ function that determines whether a 9x9 Sudoku board is valid. The board is represented as a `vector<vector<char>>` where each cell contains a digit `'1'`–`'9'` or a dot `'.'` for an empty cell. A valid board satisfies three rules: each row contains no duplicate digits, each column contains no duplicate digits, and each of the nine 3x3 sub-boxes contains no duplicate digits. Empty cells (`.`) are ignored. The function must return `true` if the board is valid, `false` otherwise. The input may have trailing or leading dots, and there is no need to check whether the board is solvable—only whether the current placement of digits is valid.
*/

#include <vector>

// Returns true if the given 9x9 Sudoku board is valid (no duplicate digits in any row, column, or 3x3 sub-box).
bool isValidSudoku(const std::vector<std::vector<char>>& board) {
    // Helper lambda to check if placing 'value' at (r,c) is safe under Sudoku rules.
    auto isSafe = [&](int r, int c, char value) -> bool {
        // Check the entire column for duplicates (skip current cell).
        for (int i = 0; i < 9; ++i) {
            if (board[i][c] == value && i != r) return false;
        }
        // Check the entire row for duplicates (skip current cell).
        for (int j = 0; j < 9; ++j) {
            if (board[r][j] == value && j != c) return false;
        }
        // Check the 3x3 block containing (r,c).
        int blockRow = 3 * (r / 3);
        int blockCol = 3 * (c / 3);
        for (int i = 0; i < 9; ++i) {
            int x = blockRow + (i / 3);
            int y = blockCol + (i % 3);
            if (board[x][y] == value && (x != r || y != c)) return false;
        }
        return true;
    };

    // Iterate over all cells.
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            if (board[i][j] != '.') {
                if (!isSafe(i, j, board[i][j])) {
                    return false;
                }
            }
        }
    }
    return true;
}

#include <cassert>
#include <vector>

// Include the solution function here (or paste above in a real project).

int main() {
    // Valid board (just one row and column check).
    std::vector<std::vector<char>> valid1 = {
        {'5','3','.','.','7','.','.','.','.'},
        {'6','.','.','1','9','5','.','.','.'},
        {'.','9','8','.','.','.','.','6','.'},
        {'8','.','.','.','6','.','.','.','3'},
        {'4','.','.','8','.','3','.','.','1'},
        {'7','.','.','.','2','.','.','.','6'},
        {'.','6','.','.','.','.','2','8','.'},
        {'.','.','.','4','1','9','.','.','5'},
        {'.','.','.','.','8','.','.','7','9'}
    };
    assert(isValidSudoku(valid1) == true);

    // Invalid: duplicate '5' in first row (columns 0 and 8).
    std::vector<std::vector<char>> invalidRow = {
        {'5','3','.','.','7','.','.','.','5'},
        {'6','.','.','1','9','5','.','.','.'},
        {'.','9','8','.','.','.','.','6','.'},
        {'8','.','.','.','6','.','.','.','3'},
        {'4','.','.','8','.','3','.','.','1'},
        {'7','.','.','.','2','.','.','.','6'},
        {'.','6','.','.','.','.','2','8','.'},
        {'.','.','.','4','1','9','.','.','5'},
        {'.','.','.','.','8','.','.','7','9'}
    };
    assert(isValidSudoku(invalidRow) == false);

    // Invalid: duplicate '3' in first column (rows 0 and 6).
    std::vector<std::vector<char>> invalidCol = {
        {'5','3','.','.','7','.','.','.','.'},
        {'6','.','.','1','9','5','.','.','.'},
        {'.','9','8','.','.','.','.','6','.'},
        {'8','.','.','.','6','.','.','.','3'},
        {'4','.','.','8','.','3','.','.','1'},
        {'7','.','.','.','2','.','.','.','6'},
        {'3','6','.','.','.','.','2','8','.'},
        {'.','.','.','4','1','9','.','.','5'},
        {'.','.','.','.','8','.','.','7','9'}
    };
    assert(isValidSudoku(invalidCol) == false);

    // Invalid: duplicate '8' in top-left 3x3 block (positions (0,0) and (2,2)).
    std::vector<std::vector<char>> invalidBlock = {
        {'8','3','.','.','7','.','.','.','.'},
        {'6','.','.','1','9','5','.','.','.'},
        {'.','9','8','.','.','.','.','6','.'},
        {'8','.','.','.','6','.','.','.','3'},
        {'4','.','.','8','.','3','.','.','1'},
        {'7','.','.','.','2','.','.','.','6'},
        {'.','6','.','.','.','.','2','8','.'},
        {'.','.','.','4','1','9','.','.','5'},
        {'.','.','.','.','8','.','.','7','9'}
    };
    assert(isValidSudoku(invalidBlock) == false);

    // All dots (empty board) is valid.
    std::vector<std::vector<char>> emptyBoard(9, std::vector<char>(9, '.'));
    assert(isValidSudoku(emptyBoard) == true);

    // Single digit in each row, column, block (valid).
    std::vector<std::vector<char>> valid2 = {
        {'1','.','.','.','.','.','.','.','.'},
        {'.','2','.','.','.','.','.','.','.'},
        {'.','.','3','.','.','.','.','.','.'},
        {'.','.','.','4','.','.','.','.','.'},
        {'.','.','.','.','5','.','.','.','.'},
        {'.','.','.','.','.','6','.','.','.'},
        {'.','.','.','.','.','.','7','.','.'},
        {'.','.','.','.','.','.','.','8','.'},
        {'.','.','.','.','.','.','.','.','9'}
    };
    assert(isValidSudoku(valid2) == true);

    // Duplicate digit in same 3x3 but different row and column (invalid).
    std::vector<std::vector<char>> invalidOverlap = {
        {'1','.','.','.','.','.','.','.','.'},
        {'.','1','.','.','.','.','.','.','.'},
        {'.','.','.','.','.','.','.','.','.'},
        {'.','.','.','.','.','.','.','.','.'},
        {'.','.','.','.','.','.','.','.','.'},
        {'.','.','.','.','.','.','.','.','.'},
        {'.','.','.','.','.','.','.','.','.'},
        {'.','.','.','.','.','.','.','.','.'},
        {'.','.','.','.','.','.','.','.','.'}
    };
    assert(isValidSudoku(invalidOverlap) == false);

    return 0;
}

// The solution iterates through every cell of the board. For each cell that is not `.`, we check the three constraints using a helper function `isSafe`. For a cell at `(cur_row, cur_col)`, we first check all cells in the same column (varying row index) and all cells in the same row (varying column index) for the same digit, ensuring we skip the cell itself. Then we check the 3x3 block: the block starts at row `3*(cur_row/3)` and column `3*(cur_col/3)`. By iterating an index `i` from 0 to 8, we compute `x = 3*(cur_row/3) + (i/3)` and `y = 3*(cur_col/3) + (i%3)` to cover all 9 cells in the block, again skipping the cell itself. If any duplicate digit is found in any of the three constraints, the board is invalid. The algorithm checks each of the 81 cells once and for each non-dot cell performs at most 27 comparisons, so the total time complexity is \(O(81 \times 27) = O(1)\) (constant, since the board is fixed 9x9). Space complexity is \(O(1)\) as no extra data structures are used. Edge cases include a completely empty board (all dots), which is valid; a board with duplicate digits in a single row, column, or block; and a board where digits appear only once everywhere. The approach correctly handles these by ignoring dots and comparing only actual digit characters.
