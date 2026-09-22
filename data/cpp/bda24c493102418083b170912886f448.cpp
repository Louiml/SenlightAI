/*
Given a 2D grid of non-negative integers where each cell value represents the height of terrain, write a C++ function `int trappedWater(const vector<vector<int>>& heights)` that returns the total amount of water that can be trapped between the cells after a heavy rain, assuming the grid is surrounded by infinite walls of height 0. Water can flow in all four cardinal directions (up, down, left, right). The grid is at least 1×1 in size.
*/

#include <vector>
#include <queue>
#include <functional>

// Type alias for cell coordinates
using Position = std::pair<int, int>;

// Compute total trapped water in a 2D height grid.
// Boundary cells cannot trap water; internal cells trap water based on the
// minimum of the maximum heights along escape paths to the boundary.
int trappedWater(const std::vector<std::vector<int>>& heights) {
    if (heights.empty()) return 0;
    const int rows = static_cast<int>(heights.size());
    const int cols = static_cast<int>(heights[0].size());
    if (rows <= 2 || cols <= 2) return 0; // No internal cells

    std::vector<std::vector<bool>> visited(rows, std::vector<bool>(cols, false));
    using HeapNode = std::pair<int, Position>; // {height, (row, col)}
    std::priority_queue<HeapNode, std::vector<HeapNode>, std::greater<HeapNode>> minHeap;

    // Push all boundary cells into heap
    for (int i = 0; i < rows; ++i) {
        minHeap.push({heights[i][0], {i, 0}});
        minHeap.push({heights[i][cols - 1], {i, cols - 1}});
        visited[i][0] = true;
        visited[i][cols - 1] = true;
    }
    for (int j = 0; j < cols; ++j) {
        minHeap.push({heights[0][j], {0, j}});
        minHeap.push({heights[rows - 1][j], {rows - 1, j}});
        visited[0][j] = true;
        visited[rows - 1][j] = true;
    }

    int totalWater = 0;
    const int dr[4] = {-1, 1, 0, 0};
    const int dc[4] = {0, 0, -1, 1};

    while (!minHeap.empty()) {
        auto [currentHeight, pos] = minHeap.top();
        minHeap.pop();
        int r = pos.first;
        int c = pos.second;

        for (int dir = 0; dir < 4; ++dir) {
            int nr = r + dr[dir];
            int nc = c + dc[dir];
            if (nr >= 0 && nr < rows && nc >= 0 && nc < cols && !visited[nr][nc]) {
                visited[nr][nc] = true;
                // If neighbor's height is lower than current boundary height,
                // water gets trapped at the difference.
                if (heights[nr][nc] < currentHeight) {
                    totalWater += currentHeight - heights[nr][nc];
                    // Push neighbor with effective height = currentHeight (water level)
                    minHeap.push({currentHeight, {nr, nc}});
                } else {
                    // Otherwise push neighbor with its own height
                    minHeap.push({heights[nr][nc], {nr, nc}});
                }
            }
        }
    }
    return totalWater;
}

#include <cassert>
#include <vector>

int trappedWater(const std::vector<std::vector<int>>& heights); // Declared from solution

int main() {
    // Single cell
    assert(trappedWater({{5}}) == 0);
    // Single row
    assert(trappedWater({{3, 0, 2}}) == 0);
    // Single column
    assert(trappedWater({{1}, {0}, {2}}) == 0);
    // Empty grid (edge case)
    assert(trappedWater({}) == 0);
    // 3x3 grid with center low
    std::vector<std::vector<int>> grid1 = {
        {3, 3, 3},
        {3, 0, 3},
        {3, 3, 3}
    };
    assert(trappedWater(grid1) == 3);
    // Classic 3x3 example
    std::vector<std::vector<int>> grid2 = {
        {1, 4, 3},
        {4, 2, 4},
        {3, 4, 1}
    };
    assert(trappedWater(grid2) == 2);
    // Larger example with varying heights
    std::vector<std::vector<int>> grid3 = {
        {0, 1, 0, 2, 1},
        {1, 0, 1, 3, 2},
        {2, 3, 2, 1, 0},
        {1, 2, 3, 2, 1}
    };
    assert(trappedWater(grid3) == 3);
    // All flat grid
    std::vector<std::vector<int>> grid4(4, std::vector<int>(5, 2));
    assert(trappedWater(grid4) == 0);
    // V-shape
    std::vector<std::vector<int>> grid5 = {
        {5, 5, 5, 5},
        {5, 1, 1, 5},
        {5, 1, 1, 5},
        {5, 5, 5, 5}
    };
    assert(trappedWater(grid5) == 16);
    // Height differences with edges higher than interior
    std::vector<std::vector<int>> grid6 = {
        {10, 10, 10},
        {10, 0, 10},
        {10, 10, 10}
    };
    assert(trappedWater(grid6) == 10);
    // Step-wise descending interior
    std::vector<std::vector<int>> grid7 = {
        {2, 2, 2, 2},
        {2, 1, 0, 2},
        {2, 0, 1, 2},
        {2, 2, 2, 2}
    };
    assert(trappedWater(grid7) == 6); // (1+2+2+1) = 6
    return 0;
}

// The problem is a 2D version of the classic "trapping rain water" problem. The key insight is that water trapped at a cell is determined by the minimum of the maximum heights along all paths from that cell to the boundary. A boundary cell can never hold water because water can flow out. We use a min-heap (priority queue) initialized with all boundary cells (their heights as keys). We maintain a visited boolean matrix. Pop the cell with the smallest height. For each of its four neighbors, if the neighbor is not visited, we compare the neighbor's height with the current cell's height. If the neighbor's height is less than the current cell's height, the neighbor can trap water equal to `current_height - neighbor_height`; we add that to the answer, and then push the neighbor into the heap with the effective height equal to `current_height` (since the water level around that neighbor is now at least `current_height`). Otherwise, we push the neighbor with its own height. The algorithm works because a min-heap ensures we always process the lowest boundary first, so by the time we reach a cell, we know the minimum of the maximum boundary heights that can reach it. Edge cases: single-row or single-column grids—boundary covers all cells, so answer is 0; grid with only one cell—answer 0. Time complexity is O(N*M log(N*M)) where N and M are grid dimensions, since each cell is pushed and popped at most once. Space complexity is O(N*M) for the visited matrix and heap storage.
