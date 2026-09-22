/*
Write a C++ function named `deathKnightMinMoves` that, given an integer board size `N` (where the board is an `N x N` grid with coordinates from `(0,0)` to `(N-1,N-1)`), a starting coordinate `(r1, c1)`, and a target coordinate `(r2, c2)`, returns the minimum number of moves required for a "death knight" piece to reach the target from the start. The death knight moves in exactly six possible directions: `(-2,-1)`, `(-2,1)`, `(0,-2)`, `(0,2)`, `(2,-1)`, and `(2,1)`. If the target is unreachable, the function must return `-1`. All inputs are non-negative integers within the board bounds for start/target; `N` is at least 1.
*/
#include <vector>
#include <queue>
#include <utility>

// Return minimum moves for a death knight on an N x N board, or -1 if unreachable.
int deathKnightMinMoves(int N, int r1, int c1, int r2, int c2) {
    const int dx[6] = {-2, -2, 0, 0, 2, 2};
    const int dy[6] = {-1, 1, -2, 2, -1, 1};

    std::vector<std::vector<int>> dist(N, std::vector<int>(N, -1));
    std::queue<std::pair<int, int>> q;

    q.push({r1, c1});
    dist[r1][c1] = 0;

    while (!q.empty()) {
        auto [x, y] = q.front();
        q.pop();

        if (x == r2 && y == c2) {
            return dist[x][y];
        }

        for (int i = 0; i < 6; ++i) {
            int nx = x + dx[i];
            int ny = y + dy[i];

            if (nx < 0 || ny < 0 || nx >= N || ny >= N) {
                continue;
            }
            if (dist[nx][ny] != -1) {
                continue;
            }
            dist[nx][ny] = dist[x][y] + 1;
            q.push({nx, ny});
        }
    }

    return -1;
}
#include <cassert>

int main() {
    // N=1, start equals target.
    assert(deathKnightMinMoves(1, 0, 0, 0, 0) == 0);

    // Simple reachable case from the problem snippet: N=7, (0,0) to (7? but N=7 means max index 6) — use N=7, (0,0) to (6,0) is unreachable? Let's test a known reachable path.
    // Starting (2,2) to (0,1) requires one move of (-2,-1).
    assert(deathKnightMinMoves(5, 2, 2, 0, 1) == 1);

    // Two moves: (2,2) -> (4,1) -> (2,0) using (2,-1) then (-2,-1) — but check bounds N=5.
    assert(deathKnightMinMoves(5, 2, 2, 2, 0) == 2);

    // Unreachable: N=3, start (0,0), target (1,1) — no move can produce that.
    assert(deathKnightMinMoves(3, 0, 0, 1, 1) == -1);

    // Unreachable because all moves leave the board: N=2, (0,0) to (1,0).
    assert(deathKnightMinMoves(2, 0, 0, 1, 0) == -1);

    // Larger reachable: N=10, (5,5) to (5,7) requires one (0,2) move.
    assert(deathKnightMinMoves(10, 5, 5, 5, 7) == 1);

    // Larger path requiring 3 moves: N=10, (2,3) -> (0,2) -> (2,1) -> (4,0) — check dist.
    // (2,3) to (4,0) — one move? (2,-3) not a move. Try (2,3) -> (4,2) -> (2,1) -> (4,0) is 3 moves.
    assert(deathKnightMinMoves(10, 2, 3, 4, 0) == 3);

    // Same start and target in larger board.
    assert(deathKnightMinMoves(10, 4, 4, 4, 4) == 0);

    // Edge case: N=1, any other start? Only (0,0) valid, so test with invalid? Not needed.

    // Test boundary: N=4, (0,0) to (0,2) requires one (0,2) move (within bounds).
    assert(deathKnightMinMoves(4, 0, 0, 0, 2) == 1);

    // Test unreachable due to parity? Already covered.

    return 0;
}
// The problem is a classic shortest-path on an unweighted grid, solvable with Breadth-First Search (BFS). We represent the board as a 2D vector of distances, initialized to `-1` to mark unvisited cells. Starting from `(r1,c1)`, we set its distance to 0 and push it onto a queue. While the queue is not empty, we pop the front cell. If it is the target, we immediately return its recorded distance. Otherwise, we iterate through the six move offsets. For each neighbor, we check if it is within the `[0, N-1]` bounds for both x and y coordinates, and if it has not been visited (distance still `-1`). If valid, we update its distance to current+1 and push it onto the queue. Because BFS explores in layers, the first time we reach the target is guaranteed to be the minimum number of moves. If the queue becomes empty without reaching the target, the target is unreachable, and we return `-1`. Edge cases include `N=1` where start equals target (return 0), and cases where all moves leave the board (return -1). Time complexity is O(N²) since each cell is enqueued at most once, and space complexity is O(N²) for the distance grid plus the queue size up to O(N²).
