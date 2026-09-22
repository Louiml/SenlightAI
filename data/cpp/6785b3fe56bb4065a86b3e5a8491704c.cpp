Write a C++ function that simulates a robot vacuum cleaner moving on an \(N \times M\) grid. The grid is represented as a 2D vector of integers where `0` means an unvisited cleanable cell, `1` means a wall, and `2` means a visited cell. The robot starts at a given cell `(r, c)` with an initial direction `d` (0=up, 1=right, 2=down, 3=left). The robot operates by repeatedly: cleaning its current cell if unvisited, then checking its left, forward, right, and backward directions (in that order) for an unvisited cleanable cell; if found, it moves there and repeats. If no unvisited cell exists in any of those four directions, the robot moves backward one cell (keeping its current direction) and repeats; if it cannot move backward because the cell behind is a wall, it stops. The function must return the total number of cells cleaned. The grid dimensions satisfy \(1 \le N, M \le 50\), and the input grid is guaranteed to contain at least one `0` cell. The function must be self-contained and not use global variables.
#include <cassert>
#include <vector>
#include <functional>

// Include the solution function here or use a header.
// For brevity, assume the robotCleaner function is defined above.

int main() {
    // Test 1: Simple open area
    std::vector<std::vector<int>> grid1 = {{0, 0}, {0, 0}};
    assert(robotCleaner(grid1, 0, 0, 0) == 4);

    // Test 2: Wall surrounds, only one cell
    std::vector<std::vector<int>> grid2 = {{1, 1, 1}, {1, 0, 1}, {1, 1, 1}};
    assert(robotCleaner(grid2, 1, 1, 1) == 1);

    // Test 3: Corridor with backtracking
    std::vector<std::vector<int>> grid3 = {
        {1, 1, 1, 1},
        {0, 0, 0, 1},
        {1, 1, 0, 1},
        {1, 1, 1, 1}
    };
    assert(robotCleaner(grid3, 1, 0, 1) == 4);

    // Test 4: L-shaped path
    std::vector<std::vector<int>> grid4 = {
        {0, 0, 1},
        {0, 0, 0},
        {1, 0, 1}
    };
    assert(robotCleaner(grid4, 0, 0, 1) == 5);

    // Test 5: Starting on visited (already cleaned)
    std::vector<std::vector<int>> grid5 = {{2, 0}, {0, 0}};
    assert(robotCleaner(grid5, 0, 0, 0) == 3);

    // Test 6: Starting on wall -> returns 0
    std::vector<std::vector<int>> grid6 = {{1, 0}, {0, 0}};
    assert(robotCleaner(grid6, 0, 0, 0) == 0);

    // Test 7: Single row
    std::vector<std::vector<int>> grid7 = {{0, 0, 1, 0, 0}};
    assert(robotCleaner(grid7, 0, 0, 1) == 4);

    // Test 8: Direction effect on order but same count
    std::vector<std::vector<int>> g1 = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}};
    std::vector<std::vector<int>> g2 = g1;
    assert(robotCleaner(g1, 1, 1, 0) == 9);
    assert(robotCleaner(g2, 1, 1, 2) == 9);

    // Test 9: Dead end forces backward
    std::vector<std::vector<int>> grid9 = {
        {1, 1, 1, 1},
        {0, 0, 0, 1},
        {1, 1, 0, 1},
        {1, 1, 1, 1}
    };
    // Starting at (1,0) going right, cleans 4 cells and then stops (no backward possible).
    assert(robotCleaner(grid9, 1, 0, 1) == 4);

    // Test 10: All walls except one
    std::vector<std::vector<int>> grid10 = {{1, 1}, {1, 0}};
    assert(robotCleaner(grid10, 1, 1, 3) == 1);

    return 0;
}
#include <vector>

// Simulate a robot vacuum cleaner on a grid.
// grid: 0=unvisited cleanable, 1=wall, 2=visited.
// start_r, start_c: starting position.
// start_dir: initial direction (0=up, 1=right, 2=down, 3=left).
// Returns the number of cells cleaned.
int robotCleaner(std::vector<std::vector<int>>& grid, int start_r, int start_c, int start_dir) {
    const int N = static_cast<int>(grid.size());
    const int M = (N > 0) ? static_cast<int>(grid[0].size()) : 0;

    // Direction vectors: up, right, down, left
    const int dx[4] = {-1, 0, 1, 0};
    const int dy[4] = {0, 1, 0, -1};

    // Recursive lambda to perform DFS cleaning.
    std::function<int(int, int, int)> dfs = [&](int x, int y, int d) -> int {
        int cleaned = 0;
        if (grid[x][y] == 0) {
            grid[x][y] = 2;
            cleaned = 1;
        }

        // Check left, forward, right, backward in that order.
        bool moved = false;
        for (int i = 0; i < 4 && !moved; ++i) {
            int nd = (d + 3 - i) % 4; // i=0: left, i=1: forward, i=2: right, i=3: backward
            int nx = x + dx[nd];
            int ny = y + dy[nd];
            if (nx >= 0 && nx < N && ny >= 0 && ny < M && grid[nx][ny] == 0) {
                cleaned += dfs(nx, ny, nd);
                moved = true;
            }
        }

        // If no unvisited neighbor, try to move backward.
        if (!moved) {
            int back_d = (d + 2) % 4;
            int bx = x + dx[back_d];
            int by = y + dy[back_d];
            if (bx < 0 || bx >= N || by < 0 || by >= M || grid[bx][by] == 1) {
                // Cannot move backward: stop and return.
                return cleaned;
            }
            // Move backward (keep direction) and continue cleaning.
            cleaned += dfs(bx, by, d);
        }

        return cleaned;
    };

    // Handle invalid start position (wall or out of bounds).
    if (start_r < 0 || start_r >= N || start_c < 0 || start_c >= M || grid[start_r][start_c] == 1) {
        return 0;
    }
    return dfs(start_r, start_c, start_dir);
}
// The robot always cleans the current cell if it's unvisited (`0` becomes `2`), incrementing the count. Then it checks the four directions in the order: left of current direction, forward, right, backward. For each direction (computed by rotating the current direction by \(3-i\) for \(i=0..3\), which corresponds to left, forward, right, backward), it checks if the adjacent cell is within bounds and unvisited (`0`); if so, it moves there and recurses, immediately returning after that move—no need to check further because the recursive call will handle the rest. If none of the four directions are unvisited, it attempts to move backward (one step in the opposite direction of current direction). If the backward cell is a wall (`1`), the robot stops, and the count is returned. Otherwise, it moves backward without changing direction and continues the same loop (which is equivalent to a recursive call to the backward cell with the same direction). This is essentially a depth-first search where the order of exploration is fixed: left, forward, right, backward, and the robot only backtracks when no unvisited neighbor exists. Edge cases include: robot starting on a wall (though guaranteed at least one `0`, the start might be a wall? The problem statement implies the start is on a cleanable cell because the original code assumes `arr[r][c]` is not a wall, but we should handle it gracefully—if the start is a wall, return 0). Also, when moving backward, the direction remains the same, so the robot may clean new cells in that path. The time complexity is \(O(N \cdot M)\) because each cell is visited at most once for cleaning and at most once for backtracking (each cell can become a dead-end and be visited in back-and-forth moves). Space complexity is \(O(N \cdot M)\) due to recursion stack in the worst case (a long corridor). The solution uses a helper function that takes the grid by reference, modifies it, and returns the count.
