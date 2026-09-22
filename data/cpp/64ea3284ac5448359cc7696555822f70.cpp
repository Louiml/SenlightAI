// Write a C++ function `std::vector<std::vector<int>> solveNQueens(int n)` that returns all distinct solutions to the N-Queens puzzle for a given board size `n`. Each solution is represented as a vector of integers of length `n`, where the integer at index `i` (0-based) indicates the column number (1-based) where a queen is placed in row `i`. The function must return solutions in lexicographically increasing order (i.e., the natural order of the column sequences). If no solutions exist, return an empty vector. The algorithm must use backtracking with constraint checking for conflicts in the same column, upper-left diagonal, and upper-right diagonal, similar to the classic N-Queens approach. Ensure the function works for `n` ranging from 1 to 12, and handle edge cases such as `n = 1` (one solution) and `n = 0` (return empty).

#include <cassert>
#include <vector>

// The solution function is defined above; here we test it.
int main() {
    // n = 1: single solution with queen at (0,0) -> column 1
    auto sol1 = solveNQueens(1);
    assert(sol1.size() == 1);
    assert(sol1[0] == std::vector<int>{1});

    // n = 2: no solutions
    auto sol2 = solveNQueens(2);
    assert(sol2.empty());

    // n = 3: no solutions
    auto sol3 = solveNQueens(3);
    assert(sol3.empty());

    // n = 4: two solutions: [2 4 1 3] and [3 1 4 2]
    auto sol4 = solveNQueens(4);
    assert(sol4.size() == 2);
    assert(sol4[0] == std::vector<int>({2, 4, 1, 3}));
    assert(sol4[1] == std::vector<int>({3, 1, 4, 2}));

    // n = 5: total solutions = 10, ensure size and lexicographic ordering
    auto sol5 = solveNQueens(5);
    assert(sol5.size() == 10);
    // first solution in lexicographic order is [1 3 5 2 4]
    assert(sol5[0] == std::vector<int>({1, 3, 5, 2, 4}));
    // last solution is [4 2 5 3 1]
    assert(sol5[9] == std::vector<int>({4, 2, 5, 3, 1}));

    // n = 6: total solutions = 4
    auto sol6 = solveNQueens(6);
    assert(sol6.size() == 4);

    // n = 0: empty
    auto sol0 = solveNQueens(0);
    assert(sol0.empty());

    // n = -1: empty
    auto solNeg = solveNQueens(-1);
    assert(solNeg.empty());

    // n = 8: known count is 92, check size only
    auto sol8 = solveNQueens(8);
    assert(sol8.size() == 92);
}

#include <vector>

// Solves the N-Queens problem and returns all solutions as column positions (1-based) per row.
// The outer vector contains each solution; each inner vector has length n, where the value at index i is the column (1-based) of the queen in row i.
// Returns an empty vector if n <= 0 or no solutions exist.
std::vector<std::vector<int>> solveNQueens(int n) {
    std::vector<std::vector<int>> ans;
    if (n <= 0) {
        return ans;
    }
    std::vector<std::vector<int>> board(n, std::vector<int>(n, 0));
    std::vector<int> current;

    // Helper lambda for checking if placing a queen at (row, col) is valid.
    auto isValid = [&](int row, int col) -> bool {
        // Check upward in the same column
        for (int r = row - 1; r >= 0; --r) {
            if (board[r][col] != 0) {
                return false;
            }
        }
        // Check upper-left diagonal
        for (int r = row - 1, c = col - 1; r >= 0 && c >= 0; --r, --c) {
            if (board[r][c] != 0) {
                return false;
            }
        }
        // Check upper-right diagonal
        for (int r = row - 1, c = col + 1; r >= 0 && c < n; --r, ++c) {
            if (board[r][c] != 0) {
                return false;
            }
        }
        return true;
    };

    // Recursive backtracking function
    std::function<void(int)> backtrack = [&](int row) {
        if (row == n) {
            ans.push_back(current);
            return;
        }
        for (int col = 0; col < n; ++col) {
            if (isValid(row, col)) {
                board[row][col] = 1;
                current.push_back(col + 1); // 1-based column
                backtrack(row + 1);
                current.pop_back();
                board[row][col] = 0;
            }
        }
    };

    backtrack(0);
    return ans;
}

// The problem is a classic backtracking problem. We place queens row by row. For each row, we try each column from 0 to n-1. Before placing a queen, we check if the position is valid by verifying that no queen exists in the same column above (checking rows above in the same column), and no queen exists on the upper-left diagonal (decrementing both row and column), and no queen exists on the upper-right diagonal (decrementing row, incrementing column). This is done by traversing upward from the current position. If a placement is valid, we record the column (1-based) in a temporary vector, place a queen on the board, recurse to the next row, and upon returning, remove the queen and backtrack. When the row index reaches `n`, we have found a valid arrangement, and we push the current temporary vector to the answer. The board is represented as an `n x n` matrix of integers (0 for empty, 1 for queen). Important edge cases: `n = 1` works because there are no conflicts. `n = 0` should return an empty vector (no rows to place). The algorithm explores all possibilities, and because we iterate columns in increasing order, the solutions are generated in lexicographic order naturally. Time complexity: In the worst case, the number of valid solutions is roughly `O(n!)`, and each validity check takes `O(n)` time, so overall time is `O(n * n!)` in the worst case (though the number of actual solutions is much smaller for moderate `n`). Space complexity: Besides the answer storage, we use `O(n^2)` for the board and `O(n)` for the recursion stack and temporary vector, so `O(n^2)` auxiliary space.
