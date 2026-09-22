Write a C++ function `std::vector<std::vector<std::string>> solveNQueens(int n)` that returns all distinct solutions to the n-queens puzzle, where `n` is a positive integer (1 ≤ n ≤ 9 for reasonable performance). Each solution is represented as a vector of `n` strings, each of length `n`, containing `'Q'` for a queen and `'.'` for an empty cell. The function must place `n` queens on an `n × n` chessboard such that no two queens attack each other (i.e., no two queens share the same row, column, or diagonal). Return all valid board configurations, with boards ordered with lexicographic ordering of the row strings (i.e., the natural order produced by iterating rows top-to-bottom and columns left-to-right). If no solutions exist (e.g., for `n=2` or `n=3`), return an empty vector.

#include <cassert>
#include <vector>
#include <string>

// The solution function is declared above (not repeated here for brevity).

int main() {
    // n = 1: single queen on a 1x1 board
    auto sol1 = solveNQueens(1);
    assert(sol1.size() == 1);
    assert(sol1[0] == std::vector<std::string>{"Q"});

    // n = 2: no solutions
    auto sol2 = solveNQueens(2);
    assert(sol2.empty());

    // n = 3: no solutions
    auto sol3 = solveNQueens(3);
    assert(sol3.empty());

    // n = 4: exactly 2 solutions
    auto sol4 = solveNQueens(4);
    assert(sol4.size() == 2);
    assert(sol4[0] == std::vector<std::string>{".Q..", "...Q", "Q...", "..Q."});
    assert(sol4[1] == std::vector<std::string>{"..Q.", "Q...", "...Q", ".Q.."});

    // n = 5: exactly 10 solutions
    auto sol5 = solveNQueens(5);
    assert(sol5.size() == 10);

    // Verify all boards in sol5 are valid (no conflicts)
    for (const auto& board : sol5) {
        assert(board.size() == 5);
        for (const auto& row : board) {
            assert(row.size() == 5);
            assert(std::count(row.begin(), row.end(), 'Q') == 1);
        }
        // Check columns and diagonals (simplified: just count total queens)
        int total_queens = 0;
        for (const auto& row : board) {
            total_queens += std::count(row.begin(), row.end(), 'Q');
        }
        assert(total_queens == 5);
    }

    return 0;
}

#include <vector>
#include <string>

// Solve the n-queens puzzle and return all distinct board configurations.
// Each board is represented as a vector of n strings, each of length n,
// where 'Q' indicates a queen and '.' indicates an empty cell.
std::vector<std::vector<std::string>> solveNQueens(int n) {
    std::vector<std::vector<std::string>> result;
    std::vector<std::string> board(n, std::string(n, '.'));
    int col_mask = 0;        // bit i set means column i occupied
    int diag1_mask = 0;      // bit for (row + col) diagonals
    int diag2_mask = 0;      // bit for (row - col + n) diagonals

    // Recursive lambda to place queens row by row
    std::function<void(int)> backtrack = [&](int row) {
        if (row == n) {
            result.push_back(board);
            return;
        }
        for (int col = 0; col < n; ++col) {
            int col_bit = 1 << col;
            int d1_bit = 1 << (row + col);
            int d2_bit = 1 << (row - col + n);
            if (col_bit & col_mask || d1_bit & diag1_mask || d2_bit & diag2_mask) {
                continue; // conflict
            }
            // Place queen
            board[row][col] = 'Q';
            col_mask |= col_bit;
            diag1_mask |= d1_bit;
            diag2_mask |= d2_bit;

            backtrack(row + 1);

            // Remove queen
            board[row][col] = '.';
            col_mask &= ~col_bit;
            diag1_mask &= ~d1_bit;
            diag2_mask &= ~d2_bit;
        }
    };

    backtrack(0);
    return result;
}

// The classic backtracking approach is used: place queens row by row (starting from row 0 to row n-1). For each row, try placing a queen in each column column. Maintain three bitmasks to track which columns, which "slash" diagonals (i - j + n), and which "backslash" diagonals (i + j) are already occupied. Since each diagonal has a constant index determined by the row and column, these bitmasks allow O(1) conflict checking. When placing a queen at (row, col), set the corresponding bits; recurse to the next row; after recursion, unset the bits and remove the queen. When row == n, a complete valid solution is found and pushed to the answer vector. Edge cases include n=1 (one solution), n=2 and n=3 (no solutions), and n=0 (not a valid input but could return empty). Time complexity is O(n!) in the worst case (but pruning reduces it significantly), and space complexity is O(n) for the recursion stack plus O(n²) for storing the current board and the answer (which can grow to O(n!·n²) for the output). The bitmask approach avoids extra memory for visited columns/diagonals.
