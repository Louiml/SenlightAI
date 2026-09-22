// Write a C++ function `int shortestPathWithOneWallBreak(int rows, int cols, const std::vector<std::string>& grid)` that determines the minimum number of moves required to travel from the top-left cell `(0,0)` to the bottom-right cell `(rows-1, cols-1)` in a rectangular grid. Each cell is either passable (`'0'`) or blocked (`'1'`). You may move up, down, left, or right one cell at a time. You are allowed to break **at most one wall** (i.e., you may pass through exactly one blocked cell during the entire journey). If the destination is reachable, return the minimum number of moves (including the start cell as move count 1); if it is unreachable even with one wall break, return `-1`. The grid will have at least 1 row and 1 column, and `grid[i]` has exactly `cols` characters. The start and end cells are guaranteed to be `'0'`. Ensure the solution is correct for all edge cases, including when no wall break is needed and when the grid is a single cell.
// The problem is a classic shortest-path in a grid with a limited "power" (here, the ability to break one wall). A standard BFS works because all moves have equal weight (each move costs 1). However, we must track not only the position but also whether we have already used the wall-break. Therefore, we maintain a 3D distance array `dist[r][c][used]` where `used` is 0 or 1 (0 = wall already broken, 1 = wall break still available). We initialize `dist[0][0][1] = 1` (starting at the top-left with the wall break unused, move count 1). In the BFS queue, each state is `(row, col, used)`. For each of the 4 neighbor cells: if the neighbor is within bounds and passable (`'0'`), we can move there without changing `used`, provided that state hasn't been visited. If the neighbor is a wall (`'1'`) and `used == 1` (we still have the break), we can move there and set `used` to 0 for that next state, again only if not visited. When we pop a state that is at the bottom-right corner, we return its distance. If the queue empties without reaching the destination, return `-1`. Critical edge cases: (1) The grid is `1x1` — the BFS should return 1 immediately. (2) The start or end is a wall? The problem guarantees they are `'0'`, so no need to handle. (3) A path exists without breaking any wall — BFS will find it. (4) A path requires exactly one wall break — the BFS handles it. (5) Some cells might be reachable both with and without using the break; the 3D visited array ensures we explore all possibilities correctly. Time complexity is `O(rows * cols * 2)` = `O(rows * cols)` since each state `(r,c,used)` is visited at most once. Space complexity is also `O(rows * cols)` for the distance array and queue.
#include <vector>
#include <string>
#include <queue>
#include <array>

// Returns the minimum number of moves from (0,0) to (rows-1, cols-1)
// allowing at most one wall break, or -1 if unreachable.
int shortestPathWithOneWallBreak(int rows, int cols, const std::vector<std::string>& grid) {
    if (rows == 0 || cols == 0) return -1;
    if (rows == 1 && cols == 1) return 1;  // already at destination

    // dist[r][c][used] : used = 1 means wall-break not yet used, 0 means used.
    std::vector<std::vector<std::array<int, 2>>> dist(
        rows, std::vector<std::array<int, 2>>(cols, {0, 0})
    );

    // Queue holds (row, col, used)
    std::queue<std::array<int, 3>> q;
    q.push({0, 0, 1});
    dist[0][0][1] = 1;

    const int dr[4] = {-1, 1, 0, 0};
    const int dc[4] = {0, 0, -1, 1};

    while (!q.empty()) {
        auto [r, c, used] = q.front();
        q.pop();

        int curDist = dist[r][c][used];

        if (r == rows - 1 && c == cols - 1) {
            return curDist;
        }

        for (int i = 0; i < 4; ++i) {
            int nr = r + dr[i];
            int nc = c + dc[i];

            if (nr < 0 || nr >= rows || nc < 0 || nc >= cols) continue;

            if (grid[nr][nc] == '0' && dist[nr][nc][used] == 0) {
                dist[nr][nc][used] = curDist + 1;
                q.push({nr, nc, used});
            } else if (grid[nr][nc] == '1' && used == 1 && dist[nr][nc][0] == 0) {
                dist[nr][nc][0] = curDist + 1;
                q.push({nr, nc, 0});
            }
        }
    }

    return -1;
}
#include <cassert>
#include <vector>
#include <string>

int main() {
    // Single cell
    assert(shortestPathWithOneWallBreak(1, 1, {"0"}) == 1);

    // Simple open path, no wall needed
    std::vector<std::string> grid1 = {
        "000",
        "000",
        "000"
    };
    assert(shortestPathWithOneWallBreak(3, 3, grid1) == 5);

    // Requires one wall break to get through
    std::vector<std::string> grid2 = {
        "000",
        "010",
        "000"
    };
    assert(shortestPathWithOneWallBreak(3, 3, grid2) == 5);

    // Wall at the end, need break
    std::vector<std::string> grid3 = {
        "00",
        "01"
    };
    assert(shortestPathWithOneWallBreak(2, 2, grid3) == 3);

    // Unreachable even with one wall break (need two breaks)
    std::vector<std::string> grid4 = {
        "000",
        "111",
        "000"
    };
    assert(shortestPathWithOneWallBreak(3, 3, grid4) == -1);

    // Start and end are same but wall in middle? Actually simple 3x1 with wall in middle
    std::vector<std::string> grid5 = {
        "0",
        "1",
        "0"
    };
    assert(shortestPathWithOneWallBreak(3, 1, grid5) == 3);

    // A path that is longer but avoids wall is found
    std::vector<std::string> grid6 = {
        "0010",
        "0110",
        "0000"
    };
    // Path: (0,0)->(1,0)->(2,0)->(2,1)->(2,2)->(2,3)->(1,3)->(0,3) = 8 moves? Let's count: start (0,0) =1, (1,0)=2, (2,0)=3, (2,1)=4, (2,2)=5, (2,3)=6, (1,3)=7, (0,3)=8. No wall break needed.
    assert(shortestPathWithOneWallBreak(3, 4, grid6) == 8);

    return 0;
}
