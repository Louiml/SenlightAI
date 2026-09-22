// Write a C++ function `std::vector<std::vector<std::string>> placeQueens(int n)` that solves the classic N-Queens problem: place `n` queens on an `n x n` chessboard so that no two queens attack each other (no two queens share the same row, column, or diagonal). The function should return a vector containing all distinct solutions. Each solution is represented as a vector of `n` strings, where each string has length `n` and contains `'Q'` for a queen and `'.'` for an empty square. The rows should be in order from top (row 0) to bottom (row `n-1`), and columns from left (column 0) to right (column `n-1`). For `n = 4`, the two valid solutions (in any order) are:
// ```
// {"..Q.", "Q...", "...Q", ".Q.."}    and    {".Q..", "...Q", "Q...", "..Q."}
// ```
// The function should work for any `n >= 1`. For `n = 1`, return `{{"Q"}}`; for `n = 2` and `n = 3`, return an empty vector. The output order of solutions does not matter. Do not include `main` in your solution.

The standard backtracking algorithm places queens row by row. Start at row 0 and for each row try every column from 0 to `n-1`. Before placing a queen at `(row, col)`, check that no previously placed queen attacks that square. Because we place row by row, we only need to check three directions: upward vertical (same column in any row above), upper-left diagonal, and upper-right diagonal. The provided snippet's `iSafe` function also checks downward and left/right directions, but that's redundant and inefficient; we can simplify to only check the upper directions since we haven't placed any queens below the current row yet. The board is a 2D grid of characters, initially filled with `'.'`. When a safe spot is found, place `'Q'`, recurse to the next row, then remove the queen (backtrack). If `row == n`, we've placed all queens, so convert the board to the required string representation and add to the answer. Edge cases: `n=1` works; `n=2` and `3` produce no solutions because a 2x2 or 3x3 board cannot fit queens without attacking; we just return an empty vector. Time complexity: For an `n`-queens solution, the worst-case time is `O(n!)` because there are at most `n` choices for the first queen, `n-1` for the second, etc., and we prune with safe checks. Space complexity: `O(n^2)` for the board plus `O(n)` for the recursion stack and `O(n^2)` for storing the output (but the output is the required result, not auxiliary). The auxiliary space for the algorithm itself (excluding the answer vector) is `O(n^2)` for the board plus `O(n)` recursion stack.

#include <vector>
#include <string>

// Solves the N-Queens problem, returning all distinct board configurations.
std::vector<std::vector<std::string>> placeQueens(int n) {
    std::vector<std::vector<std::string>> result;
    std::vector<std::string> board(n, std::string(n, '.'));

    // Check if placing a queen at (row, col) is safe relative to rows above.
    auto isSafe = [&](int row, int col) {
        // Check vertical upward
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
    };

    // Backtracking function: place queens row by row.
    std::function<void(int)> backtrack = [&](int row) {
        if (row == n) {
            result.push_back(board);
            return;
        }
        for (int col = 0; col < n; ++col) {
            if (isSafe(row, col)) {
                board[row][col] = 'Q';
                backtrack(row + 1);
                board[row][col] = '.';
            }
        }
    };

    backtrack(0);
    return result;
}

#include <cassert>
#include <vector>
#include <string>
#include <algorithm>

// Include the solution function here (or link it, but for self-contained test we include it above).

int main() {
    // n = 1: one solution with a single queen
    auto sol1 = placeQueens(1);
    assert(sol1.size() == 1);
    assert(sol1[0] == std::vector<std::string>{"Q"});

    // n = 2: no solutions
    assert(placeQueens(2).empty());

    // n = 3: no solutions
    assert(placeQueens(3).empty());

    // n = 4: exactly 2 solutions
    auto sol4 = placeQueens(4);
    assert(sol4.size() == 2);

    // Normalize order: sort lexicographically for comparison
    std::sort(sol4.begin(), sol4.end());
    std::vector<std::vector<std::string>> expected4 = {
        {".Q..", "...Q", "Q...", "..Q."},
        {"..Q.", "Q...", "...Q", ".Q.."}
    };
    std::sort(expected4.begin(), expected4.end());
    assert(sol4 == expected4);

    // n = 5: there are 10 solutions
    assert(placeQueens(5).size() == 10);

    // n = 6: there are 4 solutions
    assert(placeQueens(6).size() == 4);

    // n = 8: there are 92 solutions (quick sanity check only, not full enumeration in this exercise)
    // Uncomment the next line if you want a heavier check (may be slow for some compilers)
    // assert(placeQueens(8).size() == 92);

    // Additional sanity: every solution for n=4 contains exactly 4 'Q's and 12 '.'s
    for (const auto& board : sol4) {
        int queens = 0;
        for (const auto& row : board) {
            for (char c : row) {
                if (c == 'Q') ++queens;
            }
        }
        assert(queens == 4);
        assert(board.size() == 4);
        for (const auto& row : board) assert(row.size() == 4);
    }

    return 0;
}
