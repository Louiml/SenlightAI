Write a C++ function named `countMazeComponents` that, given a 2D grid represented as a `std::vector<std::vector<bool>>` where `true` represents a wall and `false` represents an open cell, returns the number of connected components of open cells using a union-find data structure. Cells are considered connected if they share an edge (up, down, left, right), not diagonally. The grid is non-empty, rectangular (all rows have the same length), and may contain no open cells (in which case return 0). The function must operate on a `const` reference to the grid and should not modify it.

The core idea is to model each open cell as a node in an undirected graph and find connected components via union-find. First, traverse the entire grid to count total open cells and assign each a unique ID (for simplicity, flatten the 2D indices into a single integer using `row*cols + col`). Initialize a union-find structure with one element per cell (or only for open cells to save space), then for each open cell, check its right and down neighbors (to avoid double-checking) and union their IDs if the neighbor is also open. After processing all edges, count the number of distinct roots among open cells by iterating over all cells and using a set or by checking if a cell is its own root and it is open. Edge cases: empty rows or columns, all walls, single open cell, and disconnected grids. Time complexity is O(rows*cols*α(n)) where α is the inverse Ackermann function (nearly constant), and space complexity is O(rows*cols) for the union-find parent and rank arrays.

#include <vector>
#include <numeric>
#include <cassert>

class UnionFind {
private:
    std::vector<int> parent;
    std::vector<int> rank;

public:
    UnionFind(int n) : parent(n), rank(n, 0) {
        std::iota(parent.begin(), parent.end(), 0);
    }

    int find(int x) {
        if (parent[x] != x) {
            parent[x] = find(parent[x]); // path compression
        }
        return parent[x];
    }

    void unite(int x, int y) {
        int rx = find(x);
        int ry = find(y);
        if (rx == ry) return;
        if (rank[rx] < rank[ry]) {
            parent[rx] = ry;
        } else if (rank[rx] > rank[ry]) {
            parent[ry] = rx;
        } else {
            parent[ry] = rx;
            rank[rx]++;
        }
    }
};

// Count connected components of open (false) cells in a binary grid.
int countMazeComponents(const std::vector<std::vector<bool>>& grid) {
    if (grid.empty() || grid[0].empty()) return 0;
    const int rows = static_cast<int>(grid.size());
    const int cols = static_cast<int>(grid[0].size());
    const int total = rows * cols;

    UnionFind uf(total);

    // Union adjacent open cells.
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            if (grid[r][c]) continue; // wall
            int id = r * cols + c;

            // Check right neighbor
            if (c + 1 < cols && !grid[r][c + 1]) {
                uf.unite(id, id + 1);
            }

            // Check down neighbor
            if (r + 1 < rows && !grid[r + 1][c]) {
                uf.unite(id, id + cols);
            }
        }
    }

    // Count distinct roots of open cells.
    int components = 0;
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            if (grid[r][c]) continue;
            int id = r * cols + c;
            if (uf.find(id) == id) {
                components++;
            }
        }
    }
    return components;
}

#include <cassert>
#include <vector>

int main() {
    // Test 1: Empty grid
    std::vector<std::vector<bool>> grid1 = {};
    assert(countMazeComponents(grid1) == 0);

    // Test 2: All walls
    std::vector<std::vector<bool>> grid2 = {{true, true}, {true, true}};
    assert(countMazeComponents(grid2) == 0);

    // Test 3: Single open cell
    std::vector<std::vector<bool>> grid3 = {{false}};
    assert(countMazeComponents(grid3) == 1);

    // Test 4: Simple connected open area
    std::vector<std::vector<bool>> grid4 = {{false, false}, {false, false}};
    assert(countMazeComponents(grid4) == 1);

    // Test 5: Disconnected components
    std::vector<std::vector<bool>> grid5 = {{false, true, false},
                                             {true, true, true},
                                             {false, true, false}};
    assert(countMazeComponents(grid5) == 4);

    // Test 6: Horizontal and vertical connections
    std::vector<std::vector<bool>> grid6 = {{false, false, true},
                                             {true, false, false},
                                             {true, true, true}};
    // Open cells: (0,0),(0,1),(1,1),(1,2) -> all connected via (0,1)-(1,1)
    assert(countMazeComponents(grid6) == 1);

    // Test 7: Diagonals are not connected
    std::vector<std::vector<bool>> grid7 = {{false, true, false},
                                             {true, true, true},
                                             {false, true, false}};
    assert(countMazeComponents(grid7) == 4);

    // Test 8: Larger grid with multiple components
    std::vector<std::vector<bool>> grid8 = {{false, true, false, false},
                                             {false, true, false, true},
                                             {true, true, false, true},
                                             {false, false, false, true}};
    // Components: 
    // - (0,0) and (1,0) connected
    // - (0,2),(0,3),(1,2) connected
    // - (3,0),(3,1),(3,2) connected
    // - (1,3),(2,3),(3,3) connected? Check: (1,3)-(2,3)-(3,3) yes
    // Actually (1,3) and (2,3) are open, (3,3) open, but (1,3) not connected to (0,3) because (1,2) is open but (1,2) not adjacent to (1,3) due to (1,2) has wall at (1,3)? No, (1,2) and (1,3) are adjacent and both open? Wait (1,3) is false, yes. So (1,2) connects to (1,3). So (0,2),(0,3),(1,2),(1,3),(2,3),(3,3) are all one component. That's one. (0,0),(1,0) another. (3,0),(3,1),(3,2) third. So total 3.
    assert(countMazeComponents(grid8) == 3);

    return 0;
}
