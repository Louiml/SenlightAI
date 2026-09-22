Write a C++ function named `countNQueensSolutions` that takes an integer `n` (where `1 ≤ n ≤ 15`) and returns the number of distinct ways to place `n` queens on an `n × n` chessboard such that no two queens attack each other. A queen attacks another if they share the same row, column, or diagonal. The function must return an `int` representing the total count of valid configurations. Your implementation should use a backtracking approach, placing queens row by row and checking conflicts with previously placed queens only (no need to check rows below the current row). The solution must be self-contained and not use any global mutable state; all state must be local to the function and its helper routines.
The problem is the classic N-Queens counting variant. We place queens row by row starting from the top. For each row, we try each column (0 to n-1) and check if placing a queen there is safe with respect to queens already placed in the previous rows. The safety check must verify three things: (1) no queen in the same column above, (2) no queen on the upper-left diagonal, and (3) no queen on the upper-right diagonal. We do not need to check below the current row because no queens have been placed there yet. When we reach a row index equal to `n`, that means we have successfully placed all `n` queens, so we increment a counter. Otherwise, for each safe column, we place a queen (mark the board cell with `'Q'`), recursively proceed to the next row, and then remove the queen (mark the cell with `'.'`) to backtrack.

Important edge cases: `n = 1` trivially returns 1 (one queen on a 1×1 board). For `n = 0` (not in the constraints) we could return 0, but since the task says `n ≥ 1`, we don't need to handle it. The algorithm’s time complexity is O(n!) in the worst case because the number of valid placements grows roughly factorially (the exact number is known to be sub-factorial, but the backtracking explores a tree of depth n with branching factor decreasing). Space complexity is O(n²) for the board plus O(n) for the recursion stack (since we go n levels deep). We can optimize space by using arrays for column and diagonal checks, but the board approach is straightforward and matches the given snippet's style.
#include <vector>
#include <string>

// Helper: check if placing a queen at (row, col) is safe given the current board.
// We only need to check rows above (0..row-1) because we place row by row.
bool isSafe(const std::vector<std::string>& board, int row, int col) {
    int n = board.size();
    // Check column upwards
    for (int i = row; i >= 0; --i) {
        if (board[i][col] == 'Q') return false;
    }
    // Check upper-left diagonal
    for (int i = row, j = col; i >= 0 && j >= 0; --i, --j) {
        if (board[i][j] == 'Q') return false;
    }
    // Check upper-right diagonal
    for (int i = row, j = col; i >= 0 && j < n; --i, ++j) {
        if (board[i][j] == 'Q') return false;
    }
    return true;
}

// Recursive backtracking helper
void solveNQueens(std::vector<std::string>& board, int row, int& count) {
    int n = board.size();
    if (row == n) {
        ++count;
        return;
    }
    for (int col = 0; col < n; ++col) {
        if (isSafe(board, row, col)) {
            board[row][col] = 'Q';
            solveNQueens(board, row + 1, count);
            board[row][col] = '.';
        }
    }
}

// Main function: returns the number of distinct solutions for n-queens.
int countNQueensSolutions(int n) {
    std::vector<std::string> board(n, std::string(n, '.'));
    int count = 0;
    solveNQueens(board, 0, count);
    return count;
}
#include <cassert>

int main() {
    assert(countNQueensSolutions(1) == 1);
    assert(countNQueensSolutions(2) == 0);
    assert(countNQueensSolutions(3) == 0);
    assert(countNQueensSolutions(4) == 2);
    assert(countNQueensSolutions(5) == 10);
    assert(countNQueensSolutions(6) == 4);
    assert(countNQueensSolutions(7) == 40);
    assert(countNQueensSolutions(8) == 92);
    assert(countNQueensSolutions(9) == 352);
    assert(countNQueensSolutions(10) == 724);
    // Note: For n=11 and above, the count grows quickly; testing up to 10 is sufficient.
    return 0;
}
