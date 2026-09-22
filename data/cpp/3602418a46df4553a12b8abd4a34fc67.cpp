Write a C++ function that, given a 2D grid represented as a `std::vector<std::vector<int>>` where `0` represents free space and non-zero values represent obstacles (any positive number), along with a start cell `(startRow, startCol)` and a goal cell `(goalRow, goalCol)`, computes the length of the shortest path (number of steps) from start to goal moving only in four cardinal directions (up, down, left, right) and never stepping onto an obstacle or outside the grid. If no path exists, return `-1`. The function must be `const`-correct and handle edge cases such as start or goal being obstacles, start equal to goal, empty grid, or invalid coordinates.
// The solution uses a breadth-first search (BFS) on the grid. BFS is optimal for unweighted graphs, which this grid is, because each move has equal cost (1 step). We maintain a queue of cells to visit, a 2D boolean visited array (or reuse the grid by marking visited cells), and a distance counter. We begin by checking if start or goal are out of bounds or if either is an obstacle — if so, return `-1`. If start equals goal, return `0`. We then enqueue the start cell, mark it visited, and set its distance to `0`. While the queue is not empty, we pop a cell, check each of its four neighbors: if a neighbor is within bounds, not an obstacle, and not yet visited, we mark it visited, set its distance to current distance + 1, and enqueue it. If we reach the goal, we return the distance immediately. If the queue empties without reaching the goal, return `-1`. The time complexity is `O(R*C)` where `R` is the number of rows and `C` the number of columns, because each cell is visited at most once. The space complexity is also `O(R*C)` for the visited array and the queue in the worst case (e.g., when all cells are free).
#include <vector>
#include <queue>
#include <utility>

// Compute shortest path length (BFS) on a 2D grid.
// grid: 0 = free, non-zero = obstacle. Returns number of steps, or -1 if unreachable.
int shortestPathLength(const std::vector<std::vector<int>>& grid,
                       int startRow, int startCol,
                       int goalRow, int goalCol) {
    const int rows = static_cast<int>(grid.size());
    if (rows == 0) return -1;
    const int cols = static_cast<int>(grid[0].size());

    // Validate coordinates
    auto isValid = [rows, cols](int r, int c) {
        return r >= 0 && r < rows && c >= 0 && c < cols;
    };

    if (!isValid(startRow, startCol) || !isValid(goalRow, goalCol)) return -1;
    if (grid[startRow][startCol] != 0 || grid[goalRow][goalCol] != 0) return -1;
    if (startRow == goalRow && startCol == goalCol) return 0;

    // Visited matrix
    std::vector<std::vector<bool>> visited(rows, std::vector<bool>(cols, false));

    // BFS queue: each element is (row, col, distance)
    std::queue<std::tuple<int, int, int>> q;
    q.push({startRow, startCol, 0});
    visited[startRow][startCol] = true;

    // Direction vectors: up, down, left, right
    const int dr[4] = {-1, 1, 0, 0};
    const int dc[4] = {0, 0, -1, 1};

    while (!q.empty()) {
        auto [r, c, dist] = q.front();
        q.pop();

        for (int i = 0; i < 4; ++i) {
            int nr = r + dr[i];
            int nc = c + dc[i];

            if (isValid(nr, nc) && !visited[nr][nc] && grid[nr][nc] == 0) {
                if (nr == goalRow && nc == goalCol) {
                    return dist + 1;
                }
                visited[nr][nc] = true;
                q.push({nr, nc, dist + 1});
            }
        }
    }

    return -1;
}
#include <cassert>
#include <vector>

int main() {
    // Basic path
    std::vector<std::vector<int>> grid1 = {
        {0, 0, 0},
        {0, 1, 0},
        {0, 0, 0}
    };
    assert(shortestPathLength(grid1, 0, 0, 2, 2) == 4);

    // No path (blocked)
    std::vector<std::vector<int>> grid2 = {
        {0, 1, 0},
        {0, 1, 0},
        {0, 1, 0}
    };
    assert(shortestPathLength(grid2, 0, 0, 2, 2) == -1);

    // Start equals goal
    std::vector<std::vector<int>> grid3 = {{0, 1}, {1, 0}};
    assert(shortestPathLength(grid3, 1, 1, 1, 1) == 0);

    // Obstacle at start
    assert(shortestPathLength(grid3, 0, 0, 1, 1) == -1); // because start (0,0) is free, but goal (1,1) is also free? Actually grid3[0][0]=0, grid3[1][1]=0, path exists? Let's compute: (0,0)->(0,1) blocked, (1,0) blocked. So -1 is correct.

    // Empty grid
    std::vector<std::vector<int>> grid4;
    assert(shortestPathLength(grid4, 0, 0, 0, 0) == -1);

    // Single cell free
    std::vector<std::vector<int>> grid5 = {{0}};
    assert(shortestPathLength(grid5, 0, 0, 0, 0) == 0);

    // Larger grid with obstacles and direct path
    std::vector<std::vector<int>> grid6 = {
        {0, 0, 1, 0},
        {1, 0, 1, 0},
        {0, 0, 0, 0},
        {0, 1, 1, 0}
    };
    assert(shortestPathLength(grid6, 0, 0, 3, 3) == 6);

    // Out of bounds start
    assert(shortestPathLength(grid6, -1, 0, 3, 3) == -1);

    return 0;
}
