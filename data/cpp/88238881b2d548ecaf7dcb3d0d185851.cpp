/*
Write a C++ function `bool isValidSudoku(const std::vector<std::vector<char>>& board)` that determines whether a given 9×9 Sudoku board is valid according to the standard rules: each row, each column, and each of the nine 3×3 sub-grids must contain the digits '1'–'9' without repetition. Empty cells are represented by `'.'`. The function should return `true` if the board is valid, and `false` otherwise. The board is guaranteed to be exactly 9×9, and it is not required to be a complete or solvable puzzle—only the partial filling must satisfy the constraints.
*/
#include <vector>

// Check if a 9x9 Sudoku board is partially valid (rows, columns, and 3x3 sub-grids have no duplicates).
bool isValidSudoku(const std::vector<std::vector<char>>& board) {
    const int size = 9;
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            char c = board[i][j];
            if (c == '.') continue;

            // Check row
            for (int k = 0; k < size; ++k) {
                if (k == j) continue;
                if (board[i][k] == c) return false;
            }

            // Check column
            for (int k = 0; k < size; ++k) {
                if (k == i) continue;
                if (board[k][j] == c) return false;
            }

            // Check 3x3 block
            int top = (i / 3) * 3;
            int left = (j / 3) * 3;
            for (int x = top; x < top + 3; ++x) {
                for (int y = left; y < left + 3; ++y) {
                    if (x == i && y == j) continue;
                    if (board[x][y] == c) return false;
                }
            }
        }
    }
    return true;
}
#include <cassert>
#include <vector>

// Assume isValidSudoku is declared above.

int main() {
    // Valid partial board
    std::vector<std::vector<char>> b1 = {
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
    assert(isValidSudoku(b1) == true);

    // Duplicate in row (first row has two 5's)
    std::vector<std::vector<char>> b2 = b1;
    b2[0][2] = '5';
    assert(isValidSudoku(b2) == false);

    // Duplicate in column (first column has two 5's)
    std::vector<std::vector<char>> b3 = b1;
    b3[2][0] = '5';
    assert(isValidSudoku(b3) == false);

    // Duplicate in 3x3 block (top-left block has two 5's)
    std::vector<std::vector<char>> b4 = b1;
    b4[2][2] = '5';
    assert(isValidSudoku(b4) == false);

    // Fully empty board is valid
    std::vector<std::vector<char>> b5(9, std::vector<char>(9, '.'));
    assert(isValidSudoku(b5) == true);

    // Fully filled valid solution (simplified: all rows are permutations 1-9, but columns/blocks also fine)
    std::vector<std::vector<char>> b6 = {
        {'1','2','3','4','5','6','7','8','9'},
        {'4','5','6','7','8','9','1','2','3'},
        {'7','8','9','1','2','3','4','5','6'},
        {'2','3','4','5','6','7','8','9','1'},
        {'5','6','7','8','9','1','2','3','4'},
        {'8','9','1','2','3','4','5','6','7'},
        {'3','4','5','6','7','8','9','1','2'},
        {'6','7','8','9','1','2','3','4','5'},
        {'9','1','2','3','4','5','6','7','8'}
    };
    assert(isValidSudoku(b6) == true);

    // Invalid: duplicate '1' in first column (b6[0][0] and b6[8][0])
    std::vector<std::vector<char>> b7 = b6;
    b7[8][0] = '1';
    assert(isValidSudoku(b7) == false);

    return 0;
}
// The solution checks every cell that contains a digit and verifies that the same digit does not appear elsewhere in its row, column, or 3×3 block. The simplest approach is a triple nested loop: for each cell (i, j), if the character is not `'.'`, scan its entire row (all columns for fixed i), its entire column (all rows for fixed j), and its 3×3 block (starting at `top = (i/3)*3` and `left = (j/3)*3`). Skip the cell itself to avoid a false duplicate. If any duplicate is found, return `false` immediately. Edge cases include empty board cells (ignored), a fully empty board (valid), and boards with multiple empty cells. The time complexity is O(9^4) in the worst case because for each of at most 81 non-empty cells, we check up to 8 row, 8 column, and 8 block cells (constant per cell), so it’s effectively O(81 * 24) ≈ O(1) for a fixed 9×9 board, but expressed generally it is O(n^4) for an n×n board with n blocks. Space complexity is O(1) since no extra data structures are used.
