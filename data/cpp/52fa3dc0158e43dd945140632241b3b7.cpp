/*
Write a C++ function `int minimumLootCost(const std::vector<std::vector<int>>& grid)` that, given a square grid of non-negative integers (representing the cost of entering each cell), returns the minimum total cost to travel from the top-left cell `(0,0)` to the bottom-right cell `(N-1,N-1)`. Movement is allowed only up, down, left, or right (no diagonal moves), and each cell's cost is added exactly once when you enter it, including the starting cell's cost. The grid size `N` will be between 1 and 125 inclusive, and each cell value is between 0 and 1000. The function must compute the result efficiently, as the naïve recursive approach without visited tracking could lead to exponential time. The output is the minimum possible sum of cell costs along any valid path from start to end.
*/
#include <vector>
#include <queue>
#include <limits>

// Return the minimum cost to travel from top-left to bottom-right of the grid.
int minimumLootCost(const std::vector<std::vector<int>>& grid) {
    const int N = static_cast<int>(grid.size());
    if (N == 0) return 0;
    
    const int INF = std::numeric_limits<int>::max();
    std::vector<std::vector<int>> dist(N, std::vector<int>(N, INF));
    
    // Min-heap: (cost, row, col). Use pair<int, pair<int,int>> for simplicity.
    using Node = std::pair<int, std::pair<int,int>>;
    std::priority_queue<Node, std::vector<Node>, std::greater<Node>> pq;
    
    dist[0][0] = grid[0][0];
    pq.push({grid[0][0], {0,0}});
    
    const int dr[4] = {-1, 1, 0, 0};
    const int dc[4] = {0, 0, -1, 1};
    
    while (!pq.empty()) {
        auto [cost, pos] = pq.top();
        pq.pop();
        int r = pos.first;
        int c = pos.second;
        
        // If we already found a better path, skip this stale entry.
        if (cost != dist[r][c]) continue;
        
        // Early exit if we reach the destination (optional but efficient).
        if (r == N-1 && c == N-1) break;
        
        for (int k = 0; k < 4; ++k) {
            int nr = r + dr[k];
            int nc = c + dc[k];
            if (nr >= 0 && nr < N && nc >= 0 && nc < N) {
                int newCost = cost + grid[nr][nc];
                if (newCost < dist[nr][nc]) {
                    dist[nr][nc] = newCost;
                    pq.push({newCost, {nr, nc}});
                }
            }
        }
    }
    
    return dist[N-1][N-1];
}
#include <cassert>
#include <vector>

// Solution function (declared above, but for completeness, include it here too)
int minimumLootCost(const std::vector<std::vector<int>>& grid);

int main() {
    // Test 1: 1x1 grid, result is the only cell's cost.
    assert(minimumLootCost({{5}}) == 5);
    
    // Test 2: 2x2 grid, path options: (0,0)->(0,1)->(1,1) or (0,0)->(1,0)->(1,1)
    std::vector<std::vector<int>> grid2 = {{1, 100}, {1, 1}};
    // Best: 1 (start) + 1 (down) + 1 (right) = 3; alternative: 1 + 100 + 1 = 102.
    assert(minimumLootCost(grid2) == 3);
    
    // Test 3: 3x3 grid with uniform costs of 1, all paths cost 5 (start + 4 steps).
    std::vector<std::vector<int>> grid3(3, std::vector<int>(3, 1));
    assert(minimumLootCost(grid3) == 5);
    
    // Test 4: 3x3 grid with obstacles-like high costs.
    std::vector<std::vector<int>> grid4 = {
        {0, 100, 0},
        {0, 100, 0},
        {0, 0, 0}
    };
    // Best path: go right along top, then right, then down, down? Actually better: down left column then right.
    // Path: (0,0)->(1,0)->(2,0)->(2,1)->(2,2) costs 0+0+0+0+0 = 0.
    assert(minimumLootCost(grid4) == 0);
    
    // Test 5: Larger grid with random values; manually computed for a small case.
    std::vector<std::vector<int>> grid5 = {
        {2, 8, 3},
        {1, 9, 1},
        {5, 4, 6}
    };
    // Best path: 2 -> 1 (down) -> 9? No, 2+1=3, then right to 9? Actually better: 2->8->3->1->6 = 20, or 2->1->5->4->6 = 18, or 2->1->9->1->6 = 19, or 2->8->3->1->6=20, or 2->8->9->1->6=26. So 18 is best.
    assert(minimumLootCost(grid5) == 18);
    
    // Test 6: Grid with all zeros, minimum cost is 0.
    std::vector<std::vector<int>> grid6(4, std::vector<int>(4, 0));
    assert(minimumLootCost(grid6) == 0);
    
    // Test 7: Path must wrap around, ensuring all neighbors are considered.
    std::vector<std::vector<int>> grid7 = {
        {10, 1, 10},
        {10, 1, 10},
        {10, 1, 1}
    };
    // Best path: (0,0)=10 -> (0,1)=1 -> (1,1)=1 -> (2,1)=1 -> (2,2)=1 => total 14.
    assert(minimumLootCost(grid7) == 14);
    
    // Test 8: Larger 5x5 grid with increasing costs, ensure no negative issues.
    std::vector<std::vector<int>> grid8(5, std::vector<int>(5, 0));
    // Set a high-cost wall, but path around it.
    for (int i = 0; i < 5; ++i) grid8[i][2] = 1000;
    // Grid: all zeros except column 2 is 1000. Best path: go left column all the way down, then right around bottom, then up? Actually from (0,0) to (4,4), avoid column 2. Path: (0,0)->(1,0)->(2,0)->(3,0)->(4,0)->(4,1)->(4,3)->(4,4)?? That costs 0 for all. Let's just check it is not 1000.
    assert(minimumLootCost(grid8) == 0);
    
    // Test 9: Edge case with very large grid and max costs, ensure result is finite.
    std::vector<std::vector<int>> grid9(125, std::vector<int>(125, 1000));
    // Any path has 249 steps (125+125-1), cost = 250*1000 = 250000? Actually start + 248 others = 249*1000 = 249000.
    assert(minimumLootCost(grid9) == 249000);
    
    return 0;
}
// This is a classic shortest-path problem on a weighted grid where each node is a cell, and edges connect adjacent cells with weight equal to the destination cell's cost. The total path cost includes the starting cell's cost plus the costs of all subsequent cells. The main algorithm is Dijkstra's algorithm, applied to the grid. We maintain a 2D distance array initialized to infinity, set the starting distance to `grid[0][0]`, and use a min-heap (priority queue) to always expand the cell with the current smallest known distance. For each popped cell, we examine its four neighbors (checking bounds). If going through the current cell gives a lower cost to that neighbor (current distance + neighbor's cell cost < neighbor's current distance), we update the neighbor's distance and push it into the heap. Because all edge weights are non-negative, Dijkstra's algorithm guarantees correctness. A critical edge case is a 1x1 grid, where the answer is simply `grid[0][0]` (no movement needed). Also note that unlike typical grid problems, we do not need a visited set because the distance checks prevent redundant processing, but a visited flag can be used for efficiency to avoid re-processing a finalized node, although it's not strictly necessary. The time complexity is `O(N^2 log N)` due to potentially `N^2` nodes each pushed once, and each heap operation is `O(log(N^2)) = O(log N)`. Space complexity is `O(N^2)` for the distance array and heap.
