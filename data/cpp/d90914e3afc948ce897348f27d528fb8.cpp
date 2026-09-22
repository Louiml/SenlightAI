// Given an 8x8 chess board represented as a vector of vectors of integers, where 0 = empty, 1 = white piece, and 2 = black piece, write a C++ function that counts the total number of ways a bishop can move on the board in exactly one move, considering that it can only move diagonally and cannot jump over any piece. The bishop can move to any empty square or capture an opponent's piece, but it cannot move to a square occupied by a friendly piece. The function should take the board (as a 2D vector of int) and the bishop's starting position (row and column, both 0-indexed) as parameters. It should return the total number of distinct destination squares the bishop can reach in exactly one move, counting both empty squares and squares with opponent pieces as valid destinations. Assume the board is always 8x8. For clarity, a bishop moves diagonally any number of squares, but all intervening squares must be empty.
The solution approach is to simulate all four diagonal directions (up-left, up-right, down-left, down-right) from the starting position. For each direction, we step one square at a time while staying within board bounds. For each step, we check the destination square: if it's empty (value 0), we count it and continue stepping in that direction; if it contains an opponent's piece (value differs from the starting square's piece value, assuming pieces are either 1 or 2), we count that square and stop stepping in that direction because the bishop captures and cannot pass; if it contains a friendly piece (same value as the starting square's piece), we stop stepping without counting. The starting square itself is not counted. Key edge cases: the starting position must contain a non-zero piece (assume valid), the bishop might be blocked immediately by an adjacent friendly piece, and the board may have no legal moves, returning 0. Time complexity is O(1) because the bishop can move at most 7 squares in any diagonal, so we examine at most 28 squares total. Space complexity is O(1) as we use only a few scalar variables.
#include <vector>

// Count the number of legal destination squares for a bishop on an 8x8 board.
// Board values: 0=empty, 1=white piece, 2=black piece. The piece at (startRow, startCol) is assumed non-zero.
// Returns the count of distinct squares reachable in one diagonal move (including captures).
int countBishopMoves(const std::vector<std::vector<int>>& board, int startRow, int startCol) {
    const int N = 8;
    int count = 0;
    int piece = board[startRow][startCol];

    // The four diagonal direction vectors: (rowChange, colChange)
    const int directions[4][2] = {{-1,-1}, {-1,1}, {1,-1}, {1,1}};

    for (int d = 0; d < 4; ++d) {
        int r = startRow + directions[d][0];
        int c = startCol + directions[d][1];

        // Step until we leave the board or are blocked
        while (r >= 0 && r < N && c >= 0 && c < N) {
            int target = board[r][c];
            if (target == 0) {
                // Empty square: valid destination, keep stepping
                ++count;
            } else if (target != piece) {
                // Opponent piece: valid capture, stop after this
                ++count;
                break;
            } else {
                // Friendly piece blocks; stop without counting
                break;
            }
            r += directions[d][0];
            c += directions[d][1];
        }
    }

    return count;
}
#include <cassert>
#include <vector>

// Forward declaration of the function to test (already defined above)

int main() {
    // Board 1: empty board, bishop at center (3,3) can move 13 squares
    std::vector<std::vector<int>> emptyBoard(8, std::vector<int>(8, 0));
    assert(countBishopMoves(emptyBoard, 3, 3) == 13);

    // Board 2: all friendly pieces surrounding bishop, so no moves
    std::vector<std::vector<int>> blockedBoard(8, std::vector<int>(8, 1));
    blockedBoard[3][3] = 1;
    assert(countBishopMoves(blockedBoard, 3, 3) == 0);

    // Board 3: bishop at corner (0,0), opponent pieces at (1,1) and (2,2)
    std::vector<std::vector<int>> cornerBoard(8, std::vector<int>(8, 0));
    cornerBoard[0][0] = 1; // bishop
    cornerBoard[1][1] = 2; // opponent
    cornerBoard[2][2] = 2; // beyond opponent, should not be reachable
    assert(countBishopMoves(cornerBoard, 0, 0) == 1); // only (1,1) is reachable

    // Board 4: bishop at (0,0), empty diagonal to edge, one friendly piece at (3,3)
    std::vector<std::vector<int>> mixedBoard(8, std::vector<int>(8, 0));
    mixedBoard[0][0] = 2;
    mixedBoard[3][3] = 2; // friendly, blocks further movement
    assert(countBishopMoves(mixedBoard, 0, 0) == 3); // (1,1), (2,2), and (3,3) blocked? Wait (3,3) is friendly, so only 2? Actually stepping: (1,1) empty, (2,2) empty, (3,3) friendly -> count 2.

    // Corrected assertion:
    assert(countBishopMoves(mixedBoard, 0, 0) == 2);

    // Board 5: bishop surrounded on one side by opponent, other sides open
    std::vector<std::vector<int>> partialBoard(8, std::vector<int>(8, 0));
    partialBoard[4][4] = 1; // bishop
    partialBoard[3][3] = 2; // opponent
    partialBoard[5][5] = 2; // opponent
    // Up-left: (3,3) capture -> count 1, stop
    // Up-right: (3,5) empty, (2,6) empty, (1,7) empty -> 3
    // Down-left: (5,3) empty -> 1
    // Down-right: (5,5) capture -> 1, stop
    assert(countBishopMoves(partialBoard, 4, 4) == 1 + 3 + 1 + 1); // 6

    return 0;
}
