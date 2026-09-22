You are given a simplified chess position encoded in Forsyth–Edwards Notation (FEN) but with only two symbols: `b` for an empty square and a digit `1`–`8` for that many consecutive occupied squares (represented as `1` on the internal board). The board always fits in a fixed 10×10 integer grid (1 = occupied, 0 = empty), but the actual active board size is derived from the input string. The input consists of multiple test cases until a line with `#` terminates the program. For each test case, the first line is the FEN‑like string with rows separated by `/`. After reading the board, you receive two 1‑based coordinates (row and column) `py, px` and `ny, nx`; you must swap the contents of those two squares (which may be the same square, in which case nothing changes), then output the updated board in the same FEN‑like format: for each row, output the digit count of consecutive `1`s, followed by `b` for each `0`, with rows separated by `/`. Write a C++ function `std::string fenAfterSwap(const std::string& fen, int row1, int col1, int row2, int col2)` that takes the original FEN string (without the terminating `#`) and the two 1‑based positions, performs the swap, and returns the updated FEN string. The function must not use global variables; it should parse the FEN into a local 10×10 integer board, track the number of rows and columns, perform the swap, and re‑encode the board into the output string.
#include <cassert>
#include <string>

// The solution function is declared here (or included from the header).
std::string fenAfterSwap(const std::string& fen, int row1, int col1, int row2, int col2);

int main() {
    // Simple 1x1 board, swapping same cell: unchanged.
    assert(fenAfterSwap("b", 1, 1, 1, 1) == "b");
    // 1x1 board swapping same cell when occupied: unchanged.
    assert(fenAfterSwap("1", 1, 1, 1, 1) == "1");

    // 1x2 board: "1b" means occupied, empty. Swap (1,1) and (1,2): becomes "b1".
    assert(fenAfterSwap("1b", 1, 1, 1, 2) == "b1");

    // 2x2 board: all occupied, swap any two yields same representation "4".
    // But FEN format for 2 rows: "2/2" (two rows each with 2 consecutive).
    // If we swap (1,1) and (2,2), still all occupied -> "2/2".
    assert(fenAfterSwap("2/2", 1, 1, 2, 2) == "2/2");

    // 2x2 board: "1b/b1" means row1: occ empty, row2: empty occ.
    // Original: cells (1,1)=1, (1,2)=0, (2,1)=0, (2,2)=1.
    // Swap (1,2) and (2,1): both are 0, still "1b/b1".
    assert(fenAfterSwap("1b/b1", 1, 2, 2, 1) == "1b/b1");
    // Swap (1,1) and (2,2): both are 1, still "1b/b1".
    assert(fenAfterSwap("1b/b1", 1, 1, 2, 2) == "1b/b1");
    // Swap (1,1) and (1,2): now cells: (1,1)=0, (1,2)=1, (2,1)=0, (2,2)=1
    // -> row1: "b1", row2: "b1" => output "b1/b1".
    assert(fenAfterSwap("1b/b1", 1, 1, 1, 2) == "b1/b1");

    // 3x3 board given as "8/8/8" (actually 3 rows of 3 occupied each, but 8 is invalid
    // for 3 columns, so use "3/3/3"). Swap (2,2) with (3,3): still all occupied -> "3/3/3".
    assert(fenAfterSwap("3/3/3", 2, 2, 3, 3) == "3/3/3");

    // More complex: "1b1/2b/11b" (row1: occ empty occ, row2: two occ then empty, row3: occ occ empty)
    // 3 columns each. Board:
    // row1: 1 0 1
    // row2: 1 1 0
    // row3: 1 1 0
    // Swap (1,2) [0] and (3,3) [0] -> no change.
    assert(fenAfterSwap("1b1/2b/11b", 1, 2, 3, 3) == "1b1/2b/11b");
    // Swap (1,2) [0] and (2,2) [1] -> now row1: 1 1 1, row2: 1 0 0, row3: 1 1 0
    // row1: "3", row2: "1bb", row3: "11b" -> output "3/1bb/11b"
    assert(fenAfterSwap("1b1/2b/11b", 1, 2, 2, 2) == "3/1bb/11b");

    return 0;
}
#include <string>
#include <vector>
#include <algorithm>
#include <cstddef>

// Returns the FEN string after swapping the contents of two 1-based positions.
std::string fenAfterSwap(const std::string& fen, int row1, int col1, int row2, int col2) {
    // Fixed size board; only the relevant part is used.
    int board[10][10] = {0};
    int rows = 0;
    int cols = 0;
    int currentRow = 0;
    int currentCol = 0;

    // Decode the FEN string into the board.
    for (char ch : fen) {
        if (ch == '/') {
            ++currentRow;
            currentCol = 0;
            continue;
        }
        if (ch == 'b') {
            board[currentRow][currentCol] = 0;
            ++currentCol;
            continue;
        }
        // ch must be a digit '1'..'8'
        int count = ch - '0';
        for (int j = 0; j < count; ++j) {
            board[currentRow][currentCol + j] = 1;
        }
        currentCol += count;
        // Track maximum columns seen (only needed for encoding).
        cols = std::max(cols, currentCol);
    }
    rows = currentRow + 1; // number of rows is last used row index + 1
    if (cols == 0) { // safety: if no row had any content, default to 1 col
        cols = 1;
    }

    // Perform the swap using 0-based indices.
    std::swap(board[row1 - 1][col1 - 1], board[row2 - 1][col2 - 1]);

    // Encode the board back to FEN format.
    std::string result;
    for (int i = 0; i < rows; ++i) {
        int consecutive = 0;
        for (int j = 0; j < cols; ++j) {
            if (board[i][j] == 1) {
                ++consecutive;
            } else {
                if (consecutive > 0) {
                    result += static_cast<char>('0' + consecutive);
                    consecutive = 0;
                }
                result += 'b';
            }
        }
        if (consecutive > 0) {
            result += static_cast<char>('0' + consecutive);
        }
        if (i != rows - 1) {
            result += '/';
        }
    }
    return result;
}
// The solution requires two main steps: decoding the input FEN into a dense row‑major 2D array, and encoding that array back into FEN. For decoding, iterate through the FEN string character by character. When a digit `d` is encountered, set the next `d` cells in the current row to `1` and advance the column counter by `d`. When a `/` is seen, increment the row counter and reset the column counter to 0. When `b` is seen, set the current cell to `0` and advance. After parsing, the number of rows is `gyou+1` and the number of columns is the total count of cells in the first row (or in general, the maximum `retu` reached). Since coordinates are 1‑based, convert to 0‑based indices by subtracting 1 from each row and column before swapping. The swap is a simple `std::swap` on the two board cells; if the coordinates are identical, `std::swap` does nothing. For encoding, for each row, scan columns from left to right, counting consecutive `1`s. When a `0` is found, if the counter is positive, output the counter as a digit and reset it, then output `b`. After the row, if the counter is positive, output it. Add `/` between rows except after the last row. Complexity: parsing and encoding each take O(R*C) where R and C are the number of rows and columns of the board, which is at most 10×10, so constant‑time per test case (O(1) asymptotically). Space is O(1) for the fixed 10×10 board plus the output string.
