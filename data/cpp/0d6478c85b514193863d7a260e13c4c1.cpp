// Write a C++ function that takes an integer `N` (where 2 ≤ N ≤ 25) and a vector of strings representing an `N × N` grid of characters `'0'` and `'1'`. Adjacent `'1'` cells (sharing an edge, not a corner) form a connected "danzi" (cluster). The function must return a vector of integers containing the sizes of all distinct danzi clusters in the grid, sorted in ascending order. The input guarantees at least one `'1'` cell exists. The function should be named `findDanziSizes` and accept the grid size and the grid as parameters, returning `std::vector<int>`.

// The problem is a classic connected-component counting and sizing problem on a 2D grid. The main algorithm uses Depth-First Search (DFS) or Breadth-First Search (BFS) to traverse each unvisited `'1'` cell. For each unvisited `'1'`, increment a component counter, start a DFS from that cell, and count all reachable `'1'` cells via the four orthogonal directions (up, down, left, right). Mark visited cells to avoid double counting. After processing all cells, sort the collected sizes in ascending order.
//
// Edge cases:  
// - The grid may contain only one cluster (entirely filled with `'1'`s) or multiple clusters.  
// - Clusters are not connected diagonally; only orthogonal adjacency matters.  
// - The grid is square, but the bounds check must ensure indices stay within `[0, N-1]`.  
// - If a cell is `'0'`, it is ignored and never visited.  
//
// Time complexity: Each cell is visited at most once (when it is `'1'` and not yet visited), so the DFS traversal is O(N²). Sorting the list of sizes takes O(K log K) where K is the number of clusters, but K ≤ N², so overall time is O(N² log N) in the worst case. Space complexity: O(N²) for the visited array and recursion stack in the worst case (if the entire grid is a single cluster).

#include <vector>
#include <string>
#include <algorithm>

// Compute sizes of all connected clusters of '1's in an N x N grid.
// Clusters are connected orthogonally (up, down, left, right).
// Returns a sorted vector of cluster sizes.
std::vector<int> findDanziSizes(int N, const std::vector<std::string>& grid) {
    // Visited matrix: false initially, true once a cell has been explored.
    std::vector<std::vector<bool>> visited(N, std::vector<bool>(N, false));
    
    // Direction arrays for four orthogonal moves: up, down, left, right.
    const int dx[4] = {-1, 1, 0, 0};
    const int dy[4] = {0, 0, -1, 1};
    
    // Helper lambda for DFS; uses recursion and captures by reference.
    std::function<int(int, int)> dfs = [&](int x, int y) -> int {
        // Mark current cell as visited and start cluster size at 1.
        visited[x][y] = true;
        int cluster_size = 1;
        
        // Explore all four directions.
        for (int i = 0; i < 4; ++i) {
            int nx = x + dx[i];
            int ny = y + dy[i];
            
            // Check bounds and whether the neighbor is part of the same cluster.
            if (nx >= 0 && nx < N && ny >= 0 && ny < N) {
                if (!visited[nx][ny] && grid[nx][ny] == '1') {
                    cluster_size += dfs(nx, ny);
                }
            }
        }
        return cluster_size;
    };
    
    std::vector<int> sizes;
    
    // Iterate over every cell in the grid.
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            // If the cell is '1' and not yet visited, start a new cluster.
            if (!visited[i][j] && grid[i][j] == '1') {
                sizes.push_back(dfs(i, j));
            }
        }
    }
    
    // Sort sizes in ascending order as required.
    std::sort(sizes.begin(), sizes.end());
    return sizes;
}

#include <cassert>
#include <vector>
#include <string>

// Include the solution function here (or link appropriately)
// For self-contained test, copy the solution code above.

int main() {
    // Test 1: Single isolated cell
    std::vector<std::string> grid1 = {"1"};
    assert(findDanziSizes(1, grid1) == std::vector<int>{1});
    
    // Test 2: Two separate clusters: top-left 2x2 and bottom-right 1x1
    std::vector<std::string> grid2 = {
        "1100",
        "1100",
        "0000",
        "0001"
    };
    assert(findDanziSizes(4, grid2) == std::vector<int>({1, 4}));
    
    // Test 3: Single large cluster spanning entire grid
    std::vector<std::string> grid3 = {
        "111",
        "111",
        "111"
    };
    assert(findDanziSizes(3, grid3) == std::vector<int>({9}));
    
    // Test 4: Diagonal cells not connected
    std::vector<std::string> grid4 = {
        "10",
        "01"
    };
    assert(findDanziSizes(2, grid4) == std::vector<int>({1, 1}));
    
    // Test 5: Mixed clusters, sizes need sorting
    std::vector<std::string> grid5 = {
        "101",
        "010",
        "101"
    };
    // Clusters: each '1' is isolated (no orthogonal neighbor), so sizes all 1
    assert(findDanziSizes(3, grid5) == std::vector<int>({1, 1, 1, 1, 1}));
    
    // Test 6: Larger grid with three clusters: sizes 3, 2, 1 -> sorted 1,2,3
    std::vector<std::string> grid6 = {
        "11000",
        "11000",
        "00100",
        "00010",
        "00010"
    };
    assert(findDanziSizes(5, grid6) == std::vector<int>({1, 2, 4}));
    
    // Test 7: Row of connected cells
    std::vector<std::string> grid7 = {
        "1111"
    };
    assert(findDanziSizes(1, grid7) == std::vector<int>({4}));
    
    // Test 8: Two clusters in a 2x3 grid
    std::vector<std::string> grid8 = {
        "101",
        "101"
    };
    // Left column top and bottom connected, right column top and bottom connected, but columns separated
    assert(findDanziSizes(2, grid8) == std::vector<int>({2, 2}));
    
    // Test 9: Empty grid with no '1's? Not allowed by spec, but test if provided would return empty
    std::vector<std::string> grid9 = {
        "000",
        "000",
        "000"
    };
    assert(findDanziSizes(3, grid9) == std::vector<int>());
    
    // Test 10: Complex shape with a hole
    std::vector<std::string> grid10 = {
        "11111",
        "10001",
        "10101",
        "10001",
        "11111"
    };
    // Outer ring is one cluster (16 cells), center is '1' but isolated? Let's check: (2,2) is '1' and has no orthogonal '1' neighbor? Actually (2,2) has (1,2)='0', (3,2)='0', (2,1)='0', (2,3)='0' -> isolated. So sizes: 16 and 1
    assert(findDanziSizes(5, grid10) == std::vector<int>({1, 16}));
}
