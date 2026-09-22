// Write a C++ function `int minimumObstaclesToReachEnd(vector<vector<int>>& grid)` that takes a 2D grid of non-negative integers, where each cell contains the cost of entering that cell (0 for free passage, 1 for an obstacle that must be removed), and returns the minimum total number of obstacles that must be removed to travel from the top-left cell (0,0) to the bottom-right cell (n-1, m-1). You may move only up, down, left, or right by one cell. The starting cell has zero cost and does not require removal. If the destination is unreachable, return -1. The grid dimensions will satisfy 1 ≤ n, m ≤ 1000, and each cell value is 0 or 1.
The problem is a shortest-path variant where edge weights are 0 or 1 (the cost of entering a neighbor cell). We use Dijkstra's algorithm with a priority queue (or a `set` acting as a min-heap) because all costs are non-negative and we need the minimal accumulated cost to reach each cell. We maintain a `dp` matrix initialized to infinity, with `dp[0][0] = 0`. We start from (0,0) and repeatedly extract the cell with the smallest accumulated cost from the frontier. For each extract, we examine its four neighbors; if moving into a neighbor yields a lower cost than previously recorded, we update `dp` and push the neighbor into the frontier. We stop early when we extract the destination. Since costs are only 0 or 1, this is essentially a 0-1 BFS, but Dijkstra works generically. Edge cases include when the start is also the end (returns 0), when the grid is all 1s (returns n+m-2), and when there is no path (returns -1, though with 0/1 weights the grid is always fully connected, so -1 is only for safety). Time complexity is O(n*m log(n*m)) due to heap operations, space O(n*m) for the distance matrix and frontier.
#include <vector>
#include <set>
#include <climits>

// Returns the minimum total cost (number of obstacles removed) to travel from (0,0) to (n-1,m-1).
// Each cell costs either 0 (free) or 1 (obstacle). Only orthogonal moves allowed.
int minimumObstaclesToReachEnd(std::vector<std::vector<int>>& grid) {
    const int n = static_cast<int>(grid.size());
    const int m = static_cast<int>(grid[0].size());
    const int INF = 1e9;
    
    // Distance matrix initialized to infinity
    std::vector<std::vector<int>> dist(n, std::vector<int>(m, INF));
    
    // Set works as a min-heap: {cost, row, col} sorted by cost first
    std::set<std::vector<int>> frontier;
    dist[0][0] = 0;
    frontier.insert({0, 0, 0});
    
    // Directions: down, left, up, right (as pairs)
    const int dr[4] = {1, 0, -1, 0};
    const int dc[4] = {0, -1, 0, 1};
    
    while (!frontier.empty()) {
        // Get the element with the smallest cost
        auto it = frontier.begin();
        int cost = (*it)[0];
        int r = (*it)[1];
        int c = (*it)[2];
        frontier.erase(it);
        
        // If we reached the destination, return the cost
        if (r == n - 1 && c == m - 1) {
            return cost;
        }
        
        // Skip if we have already found a better path to this cell
        if (dist[r][c] < cost) {
            continue;
        }
        
        // Explore four neighbors
        for (int k = 0; k < 4; ++k) {
            int nr = r + dr[k];
            int nc = c + dc[k];
            if (nr >= 0 && nr < n && nc >= 0 && nc < m) {
                int newCost = cost + grid[nr][nc];
                if (newCost < dist[nr][nc]) {
                    dist[nr][nc] = newCost;
                    frontier.insert({newCost, nr, nc});
                }
            }
        }
    }
    
    // The grid is always connected with 0/1 costs, but keep for safety
    return -1;
}
#include <cassert>
#include <vector>

// The solution function is declared above; this is the test harness.
int main() {
    // Simple 2x2 grid with no obstacles
    std::vector<std::vector<int>> grid1 = {{0,0},{0,0}};
    assert(minimumObstaclesToReachEnd(grid1) == 0);
    
    // 2x2 grid with one obstacle in the middle path
    std::vector<std::vector<int>> grid2 = {{0,1},{1,0}};
    assert(minimumObstaclesToReachEnd(grid2) == 1);
    
    // 3x3 grid requiring removal of two obstacles
    std::vector<std::vector<int>> grid3 = {{0,1,1},{1,1,0},{1,1,0}};
    assert(minimumObstaclesToReachEnd(grid3) == 2);
    
    // Single cell grid
    std::vector<std::vector<int>> grid4 = {{0}};
    assert(minimumObstaclesToReachEnd(grid4) == 0);
    
    // All obstacles except start and end
    std::vector<std::vector<int>> grid5 = {{0,1},{1,0}};
    assert(minimumObstaclesToReachEnd(grid5) == 1);
    
    // Long corridor with alternating obstacles
    std::vector<std::vector<int>> grid6 = {{0,1,0,1,0},{1,0,1,0,1},{0,0,0,0,0}};
    // Path: (0,0)->(1,0) cost 1, (2,0) cost 0, (2,1) cost 0, (2,2) cost 0, (2,3) cost 0, (2,4) cost 0, (1,4) cost 1, (0,4) cost 0 => total 2
    assert(minimumObstaclesToReachEnd(grid6) == 2);
    
    // Large grid with mixed costs
    std::vector<std::vector<int>> grid7(1000, std::vector<int>(1000, 0));
    grid7[0][1] = 1; grid7[1][1] = 1;
    assert(minimumObstaclesToReachEnd(grid7) == 1);
    
    // Another simple case with a direct diagonal impossible but orthogonal path
    std::vector<std::vector<int>> grid8 = {{0,1,1},{1,0,1},{1,0,0}};
    assert(minimumObstaclesToReachEnd(grid8) == 1); // Path: (0,0)->(1,0) cost1, (2,0) cost1, (2,1) cost0, (2,2) cost0 => total 2? Let's compute: actually 0,0->1,0 (1), 2,0 (1), 2,1 (0), 2,2 (0) = 2. Wait, but there is also 0,0->0,1 (1),1,1 (0),2,1 (0),2,2(0) = 1? No, 1,1 is 0 so path: 0,0 cost0,0,1 cost1,1,1 cost0,2,1 cost0,2,2 cost0 => 1. So assert 1.
    assert(minimumObstaclesToReachEnd(grid8) == 1);
    
    // Test unreachable? With 0/1 cells, always reachable, but test case with a wall of 1s? Actually all cells are traversable, just cost 1. So always reachable.
    // But to test -1, we can make a grid where movement is impossible? Not possible with this problem since all cells are traversable. So skip.
    
    return 0;
}
