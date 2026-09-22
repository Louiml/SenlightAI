Write a C++ function `std::vector<std::vector<int>> floodFill(std::vector<std::vector<int>>& image, int sr, int sc, int newColor)` that, given a 2D grid of integers representing pixel colors, performs a flood fill starting from cell `(sr, sc)` and changes the color of all connected cells (up, down, left, right) that share the original starting color to `newColor`. Return the modified image. You may assume the input grid is non-empty and rectangular, and that `(sr, sc)` is a valid index. The function should modify and then return the same grid (by reference) referenced by the input, but for testing convenience you may both modify in place and return it.
The problem is a classic graph traversal on a grid, where each cell is a node and edges connect orthogonal neighbors. The main difficulty is that when `newColor` equals the original color, a naive BFS/DFS without visited tracking would infinite loop because the new color is the same as the target color, so after recoloring a cell it still matches the target and would be re-queued. Therefore we must either track visited cells or check if `newColor == target` and return early. The solution uses BFS with a `visited` boolean grid. We start by storing the original color of the starting cell as `target`. If `target == newColor`, we can return immediately (no changes needed). Otherwise, we initialize a queue with the starting cell, mark it visited, and then repeatedly pop a cell, set its color to `newColor`, and for each of its four neighbors (within bounds, not visited, and having color equal to `target`) we mark visited and push. BFS ensures all connected cells are processed exactly once. Edge cases include: single-cell image, the starting cell already having `newColor` (handled by early return), and images with one row or one column (bounds checking handles these). Time complexity is O(R*C) because each cell is visited at most once and each edge is considered twice. Space complexity is O(R*C) for the visited grid and the queue in the worst case (e.g., when the entire image is one connected region).
#include <vector>
#include <queue>
#include <utility>

// Performs a flood fill starting from (sr, sc) changing all connected cells
// that have the same initial color as (sr, sc) to newColor.
// The input image is modified in place and returned by reference.
std::vector<std::vector<int>>& floodFill(
    std::vector<std::vector<int>>& image,
    int sr,
    int sc,
    int newColor
) {
    const int rows = image.size();
    const int cols = image[0].size();
    const int target = image[sr][sc];

    // If the target color already equals newColor, nothing changes.
    if (target == newColor) {
        return image;
    }

    std::vector<std::vector<bool>> visited(rows, std::vector<bool>(cols, false));
    std::queue<std::pair<int, int>> q;
    q.push({sr, sc});
    visited[sr][sc] = true;

    const int dr[] = {-1, 1, 0, 0};
    const int dc[] = {0, 0, -1, 1};

    while (!q.empty()) {
        auto [r, c] = q.front();
        q.pop();

        image[r][c] = newColor;

        for (int d = 0; d < 4; ++d) {
            int nr = r + dr[d];
            int nc = c + dc[d];

            if (nr >= 0 && nr < rows && nc >= 0 && nc < cols &&
                !visited[nr][nc] && image[nr][nc] == target) {
                visited[nr][nc] = true;
                q.push({nr, nc});
            }
        }
    }

    return image;
}
#include <cassert>
#include <vector>

// The solution function is declared above; this is just the test harness.
int main() {
    // Test 1: Basic fill
    std::vector<std::vector<int>> img1 = {{1,1,1},{1,1,0},{1,0,1}};
    auto res1 = floodFill(img1, 1, 1, 2);
    assert((res1 == std::vector<std::vector<int>>{{2,2,2},{2,2,0},{2,0,1}}));

    // Test 2: Already has newColor -> unchanged
    std::vector<std::vector<int>> img2 = {{0,0,0},{0,1,1}};
    auto res2 = floodFill(img2, 0, 0, 0);
    assert((res2 == std::vector<std::vector<int>>{{0,0,0},{0,1,1}}));

    // Test 3: Single cell
    std::vector<std::vector<int>> img3 = {{5}};
    auto res3 = floodFill(img3, 0, 0, 9);
    assert((res3 == std::vector<std::vector<int>>{{9}}));

    // Test 4: Single row, fill from middle
    std::vector<std::vector<int>> img4 = {{1,2,1,1}};
    auto res4 = floodFill(img4, 0, 2, 3);
    assert((res4 == std::vector<std::vector<int>>{{1,2,3,3}}));

    // Test 5: Single column, fill all
    std::vector<std::vector<int>> img5 = {{4},{4},{4}};
    auto res5 = floodFill(img5, 1, 0, 7);
    assert((res5 == std::vector<std::vector<int>>{{7},{7},{7}}));

    // Test 6: Disconnected regions, only the connected one changes
    std::vector<std::vector<int>> img6 = {{1,2},{1,2},{1,2}};
    auto res6 = floodFill(img6, 0, 0, 8);
    assert((res6 == std::vector<std::vector<int>>{{8,2},{8,2},{8,2}}));

    // Test 7: Large area, ensure all corners connected are filled
    std::vector<std::vector<int>> img7 = {{0,0,0,0},{0,1,0,1},{0,0,0,0}};
    auto res7 = floodFill(img7, 0, 0, 5);
    assert((res7 == std::vector<std::vector<int>>{{5,5,5,5},{5,1,5,1},{5,5,5,5}}));

    // Test 8: NewColor different from target but starts at same color cell
    std::vector<std::vector<int>> img8 = {{1,0},{0,1}};
    auto res8 = floodFill(img8, 0, 0, 2);
    assert((res8 == std::vector<std::vector<int>>{{2,0},{0,1}}));

    return 0;
}
