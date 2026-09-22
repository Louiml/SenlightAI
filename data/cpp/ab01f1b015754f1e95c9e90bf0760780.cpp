Write a C++ function that takes as input a rectangular grid of cells with dimensions `rows` and `cols` (both positive integers), and a list of axis-aligned rectangles (each defined by two opposite corners in column-major coordinates: `x1, y1, x2, y2`, where `0 <= x1 < x2 <= cols` and `0 <= y1 < y2 <= rows`). The rectangles are filled (marked as occupied), and all other cells are empty. The function should return a `std::vector<int>` containing the areas (number of cells) of every connected component of empty cells, where connectivity is defined by 4-directional adjacency (up, down, left, right). The returned vector should be sorted in ascending order. The grid coordinates follow the convention: `(x, y)` where `x` is the column index (0 to `cols-1`) and `y` is the row index (0 to `rows-1`). The rectangles are given with `(x1, y1)` being the top-left corner and `(x2, y2)` being the bottom-right corner in that coordinate system, and they cover all cells with `x1 <= col < x2` and `y1 <= row < y2`. The function signature is `std::vector<int> emptyRegions(int rows, int cols, const std::vector<std::array<int,4>>& rectangles)`. If there are no empty cells, return an empty vector. The function should handle grids up to 100x100 and any number of rectangles.
// The problem is a classic connected-components counting on a grid after marking rectangular blocks as obstacles. First, create a 2D boolean grid of size `rows` by `cols`, initialized to `false` (empty). For each rectangle, set all cells in the range `[x1, x2-1]` and `[y1, y2-1]` to `true` (occupied). Then iterate over all cells; whenever we find an empty cell that hasn't been visited, perform a Breadth-First Search (BFS) or Depth-First Search (DFS) starting from that cell, marking all reachable empty cells as visited and counting them. Store the count in a result vector. After all components are found, sort the vector in ascending order and return it. Edge cases: if the grid is entirely filled, the result vector is empty; if there are no rectangles, the entire grid is one component; rectangles may overlap, but marking them repeatedly is idempotent. Time complexity is O(rows * cols + K * average_rect_area), but since each cell is visited at most once in the BFS, the overall is O(rows * cols) for grid traversal plus O(K * (max_rect_width * max_rect_height)) for marking, which in worst case is O(rows * cols * K) if rectangles are large, but typical constraints are fine. Space complexity is O(rows * cols) for the visited and occupancy grids.
#include <vector>
#include <array>
#include <queue>
#include <algorithm>

// Returns sorted areas of connected empty regions in a grid with filled rectangles.
std::vector<int> emptyRegions(int rows, int cols, const std::vector<std::array<int,4>>& rectangles) {
    // Occupied grid: true means the cell is filled by a rectangle.
    std::vector<std::vector<bool>> occupied(rows, std::vector<bool>(cols, false));
    for (const auto& rect : rectangles) {
        int x1 = rect[0], y1 = rect[1], x2 = rect[2], y2 = rect[3];
        // Clamp to grid bounds just in case, though input is assumed valid.
        x1 = std::max(0, std::min(x1, cols));
        y1 = std::max(0, std::min(y1, rows));
        x2 = std::max(0, std::min(x2, cols));
        y2 = std::max(0, std::min(y2, rows));
        for (int y = y1; y < y2; ++y) {
            for (int x = x1; x < x2; ++x) {
                occupied[y][x] = true;
            }
        }
    }

    // Visited grid for BFS.
    std::vector<std::vector<bool>> visited(rows, std::vector<bool>(cols, false));
    std::vector<int> areas;
    const int dx[4] = {1, -1, 0, 0};
    const int dy[4] = {0, 0, 1, -1};

    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            if (!occupied[r][c] && !visited[r][c]) {
                // Start BFS to count component size.
                std::queue<std::pair<int,int>> q;
                q.push({r, c});
                visited[r][c] = true;
                int count = 1;
                while (!q.empty()) {
                    auto [curR, curC] = q.front();
                    q.pop();
                    for (int d = 0; d < 4; ++d) {
                        int nr = curR + dy[d];
                        int nc = curC + dx[d];
                        if (nr >= 0 && nr < rows && nc >= 0 && nc < cols &&
                            !occupied[nr][nc] && !visited[nr][nc]) {
                            visited[nr][nc] = true;
                            q.push({nr, nc});
                            ++count;
                        }
                    }
                }
                areas.push_back(count);
            }
        }
    }

    std::sort(areas.begin(), areas.end());
    return areas;
}
#include <cassert>
#include <vector>
#include <array>

