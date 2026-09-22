Write a C++ function `connectedComponentSizes` that takes a rectangular grid of characters where `'O'` represents water and any other character (e.g., `'X'` or `'.'`) represents land. The function should return a vector of integers containing the sizes of all distinct water-connected components (4-directionally adjacent, not diagonal) in the grid. Each component's size is the total number of cells in that connected region. If a water cell is part of a component, its size contributes exactly once. Return the sizes in any order, but every water cell must be assigned to exactly one component. The grid is given as a vector of strings, all of the same length, with at least one row and one column. The function must not modify the input grid.
// The task requires identifying connected components of water cells using 4-directional adjacency (up, down, left, right). A simple approach is to perform a flood-fill (DFS/BFS) on each unvisited water cell. Maintain a visited grid (e.g., a vector of vector bool or an integer grid where 0 means unvisited, positive integers indicate component ID). For each unvisited `'O'`, start a DFS/BFS, count the number of cells visited during that traversal, and push the count into a result vector. Edge cases: an empty grid (but the problem states at least one cell), a grid with no water cells (result is empty vector), single-cell components, and large grids requiring iterative recursion to avoid stack overflow (though with small test sizes recursion is fine; the solution below uses recursion but with board dimensions modest). Time complexity: Each cell is visited exactly once, so O(R*C) where R is rows and C is columns. Space complexity: O(R*C) for the visited grid, plus recursion stack in worst case O(R*C) for a fully water grid.
#include <vector>
#include <string>

// Depth-first search to mark all water cells in a component and return its size.
int dfs(int r, int c, const std::vector<std::string>& grid,
        std::vector<std::vector<bool>>& visited) {
    int rows = static_cast<int>(grid.size());
    int cols = static_cast<int>(grid[0].size());
    if (r < 0 || r >= rows || c < 0 || c >= cols ||
        visited[r][c] || grid[r][c] != 'O') {
        return 0;
    }
    visited[r][c] = true;
    int size = 1;
    // Explore four directions.
    size += dfs(r - 1, c, grid, visited);
    size += dfs(r + 1, c, grid, visited);
    size += dfs(r, c - 1, grid, visited);
    size += dfs(r, c + 1, grid, visited);
    return size;
}

// Returns sizes of all water-connected components in the grid.
std::vector<int> connectedComponentSizes(const std::vector<std::string>& grid) {
    if (grid.empty() || grid[0].empty()) return {};
    int rows = static_cast<int>(grid.size());
    int cols = static_cast<int>(grid[0].size());
    std::vector<std::vector<bool>> visited(rows, std::vector<bool>(cols, false));
    std::vector<int> sizes;
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            if (grid[r][c] == 'O' && !visited[r][c]) {
                sizes.push_back(dfs(r, c, grid, visited));
            }
        }
    }
    return sizes;
}
#include <cassert>
#include <algorithm>

// Helper to check if two vectors contain the same multiset of elements.
bool sameMultiset(std::vector<int> a, std::vector<int> b) {
    std::sort(a.begin(), a.end());
    std::sort(b.begin(), b.end());
    return a == b;
}

int main() {
    // Single water cell.
    std::vector<std::string> grid1 = {"O"};
    assert(sameMultiset(connectedComponentSizes(grid1), {1}));

    // All land, no components.
    std::vector<std::string> grid2 = {"X", "X"};
    assert(connectedComponentSizes(grid2).empty());

    // Two separate water components.
    std::vector<std::string> grid3 = {"OXO", "XXX"};
    assert(sameMultiset(connectedComponentSizes(grid3), {1, 1}));

    // One large component with multiple cells.
    std::vector<std::string> grid4 = {"OXX", "OOX", "XOO"};
    assert(sameMultiset(connectedComponentSizes(grid4), {4}));  // positions (0,0),(1,0),(1,1),(2,1)

    // Component not wrapping diagonally.
    std::vector<std::string> grid5 = {"OO", "XO"};
    // Cells: (0,0),(0,1) connected; (1,1) separate? Actually (1,1) adjacent to (0,1) vertically -> same component.
    // So all three are one component.
    assert(sameMultiset(connectedComponentSizes(grid5), {3}));

    // Mixed sizes.
    std::vector<std::string> grid6 = {"OOX", "XOX", "XXO"};
    // Component1: (0,0),(0,1) size2; Component2: (1,2) size1; Component3: (2,2) size1.
    assert(sameMultiset(connectedComponentSizes(grid6), {2, 1, 1}));

    // Rectangular non-square.
    std::vector<std::string> grid7 = {"OXXO", "OXXO"};
    // Left component (0,0),(1,0) size2; right component (0,3),(1,3) size2.
    assert(sameMultiset(connectedComponentSizes(grid7), {2, 2}));

    return 0;
}
