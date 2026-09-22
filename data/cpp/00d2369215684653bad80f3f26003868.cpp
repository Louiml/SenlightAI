You are given a 2D grid of varying column heights, represented by an array `g` where `g[i]` is the number of valid rows (0 through `g[i]`) in column `i` (1-indexed). The grid has `n` columns, and the maximum possible height of any column is 80. You start at a given coordinate `(startX, startY)` where `1 <= startX <= n` and `0 <= startY <= g[startX]`. From any cell `(x, y)` you can move in one of four cardinal directions (up, down, left, right). However, moving is constrained by the column heights: if moving right or left into column `mx`, you may only occupy a row `my` such that `0 <= my <= g[mx]`. If you attempt to move to a cell that is outside the valid range for that column, the move is invalid and must be discarded. Additionally, there is a special "wrap-around" rule: if you are in column `x`, row `y`, and you move left (decreasing `y`) to `y = -1` while `x > 1`, you "fall" to column `x-1` at the bottom row of that column (i.e., row `g[x-1]`). Similarly, if you move right (increasing `y`) to `y = g[x]+1` while `x < n`, you "fall" to column `x+1` at row 0. If you move up or down, the row changes by ±1, but if the new row exceeds `g[x]`, you simply clamp it to `g[x]` (i.e., you cannot go above the top of the column, so you stay at the top). Write a C++ function `int shortestPath(int n, const std::vector<int>& g, int startX, int startY, int targetX, int targetY)` that returns the minimum number of moves to reach the target cell `(targetX, targetY)`, or `-1` if it is unreachable. The grid is small (max `n=120`, max height 80), so a BFS over all valid cells is appropriate. Note that the input coordinates are 1-indexed for columns and 0-indexed for rows.
#include <cassert>
#include <vector>

// The solution function is defined above (in the Solution section).

int main() {
    // Example 1: simple grid, all heights 2, start (1,0) target (3,2)
    // Path: (1,0)->(2,0)->(3,0)->(3,1)->(3,2) or other, length 4
    {
        int n = 3;
        std::vector<int> g = {0, 2, 2, 2}; // 1-indexed, g[0] unused
        assert(shortestPath(n, g, 1, 0, 3, 2) == 4);
    }

    // Example 2: wrap-around left from column 2, row 0 to column 1, row g[1]
    {
        int n = 2;
        std::vector<int> g = {0, 3, 5};
        // Start at (2,0): move left -> (1,3) (since g[1]=3), then move down to (1,2)
        // Target (1,2): distance 2
        assert(shortestPath(n, g, 2, 0, 1, 2) == 2);
    }

    // Example 3: wrap-around right from column 1, row g[1] to column 2, row 0
    {
        int n = 2;
        std::vector<int> g = {0, 4, 4};
        // Start (1,4), move right -> (2,0), then move down/up etc. Target (2,3)
        // (1,4)->(2,0)->(2,1)->(2,2)->(2,3): length 4
        assert(shortestPath(n, g, 1, 4, 2, 3) == 4);
    }

    // Example 4: target unreachable because of isolated high column? Actually all valid cells are reachable by up/down/left/right with wrap? Let's test a case where start and target in different "components" due to wrap restrictions? But with the given rules, it's basically fully connected if n>1. For n=1, only one column, no horizontal moves, but vertical moves clamp: so from (1,0) you can reach all rows up to g[1] via up moves (and back down), so still connected. Test n=1:
    {
        int n = 1;
        std::vector<int> g = {0, 5};
        assert(shortestPath(n, g, 1, 0, 1, 5) == 5); // 5 upward moves
        assert(shortestPath(n, g, 1, 3, 1, 0) == 3); // 3 downward moves
    }

    // Example 5: inconsistent coordinates (out of bounds) should return -1 because never reachable
    {
        int n = 2;
        std::vector<int> g = {0, 2, 2};
        assert(shortestPath(n, g, 1, 0, 3, 0) == -1); // target column 3 out of range
    }

    // Example 6: start equals target
    {
        int n = 3;
        std::vector<int> g = {0, 1, 1, 1};
        assert(shortestPath(n, g, 2, 1, 2, 1) == 0);
    }

    // Example 7: move up clamps at top; from (1,0) to (1,2) where g[1]=2, distance 2 directly
    {
        int n = 1;
        std::vector<int> g = {0, 2};
        assert(shortestPath(n, g, 1, 0, 1, 2) == 2);
    }

    // Example 8: move that attempts to go above top, clamps to top, so from (1,0) to (1,2) where g[1]=0? Actually g[1]=0 means only row 0 exists; from (1,0) moving up attempts to go to (1,1) but clamped to (1,0) so distance 0 stays.
    {
        int n = 1;
        std::vector<int> g = {0, 0};
        assert(shortestPath(n, g, 1, 0, 1, 0) == 0);
    }

    return 0;
}
#include <vector>
#include <queue>
#include <limits>

