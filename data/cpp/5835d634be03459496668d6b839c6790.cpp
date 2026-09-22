/*
Given a rectangular grid of uppercase letters (A–Z), write a C++ function that computes the sum of products of area and perimeter for each connected region of identical letters. Two cells belong to the same region if they are orthogonally adjacent (up, down, left, right) and contain the same letter. The perimeter of a region is the number of cell sides that are not shared with another cell of the same region (i.e., sides bordering outside the grid or a different letter). The function should take a `vector<string>` representing the grid and return a `long long` equal to the sum over all regions of `(area * perimeter)`. The input grid is non-empty, rectangular, and may contain multiple disconnected regions of the same letter. Each region is considered independently even if it has the same letter as another region.
*/

#include <vector>
#include <string>
#include <cmath>

// Compute sum of (area * perimeter) for all connected regions of identical letters.
long long totalFencingCost(const std::vector<std::string>& grid) {
    if (grid.empty() || grid[0].empty()) return 0;

    int rows = grid.size();
    int cols = grid[0].size();
    // We'll use a mutable copy to mark visited cells by negating letter id.
    std::vector<std::vector<int>> nums(rows, std::vector<int>(cols));
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            nums[r][c] = grid[r][c] - 'A' + 1; // 1..26
        }
    }

    const int dr[4] = {-1, 0, 1, 0};
    const int dc[4] = {0, 1, 0, -1};

    // Recursive lambda for DFS.
    auto dfs = [&](int r, int c, int& area, int& perimeter, auto&& self) -> void {
        int curr = nums[r][c];
        area++;
        int shared = 0;
        for (int d = 0; d < 4; ++d) {
            int nr = r + dr[d];
            int nc = c + dc[d];
            if (nr >= 0 && nr < rows && nc >= 0 && nc < cols) {
                int next = nums[nr][nc];
                if (next == curr || -next == curr) {
                    shared++;
                }
            }
        }
        perimeter += (4 - shared);
        nums[r][c] = -curr; // mark visited preserving original letter via abs

        for (int d = 0; d < 4; ++d) {
            int nr = r + dr[d];
            int nc = c + dc[d];
            if (nr >= 0 && nr < rows && nc >= 0 && nc < cols) {
                if (nums[nr][nc] == curr) {
                    self(nr, nc, area, perimeter, self);
                }
            }
        }
    };

    long long answer = 0;
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            if (nums[r][c] >= 0) { // unvisited
                int area = 0;
                int perimeter = 0;
                dfs(r, c, area, perimeter, dfs);
                answer += static_cast<long long>(area) * perimeter;
            }
        }
    }
    return answer;
}

#include <cassert>
#include <vector>
#include <string>

int main() {
    // Single cell
    assert(totalFencingCost({"A"}) == 1 * 4);
    // Two adjacent same letters
    assert(totalFencingCost({"AA"}) == 2 * 6); // area 2, perimeter 6 => 12
    // Two different letters side by side
    assert(totalFencingCost({"AB"}) == 1*4 + 1*4);
    // 2x2 all same
    assert(totalFencingCost({"AA", "AA"}) == 4 * 8); // area 4, perimeter 8 => 32
    // 2x2 checkerboard
    assert(totalFencingCost({"AB", "BA"}) == 4 * 4); // four separate regions each area 1 perimeter 4 => 16
    // L-shaped region
    assert(totalFencingCost({"AA", "A"}) == 3 * 8); // area 3, perimeter 8 => 24
    // Mixed with two regions of same letter
    assert(totalFencingCost({"A", "A"}) == 1*4 + 1*4);
    // Larger example: 2x3 with two regions
    // AA B
    // AA B
    assert(totalFencingCost({"AAB", "AAB"}) == 4*8 + 2*6); // 32 + 12 = 44
    // Empty? Not expected but handle gracefully
    assert(totalFencingCost({}) == 0);
    return 0;
}

// The core approach is a depth-first search (DFS) flood fill. For each unvisited cell, we initiate a DFS that traverses all connected cells with the same letter. During traversal, we count the region's `area` (number of cells) and `perimeter` (number of exposed sides). To avoid revisiting cells, we mark visited cells by negating their value in a copy of the grid or using a separate boolean visited array. For each cell, we examine its four orthogonal neighbors. A side is considered "exposed" if the neighbor is outside the grid or has a different letter (including a previously visited cell of the same letter? No—careful: when we negate visited cells, the original letter is still recoverable as `abs(value)`. So for a neighbor that has been visited and negated, its absolute value still equals the current letter, so the side is shared, not exposed). Thus, for each neighbor, if it is inside bounds and `abs(neighbor_value) == current_letter`, we do not count that side in the perimeter; otherwise, we do. After processing all neighbors, the perimeter contribution is `4 - shared_sides`. The total region perimeter is accumulated as we DFS through the region. After the DFS finishes, we multiply area by perimeter and add to the global answer. Edge cases include single-cell regions (perimeter 4), regions touching the grid boundary (those sides count), and multiple identical regions that must be visited separately—handled naturally by iterating through all cells and starting DFS from each unvisited cell. Time complexity is O(R*C) because each cell is visited exactly once, and each neighbor check is constant. Space complexity is O(R*C) in the worst case for the recursion stack (if the grid is one large region) and for the visited markers.
