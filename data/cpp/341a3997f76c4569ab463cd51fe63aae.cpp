/*
Given a 2D grid `mat` of non-negative integers where `0` represents an empty cell and `1` represents an obstacle, write a C++ function `vector<vector<int>> distanceToNearestZero(const vector<vector<int>>& mat)` that returns a new grid of the same dimensions where each cell contains the Manhattan distance to the nearest `0` cell. If the grid contains at least one `0`, every cell should be reachable (assume obstacles do not block movement; movement is allowed in four orthogonal directions: up, down, left, right). If the grid contains no `0`, return a grid of all `-1` values. The input grid is guaranteed to be non-empty, rectangular (all rows have the same length), and contain only `0` or `1` values. The function must handle edge cases such as a single-cell grid, grids that are entirely `1`s, and grids where every cell is `0`.
*/
#include <vector>
#include <queue>
#include <utility>

// Return a grid of Manhattan distances to the nearest zero cell.
// If no zero exists, returns a grid of -1 values.
std::vector<std::vector<int>> distanceToNearestZero(const std::vector<std::vector<int>>& mat) {
    int n = mat.size();
    int m = mat[0].size();
    
    // Result grid, initialized to -1 to handle the "no zeros" case.
    std::vector<std::vector<int>> dist(n, std::vector<int>(m, -1));
    std::queue<std::pair<int, int>> q;
    
    // Initialize all zero cells as sources with distance 0.
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (mat[i][j] == 0) {
                dist[i][j] = 0;
                q.push({i, j});
            }
        }
    }
    
    // If there are no zeros, return all -1 (already set).
    if (q.empty()) {
        return dist;
    }
    
    // Direction vectors: up, right, down, left.
    const int dr[4] = {-1, 0, 1, 0};
    const int dc[4] = {0, 1, 0, -1};
    
    while (!q.empty()) {
        int r = q.front().first;
        int c = q.front().second;
        q.pop();
        
        for (int d = 0; d < 4; ++d) {
            int nr = r + dr[d];
            int nc = c + dc[d];
            // Check bounds and if the neighbor is unprocessed (dist == -1).
            if (nr >= 0 && nr < n && nc >= 0 && nc < m && dist[nr][nc] == -1) {
                dist[nr][nc] = dist[r][c] + 1;
                q.push({nr, nc});
            }
        }
    }
    
    return dist;
}
#include <cassert>
#include <vector>

int main() {
    // Basic 2x2 grid.
    std::vector<std::vector<int>> mat1 = {{0, 1}, {1, 1}};
    std::vector<std::vector<int>> res1 = distanceToNearestZero(mat1);
    assert((res1 == std::vector<std::vector<int>>{{0, 1}, {1, 2}}));
    
    // All zeros.
    std::vector<std::vector<int>> mat2 = {{0, 0}, {0, 0}};
    assert((distanceToNearestZero(mat2) == std::vector<std::vector<int>>{{0, 0}, {0, 0}}));
    
    // All ones (no zero).
    std::vector<std::vector<int>> mat3 = {{1, 1}, {1, 1}};
    std::vector<std::vector<int>> res3 = distanceToNearestZero(mat3);
    assert((res3 == std::vector<std::vector<int>>{{-1, -1}, {-1, -1}}));
    
    // Single cell with zero.
    std::vector<std::vector<int>> mat4 = {{0}};
    assert((distanceToNearestZero(mat4) == std::vector<std::vector<int>>{{0}}));
    
    // Single cell with one.
    std::vector<std::vector<int>> mat5 = {{1}};
    assert((distanceToNearestZero(mat5) == std::vector<std::vector<int>>{{-1}}));
    
    // Larger grid with multiple zeros.
    std::vector<std::vector<int>> mat6 = {{0, 1, 1}, {1, 1, 1}, {1, 1, 0}};
    std::vector<std::vector<int>> res6 = distanceToNearestZero(mat6);
    assert((res6 == std::vector<std::vector<int>>{{0, 1, 2}, {1, 2, 1}, {2, 1, 0}}));
    
    // Non-square grid.
    std::vector<std::vector<int>> mat7 = {{0, 1, 1, 1}};
    assert((distanceToNearestZero(mat7) == std::vector<std::vector<int>>{{0, 1, 2, 3}}));
    
    // Grid with a zero in the middle.
    std::vector<std::vector<int>> mat8 = {{1, 1, 1}, {1, 0, 1}, {1, 1, 1}};
    assert((distanceToNearestZero(mat8) == std::vector<std::vector<int>>{{2, 1, 2}, {1, 0, 1}, {2, 1, 2}}));
    
    // 1xN grid with no zero.
    std::vector<std::vector<int>> mat9 = {{1, 1, 1}};
    assert((distanceToNearestZero(mat9) == std::vector<std::vector<int>>{{-1, -1, -1}}));
    
    // Empty input is not allowed by specification, but test a trivial 1x2 with zero.
    std::vector<std::vector<int>> mat10 = {{1, 0}};
    assert((distanceToNearestZero(mat10) == std::vector<std::vector<int>>{{1, 0}}));
    
    return 0;
}
// The problem is a classic multi-source BFS (Breadth-First Search) on an unweighted grid. The key insight is that instead of running BFS from each `1` individually (which would be O(n*m*numberOfOnes) and could be too slow), we run a single BFS starting from **all** `0` cells simultaneously. Initialize a queue with all cells that hold `0`, mark them as visited (distance 0), and then expand outward layer by layer. Each time we pop a cell, its distance is the shortest distance to the nearest `0` because BFS processes nodes in increasing distance order. For each of the four orthogonal neighbors that are not yet visited (and are inside bounds), we assign distance = current distance + 1, mark them visited, and push them into the queue. This works correctly even when the grid contains `1`s, because we treat both `0` and `1` cells as traversable, but only `0` cells are used as starting points. If the grid contains no `0` cells at all, the queue is initially empty, and we must explicitly return a grid filled with `-1` to signal unreachable distance. Edge cases: grid size 1x1 with `0` returns `0`; grid 1x1 with `1` returns `-1`; grid with all zeros returns all zeros. Time complexity is O(n*m) because each cell is enqueued and dequeued at most once, and each neighbor check is O(1). Space complexity is O(n*m) for the queue (in worst case many cells) and the visited/distance grids.
