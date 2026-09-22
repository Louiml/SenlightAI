// Complete the following Sudoku helper library by writing a C++ function `bool isValidSudoku(const int grid[9][9])` that validates an entire 9×9 Sudoku board according to standard Sudoku rules: every row, every column, and every 3×3 sub-grid must contain no duplicate numbers from 1 to 9 (zeros represent empty cells and are ignored). The function must return `true` if the board is valid in all three dimensions and `false` otherwise. The input array is always 9×9 and may contain any integers; only values in the range 1–9 are considered significant, and all other values (including 0) are treated as empty. The function must not modify the input array, and you must write it using the helper functions provided in the given snippet—specifically, you should reuse `checkRow`, `checkCol`, and `checkSubGrid` (which is already directly accessible). You may also use `findFirstEmptyRow` and/or `findFirstEmptyCol` if helpful, but they are not required. The final solution should be a single, self-contained function suitable for inclusion in the same library.
#include <cassert>

// Assume the solution function and all helpers are declared above.
int main() {
    // Valid empty board (all zeros)
    int empty[9][9] = {0};
    assert(isValidSudoku(empty) == true);

    // Valid fully solved board
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

    // Invalid: duplicate 5 in first row
    int badRow[9][9] = {
        {5,3,5,6,7,8,9,1,2},
        {6,7,2,1,9,5,3,4,8},
        {1,9,8,3,4,2,5,6,7},
        {8,5,9,7,6,1,4,2,3},
        {4,2,6,8,5,3,7,9,1},
        {7,1,3,9,2,4,8,5,6},
        {9,6,1,5,3,7,2,8,4},
        {2,8,7,4,1,9,6,3,5},
        {3,4,5,2,8,6,1,7,9}
    };
    assert(isValidSudoku(badRow) == false);

    // Invalid: duplicate 7 in first column
    int badCol[9][9] = {
        {5,3,4,6,7,8,9,1,2},
        {7,7,2,1,9,5,3,4,8},
        {1,9,8,3,4,2,5,6,7},
        {8,5,9,7,6,1,4,2,3},
        {4,2,6,8,5,3,7,9,1},
        {7,1,3,9,2,4,8,5,6},
        {7,6,1,5,3,7,2,8,4},
        {2,8,7,4,1,9,6,3,5},
        {3,4,5,2,8,6,1,7,9}
    };
    assert(isValidSudoku(badCol) == false);

    // Invalid: duplicate 8 in top-left 3x3 sub-grid
    int badSub[9][9] = {
        {5,3,8,6,7,8,9,1,2},
        {6,7,2,1,9,5,3,4,8},
        {1,9,8,3,4,2,5,6,7},
        {8,5,9,7,6,1,4,2,3},
        {4,2,6,8,5,3,7,9,1},
        {7,1,3,9,2,4,8,5,6},
        {9,6,1,5,3,7,2,8,4},
        {2,8,7,4,1,9,6,3,5},
        {3,4,5,2,8,6,1,7,9}
    };
    assert(isValidSudoku(badSub) == false);

    // Valid: board with zeros (partial solution)
    int partial[9][9] = {
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
}
#include <cstddef>

// Helper forward declarations (from provided snippet)
bool checkRow(const int grid[9][9], int row);
bool checkCol(const int grid[9][9], int col);
bool checkSubGrid(const int grid[9][9], int x, int y);

// Validate an entire 9x9 Sudoku board.
// Returns true if every row, column, and 3x3 sub-grid has no duplicate nonzero values.
bool isValidSudoku(const int grid[9][9]) {
    // Check all rows
    for (int row = 0; row < 9; ++row) {
        if (!checkRow(grid, row)) {
            return false;
        }
    }
    // Check all columns
    for (int col = 0; col < 9; ++col) {
        if (!checkCol(grid, col)) {
            return false;
        }
    }
    // Check all 3x3 sub-grids (top-left corners at (0,0), (0,3), (0,6), ...)
    for (int startRow = 0; startRow < 9; startRow += 3) {
        for (int startCol = 0; startCol < 9; startCol += 3) {
            if (!checkSubGrid(grid, startRow, startCol)) {
                return false;
            }
        }
    }
    return true;
}
(Note: The helper functions `checkRow`, `checkCol`, and `checkSubGrid` are assumed to be available elsewhere in the library, as provided in the snippet. The above code uses them directly. For completeness, if the helpers are not in scope, the solution should also include their implementations, but the task states they are already provided.)
// The solution iterates over all 9 rows and all 9 columns, calling `checkRow` on each row and `checkCol` on each column. Simultaneously, it iterates over all nine 3×3 sub-grids by selecting the top-left coordinate of each sub-grid (indices `0,3,6` for both row and column) and calling `checkSubGrid` on each. The `checkSubGrid` function already handles duplicates within a 3×3 box by maintaining an availability array and returning `false` as soon as a duplicate nonzero value is found. Similarly, `checkRow` and `checkCol` loop through each row/column and compare each nonzero value with every subsequent value in the same line, returning `false` on any duplicate. Edge cases include all-zero boards (which are valid because no duplicates exist), boards with only a single row/column invalid, and boards where duplicates appear only in the 3×3 sub-grid but not in rows/columns. Time complexity is \(O(9^2)\) for rows, \(O(9^2)\) for columns, and \(O(9 \times 9) = O(81)\) for sub-grids, totaling \(O(1)\) since the board size is fixed. Space complexity is \(O(1)\) because only constant-size auxiliary arrays are used inside the helper functions.
