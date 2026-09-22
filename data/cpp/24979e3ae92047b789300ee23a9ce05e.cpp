// You are given a rectangular grid of size \( m \times n \) where each cell is either open (`.`) or blocked (`#`). You also have a maximum step length \( k \). From any open cell, you may move in a straight horizontal or vertical direction to any open cell that is at most \( k \) cells away in that direction, provided there are no blocked cells between the starting and destination cells (the destination must be open). Write a C++ function `int shortestPath(int m, int n, int k, const std::vector<std::string>& grid, int startR, int startC, int targetR, int targetC)` that returns the minimum number of moves needed to go from the start cell to the target cell, or `-1` if unreachable. Both coordinates are 1-indexed. The grid is guaranteed to contain exactly \( m \) rows and \( n \) columns, and start and target cells are open.
#include <cassert>
#include <vector>
#include <string>
#include "solution.h" // assuming the function is in separate file or above

int main() {
    // Simple 3x3 open grid, k=1 (only adjacent moves)
    std::vector<std::string> grid1 = {
        "...",
        "...",
        "..."
    };
    assert(shortestPath(3,3,1,grid1,1,1,3,3) == 4); // (1,1)->(1,2)->(1,3)->(2,3)->(3,3)

    // Same grid but k=2: can jump two cells
    assert(shortestPath(3,3,2,grid1,1,1,3,3) == 2); // (1,1)->(1,3)->(3,3)

    // Start equals target
    assert(shortestPath(3,3,2,grid1,2,2,2,2) == 0);

    // Single blocked cell in middle, k=2
    std::vector<std::string> grid2 = {
        "...",
        ".#.",
        "..."
    };
    // Path around the block: (1,1)->(1,3)->(3,3) with k=2
    assert(shortestPath(3,3,2,grid2,1,1,3,3) == 2);

    // Blocked start target? (not open) – but task guarantees open, we just test with blocked target as unreachable if never reached
    // Here target is open but unreachable due to wall
    std::vector<std::string> grid3 = {
        ".#.",
        ".#.",
        ".#."
    };
    // Vertical wall, left side (1,1) vs right side (1,3) separated by wall, k=1
    assert(shortestPath(3,3,1,grid3,1,1,3,3) == -1); // cannot cross

    // k=0: no moves, only same cell
    assert(shortestPath(3,3,0,grid1,1,1,2,2) == -1);
    assert(shortestPath(3,3,0,grid1,2,2,2,2) == 0);

    // Multiple unvisited cells in a row, k=10, all open
    std::vector<std::string> grid4 = {
        ".....",
        ".....",
        "....."
    };
    assert(shortestPath(3,5,10,grid4,1,1,3,5) == 2); // right then down

    // Large k but blocked boundaries: single column with open cells separated by wall
    std::vector<std::string> grid5 = {
        ".#.",
        ".#.",
        "..."
    };
    // From (1,1) to (3,3) with k=2: can go down to (3,1) then right
    assert(shortestPath(3,3,2,grid5,1,1,3,3) == 2);

    return 0;
}
#include <vector>
#include <string>
#include <queue>
#include <set>
#include <algorithm>
#include <climits>

