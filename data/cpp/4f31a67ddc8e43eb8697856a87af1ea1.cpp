Write a standalone C++ function `bool isValidSudoku(const std::vector<std::vector<char>>& board)` that determines whether a partially filled 9x9 Sudoku board (represented as a vector of vectors of characters, where `'.'` denotes an empty cell) is valid according to the standard Sudoku rules: each row, each column, and each of the nine 3x3 sub-boxes must contain no duplicate digits from '1' to '9'. The board is guaranteed to be exactly 9x9. The input board is not expected to be mutable, and your function should not modify it. Return `true` if the board is valid (i.e., all filled numbers obey the constraints), and `false` otherwise. Do not worry about whether the puzzle is solvable; only check the current filled entries.
// The main algorithm iterates through every cell of the 9x9 board exactly once. For each cell containing a digit (not `'.'`), we need to check three independent constraints: no duplicate in the same row, same column, and same 3x3 sub-box. The sub-box index for a cell at row `r` and column `c` is computed as `(r / 3) * 3 + (c / 3)`, which maps rows 0-2 with columns 0-2 to box 0, rows 0-2 with columns 3-5 to box 1, etc. To track seen digits efficiently, we use three arrays of `std::unordered_set<char>` (or `bool[9][9]` for fixed-size optimization) — one for each row, column, and box. If we encounter a digit that already exists in any of the three sets for that cell, we return `false` immediately; otherwise, we insert it into all three sets. Edge cases include empty cells (ignored), valid boards with no duplicates, and boards where duplicate digits appear across different rows/columns/boxes (which is allowed and must not trigger a false positive). The algorithm runs in O(81) = O(1) time because the board size is fixed, but for a general N×N Sudoku it would be O(N²) with O(N²) auxiliary space for the sets. For this fixed 9x9 problem, space complexity is O(1) since the sets have at most 9 entries each.
#include <vector>
#include <unordered_set>

// Check if a partially filled 9x9 Sudoku board is valid (no duplicate digits in any row, column, or 3x3 box).
bool isValidSudoku(const std::vector<std::vector<char>>& board) {
    std::vector<std::unordered_set<char>> rows(9), cols(9), boxes(9);

    for (int r = 0; r < 9; ++r) {
        for (int c = 0; c < 9; ++c) {
            char digit = board[r][c];
            if (digit == '.') continue;

            int box_index = (r / 3) * 3 + (c / 3);

            if (rows[r].count(digit) != 0 ||
                cols[c].count(digit) != 0 ||
                boxes[box_index].count(digit) != 0) {
                return false;
            }

            rows[r].insert(digit);
            cols[c].insert(digit);
            boxes[box_index].insert(digit);
        }
    }
    return true;
}
#include <cassert>
#include <vector>
#include <string>

// Declaration of the solution function (provided above).
bool isValidSudoku(const std::vector<std::vector<char>>& board);

int main() {
    // Valid board (partially filled, no duplicates in rows/cols/boxes)
    std::vector<std::vector<char>> valid_board = {
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
    assert(isValidSudoku(valid_board) == true);

    // Duplicate in row (row 0 has '5' twice)
    std::vector<std::vector<char>> duplicate_row = valid_board;
    duplicate_row[0][8] = '5';
    assert(isValidSudoku(duplicate_row) == false);

    // Duplicate in column (column 1 has '3' twice)
    std::vector<std::vector<char>> duplicate_col = valid_board;
    duplicate_col[8][1] = '3';
    assert(isValidSudoku(duplicate_col) == false);

    // Duplicate in a 3x3 box (top-left box has '5' twice)
    std::vector<std::vector<char>> duplicate_box = valid_board;
    duplicate_box[1][0] = '5';
    assert(isValidSudoku(duplicate_box) == false);

    // All empty cells is valid
    std::vector<std::vector<char>> empty_board(9, std::vector<char>(9, '.'));
    assert(isValidSudoku(empty_board) == true);

    // Fully filled valid board
    std::vector<std::vector<char>> full_valid = {
        {'5','3','4','6','7','8','9','1','2'},
        {'6','7','2','1','9','5','3','4','8'},
        {'1','9','8','3','4','2','5','6','7'},
        {'8','5','9','7','6','1','4','2','3'},
        {'4','2','6','8','5','3','7','9','1'},
        {'7','1','3','9','2','4','8','5','6'},
        {'9','6','1','5','3','7','2','8','4'},
        {'2','8','7','4','1','9','6','3','5'},
        {'3','4','5','2','8','6','1','7','9'}
    };
    assert(isValidSudoku(full_valid) == true);

    // Duplicate across different boxes but same row (invalid because row duplicate)
    std::vector<std::vector<char>> tricky = valid_board;
    tricky[0][4] = '3'; // Row 0 already has '3' at column 1
    assert(isValidSudoku(tricky) == false);

    return 0;
}
