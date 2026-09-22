Write a standalone C++ function `countNQueensSolutions(int n)` that returns the number of distinct valid placements of `n` queens on an `n x n` chessboard such that no two queens threaten each other (no same row, column, or diagonal). The function must use a backtracking algorithm that builds the board row by row, placing one queen per row at a column index stored in a vector of size `n`. A placement is valid only if no two queens share a column or diagonal. The function should handle `n = 0` (return 1, since an empty board trivially has one valid placement) and `n = 1` (return 1). It must not print any output; only return the integer count. The function should work for `n` up to 15, returning the exact count without overflow. Use recursion with pruning (checking compatibility only against previously placed queens), not brute‑force enumeration of all permutations.
The solution uses classic backtracking. The board is represented as a vector `cols` of length `n`, where `cols[row]` stores the column index of the queen in that row. We start with `row = 0` and recursively attempt to place a queen in each column of the current row. Before placing, we check whether `(row, col)` conflicts with any already placed queen in rows `0` to `row-1`. A conflict occurs if the columns match, or if the absolute difference in rows equals the absolute difference in columns (diagonal attack). If no conflict, we set `cols[row] = col` and recurse to the next row. If we reach `row == n`, we have found one valid solution and increment the counter. Important edge cases: `n = 0` returns 1 (empty board); `n = 1` returns 1; also note that for `n = 2` and `n = 3` there are zero solutions. The recursion prunes early, so it does not enumerate all permutations. Time complexity is O(n!) in the worst case (number of nodes visited), but with pruning it is much faster in practice; space complexity is O(n) for the recursion stack and the `cols` vector.
#include <vector>
#include <cstdlib>

// Returns the number of distinct valid placements of n queens on an n x n board.
int countNQueensSolutions(int n) {
    if (n == 0) return 1;  // empty board has exactly one valid placement
    std::vector<int> cols(n, -1);  // cols[row] = column of queen in that row, -1 means unassigned
    int count = 0;

    // Recursive lambda for backtracking.
    // row: current row index to place a queen
    std::function<void(int)> backtrack = [&](int row) {
        if (row == n) {
            ++count;  // found a valid full placement
            return;
        }
        for (int col = 0; col < n; ++col) {
            bool safe = true;
            for (int prevRow = 0; prevRow < row; ++prevRow) {
                int prevCol = cols[prevRow];
                // Same column conflict
                if (prevCol == col) {
                    safe = false;
                    break;
                }
                // Diagonal conflict: equal row difference and column difference
                if (std::abs(row - prevRow) == std::abs(col - prevCol)) {
                    safe = false;
                    break;
                }
            }
            if (safe) {
                cols[row] = col;
                backtrack(row + 1);
                cols[row] = -1;  // backtrack
            }
        }
    };

    backtrack(0);
    return count;
}
#include <cassert>

// Test the countNQueensSolutions function.
int main() {
    // Known values for small n
    assert(countNQueensSolutions(0) == 1);
    assert(countNQueensSolutions(1) == 1);
    assert(countNQueensSolutions(2) == 0);
    assert(countNQueensSolutions(3) == 0);
    assert(countNQueensSolutions(4) == 2);
    assert(countNQueensSolutions(5) == 10);
    assert(countNQueensSolutions(6) == 4);
    assert(countNQueensSolutions(7) == 40);
    assert(countNQueensSolutions(8) == 92);

    // Ensure it does not return negative or overflow for a moderately large n
    int result9 = countNQueensSolutions(9);
    assert(result9 > 0 && result9 == 352);

    return 0;
}
