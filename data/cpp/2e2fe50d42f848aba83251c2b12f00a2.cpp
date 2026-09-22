/*
Write a C++ function named `knight_distance` that takes a rectangular grid of size `n` (rows) by `m` (columns) and a starting cell `(start_x, start_y)` (0-indexed), and returns a 2D vector of type `long long` where each cell contains the minimum number of knight moves required to reach it from the starting cell, or `-1` if the cell is unreachable. Use Breadth-First Search (BFS) on the knight's 8 possible L-shaped moves. The function should accept `size_t` for dimensions and coordinates (noting that coordinates are 0-indexed), and return the grid as `std::vector<std::vector<long long>>`. The starting cell itself should have a distance of `0`. Cells outside the grid are ignored. You may assume `n` and `m` are at least 1, and the starting cell is within bounds.
*/
#include <vector>
#include <queue>
#include <cstddef>

// Returns a grid of minimum knight moves from (start_x, start_y) on an n x m board.
std::vector<std::vector<long long>> knight_distance(
    size_t n, size_t m, size_t start_x, size_t start_y) {
    // Result grid initialized to -1 (unvisited)
    std::vector<std::vector<long long>> dist(n, std::vector<long long>(m, -1));
    dist[start_x][start_y] = 0;

    // Direction offsets for a knight's move
    const int dx[8] = {2, 2, 1, 1, -1, -1, -2, -2};
    const int dy[8] = {1, -1, 2, -2, 2, -2, 1, -1};

    // BFS queue storing pairs (row, column)
    std::queue<std::pair<size_t, size_t>> q;
    q.push({start_x, start_y});

    while (!q.empty()) {
        auto [x, y] = q.front();
        q.pop();
        long long current_dist = dist[x][y];

        for (int i = 0; i < 8; ++i) {
            size_t nx = static_cast<size_t>(static_cast<long long>(x) + dx[i]);
            size_t ny = static_cast<size_t>(static_cast<long long>(y) + dy[i]);

            // Check bounds
            if (nx < n && ny < m && dist[nx][ny] == -1) {
                dist[nx][ny] = current_dist + 1;
                q.push({nx, ny});
            }
        }
    }

    return dist;
}
#include <cassert>
#include <vector>
#include <cstddef>

// Declaration of the function to test (assume it's included from the solution)
std::vector<std::vector<long long>> knight_distance(
    size_t n, size_t m, size_t start_x, size_t start_y);

int main() {
    // Test 1: 1x1 board, start at (0,0) -> only cell is 0
    auto g1 = knight_distance(1, 1, 0, 0);
    assert(g1.size() == 1 && g1[0].size() == 1 && g1[0][0] == 0);

    // Test 2: 3x3 board, start at (0,0) -> only corner cells reachable
    auto g2 = knight_distance(3, 3, 0, 0);
    assert(g2[0][0] == 0);
    assert(g2[0][1] == -1);
    assert(g2[0][2] == -1);
    assert(g2[1][0] == -1);
    assert(g2[1][1] == -1);
    assert(g2[1][2] == -1);
    assert(g2[2][0] == -1);
    assert(g2[2][1] == -1);
    assert(g2[2][2] == -1);

    // Test 3: 4x4 board, start at (0,0) -> check distances to a few cells
    auto g3 = knight_distance(4, 4, 0, 0);
    assert(g3[0][0] == 0);
    assert(g3[1][2] == 1);  // (1,2) is one move away
    assert(g3[2][1] == 1);  // (2,1) is one move away
    assert(g3[2][3] == 2);  // (2,3) is reachable in 2 moves
    assert(g3[3][2] == 2);  // (3,2) is reachable in 2 moves

    // Test 4: 5x5 board, start at (2,2) (center) -> check symmetry
    auto g4 = knight_distance(5, 5, 2, 2);
    assert(g4[2][2] == 0);
    // All eight neighbors at distance 1
    assert(g4[0][1] == 1 && g4[0][3] == 1);
    assert(g4[1][0] == 1 && g4[1][4] == 1);
    assert(g4[3][0] == 1 && g4[3][4] == 1);
    assert(g4[4][1] == 1 && g4[4][3] == 1);
    // A far cell like (0,0) should be 2 moves
    assert(g4[0][0] == 2);

    // Test 5: Board where start is not at corner, check a known distance
    auto g5 = knight_distance(8, 8, 0, 0);
    assert(g5[7][7] == 6); // From (0,0) to (7,7) on 8x8 knight distance is 6

    // Test 6: Board with width 2 and height 2, start at (0,0) -> only that cell reachable
    auto g6 = knight_distance(2, 2, 0, 0);
    for (size_t i = 0; i < 2; ++i)
        for (size_t j = 0; j < 2; ++j)
            assert(g6[i][j] == (i == 0 && j == 0 ? 0 : -1));

    // Test 7: Board 2x3, start at (0,0) -> (1,2) is one move, others unreachable
    auto g7 = knight_distance(2, 3, 0, 0);
    assert(g7[0][0] == 0);
    assert(g7[0][1] == -1);
    assert(g7[0][2] == -1);
    assert(g7[1][0] == -1);
    assert(g7[1][1] == -1);
    assert(g7[1][2] == 1);

    return 0;
}
// The problem is a classic shortest-path search on an unweighted graph, where each cell is a node and edges connect cells reachable by a knight's move. Since all edges have equal weight (1 move), BFS guarantees the first time a cell is visited yields its minimum distance. The algorithm initializes a 2D vector of size `n x m` filled with `-1`, sets the start cell to `0`, and uses a queue of coordinates. While the queue is non-empty, pop the front cell, and for each of the 8 legal move offsets (`(2,1), (2,-1), (1,2), (1,-2), (-2,1), (-2,-1), (-1,2), (-1,-2)`), compute the neighbor coordinates. If the neighbor is within grid bounds and its distance is still `-1` (unvisited), set its distance to current distance + 1 and push it into the queue. Because BFS processes nodes in order of increasing distance, the first time we assign a value is minimal. Edge cases include: the starting cell having distance 0; cells unreachable (e.g., on a 1x1 grid, all other nonexistent cells are irrelevant, but for large grids some cells may remain `-1` if the knight cannot reach them due to board shape); and handling negative coordinates carefully by checking bounds before accessing the vector. Time complexity is O(n*m) because each cell is enqueued at most once and each dequeued cell checks 8 neighbors, so O(8*m*n) = O(n*m). Space complexity is O(n*m) for the distance grid plus O(n*m) worst-case for the queue (when many cells are enqueued).
