Write a C++ function `int minimumEnergyPath(const std::vector<std::vector<int>>& grid, int startRow, int startCol, int endRow, int endCol)` that, given a rectangular grid where each cell contains a non-negative integer energy cost (values ≥ 0) except for obstacles marked with `-1`, finds the minimum total "energy expenditure" to travel from a starting cell to a destination cell. You can move only up, down, left, or right, and you cannot enter obstacle cells (`-1`). The energy to move into a cell is the value in that cell; the starting cell's own value is not counted (you start there for free). If no path exists, return `-1`. The grid dimensions are at least 1×1, and the start and end cells are valid (not obstacles). All energy values are non-negative and fit in a 32-bit signed integer, but the accumulated cost may exceed that; use a 64-bit type internally, and return `-1` if the total exceeds `INT_MAX` (though practically, with reasonable constraints, it won't). The function must use Dijkstra's algorithm adapted for this grid-based movement.
#include <cassert>
#include <vector>

// The solution function is declared above (in the same translation unit).
// This main() tests the function.

int main() {
    // Test 1: Simple 3x3 grid, no obstacles, straightforward path.
    std::vector<std::vector<int>> grid1 = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };
    // From (0,0) to (2,2): path (0,0)->(1,0)->(2,0)->(2,1)->(2,2) costs 4+7+8+9 = 28
    // A better path: (0,0)->(0,1)->(0,2)->(1,2)->(2,2) costs 2+3+6+9 = 20
    assert(minimumEnergyPath(grid1, 0, 0, 2, 2) == 20);

    // Test 2: Start equals end.
    assert(minimumEnergyPath(grid1, 1, 1, 1, 1) == 0);

    // Test 3: Obstacle blocks all paths.
    std::vector<std::vector<int>> grid2 = {
        {1, -1, 1},
        {1, -1, 1},
        {1, 1, 1}
    };
    // From (0,0) to (0,2): the middle column is blocked; cannot pass.
    assert(minimumEnergyPath(grid2, 0, 0, 0, 2) == -1);

    // Test 4: Single cell grid, non-negative value.
    std::vector<std::vector<int>> grid3 = {{5}};
    assert(minimumEnergyPath(grid3, 0, 0, 0, 0) == 0);

    // Test 5: Grid with zero-cost cells.
    std::vector<std::vector<int>> grid4 = {
        {0, 0, 0},
        {0, -1, 0},
        {0, 0, 0}
    };
    // From (0,0) to (2,2): go around the obstacle; all cells cost 0.
    assert(minimumEnergyPath(grid4, 0, 0, 2, 2) == 0);

    // Test 6: Large values, but no overflow (use int64).
    std::vector<std::vector<int>> grid5 = {
        {1000000, 1000000},
        {1000000, 1000000}
    };
    // Path (0,0)->(0,1)->(1,1) costs 2000000, fits in int.
    assert(minimumEnergyPath(grid5, 0, 0, 1, 1) == 2000000);

    // Test 7: Unreachable destination due to complete isolation.
    std::vector<std::vector<int>> grid6 = {
        {1, 1, 1},
        {1, -1, 1},
        {1, 1, 1}
    };
    // Destination (1,1) is an obstacle, but start is (0,0) and end is (1,1) invalid.
    // But test end at (0,0) to (2,2) still reachable.
    assert(minimumEnergyPath(grid6, 0, 0, 2, 2) == 6); // Path cost: 1+1+1+1+1+1 = 6

    return 0;
}
#include <vector>
#include <queue>
#include <climits>
#include <cstdint>

// Return the minimum energy cost from (startRow, startCol) to (endRow, endCol),
// or -1 if unreachable. Obstacles are -1. Moving into a cell costs its value.
// The starting cell's value is not counted.
int minimumEnergyPath(const std::vector<std::vector<int>>& grid,
                      int startRow, int startCol, int endRow, int endCol) {
    int rows = static_cast<int>(grid.size());
    int cols = static_cast<int>(grid[0].size());
    
    // Trivial case: start equals end, no movement needed.
    if (startRow == endRow && startCol == endCol) return 0;
    
    // Dijkstra's algorithm with priority queue (min-heap).
    // Distance is stored as int64_t to avoid overflow.
    const int64_t INF = INT64_MAX;
    std::vector<std::vector<int64_t>> dist(rows, std::vector<int64_t>(cols, INF));
    using State = std::pair<int64_t, int>; // (distance, cell index = r*cols + c)
    std::priority_queue<State, std::vector<State>, std::greater<State>> pq;
    
    int startIdx = startRow * cols + startCol;
    dist[startRow][startCol] = 0;
    pq.push({0, startIdx});
    
    // Directions: up, down, left, right.
    const int dr[4] = {-1, 1, 0, 0};
    const int dc[4] = {0, 0, -1, 1};
    
    while (!pq.empty()) {
        auto [curDist, idx] = pq.top();
        pq.pop();
        int r = idx / cols;
        int c = idx % cols;
        
        // If we've already found a better path, skip.
        if (curDist != dist[r][c]) continue;
        
        // If we reached the destination, return the cost.
        if (r == endRow && c == endCol) {
            return static_cast<int>(curDist);
        }
        
        // Explore neighbors.
        for (int d = 0; d < 4; ++d) {
            int nr = r + dr[d];
            int nc = c + dc[d];
            if (nr >= 0 && nr < rows && nc >= 0 && nc < cols && grid[nr][nc] != -1) {
                int64_t newDist = curDist + grid[nr][nc];
                if (newDist < dist[nr][nc]) {
                    dist[nr][nc] = newDist;
                    pq.push({newDist, nr * cols + nc});
                }
            }
        }
    }
    
    return -1; // Destination unreachable.
}
// This problem is a classic single-source shortest path on an unweighted (in terms of step count) but vertex-weighted grid. Since movement cost depends only on the destination cell's value, we can treat each cell as a node with a weight equal to its value, and edges connect adjacent non-obstacle cells. The total cost to reach a cell is the sum of the weights of all cells visited after the start. We run Dijkstra's algorithm using a priority queue (min-heap) to always expand the cell with the smallest current total cost. We initialize the start cell's distance to `0` (since its own value is not counted). For each neighbor that is not an obstacle, we relax: if `currentDistance + neighborValue < neighborDistance`, update. We stop when we pop the destination from the queue. Edge cases: if start equals end, return `0` immediately. If the destination is surrounded by obstacles or unreachable, the priority queue empties and we return `-1`. Also, note that with non-negative weights, Dijkstra is correct; there are no negative cycles. Time complexity is `O(R*C log(R*C))` due to the priority queue, and space complexity is `O(R*C)` for distances and visited/marked cells. We must be careful to use a 64-bit accumulator to avoid overflow when summing many large values.