// Returns the minimum number of moves from start to target, or -1.
int shortestPath(int m, int n, int k,
                 const std::vector<std::string>& grid,
                 int startR, int startC, int targetR, int targetC) {
    // Direction reachable distances, initialized to -1 for blocked cells.
    std::vector<std::vector<int>> up(m+2, std::vector<int>(n+2, -1));
    std::vector<std::vector<int>> down(m+2, std::vector<int>(n+2, -1));
    std::vector<std::vector<int>> left(m+2, std::vector<int>(n+2, -1));
    std::vector<std::vector<int>> right(m+2, std::vector<int>(n+2, -1));
    std::vector<std::vector<int>> dist(m+2, std::vector<int>(n+2, -1));

    // Precompute upward reachable distance.
    for (int r = 1; r <= m; ++r) {
        for (int c = 1; c <= n; ++c) {
            if (grid[r-1][c-1] == '#') continue;
            int val = (r == 1) ? 0 : up[r-1][c];
            up[r][c] = (val == -1) ? 1 : std::min(val + 1, k);
        }
    }
    // Precompute downward reachable distance.
    for (int r = m; r >= 1; --r) {
        for (int c = 1; c <= n; ++c) {
            if (grid[r-1][c-1] == '#') continue;
            int val = (r == m) ? 0 : down[r+1][c];
            down[r][c] = (val == -1) ? 1 : std::min(val + 1, k);
        }
    }
    // Precompute left reachable distance.
    for (int r = 1; r <= m; ++r) {
        for (int c = 1; c <= n; ++c) {
            if (grid[r-1][c-1] == '#') continue;
            int val = (c == 1) ? 0 : left[r][c-1];
            left[r][c] = (val == -1) ? 1 : std::min(val + 1, k);
        }
    }
    // Precompute right reachable distance.
    for (int r = 1; r <= m; ++r) {
        for (int c = n; c >= 1; --c) {
            if (grid[r-1][c-1] == '#') continue;
            int val = (c == n) ? 0 : right[r][c+1];
            right[r][c] = (val == -1) ? 1 : std::min(val + 1, k);
        }
    }

    // Row sets: unvisited open columns per row.
    std::vector<std::set<int>> rowSets(m+1);
    // Column sets: unvisited open rows per column.
    std::vector<std::set<int>> colSets(n+1);
    for (int r = 1; r <= m; ++r) {
        for (int c = 1; c <= n; ++c) {
            if (grid[r-1][c-1] == '.') {
                rowSets[r].insert(c);
                colSets[c].insert(r);
            }
        }
    }

    std::queue<std::pair<int,int>> q;
    // Helper to add a cell and mark as visited.
    auto addCell = [&](int r, int c, int d) {
        dist[r][c] = d;
        rowSets[r].erase(c);
        colSets[c].erase(r);
        q.push({r,c});
    };

    addCell(startR, startC, 0);
    while (!q.empty()) {
        auto [r, c] = q.front();
        q.pop();
        if (r == targetR && c == targetC) {
            return dist[r][c];
        }
        int d = dist[r][c] + 1;
        // Move right within reachable distance.
        while (!rowSets[r].empty() && *rowSets[r].rbegin() > c) {
            auto it = rowSets[r].lower_bound(c);
            if (it == rowSets[r].end()) break;
            int nc = *it;
            if (nc <= c + right[r][c]) {
                addCell(r, nc, d);
            } else break;
        }
        // Move left within reachable distance.
        while (!rowSets[r].empty() && *rowSets[r].begin() < c) {
            auto it = rowSets[r].lower_bound(c);
            if (it == rowSets[r].begin()) break;
            --it;
            int nc = *it;
            if (nc >= c - left[r][c]) {
                addCell(r, nc, d);
            } else break;
        }
        // Move down within reachable distance.
        while (!colSets[c].empty() && *colSets[c].rbegin() > r) {
            auto it = colSets[c].lower_bound(r);
            if (it == colSets[c].end()) break;
            int nr = *it;
            if (nr <= r + down[r][c]) {
                addCell(nr, c, d);
            } else break;
        }
        // Move up within reachable distance.
        while (!colSets[c].empty() && *colSets[c].begin() < r) {
            auto it = colSets[c].lower_bound(r);
            if (it == colSets[c].begin()) break;
            --it;
            int nr = *it;
            if (nr >= r - up[r][c]) {
                addCell(nr, c, d);
            } else break;
        }
    }
    return -1;
}
// We need a BFS shortest path on a graph where each node is an open cell. The naive approach of adding an edge from a cell to every open cell within distance \( k \) in four directions would be too slow for large grids because it could be \( O(k \cdot m \cdot n) \) per direction. Instead, we precompute for each cell the maximum distance it can reach in each of the four directions without hitting a blocked cell, capped at \( k \). These are:
// - `hi[i][j]`: distance upward (towards row 1) from `(i,j)` before a blocked cell or boundary, capped at `k`.
// - `lo[i][j]`: distance downward (towards row m).
// - `le[i][j]`: distance leftward (towards column 1).
// - `ri[i][j]`: distance rightward (towards column n).
//
// We maintain for each row a `std::set<int>` of column indices of unvisited open cells in that row, and for each column a `std::set<int>` of row indices of unvisited open cells in that column. When BFS visits a cell `(r,c)`, we need to explore all unvisited open cells in the same row within `ri[r][c]` to the right and `le[r][c]` to the left, and in the same column within `lo[r][c]` downward and `hi[r][c]` upward. We use the set to quickly find the nearest unvisited cell in each direction: for right, we get `lower_bound(c)` (but we should skip `c` itself, and because `c` is removed from the set when visited, `lower_bound(c)` gives the first greater column). For left, we use `prev(lower_bound(c))`. Similarly for columns.
//
// When we encounter such a cell, we remove it from both its row and column sets and push it into the BFS queue with distance `dist+1`. This ensures each cell is processed exactly once. The BFS terminates when we pop the target cell, or returns `-1` if queue empties.
//
// Time complexity: Each cell is inserted into a set once and removed once, so each set operation is \( O(\log n) \) or \( O(\log m) \). The while loops in each direction may iterate over many cells, but total over all visits is \( O(mn \log(\max(n,m))) \). Precomputation is \( O(mn) \). Space complexity is \( O(mn) \) for the distance and direction arrays plus the sets.
//
// Edge cases: start equals target (distance 0), unreachable target, blocked cells, \( k=0 \) (no moves unless start==target), and large grids where BFS must skip many cells efficiently.
