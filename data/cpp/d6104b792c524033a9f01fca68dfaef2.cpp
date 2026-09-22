// Write a C++ function that takes an 8x8 chess board represented as a 2D vector of integers (0 = empty, 1 = friendly piece, 2 = opponent piece), along with the coordinates (x, y) of a queen, and returns a vector of pairs containing all valid squares the queen can move to. The queen moves horizontally, vertically, and diagonally in all eight directions, but must stop before a friendly piece (cannot capture it) and must stop after an opponent piece (can capture it but cannot pass through). The function should identify the piece at (x,y) as the queen's own piece (you may assume it is present), and treat all other squares as either empty or occupied by friendly/opponent pieces. Implement the function `getQueenMoves(const std::vector<std::vector<int>>& board, int x, int y)` that returns the moves in any order, excluding the queen's starting square.

The algorithm follows the queen's movement rules by scanning in eight directions from the starting square. For each direction, we step one square at a time while staying within the 8x8 board boundaries. At each candidate square, we check its content: if it is empty (0), add it to the result and continue; if it contains an opponent piece (2), add it (capture) and stop this direction; if it contains a friendly piece (1), stop without adding it. We use eight separate loops (or a direction vector approach) to cover the four diagonal and four straight-line directions. Edge cases include the queen being near a board edge (no moves beyond bounds) or surrounded by friendly pieces (no moves at all). The time complexity is O(1) since the board is fixed at 8x8 and we scan at most 7 squares per direction, and space complexity is O(1) excluding the output vector.

#include <vector>
#include <utility>

// Return all squares a queen can move to on an 8x8 board.
// board[y][x] is 0 for empty, 1 for friendly, 2 for opponent.
// (x, y) is the queen's position (assumed to contain a friendly piece).
std::vector<std::pair<int,int>> getQueenMoves(
    const std::vector<std::vector<int>>& board,
    int x, int y)
{
    std::vector<std::pair<int,int>> moves;
    
    // Direction vectors: (dx, dy) for all 8 directions.
    const int dx[8] = {1, 1, 0, -1, -1, -1, 0, 1};
    const int dy[8] = {0, 1, 1, 1, 0, -1, -1, -1};
    
    for (int d = 0; d < 8; ++d) {
        int nx = x + dx[d];
        int ny = y + dy[d];
        
        while (nx >= 0 && nx < 8 && ny >= 0 && ny < 8) {
            int cell = board[ny][nx];
            if (cell == 0) {
                // Empty square: add and continue.
                moves.emplace_back(nx, ny);
            } else if (cell == 2) {
                // Opponent piece: capture and stop this direction.
                moves.emplace_back(nx, ny);
                break;
            } else { // cell == 1 (friendly)
                // Friendly piece blocks further movement.
                break;
            }
            nx += dx[d];
            ny += dy[d];
        }
    }
    
    return moves;
}

#include <cassert>
#include <vector>
#include <utility>

int main() {
    // Board with queen at (4,4), surrounded by empty spaces.
    std::vector<std::vector<int>> emptyBoard(8, std::vector<int>(8, 0));
    auto moves = getQueenMoves(emptyBoard, 4, 4);
    assert(moves.size() == 28); // 7 in each of 4 directions

    // Board with friendly pieces blocking all moves.
    std::vector<std::vector<int>> blockedBoard(8, std::vector<int>(8, 0));
    blockedBoard[4][3] = 1; // left
    blockedBoard[4][5] = 1; // right
    blockedBoard[3][4] = 1; // up
    blockedBoard[5][4] = 1; // down
    blockedBoard[3][3] = 1; // up-left
    blockedBoard[3][5] = 1; // up-right
    blockedBoard[5][3] = 1; // down-left
    blockedBoard[5][5] = 1; // down-right
    moves = getQueenMoves(blockedBoard, 4, 4);
    assert(moves.empty());

    // Queen in a corner with mixer of empty and opponent pieces.
    std::vector<std::vector<int>> cornerBoard(8, std::vector<int>(8, 0));
    cornerBoard[0][1] = 2; // opponent to the right
    cornerBoard[1][0] = 2; // opponent below
    cornerBoard[1][1] = 1; // friendly diagonal (blocked)
    moves = getQueenMoves(cornerBoard, 0, 0);
    // Expected moves: (1,0), (0,1) only; diagonal is blocked by friendly.
    assert(moves.size() == 2);
    assert(std::find(moves.begin(), moves.end(), std::pair<int,int>(1,0)) != moves.end());
    assert(std::find(moves.begin(), moves.end(), std::pair<int,int>(0,1)) != moves.end());

    // Queen with an opponent piece that stops movement beyond it.
    std::vector<std::vector<int>> captureBoard(8, std::vector<int>(8, 0));
    captureBoard[4][5] = 2; // opponent to the right at (5,4)
    captureBoard[4][6] = 0; // empty beyond, should not be reached
    moves = getQueenMoves(captureBoard, 4, 4);
    // Right direction should include (5,4) but not (6,4).
    assert(std::find(moves.begin(), moves.end(), std::pair<int,int>(5,4)) != moves.end());
    assert(std::find(moves.begin(), moves.end(), std::pair<int,int>(6,4)) == moves.end());

    // Queen at (7,7), only valid moves are up and left.
    std::vector<std::vector<int>> edgeBoard(8, std::vector<int>(8, 0));
    moves = getQueenMoves(edgeBoard, 7, 7);
    assert(moves.size() == 21); // 7 up, 7 left, 7 diagonal up-left
    assert(std::find(moves.begin(), moves.end(), std::pair<int,int>(6,6)) != moves.end());
    assert(std::find(moves.begin(), moves.end(), std::pair<int,int>(0,7)) != moves.end());
    assert(std::find(moves.begin(), moves.end(), std::pair<int,int>(7,0)) != moves.end());
    assert(std::find(moves.begin(), moves.end(), std::pair<int,int>(7,8)) == moves.end()); // out of bounds

    return 0;
}
