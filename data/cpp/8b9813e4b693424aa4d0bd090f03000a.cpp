/*
Write a C++ function `int countSafeSquares(int R, int C, const std::vector<std::vector<int>>& board)` that takes a board of size `R` rows and `C` columns (both between 1 and 1000, inclusive) where each cell contains an integer: `0` = empty, `1` = Queen, `2` = Knight, `3` = Pawn (pawns are non-blocking decorative pieces that do not attack or block). The function must simulate the attack ranges of all Queens and Knights on the board and return the number of cells that are **safe** (i.e., not attacked by any Queen or Knight, and not occupied by any piece). Queens attack in the 8 standard directions (horizontal, vertical, diagonal) in a straight line until blocked by any non-empty cell (including a pawn, queen, or knight) or the board edge; attacked cells include the blocking piece’s cell itself. Knights attack in the 8 L-shaped moves (as per standard chess) and may attack over any intervening pieces; the attacked target cell must be on the board and empty (not occupied by any piece) to be marked as attacked. Board cells are 1-indexed both rows and columns for internal logic, but the input board uses 0-indexing in the vector (so row i corresponds to vector index i-1). The function must not modify the input board. Return the total count of empty cells that are not attacked.
*/
#include <vector>

// Count safe squares on a chess-like board with queens and knights.
// board: R x C grid, 0=empty, 1=queen, 2=knight, 3=pawn.
// Returns number of empty cells not attacked by any queen or knight.
int countSafeSquares(int R, int C, const std::vector<std::vector<int>>& board) {
    // 1-indexed grid initialized to 0
    std::vector<std::vector<int>> T(R + 2, std::vector<int>(C + 2, 0));

    // Copy board to 1-indexed grid
    for (int i = 0; i < R; ++i) {
        for (int j = 0; j < C; ++j) {
            T[i + 1][j + 1] = board[i][j];
        }
    }

    // Direction vectors for queen (8 directions) and knight (8 L-moves)
    const int di[] = {0, 0, 1, -1, 1, -1, 1, -1, 2, 1, -1, -2, -2, -1, 1, 2};
    const int dj[] = {1, -1, 0, 0, 1, -1, -1, 1, 1, 2, 2, 1, -1, -2, -2, -1};

    // Helper lambda for queen directional walk
    auto queenWalk = [&](int x, int y, int dir) {
        int nx = x + di[dir];
        int ny = y + dj[dir];
        while (nx >= 1 && nx <= R && ny >= 1 && ny <= C) {
            int cell = T[nx][ny];
            if (cell == 0 || cell == 3) {
                // empty or pawn: mark attacked and continue
                T[nx][ny] = 5;
            } else {
                // queen or knight: mark attacked and stop
                T[nx][ny] = 5;
                break;
            }
            nx += di[dir];
            ny += dj[dir];
        }
    };

    // Process all cells
    for (int i = 1; i <= R; ++i) {
        for (int j = 1; j <= C; ++j) {
            if (T[i][j] == 1) { // Queen
                for (int k = 0; k < 8; ++k) {
                    queenWalk(i, j, k);
                }
            } else if (T[i][j] == 2) { // Knight
                for (int k = 8; k < 16; ++k) {
                    int nx = i + di[k];
                    int ny = j + dj[k];
                    if (nx >= 1 && nx <= R && ny >= 1 && ny <= C && T[nx][ny] == 0) {
                        T[nx][ny] = 5;
                    }
                }
            }
        }
    }

    // Count safe squares (still 0)
    int safe = 0;
    for (int i = 1; i <= R; ++i) {
        for (int j = 1; j <= C; ++j) {
            if (T[i][j] == 0) {
                ++safe;
            }
        }
    }
    return safe;
}
#include <cassert>
#include <vector>

int countSafeSquares(int R, int C, const std::vector<std::vector<int>>& board);

