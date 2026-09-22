// Write a C++ function `int countNQueensSolutions(int n)` that returns the number of distinct solutions to the classic N-Queens problem for a board of size `n × n`, where queens must be placed so that no two attack each other (same row, column, or diagonal). The function should accept `n` in the range `1 ≤ n ≤ 12`; for `n = 1` the answer is `1`, for `n = 2` and `n = 3` it is `0`, and for larger values it grows accordingly (e.g., `n=4`→`2`, `n=8`→`92`). Implement backtracking with a 1‑based index array to track column positions per row, and count only complete placements.

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
    return 0;
}

#include <vector>
#include <cstdlib>

// Count the number of solutions to the N-Queens problem for a given board size.
int countNQueensSolutions(int n) {
    std::vector<int> board(n + 1, 0);
    int count = 0;

    // Check if placing a queen at (row, col) is safe given existing placements.
    auto isSafe = [&](int row, int col) -> bool {
        for (int i = 1; i < row; ++i) {
            if (board[i] == col || std::abs(board[i] - col) == std::abs(i - row)) {
                return false;
            }
        }
        return true;
    };

    // Recursive backtracking helper.
    void backtrack(int row) {
        if (row > n) {
            ++count;
            return;
        }
        for (int col = 1; col <= n; ++col) {
            if (isSafe(row, col)) {
                board[row] = col;
                backtrack(row + 1);
                board[row] = 0;
            }
        }
    };

    backtrack(1);
    return count;
}

// The solution uses recursive backtracking. We maintain an array `board[1..n]` where `board[row]` stores the column of the queen in that row. The `place(row, col)` function checks safety by verifying that no previously placed queen is in the same column (`board[i] == col`) or on the same diagonal (`abs(board[i] - col) == abs(i - row)`). The recursion tries every column for the current row; if safe, it places the queen and recurses to the next row. When `row` exceeds `n`, a complete solution is found, and we increment the counter. For edge cases: `n=1` has one solution, and `n=2`/`n=3` return zero (backtracking naturally finds none). The algorithm’s time complexity is \(O(n!)\) in the worst case (but with strong pruning it is much faster in practice; for `n=12` it runs in a fraction of a second). Space complexity is \(O(n)\) for the recursion stack and the board array.
