Write a C++ function named `minimumEffortPath` that accepts a non-empty 2D vector of integers, `heights`, representing a grid of elevation values. Starting at the top-left cell `(0,0)` and ending at the bottom-right cell `(rows-1, cols-1)`, you may move up, down, left, or right to adjacent cells. The “effort” for any path is defined as the maximum absolute height difference between any two consecutive cells along that path. Your function must return the minimum possible effort among all valid paths from start to end. The grid dimensions may be up to 100×100, and height values can be negative, zero, or positive. If the grid has only one cell, the answer is `0`.
#include <cassert>
#include <vector>

int main() {
    // Single cell -> no movement, effort is 0
    assert(minimumEffortPath({{5}}) == 0);

    // 2x2 grid with clear minimal path
    assert(minimumEffortPath({{1, 2}, {2, 3}}) == 1);

    // Negative heights: abs difference handles them
    assert(minimumEffortPath({{-5, -3}, {-4, -2}}) == 1);

    // Larger grid requiring choice of path
    std::vector<std::vector<int>> grid1 = {
        {1, 2, 2},
        {3, 8, 2},
        {5, 3, 5}
    };
    assert(minimumEffortPath(grid1) == 2);

    // Grid where going around a tall spike is better
    std::vector<std::vector<int>> grid2 = {
        {1, 100, 1},
        {1, 100, 1},
        {1, 1, 1}
    };
    assert(minimumEffortPath(grid2) == 0);

    // Straight vertical descent
    std::vector<std::vector<int>> grid3 = {
        {10, 0},
        {5, 0}
    };
    assert(minimumEffortPath(grid3) == 5);

    // All equal heights
    assert(minimumEffortPath({{4, 4}, {4, 4}, {4, 4}}) == 0);

    // 3x3 with monotone diagonal
    std::vector<std::vector<int>> grid4 = {
        {0, 1, 2},
        {3, 4, 5},
        {6, 7, 8}
    };
    assert(minimumEffortPath(grid4) == 1);

    // Wide grid, check path along perimeter
    std::vector<std::vector<int>> grid5 = {
        {1, 3, 5, 7},
        {2, 4, 6, 8},
        {3, 5, 7, 9}
    };
    assert(minimumEffortPath(grid5) == 1);

    // Large height differences but alternative path
    std::vector<std::vector<int>> grid6 = {
        {0, 1000, 0},
        {0, 1000, 0},
        {0, 0, 0}
    };
    assert(minimumEffortPath(grid6) == 0);

    return 0;
}
#include <vector>
#include <queue>
#include <climits>
#include <cmath>
#include <functional>

// Returns the minimum possible maximum absolute height difference along any path
// from the top-left to the bottom-right cell of the grid.
int minimumEffortPath(const std::vector<std::vector<int>>& heights) {
    const int rows = static_cast<int>(heights.size());
    const int cols = static_cast<int>(heights[0].size());

    // dist[r][c] = minimal "max difference" needed to reach (r,c)
    std::vector<std::vector<int>> dist(rows, std::vector<int>(cols, INT_MAX));
    dist[0][0] = 0;

    // Priority queue stores (currentMaxDifference, {row, col})
    using State = std::pair<int, std::pair<int, int>>;
    std::priority_queue<State, std::vector<State>, std::greater<State>> minHeap;
    minHeap.push({0, {0, 0}});

    const int dr[] = {-1, 1, 0, 0};
    const int dc[] = {0, 0, -1, 1};

    while (!minHeap.empty()) {
        auto [currentEffort, pos] = minHeap.top();
        minHeap.pop();
        const int r = pos.first;
        const int c = pos.second;

        // If we've already found a better way to this cell, skip
        if (currentEffort > dist[r][c]) continue;

        for (int i = 0; i < 4; ++i) {
            const int nr = r + dr[i];
            const int nc = c + dc[i];

            if (nr >= 0 && nr < rows && nc >= 0 && nc < cols) {
                const int diff = std::abs(heights[nr][nc] - heights[r][c]);
                const int newEffort = std::max(currentEffort, diff);

                if (newEffort < dist[nr][nc]) {
                    dist[nr][nc] = newEffort;
                    minHeap.push({newEffort, {nr, nc}});
                }
            }
        }
    }

    return dist[rows - 1][cols - 1];
}
// This problem is a classic shortest-path variant where the cost of a path is the maximum edge weight along it, not the sum. The most efficient approach is to use Dijkstra's algorithm with a min-heap, but instead of accumulating distances, we propagate the maximum absolute difference seen so far. We maintain a `dist` matrix initialized to `INT_MAX`, where `dist[r][c]` stores the best (smallest) “maximum difference” needed to reach cell `(r,c)`. Starting from `(0,0)` with distance `0`, we pop the cell with the smallest current distance from a priority queue. For each of its four neighbors, we compute `newCost = max(currentDistance, abs(heights[neighbor] - heights[current]))`. If `newCost` is less than the stored distance for that neighbor, we update and push it into the heap. This correctly finds the minimum maximum edge weight because the priority queue always explores the least-effort frontier first, and any better path to a node would be discovered before a worse one. Edge cases include a single-cell grid (answer `0`), negative heights (absolute difference handles them), and grids where the optimal path may require moving away from the destination temporarily. Time complexity is `O(M*N log(M*N))` due to the heap operations for each cell, and space complexity is `O(M*N)` for the distance matrix and heap storage.
