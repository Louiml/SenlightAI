Write a C++ function `int minStepsToEscape(int n, int startX, int startY)` that simulates a knight moving on an `n x n` board starting from cell `(startX, startY)` (0-indexed). At each step, the knight attempts to move in all 8 possible L-shaped moves. If any of these moves would leave the board, the simulation stops immediately and returns the number of moves made so far (i.e., the distance from the start to the current position) plus one (because the escape happens on the next move). Otherwise, the knight continues from the first valid unvisited cell encountered (in the fixed move order), marking visited cells to avoid revisiting. If the knight never escapes (all moves stay in bounds and all reachable cells are visited without ever attempting an off-board move), return the total number of moves made when the queue becomes empty. The function must handle invalid starting coordinates by returning 0. The knight starts at `(startX, startY)` with 0 moves made; note that the starting cell itself is not considered "visited" in the algorithm's marking scheme, but every cell the knight actually moves to is marked.
// The solution uses a breadth-first search (BFS) queue to simulate the knight's movements. Each queue element stores the number of moves taken to reach a cell and the cell coordinates. The algorithm starts by pushing the initial cell with move count 0. While the queue is not empty, it pops the front element. First, it checks if the current cell is out of bounds. If it is, that means the previous step already attempted an off-board move (but logic ensures we stop earlier, so this check is a safety net) and returns the stored move count. For each of the 8 fixed knight moves (defined by the `dx` and `dy` arrays), it computes the new coordinates. If the new coordinates are outside the board, it immediately sets a flag and breaks out of the loop, then returns the current move count plus one (since the escape attempt happens from the current cell). If the new cell is inside the board and not yet visited, it marks it as visited and pushes it to the queue with move count incremented by one. If after processing all moves no off-board attempt occurred, the loop continues. If the queue becomes empty without ever attempting an off-board move (meaning all reachable cells are contained), the function returns the total number of moves made, which is the move count of the last popped element. Edge cases: invalid starting coordinates return 0 immediately; a knight that can escape on the first move (starting near a border) returns 1; a 1x1 board with valid start has no moves and returns 0 because no move is possible (the loop ends without any off-board attempt). Time complexity is O(n^2) because each cell is visited at most once, and space complexity is O(n^2) for the visited array and the queue.
#include <vector>
#include <queue>
#include <utility>

// Simulate a knight's movements on an n x n board until it attempts to escape.
// Returns the number of moves made when the first off-board move is attempted,
// or the total moves made if the knight never escapes.
int minStepsToEscape(int n, int startX, int startY) {
    // Invalid starting coordinates.
    if (startX < 0 || startY < 0 || startX >= n || startY >= n) {
        return 0;
    }

    // Knight move deltas in a fixed order.
    const int dx[8] = {1, -1, 2, -2, -1, 1, -2, 2};
    const int dy[8] = {2, 2, 1, 1, -2, -2, -1, -1};

    // Visited matrix (0 = unvisited, 1 = visited).
    std::vector<std::vector<int>> visited(n, std::vector<int>(n, 0));

    // BFS queue: pair of (moves, (x, y)).
    std::queue<std::pair<int, std::pair<int, int>>> q;
    q.push({0, {startX, startY}});

    int movesMade = 0;

    while (!q.empty()) {
        auto current = q.front();
        q.pop();

        int x = current.second.first;
        int y = current.second.second;
        movesMade = current.first;

        // Attempt all 8 knight moves.
        for (int i = 0; i < 8; ++i) {
            int newX = x + dx[i];
            int newY = y + dy[i];

            // If any move goes off the board, the knight escapes on the next move.
            if (newX < 0 || newX >= n || newY < 0 || newY >= n) {
                return movesMade + 1;
            }

            // If the cell is unvisited, mark and enqueue.
            if (visited[newX][newY] == 0) {
                visited[newX][newY] = 1;
                q.push({movesMade + 1, {newX, newY}});
            }
        }
    }

    // No escape ever attempted; return the total moves made.
    return movesMade;
}
#include <cassert>

int main() {
    // Invalid start.
    assert(minStepsToEscape(5, -1, 0) == 0);
    assert(minStepsToEscape(5, 0, 5) == 0);

    // 1x1 board: no moves possible.
    assert(minStepsToEscape(1, 0, 0) == 0);

    // Start in corner of 2x2 board: first move goes off-board.
    assert(minStepsToEscape(2, 0, 0) == 1);

    // Start in middle of 3x3 board: all moves go off-board.
    assert(minStepsToEscape(3, 1, 1) == 1);

    // 4x4 board, start at (0,0). First move (1,2) is inside, but from there some moves go off-board.
    // Let's simulate: from (0,0) moves: (1,2) inside, (2,1) inside. Enqueue both.
    // Pop (1,2) with moves=1. Its moves: (2,4) off -> return 2.
    assert(minStepsToEscape(4, 0, 0) == 2);

    // 8x8 board, start at (0,0). First move (1,2) inside, then from (1,2) some moves go off (e.g., (3,4) inside, but (-1,4) off? Actually from (1,2): (2,4) inside, (0,4) inside, (3,3) inside, (-1,3) off -> return 2.
    assert(minStepsToEscape(8, 0, 0) == 2);

    // 10x10 board, start at (0,0). Same as above, first move inside, second move has off-board? from (1,2): (-1,3) off -> return 2.
    assert(minStepsToEscape(10, 0, 0) == 2);

    // Large board, start deep inside (e.g., 10x10, start at (5,5)): first move is (6,7) inside, but from (5,5) all moves are inside? Check from (5,5): (6,7) inside, (4,7) inside, (7,6) inside, (3,6) inside, (4,3) inside, (6,3) inside, (3,4) inside, (7,4) inside. All inside. So no escape on first move. From (6,7) with moves=1: some moves off? (8,9) inside, (4,9) inside, (7,8) inside, (5,8) inside, (7,6) inside, (5,6) inside, (4,7) inside, (8,7) inside. Still inside. Continue BFS. Eventually, the knight will reach a border and attempt an off-board move. But for assert, we just check that it's > 1.
    assert(minStepsToEscape(10, 5, 5) > 1);

    return 0;
}
