Write a C++ function named `chessboardDistance` that takes two positive integers `cell1` and `cell2`, each representing a square on a standard 4-column chessboard-like grid (where squares are numbered 1 through N sequentially row by row, and each row has exactly 4 squares). The function must return the minimum number of moves a king would need to travel from `cell1` to `cell2`. A king can move one square horizontally, vertically, or diagonally per move, and the board only allows movement within the 4-column grid. The function must compute this distance without using loops, simply using arithmetic. Assume both inputs are within the range 1 to 1,000,000.

// The problem reduces to finding the Manhattan distance in a grid where moves are allowed diagonally. On a standard grid with 4 columns, we can map each cell number to a (row, column) coordinate. Given a cell `n`, the 0-based row index is `(n-1) // 4` and the 0-based column index is `(n-1) % 4`. However, the original snippet uses a different mapping where row and column are 1-based and the last column is 4, but the distance formula is the same regardless of 0/1 based. For two cells, the king's minimum moves is the maximum of the absolute row difference and absolute column difference, because diagonals cover both axes simultaneously. For example, to go from (r1,c1) to (r2,c2), each move reduces both differences by at most 1, so you need at least `max(|r1-r2|, |c1-c2|)` moves, and that many are always sufficient. Edge cases: when both cells are the same, distance is 0; when they are in the same row or column, distance is the difference along that axis. Time complexity is O(1), space O(1).

#include <cstdlib>

// Compute the minimum king moves between two cells on a 4-column grid.
int chessboardDistance(int cell1, int cell2) {
    // Convert to 0-based row and column indices.
    int row1 = (cell1 - 1) / 4;
    int col1 = (cell1 - 1) % 4;
    int row2 = (cell2 - 1) / 4;
    int col2 = (cell2 - 1) % 4;

    int rowDiff = std::abs(row1 - row2);
    int colDiff = std::abs(col1 - col2);

    // King's distance is the maximum of the two differences.
    return rowDiff > colDiff ? rowDiff : colDiff;
}

#include <cassert>

int main() {
    // Same cell
    assert(chessboardDistance(1, 1) == 0);
    // Adjacent horizontally
    assert(chessboardDistance(1, 2) == 1);
    // Adjacent vertically (cell 1 is row 0 col 0, cell 5 is row 1 col 0)
    assert(chessboardDistance(1, 5) == 1);
    // Diagonal neighbor (cell 1 to cell 6: row0 col0 to row1 col1)
    assert(chessboardDistance(1, 6) == 1);
    // Same row far apart (cell 1 to cell 4: row0 col0 to row0 col3)
    assert(chessboardDistance(1, 4) == 3);
    // Same column far apart (cell 1 to cell 13: row0 col0 to row3 col0)
    assert(chessboardDistance(1, 13) == 3);
    // General case (cell 2 to cell 14: row0 col1 to row3 col1) -> vertical diff 3, horizontal 0
    assert(chessboardDistance(2, 14) == 3);
    // General diagonal (cell 2 to cell 15: row0 col1 to row3 col2) -> diff rows 3, cols 1, max 3
    assert(chessboardDistance(2, 15) == 3);
    // Large numbers (cell 1000000 to cell 1)
    assert(chessboardDistance(1000000, 1) == 249999);
    // Edge of board row end to next row start (cell 4 to cell 5: row0 col3 to row1 col0) -> row diff 1, col diff 3, max 3
    assert(chessboardDistance(4, 5) == 3);
    return 0;
}
