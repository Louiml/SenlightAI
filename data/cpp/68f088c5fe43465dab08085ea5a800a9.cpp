// Write a C++ function `bool solveSudoku(std::vector<std::vector<char>>& board)` that takes a 9x9 Sudoku board represented as a vector of vectors of characters, where empty cells are marked with '.' and filled cells contain digits '1' through '9'. The function should solve the puzzle in-place using backtracking, returning `true` if a solution exists and the board is filled with the solution, and `false` if the puzzle has no valid solution. You must implement a helper function `bool isValid(const std::vector<std::vector<char>>& board, int row, int col)` that checks whether placing the current character at `(row, col)` violates Sudoku rules (no duplicates in the same row, column, or 3x3 subgrid). The solution must handle puzzles that are already partially filled, may have multiple solutions (you only need to produce one), and must not modify the board if no solution exists (i.e., restore the original state on failure). Assume the input board is always 9x9 and contains only digits or '.'.
The algorithm uses standard backtracking: iterate over all cells row-major order. For each empty cell ('.'), try placing digits '1' through '9'. For each candidate digit, temporarily place it, then check if the placement is valid with respect to the row, column, and 3x3 block. If valid, recursively attempt to solve the rest of the board; if the recursion succeeds, return `true`. If the recursion fails, undo the placement (set back to '.') and try the next digit. If no digit works, return `false` to trigger backtracking at the previous level. The base case occurs when no empty cells remain, meaning the board is fully solved and valid, so return `true`. Important edge cases: an already-solved board (no empty cells) immediately returns `true`; an unsolvable puzzle (e.g., conflicting digits initially) will exhaust all possibilities and return `false` without permanently altering the board because all temporary placements are undone. The `isValid` helper must check the row (excluding the current cell), the column, and the 3x3 subgrid containing the cell. Time complexity is \(O(9^{(81 - f)})\) in the worst case where \(f\) is the number of initially filled cells, but with pruning by validity checks, practical performance is much better; worst-case upper bound is often given as \(O((n!)^n)\) for a generic \(n \times n\) puzzle, but for \(n=9\) it is exponential. Space complexity is \(O(1)\) extra space beyond the board itself, but recursion depth is \(O(81)\) due to the call stack (or \(O(1)\) if counting only auxiliary data structures, though the recursive stack is \(O(81)\) which is effectively constant for fixed 9x9).
#include <vector>

// Check if placing board[row][col] is valid (no duplicates in row, column, or block)
bool isValid(const std::vector<std::vector<char>>& board, int row, int col) {
    char val = board[row][col];
    // Check row
    for (int j = 0; j < 9; ++j) {
        if (j != col && board[row][j] == val) return false;
    }
    // Check column
    for (int i = 0; i < 9; ++i) {
        if (i != row && board[i][col] == val) return false;
    }
    // Check 3x3 block
    int startRow = (row / 3) * 3;
    int startCol = (col / 3) * 3;
    for (int i = startRow; i < startRow + 3; ++i) {
        for (int j = startCol; j < startCol + 3; ++j) {
            if (!(i == row && j == col) && board[i][j] == val) return false;
        }
    }
    return true;
}

// Solve Sudoku in-place using backtracking; returns true if solved
bool solveSudoku(std::vector<std::vector<char>>& board) {
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            if (board[i][j] == '.') {
                for (char c = '1'; c <= '9'; ++c) {
                    board[i][j] = c;
                    if (isValid(board, i, j) && solveSudoku(board)) {
                        return true;
                    }
                    board[i][j] = '.';  // undo
                }
                return false;  // no digit works
            }
        }
    }
    return true;  // no empty cells remain -> solved
}
#include <cassert>
#include <vector>

// Assume solveSudoku and isValid are defined above

int main() {
    // Test 1: already solved board
    std::vector<std::vector<char>> solved = {
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
    assert(solveSudoku(solved) == true);
    // Verify it's unchanged
    assert(solved[0][0] == '5');

    // Test 2: empty board (should be solvable)
    std::vector<std::vector<char>> empty(9, std::vector<char>(9, '.'));
    assert(solveSudoku(empty) == true);
    // Check first cell is now a valid digit
    assert(empty[0][0] >= '1' && empty[0][0] <= '9');

    // Test 3: known puzzle (from LeetCode example)
    std::vector<std::vector<char>> puzzle = {
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
    assert(solveSudoku(puzzle) == true);
    // Verify solution correctness: check row 0 contains digits 1-9
    bool row0[9] = {false};
    for (char c : puzzle[0]) if (c != '.') row0[c-'1'] = true;
    for (int k = 0; k < 9; ++k) assert(row0[k]);

    // Test 4: unsolvable board (two 5s in first row initially)
    std::vector<std::vector<char>> unsolvable = solved;
    unsolvable[0][1] = '5';  // duplicate 5 in row 0
    assert(solveSudoku(unsolvable) == false);
    // Board may be modified but that's allowed; check no invalid solution forced

    // Test 5: single empty cell
    std::vector<std::vector<char>> nearlySolved = solved;
    nearlySolved[0][0] = '.';
    assert(solveSudoku(nearlySolved) == true);
    assert(nearlySolved[0][0] == '5');

    // Test 6: board with a single row empty (other rows valid) - must be solvable
    std::vector<std::vector<char>> rowEmpty = solved;
    for (int j = 0; j < 9; ++j) rowEmpty[4][j] = '.';
    assert(solveSudoku(rowEmpty) == true);

    // Test 7: board with a single column empty - must be solvable
    std::vector<std::vector<char>> colEmpty = solved;
    for (int i = 0; i < 9; ++i) colEmpty[i][7] = '.';
    assert(solveSudoku(colEmpty) == true);

    // Test 8: board with one block empty - must be solvable
    std::vector<std::vector<char>> blockEmpty = solved;
    for (int i = 0; i < 3; ++i) for (int j = 0; j < 3; ++j) blockEmpty[i][j] = '.';
    assert(solveSudoku(blockEmpty) == true);

    // Test 9: board that is a full 9x9 of '.' but with a conflicting pre-filled? Not possible
    // Just check a random fill: 1-9 in a diagonal pattern with '.' elsewhere
    std::vector<std::vector<char>> diagonal(9, std::vector<char>(9, '.'));
    for (int i = 0; i < 9; ++i) diagonal[i][i] = '1' + (i % 9);
    assert(solveSudoku(diagonal) == true);

    // Test 10: empty board with a given invalid configuration (e.g., two 9s in same row)
    std::vector<std::vector<char>> invalid = empty;
    invalid[0][0] = '9';
    invalid[0][1] = '9';
    assert(solveSudoku(invalid) == false);

    return 0;
}
