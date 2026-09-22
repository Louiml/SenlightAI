Write a C++ function that reads an 8x8 board as input, where each of the 8 lines contains exactly 8 characters representing squares (characters can be `'F'` for a piece or `'.'` for an empty square). The function should return the number of pieces located on white squares, where a square is considered white if the sum of its 1-based row and 1-based column indices is even (i.e., rows and columns numbered 1 through 8 from top-left). The function must not perform any console I/O; it should accept the board as a `const` reference to a vector of strings (or array of strings) and return an integer count.
The main idea is to iterate over each row and each column of the 8x8 board. For each cell, check two conditions: (1) the character is `'F'`, and (2) the square is white. A square is white if `(row + column) % 2 == 0`, using 1-based indexing. To simplify, when the inner loop uses 0-based indices, the condition can be rewritten as `((row + 1) + (column + 1)) % 2 == 0`, which simplifies to `(row + column + 2) % 2 == 0`, which is equivalent to `(row + column) % 2 == 0`. Edge cases include ensuring we do not index out of bounds (the board is always exactly 8x8) and treating characters other than `'F'` (like `'.'`) correctly. The algorithm runs in O(64) = O(1) time since the board size is fixed, and uses O(1) extra space beyond the input storage.
#include <string>
#include <vector>

// Count pieces ('F') located on white squares of an 8x8 board.
// A square is white if the sum of its 1-based row and column is even.
int countPiecesOnWhiteSquares(const std::vector<std::string>& board) {
    int count = 0;
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            if (board[row][col] == 'F' && (row + col) % 2 == 0) {
                ++count;
            }
        }
    }
    return count;
}
#include <cassert>
#include <vector>
#include <string>

int countPiecesOnWhiteSquares(const std::vector<std::string>& board);

int main() {
    // Empty board
    std::vector<std::string> board1 = {
        "........",
        "........",
        "........",
        "........",
        "........",
        "........",
        "........",
        "........"
    };
    assert(countPiecesOnWhiteSquares(board1) == 0);

    // One piece on white square (row 1, col 1)
    std::vector<std::string> board2 = {
        "F.......",
        "........",
        "........",
        "........",
        "........",
        "........",
        "........",
        "........"
    };
    assert(countPiecesOnWhiteSquares(board2) == 1);

    // One piece on black square (row 1, col 2)
    std::vector<std::string> board3 = {
        ".F......",
        "........",
        "........",
        "........",
        "........",
        "........",
        "........",
        "........"
    };
    assert(countPiecesOnWhiteSquares(board3) == 0);

    // All 32 white squares have 'F'
    std::vector<std::string> board4 = {
        "F.F.F.F.",
        ".F.F.F.F",
        "F.F.F.F.",
        ".F.F.F.F",
        "F.F.F.F.",
        ".F.F.F.F",
        "F.F.F.F.",
        ".F.F.F.F"
    };
    assert(countPiecesOnWhiteSquares(board4) == 32);

    // Mixed board with both colors
    std::vector<std::string> board5 = {
        "F.F.....",
        "........",
        "........",
        "........",
        "........",
        "........",
        "........",
        "........"
    };
    // Only first 'F' at (1,1) is white; second at (1,3) is also white (1+3=4 even)
    assert(countPiecesOnWhiteSquares(board5) == 2);

    // Board with black square pieces only
    std::vector<std::string> board6 = {
        ".F.F....",
        "........",
        "........",
        "........",
        "........",
        "........",
        "........",
        "........"
    };
    // Positions (1,2) and (1,4): sums 3 and 5, both odd → black
    assert(countPiecesOnWhiteSquares(board6) == 0);
}
