/*
Implement a C++ function `isConnectedFluidRegion` that, given a 2D grid of characters where `'#'` represents an obstacle and any other character (e.g., `'.'`, `' '`, `'A'`) represents an open cell, determines whether all open cells form a single connected component using 4-directional adjacency (up, down, left, right). The grid is provided as a vector of strings, all of equal length, with at least one open cell. The function should return `true` if every open cell is reachable from every other open cell via a path that does not pass through obstacles, and `false` otherwise. The grid dimensions can vary at runtime (no compile-time constants). Handle edge cases such as a grid with only one open cell, grids with multiple isolated open regions, and grids entirely filled with obstacles (which, by definition of having at least one open cell, cannot occur, but your code should handle an empty vector gracefully by returning `false`). This task isolates the graph connectivity problem from the larger fluid simulation in the provided snippet, focusing on a clean, standalone algorithm.
*/

#include <vector>
#include <string>
#include <queue>
#include <utility>

// Determine if all non-'#' cells in the grid form one connected component.
// Cells are adjacent only orthogonally (up, down, left, right).
bool isConnectedFluidRegion(const std::vector<std::string>& grid) {
    if (grid.empty() || grid[0].empty()) return false;

    int rows = static_cast<int>(grid.size());
    int cols = static_cast<int>(grid[0].size());

    // Find first open cell
    int startRow = -1, startCol = -1;
    int openCount = 0;
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            if (grid[r][c] != '#') {
                ++openCount;
                if (startRow == -1) {
                    startRow = r;
                    startCol = c;
                }
            }
        }
    }
    if (openCount == 0) return false; // No open cells

    // BFS from start cell
    std::vector<std::vector<bool>> visited(rows, std::vector<bool>(cols, false));
    std::queue<std::pair<int, int>> q;
    visited[startRow][startCol] = true;
    q.push({startRow, startCol});
    int visitedCount = 0;

    const int dr[4] = {-1, 1, 0, 0};
    const int dc[4] = {0, 0, -1, 1};

    while (!q.empty()) {
        auto [r, c] = q.front();
        q.pop();
        ++visitedCount;
        for (int k = 0; k < 4; ++k) {
            int nr = r + dr[k];
            int nc = c + dc[k];
            if (nr >= 0 && nr < rows && nc >= 0 && nc < cols &&
                grid[nr][nc] != '#' && !visited[nr][nc]) {
                visited[nr][nc] = true;
                q.push({nr, nc});
            }
        }
    }

    // All open cells must be visited
    return visitedCount == openCount;
}

#include <cassert>

int main() {
    // Simple connected grid
    std::vector<std::string> grid1 = {"....", ".##.", "...."};
    assert(isConnectedFluidRegion(grid1) == true);

    // Two separate components
    std::vector<std::string> grid2 = {"...##", "###..", "....."};
    // Actually this is connected? Let's make clearly disconnected:
    std::vector<std::string> grid3 = {"..#..", "..#..", "....."};
    assert(isConnectedFluidRegion(grid3) == true); // Wait, the dots form one component? Left and right separated by #? Actually all dots connect around, so true.

    // Disconnected: two islands
    std::vector<std::string> grid4 = {".#.#", "....", ".#.#"};
    assert(isConnectedFluidRegion(grid4) == false);

    // Single open cell
    std::vector<std::string> grid5 = {"###", "#.#", "###"};
    assert(isConnectedFluidRegion(grid5) == true);

    // Only obstacles? Empty vector case
    std::vector<std::string> grid6;
    assert(isConnectedFluidRegion(grid6) == false);

    // All obstacles (no open cells)
    std::vector<std::string> grid7 = {"###", "###"};
    assert(isConnectedFluidRegion(grid7) == false);

    // Connected region with spaces and letters (all non '#')
    std::vector<std::string> grid8 = {"A   ", "  # ", "   B"};
    assert(isConnectedFluidRegion(grid8) == true);

    // Disconnected with one isolated cell
    std::vector<std::string> grid9 = {"..##", "...#", "...."};
    assert(isConnectedFluidRegion(grid9) == false);

    return 0;
}

// The solution uses a standard flood fill (breadth-first search or depth-first search) starting from any open cell. First, scan the grid to locate the first non-`'#'` cell; if none exists (empty or all obstacles), return `false`. Maintain a visited boolean matrix of the same dimensions. Perform a BFS from the starting cell, exploring all 4 orthogonal neighbors (bounded by grid dimensions) that are not obstacles and not yet visited. Count the number of visited cells during the BFS. After the BFS completes, scan the entire grid again; if any open cell is not visited, it belongs to a separate component, so return `false`. If all open cells are visited, return `true`. Time complexity is \(O(R \times C)\) where \(R\) is the number of rows and \(C\) is the number of columns, since each cell is processed at most once in the scan and at most once in the BFS. Space complexity is \(O(R \times C)\) for the visited matrix and the BFS queue (in the worst case, the queue can hold all cells in a large open region). Edge cases: single open cell → BFS visits it, count equals total open count, returns `true`. Multiple isolated cells → BFS visits only the first component, the later scan finds unvisited open cell, returns `false`. Infinite loops are avoided by marking visited immediately upon enqueueing.
