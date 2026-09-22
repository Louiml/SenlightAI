// Write a C++ function that, given a fixed 8×8 chessboard with one queen already placed at a specific row and column (0-indexed coordinates), returns a `std::vector<std::string>` containing all valid arrangements of 8 queens (one per row) as strings of 8 digits (each digit is 1-indexed column position for that row, with the pre-placed queen at its given column). The results must be sorted in lexicographical order. The function should take two integers `fixedRow` and `fixedCol` (both in [0,7]) and return the list of solution strings.
#include <cassert>
#include <string>
#include <vector>
#include <iostream>

int main() {
    // Test fixed queen at (0,0) – known to have 4 solutions
    auto sol1 = nQueensWithFixedQueen(0, 0);
    assert(sol1.size() == 4);
    // Check lexicographic order and content (first solution is 15863724)
    assert(sol1[0] == "15863724");
    assert(sol1[1] == "16837425");
    assert(sol1[2] == "17468253");
    assert(sol1[3] == "17582463");

    // Test fixed queen at (7,7) – should also have 4 solutions (mirror of above)
    auto sol2 = nQueensWithFixedQueen(7, 7);
    assert(sol2.size() == 4);
    // First solution for (7,7) is 41582736 (mirror)
    assert(sol2[0] == "41582736");

    // Test fixed queen at center (3,4) – known to have solutions; just check count > 0 and all rows have valid columns
    auto sol3 = nQueensWithFixedQueen(3, 4);
    assert(!sol3.empty());
    // Verify each solution is a permutation of 1..8 and fixed column at row 3 is '5'
    for (const auto& str : sol3) {
        assert(str.size() == 8);
        bool seen[8] = {false};
        for (char ch : str) {
            int val = ch - '1';
            assert(val >= 0 && val < 8);
            assert(!seen[val]);
            seen[val] = true;
        }
        assert(str[3] == '5'); // fixed row 3, col 4 → digit '5'
    }

    // Test invalid? Not possible; but check that for any fixed position, solutions list is sorted
    auto sol4 = nQueensWithFixedQueen(2, 6);
    assert(std::is_sorted(sol4.begin(), sol4.end()));

    // Test all solutions are distinct
    auto sol5 = nQueensWithFixedQueen(0, 3);
    for (size_t i = 1; i < sol5.size(); ++i) {
        assert(sol5[i] != sol5[i-1]);
    }

    std::cout << "All tests passed.\n";
    return 0;
}
#include <string>
#include <vector>
#include <algorithm>

// Return all valid 8-queen arrangements with a queen fixed at (fixedRow, fixedCol).
// Each arrangement is a string of 8 digits (1-indexed column per row).
std::vector<std::string> nQueensWithFixedQueen(int fixedRow, int fixedCol) {
    std::vector<std::string> solutions;
    std::vector<int> cols(8, -1);        // cols[row] = column index (0-7) for each row
    std::vector<bool> colUsed(8, false); // columns in use
    std::vector<bool> diag1(15, false);  // row - col + 7
    std::vector<bool> diag2(15, false);  // row + col

    // Place the fixed queen
    cols[fixedRow] = fixedCol;
    colUsed[fixedCol] = true;
    diag1[fixedRow - fixedCol + 7] = true;
    diag2[fixedRow + fixedCol] = true;

    // Backtracking helper
    // row: current row to place a queen (0-7)
    // placed: number of queens placed so far (excluding fixed queen)
    std::function<void(int, int)> backtrack = [&](int row, int placed) {
        if (placed == 8) { // all 8 queens placed (including fixed)
            std::string sol;
            for (int c : cols) {
                sol += char('1' + c);
            }
            solutions.push_back(sol);
            return;
        }
        if (row == fixedRow) {
            // Skip the row with fixed queen, continue to next row
            backtrack(row + 1, placed);
            return;
        }
        if (row >= 8) return; // out of rows, shouldn't happen if placed == 8

        for (int col = 0; col < 8; ++col) {
            if (!colUsed[col] && !diag1[row - col + 7] && !diag2[row + col]) {
                cols[row] = col;
                colUsed[col] = true;
                diag1[row - col + 7] = true;
                diag2[row + col] = true;
                backtrack(row + 1, placed + 1);
                diag2[row + col] = false;
                diag1[row - col + 7] = false;
                colUsed[col] = false;
                cols[row] = -1; // optional reset
            }
        }
    };

    backtrack(0, 1); // start with 1 queen already placed (fixed)
    // The recursion naturally generates solutions in lexicographic order
    // since we iterate col from 0 to 7 and rows in order.
    return solutions;
}
// The task is a classic N-Queens variant with one queen pre-placed. The algorithm uses recursive backtracking row by row. At each row, we iterate through columns that are not attacked by already-placed queens. A queen attacks same row, same column, and both diagonals. We track used columns using a `bool` array, and diagonals using two `bool` arrays: one for difference `row - col` (offset by 7 to make non-negative) and one for sum `row + col` (both in range 0–14). When we reach the fixed row, we do not place a new queen but skip to the next row, because the fixed queen is already there. We build the column sequence as we go; when all 8 rows are processed, we convert the column list to a string of digits (1-indexed) and store it. Since we explore columns in increasing order and rows in fixed order, the resulting strings are naturally generated in lexicographical order. Edge cases: the fixed queen may be at the first or last row; we must not attempt to place another queen there. Also, the fixed queen itself is placed before recursion starts, column and diagonals marked. Time complexity is O(N!) in the worst case, but for N=8 it is small (fewer than 100 valid solutions). Space complexity is O(N) for recursion stack plus O(N) for storage.
