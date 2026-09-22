/*
You are given a rectangular grid of cells, each labeled either `.` (empty) or `X` (broken). You start at a given cell `(r1,c1)` and want to reach another given cell `(r2,c2)`. You can move up/down/left/right one cell at a time. Normally, you may only step on empty cells `.`, except that you are allowed to step onto the destination cell `(r2,c2)` even if it is `X` — but only as the very last move. Additionally, the start cell `(r1,c1)` is guaranteed to be empty (`.`). You may pass through any cell multiple times. Determine whether it is possible to reach the destination under these rules, considering that if the destination is `.`, you must still be able to enter it (which is fine), but if the destination is `X`, you are allowed to step onto it only as the final move. More precisely, you are allowed to step on `X` only if that cell is the destination, and that step must be the last step (i.e., after that you stop). Write a C++ function `bool canReach(const std::vector<std::string>& grid, int r1, int c1, int r2, int c2)` that returns `true` if a valid path exists, `false` otherwise. The grid has dimensions `n` (rows) and `m` (columns), with 0-indexed coordinates.
*/

#include <vector>
#include <queue>
#include <string>

// Determine if we can reach (r2,c2) from (r1,c1) in the given grid.
// Movement is 4-directional. We may step on '.' any number of times.
// We may step on 'X' only if it is the destination cell, and that stepping
// must be the final move (so we stop there).
bool canReach(const std::vector<std::string>& grid, int r1, int c1, int r2, int c2) {
    int n = static_cast<int>(grid.size());
    if (n == 0) return false;
    int m = static_cast<int>(grid[0].size());

    // Already at destination.
    if (r1 == r2 && c1 == c2) return true;

    std::vector<std::vector<bool>> visited(n, std::vector<bool>(m, false));
    std::queue<std::pair<int,int>> q;
    q.push({r1, c1});
    visited[r1][c1] = true;

    const int dr[4] = {1, 0, -1, 0};
    const int dc[4] = {0, 1, 0, -1};

    while (!q.empty()) {
        auto [r, c] = q.front();
        q.pop();

        for (int k = 0; k < 4; ++k) {
            int nr = r + dr[k];
            int nc = c + dc[k];
            if (nr < 0 || nr >= n || nc < 0 || nc >= m) continue;

            // If this is the destination, we can step onto it and stop.
            if (nr == r2 && nc == c2) {
                return true;
            }

            // Otherwise, only empty cells are passable.
            if (grid[nr][nc] == 'X') continue;

            if (!visited[nr][nc]) {
                visited[nr][nc] = true;
                q.push({nr, nc});
            }
        }
    }

    return false;
}

#include <cassert>
#include <vector>
#include <string>

// (Function definition from Solution section goes here)

int main() {
    // Simple reachable through empty cells
    std::vector<std::string> grid1 = {
        "....",
        ".XX.",
        "...."
    };
    assert(canReach(grid1, 0, 0, 2, 3) == true);

    // Destination is X but reachable as final step
    std::vector<std::string> grid2 = {
        "....",
        ".X.X",
        "...."
    };
    assert(canReach(grid2, 0, 0, 1, 2) == true);

    // Destination is X but not reachable (blocked)
    std::vector<std::string> grid3 = {
        "..X.",
        "XXXX",
        "..X."
    };
    assert(canReach(grid3, 0, 0, 2, 0) == false);

    // Start equals destination
    std::vector<std::string> grid4 = {
        "X.",
        ".."
    };
    assert(canReach(grid4, 0, 0, 0, 0) == true);

    // Destination is X directly adjacent to start
    std::vector<std::string> grid5 = {
        ".X",
        ".."
    };
    assert(canReach(grid5, 0, 0, 0, 1) == true);

    // No path at all, destination empty but isolated
    std::vector<std::string> grid6 = {
        ".X.",
        "XXX",
        ".X."
    };
    assert(canReach(grid6, 0, 0, 2, 0) == false);

    // Path exists, destination is empty but surrounded by X except entry
    std::vector<std::string> grid7 = {
        "....",
        ".XXX",
        ".X.X",
        ".XXX"
    };
    assert(canReach(grid7, 0, 0, 2, 3) == true);

    // Single cell grid, start and destination same
    std::vector<std::string> grid8 = {"."};
    assert(canReach(grid8, 0, 0, 0, 0) == true);

    // Single cell grid, start different from destination impossible (but same cell so true)
    // Also test a case where start is trapped and destination is X adjacent
    std::vector<std::string> grid9 = {
        ".X.",
        "X.X",
        ".X."
    };
    assert(canReach(grid9, 0, 0, 1, 1) == false);

    // Large grid, path exists
    std::vector<std::string> grid10 = {
        ".....",
        ".XXX.",
        ".X.X.",
        ".XXX.",
        "....."
    };
    assert(canReach(grid10, 0, 0, 2, 2) == true);

    return 0;
}

