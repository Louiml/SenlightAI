/*
Write a C++ function named `nQueensConflict` that takes a non-negative integer `M` representing the size of an M×M chessboard (with rows and columns numbered 1 through M) and a vector of `N` coordinate pairs, each pair being a `std::pair<int,int>` (row, column), representing the positions of queens. The function must return a `bool` indicating whether any two queens share the same row, column, or diagonal (i.e., whether the placement is invalid due to a conflict). Queens are placed one by one in the order given. The function should stop checking and return `true` as soon as a conflict is detected with a newly placed queen against previously placed ones. Assume `M >= 1` and `N >= 0`. If `N` is 0, the function returns `false` (no conflict). Each coordinate is guaranteed to be within [1, M]. The function must handle cases with duplicate positions gracefully (they conflict in both row and column).
*/

#include <vector>
#include <utility>

// Checks if placing queens in the given order (row, column pairs) causes any conflict.
// Returns true if any two queens share a row, column, or diagonal; false otherwise.
bool nQueensConflict(int M, const std::vector<std::pair<int,int>>& queens) {
    if (queens.empty()) return false;

    std::vector<bool> rowUsed(M, false);      // index row-1
    std::vector<bool> colUsed(M, false);      // index col-1
    std::vector<bool> diag1Used(2 * M - 1, false); // (r - c) + (M-1)
    std::vector<bool> diag2Used(2 * M - 1, false); // (r + c) - 2

    for (const auto& q : queens) {
        int r = q.first;
        int c = q.second;
        int idx_row = r - 1;
        int idx_col = c - 1;
        int idx_d1 = (r - c) + (M - 1);
        int idx_d2 = (r + c) - 2;

        if (rowUsed[idx_row] || colUsed[idx_col] || diag1Used[idx_d1] || diag2Used[idx_d2]) {
            return true;
        }

        rowUsed[idx_row] = true;
        colUsed[idx_col] = true;
        diag1Used[idx_d1] = true;
        diag2Used[idx_d2] = true;
    }

    return false;
}

#include <cassert>
#include <vector>
#include <utility>

// The solution function declaration is assumed to be visible here.
bool nQueensConflict(int M, const std::vector<std::pair<int,int>>& queens);

int main() {
    // No queens
    assert(nQueensConflict(4, {}) == false);

    // Single queen - no conflict
    assert(nQueensConflict(4, {{1,1}}) == false);

    // Two queens in same row
    assert(nQueensConflict(4, {{1,1}, {1,3}}) == true);

    // Two queens in same column
    assert(nQueensConflict(4, {{2,2}, {3,2}}) == true);

    // Two queens on same main diagonal (r-c constant)
    assert(nQueensConflict(5, {{1,1}, {2,2}}) == true);

    // Two queens on same anti-diagonal (r+c constant)
    assert(nQueensConflict(5, {{1,4}, {2,3}}) == true);

    // Queens that do not conflict: (1,1) and (2,3) on 4x4
    assert(nQueensConflict(4, {{1,1}, {2,3}}) == false);

    // Three queens, but the third conflicts with the first
    assert(nQueensConflict(4, {{1,2}, {3,1}, {2,4}}) == true);

    // Valid placement of two queens that share no line
    assert(nQueensConflict(6, {{1,1}, {2,3}, {3,5}}) == false);

    // Duplicate exact position conflicts immediately
    assert(nQueensConflict(3, {{2,2}, {2,2}}) == true);

    // Large board small test
    assert(nQueensConflict(10, {{5,5}, {6,4}, {7,3}}) == true); // all on anti-diagonal r+c=10

    return 0;
}

// The problem requires detecting whether any queen in a given sequence attacks another queen already placed. We can solve this by tracking which rows, columns, and diagonals have been occupied so far. For each queen in order, we check if its row, column, or either diagonal has already been used. If so, we return `true` immediately; otherwise, we mark those as occupied and continue. For diagonals: a queen at (r, c) lies on two diagonals: one with constant (r - c) and one with constant (r + c). In an M×M board with rows and columns numbered 1..M, the value (r - c) ranges from -(M-1) to (M-1). We can shift it by (M-1) to index a vector of size 2M-1. The value (r + c) ranges from 2 to 2M, so we can shift by -2 to index a vector of size 2M-1 as well. Since we stop at the first conflict, we do not need to process later queens. The algorithm runs in O(N) time and O(M) auxiliary space. Edge cases: M=1 with N=1 is fine (no conflict); N=0 returns false; duplicates will trigger conflict because row and column already occupied; if the input vector contains coordinates outside [1,M], the function is undefined (but we assume valid input as per task).
