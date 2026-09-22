Write a C++ function `paintRegion` that takes a 2D vector of integers representing an image, a starting row `sr`, a starting column `sc`, and a target `color`. The function must change the color of the starting pixel and all pixels connected to it via 4-directional adjacency (up, down, left, right) that originally had the same color as the starting pixel. The change applies only to these connected pixels; all other pixels remain unchanged. The function should return the modified image as a new 2D vector (or modify in place and return the same reference). You may assume the grid is non-empty, rectangular, and the starting coordinates are valid. Handle the edge case where the starting pixel already has the target color—in that case, return the image unchanged. Ensure your implementation is efficient and uses a queue-based breadth-first search (BFS) without recursion.
#include <cassert>
#include <vector>

int main() {
    // Test 1: Basic flood fill on a small grid.
    std::vector<std::vector<int>> img1 = {
        {1, 1, 1},
        {1, 1, 0},
        {1, 0, 1}
    };
    auto res1 = paintRegion(img1, 1, 1, 2);
    std::vector<std::vector<int>> expected1 = {
        {2, 2, 2},
        {2, 2, 0},
        {2, 0, 1}
    };
    assert(res1 == expected1);

    // Test 2: Start color already equals target color -> unchanged.
    std::vector<std::vector<int>> img2 = {
        {0, 0},
        {0, 0}
    };
    auto res2 = paintRegion(img2, 0, 0, 0);
    assert(res2 == img2);

    // Test 3: Single-cell image.
    std::vector<std::vector<int>> img3 = {{5}};
    auto res3 = paintRegion(img3, 0, 0, 9);
    assert(res3 == std::vector<std::vector<int>>{{9}});

    // Test 4: Flood fill does not cross different-colored pixels.
    std::vector<std::vector<int>> img4 = {
        {1, 0, 1},
        {1, 0, 1},
        {1, 0, 1}
    };
    auto res4 = paintRegion(img4, 0, 0, 7);
    std::vector<std::vector<int>> expected4 = {
        {7, 0, 1},
        {7, 0, 1},
        {7, 0, 1}
    };
    assert(res4 == expected4);

    // Test 5: Large connected region but isolated pixels unchanged.
    std::vector<std::vector<int>> img5 = {
        {2, 2, 2, 2},
        {2, 3, 3, 2},
        {2, 2, 2, 2}
    };
    auto res5 = paintRegion(img5, 0, 0, 8);
    std::vector<std::vector<int>> expected5 = {
        {8, 8, 8, 8},
        {8, 3, 3, 8},
        {8, 8, 8, 8}
    };
    assert(res5 == expected5);

    return 0;
}
#include <vector>
#include <queue>
#include <utility> // for std::pair

// Performs a flood fill on the given image starting from (sr, sc) with the new color.
// Returns the modified image (same grid, modified in place).
std::vector<std::vector<int>> paintRegion(std::vector<std::vector<int>>& image,
                                          int sr, int sc, int color) {
    // If the starting pixel already has the target color, nothing to do.
    if (image[sr][sc] == color) {
        return image;
    }

    const int rows = static_cast<int>(image.size());
    const int cols = static_cast<int>(image[0].size());
    const int originalColor = image[sr][sc];

    // Visited matrix to avoid re-processing nodes.
    std::vector<std::vector<bool>> visited(rows, std::vector<bool>(cols, false));

    std::queue<std::pair<int, int>> q;
    q.push({sr, sc});
    visited[sr][sc] = true;

    // Direction vectors for up, down, left, right.
    const int dr[] = {-1, 1, 0, 0};
    const int dc[] = {0, 0, -1, 1};

    while (!q.empty()) {
        auto [r, c] = q.front();
        q.pop();

        image[r][c] = color;

        for (int i = 0; i < 4; ++i) {
            int nr = r + dr[i];
            int nc = c + dc[i];

            // Check bounds, ensure the neighbor has the original color,
            // and that it hasn't been visited yet.
            if (nr >= 0 && nr < rows && nc >= 0 && nc < cols &&
                !visited[nr][nc] && image[nr][nc] == originalColor) {
                visited[nr][nc] = true;
                q.push({nr, nc});
            }
        }
    }

    return image;
}
// The solution uses iterative BFS to traverse all pixels reachable from the start position that share the original color. First, check if the start pixel already equals the target color; if so, return immediately to avoid unnecessary work. Store the original color in a temporary variable. Initialize a queue with the starting coordinates and a visited matrix (or rely on color changes themselves as visited markers) to prevent revisiting. For each pixel popped, set its color to the target, then inspect its four neighbors. If a neighbor is within bounds and has the original color and hasn’t been visited (or hasn’t been changed yet), enqueue it. The visited tracking is essential because changing the color in-place would otherwise break the condition for detecting unvisited neighbors. Time complexity is O(m·n) where m and n are grid dimensions, as each pixel is processed at most once. Space complexity is O(m·n) for the visited matrix and O(m·n) in the worst case for the queue, but the queue typically holds fewer nodes (bounded by the perimeter). Edge cases include: start color equals target, single-cell grid, full-grid flood, and unreachable same-color pixels separated by a different color.
