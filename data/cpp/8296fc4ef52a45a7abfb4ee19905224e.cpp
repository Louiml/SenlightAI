Write a C++ function `void printAllNQueens(int n)` that prints every distinct solution to the N-Queens problem for a given board size `n` (where `1 <= n <= 20`). In each solution, print the queen positions row by row as integers (the column index for each row, 1-indexed), each value right-aligned in a field of width 5, and the entire row followed by a newline. Solutions must be printed in lexicographic order of the column-index sequence. If `n <= 3`, print exactly `no solute!` on a single line (without the field width formatting) and return. The function should not return any value; output is written to `std::cout`. If `n` is larger, the function prints all valid solutions but produces no extra output besides the solution lines.
The task is a classic backtracking search over the rows of an `n`×`n` board. We maintain an array `cols[1..n]` where `cols[row]` stores the column chosen for that row. We recursively place a queen in the next row by trying each column from 1 to `n`. For each candidate column, we check it against all previously placed queens: the new column must differ from every earlier column (no same-column attack) and the row and column differences must not be equal in absolute value (no diagonal attack). If valid, we recurse to the next row. When we reach row `n+1`, we have a complete solution and print it with each column value right-aligned in width 5, followed by a newline. The backtracking naturally generates solutions in lexicographic order because we iterate columns in increasing order at each row. Edge cases: for `n <= 3`, no solutions exist, so we immediately print `no solute!` and return; this includes `n = 1` and `n = 2` as well (the original snippet prints the no-solution message even for `n=1` which has one trivial solution, but the task specification here explicitly says to print the message for all `n <= 3`). Time complexity: the worst-case number of nodes explored is exponentially bounded by the number of permutations of `n` columns, but due to pruning it is roughly `O(n!)` in the worst case (e.g., for `n=10`, ~724 solutions, but the recursion explores many more partial placements). Space complexity: `O(n)` for the column array and recursion stack depth.
#include <iostream>
#include <iomanip>
#include <vector>

// Print all N-Queens solutions for board size n, or "no solute!" if n <= 3.
// Each solution line shows column indices for rows 1..n, each in width 5.
void printAllNQueens(int n) {
    if (n <= 3) {
        std::cout << "no solute!" << std::endl;
        return;
    }

    std::vector<int> cols(n + 1, 0); // cols[row] = column, 1-indexed rows/cols

    // Recursive DFS: place queen in row 'row'
    std::function<void(int)> dfs = [&](int row) {
        if (row == n + 1) {
            for (int r = 1; r <= n; ++r) {
                std::cout << std::setw(5) << cols[r];
            }
            std::cout << std::endl;
            return;
        }

        for (int col = 1; col <= n; ++col) {
            bool safe = true;
            for (int prevRow = 1; prevRow < row; ++prevRow) {
                if (cols[prevRow] == col ||
                    std::abs(row - prevRow) == std::abs(col - cols[prevRow])) {
                    safe = false;
                    break;
                }
            }
            if (safe) {
                cols[row] = col;
                dfs(row + 1);
            }
        }
    };

    dfs(1);
}
#include <cassert>
#include <sstream>
#include <string>

// Redirect cout to a stringstream for testing
void testNQueens(int n, const std::string& expected) {
    std::ostringstream oss;
    std::streambuf* oldBuf = std::cout.rdbuf(oss.rdbuf());
    printAllNQueens(n);
    std::cout.rdbuf(oldBuf);
    assert(oss.str() == expected);
}

int main() {
    testNQueens(1, "no solute!\n");
    testNQueens(2, "no solute!\n");
    testNQueens(3, "no solute!\n");
    testNQueens(4, "    2    4    1    3\n    3    1    4    2\n");
    testNQueens(5, "    1    3    5    2    4\n    1    4    2    5    3\n    2    4    1    3    5\n    2    5    3    1    4\n    3    1    4    2    5\n    3    5    2    4    1\n    4    1    3    5    2\n    4    2    5    3    1\n    5    2    4    1    3\n    5    3    1    4    2\n");
    // Test a larger size (n=6) to ensure correct count and order
    std::ostringstream oss;
    std::streambuf* oldBuf = std::cout.rdbuf(oss.rdbuf());
    printAllNQueens(6);
    std::cout.rdbuf(oldBuf);
    // Count the number of lines (solutions) and verify first line
    std::string output = oss.str();
    int lines = 0;
    for (char c : output) if (c == '\n') ++lines;
    assert(lines == 4); // known: 4 solutions for 6-queens
    assert(output.find("    2    4    6    1    3    5") == 0); // lexicographically first
    return 0;
}
