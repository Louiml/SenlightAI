Given an `m x n` binary matrix `isWater` where `1` represents a water cell and `0` represents a land cell, write a C++ function named `highestPeak` that returns an `m x n` integer matrix `height` such that: water cells have height `0`, every land cell has a non-negative height, adjacent cells (sharing an edge) differ in height by at most `1`, and the maximum height in the entire matrix is as large as possible. If multiple valid height assignments exist, any one is acceptable. The input is guaranteed to have at least one water cell, and dimensions satisfy `1 <= m, n <= 1000`. Your function should take the input by `const` reference and return the resulting height matrix by value.
// The problem asks us to assign heights to maximize the peak height while ensuring adjacent cells differ by at most 1 and water cells are at height 0. This is equivalent to computing the shortest distance (in number of steps) from each cell to the nearest water cell. Since height must increase by 1 per step away from water, assigning each land cell the minimum Manhattan distance to a water cell maximizes the minimum height across the grid—and because the constraint is local, this also maximizes the global maximum. The natural approach is a multi-source BFS: initialize a queue with all water cells (height 0), mark them visited, then perform a level-order traversal. Each time we pop a cell, we check its four neighbors; any neighbor that has not been assigned a height gets set to current height + 1 and is pushed onto the queue. This guarantees that every cell gets the shortest distance to the nearest water, and because BFS explores in increasing distance order, the maximum height will be the maximum distance to water. Edge cases: the grid may be a single cell (which must be water), or land cells may exist far from any water, but BFS handles all uniformly. Time complexity is O(m * n) because each cell is enqueued and processed once. Space complexity is O(m * n) for the result matrix and the queue (which in worst case holds all cells).
#include <vector>
#include <queue>

// Given a grid where 1 = water, 0 = land, return height grid maximizing peak height.
std::vector<std::vector<int>> highestPeak(const std::vector<std::vector<int>>& isWater) {
    int m = isWater.size();
    int n = isWater[0].size();
    
    // Initialize height with -1 (unvisited), and queue for BFS.
    std::vector<std::vector<int>> height(m, std::vector<int>(n, -1));
    std::queue<std::pair<int, int>> q;
    
    // Seed BFS with all water cells.
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            if (isWater[i][j] == 1) {
                height[i][j] = 0;
                q.push({i, j});
            }
        }
    }
    
    // Directions: north, south, east, west.
    const std::vector<std::pair<int, int>> dirs = {{-1,0}, {1,0}, {0,-1}, {0,1}};
    
    while (!q.empty()) {
        auto [x, y] = q.front();
        q.pop();
        
        for (const auto& [dx, dy] : dirs) {
            int nx = x + dx;
            int ny = y + dy;
            if (nx >= 0 && nx < m && ny >= 0 && ny < n && height[nx][ny] == -1) {
                height[nx][ny] = height[x][y] + 1;
                q.push({nx, ny});
            }
        }
    }
    
    return height;
}
#include <cassert>
#include <vector>

int main() {
    // Example 1: [[0,1],[0,0]] -> [[1,0],[2,1]] (or any valid)
    std::vector<std::vector<int>> in1 = {{0,1},{0,0}};
    auto out1 = highestPeak(in1);
    assert(out1.size() == 2 && out1[0].size() == 2);
    assert(out1[0][1] == 0 && out1[1][1] == 1);
    assert(out1[0][0] == 1 && out1[1][0] == 2);
    
    // Example 2: [[0,0,1],[1,0,0],[0,0,0]] -> maximum height must be 2
    std::vector<std::vector<int>> in2 = {{0,0,1},{1,0,0},{0,0,0}};
    auto out2 = highestPeak(in2);
    assert(out2.size() == 3 && out2[0].size() == 3);
    int max2 = 0;
    for (auto& row : out2) for (int v : row) { max2 = std::max(max2, v); assert(v >= 0); }
    assert(max2 == 2);
    // Check water cells are 0
    for (int i = 0; i < 3; ++i) for (int j = 0; j < 3; ++j) if (in2[i][j] == 1) assert(out2[i][j] == 0);
    
    // Single water cell
    std::vector<std::vector<int>> in3 = {{1}};
    auto out3 = highestPeak(in3);
    assert(out3.size() == 1 && out3[0].size() == 1 && out3[0][0] == 0);
    
    // All water: 2x2
    std::vector<std::vector<int>> in4 = {{1,1},{1,1}};
    auto out4 = highestPeak(in4);
    for (auto& row : out4) for (int v : row) assert(v == 0);
    
    // 1x3 with water in middle
    std::vector<std::vector<int>> in5 = {{0,1,0}};
    auto out5 = highestPeak(in5);
    assert(out5[0][0] == 1 && out5[0][1] == 0 && out5[0][2] == 1);
    
    // Large case sanity: 3x3 with one corner water, max should be 4? Actually distance from (0,0) to (2,2) is 4 steps, so max height 4.
    std::vector<std::vector<int>> in6 = {{1,0,0},{0,0,0},{0,0,0}};
    auto out6 = highestPeak(in6);
    assert(out6[0][0] == 0);
    assert(out6[2][2] == 4);
    // Check adjacency constraint for all cells
    std::vector<std::pair<int,int>> dirs = {{-1,0},{1,0},{0,-1},{0,1}};
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            for (auto [dx, dy] : dirs) {
                int ni = i + dx, nj = j + dy;
                if (ni >= 0 && ni < 3 && nj >= 0 && nj < 3) {
                    assert(std::abs(out6[i][j] - out6[ni][nj]) <= 1);
                }
            }
        }
    }
    
    return 0;
}
