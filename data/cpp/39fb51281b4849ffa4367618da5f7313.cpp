You are given a rectangular grid with `n` rows and `m` columns. Each cell contains either a wall (`#`), a floor (`.`), a good person (`G`), or a bad person (`B`). All good and bad persons are standing on floor cells initially, and they can move up, down, left, or right, but cannot pass through walls. A good person must be able to reach the bottom-right cell `(n-1, m-1)`. You may change any number of floor (`.`) cells into walls (`#`) before anyone starts moving. Your goal is to determine whether it is possible to place walls (only on floor cells) so that: (1) every good person can reach the destination, and (2) no bad person can reach the destination. Write a C++ function `bool canProtectGood(const vector<string>& grid)` that takes the grid and returns `true` if such a wall placement is possible, otherwise `false`. The grid dimensions are at least 1×1, and the destination cell may contain a wall, a floor, a good, or a bad person. If the destination is initially a bad person, then no placement can allow a good person to reach it (since the bad person would also be there), except if no good persons exist—then the answer is trivially `true` (no good person to protect). If a good person is initially adjacent to a bad person, you can wall off the floor cells around the good person, but be careful: placing a wall on a floor cell may block paths for other good persons. The only cells you may convert to walls are floor cells (`.`), not cells containing `G`, `B`, or `#`. The function should be standalone; ignore any input/output formatting.

// The key observation: A good person can be blocked by placing walls on all adjacent floor cells, but that is often unnecessary and may harm other good persons. The correct approach is to first wall off all floor cells that are adjacent (up, down, left, right) to any bad person, because any good person that can reach such a floor cell could be intercepted by the bad person moving onto that floor cell, and then the bad person could follow the good person's path to the destination. Therefore, for every bad person, change all neighboring `.` cells to `#`. If after this transformation any good person becomes adjacent to a bad person (because the bad person is on an original `B` cell and the good person is on an original `G` cell that are neighbors), that is impossible because we cannot wall off the `G` cell itself. So we check that after marking adjacent floors as walls, no `G` is adjacent to a `B` (including diagonal? No, only orthogonal adjacency matters because movement is orthogonal). Then, we need to verify that every good person can still reach the destination. The destination cell might be a `.` that we turned into `#` because it was adjacent to a bad person. If the destination becomes a wall, then no one can reach it, so if there is at least one good person, the answer is `false`. If there are no good persons, the answer is `true` regardless of bad persons (since we don't need to protect anyone). Also, if any bad person is already on the destination, then no good person can reach it (because the bad person would be there), so if there is at least one good person, answer is `false`. If there are no good persons, answer is `true`. After performing the wall conversions, run a DFS/BFS from each good person to the destination, treating walls as impassable; if any good person cannot reach, return `false`. If all reach, return `true`. Time complexity is O(n*m) for the conversions and O(n*m) per DFS, but each cell is visited at most once per DFS, and we run one DFS per good person. In the worst case with many good persons, it could be O(G * n*m), but typically we can optimize by running a multi-source BFS from all good persons simultaneously or just run one DFS from each (the problem constraints are small). Space complexity is O(n*m) for the visited matrix and the modified grid.

#include <vector>
#include <string>
#include <queue>

