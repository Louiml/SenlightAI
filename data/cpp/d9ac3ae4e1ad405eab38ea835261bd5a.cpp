Write a C++ function `bool isValidQueenPlacement(int n, const std::vector<int>& colPositions)` that takes the board size `n` and a vector `colPositions` of length `n`, where `colPositions[row]` is the column index (0-based) of a queen placed in that row. The function should return `true` if no two queens attack each other (i.e., no two share the same column, main diagonal (`row - col`), or anti-diagonal (`row + col`)), and `false` otherwise. The input may contain any integer for column positions (not necessarily within `[0, n-1]`), and duplicates are possible. Assume `n >= 1` and the vector size is exactly `n`. The function must handle large `n` efficiently using hash sets.
#include <cassert>
#include <vector>

int main() {
    // 4-queens valid solution: columns [1,3,0,2]
    std::vector<int> valid4 = {1, 3, 0, 2};
    assert(isValidQueenPlacement(4, valid4) == true);

    // 4-queens invalid due to same column (both col 0)
    std::vector<int> sameCol = {0, 0, 1, 2};
    assert(isValidQueenPlacement(4, sameCol) == false);

    // 4-queens invalid due to main diagonal conflict (row0,col0 and row1,col1)
    std::vector<int> mainDiagConflict = {0, 1, 2, 3};
    assert(isValidQueenPlacement(4, mainDiagConflict) == false);

    // 4-queens invalid due to anti-diagonal conflict (row0,col2 and row1,col1)
    std::vector<int> antiDiagConflict = {2, 1, 0, 3};
    assert(isValidQueenPlacement(4, antiDiagConflict) == false);

    // Single queen (n=1) always valid
    std::vector<int> oneQueen = {0};
    assert(isValidQueenPlacement(1, oneQueen) == true);

    // Single queen with out-of-range column still valid (no conflicts)
    std::vector<int> outOfRange = {5};
    assert(isValidQueenPlacement(1, outOfRange) == true);

    // Large n with valid diagonal placement (only one queen per row, distinct cols)
    int n = 1000;
    std::vector<int> largeValid(n);
    for (int i = 0; i < n; ++i) {
        largeValid[i] = (i * 7) % n;  // distinct columns, but might have diagonal conflicts — we won't assert true, just usage
    }
    // But at least call it to ensure no crash
    (void)isValidQueenPlacement(n, largeValid);

    // Clear conflict: two rows with same column in a 5x5
    std::vector<int> twoSameCol = {2, 2, 0, 1, 3};
    assert(isValidQueenPlacement(5, twoSameCol) == false);

    // Valid 8-queens solution: [4, 2, 7, 3, 6, 8, 5, 1] (0-based: [4,2,7,3,6,8,5,1] but col 8 is out of range, so use [4,2,7,3,6,0,5,1] as a known valid 8-queen? Actually let's use a real one: [0,4,7,5,2,6,1,3] is invalid? Use known valid: [3,6,2,7,1,4,0,5]? For simplicity use a known valid: [4,2,7,3,6,8,5,1] is invalid due to col 8; let's use [0,4,7,5,2,6,1,3] which is actually invalid because row0 col0 and row2 col7? No, I'll just use a known valid 8-queens: [3,6,2,7,1,4,0,5] is not valid because row0 col3 and row1 col6 conflict? Actually I'll use the famous solution: [4,2,7,3,6,8,0,5] is for 8? I'll just use a simple valid one for 5: for 5 no solution? Actually 5 has valid: [0,2,4,1,3]? Let's check: row0col0, row1col2, row2col4, row3col1, row4col3 — no conflicts? Check main diag: 0-0=0,1-2=-1,2-4=-2,3-1=2,4-3=1 all distinct. Anti: 0+0=0,1+2=3,2+4=6,3+1=4,4+3=7 distinct. Columns distinct. So valid.
    std::vector<int> valid5 = {0, 2, 4, 1, 3};
    assert(isValidQueenPlacement(5, valid5) == true);

    return 0;
}
#include <vector>
#include <unordered_set>

// Returns true if no two queens attack each other on an n x n board.
// colPositions[row] gives the column index (0-based) of the queen in that row.
bool isValidQueenPlacement(int n, const std::vector<int>& colPositions) {
    std::unordered_set<int> columns;
    std::unordered_set<int> mainDiagonals;   // row - col
    std::unordered_set<int> antiDiagonals;   // row + col

    for (int row = 0; row < n; ++row) {
        int col = colPositions[row];
        
        // Check column conflict
        if (columns.find(col) != columns.end()) {
            return false;
        }
        columns.insert(col);

        // Check main diagonal conflict (constant row - col)
        int mainDiag = row - col;
        if (mainDiagonals.find(mainDiag) != mainDiagonals.end()) {
            return false;
        }
        mainDiagonals.insert(mainDiag);

        // Check anti-diagonal conflict (constant row + col)
        int antiDiag = row + col;
        if (antiDiagonals.find(antiDiag) != antiDiagonals.end()) {
            return false;
        }
        antiDiagonals.insert(antiDiag);
    }

    return true;
}
// The solution uses three hash sets: one for columns, one for main diagonals (`row - col`), and one for anti-diagonals (`row + col`). For each row index `i` (from 0 to n-1), we read the corresponding column value `colPositions[i]`. Before inserting, we check if the column, the main diagonal (`i - col`), or the anti-diagonal (`i + col`) already exists in its respective set. If any already exists, we immediately return `false` because two queens share that line of attack. Otherwise, we insert and continue. After processing all rows, if no conflict is found, we return `true`. Important edge cases: duplicate column values anywhere will be caught as soon as the second occurrence is processed; out-of-range column positions are fine because they just produce unique diagonal values; if the vector length is less than `n`, the function would invoke undefined behavior, but we assume the input is valid. Time complexity is O(n) because each operation is average O(1) with hash sets. Space complexity is O(n) for the three sets, each holding at most n elements.
