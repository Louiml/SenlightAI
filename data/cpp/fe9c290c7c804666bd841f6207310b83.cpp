Write a C++ function `solveNQueens(int n)` that returns all distinct solutions to the N-Queens puzzle for a given board size `n` (where `n >= 1`). Each solution is represented as a 2D vector of strings, with `"Q"` for a queen and `"."` for an empty cell. The function must return a vector of such boards, where each board contains exactly `n` queens placed such that no two queens attack each other (i.e., no two share the same row, column, or diagonal). The solutions can be returned in any order. You may assume `n` is a valid integer in the range [1, 15] (a typical constraint for backtracking with pruning). Do not use any global variables or external libraries beyond standard C++ headers.

// The problem is a classic backtracking search. We process the board row by row, attempting to place a queen in each column of the current row while ensuring that the column and both diagonals are not already occupied. Use three boolean arrays to track used columns, main diagonals (`row - col + n - 1`), and anti-diagonals (`row + col`). These arrays allow O(1) checks for conflicts. When a queen is placed, mark the corresponding column and diagonals, then recurse to the next row. If all rows are filled, we record the current board state as a valid solution. Otherwise, we backtrack by unmarking and resetting the cell to `"."`. The base case is when `row == n`, meaning a complete placement. Edge cases: `n = 1` yields exactly one solution; `n = 2` and `n = 3` yield zero solutions. Time complexity is O(n!) in the worst case (with pruning it is much faster in practice, around O(n^3) to O(n!) depending on pruning), and space complexity is O(n^2) for the board plus O(n) for the tracking arrays and recursion stack depth O(n).

#include <vector>
#include <string>

// Return all distinct solutions to the N-Queens problem for a board of size n.
// Each solution is a 2D vector of strings with 'Q' for queen and '.' for empty.
std::vector<std::vector<std::string>> solveNQueens(int n) {
    std::vector<std::vector<std::string>> result;
    std::vector<std::string> board(n, std::string(n, '.'));
    std::vector<bool> cols(n, false);
    std::vector<bool> diag1(2 * n - 1, false); // row - col + n - 1
    std::vector<bool> diag2(2 * n - 1, false); // row + col

    // Recursive backtracking function
    auto backtrack = [&](int row, auto&& self) -> void {
        if (row == n) {
            result.push_back(board);
            return;
        }
        for (int col = 0; col < n; ++col) {
            if (cols[col] || diag1[row - col + n - 1] || diag2[row + col]) {
                continue;
            }
            // Place queen
            board[row][col] = 'Q';
            cols[col] = diag1[row - col + n - 1] = diag2[row + col] = true;
            self(row + 1, self);
            // Remove queen
            board[row][col] = '.';
            cols[col] = diag1[row - col + n - 1] = diag2[row + col] = false;
        }
    };

    backtrack(0, backtrack);
    return result;
}

#include <cassert>
#include <vector>
#include <string>

// Function prototype for testing
std::vector<std::vector<std::string>> solveNQueens(int n);

int main() {
    // n=1: one solution with a single queen
    auto r1 = solveNQueens(1);
    assert(r1.size() == 1);
    assert(r1[0] == std::vector<std::string>{"Q"});

    // n=2: no valid placements
    assert(solveNQueens(2).empty());

    // n=3: no valid placements
    assert(solveNQueens(3).empty());

    // n=4: exactly two distinct solutions
    auto r4 = solveNQueens(4);
    assert(r4.size() == 2);
    // Verify each board has exactly 4 queens and no conflicts (simplified check)
    for (const auto& board : r4) {
        int queenCount = 0;
        for (const auto& row : board) {
            for (char c : row) {
                if (c == 'Q') ++queenCount;
            }
        }
        assert(queenCount == 4);
    }

    // n=5: known number of solutions is 10
    auto r5 = solveNQueens(5);
    assert(r5.size() == 10);

    // n=6: known number of solutions is 4
    assert(solveNQueens(6).size() == 4);

    // n=7: known number of solutions is 40
    assert(solveNQueens(7).size() == 40);

    // n=8: known number of solutions is 92 (classic)
    assert(solveNQueens(8).size() == 92);

    // Check that all boards are square and size n
    auto r8 = solveNQueens(8);
    for (const auto& board : r8) {
        assert(board.size() == 8);
        for (const auto& row : board) {
            assert(row.size() == 8);
        }
    }

    return 0;
}
