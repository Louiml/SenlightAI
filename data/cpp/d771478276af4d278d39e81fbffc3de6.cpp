// Write a C++ function `void chess_puzzle(int n)` that solves the classic N-Queens problem for an \(n \times n\) chessboard. The function must find *any* valid placement of \(n\) queens such that no two queens attack each other (i.e., no two share the same row, column, or diagonal). The output should print each queen's row and column as "row column" on its own line, starting with row 0 and ending at row \(n-1\). If no solution exists, print exactly `"No solution exists\n"`. The function must handle edge cases: \(n = 0\) (no queens, print nothing), \(n = 1\) (print `0 0`), and larger \(n\) where solutions always exist for \(n \ne 2\) and \(n \ne 3\). For \(n = 2\) or \(n = 3\), no solution exists, so print the no-solution message. The function takes sole responsibility for output; do not call `main` or any other helper in your solution.
The solution uses a standard backtracking algorithm. We maintain a vector `queens` of size \(n\) where `queens[row]` stores the column index of the queen placed in that row (initialized to -1 for unfilled rows). The recursive function `solveNQueens(row, n, queens)` attempts to place a queen in each column of the current row, checking safety with `isSafe(row, col, queens)`. The safety check iterates over all previously placed rows `i = 0 ... row-1` and verifies: (1) no same column (`queens[i] == col`), and (2) no diagonal conflict (`abs(queens[i] - col) == abs(i - row)`). If a safe column is found, we tentatively place the queen, recurse to the next row, and if that recursion returns true, we propagate the success up. If all columns fail, we backtrack by resetting `queens[row] = -1` and return false. When `row == n`, all queens are placed, so return true. The main function then prints each row's index and column. For \(n = 2\) and \(n = 3\), the algorithm will exhaust all possibilities and return false, triggering the no-solution message. For \(n = 0\), the recursion immediately reaches `row == n` with an empty vector, printing nothing (as required). The time complexity is \(O(n!)\) in the worst case (since the number of permutations to try is factorial), but in practice it prunes many branches. The space complexity is \(O(n)\) for the recursion stack and the queens vector combined. Edge cases handled: no solution for n=2,3; n=0 output nothing; n=1 output `0 0`.
#include <iostream>
#include <vector>
#include <cmath>
#include <string>

// Checks if placing a queen at (row, col) conflicts with any previously placed queens.
bool isSafe(int row, int col, const std::vector<int>& queens) {
    for (int i = 0; i < row; ++i) {
        int qCol = queens[i];
        // Same column or same diagonal (absolute difference of columns equals row difference)
        if (qCol == col || std::abs(qCol - col) == std::abs(i - row)) {
            return false;
        }
    }
    return true;
}

// Recursive backtracking to place queens row by row.
bool solveNQueens(int row, int n, std::vector<int>& queens) {
    if (row == n) {
        return true; // All rows have a queen placed
    }

    for (int col = 0; col < n; ++col) {
        if (isSafe(row, col, queens)) {
            queens[row] = col;  // Place queen
            if (solveNQueens(row + 1, n, queens)) {
                return true;    // Successful placement found
            }
            queens[row] = -1;   // Backtrack
        }
    }
    return false; // No valid column in this row
}

// Solves the N-Queens puzzle for an n x n board and prints output.
void chess_puzzle(int n) {
    std::vector<int> queens(n, -1); // queens[row] = column, -1 if unset

    if (n >= 0 && solveNQueens(0, n, queens)) {
        for (int i = 0; i < n; ++i) {
            std::cout << i << " " << queens[i] << "\n";
        }
    } else if (n > 0) {
        std::cout << "No solution exists\n";
    }
    // For n == 0, nothing is printed (no queens to place).
}
#include <cassert>
#include <sstream>
#include <iostream>
#include <vector>
#include <cmath>

// Declare the function to test (with a wrapper to capture output)
void chess_puzzle(int n);

// Helper: run chess_puzzle and redirect cout to capture output
std::string run_puzzle(int n) {
    std::ostringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    chess_puzzle(n);
    std::cout.rdbuf(old);
    return buffer.str();
}

// Helper: verify a solution string against constraints for N-Queens
bool is_valid_solution(const std::string& output, int n) {
    if (n == 0) return output.empty();
    if (n == 2 || n == 3) return output == "No solution exists\n";
    std::istringstream iss(output);
    std::vector<int> cols(n, -1);
    std::vector<bool> used_col(n, false);
    std::vector<bool> used_row(n, false);
    int row, col;
    int count = 0;
    while (iss >> row >> col) {
        if (row < 0 || row >= n || col < 0 || col >= n) return false;
        if (used_row[row] || used_col[col]) return false;
        used_row[row] = true; used_col[col] = true;
        for (int r = 0; r < row; ++r) {
            if (cols[r] != -1 && (cols[r] == col || std::abs(cols[r] - col) == std::abs(r - row))) 
                return false;
        }
        cols[row] = col;
        count++;
    }
    return count == n;
}

int main() {
    // Test edge cases
    assert(run_puzzle(0) == "");
    assert(run_puzzle(1) == "0 0\n");
    assert(run_puzzle(2) == "No solution exists\n");
    assert(run_puzzle(3) == "No solution exists\n");

    // Test n=4 (known solutions exist)
    std::string out4 = run_puzzle(4);
    assert(is_valid_solution(out4, 4));

    // Test n=8 (classic puzzle)
    std::string out8 = run_puzzle(8);
    assert(is_valid_solution(out8, 8));

    // Test n=5,6,7
    assert(is_valid_solution(run_puzzle(5), 5));
    assert(is_valid_solution(run_puzzle(6), 6));
    assert(is_valid_solution(run_puzzle(7), 7));

    // Test output format: each line has "row column"
    std::istringstream iss(out4);
    int row, col;
    int lines = 0;
    while (iss >> row >> col) {
        lines++;
        assert(row >= 0 && row < 4 && col >= 0 && col < 4);
    }
    assert(lines == 4);

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