int main() {
    // Board 1: 1x1 empty
    assert(countSafeSquares(1, 1, {{0}}) == 1);

    // Board 2: 1x1 queen
    assert(countSafeSquares(1, 1, {{1}}) == 0);

    // Board 3: 2x2 queen at corner attacks all other cells
    std::vector<std::vector<int>> b3 = {{1,0},{0,0}};
    assert(countSafeSquares(2, 2, b3) == 0);

    // Board 4: 2x2 knight at corner attacks one cell, other is safe
    std::vector<std::vector<int>> b4 = {{2,0},{0,0}};
    // Knight at (1,1) attacks (2,2)? Actually (1+2,1+1)=(3,2) out of bounds; (1+1,1+2)=(2,3) out. So no attacks.
    assert(countSafeSquares(2, 2, b4) == 3);

    // Board 5: 3x3 queen at center attacks all 8 others
    std::vector<std::vector<int>> b5 = {{0,0,0},{0,1,0},{0,0,0}};
    assert(countSafeSquares(3, 3, b5) == 0);

    // Board 6: Pawn blocks queen
    std::vector<std::vector<int>> b6 = {{1,3,0},{0,0,0},{0,0,0}};
    // Queen at (1,1) attacks (1,2) pawn then blocked. Attacks (2,1),(3,1),(2,2). So safe cells: (1,3)? queen also attacks (1,3) diagonally? from (1,1) diag right-down goes to (2,2) blocked? No block, so attacks (2,2) and (3,3). (1,3) is horizontal but blocked by pawn at (1,2). So (1,3) safe. Others? (2,3) attacked diag? (1,1)->(2,2) then (3,3) not (2,3). (3,2) attacked diag (1,1)->(2,2)->(3,3) not (3,2). (3,1) attacked vertical. (2,1) attacked vertical. So safe cells: (1,3),(2,3),(3,2) = 3.
    assert(countSafeSquares(3, 3, b6) == 3);

    // Board 7: Knight can jump over pawn
    std::vector<std::vector<int>> b7 = {{2,3,0},{0,0,0},{0,0,0}};
    // Knight at (1,1) attacks (2,3) and (3,2) (both empty). (2,3) is empty? row2 col3 is 0. (3,2) is 0. So those are attacked. Safe cells: (1,2) pawn, (1,3), (2,1),(2,2),(3,1),(3,3) = 5? But (2,1) not attacked, (2,2) not, (3,1) not, (3,3) not, (1,3) not, (1,2) pawn not safe. So count empty not attacked: (1,3),(2,1),(2,2),(3,1),(3,3)=5.
    assert(countSafeSquares(3, 3, b7) == 5);

    // Board 8: Queen and knight together
    std::vector<std::vector<int>> b8 = {{1,0,0},{0,2,0},{0,0,0}};
    // Queen at (1,1) attacks (1,2),(1,3),(2,1),(3,1),(2,2),(3,3). Knight at (2,2) attacks (1,4 out),(4,1 out),(4,3 out),(3,4 out),(1,? actually (2+2,2+1)=(4,3) out, (2+1,2+2)=(3,4) out, (2-1,2+2)=(1,4) out, (2-2,2+1)=(0,3) out, (2-2,2-1)=(0,1) out, (2-1,2-2)=(1,0) out, (2+1,2-2)=(3,0) out, (2+2,2-1)=(4,1) out). So knight attacks none. All cells attacked by queen except? (2,3)? queen diag from (1,1) goes to (2,2) which is knight, so blocked at (2,2), so (3,3) is not attacked because (2,2) blocks diag. (2,3) is horizontal? (1,1)->(1,2)->(1,3) then down? no. Vertical (1,1)->(2,1)->(3,1). So (2,3) not attacked. (3,2) not attacked. So safe cells: (2,3) and (3,2). Total 2.
    assert(countSafeSquares(3, 3, b8) == 2);

    // Board 9: Empty large board 5x5
    std::vector<std::vector<int>> b9(5, std::vector<int>(5, 0));
    assert(countSafeSquares(5, 5, b9) == 25);

    return 0;
}
// The solution is a direct simulation. First, create a 2D integer grid `T` (1-indexed for convenience) initialized to `0`. Copy the input board values (adjusting for 1-indexing) into this grid. Then iterate through all cells. When a cell contains a `1` (Queen), for each of the 8 directional vectors `(0,1),(0,-1),(1,0),(-1,0),(1,1),(-1,-1),(1,-1),(-1,1)`, perform a recursive or iterative walk from the Queen’s position moving step by step in that direction. At each step, if the new position is out of bounds, stop. If the current cell is empty (value `0`) or contains a pawn (`3`), mark it as attacked (change to `5`) and continue moving. If the cell contains a queen (`1`) or knight (`2`), mark it as attacked and stop (because it blocks). When a cell contains a `2` (Knight), for each of the 8 knight move offsets `(2,1),(1,2),(-1,2),(-2,1),(-2,-1),(-1,-2),(1,-2),(2,-1)`, compute the target cell; if it’s within bounds and the target cell is empty (`0`), mark it as attacked (`5`). Pawns (`3`) do nothing. After processing all queens and knights, count cells that remain `0`. Edge cases: board may contain no queens or knights; queens may be completely blocked by adjacent pieces; knights cannot attack occupied cells; multiple queens may attack the same cells, but marking as `5` repeatedly is harmless; queens can attack pawns and mark them as attacked, but pawns are not counted as safe anyway. Time complexity: O(R*C*8) in the worst case for queens because each cell is visited at most once per direction from a queen; knights are O(R*C) constant per knight. Space: O(R*C) for the grid.
