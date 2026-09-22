// You are given a rectangular grid of `R` rows and `C` columns, where some cells are blocked (considered mines) and all others are free. A robot starts at a given start cell `(sr, sc)` and wants to reach a target cell `(tr, tc)`. The robot can move only up, down, left, or right into adjacent free cells, and cannot step onto blocked cells or leave the grid. Every move counts as 1 unit of time. Write a C++ function `int shortestPath(int R, int C, const vector<pair<int,int>>& blocked, pair<int,int> start, pair<int,int> target)` that returns the minimum number of moves to reach the target, or `-1` if it is impossible. The grid coordinates are 0-indexed (0 ≤ row < R, 0 ≤ col < C). The start and target cells are guaranteed to be free (not in `blocked`). The grid may be large (up to 10^4 rows and 10^4 columns, total cells up to 10^8), so avoid a full explicit grid if possible; use a hash set for blocked cells.

// This is a classic shortest-path problem on an unweighted grid, solvable with Breadth-First Search (BFS). The graph has nodes as free cells, and edges between orthogonal adjacent free cells. BFS from the start cell gives the shortest number of moves because all edges have equal weight (1). We track visited status using a hash set of visited coordinates (or a 2D boolean array if the grid is small enough, but we must consider memory). Edge cases: if start equals target, return 0; if target is unreachable because blocked or isolated, return -1. Also, if start or target is outside the grid, return -1 (though the input guarantees inside). When blocking cells, we store them in an unordered_set with a combined key (row * C + col). During BFS, for each neighbor, check bounds, not blocked, and not visited. Time complexity is O(R*C) in the worst case if all cells are free and we visit all, but with a hash set for blocked cells, we still may visit up to the number of free cells. Space complexity is O(number of free cells) for the visited set and queue, but using a 2D array of booleans would be O(R*C). Since the grid can be up to 10^8 cells, we should use the hash set approach to store only visited cells (which could be as many as free cells, but that is still large; however, it is more flexible). The solution below uses an unordered_set of long long keys for visited and blocked.

#include <vector>
#include <queue>
#include <unordered_set>
#include <utility>

// Return minimum moves from start to target, skipping blocked cells.
// Return -1 if unreachable.
int shortestPath(int R, int C,
                 const std::vector<std::pair<int,int>>& blocked,
                 std::pair<int,int> start,
                 std::pair<int,int> target) {
    // Helper key for a cell.
    auto key = [C](int r, int c) -> long long {
        return static_cast<long long>(r) * C + c;
    };

    // Store blocked cells.
    std::unordered_set<long long> blockedSet;
    for (const auto& p : blocked) {
        blockedSet.insert(key(p.first, p.second));
    }

    // BFS queue of (row, col), and distances are implicit from level count.
    std::queue<std::pair<int,int>> q;
    std::unordered_set<long long> visited;

    int sr = start.first, sc = start.second;
    int tr = target.first, tc = target.second;

    // Check if start or target out of bounds.
    if (sr < 0 || sr >= R || sc < 0 || sc >= C ||
        tr < 0 || tr >= R || tc < 0 || tc >= C) {
        return -1;
    }

    // Start BFS.
    q.push({sr, sc});
    visited.insert(key(sr, sc));
    int distance = 0;

    // Directions: up, down, left, right.
    const int dr[4] = {-1, 1, 0, 0};
    const int dc[4] = {0, 0, -1, 1};

    while (!q.empty()) {
        int levelSize = q.size();
        for (int i = 0; i < levelSize; ++i) {
            auto [r, c] = q.front(); q.pop();
            if (r == tr && c == tc) {
                return distance;
            }
            for (int d = 0; d < 4; ++d) {
                int nr = r + dr[d];
                int nc = c + dc[d];
                if (nr >= 0 && nr < R && nc >= 0 && nc < C) {
                    long long nk = key(nr, nc);
                    if (blockedSet.find(nk) == blockedSet.end() &&
                        visited.find(nk) == visited.end()) {
                        visited.insert(nk);
                        q.push({nr, nc});
                    }
                }
            }
        }
        ++distance;
    }

    // Target not reached.
    return -1;
}

#include <cassert>
#include <vector>
#include <utility>

// Assuming the solution function is defined above.

int main() {
    // Basic open grid 3x3, start (0,0), target (2,2).
    std::vector<std::pair<int,int>> blocked1;
    assert(shortestPath(3, 3, blocked1, {0,0}, {2,2}) == 4);

    // Start equals target.
    assert(shortestPath(5, 5, blocked1, {1,2}, {1,2}) == 0);

    // Blocked path forces detour.
    std::vector<std::pair<int,int>> blocked2 = {{1,1}, {1,2}};
    // From (0,0) to (2,2): shortest is down to (2,0), right to (2,2) = 4 moves? Actually (0,0)->(1,0)->(2,0)->(2,1)->(2,2) = 4.
    assert(shortestPath(3, 3, blocked2, {0,0}, {2,2}) == 4);

    // Target completely blocked off.
    std::vector<std::pair<int,int>> blocked3 = {{1,0}, {1,1}, {1,2}};
    // Start (0,0), target (2,1). The only way down is blocked, so unreachable.
    assert(shortestPath(3, 3, blocked3, {0,0}, {2,1}) == -1);

    // Single row.
    std::vector<std::pair<int,int>> blocked4;
    assert(shortestPath(1, 5, blocked4, {0,0}, {0,4}) == 4);
    std::vector<std::pair<int,int>> blocked5 = {{0,2}};
    assert(shortestPath(1, 5, blocked5, {0,0}, {0,4}) == -1);

    // Large grid: 10000x10000, but all free, start (0,0), target (9999,9999).
    // This would take too long to BFS all, but we can test a smaller portion.
    // For correctness, test a moderately sized grid with a known path.
    std::vector<std::pair<int,int>> blocked6;
    assert(shortestPath(10, 10, blocked6, {0,0}, {9,9}) == 18);

    // Blocked entire border except one way.
    std::vector<std::pair<int,int>> blocked7 = {{0,1},{0,2},{0,3},{1,3},{2,3},{3,3}};
    // Start (0,0), target (3,0). Path: (0,0)->(1,0)->(2,0)->(3,0) = 3 moves.
    assert(shortestPath(4, 4, blocked7, {0,0}, {3,0}) == 3);
    // Target (3,3) is blocked? No, (3,3) not in blocked, but can we reach? Path from (0,0): go right to (0,0) then down to (3,0) then right to (3,3) but (3,1) and (3,2) are free? They are not blocked, so yes reachable: (0,0)->(1,0)->(2,0)->(3,0)->(3,1)->(3,2)->(3,3) = 6.
    assert(shortestPath(4, 4, blocked7, {0,0}, {3,3}) == 6);

    return 0;
}
