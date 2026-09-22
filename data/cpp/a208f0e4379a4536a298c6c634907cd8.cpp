/*
Write a C++ function named `placeQueens` that, given an integer `n` (where `1 ≤ n ≤ 10`), returns a vector of strings representing a solution to the classic N-Queens problem: place `n` queens on an `n x n` chessboard such that no two queens attack each other (no shared row, column, or diagonal). The board representation should use `'Q'` for a queen and `'.'` for an empty cell, with each string being one row of the board (length `n`), and the vector containing all `n` rows in order. If no solution exists (which occurs only for `n = 2` or `n = 3`), return an empty vector. The function must be self-contained, take only `n` as a parameter, and not rely on any global state or external input/output.
*/

#include <string>
#include <vector>

// Check if placing a queen at (row, col) is safe given the current board.
static bool isSafe(const std::vector<std::string>& board, int row, int col, int n) {
    // Check column above
    for (int i = 0; i < row; ++i) {
        if (board[i][col] == 'Q') return false;
    }
    // Check upper-left diagonal
    for (int i = row - 1, j = col - 1; i >= 0 && j >= 0; --i, --j) {
        if (board[i][j] == 'Q') return false;
    }
    // Check upper-right diagonal
    for (int i = row - 1, j = col + 1; i >= 0 && j < n; --i, ++j) {
        if (board[i][j] == 'Q') return false;
    }
    return true;
}

// Backtracking helper: tries to fill rows from 'row' to n-1.
static bool solve(std::vector<std::string>& board, int row, int n) {
    if (row == n) return true; // All queens placed
    for (int col = 0; col < n; ++col) {
        if (isSafe(board, row, col, n)) {
            board[row][col] = 'Q';
            if (solve(board, row + 1, n)) return true;
            board[row][col] = '.'; // backtrack
        }
    }
    return false;
}

// Main solution function: returns the board or empty vector if no solution.
std::vector<std::string> placeQueens(int n) {
    if (n == 2 || n == 3) return {}; // No solution for these sizes
    std::vector<std::string> board(n, std::string(n, '.'));
    if (solve(board, 0, n)) {
        return board;
    }
    return {};
}

#include <cassert>
#include <string>
#include <vector>

// (The solution function would be placed here or included from above.)

int main() {
    // Test n=1: one queen at [0][0]
    auto b1 = placeQueens(1);
    assert(b1.size() == 1);
    assert(b1[0] == "Q");

    // Test n=2: no solution -> empty vector
    assert(placeQueens(2).empty());

    // Test n=3: no solution -> empty vector
    assert(placeQueens(3).empty());

    // Test n=4: known solution is one valid arrangement
    auto b4 = placeQueens(4);
    assert(b4.size() == 4);
    for (const auto& row : b4) {
        assert(row.size() == 4);
        assert(row.find('Q') != std::string::npos); // at least one queen per row
    }
    // Manually verify the classic 4-queens solution: rows 0: ".Q..", 1: "...Q", 2: "Q...", 3: "..Q."
    std::vector<std::string> expected4 = {".Q..", "...Q", "Q...", "..Q."};
    assert(b4 == expected4);

    // Test n=8: should have 8 rows, each with exactly one 'Q' and no conflicts (we'll just check row count and queen count)
    auto b8 = placeQueens(8);
    assert(b8.size() == 8);
    int queenCount = 0;
    for (const auto& row : b8) {
        assert(row.size() == 8);
        // Count 'Q' in this row
        int rowQueens = 0;
        for (char c : row) if (c == 'Q') rowQueens++;
        assert(rowQueens == 1);
        queenCount += rowQueens;
    }
    assert(queenCount == 8);

    // Additional check: ensure no two queens share a column (since rows are unique by construction)
    std::vector<bool> colUsed(8, false);
    for (int r = 0; r < 8; ++r) {
        int c = b8[r].find('Q');
        assert(c != std::string::npos);
        assert(!colUsed[c]);
        colUsed[c] = true;
    }

    // Test n=10: should return a valid board of size 10
    auto b10 = placeQueens(10);
    assert(b10.size() == 10);
    for (const auto& row : b10) {
        assert(row.size() == 10);
    }

    // A simple validity check for n=10: count queens (should be 10)
    int total = 0;
    for (const auto& row : b10) for (char c : row) if (c == 'Q') total++;
    assert(total == 10);
}

// This is a classic backtracking problem. The main algorithm places queens one row at a time, starting from row 0. For each row, try every column in that row, and before placing a queen at `(row, col)`, check if it conflicts with any previously placed queen in the same column, or on either diagonal (both the upward-left and downward-left diagonals). Since we place queens row by row from top to bottom, there is never a conflict in the same row, and we only need to check rows above the current row. If the cell is safe, place the queen, recurse to the next row, and if that recursive call succeeds, return the board; if it fails, remove the queen (backtrack) and try the next column. The recursion base case is when `row == n`, meaning all queens are placed, so return the board. Key edge cases: for `n=1` return a board with one queen; for `n=2` and `n=3` there are no solutions, so return an empty vector. Time complexity is \(O(n!)\) in the worst case because of backtracking (though constrained by `n ≤ 10`), and space complexity is \(O(n^2)\) for the board plus \(O(n)\) for the recursion stack. The function can be implemented with a helper that takes a reference to the board and the current row, returning a boolean to indicate success.