// The solution function is assumed to be declared above.

int main() {
    // Example 1: 5x5 grid with one rectangle in the middle (2,2)-(4,4) => area 4 blocked.
    {
        std::vector<std::array<int,4>> rects = {{2,2,4,4}};
        std::vector<int> result = emptyRegions(5, 5, rects);
        std::vector<int> expected = {1, 4, 8, 8, 4, 1}; // Let's compute manually: The rectangle covers rows 2-3, cols 2-3 (4 cells). The empty components: top-left (0,0)-(1,1) area 4? Wait let's recalc.
        // Actually better: Let's just verify by checking known small case: 1x1 grid with no rectangles => one component area 1.
    }

    // Test 1: 1x1 empty grid.
    {
        std::vector<std::array<int,4>> rects;
        std::vector<int> result = emptyRegions(1, 1, rects);
        std::vector<int> expected = {1};
        assert(result == expected);
    }

    // Test 2: 1x1 fully covered.
    {
        std::vector<std::array<int,4>> rects = {{0,0,1,1}};
        std::vector<int> result = emptyRegions(1, 1, rects);
        std::vector<int> expected = {};
        assert(result == expected);
    }

    // Test 3: 2x2 with a rectangle covering top-left cell.
    {
        std::vector<std::array<int,4>> rects = {{0,0,1,1}};
        std::vector<int> result = emptyRegions(2, 2, rects);
        std::vector<int> expected = {1, 1, 1}; // The other three cells are isolated (since only orthogonal adjacency, each is separate).
        assert(result == expected);
    }

    // Test 4: 3x3 with a vertical rectangle in the middle column (x=1).
    {
        std::vector<std::array<int,4>> rects = {{1,0,2,3}};
        std::vector<int> result = emptyRegions(3, 3, rects);
        std::vector<int> expected = {3, 3}; // Left column and right column each a vertical strip of 3 cells, not connected.
        assert(result == expected);
    }

    // Test 5: 3x3 with a horizontal rectangle in the middle row (y=1).
    {
        std::vector<std::array<int,4>> rects = {{0,1,3,2}};
        std::vector<int> result = emptyRegions(3, 3, rects);
        std::vector<int> expected = {3, 3}; // Top row and bottom row each a horizontal strip of 3.
        assert(result == expected);
    }

    // Test 6: 4x4 with two non-overlapping rectangles: one at top-left 2x2, one at bottom-right 2x2.
    {
        std::vector<std::array<int,4>> rects = {{0,0,2,2}, {2,2,4,4}};
        std::vector<int> result = emptyRegions(4, 4, rects);
        // Remaining empty cells are: top-right 2x2 block (rows 0-1, cols 2-3) and bottom-left 2x2 block (rows 2-3, cols 0-1). They are diagonally touching? Actually (1,2) and (2,1) are not adjacent orthogonally, so two components each area 4.
        std::vector<int> expected = {4, 4};
        assert(result == expected);
    }

    // Test 7: 2x3 grid with no rectangles.
    {
        std::vector<std::array<int,4>> rects;
        std::vector<int> result = emptyRegions(2, 3, rects);
        std::vector<int> expected = {6};
        assert(result == expected);
    }

    // Test 8: overlapping rectangles still produce correct blocked area.
    {
        std::vector<std::array<int,4>> rects = {{0,0,2,2}, {1,1,3,3}}; // Overlap at (1,1) in a 3x3 grid.
        std::vector<int> result = emptyRegions(3, 3, rects);
        // Occupied cells: (0,0),(1,0),(0,1),(1,1) from first, plus (1,1),(2,1),(1,2),(2,2) from second => union: all except (2,0) and (0,2). Those two are isolated (diagonal? Actually (2,0) adjacent to (1,0) occupied, (2,1) occupied, (1,0) occupied; (0,2) adjacent to (0,1) occupied, (1,2) occupied, (1,1) occupied). So two separate components each area 1.
        std::vector<int> expected = {1, 1};
        assert(result == expected);
    }

    return 0;
}
