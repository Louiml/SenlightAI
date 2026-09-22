// Write a standalone C++ function named `isValidSudokuBoard` that takes a constant reference to a 9x9 `std::vector<std::vector<char>>` representing a Sudoku board. The board may contain digits `'1'`–`'9'` or the character `'.'` (representing an empty cell). The function must return `true` if the board is a valid partially filled Sudoku, meaning that in **every row**, **every column**, and **every one of the nine 3×3 sub-grids**, no digit appears more than once. Ignore `'.'` cells. Do not check whether the puzzle is solvable; only check the validity of the current filled digits. The function should be `const`‑correct, use no external global state, and operate in O(1) time and O(1) auxiliary space.
#include <cassert>
#include <vector>

// The solution function is declared in this test file for linking.

int main() {
    // Valid empty board (all '.')
    std::vector<std::vector<char>> emptyBoard(9, std::vector<char>(9, '.'));
    assert(isValidSudokuBoard(emptyBoard) == true);

    // Valid partially filled board (example from LeetCode)
    std::vector<std::vector<char>> validBoard = {
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
    assert(isValidSudokuBoard(validBoard) == true);

    // Duplicate in first row
    auto rowDup = validBoard;
    rowDup[0][1] = '5'; // row 0 already has '5' at col 0
    assert(isValidSudokuBoard(rowDup) == false);

    // Duplicate in first column
    auto colDup = validBoard;
    colDup[1][0] = '5'; // col 0 already has '5' at row 0
    assert(isValidSudokuBoard(colDup) == false);

    // Duplicate in top-left 3x3 block
    auto blockDup = validBoard;
    blockDup[1][1] = '5'; // block top-left already has '5' at (0,0)
    assert(isValidSudokuBoard(blockDup) == false);

    // Invalid digit '0'
    auto badDigit = validBoard;
    badDigit[0][2] = '0';
    assert(isValidSudokuBoard(badDigit) == false);

    // Invalid character (non-digit, non-dot)
    auto badChar = validBoard;
    badChar[0][2] = 'a';
    assert(isValidSudokuBoard(badChar) == false);

    // Valid board with all digits filled correctly (no duplicates in rows/cols/blocks)
    std::vector<std::vector<char>> fullValidBoard = {
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
    assert(isValidSudokuBoard(fullValidBoard) == true);

    return 0;
}
#include <vector>
#include <cctype>

// Returns true if the 9x9 Sudoku board is partially valid.
// Ignore '.' cells; check rows, columns, and 3x3 sub-grids.
bool isValidSudokuBoard(const std::vector<std::vector<char>>& board) {
    const int N = 9;

    // Check each row
    for (int row = 0; row < N; ++row) {
        bool seen[10] = {false};
        for (int col = 0; col < N; ++col) {
            char ch = board[row][col];
            if (ch == '.') continue;
            int digit = ch - '0';
            if (digit < 1 || digit > 9) return false;
            if (seen[digit]) return false;
            seen[digit] = true;
        }
    }

    // Check each column
    for (int col = 0; col < N; ++col) {
        bool seen[10] = {false};
        for (int row = 0; row < N; ++row) {
            char ch = board[row][col];
            if (ch == '.') continue;
            int digit = ch - '0';
            if (digit < 1 || digit > 9) return false;
            if (seen[digit]) return false;
            seen[digit] = true;
        }
    }

    // Check each 3x3 sub-grid
    for (int blockRow = 0; blockRow < N; blockRow += 3) {
        for (int blockCol = 0; blockCol < N; blockCol += 3) {
            bool seen[10] = {false};
            for (int i = 0; i < 3; ++i) {
                for (int j = 0; j < 3; ++j) {
                    char ch = board[blockRow + i][blockCol + j];
                    if (ch == '.') continue;
                    int digit = ch - '0';
                    if (digit < 1 || digit > 9) return false;
                    if (seen[digit]) return false;
                    seen[digit] = true;
                }
            }
        }
    }

    return true;
}
// The core idea is to scan the board systematically and validate three types of regions: rows, columns, and 3×3 sub‑grids. The simplest approach is to iterate over all nine rows and all nine columns directly, and for each row/column, use a boolean array of size 10 (indices 1–9) to mark which digits have been seen. If a digit repeats, immediately return `false`. For sub‑grids, iterate over the top‑left corners (0,0), (0,3), (0,6), (3,0), ... and check each 3×3 block similarly. A more compact approach is to iterate once through all cells and maintain three separate boolean matrices: one for rows, one for columns, and one for blocks. However, the simplest and most readable implementation checks each region separately. Edge cases include boards with only `'.'` (should return `true`), a board with a duplicate in the same block but not row/column, and a board with a valid configuration. The time complexity is O(81) = O(1) because the board size is fixed; auxiliary space is O(9×9×3) = O(1) if using separate arrays, or O(9) per check if using temporary masks. Since the board is fixed at 9×9, both time and space are constant.
