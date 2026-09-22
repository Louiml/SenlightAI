/*
Write a C++ function `int minimumSnakesAndLaddersMoves(const std::vector<std::vector<int>>& board)` that, given an `n x n` board (where `n >= 1`) representing the game Snakes and Ladders, returns the minimum number of dice rolls (each die has faces 1 through 6) needed to reach the final square `n*n` starting from square `1`. The board is indexed such that the bottom-left cell is square `1`, and squares are numbered sequentially left-to-right on each row, moving upward and reversing direction on each row (boustrophedon style). Each cell contains either `-1` (normal square) or a number `x` (1 ≤ x ≤ n*n) representing a snake or ladder: if you land on that cell, you immediately jump to square `x`. You must always move exactly 1–6 steps from your current square; if you overshoot `n*n`, you cannot make that move. Ensure the function handles cases where reaching the end is impossible (return `-1` in that case). The function must not modify the input board.
*/

#include <vector>
#include <queue>
#include <numeric>

// Minimum number of dice rolls to reach square n*n on a snakes-and-ladders board.
// board[r][c] is -1 for a normal square, otherwise the destination square after a snake/ladder.
// Square numbering: start at bottom-left cell as square 1, snake left-to-right on bottom row,
// then right-to-left on next row up, and so on.
int minimumSnakesAndLaddersMoves(const std::vector<std::vector<int>>& board) {
    const int n = static_cast<int>(board.size());
    const int totalSquares = n * n;

    // Map each square number (1..totalSquares) to its (row, col) coordinates.
    std::vector<std::pair<int, int>> squareToCoord(totalSquares + 1);
    std::vector<int> colIndices(n);
    std::iota(colIndices.begin(), colIndices.end(), 0);

    int square = 1;
    for (int row = n - 1; row >= 0; --row) {
        for (int col : colIndices) {
            squareToCoord[square++] = {row, col};
        }
        // Reverse order for the next row above.
        std::reverse(colIndices.begin(), colIndices.end());
    }

    // BFS to find minimum moves.
    std::vector<int> dist(totalSquares + 1, -1);
    std::queue<int> q;
    dist[1] = 0;
    q.push(1);

    while (!q.empty()) {
        int current = q.front();
        q.pop();

        for (int roll = 1; roll <= 6; ++roll) {
            int rawNext = current + roll;
            if (rawNext > totalSquares) {
                break; // Larger rolls also overshoot.
            }

            auto [r, c] = squareToCoord[rawNext];
            int nextSquare = board[r][c] == -1 ? rawNext : board[r][c];

            if (dist[nextSquare] == -1) {
                dist[nextSquare] = dist[current] + 1;
                q.push(nextSquare);
            }
        }
    }

    return dist[totalSquares];
}

#include <cassert>
#include <vector>

