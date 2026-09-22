// Given an \(N \times M\) binary maze (grid) consisting of cells labeled `0` (blocked) or `1` (open), write a C++ function `int shortestPathLength(const std::vector<std::vector<int>>& maze)` that returns the minimum number of cells (including the start and end) in a path from the top‑left cell `(0,0)` to the bottom‑right cell `(N-1, M-1)`. You may move up, down, left, or right by one cell at a time, and you may only step on open cells. The start and end cells are guaranteed to be open (`1`). If no path exists, return `-1`. The input maze will have dimensions between 1 and 100 inclusive.
#include <cassert>
#include <vector>

// The solution function is declared above (in the same file).
int main() {
    // Single cell maze
    assert(shortestPathLength({{1}}) == 1);

    // Straight line
    assert(shortestPathLength({{1,1,1,1}}) == 4);

    // No path (blocked destination)
    assert(shortestPathLength({{1,1},{1,0}}) == -1);

    // Classic 4x4 maze
    std::vector<std::vector<int>> maze1 = {
        {1,0,1,1},
        {1,1,1,0},
        {0,1,0,1},
        {1,1,1,1}
    };
    assert(shortestPathLength(maze1) == 7); // path: (0,0)->(1,0)->(1,1)->(1,2)->(2,1)->(3,1)->(3,2)->(3,3) length = 8, but shortest is actually 8? Let's compute: another path: (0,0)->(0,1) blocked, so go down (1,0),(1,1),(1,2),(2,2) blocked, so (2,1),(3,1),(3,2),(3,3) = 8. So correct answer is 8.
    // Actually let's fix this: assert should be 8.
    assert(shortestPathLength(maze1) == 8);

    // All open large grid
    std::vector<std::vector<int>> maze2(5, std::vector<int>(5, 1));
    assert(shortestPathLength(maze2) == 9); // 5+5-1

    // Blocked start
    assert(shortestPathLength({{0,1},{1,1}}) == -1);
}
#include <vector>
#include <queue>
#include <utility>

// Returns the minimum number of cells in a path from (0,0) to (N-1, M-1) in the given binary maze.
// Returns -1 if no path exists.
int shortestPathLength(const std::vector<std::vector<int>>& maze) {
    if (maze.empty() || maze[0].empty()) return -1;
    const int rows = static_cast<int>(maze.size());
    const int cols = static_cast<int>(maze[0].size());
    if (maze[0][0] == 0 || maze[rows-1][cols-1] == 0) return -1;
    if (rows == 1 && cols == 1) return 1;

    std::vector<std::vector<bool>> visited(rows, std::vector<bool>(cols, false));
    visited[0][0] = true;

    // Queue stores pairs of (y, x) coordinates and the distance from start.
    std::queue<std::pair<std::pair<int,int>, int>> q;
    q.push({{0,0}, 1});

    const int dy[4] = {0, 0, 1, -1};
    const int dx[4] = {1, -1, 0, 0};

    while (!q.empty()) {
        auto [coords, dist] = q.front();
        q.pop();
        int y = coords.first;
        int x = coords.second;

        for (int i = 0; i < 4; ++i) {
            int ny = y + dy[i];
            int nx = x + dx[i];
            if (ny >= 0 && ny < rows && nx >= 0 && nx < cols &&
                maze[ny][nx] == 1 && !visited[ny][nx]) {
                if (ny == rows-1 && nx == cols-1) {
                    return dist + 1;
                }
                visited[ny][nx] = true;
                q.push({{ny, nx}, dist + 1});
            }
        }
    }
    return -1;
}
// The problem is a classic shortest‑path search on an unweighted grid, which is optimally solved with Breadth‑First Search (BFS). We start a queue with the initial cell `(0,0)` and a distance of `1`. For each cell dequeued, we explore its four orthogonal neighbors. If a neighbor is inside the grid, is an open cell (`1`), and has not been visited yet, we mark it visited, push it into the queue with distance = current distance + 1, and check if it is the destination `(N-1, M-1)`. When the destination is found, we return its distance immediately. If the queue becomes empty without reaching the destination, we return `-1`. Edge cases include a 1×1 maze (answer is 1), a maze where start = end, or a maze where the start is blocked (but per constraints it is open), and mazes that have no path. Time complexity is \(O(N \times M)\) because each cell is enqueued at most once. Space complexity is also \(O(N \times M)\) for the visited matrix and the queue in the worst case.
