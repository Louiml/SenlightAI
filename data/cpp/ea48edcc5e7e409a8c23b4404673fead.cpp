// Write a C++ function named `countIslands` that accepts a 2D vector of booleans (`std::vector<std::vector<bool>>`) representing a grid where `true` denotes land and `false` denotes water. The function must return the number of distinct islands, where an island is defined as a group of adjacent land cells connected horizontally or vertically (not diagonally). The input grid is guaranteed to be non-empty (at least 1 row and 1 column) and rectangular (all rows equal length). The function should not modify the input grid, and it must be implemented using **iterative depth-first search (DFS)** with an explicit stack or **union-find (disjoint-set)**—the provided code snippet shows a recursive DFS and a union-find approach, but you must choose one and implement it cleanly in an iterative or union-find manner (recursive DFS is not allowed). The function must be marked `const`-correct by taking the grid by `const` reference. Edge cases: grids with no land return 0, a single land cell returns 1, and fully land-filled grids return 1.
// The task requires counting connected components in a binary grid. The provided snippet already shows two methods: recursive DFS and union-find. Since recursion is disallowed, we choose **union-find (disjoint-set)** with path compression and union by size (or just simple union). The approach:  
// 1. Map each cell `(r,c)` to a unique index `r * cols + c`.  
// 2. Initialize a parent array where each index points to itself.  
// 3. Iterate over all cells; for each land cell, check its right and down neighbors (to avoid duplicate unions). If a neighbor is also land, union the two indices.  
// 4. After processing all cells, count the number of distinct roots among all land cells: use a `const` method—first find the root of each land cell, mark it in a boolean visited array of size `rows*cols` (or use a set), and increment count when a new root is encountered.  
// Edge cases: empty grid (but per spec not empty), grid with no land (return 0), grid with single cell (return 1).  
// Time complexity: O(rows*cols * α(n)) where α is the inverse Ackermann function (nearly constant) for union-find operations. Space complexity: O(rows*cols) for the parent array and visited array. Unlike recursive DFS, this avoids stack overflow on large grids.
#include <vector>

// Count the number of distinct islands in a binary grid using union-find.
int countIslands(const std::vector<std::vector<bool>>& grid) {
    if (grid.empty() || grid[0].empty()) return 0;
    
    int rows = grid.size();
    int cols = grid[0].size();
    int total = rows * cols;
    
    std::vector<int> parent(total);
    for (int i = 0; i < total; ++i) parent[i] = i;
    
    // Find with path compression
    auto find = [&](int x, auto&& find_ref) -> int {
        if (parent[x] != x) {
            parent[x] = find_ref(parent[x], find_ref);
        }
        return parent[x];
    };
    
    // Union by setting parent of one root to another
    auto unionSets = [&](int x, int y) {
        int rootX = find(x, find);
        int rootY = find(y, find);
        if (rootX != rootY) {
            parent[rootX] = rootY;
        }
    };
    
    // Union right and down neighbors for each land cell
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            if (!grid[r][c]) continue;
            int idx = r * cols + c;
            if (c + 1 < cols && grid[r][c + 1]) {
                unionSets(idx, idx + 1);
            }
            if (r + 1 < rows && grid[r + 1][c]) {
                unionSets(idx, idx + cols);
            }
        }
    }
    
    // Count distinct roots among land cells
    std::vector<bool> seen(total, false);
    int islands = 0;
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            if (grid[r][c]) {
                int root = find(r * cols + c, find);
                if (!seen[root]) {
                    seen[root] = true;
                    ++islands;
                }
            }
        }
    }
    
    return islands;
}
#include <cassert>
#include <vector>

// The solution function is assumed to be declared above or included.
// Test cases for countIslands
int main() {
    // Test 1: Empty grid (but spec says non-empty, still test for safety)
    std::vector<std::vector<bool>> empty = {};
    assert(countIslands(empty) == 0);
    
    // Test 2: Single land cell
    std::vector<std::vector<bool>> single = {{true}};
    assert(countIslands(single) == 1);
    
    // Test 3: No land
    std::vector<std::vector<bool>> noLand = {{false, false}, {false, false}};
    assert(countIslands(noLand) == 0);
    
    // Test 4: All land in 3x3
    std::vector<std::vector<bool>> allLand = {{true, true, true}, {true, true, true}, {true, true, true}};
    assert(countIslands(allLand) == 1);
    
    // Test 5: Two separate islands (diagonal not connected)
    std::vector<std::vector<bool>> twoIslands = {{true, false}, {false, true}};
    assert(countIslands(twoIslands) == 2);
    
    // Test 6: L-shaped island plus isolated cell
    std::vector<std::vector<bool>> shape = {
        {true, false, false},
        {true, true, false},
        {false, false, true}
    };
    assert(countIslands(shape) == 2);
    
    // Test 7: Vertical and horizontal connections
    std::vector<std::vector<bool>> connected = {
        {false, true, false},
        {true, true, true},
        {false, true, false}
    };
    assert(countIslands(connected) == 1);
    
    // Test 8: Row of land and column of land, crossing
    std::vector<std::vector<bool>> cross = {
        {false, true, false},
        {true, true, true},
        {false, true, false}
    };
    assert(countIslands(cross) == 1);
    
    // Test 9: Complex grid with multiple islands (3 islands)
    std::vector<std::vector<bool>> complex = {
        {true, false, true, true},
        {false, false, true, false},
        {true, false, false, true},
        {true, true, false, false}
    };
    // Island 1: (0,0); Island 2: (0,2)-(0,3)-(1,2); Island 3: (2,0)-(3,0)-(3,1) → 3 islands
    assert(countIslands(complex) == 3);
    
    // Test 10: Single row with alternating land/water
    std::vector<std::vector<bool>> singleRow = {{true, false, true, false, true}};
    assert(countIslands(singleRow) == 3);
    
    return 0;
}
