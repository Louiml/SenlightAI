Write a C++ function that, given a 2D grid of characters where `'S'` marks the start, `'E'` marks the end, `'.'` are open cells, and `'#'` are walls, returns the length of the shortest path from start to end using only up/down/left/right moves, or `-1` if no path exists. The grid is represented as a `std::vector<std::string>` with at least one row and one column, and there is exactly one `'S'` and one `'E'`. The function must be named `shortestPathLength` and take the grid by const reference. You may assume the grid is rectangular (all strings equal length) and non-empty.

Use a breadth-first search (BFS) from the start cell. BFS guarantees the first time we reach the end cell, the number of steps is minimal because all moves have equal weight (1). Maintain a queue of positions and a distance matrix initialized to `-1` (unvisited). Start by setting the start cell's distance to 0 and pushing it. Then, while the queue is not empty, pop a cell, and for each of the four directions (up, down, left, right), compute the neighbor's coordinates. If the neighbor is inside the grid, not a wall (`'#'`), and has distance `-1` (unvisited), set its distance to current distance + 1 and push it. If the neighbor is the end cell, we can return the distance immediately. If the queue is exhausted without reaching the end, return `-1`. Edge cases include the start being adjacent to the end (answer 1), start equals end (not possible per constraints), and grids where the end is surrounded by walls (return -1). Time complexity is O(rows × cols) because each cell is visited at most once. Space complexity is O(rows × cols) for the distance matrix and queue.

#include <vector>
#include <string>
#include <queue>

// Returns the length of the shortest path from 'S' to 'E' in the grid.
// Uses '.' for open cells and '#' for walls. Returns -1 if unreachable.
int shortestPathLength(const std::vector<std::string>& grid) {
    const int rows = static_cast<int>(grid.size());
    const int cols = static_cast<int>(grid[0].size());

    // Locate start and end positions.
    int startR = -1, startC = -1;
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            if (grid[r][c] == 'S') {
                startR = r;
                startC = c;
            }
        }
    }

    // Distance matrix initialized to -1 (unvisited).
    std::vector<std::vector<int>> dist(rows, std::vector<int>(cols, -1));
    std::queue<std::pair<int, int>> q;
    dist[startR][startC] = 0;
    q.push({startR, startC});

    // Direction vectors: up, down, left, right.
    const int dr[4] = {-1, 1, 0, 0};
    const int dc[4] = {0, 0, -1, 1};

    while (!q.empty()) {
        auto [r, c] = q.front();
        q.pop();

        for (int d = 0; d < 4; ++d) {
            int nr = r + dr[d];
            int nc = c + dc[d];

            // Check bounds and wall.
            if (nr < 0 || nr >= rows || nc < 0 || nc >= cols) continue;
            if (grid[nr][nc] == '#') continue;
            if (dist[nr][nc] != -1) continue;

            dist[nr][nc] = dist[r][c] + 1;
            if (grid[nr][nc] == 'E') {
                return dist[nr][nc];
            }
            q.push({nr, nc});
        }
    }

    return -1; // End not reachable.
}

#include <cassert>
#include <vector>
#include <string>

int main() {
    // Simple 3x3 grid with clear path.
    std::vector<std::string> grid1 = {
        "S..",
        ".#.",
        "..E"
    };
    assert(shortestPathLength(grid1) == 4); // S->(0,1)->(0,2)->(1,2)->(2,2)

    // Direct adjacency.
    std::vector<std::string> grid2 = {
        "SE"
    };
    assert(shortestPathLength(grid2) == 1);

    // Wall blocks path.
    std::vector<std::string> grid3 = {
        "S#E"
    };
    assert(shortestPathLength(grid3) == -1);

    // Larger maze with detour.
    std::vector<std::string> grid4 = {
        "S.#.",
        ".#..",
        "...#",
        "..E."
    };
    assert(shortestPathLength(grid4) == 6);

    // Start and end in 2x2 with no obstacles.
    std::vector<std::string> grid5 = {
        "S.",
        ".E"
    };
    assert(shortestPathLength(grid5) == 2);

    // Single row with multiple columns.
    std::vector<std::string> grid6 = {
        "S...E"
    };
    assert(shortestPathLength(grid6) == 4);

    // End completely surrounded by walls (but start outside).
    std::vector<std::string> grid7 = {
        "S..",
        "###",
        ".E."
    };
    assert(shortestPathLength(grid7) == -1);

    // Start at top-left, end at bottom-right, no walls.
    std::vector<std::string> grid8 = {
        "S..",
        "...",
        "..E"
    };
    assert(shortestPathLength(grid8) == 4);
}
