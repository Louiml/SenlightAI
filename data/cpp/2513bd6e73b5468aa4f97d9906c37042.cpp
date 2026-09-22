Write a C++ function that, given an integer `n` representing the size of an \(n \times n\) chessboard, returns the total number of distinct ways to place `n` queens on the board such that no two queens attack each other (i.e., no two share the same row, column, or diagonal). The function should be named `countNQueensSolutions` and must work for any `n` from 1 to 12 inclusive. For a standard 8×8 board, the result should be 92. The solution must use backtracking with a 2D board representation or an equivalent 1D array to track queen positions, and must be efficient enough to compute results for `n = 12` within a reasonable time.

#include <cassert>

int main() {
    // Known results for n = 1..12 (verified values)
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
    assert(countNQueensSolutions(11) == 2680);
    // For n=12, result is 14200 (may take a few seconds)
    assert(countNQueensSolutions(12) == 14200);
    // Invalid input returns 0
    assert(countNQueensSolutions(0) == 0);
    return 0;
}

#include <vector>

// Count the number of distinct solutions for the N-Queens problem.
int countNQueensSolutions(int n) {
    if (n <= 0) return 0;          // Invalid board size
    std::vector<int> colPos(n, -1); // colPos[row] = column where queen is placed for that row
    int count = 0;

    // Depth-first search to place queens row by row.
    // Uses a lambda for recursion to capture variables by reference.
    std::function<void(int)> backtrack = [&](int row) {
        if (row == n) {
            ++count;  // All queens placed
            return;
        }
        for (int col = 0; col < n; ++col) {
            bool safe = true;
            // Check conflicts with previously placed queens (rows 0..row-1)
            for (int prevRow = 0; prevRow < row; ++prevRow) {
                int prevCol = colPos[prevRow];
                if (prevCol == col ||                     // same column
                    prevRow - prevCol == row - col ||     // same main diagonal
                    prevRow + prevCol == row + col) {     // same anti-diagonal
                    safe = false;
                    break;
                }
            }
            if (safe) {
                colPos[row] = col;
                backtrack(row + 1);
                colPos[row] = -1;  // Backtrack
            }
        }
    };

    backtrack(0);
    return count;
}

// The problem is the classic N-Queens counting problem, solved via recursive backtracking. The main idea is to place queens row by row, starting from row 0. For each row, try every column from 0 to `n-1`. Before placing a queen at `(row, col)`, verify that no previously placed queen (in rows 0 to `row-1`) shares the same column, the same upward diagonal (where the difference `row - col` is constant), or the same downward diagonal (where the sum `row + col` is constant). If the position is safe, place the queen and recurse to the next row. When `row == n`, all queens are placed successfully, so increment the solution counter. After returning from the recursion, remove the queen (backtrack) to try other columns. 
//
// Edge cases: For `n = 1`, there is exactly one solution (a single queen on a 1×1 board). For `n = 2` and `n = 3`, there are zero solutions. The algorithm must handle small `n` correctly. Time complexity is \(O(n!)\) in the worst case for the backtracking search, though pruning reduces it significantly; for `n = 12` the number of recursive calls is on the order of a few million, which is acceptable. Space complexity is \(O(n)\) for the recursion stack and the board storage (if using a 1D array of column positions per row, or `O(n^2)` if using a full 2D board, but \(O(n)\) is preferable).
