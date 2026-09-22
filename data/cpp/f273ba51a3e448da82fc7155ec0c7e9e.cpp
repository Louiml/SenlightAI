/*
You are given a grid of size `n` × `m` where each cell is either open (represented by `'O'`) or blocked (represented by any character other than `'O'`). A wizard starts at a given cell `(startR, startC)` and can move in the four cardinal directions (up, down, left, right) through open cells only. There is also a special "crystal" cell `(crystalR, crystalC)` that, when activated, instantly illuminates all open cells in the eight compass directions (horizontal, vertical, and both diagonals) that are continuously visible from it, meaning the line extends until the first blocked cell or the grid boundary. The wizard wants to reach **any** cell that is illuminated by the crystal (including the crystal cell itself, which is illuminated). The wizard cannot pass through blocked cells. The goal is to compute the **minimum number of moves** required for the wizard to step onto an illuminated cell. If the wizard starts already on an illuminated cell, the answer is `0`. If it is impossible to reach any illuminated cell, output `-1`. Write a function `int minMovesToCrystal(const vector<string>& grid, int startR, int startC, int crystalR, int crystalC)` that returns this minimum moves count.
*/
#include <vector>
#include <string>
#include <queue>
#include <utility>

// Directions: up, down, left, right
const int dr[4] = {-1, 1, 0, 0};
const int dc[4] = {0, 0, -1, 1};

