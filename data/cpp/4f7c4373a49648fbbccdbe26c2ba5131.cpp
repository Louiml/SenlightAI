// Write a C++ function `int64_t findShortestPathWithCorruptions(int gridSize, const std::vector<std::pair<int,int>>& fallingBytes, int byteLimit)` that simulates a memory grid of size `(gridSize+1) × (gridSize+1)` (with coordinates from 0 to `gridSize` inclusive), starting with all cells as open space. The function reads a list of byte positions (each represented as `{x,y}`) in order. It first marks the first `byteLimit` positions as corrupted walls, then computes the shortest path (in Manhattan moves, no diagonals) from top-left `(0,0)` to bottom-right `(gridSize,gridSize)`, returning the minimum number of steps if a path exists, or `-1` if impossible. For example, with `gridSize=6` and bytes at `(5,4)`, `(4,2)`, `(4,5)`, etc., after applying the first 12 bytes, the shortest path might be 22 steps; if the grid becomes disconnected, return `-1`.
// The core algorithm is Dijkstra’s algorithm (or BFS since all edge weights are 1) on a 2D grid. First, place all the first `byteLimit` given bytes as walls on the board. Then run a breadth-first search from `(0,0)` to `(gridSize,gridSize)`. Use a queue of points and a visited set (or 2D boolean array) to avoid revisiting cells. Expand each cell in four directions, skipping out-of-bounds or wall cells. The first time we reach the target, that distance is the answer. If the queue empties without reaching the target, return `-1`. Edge cases: if `byteLimit` is 0, no walls are placed; if the start or end is a wall (impossible as per typical inputs but can be handled by returning `-1`); if gridSize is 0, start equals end, return 0. Time complexity is O(gridSize²) because each cell is visited at most once, and space is O(gridSize²) for the board and visited structure. Using a 2D vector of `bool` for walls and a queue of pairs keeps it efficient.
#include <vector>
#include <queue>
#include <utility>
#include <cstdint>

// Returns the shortest path length from (0,0) to (gridSize,gridSize)
// after applying the first byteLimit falling bytes as walls.
// Returns -1 if no path exists.
int64_t findShortestPathWithCorruptions(int gridSize,
                                        const std::vector<std::pair<int,int>>& fallingBytes,
                                        int byteLimit) {
    const int n = gridSize + 1;  // number of cells per side
    std::vector<std::vector<bool>> isWall(n, std::vector<bool>(n, false));

    // Mark the first byteLimit positions as walls (if any)
    for (int i = 0; i < byteLimit && i < static_cast<int>(fallingBytes.size()); ++i) {
        int x = fallingBytes[i].first;
        int y = fallingBytes[i].second;
        if (x >= 0 && x < n && y >= 0 && y < n) {
            isWall[y][x] = true;
        }
    }

    // If start or end is a wall, no path exists
    if (isWall[0][0] || isWall[gridSize][gridSize]) {
        return -1;
    }

    // BFS
    std::vector<std::vector<int>> dist(n, std::vector<int>(n, -1));
    std::queue<std::pair<int,int>> q;
    q.push({0, 0});
    dist[0][0] = 0;

    const int dx[4] = {1, -1, 0, 0};
    const int dy[4] = {0, 0, 1, -1};

    while (!q.empty()) {
        auto [x, y] = q.front();
        q.pop();

        if (x == gridSize && y == gridSize) {
            return dist[y][x];
        }

        for (int d = 0; d < 4; ++d) {
            int nx = x + dx[d];
            int ny = y + dy[d];
            if (nx >= 0 && nx < n && ny >= 0 && ny < n &&
                !isWall[ny][nx] && dist[ny][nx] == -1) {
                dist[ny][nx] = dist[y][x] + 1;
                q.push({nx, ny});
            }
        }
    }

    return -1;
}
#include <cassert>
#include <vector>
#include <utility>

// The solution function is declared above; here we test it.

int main() {
    // Basic 2x2 grid (0..1), no walls
    {
        std::vector<std::pair<int,int>> bytes;
        assert(findShortestPathWithCorruptions(1, bytes, 0) == 2); // (0,0)->(1,0)->(1,1) or via (0,1)
    }

    // 2x2 grid, one wall blocks the direct path but not the only path
    {
        std::vector<std::pair<int,int>> bytes = {{1,0}};
        // After 1 byte, (1,0) is wall. Path: (0,0)->(0,1)->(1,1) = 2 steps
        assert(findShortestPathWithCorruptions(1, bytes, 1) == 2);
        // After 2 bytes (also add (0,1)), all paths blocked? Try (1,1) still reachable? No, both neighbors blocked -> -1
        bytes.push_back({0,1});
        assert(findShortestPathWithCorruptions(1, bytes, 2) == -1);
    }

    // 3x3 grid (0..2), byteLimit=1 at (1,1)
    {
        std::vector<std::pair<int,int>> bytes = {{1,1}};
        // Need to go around: (0,0)->(1,0)->(2,0)->(2,1)->(2,2) = 4 steps
        assert(findShortestPathWithCorruptions(2, bytes, 1) == 4);
    }

    // Larger grid 6x6, typical puzzle: apply 12 bytes
    {
        std::vector<std::pair<int,int>> bytes = {
            {5,4}, {4,2}, {4,5}, {3,0}, {2,1}, {6,3},
            {2,4}, {1,5}, {0,6}, {3,3}, {2,6}, {5,1}
        };
        // Shortest path from (0,0) to (6,6) with these 12 walls is 22 steps
        assert(findShortestPathWithCorruptions(6, bytes, 12) == 22);
        // With one more byte at (6,6) the target becomes a wall -> -1
        bytes.push_back({6,6});
        assert(findShortestPathWithCorruptions(6, bytes, 13) == -1);
    }

    // byteLimit larger than available bytes -> only those present are placed
    {
        std::vector<std::pair<int,int>> bytes = {{1,0}};
        assert(findShortestPathWithCorruptions(1, bytes, 100) == 2);
    }

    // Single-cell grid (0,0) is start and end
    {
        std::vector<std::pair<int,int>> bytes;
        assert(findShortestPathWithCorruptions(0, bytes, 0) == 0);
    }

    // Start is immediately walled
    {
        std::vector<std::pair<int,int>> bytes = {{0,0}};
        assert(findShortestPathWithCorruptions(2, bytes, 1) == -1);
    }

    // All walls blocking a 2x2 grid completely
    {
        std::vector<std::pair<int,int>> bytes = {{1,0}, {0,1}};
        assert(findShortestPathWithCorruptions(1, bytes, 2) == -1);
    }

    return 0;
}