// Returns the minimum number of moves from (startX, startY) to (targetX, targetY)
// on a grid with column heights g[1..n]. Returns -1 if unreachable.
int shortestPath(int n, const std::vector<int>& g, int startX, int startY, int targetX, int targetY) {
    const int INF = std::numeric_limits<int>::max();
    // Dimensions: n columns, heights from 0 to 80 inclusive
    std::vector<std::vector<int>> dist(n + 1, std::vector<int>(81, INF));

    std::queue<std::pair<int,int>> q;
    dist[startX][startY] = 0;
    q.emplace(startX, startY);

    // Directions: down, right, up, left (dx, dy)
    int dx[4] = {1, 0, -1, 0};
    int dy[4] = {0, 1, 0, -1};

    while (!q.empty()) {
        int x = q.front().first;
        int y = q.front().second;
        q.pop();

        // If we reached target, we can early exit (but careful: BFS ensures shortest)
        // We'll continue to be safe, but could break here.

        for (int i = 0; i < 4; ++i) {
            int mx = x + dx[i];
            int my = y + dy[i];

            // Apply movement rules:
            // Left move (x same, y decreases)
            if (dx[i] == 0 && dy[i] == -1) {
                if (my < 0) {
                    if (x - 1 >= 1) {
                        mx = x - 1;
                        my = g[mx];
                    } else {
                        continue; // invalid: cannot move left from column 1
                    }
                }
            }
            // Right move (x same, y increases)
            else if (dx[i] == 0 && dy[i] == 1) {
                if (my > g[x]) {
                    if (x + 1 <= n) {
                        mx = x + 1;
                        my = 0;
                    } else {
                        continue; // invalid: cannot move right from column n
                    }
                }
            }
            // Up/down move (x changes, y same)
            else if (dx[i] != 0) {
                // When moving vertically, clamp y to g[mx] if necessary
                if (my > g[mx]) {
                    my = g[mx];
                }
                // also ensure my >= 0 (since y starts >=0 and dy=-1 could make -1, but that's invalid)
            }

            // Validate the resulting cell
            if (mx >= 1 && mx <= n && my >= 0 && my <= g[mx]) {
                if (dist[x][y] + 1 < dist[mx][my]) {
                    dist[mx][my] = dist[x][y] + 1;
                    q.emplace(mx, my);
                }
            }
        }
    }

    if (dist[targetX][targetY] == INF) {
        return -1;
    }
    return dist[targetX][targetY];
}
// The problem is a shortest-path search on a graph where each valid cell `(x, y)` (with `1 <= x <= n`, `0 <= y <= g[x]`) is a node, and edges exist between neighboring cells according to the movement rules. The movement rules are identical to four-directional movement, but with specific boundary handling: moving left from `y=0` in column `x` teleports to `(x-1, g[x-1])` if `x>1`; moving right from `y=g[x]` in column `x` teleports to `(x+1, 0)` if `x<n`; moving up from `y=g[x]` results in staying at `(x, g[x])` (since the row is clamped); moving down from `y=0` to `y=-1` is invalid. Because each edge has unit weight, we can use a Breadth-First Search (BFS) from the start cell. The state space is at most `n * 81` cells (since heights are between 0 and 80), which is at most `120*81 = 9720` nodes, so BFS is efficient. We initialize a 2D distance array with a large value, set the start distance to 0, and push it into a queue. Then, for each popped cell, we generate the four candidate moves according to the rules, check if they produce a valid cell (within the grid and within height bounds), and relax the distance if the new cell is unvisited or can be reached with fewer moves. After BFS completes, we read the target distance. If it remains at the sentinel (infinity), the target is unreachable. The time complexity is O(n * H) where H is the maximum height (80), because each cell is visited at most once and each visit processes four neighbors. The space complexity is also O(n * H) for the distance array and the queue.
