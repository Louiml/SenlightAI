Implement a standalone C++ function that computes the distance transform of a 2D binary grid. The grid is represented as a 2D vector of integers where `1` indicates a walkable cell and `0` indicates an obstacle (boundary). The function `computeDistanceTransform` takes a rectangular grid, and for each walkable cell, computes the Chebyshev distance (where diagonal moves count as 1 step, equivalent to using steps of 1 for orthogonal and diagonal neighbors) to the nearest obstacle cell. The result is returned as a 2D vector of the same dimensions, where obstacle cells have distance 0, and walkable cells have a positive distance representing the minimum number of steps (including diagonal moves) to reach any obstacle. The implementation must use a two-pass dynamic programming approach similar to a chamfer distance transform: first pass scans from top-left to bottom-right, second from bottom-right to top-left, updating distances based on already-visited neighbors (orthogonal distance cost 1, diagonal cost 1). The function must handle grids with no obstacles (in that case all walkable cells should get distance equal to the maximum possible in that grid, but since there is no obstacle, define the distance as the Manhattan distance to the nearest border, though in practice for an infinite grid it's undefined—here assume if no obstacles are present, return a grid filled with `INT_MAX`). Also, ensure that all cells are initialized to a large sentinel value (e.g., `INT_MAX`), obstacle cells are set to 0, and the final distances are computed correctly for single-row, single-column, and all-obstacle cases.

The algorithm is a two-pass distance transform for Chebyshev distance (also known as chessboard distance) where orthogonal and diagonal neighbors both have cost 1. In the first pass, iterate cells from top-left to bottom-right; for each cell, consider its already-visited neighbors that are above and to the left: specifically the cells at offsets (-1,0), (0,-1), (-1,-1), and (-1,1) (the latter is diagonally above-right). For each neighbor that exists, if the neighbor has a finite distance value, update the current cell's distance as `min(current, neighbor_distance + 1)`. In the second pass, iterate from bottom-right to top-left and consider the neighbors below and to the right: offsets (1,0), (0,1), (1,1), and (1,-1), updating similarly. The correctness relies on the fact that any shortest path to an obstacle can be decomposed into moves that are either downward-right or upward-left, and the two passes collectively propagate distances correctly for the Chebyshev metric. Edge cases include: (1) a grid with obstacles, where obstacle cells are 0 and interior cells get positive distances; (2) a grid with no obstacles—since no obstacle exists, the distance to any obstacle is undefined; we define the function to return a grid filled with `INT_MAX` in that case; (3) grids with only one row or one column, where only orthogonal neighbors exist; (4) a grid that is entirely obstacles, which results in all zeros. Time complexity is O(rows × cols) because each cell is visited twice, and space complexity is O(rows × cols) for the output grid.

#include <vector>
#include <climits>
#include <algorithm>

// Compute Chebyshev distance transform for a binary grid.
// 'grid' has 1 for walkable cells, 0 for obstacles.
// Returns a grid of same size where each cell holds the minimum number of steps
// (orthogonal or diagonal, cost 1 each) to reach an obstacle.
// If no obstacles exist, returns a grid filled with INT_MAX.
std::vector<std::vector<int>> computeDistanceTransform(const std::vector<std::vector<int>>& grid) {
    if (grid.empty()) return {};
    const int rows = static_cast<int>(grid.size());
    const int cols = static_cast<int>(grid[0].size());

    // Initialize distance grid with sentinel values.
    std::vector<std::vector<int>> dist(rows, std::vector<int>(cols, INT_MAX));

    bool hasObstacle = false;
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            if (grid[r][c] == 0) {
                dist[r][c] = 0;
                hasObstacle = true;
            }
        }
    }

    // If no obstacles, return INT_MAX grid (distance undefined).
    if (!hasObstacle) return dist;

    // Helper lambda to check bounds and update distance.
    auto update = [&](int r, int c, int nr, int nc) {
        if (nr < 0 || nr >= rows || nc < 0 || nc >= cols) return;
        if (dist[nr][nc] == INT_MAX) return;
        if (dist[nr][nc] + 1 < dist[r][c]) {
            dist[r][c] = dist[nr][nc] + 1;
        }
    };

    // First pass: top-left to bottom-right.
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            if (grid[r][c] == 0) continue;
            // Neighbors: up, left, up-left, up-right (relative to current).
            update(r, c, r - 1, c);     // up
            update(r, c, r, c - 1);     // left
            update(r, c, r - 1, c - 1); // up-left
            update(r, c, r - 1, c + 1); // up-right
        }
    }

    // Second pass: bottom-right to top-left.
    for (int r = rows - 1; r >= 0; --r) {
        for (int c = cols - 1; c >= 0; --c) {
            if (grid[r][c] == 0) continue;
            // Neighbors: down, right, down-right, down-left.
            update(r, c, r + 1, c);     // down
            update(r, c, r, c + 1);     // right
            update(r, c, r + 1, c + 1); // down-right
            update(r, c, r + 1, c - 1); // down-left
        }
    }

    return dist;
}

