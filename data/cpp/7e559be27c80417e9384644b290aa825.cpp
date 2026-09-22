/*
You are given a rectangular grid of characters where `.` denotes a passable cell, `#` denotes an obstacle, `R` is a single starting cell, and `D` is a single destination cell. Write a C++ function `std::vector<std::string> markShortestPath(const std::vector<std::string>& grid)` that, if there is at least one path from `R` to `D` moving only up, down, left, or right (without stepping on `#`), returns a new grid (as a vector of strings) where the destination `D` remains unchanged, the starting cell `R` remains unchanged, and every cell that lies on exactly one chosen shortest path (the lexicographically smallest one, determined by the order of moves: right, left, up, down) between `R` and `D` is replaced with `'X'`. If there is no path from `R` to `D`, return the original grid unchanged. All cells not on that specific path remain unchanged. The grid dimensions are at least 1×1 and at most 1000×1000. The grid is guaranteed to contain exactly one `R` and one `D`.
*/

#include <vector>
#include <string>
#include <queue>
#include <utility>

// Mark a deterministic shortest path from 'R' to 'D' with 'X', if one exists.
std::vector<std::string> markShortestPath(const std::vector<std::string>& grid) {
    int n = static_cast<int>(grid.size());
    int m = static_cast<int>(grid[0].size());

    int si = -1, sj = -1, di = -1, dj = -1;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (grid[i][j] == 'R') {
                si = i; sj = j;
            } else if (grid[i][j] == 'D') {
                di = i; dj = j;
            }
        }
    }

    // If somehow R or D is missing, return a copy.
    if (si == -1 || di == -1) return grid;

    // Copy grid to modify.
    std::vector<std::string> result = grid;

    std::vector<std::vector<bool>> visited(n, std::vector<bool>(m, false));
    std::vector<std::vector<std::pair<int,int>>> parent(n, std::vector<std::pair<int,int>>(m, {-1,-1}));

    // Directions in fixed order: right, left, up, down.
    const int dx[4] = {0, 0, -1, 1};
    const int dy[4] = {1, -1, 0, 0};

    std::queue<std::pair<int,int>> q;
    visited[si][sj] = true;
    q.push({si, sj});

    bool found = false;
    while (!q.empty() && !found) {
        auto [x, y] = q.front();
        q.pop();

        for (int k = 0; k < 4; ++k) {
            int nx = x + dx[k];
            int ny = y + dy[k];
            if (nx >= 0 && nx < n && ny >= 0 && ny < m &&
                !visited[nx][ny] && result[nx][ny] != '#') {
                visited[nx][ny] = true;
                parent[nx][ny] = {x, y};
                q.push({nx, ny});
                if (nx == di && ny == dj) {
                    found = true;
                    break;
                }
            }
        }
    }

    if (found) {
        // Reconstruct path from D back to R.
        int cx = di, cy = dj;
        while (true) {
            auto [px, py] = parent[cx][cy];
            if (px == -1 && py == -1) break; // Should not happen if found.
            // Move to parent
            cx = px;
            cy = py;
            if (result[cx][cy] == 'R') break;
            if (result[cx][cy] != 'D') {
                result[cx][cy] = 'X';
            }
        }
    }

    return result;
}

#include <cassert>
#include <vector>
#include <string>

// The solution function is declared here (or included from above).
// For testing, we just need the function.

int main() {
    // Example 1: Simple open path
    std::vector<std::string> g1 = {
        "R..",
        "...",
        "..D"
    };
    auto r1 = markShortestPath(g1);
    // Shortest path length 4 (right, right, down, down). Mark intermediates.
    assert(r1[0] == "RXX");
    assert(r1[1] == "...");
    assert(r1[2] == "..D");

    // Example 2: No path (wall blocking)
    std::vector<std::string> g2 = {
        "R.#",
        "###",
        "#.D"
    };
    auto r2 = markShortestPath(g2);
    assert(r2 == g2); // unchanged

    // Example 3: Direct adjacent path
    std::vector<std::string> g3 = {
        "RD"
    };
    auto r3 = markShortestPath(g3);
    assert(r3 == g3); // no intermediate cells

    // Example 4: Multiple shortest paths but deterministic tie-breaking
    std::vector<std::string> g4 = {
        "R..",
        ".#.",
        "..D"
    };
    auto r4 = markShortestPath(g4);
    // Only one shortest path due to obstacle.
    assert(r4[0] == "R.X"); // path goes right, right, down, down
    assert(r4[1] == ".#.");
    assert(r4[2] == "..D");

    // Example 5: Larger grid with multiple shortest paths, check deterministic order
    std::vector<std::string> g5 = {
        "R..D",
        "....",
        "...."
    };
    auto r5 = markShortestPath(g5);
    assert(r5[0] == "RXXD"); // path right, right, right
    assert(r5[1] == "....");
    assert(r5[2] == "....");

    // Example 6: Start and destination separated by walls but path exists
    std::vector<std::string> g6 = {
        "R.#",
        ".#.",
        "#.D"
    };
    auto r6 = markShortestPath(g6);
    // Only one path: right, down, right, down
    assert(r6[0] == "R.#");
    assert(r6[1] == ".X.");
    assert(r6[2] == "#.D");

    // Example 7: 1x1 grid impossible, but just in case (should not happen)
    // Skip due to guaranteed distinct R and D.

    // Example 8: path goes through start and destination only
    std::vector<std::string> g8 = {
        "R.D"
    };
    auto r8 = markShortestPath(g8);
    assert(r8[0] == "R.D"); // no intermediate

    return 0;
}

// The problem is a classic shortest-path in an unweighted grid using BFS. We start BFS from `R`, exploring neighbors in the fixed order `{0,1}`, `{0,-1}`, `{-1,0}`, `{1,0}` (right, left, up, down) to ensure a deterministic shortest path. We maintain a `parent` array to reconstruct the path. When BFS reaches `D`, we know a path exists; otherwise, no path exists. To reconstruct the path, we start from `D` and repeatedly move to its parent until we reach `R`. Along the way, we mark every intermediate cell (excluding `R` and `D`) as `'X'`. The BFS guarantees the shortest path length; the deterministic neighbor order makes the path unique among all shortest paths. Edge cases: when `R` and `D` are the same (guaranteed not to happen since they are distinct characters), when the grid is 1×1 (only one character, but likely impossible due to both `R` and `D`), when there is no path, and when walls block the direct route. Time complexity is O(n*m) for BFS and path reconstruction, space O(n*m) for visited, parent, and queue.
