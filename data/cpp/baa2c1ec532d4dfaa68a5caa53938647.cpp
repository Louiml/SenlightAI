// Write a C++ function `std::pair<int, int> largestBlobAreaPerimeter(const std::vector<std::vector<char>>& grid)` that takes a square grid of characters where `'#'` represents filled cells and `.` represents empty cells, and returns a pair `{area, perimeter}` for the connected component (blob) of `'#'` cells that has the maximum area. If multiple blobs have the same maximum area, return the one with the smallest perimeter. Cells are considered connected only horizontally or vertically (4-directional). The grid will have at least 1 row and at least 1 column. The perimeter of a blob is the total number of edges of its cells that are adjacent to an empty cell or outside the grid boundary. The function should be self-contained and not rely on file I/O.

The solution uses flood fill (DFS) to explore each unvisited `'#'` cell. For each blob, we track its area (number of `'#'` cells) and its perimeter. The perimeter is computed by, for each `'#'` cell in the blob, counting its neighbors that are either outside the grid or are `.`. To avoid double counting, we only count a neighbor as contributing to the perimeter if that neighbor is not a `'#'` cell that belongs to the same blob (i.e., not visited yet). An efficient approach: during DFS, when we move to a neighbor, if the neighbor is out-of-bounds or is `.`, we increment the current blob's perimeter; if the neighbor is `'#'` and unvisited, we recurse. We maintain a visited matrix parallel to the grid. After all blobs are processed, we select the one with the largest area, and among ties, the smallest perimeter. Edge cases: a grid with only one cell, a blob that touches the boundary, blobs with holes (but holes are not part of the blob, so the perimeter includes the hole boundary because the hole cells are `.`). Time complexity is O(N^2) for an N×N grid because each cell is visited at most once. Space complexity is O(N^2) for the visited matrix and recursion stack in worst case.

#include <vector>
#include <utility>
#include <functional>

// Returns {area, perimeter} for the blob with maximum area (ties: smallest perimeter).
std::pair<int, int> largestBlobAreaPerimeter(const std::vector<std::vector<char>>& grid) {
    if (grid.empty() || grid[0].empty()) return {0, 0};

    const int rows = static_cast<int>(grid.size());
    const int cols = static_cast<int>(grid[0].size());

    std::vector<std::vector<bool>> visited(rows, std::vector<bool>(cols, false));

    int bestArea = 0;
    int bestPerimeter = 0;

    // Directions: up, down, left, right
    const int dr[4] = {-1, 1, 0, 0};
    const int dc[4] = {0, 0, -1, 1};

    std::function<void(int, int, int&, int&)> dfs = [&](int r, int c, int& area, int& perimeter) {
        visited[r][c] = true;
        area++;

        for (int k = 0; k < 4; k++) {
            int nr = r + dr[k];
            int nc = c + dc[k];

            // If neighbor is out of bounds or empty, it contributes to perimeter.
            if (nr < 0 || nr >= rows || nc < 0 || nc >= cols || grid[nr][nc] == '.') {
                perimeter++;
            } else if (grid[nr][nc] == '#' && !visited[nr][nc]) {
                // Recurse into unvisited filled neighbor.
                dfs(nr, nc, area, perimeter);
            }
        }
    };

    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            if (grid[r][c] == '#' && !visited[r][c]) {
                int area = 0;
                int perimeter = 0;
                dfs(r, c, area, perimeter);

                // Update best: larger area, or same area with smaller perimeter.
                if (area > bestArea || (area == bestArea && perimeter < bestPerimeter)) {
                    bestArea = area;
                    bestPerimeter = perimeter;
                }
            }
        }
    }

    return {bestArea, bestPerimeter};
}

#include <cassert>
#include <vector>
#include <utility>

// Include the solution header or paste the solution here.
std::pair<int, int> largestBlobAreaPerimeter(const std::vector<std::vector<char>>& grid);

int main() {
    // 1x1 filled cell: area 1, perimeter 4
    assert(largestBlobAreaPerimeter({{'#'}}) == std::make_pair(1, 4));

    // 1x1 empty cell: no blob, area 0, perimeter 0
    assert(largestBlobAreaPerimeter({{'.'}}) == std::make_pair(0, 0));

    // 2x2 full of '#': one blob area 4, perimeter 8 (boundary edges)
    assert(largestBlobAreaPerimeter({{'#','#'},{'#','#'}}) == std::make_pair(4, 8));

    // Two separate blobs: one area 2, one area 3; best is area 3
    std::vector<std::vector<char>> grid3 = {
        {'#','#','.'},
        {'.','.','#'},
        {'.','.','#'}
    };
    assert(largestBlobAreaPerimeter(grid3) == std::make_pair(3, 6));

    // Tie in area: two blobs area 2, pick the one with smaller perimeter.
    // Left blob perimeter 6 (touches boundary), right blob perimeter 8 (interior)
    std::vector<std::vector<char>> grid4 = {
        {'#','#','.','.'},
        {'.','.','.','#'},
        {'.','.','.','#'}
    };
    assert(largestBlobAreaPerimeter(grid4) == std::make_pair(2, 6));

    // Blob with a hole: outer ring area 8, perimeter 16 (4 outer + 4 inner)
    std::vector<std::vector<char>> grid5 = {
        {'#','#','#','#'},
        {'#','.','.','#'},
        {'#','.','.','#'},
        {'#','#','#','#'}
    };
    assert(largestBlobAreaPerimeter(grid5) == std::make_pair(8, 16));

    // All empty grid
    std::vector<std::vector<char>> grid6 = {
        {'.','.'},
        {'.','.'}
    };
    assert(largestBlobAreaPerimeter(grid6) == std::make_pair(0, 0));

    // Single row with two blobs separated by '.'
    std::vector<std::vector<char>> grid7 = {{'#','#','.','#','#'}};
    assert(largestBlobAreaPerimeter(grid7) == std::make_pair(2, 6));

    return 0;
}
