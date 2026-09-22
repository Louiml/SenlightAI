// Write a C++ function `bool solveSudoku(std::vector<std::vector<char>>& board)` that takes a 9×9 Sudoku board represented as a vector of vectors of characters, where empty cells are `.` and filled cells are digits `'1'`–`'9'`. The function must solve the puzzle in-place using backtracking and return `true` if a solution exists, `false` otherwise. If a solution exists, the board must be completely filled with valid digits. The input is guaranteed to be a valid partially filled Sudoku puzzle (no duplicate digits in any row, column, or 3×3 block), but not necessarily solvable. The function should not use any global state.
// The algorithm is classic recursive backtracking: find the first empty cell (using row‑major order), try digits `'1'` through `'9'` in that cell, and for each digit check validity against the current row, column, and 3×3 sub‑grid. If valid, place the digit and recursively attempt to solve the rest of the board. If the recursion returns `true`, propagate success upward; otherwise undo the placement (backtrack) and try the next digit. If no digit works, return `false`. The base case is reached when there are no empty cells, meaning the board is complete. Edge cases: the input may already be complete (returns `true` immediately), may have multiple solutions (any valid one is acceptable), or may be unsolvable (returns `false` and leaves the board unchanged because we restore each attempted cell to `.` before returning `false`). Time complexity is O(9^(m)) where m is the number of empty cells (worst case 81, but pruning drastically reduces practical cases). Space complexity is O(m) for the recursion stack, up to O(81) in the worst case.
#include <vector>

// Check if placing digit at (row, col) is valid in the current board.
bool isValid(const std::vector<std::vector<char>>& board, int row, int col, char digit) {
    // Check row and column
    for (int i = 0; i < 9; ++i) {
        if (board[row][i] == digit || board[i][col] == digit) {
            return false;
        }
    }
    // Check 3x3 box
    int boxRow = (row / 3) * 3;
    int boxCol = (col / 3) * 3;
    for (int i = boxRow; i < boxRow + 3; ++i) {
        for (int j = boxCol; j < boxCol + 3; ++j) {
            if (board[i][j] == digit) {
                return false;
            }
        }
    }
    return true;
}

// Recursive backtracking helper.
bool solveBacktrack(std::vector<std::vector<char>>& board) {
    // Find first empty cell
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            if (board[i][j] == '.') {
                // Try each digit
                for (char digit = '1'; digit <= '9'; ++digit) {
                    if (isValid(board, i, j, digit)) {
                        board[i][j] = digit;
                        if (solveBacktrack(board)) {
                            return true;
                        }
                        board[i][j] = '.'; // backtrack
                    }
                }
                return false; // no digit works
            }
        }
    }
    return true; // no empty cells -> solved
}

// Public function: solves the Sudoku in-place. Returns true if solvable.
bool solveSudoku(std::vector<std::vector<char>>& board) {
    return solveBacktrack(board);
}
#include <cassert>
#include <vector>

// Assume solveSudoku and helper functions are defined above.

int main() {
    // Test 1: Simple solvable board
    std::vector<std::vector<char>> board1 = {
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
    assert(solveSudoku(board1) == true);
    // Verify no empty cells remain
    for (const auto& row : board1)
        for (char c : row)
            assert(c != '.');

    // Test 2: A board that is already solved
    std::vector<std::vector<char>> board2 = {
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
    assert(solveSudoku(board2) == true);
    // Check a known filled value remains unchanged
    assert(board2[0][0] == '5');

    // Test 3: Unsolvable board (two same digits in a row)
    std::vector<std::vector<char>> board3 = {
        {'1','1','.','.','.','.','.','.','.'},
        {'.','.','.','.','.','.','.','.','.'},
        {'.','.','.','.','.','.','.','.','.'},
        {'.','.','.','.','.','.','.','.','.'},
        {'.','.','.','.','.','.','.','.','.'},
        {'.','.','.','.','.','.','.','.','.'},
        {'.','.','.','.','.','.','.','.','.'},
        {'.','.','.','.','.','.','.','.','.'},
        {'.','.','.','.','.','.','.','.','.'}
    };
    assert(solveSudoku(board3) == false);

    // Test 4: Single empty cell solvable
    std::vector<std::vector<char>> board4 = {
        {'1','2','3','4','5','6','7','8','9'},
        {'4','5','6','7','8','9','1','2','3'},
        {'7','8','9','1','2','3','4','5','6'},
        {'2','3','4','5','6','7','8','9','1'},
        {'5','6','7','8','9','1','2','3','4'},
        {'8','9','1','2','3','4','5','6','7'},
        {'3','4','5','6','7','8','9','1','2'},
        {'6','7','8','9','1','2','3','4','5'},
        {'9','1','2','3','4','5','6','7','.'}
    };
    assert(solveSudoku(board4) == true);
    assert(board4[8][8] == '8');

    // Test 5: Empty board (solvable)
    std::vector<std::vector<char>> board5(9, std::vector<char>(9, '.'));
    assert(solveSudoku(board5) == true);

    return 0;
}