#include <cassert>
#include <vector>

// Function declaration (assumed from solution).
std::vector<std::vector<int>> computeDistanceTransform(const std::vector<std::vector<int>>& grid);

int main() {
    // Test 1: Simple grid with a single obstacle in the center.
    std::vector<std::vector<int>> grid1 = {
        {1, 1, 1},
        {1, 0, 1},
        {1, 1, 1}
    };
    auto d1 = computeDistanceTransform(grid1);
    assert(d1[0][0] == 2);
    assert(d1[0][1] == 1);
    assert(d1[0][2] == 2);
    assert(d1[1][0] == 1);
    assert(d1[1][1] == 0);
    assert(d1[1][2] == 1);
    assert(d1[2][0] == 2);
    assert(d1[2][1] == 1);
    assert(d1[2][2] == 2);

    // Test 2: Obstacle at corner.
    std::vector<std::vector<int>> grid2 = {
        {0, 1, 1},
        {1, 1, 1},
        {1, 1, 1}
    };
    auto d2 = computeDistanceTransform(grid2);
    assert(d2[0][0] == 0);
    assert(d2[0][1] == 1);
    assert(d2[0][2] == 1);
    assert(d2[1][0] == 1);
    assert(d2[1][1] == 1);
    assert(d2[1][2] == 1);
    assert(d2[2][0] == 1);
    assert(d2[2][1] == 1);
    assert(d2[2][2] == 1);

    // Test 3: Entire grid is obstacles.
    std::vector<std::vector<int>> grid3 = {
        {0, 0},
        {0, 0}
    };
    auto d3 = computeDistanceTransform(grid3);
    assert(d3[0][0] == 0 && d3[0][1] == 0 && d3[1][0] == 0 && d3[1][1] == 0);

    // Test 4: No obstacles.
    std::vector<std::vector<int>> grid4 = {
        {1, 1},
        {1, 1}
    };
    auto d4 = computeDistanceTransform(grid4);
    assert(d4[0][0] == INT_MAX && d4[0][1] == INT_MAX && d4[1][0] == INT_MAX && d4[1][1] == INT_MAX);

    // Test 5: Single row with obstacle.
    std::vector<std::vector<int>> grid5 = {
        {1, 1, 0, 1, 1}
    };
    auto d5 = computeDistanceTransform(grid5);
    assert(d5[0][0] == 2 && d5[0][1] == 1 && d5[0][2] == 0 && d5[0][3] == 1 && d5[0][4] == 2);

    // Test 6: Single column.
    std::vector<std::vector<int>> grid6 = {
        {1},
        {0},
        {1}
    };
    auto d6 = computeDistanceTransform(grid6);
    assert(d6[0][0] == 1 && d6[1][0] == 0 && d6[2][0] == 1);

    // Test 7: 4x4 with obstacles on two sides.
    std::vector<std::vector<int>> grid7 = {
        {0, 1, 1, 0},
        {1, 1, 1, 1},
        {1, 1, 1, 1},
        {0, 1, 1, 0}
    };
    auto d7 = computeDistanceTransform(grid7);
    // Center cell (1,1) distance to nearest obstacle: to (0,0) or (3,3) = 2 steps diag.
    assert(d7[1][1] == 2);
    assert(d7[0][0] == 0);
    assert(d7[3][3] == 0);
    assert(d7[2][2] == 1); // adjacent diag to border obstacle.

    // Test 8: Empty grid.
    auto d8 = computeDistanceTransform(std::vector<std::vector<int>>());
    assert(d8.empty());

    return 0;
}