// 8 directions for the illumination rays
const int kr[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
const int kc[8] = {-1, 0, 1, -1, 1, -1, 0, 1};

int minMovesToCrystal(const std::vector<std::string>& grid, int startR, int startC, int crystalR, int crystalC) {
    int n = static_cast<int>(grid.size());
    int m = static_cast<int>(grid[0].size());

    // If start or crystal is out of bounds or blocked -> impossible (but assume valid input)
    if (startR < 0 || startR >= n || startC < 0 || startC >= m ||
        crystalR < 0 || crystalR >= n || crystalC < 0 || crystalC >= m) {
        return -1;
    }
    if (grid[startR][startC] != 'O' || grid[crystalR][crystalC] != 'O') {
        return -1;
    }

    // Mark illuminated cells
    std::vector<std::vector<bool>> illuminated(n, std::vector<bool>(m, false));
    illuminated[crystalR][crystalC] = true;
    for (int dir = 0; dir < 8; ++dir) {
        int r = crystalR + kr[dir];
        int c = crystalC + kc[dir];
        while (r >= 0 && r < n && c >= 0 && c < m && grid[r][c] == 'O') {
            illuminated[r][c] = true;
            r += kr[dir];
            c += kc[dir];
        }
    }

    // BFS from start
    std::vector<std::vector<int>> dist(n, std::vector<int>(m, -1));
    std::queue<std::pair<int,int>> q;
    dist[startR][startC] = 0;
    q.push({startR, startC});

    while (!q.empty()) {
        auto [r, c] = q.front();
        q.pop();

        if (illuminated[r][c]) {
            return dist[r][c];
        }

        for (int d = 0; d < 4; ++d) {
            int nr = r + dr[d];
            int nc = c + dc[d];
            if (nr >= 0 && nr < n && nc >= 0 && nc < m &&
                grid[nr][nc] == 'O' && dist[nr][nc] == -1) {
                dist[nr][nc] = dist[r][c] + 1;
                q.push({nr, nc});
            }
        }
    }

    return -1;
}
#include <cassert>
#include <vector>
#include <string>

// The solution function is already defined above.
// Test harness
int main() {
    // Simple grid: 3x3, crystal at (0,0), start at (2,2) but diagonal blocked?
    {
        std::vector<std::string> grid = {
            "OOO",
            "OOO",
            "OOO"
        };
        // Illuminated from (0,0): row0 all, col0 all, diagonal (1,1),(2,2)
        // Start (2,2) is illuminated -> 0
        assert(minMovesToCrystal(grid, 2, 2, 0, 0) == 0);
    }

    // Wall between, must walk around
    {
        std::vector<std::string> grid = {
            "OOO",
            "OXO",
            "OOO"
        };
        // Crystal at (0,0), start at (2,2)
        // Illuminated from (0,0): (0,1),(0,2),(1,1? but blocked), (2,2) diagonal? Actually (0,0) to (1,1) blocked, so not illuminated. So only row0 and col0 illuminated.
        // Start (2,2) is not illuminated. Need to walk to e.g. (2,0) or (0,2) via open cells.
        // Route: (2,2)->(2,1)->(2,0) distance 2, or (2,2)->(1,2)->(0,2) distance 2. So answer 2.
        assert(minMovesToCrystal(grid, 2, 2, 0, 0) == 2);
    }

    // Unreachable due to walls
    {
        std::vector<std::string> grid = {
            "OOO",
            "OXO",
            "OXO"
        };
        // Start (2,2) is enclosed? Actually (2,2) is open, but can only move to (1,2) which is blocked? Wait lower row: (2,0) O, (2,1) X, (2,2) O. So start (2,2) can only go up to (1,2) which is blocked? Actually (1,2) is O, (1,0) O, (1,1) X. So start can go up to (1,2), then left to (1,0), then up to (0,0) which is illuminated? Yes that path exists. So not unreachable. Let's design a truly unreachable case: start in a separated area.
        // Better test: grid 3x3, crystal at (0,0), start at (2,2), but a wall along diagonal (1,1) and also (1,2) blocked?
        // We'll just test a known unreachable: 
        grid = {
            "OOO",
            "XXO",
            "OOO"
        };
        // Start (2,2) can go up to (1,2) which is open, then up to (0,2) which is illuminated? (0,2) is on row0 illuminated from (0,0). So reachable. 
        // Let's test a simple unreachable: start at bottom-left corner, crystal at top-right, both separated by a vertical wall? Actually with open grid it's always reachable unless walls isolate.
        // We'll test: 
        grid = {
            "OOO",
            "OXO",
            "OOO"
        };
        // But we already did reachable. 
        // Here's an explicit unreachable: start at (2,0) but all paths to illuminated blocked by a wall at (1,0) and (2,1) blocked? But then start can't move anywhere. 
        grid = {
            "OOO",
            "XOO",
            "OXX"
        };
        // start (2,0) is open, but neighbors: (1,0) X, (2,1) X, so stuck. So -1.
        assert(minMovesToCrystal(grid, 2, 0, 0, 2) == -1);
    }

    // Start on illuminated cell directly
    {
        std::vector<std::string> grid = {
            "OOO",
            "OXO",
            "OOO"
        };
        // Crystal at (1,0), start at (0,0) is illuminated (vertical line down from (1,0) to (0,0) open)
        assert(minMovesToCrystal(grid, 0, 0, 1, 0) == 0);
    }

    // Need to move around a wall to reach a diagonal illumination line
    {
        std::vector<std::string> grid = {
            "OOOO",
            "OXXO",
            "OOOO",
            "OOOO"
        };
        // Crystal at (0,0). Illuminated line along diagonal (1,1) is blocked at (1,1) because X, so not illuminated. 
        // But line along row0: (0,1),(0,2),(0,3) illuminated. 
        // Start at (3,3) must go up to (3,0) then up? Actually (3,0) is open, then (2,0) open, (1,0) open, (0,0) illuminated? But (0,0) is crystal, distance from (3,3) to (0,0) is 6? Let's compute BFS: (3,3)->(2,3)->(1,3->(0,3) illuminated? (0,3) is on row0, so distance 3. Actually (0,3) is illuminated. So shortest = 3.
        assert(minMovesToCrystal(grid, 3, 3, 0, 0) == 3);
    }

    // Empty grid with no open cells besides start? Actually we assume start open.
    // Test that function returns -1 if start is not open (defensive).
    {
        std::vector<std::string> grid = {
            "OXO",
            "OOO",
            "OOO"
        };
        // start (0,1) is X, should return -1
        assert(minMovesToCrystal(grid, 0, 1, 1, 1) == -1);
    }

    return 0;
}
// The solution consists of two main phases. First, we need to determine which cells are illuminated by the crystal. We simulate the eight directional rays from the crystal: for each of the 8 directions, we start from the crystal's cell and move one step at a time along that direction while the cell is within bounds and open. We mark every such cell as illuminated. Note that the crystal cell itself is also illuminated (the ray begins there).  
// Second, we perform a BFS from the wizard's starting cell across the grid, moving only in the four cardinal directions and only through open cells that have not been visited yet. We maintain a distance counter. When we pop a cell, if it is illuminated (which we can check by a separate boolean array), we return the current distance. If we exhaust the BFS without finding any illuminated cell, we return `-1`.  
// Edge cases:  
// - The starting cell may be blocked, but the problem guarantees the wizard stands on an open cell? We should still handle it defensively: if the starting cell is not open, return `-1`.  
// - The crystal cell might be blocked? According to the problem, the crystal is placed on a cell; we assume it is open.  
// - The starting cell may coincide with the crystal or be illuminated directly, giving distance `0`.  
// - Diagonal moves are not allowed for the wizard; only cardinal moves.  
// - The illumination rays can pass through open cells only; the first blocked cell stops the ray, and the blocked cell itself is not illuminated.  
// Time complexity: Marking illumination takes O((n+m)·8) = O(n+m) per ray but overall O(n·m) in the worst case because each cell might be visited from multiple directions? Actually each ray is at most max(n,m) long, and there are 8 directions, so O(n+m) for marking. The BFS is O(n·m) in the worst case. Space complexity is O(n·m) for the visited/illuminated arrays and the BFS queue.
