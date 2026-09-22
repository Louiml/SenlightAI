// Write a complete C++ function `bool isValidSudoku(const std::vector<std::vector<char>>& board)` that determines whether a given 9×9 Sudoku board is valid according to the standard rules: each row must contain the digits 1–9 without repetition, each column must contain the digits 1–9 without repetition, and each of the nine 3×3 sub-boxes must contain the digits 1–9 without repetition. The board is represented as a vector of vectors of characters, where each character is either a digit `'1'`–`'9'` or a dot `'.'` representing an empty cell. The function should return `true` only if all three sets of constraints are satisfied; rows, columns, and sub-boxes may contain empty cells (dots) freely, but any digit that appears more than once in the same row, column, or 3×3 box invalidates the board. The input board is guaranteed to be exactly 9×9. You must not modify the input board; use `const` references appropriately. The task is to implement this function only (no `main`), and your solution should be self-contained with necessary headers.

// The solution checks each of the three constraint types separately, reusing a small boolean or integer array to track seen digits (1–9). For each row, column, and each of the nine 3×3 sub-boxes, iterate over the positions and, for every non-dot character, convert it to an integer digit (1–9). If that digit has already been seen in the current row/column/box, return `false`; otherwise mark it as seen. After finishing each row/column/box, reset the seen array for the next one. This approach is straightforward because it avoids complex state and only requires O(1) extra space for the seen tracker (size 10 to index 1–9). Time complexity is O(81) = O(1) since the board is fixed 9×9; more formally, it is O(N²) for an N×N Sudoku but here N is constant. Space complexity is O(1) besides the board itself. Edge cases: empty cells (dots) are ignored; a board that is valid row-wise but invalid column-wise or box-wise must be caught; duplicates in the same box must be detected even if they are in different rows/columns; and the input is always 9×9, so no size checks are needed. The implementation uses separate helper lambdas or inline loops for row, column, and box checking to keep the code readable and reusable.

#include <vector>
#include <cstring>

// Helper to check a single row, column, or 3x3 square.
// board is the 9x9 board, startR and startC are the starting indices.
// type: 0 = row (startC=0, iterate columns), 1 = column (startR=0, iterate rows), 2 = square (startR, startC are top-left).
bool checkRegion(const std::vector<std::vector<char>>& board, int startR, int startC, int type) {
    bool seen[10] = {false}; // digits 1-9
    for (int k = 0; k < 9; ++k) {
        int r, c;
        if (type == 0) {
            r = startR; c = k;
        } else if (type == 1) {
            r = k; c = startC;
        } else {
            r = startR + k / 3;
            c = startC + k % 3;
        }
        char ch = board[r][c];
        if (ch == '.') continue;
        int digit = ch - '0';
        if (digit < 1 || digit > 9 || seen[digit]) return false;
        seen[digit] = true;
    }
    return true;
}

// Determines if a 9x9 Sudoku board is valid per standard rules.
bool isValidSudoku(const std::vector<std::vector<char>>& board) {
    // Check all rows
    for (int i = 0; i < 9; ++i) {
        if (!checkRegion(board, i, 0, 0)) return false;
    }
    // Check all columns
    for (int i = 0; i < 9; ++i) {
        if (!checkRegion(board, 0, i, 1)) return false;
    }
    // Check all 3x3 boxes
    for (int i = 0; i < 9; i += 3) {
        for (int j = 0; j < 9; j += 3) {
            if (!checkRegion(board, i, j, 2)) return false;
        }
    }
    return true;
}

#include <cassert>
#include <vector>

// Declaration of the function (already defined above).
bool isValidSudoku(const std::vector<std::vector<char>>& board);

int main() {
    // Valid board (standard example)
    std::vector<std::vector<char>> valid = {
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
    assert(isValidSudoku(valid) == true);

    // Invalid: duplicate 8 in first row (position 0 and 5)
    std::vector<std::vector<char>> invalidRow = valid;
    invalidRow[0][5] = '8';
    assert(isValidSudoku(invalidRow) == false);

    // Invalid: duplicate 5 in first column (position 0 and 4)
    std::vector<std::vector<char>> invalidCol = valid;
    invalidCol[4][0] = '5';
    assert(isValidSudoku(invalidCol) == false);

    // Invalid: duplicate 6 in top-left 3x3 box (position 0,1 and 1,0)
    std::vector<std::vector<char>> invalidBox = valid;
    invalidBox[0][1] = '6'; // already has 6 at (1,0)? Actually (1,0) is '6', so adding at (0,1) is valid? Let's make a proper duplicate.
    // Set (0,1) to '6' and (1,0) already '6' -> if we set (0,1)= '6', then box has two 6s.
    invalidBox[0][1] = '6';
    assert(isValidSudoku(invalidBox) == false);

    // All empty board is valid (no duplicates)
    std::vector<std::vector<char>> empty(9, std::vector<char>(9, '.'));
    assert(isValidSudoku(empty) == true);

    // Single digit repeated in a row but not in same row? Test one duplicate in column
    std::vector<std::vector<char>> dupColumn = empty;
    dupColumn[0][0] = '1';
    dupColumn[3][0] = '1';
    assert(isValidSudoku(dupColumn) == false);

    // Duplicate in same box but different rows/columns
    std::vector<std::vector<char>> dupBox = empty;
    dupBox[0][0] = '2';
    dupBox[1][1] = '2';
    assert(isValidSudoku(dupBox) == false);

    // Valid with many empties but one of each digit in a row
    std::vector<std::vector<char>> partiallyValid = empty;
    partiallyValid[0] = {'1','2','3','4','5','6','7','8','9'};
    assert(isValidSudoku(partiallyValid) == true);

    return 0;
}
