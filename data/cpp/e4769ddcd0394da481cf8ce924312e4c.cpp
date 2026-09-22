/*
Write a C++ function `int daysToRipeAll(const std::vector<std::vector<int>>& grid)` that accepts a rectangular grid representing a tomato storage box. Each cell contains `0` (unripe), `1` (ripe at start), or `-1` (empty). Each day, every ripe tomato ripens all adjacent (up, down, left, right) unripe tomatoes simultaneously. Determine the minimum number of days needed for all unripe tomatoes to become ripe, or return `-1` if this is impossible (e.g., an unripe tomato is isolated by empty cells or grid boundaries). If all tomatoes are already ripe at the start, return `0`. The function must handle empty grids, single-row/column grids, and grids with no ripe tomatoes.
*/
#include <vector>
#include <queue>
#include <utility>

// Return minimum days for all unripe tomatoes to become ripe, or -1 if impossible.
int daysToRipeAll(const std::vector<std::vector<int>>& grid) {
    if (grid.empty() || grid[0].empty()) return 0;

    const int rows = static_cast<int>(grid.size());
    const int cols = static_cast<int>(grid[0].size());

    std::queue<std::pair<int, int>> q;
    int unripe = 0;

    // Collect initial ripe tomatoes and count unripe.
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            if (grid[r][c] == 1) {
                q.push({r, c});
            } else if (grid[r][c] == 0) {
                ++unripe;
            }
        }
    }

    // If all already ripe or no tomatoes at all.
    if (unripe == 0) return 0;

    // BFS level-order.
    int days = 0;
    const int dr[4] = {-1, 1, 0, 0};
    const int dc[4] = {0, 0, -1, 1};

    while (!q.empty() && unripe > 0) {
        int level_size = static_cast<int>(q.size());
        for (int i = 0; i < level_size; ++i) {
            auto [r, c] = q.front();
            q.pop();
            for (int d = 0; d < 4; ++d) {
                int nr = r + dr[d];
                int nc = c + dc[d];
                if (nr >= 0 && nr < rows && nc >= 0 && nc < cols && grid[nr][nc] == 0) {
                    // Mark as ripe (use const_cast carefully, or better: copy grid? 
                    // Since we are not allowed to modify const input, we simulate using a visited set.
                    // But to keep it simple and correct, we'll create a mutable copy inside.
                }
            }
        }
        ++days;
    }

    // The above incomplete because we cannot modify const grid. Let's rewrite properly.
    // Re-implement with a visited boolean grid to avoid modifying input.
    // (We'll replace the whole function content below.)
}

Actually, the correct approach requires a mutable structure. Since the input is `const`, we make a copy or use a `visited` matrix. Here’s the complete solution:

#include <vector>
#include <queue>
#include <utility>

// Return minimum days for all unripe tomatoes to become ripe, or -1 if impossible.
int daysToRipeAll(const std::vector<std::vector<int>>& grid) {
    if (grid.empty() || grid[0].empty()) return 0;

    const int rows = static_cast<int>(grid.size());
    const int cols = static_cast<int>(grid[0].size());

    // Mutable copy to track ripeness without altering const input.
    std::vector<std::vector<int>> state = grid;

    std::queue<std::pair<int, int>> q;
    int unripe = 0;

    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            if (state[r][c] == 1) {
                q.push({r, c});
            } else if (state[r][c] == 0) {
                ++unripe;
            }
        }
    }

    if (unripe == 0) return 0;

    const int dr[4] = {-1, 1, 0, 0};
    const int dc[4] = {0, 0, -1, 1};
    int days = 0;

    while (!q.empty() && unripe > 0) {
        int level_size = static_cast<int>(q.size());
        for (int i = 0; i < level_size; ++i) {
            auto [r, c] = q.front();
            q.pop();
            for (int d = 0; d < 4; ++d) {
                int nr = r + dr[d];
                int nc = c + dc[d];
                if (nr >= 0 && nr < rows && nc >= 0 && nc < cols && state[nr][nc] == 0) {
                    state[nr][nc] = 1;
                    --unripe;
                    q.push({nr, nc});
                }
            }
        }
        ++days;
    }

    return (unripe == 0) ? days : -1;
}
#include <cassert>
#include <vector>

int main() {
    // Basic 2x2 case.
    assert(daysToRipeAll({{1,0},{0,1}}) == 1);
    // All already ripe.
    assert(daysToRipeAll({{1,1},{1,1}}) == 0);
    // Impossible (isolated unripe).
    assert(daysToRipeAll({{1,-1},{0,-1}}) == -1);
    // Single row.
    assert(daysToRipeAll({{1,0,0,0}}) == 3);
    // Single column.
    assert(daysToRipeAll({{1},{0},{0},{1}}) == 2);
    // Empty grid.
    assert(daysToRipeAll({}) == 0);
    // No ripe tomatoes, but unripe present.
    assert(daysToRipeAll({{0,0},{0,0}}) == -1);
    // Mixed with empty cells blocking propagation.
    assert(daysToRipeAll({{1,0,-1,0}}) == -1);
    // Larger case.
    assert(daysToRipeAll({{1,0,0},{0,0,0},{0,0,1}}) == 2);
    return 0;
}
// The problem is classic multi-source BFS for shortest propagation time. Initialize a queue with all initially ripe tomato positions (value `1`), and count the number of unripe tomatoes. Perform level‑order BFS: for each day, process all tomatoes currently in the queue (the ones that became ripe on the previous day), and for each such tomato, check its four neighbors. If a neighbor is unripe (value `0`), mark it ripe, decrement the unripe count, and enqueue it. Increment the day counter after processing each complete layer. The BFS continues as long as the queue is not empty and there remain unripe tomatoes. If after BFS the unripe count is still positive, return `-1`. Important edge cases: an empty grid (no cells) — return `0`; grid with no ripe tomatoes but at least one unripe → `-1`; grid with all ripe → `0`; grid with unripe tomatoes reachable only through empty cells → `-1`. Time complexity: O(rows × cols) because each cell is processed at most once. Space complexity: O(rows × cols) for the queue in the worst case.
