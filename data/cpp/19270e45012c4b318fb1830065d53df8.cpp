Write a C++ function that, given a rectangular grid of non-negative integers representing a maze where cells with value `0` are walls (impassable) and positive values are open cells with movement costs equal to their value, computes the minimum total cost path from the top-left cell `(0,0)` to the bottom-right cell `(rows-1, cols-1)`. Movement is allowed only in the four cardinal directions (up, down, left, right), and you cannot step outside the grid or into wall cells. The function should return the minimum path cost, or `-1` if no path exists. Assume the start and end cells are always open (cost > 0). The function signature is: `int minPathCost(const std::vector<std::vector<int>>& grid)`.
#include <cassert>
#include <vector>

int main() {
    // Simple 2x2 all open
    std::vector<std::vector<int>> g1 = {{1, 1}, {1, 1}};
    assert(minPathCost(g1) == 3); // 1 + 1 + 1

    // 3x3 with a wall blocking direct path, must detour
    std::vector<std::vector<int>> g2 = {
        {1, 1, 1},
        {0, 0, 1},
        {1, 1, 1}
    };
    assert(minPathCost(g2) == 5); // 1+1 (top row) + 1+1+1 (right column) = 5

    // No path
    std::vector<std::vector<int>> g3 = {
        {1, 0},
        {0, 1}
    };
    assert(minPathCost(g3) == -1);

    // Single cell
    std::vector<std::vector<int>> g4 = {{5}};
    assert(minPathCost(g4) == 5);

    // Rectangular with varying costs, start and end open
    std::vector<std::vector<int>> g5 = {
        {2, 0, 0},
        {3, 1, 4},
        {0, 0, 1}
    };
    // Path: (0,0)->(1,0) cost 3, ->(1,1) cost 1, ->(1,2) cost 4, ->(2,2) cost 1 = 2+3+1+4+1=11
    assert(minPathCost(g5) == 11);

    // Row with single column but open
    std::vector<std::vector<int>> g6 = {{1}, {2}, {3}};
    assert(minPathCost(g6) == 6); // 1+2+3

    // Grid where cheaper path avoids higher costs
    std::vector<std::vector<int>> g7 = {
        {1, 100, 1},
        {1, 100, 1},
        {1, 1, 1}
    };
    // Path along top? blocked? Actually top row: 1+100+1 then down? But bottom row cheaper: (0,0)->(1,0)->(2,0)->(2,1)->(2,2) = 1+1+1+1+1=5
    assert(minPathCost(g7) == 5);

    return 0;
}
#include <vector>
#include <queue>
#include <limits>

// Compute minimum path cost in a grid where 0 is a wall and positive values are costs.
int minPathCost(const std::vector<std::vector<int>>& grid) {
    int rows = static_cast<int>(grid.size());
    if (rows == 0) return -1;
    int cols = static_cast<int>(grid[0].size());
    if (cols == 0) return -1;

    const int INF = std::numeric_limits<int>::max();
    std::vector<std::vector<int>> dist(rows, std::vector<int>(cols, INF));

    // Start cell is always open by assumption
    dist[0][0] = grid[0][0];
    // Min-heap: <cost, row, col>
    using Node = std::tuple<int, int, int>;
    std::priority_queue<Node, std::vector<Node>, std::greater<Node>> pq;
    pq.emplace(grid[0][0], 0, 0);

    const int dr[4] = {-1, 1, 0, 0};
    const int dc[4] = {0, 0, -1, 1};

    while (!pq.empty()) {
        auto [cost, r, c] = pq.top();
        pq.pop();

        if (cost != dist[r][c]) continue; // stale entry

        if (r == rows - 1 && c == cols - 1) return cost;

        for (int k = 0; k < 4; ++k) {
            int nr = r + dr[k];
            int nc = c + dc[k];
            if (nr >= 0 && nr < rows && nc >= 0 && nc < cols 
                && grid[nr][nc] > 0) {
                int newCost = cost + grid[nr][nc];
                if (newCost < dist[nr][nc]) {
                    dist[nr][nc] = newCost;
                    pq.emplace(newCost, nr, nc);
                }
            }
        }
    }
    return -1;
}
// The problem is a classic weighted grid shortest-path problem. Since edge costs are positive (each move to an open cell costs that cell's value), Dijkstra's algorithm (or a priority-queue-based BFS) is appropriate. We maintain a 2D distance array initialized to infinity, set the start distance as its cell value, and push `(startCost, startRow, startCol)` into a min-heap. While the heap is non-empty, we pop the cell with the smallest accumulated cost. If this is the target, we return its cost. Otherwise, for each of the four neighbors within bounds and with a positive value, we compute the new cost and update if it’s better than the stored distance, then push the neighbor back. Since we never revisit a node with a worse cost, the first time we pop the target gives the optimal answer. Edge cases: grid with one row/column, walls blocking the path (returns -1), and large grids (use long long for distances to avoid overflow, but the problem limits are unspecified; int is usually enough if cell values are small). Time complexity is O(R*C log(R*C)) and space O(R*C) for the distance matrix and the heap.
