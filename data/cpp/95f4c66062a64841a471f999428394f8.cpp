Write a standalone C++ function named `solveSudoku` that takes a 9x9 Sudoku board represented as `std::vector<std::vector<int>>` (with `0` denoting empty cells) and returns a solved copy of the board as a `std::vector<std::vector<int>>` if a solution exists, otherwise returns the original board unchanged. The function must use a backtracking algorithm to find a valid solution, respecting standard Sudoku rules: each row, each column, and each of the nine 3x3 subgrids must contain the digits 1–9 exactly once without repetition. The input board is guaranteed to be a valid partially filled puzzle (i.e., no conflicting pre-filled numbers). If multiple solutions exist, any valid solution is acceptable. The function must be `const`-correct, not modify the input board, and handle empty-cell detection, number validity checks, and recursive backtracking with rollback. Provide a descriptive comment above the function explaining its behavior.

#include <cassert>
#include <vector>

// The solution function is assumed to be defined above.

int main() {
    // Example 1: A simple solvable puzzle (from the snippet's style)
    std::vector<std::vector<int>> puzzle1 = {
        {5, 3, 0, 0, 7, 0, 0, 0, 0},
        {6, 0, 0, 1, 9, 5, 0, 0, 0},
        {0, 9, 8, 0, 0, 0, 0, 6, 0},
        {8, 0, 0, 0, 6, 0, 0, 0, 3},
        {4, 0, 0, 8, 0, 3, 0, 0, 1},
        {7, 0, 0, 0, 2, 0, 0, 0, 6},
        {0, 6, 0, 0, 0, 0, 2, 8, 0},
        {0, 0, 0, 4, 1, 9, 0, 0, 5},
        {0, 0, 0, 0, 8, 0, 0, 7, 9}
    };
    auto sol1 = solveSudoku(puzzle1);
    // Verify that the puzzle1 solution is valid and all cells are nonzero.
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            assert(sol1[i][j] != 0);
        }
    }

    // Example 2: An already solved board should return the same board.
    std::vector<std::vector<int>> solvedBoard = {
        {5, 3, 4, 6, 7, 8, 9, 1, 2},
        {6, 7, 2, 1, 9, 5, 3, 4, 8},
        {1, 9, 8, 3, 4, 2, 5, 6, 7},
        {8, 5, 9, 7, 6, 1, 4, 2, 3},
        {4, 2, 6, 8, 5, 3, 7, 9, 1},
        {7, 1, 3, 9, 2, 4, 8, 5, 6},
        {9, 6, 1, 5, 3, 7, 2, 8, 4},
        {2, 8, 7, 4, 1, 9, 6, 3, 5},
        {3, 4, 5, 2, 8, 6, 1, 7, 9}
    };
    auto solSolved = solveSudoku(solvedBoard);
    assert(solSolved == solvedBoard);

    // Example 3: An unsolvable board (contradictory pre-filled numbers)
    std::vector<std::vector<int>> unsolvable = {
        {1, 2, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0}
    };
    auto solUnsolvable = solveSudoku(unsolvable);
    // Note: This is actually solvable in general; to force unsolvable, we need a conflicting board.
    // For demonstration, we use a board with a known conflict:
    std::vector<std::vector<int>> conflictBoard = {
        {1, 1, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0}
    };
    auto solConflict = solveSudoku(conflictBoard);
    assert(solConflict == conflictBoard); // unsolvable, returns original

    // Example 4: A single empty cell that can be filled uniquely.
    std::vector<std::vector<int>> nearlySolved = solvedBoard;
    nearlySolved[0][0] = 0;
    auto solNearly = solveSudoku(nearlySolved);
    assert(solNearly[0][0] == 5);
    assert(solNearly == solvedBoard);

    return 0;
}

#include <vector>

// Solves a 9x9 Sudoku puzzle using backtracking.
// The input board uses 0 to represent empty cells.
// If a solution exists, returns a solved copy; otherwise, returns the original board unchanged.
std::vector<std::vector<int>> solveSudoku(const std::vector<std::vector<int>>& initialBoard) {
    // Create a mutable copy of the board.
    std::vector<std::vector<int>> board = initialBoard;
    const int size = 9;

    // Check if placing 'num' at (row, col) is valid according to Sudoku rules.
    auto isValid = [&](int row, int col, int num) {
        for (int i = 0; i < size; ++i) {
            if (board[row][i] == num || board[i][col] == num) {
                return false;
            }
        }
        int startRow = (row / 3) * 3;
        int startCol = (col / 3) * 3;
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                if (board[startRow + i][startCol + j] == num) {
                    return false;
                }
            }
        }
        return true;
    };

    // Recursive backtracking solver. Returns true if the board is solved.
    std::function<bool()> backtrack = [&]() -> bool {
        for (int row = 0; row < size; ++row) {
            for (int col = 0; col < size; ++col) {
                if (board[row][col] == 0) {
                    for (int num = 1; num <= 9; ++num) {
                        if (isValid(row, col, num)) {
                            board[row][col] = num;
                            if (backtrack()) {
                                return true;
                            }
                            board[row][col] = 0; // backtrack
                        }
                    }
                    return false; // no valid number for this cell
                }
            }
        }
        return true; // no empty cell found, solved
    };

    if (backtrack()) {
        return board;
    }
    return initialBoard; // unsolvable, return original board
}

// The solution uses a classic recursive backtracking algorithm. First, create a copy of the input board to work on. Define a helper `findEmpty` that scans for the first cell with value 0; if none exists, the board is solved. For each empty cell, try placing digits 1 through 9. For each candidate digit, check validity by verifying that the digit does not already appear in the same row, same column, or same 3x3 subgrid (computed as `(row/3)*3` and `(col/3)*3`). If valid, place the digit and recursively call the solver. If the recursion returns true, propagate success upward. If the digit fails to lead to a solution, reset the cell to 0 (backtrack) and continue with the next digit. If no digit works, return false. The base case returns true when no empty cell is found. Edge cases include: an already solved board (no zeros), a board that is unsolvable (the function returns a copy of the original), and all cells filled. Time complexity is O(9^(n)) in the worst case where n is the number of empty cells, but pruned heavily by validity checks; auxiliary space is O(1) for the board copy plus O(n) for recursion stack depth (up to 81).