int main() {
    // Single-cell board: start at square 1, already at end.
    assert(minimumSnakesAndLaddersMoves({{-1}}) == 0);

    // 2x2 board with no snakes or ladders: reach square 4.
    // Board layout: row0 (top) = [4,3], row1 (bottom) = [1,2] (but cells are -1).
    std::vector<std::vector<int>> board2 = {{-1, -1}, {-1, -1}};
    assert(minimumSnakesAndLaddersMoves(board2) == 1); // roll 3? Actually 1->2->3->4 requires 3? Wait compute: from 1, max roll 6, but board has 4 squares, so from 1 can roll 1,2,3; 3 leads to 4, so 1 move.

    // 3x3 board with a ladder from square 2 to square 9.
    // Square numbering: bottom row left-to-right: 1,2,3; middle row right-to-left: 6,5,4; top row left-to-right: 7,8,9.
    // Set cell for square 2 (row 2? Actually bottom row is row index 2, col 1) to 9.
    std::vector<std::vector<int>> board3(3, std::vector<int>(3, -1));
    // bottom-left = row2,col0 = square1; bottom-middle = row2,col1 = square2.
    board3[2][1] = 9;
    // From 1, roll 1 to 2, then jump to 9, so 1 move.
    assert(minimumSnakesAndLaddersMoves(board3) == 1);

    // Snake from square 8 (top row col1) back to square 3.
    board3 = std::vector<std::vector<int>>(3, std::vector<int>(3, -1));
    board3[0][1] = 3; // square 8 (row0,col1) -> 3 (snake)
    // From 1, need to reach 9. Without snake, 1->7->8->9? Actually 1->2->... Let's compute BFS: 1 can roll to 2..7 (max 6, so 1..6). Roll 6 -> 7, then from 7 roll 2 -> 9. So 2 moves, but if we go to 8 via roll 7? can't roll 7. So snake doesn't affect. But if we roll 1 to 2, then roll 6 to 8 (from 2+6=8), then snake to 3, bad. Anyway answer should be 2.
    assert(minimumSnakesAndLaddersMoves(board3) == 2);

    // Unreachable: create a giant snake from every square? Let's do a 2x2 where square 1 has ladder to 2? No, that's reachable. Instead, do a board where all squares except start lead to 1, but end is unreachable? Since from 1 you can only roll 1-6; on 2x2, from 1 you can only roll 1-3, all go to some destination. Make square 2,3,4 all snake back to 1. Then you can never reach 4 (end), because any move from 1 goes to 2/3/4 then back to 1. So answer -1.
    std::vector<std::vector<int>> board4 = {{2, 1}, {1, 1}}; // But need proper layout: row0 top = [?, ?], row1 bottom = [?, ?]. Let's set: bottom row (row1) col0=1 (square1), col1=square2 -> set to 1 (snake back). top row (row0) col1=square3 -> set to 1, col0=square4 -> set to 1. But careful: square4 is end, but if it's a snake to 1, then you can never stay on end. So answer -1.
    std::vector<std::vector<int>> board4 = {{1, 1}, {1, 1}}; // All cells are 1 (snake to square1).
    assert(minimumSnakesAndLaddersMoves(board4) == -1);

    // A 4x4 standard board with a ladder from 2 to 15 and snake from 16? Actually 16 is end.
    // Just ensure correctness on larger board with known path.
    int n = 4;
    std::vector<std::vector<int>> board5(n, std::vector<int>(n, -1));
    // Ladder: square 2 (bottom row col1) -> 15 (top row col2? Actually square 15 is top row right? 4x4: bottom row 1-4, row2 8,7,6,5, row3 9-12, top row 16,15,14,13. So square 15 is row0 col1. Place ladder at square2 to 15.
    board5[3][1] = 15; // square2
    // Then from 1, roll 1 to 2 -> jump to 15, then roll 1 to 16. So 2 moves.
    assert(minimumSnakesAndLaddersMoves(board5) == 2);

    // Test with a ladder that bypasses to end directly.
    std::vector<std::vector<int>> board6(3, std::vector<int>(3, -1));
    board6[2][0] = 9; // square1 (bottom-left) has ladder to 9.
    assert(minimumSnakesAndLaddersMoves(board6) == 1);

    // Test unreachable due to being stuck on a single square with all moves looping back.
    // 3x3 board where square 1,2,3 all snake to 1, and others unreachable? Actually from 1 you roll 1-6, but board only has 9 squares. If square 2,3,4,5,6,7 all snake to 1, then you never escape. Square 8,9 unreachable. Build board: bottom row (row2) col0=1 (that's start), col1=1, col2=1; middle row (row1) col0=1? Actually middle row is 6,5,4 (right-to-left). So set all cells to 1 except start's snake? But start's cell is -1? Set all cells to 1 (snake to square1). Then from 1, any roll leads to a cell with value 1, so you go back to 1. End square 9 has value 1, so you can never finish. So return -1.
    std::vector<std::vector<int>> board7(3, std::vector<int>(3, 1));
    assert(minimumSnakesAndLaddersMoves(board7) == -1);

    // Board where all cells are -1, n=6, known answer: minimum rolls from 1 to 36 is ceil(35/6)=6.
    int n6 = 6;
    std::vector<std::vector<int>> board8(n6, std::vector<int>(n6, -1));
    assert(minimumSnakesAndLaddersMoves(board8) == 6);

    return 0;
}

// The problem is a classic shortest-path (minimum moves) on an unweighted graph, where each square is a node and each possible dice roll (1–6) creates an edge to a destination square (after applying any snake/ladder jump). Since all edges have equal weight (1 roll), breadth-first search (BFS) from square 1 yields the minimum number of moves to every reachable square. First, precompute the mapping from square number (1..n*n) to board coordinates (row, col). The board’s rows are given from top (row 0) to bottom (row n-1), but square numbering starts from bottom-left. So we iterate rows from `n-1` down to `0`. On each row, if the row direction is left-to-right (for the bottom row and every other row going up), we assign columns 0..n-1; on the next row, we reverse direction. We can simulate this by starting with a column vector `{0,1,...,n-1}`, filling the row, then reversing the vector for the next row. For BFS, maintain a `vector<int> dist(n*n+1, -1)`, set `dist[1]=0`, push 1 into a queue. For each popped square `x`, for each die face `i` from 1 to 6, if `x+i > n*n`, break (since larger faces also overshoot). Compute the board cell for `x+i`, read its value; if not `-1`, the destination is that value, otherwise `x+i`. If `dist[destination]` is unvisited, set it to `dist[x]+1` and push. After BFS, return `dist[n*n]` (which remains `-1` if unreachable). Edge cases: `n=1` (board has one cell, start=end=1, answer 0); loops (e.g., snake goes back) are handled by BFS because visited squares are not revisited; ladders that skip ahead may skip over the final square but still counted correctly. Time complexity: O(n*n) for building mapping and BFS (each square processed once, each with 6 neighbors) = O(n²). Space complexity: O(n²) for mapping and dist array, plus queue.
