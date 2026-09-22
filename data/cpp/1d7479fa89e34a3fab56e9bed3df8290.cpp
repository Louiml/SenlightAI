/*
Write a C++ function `placeQueensCount(int n, int k)` that returns the number of distinct ways to place exactly `k` non-attacking queens on an `n x n` chessboard. Two queens attack each other if they share the same row, column, or diagonal. The function must handle arbitrary values of `n` and `k` (including `k=0` and `k>n`), and must not use any global variables for counting. The result can be large, so return it as `long long`. You may assume `n >= 1` and `k >= 0`, but if `k > n`, the function should return `0` because each row can hold at most one queen in a valid placement.
*/

#include <vector>

// Count the number of ways to place k non-attacking queens on an n x n board.
// Uses backtracking row by row. Returns the count as a long long.
long long placeQueensCount(int n, int k) {
    if (k == 0) return 1;
    if (k > n) return 0;
    if (n <= 0) return 0;

    std::vector<bool> colUsed(n, false);
    std::vector<bool> diag1Used(2 * n - 1, false); // row - col + (n - 1)
    std::vector<bool> diag2Used(2 * n - 1, false); // row + col

    long long count = 0;

    // Recursive lambda to explore placements.
    // 'row' is the current row index, 'queensPlaced' is number of queens placed so far.
    auto backtrack = [&](auto&& self, int row, int queensPlaced) -> void {
        if (queensPlaced == k) {
            ++count;
            return;
        }
        if (row == n) {
            return;
        }

        // Option 1: skip this row entirely (place no queen here).
        self(self, row + 1, queensPlaced);

        // Option 2: try placing a queen in each column of this row.
        for (int col = 0; col < n; ++col) {
            int d1 = row - col + n - 1;
            int d2 = row + col;
            if (!colUsed[col] && !diag1Used[d1] && !diag2Used[d2]) {
                colUsed[col] = true;
                diag1Used[d1] = true;
                diag2Used[d2] = true;

                self(self, row + 1, queensPlaced + 1);

                colUsed[col] = false;
                diag1Used[d1] = false;
                diag2Used[d2] = false;
            }
        }
    };

    backtrack(backtrack, 0, 0);
    return count;
}

#include <cassert>

// Declare the function to test (since the solution file has no main, we include it here).
// In a real test, you would include the solution header or copy the function.
long long placeQueensCount(int n, int k);

int main() {
    // Edge case: k = 0 => exactly one way (place nothing)
    assert(placeQueensCount(4, 0) == 1);
    assert(placeQueensCount(1, 0) == 1);

    // k > n => impossible
    assert(placeQueensCount(3, 4) == 0);
    assert(placeQueensCount(5, 6) == 0);

    // Classic N-Queens: n=1, k=1 => 1 way
    assert(placeQueensCount(1, 1) == 1);

    // n=2, k=2 => 0 ways (no two queens fit)
    assert(placeQueensCount(2, 2) == 0);

    // n=4, k=1 => 16 ways (each square)
    assert(placeQueensCount(4, 1) == 16);

    // n=4, k=2 => number of ways to place 2 non-attacking queens
    // Known value: on 4x4, 2 queens without attacking can be placed in 44 ways.
    assert(placeQueensCount(4, 2) == 44);

    // n=4, k=4 => classic 4-queens solutions: 2 ways
    assert(placeQueensCount(4, 4) == 2);

    // n=8, k=8 => classic 8-queens solutions: 92 ways
    assert(placeQueensCount(8, 8) == 92);

    // n=5, k=3 => count should be 160 (known combinatorial value)
    assert(placeQueensCount(5, 3) == 160);

    return 0;
}

// The solution uses backtracking with row-by-row placement. Because no two queens can share a row, we place at most one queen per row. The recursion processes rows from 0 to `n-1`, and at each row we either skip the row (placing no queen there) or try every column where a queen would be safe. Safety is checked by looking at all previously placed rows: same column, or either diagonal (difference or sum of row and column indices matching). Once we have placed exactly `k` queens, we increment the count and return. If we run out of rows before placing `k` queens, we return without counting. Edge cases: `k == 0` always yields exactly 1 way (placing no queens); `k > n` yields 0 because we cannot place more queens than rows; and if `k` equals `n`, the classic N-Queens count is returned. The time complexity is exponential in the worst case, but for moderate `n` it is acceptable; using symmetry reductions would help but is not required. The space complexity is `O(n)` for the recursion stack and `O(n^2)` for the board if we keep a board matrix, but we can optimize to `O(n)` by storing only column and diagonal occupancy arrays. Here we use simple arrays of booleans for columns, main diagonals, and anti-diagonals to make space `O(n)`.
