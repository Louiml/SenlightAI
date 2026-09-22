/*
Write a standalone C++ function `int countReachableGold(const std::vector<std::vector<int>>& grid, int startY, int startX)` that takes a 2D grid of integers where positive values represent gold amounts, negative values represent impassable walls, and zero represents empty floor. The player starts at `(startY, startX)` and can move up, down, left, and right. The function must return the total amount of gold collectable by walking through all reachable non-wall cells starting from the given position. The grid is rectangular, and the start position is guaranteed to be within bounds and not a wall. The algorithm must simulate movement without tunneling through walls, and the grid size can be up to 1000x1000. The function should not modify the input grid.
*/
#include <vector>
#include <queue>

// Returns total gold reachable from start (startY, startX) moving orthogonally.
// Walls are cells with negative values, floor is 0, gold is positive.
int countReachableGold(const std::vector<std::vector<int>>& grid,
                       int startY, int startX) {
    if (grid.empty() || grid[0].empty()) return 0;
    const int rows = static_cast<int>(grid.size());
    const int cols = static_cast<int>(grid[0].size());

    // If start is a wall, nothing is reachable
    if (grid[startY][startX] < 0) return 0;

    std::vector<std::vector<bool>> visited(rows,
                                           std::vector<bool>(cols, false));
    std::queue<std::pair<int, int>> q;
    q.emplace(startY, startX);
    visited[startY][startX] = true;

    int total = 0;
    const int dy[4] = {-1, 1, 0, 0};
    const int dx[4] = {0, 0, -1, 1};

    while (!q.empty()) {
        auto [y, x] = q.front();
        q.pop();

        if (grid[y][x] > 0) total += grid[y][x];

        for (int dir = 0; dir < 4; ++dir) {
            int ny = y + dy[dir];
            int nx = x + dx[dir];
            if (ny >= 0 && ny < rows && nx >= 0 && nx < cols &&
                !visited[ny][nx] && grid[ny][nx] >= 0) {
                visited[ny][nx] = true;
                q.emplace(ny, nx);
            }
        }
    }

    return total;
}
#include <cassert>

int main() {
    // Simple single cell with gold
    std::vector<std::vector<int>> g1 = {{5}};
    assert(countReachableGold(g1, 0, 0) == 5);

    // Small open grid with mixed values
    std::vector<std::vector<int>> g2 = {
        {1, -1, 3},
        {2,  0, 4},
        {-1, 5, 6}
    };
    // From (0,0) reachable: (0,0)=1, (1,0)=2, (1,1)=0, (2,1)=5, (2,2)=6 => total 14
    assert(countReachableGold(g2, 0, 0) == 14);
    // From (0,2) reachable: (0,2)=3, (1,2)=4, (2,2)=6 => total 13
    assert(countReachableGold(g2, 0, 2) == 13);

    // Wall at start
    std::vector<std::vector<int>> g3 = {{-1, 7}};
    assert(countReachableGold(g3, 0, 0) == 0);

    // Isolated region via wall, start outside
    std::vector<std::vector<int>> g4 = {
        {1, 2},
        {-1, 3}
    };
    // From (0,0) can reach (0,1)=2, (1,1)=3 blocked by wall at (1,0)
    assert(countReachableGold(g4, 0, 0) == 3);

    // Large empty area with no gold
    std::vector<std::vector<int>> g5(100, std::vector<int>(100, 0));
    assert(countReachableGold(g5, 50, 50) == 0);

    // Check all open from corner
    std::vector<std::vector<int>> g6 = {
        {0, 1, 2},
        {3, 4, 5}
    };
    assert(countReachableGold(g6, 0, 0) == 15);
}
// The problem reduces to a flood-fill or breadth-first search (BFS) from the start position, visiting every cell that is connected to the start through orthogonal moves, skipping walls (negative values). For each visited cell, add its value to a running total if positive. Since the grid is large, we should use an iterative BFS with a queue to avoid stack overflow from recursion. We also need a visited set (e.g., a boolean grid) to prevent re-visiting cells. The time complexity is O(R*C) where R and C are the grid dimensions, because each cell is enqueued at most once. The space complexity is also O(R*C) for the visited array and queue in the worst case (e.g., all floor cells). Edge cases include: the start cell may have gold itself, walls may enclose regions unreachable from start, and the start could be on a single-cell region. We ignore gold on walls because walls are impassable. The BFS terminates when the queue is empty.
