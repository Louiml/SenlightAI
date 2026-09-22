// Write a C++ function `countConnectedComponents` that takes an integer `n` and a square binary matrix `grid` (n x n, values 0 or 1) as input, and returns a `std::vector<int>` containing the sizes of all connected components of `1`s (using 4-directional adjacency: up, down, left, right). The components should be returned in ascending order of size. If there are no `1`s in the grid, return an empty vector. The function should be `const`-correct: the input grid and size should not be modified. The function must handle edge cases like `n = 0`, a grid full of zeros, a grid full of ones (single component of size n*n), and a grid with multiple isolated ones.
The task is a standard connected-component counting problem on a grid. Use Breadth-First Search (BFS) or Depth-First Search (DFS) to traverse each unvisited cell that contains a `1`. For each such cell, start a traversal that explores all adjacent cells (up, down, left, right) that are within bounds and also contain `1`, marking them as visited. Count the number of cells visited in that traversal, which is the size of that component. After processing all cells, collect all sizes into a vector and sort it in ascending order. The main edge cases: (1) `n = 0` → return empty vector; (2) grid with no `1`s → BFS never triggers, vector remains empty; (3) grid full of ones → a single BFS visits all cells, vector contains one element `n*n`; (4) isolated `1`s → each triggers its own BFS, each component size is 1. Time complexity is O(n^2) because each cell is visited exactly once (enqueued once and processed once). Space complexity is O(n^2) for the visited matrix plus O(n^2) worst-case for the queue (if all cells are connected), but typically O(n) for queue in sparse grids.
#include <vector>
#include <queue>
#include <algorithm>

// Count sizes of all connected components of 1s in a binary grid.
// Returns sizes sorted in ascending order.
std::vector<int> countConnectedComponents(int n, const std::vector<std::vector<int>>& grid) {
    if (n <= 0) return {};  // handle empty grid

    std::vector<std::vector<bool>> visited(n, std::vector<bool>(n, false));
    std::vector<int> componentSizes;

    // Direction vectors for 4-neighbor connectivity
    const int dx[4] = {1, 0, -1, 0};
    const int dy[4] = {0, 1, 0, -1};

    auto inBounds = [n](int x, int y) {
        return x >= 0 && x < n && y >= 0 && y < n;
    };

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (grid[i][j] == 1 && !visited[i][j]) {
                // Start BFS from this unvisited cell
                std::queue<std::pair<int, int>> q;
                q.push({i, j});
                visited[i][j] = true;
                int currentSize = 0;

                while (!q.empty()) {
                    auto [x, y] = q.front();
                    q.pop();
                    ++currentSize;

                    for (int dir = 0; dir < 4; ++dir) {
                        int nx = x + dx[dir];
                        int ny = y + dy[dir];
                        if (inBounds(nx, ny) && !visited[nx][ny] && grid[nx][ny] == 1) {
                            visited[nx][ny] = true;
                            q.push({nx, ny});
                        }
                    }
                }
                componentSizes.push_back(currentSize);
            }
        }
    }

    std::sort(componentSizes.begin(), componentSizes.end());
    return componentSizes;
}
#include <cassert>
#include <vector>

int main() {
    // Test 1: 1x1 grid with single 1
    std::vector<std::vector<int>> g1 = {{1}};
    assert(countConnectedComponents(1, g1) == std::vector<int>({1}));

    // Test 2: 2x2 grid all zeros → empty
    std::vector<std::vector<int>> g2 = {{0,0},{0,0}};
    assert(countConnectedComponents(2, g2).empty());

    // Test 3: 2x2 grid with two isolated ones
    std::vector<std::vector<int>> g3 = {{1,0},{0,1}};
    assert(countConnectedComponents(2, g3) == std::vector<int>({1,1}));

    // Test 4: 2x2 grid with one connected component of size 2
    std::vector<std::vector<int>> g4 = {{1,1},{0,0}};
    assert(countConnectedComponents(2, g4) == std::vector<int>({2}));

    // Test 5: 3x3 grid with components sizes 1, 3, 5
    std::vector<std::vector<int>> g5 = {
        {1,0,1},
        {1,1,0},
        {1,0,1}
    };
    // Components: top-left block of 4 (positions (0,0),(1,0),(1,1),(2,0)) and top-right (0,2) and bottom-right (2,2) are separate?
    // Actually (0,0) connects to (1,0) and (1,1) and (2,0) → size 4. (0,2) size 1. (2,2) size 1. So sizes {1,1,4}? Wait (2,2) is not adjacent to (0,2) — not connected. So {1,1,4}.
    std::vector<int> result5 = countConnectedComponents(3, g5);
    std::vector<int> expected5 = {1,1,4};
    assert(result5 == expected5);

    // Test 6: 4x4 grid full of ones → single component size 16
    std::vector<std::vector<int>> g6(4, std::vector<int>(4, 1));
    assert(countConnectedComponents(4, g6) == std::vector<int>({16}));

    // Test 7: n=0 → empty
    std::vector<std::vector<int>> g7;
    assert(countConnectedComponents(0, g7).empty());

    // Test 8: Check const correctness - grid not modified (compile-time check)
    const std::vector<std::vector<int>> g8 = {{1,0},{0,1}};
    auto result8 = countConnectedComponents(2, g8);
    assert(result8 == std::vector<int>({1,1}));

    // Test 9: Non-square grid? function expects n, assuming n x n. Use n=3 with 3x3 grid
    std::vector<std::vector<int>> g9 = {
        {1,1,1},
        {1,1,1},
        {0,0,0}
    };
    assert(countConnectedComponents(3, g9) == std::vector<int>({6}));

    return 0;
}