// Return true if it is possible to place walls on floor cells so that
// all good persons can reach the bottom-right cell and no bad person can.
bool canProtectGood(const std::vector<std::string>& grid) {
    int n = static_cast<int>(grid.size());
    if (n == 0) return true; // no cells, trivially true
    int m = static_cast<int>(grid[0].size());
    
    std::vector<std::string> g = grid; // copy to modify
    
    // Count good persons and check destination status
    int goodCount = 0;
    bool destWallOrBad = false;
    char destChar = g[n-1][m-1];
    if (destChar == '#' || destChar == 'B') destWallOrBad = true;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (g[i][j] == 'G') ++goodCount;
        }
    }
    if (goodCount == 0) return true; // nothing to protect
    
    // If destination is initially a wall or a bad person, no good can reach it
    if (destWallOrBad) return false;
    
    // Directions: up, down, left, right
    const int dx[4] = {-1, 1, 0, 0};
    const int dy[4] = {0, 0, -1, 1};
    
    // Step 1: For each bad person, turn all neighboring floor cells into walls
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (g[i][j] == 'B') {
                for (int d = 0; d < 4; ++d) {
                    int ni = i + dx[d];
                    int nj = j + dy[d];
                    if (ni >= 0 && ni < n && nj >= 0 && nj < m && g[ni][nj] == '.') {
                        g[ni][nj] = '#';
                    }
                }
            }
        }
    }
    
    // Step 2: After conversion, check that no good person is adjacent to a bad person
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (g[i][j] == 'G') {
                for (int d = 0; d < 4; ++d) {
                    int ni = i + dx[d];
                    int nj = j + dy[d];
                    if (ni >= 0 && ni < n && nj >= 0 && nj < m && g[ni][nj] == 'B') {
                        return false; // bad person can immediately move to good's cell
                    }
                }
            }
        }
    }
    
    // Step 3: Check that each good person can reach the destination using BFS
    // Because grid is small, we do BFS from each good person separately.
    std::vector<std::vector<bool>> visited(n, std::vector<bool>(m, false));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (g[i][j] == 'G') {
                // Re-initialize visited for each good person
                for (auto& row : visited) std::fill(row.begin(), row.end(), false);
                std::queue<std::pair<int,int>> q;
                q.push({i,j});
                visited[i][j] = true;
                bool reached = false;
                while (!q.empty()) {
                    auto [x,y] = q.front();
                    q.pop();
                    if (x == n-1 && y == m-1) {
                        reached = true;
                        break;
                    }
                    for (int d = 0; d < 4; ++d) {
                        int nx = x + dx[d];
                        int ny = y + dy[d];
                        if (nx >= 0 && nx < n && ny >= 0 && ny < m && !visited[nx][ny] && g[nx][ny] != '#') {
                            visited[nx][ny] = true;
                            q.push({nx,ny});
                        }
                    }
                }
                if (!reached) return false;
            }
        }
    }
    
    return true;
}

#include <cassert>
#include <vector>
#include <string>

// Declare the function (it would be defined above)
bool canProtectGood(const std::vector<std::string>& grid);

int main() {
    // Simple case: all empty, one good at start, reachable without walls
    assert(canProtectGood({".G.", ".#.", "..#"}) == false);
    
    // No good persons -> always true even if bad present
    assert(canProtectGood({"B..", "#..", "..#"}) == true);
    
    // Bad next to destination, good far away, need to wall off bad's neighbors
    // Grid 3x3
    // G . .
    // . B .
    // . . .
    // After walling neighbors of B, the good can reach destination
    assert(canProtectGood({"G..", ".B.", "..."}) == true);
    
    // Good adjacent to bad from start -> impossible
    assert(canProtectGood({"GB.", "...", "..."}) == false);
    
    // Destination is a bad person, no good can reach
    assert(canProtectGood({"G..", "...", "..B"}) == false);
    
    // Good and bad separated by walls, good can reach destination
    assert(canProtectGood({"G#.", ".#.", "..."}) == true);
    
    // Multiple goods, one is trapped by walls (already blocked)
    // Grid:
    // G#G
    // .#.
    // ...
    // Top-right good cannot reach because of walls, but we can place more walls? No, it's already blocked
    assert(canProtectGood({"G#G", ".#.", "..."}) == false);
    
    // Bad adjacent to floor near path, but we can wall the floor
    // G..
    // .B.
    // ...
    // We wall the floor left and right of B? Actually only floor cells adjacent to B become walls
    // After conversion, B is isolated (neighbors become walls), good path exists via right side
    assert(canProtectGood({"G..", ".B.", "..."}) == true);
    
    // No walls needed, simple straight path
    assert(canProtectGood({"G..", "...", "..."}) == true);
    
    // Bad on the only path, but we can wall the floor around bad, but then the path for good is blocked? 
    // Let's test: 2x2
    // G B
    // . .
    // After walling floor adjacent to B (cell (1,0) and (1,1)?), the good cannot reach destination because B is on (0,1) which is not a floor, we cannot turn B into wall. The only path from G to destination is through (1,0)->(1,1), but (1,0) becomes wall. So false
    assert(canProtectGood({"GB", ".."}) == false);
    
    // Single cell with good at destination
    assert(canProtectGood({"G"}) == true);
    
    return 0;
}