// The problem is a BFS/DFS traversal with a special condition at the destination. The key idea: Since you can only step on `X` if it is the destination and it is the final move, treat the destination as passable exactly once — but only as the last step. A straightforward approach: Run a BFS from the start, allowing movement only into empty cells (`.`) or into the destination cell (even if it is `X`). However, if you reach the destination during BFS, you must stop immediately (you cannot continue from it). Also, if the destination is `X`, stepping onto it is allowed only as the final move, but BFS naturally treats it as reachable; we just need to ensure that we don't accidentally treat `X` non-destination cells as passable.
//
// A more robust approach is to simulate the exact rules: BFS from start, but for each neighbor, if it is `X` and not the destination, skip. If it is the destination, then you have found a path — return `true` immediately. Otherwise, if it is `.`, push it into the queue (if not visited). After BFS, if the destination was never visited (i.e., not reachable as a final step), return `false`. But there is a subtlety: If the destination is `.`, then it is reachable normally, but you might also have the option to step off and come back, but that doesn't matter — just reaching it is enough. However, consider the case where the destination is `X` and it is adjacent to the start. BFS will immediately see it as a neighbor and return `true`, which is correct.
//
// But there is a tricky edge case: What if the destination is `X`, but it is not adjacent, and you can reach a cell adjacent to it via empty cells, then step onto it — BFS will find it. That works.
//
// Another edge case: If the destination is `X` and also the start (r1==r2 and c1==c2) — but the start is guaranteed to be `.`, so that can't happen. If the start and destination are the same cell and it is `.`, then you are already at the destination; the problem likely expects you to return `true` (you are already there). We should handle that: if start equals destination, return `true` immediately.
//
// Now, what about the original snippet's logic? It does something more nuanced: it performs BFS that allows moving into `X` only if it is the destination, but it does not stop immediately when reaching the destination during BFS — it continues and marks it visited, then later checks if the destination was visited and counts the number of adjacent cells that are either empty or the start. That is because in the original problem (from Codeforces "Ice Skating" or similar), the destination might be breakable ice `X` that breaks when you step on it, so you must step onto it as the last move, and you need at least two adjacent safe cells to step from? Actually, the original snippet's logic: It BFSes from start, treating `.` as passable and `X` only if it is the destination (and then it immediately says YES if it finds it). But then after BFS, if the destination was visited (i.e., you reached it as a final step), it also checks: for each neighbor of the destination, count how many are either `.` or the start. If that count is less than 2, it outputs NO. That seems to address a specific nuance: maybe the destination is `.` but you need to "break" it? Actually, re-reading the snippet: The BFS condition `if(s[nr][nc] == 'X' && (nr != r2 || nc != c2)) continue;` means if it's X and not destination, skip. If it's X and destination, it immediately outputs YES and returns. So if the destination is X, as soon as you find it adjacent from any reachable cell, it returns YES. That already works. Then after BFS, if the destination was not visited (meaning we never found it as X), but if it is `.`, we might have visited it. Then it checks `if(!vis[r2][c2])` returns NO — so if we never reached the destination at all, it's NO. Then it counts neighbors of destination that are either `.` or the start. If that count < 2, it outputs NO. Why? That seems to be for the case where the destination is `.` and you can reach it, but maybe you need to be able to "break" it? Actually, the snippet appears to be from a problem where both start and destination are on ice, and stepping on a `.` cell breaks it after you leave? Not sure. But the task I need to create is self-contained. I'll simplify the problem to exactly what I described: you can step on empty cells any number of times, and on the destination (even if X) only as the final step. No further constraints. So the solution is straightforward BFS. However, to be faithful to the snippet's complexity, I could include the "count neighbors" condition, but I think it's safer to describe a clean problem. Let me decide: The snippet's final "num < 2" check seems to handle the case where the destination is `.` and you can reach it, but you might need to step off it? Actually, re-reading: If destination is `.` and you reach it, BFS will mark it visited. Then the code checks `if(!vis[r2][c2])` — but it is visited, so it passes. Then it counts neighbors of destination that are either `.` or the start. If fewer than 2, it says NO. That would reject cases where the destination is accessible via a single corridor and has only one adjacent empty cell. Why would that be invalid? Possibly because in the original problem, the destination cell is also ice that breaks when you step on it, so you must be able to step onto it from a neighbor and then step back? Not sure. To avoid ambiguity, I will define a cleaner problem: You just need to reach the destination. There is no requirement to have multiple exits. So my task will be simpler. I'll write a BFS that treats the destination as reachable if it is empty or if it is X and you can step onto it as the final move. The BFS should stop when you pop the destination (or when you generate it as a neighbor). Actually, the safest: treat destination as a normal node if it is `.`, but if it is `X`, you can only enter it as the last move. In BFS, when exploring neighbors, if you encounter the destination, you can return `true` immediately (because you can step onto it as the last move). For `.` destination, you can also return `true` when you encounter it. So algorithm: BFS from start, mark visited. For each popped cell, for each neighbor: if out of bounds, skip. If the neighbor is the destination, return `true` (since you can step onto it). Otherwise, if the neighbor is `X`, skip. Otherwise (`.`), if not visited, push it. If BFS finishes without returning, return `false`. Edge case: start equals destination: return `true`. Time O(n*m), space O(n*m).
