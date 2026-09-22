Given a `row × col` grid of water cells and a list `cells` where `cells[i] = [r, c]` (1-indexed) represents a cell that turns to land on day `i` (0-indexed day meaning the cell is land after processing `cells[0..i]`), write a C++ function `int latestDayToCross(int row, int col, const std::vector<std::vector<int>>& cells)` that returns the latest day (0-indexed) such that there still exists a path of water cells from any cell in the top row to any cell in the bottom row, moving only up/down/left/right. If even after day 0 no such path exists (i.e., the very first cell blocks all top-row starts), return 0. The path must consist entirely of cells that are water (not turned to land). The input guarantees at least one cell exists in the list, and `row, col ≥ 1`.
The solution uses binary search on the day index because the feasibility (whether a top-to-bottom water path exists) is monotonic: once the path disappears, it stays gone for all larger days (adding more land cells cannot restore a water path). For a given candidate day `mid`, we mark all cells from `cells[0]` through `cells[mid]` as land (using 1-indexed conversion to 0-indexed grid). Then we perform a multi-source BFS starting from every water cell in the top row. If during BFS we reach any water cell in the bottom row, the path exists; otherwise not. The binary search finds the largest `mid` for which the path exists, which equals the answer. Edge cases: when the path exists only before any land appears (i.e., `mid = -1` conceptually), but since the problem says to return 0 in that scenario, we start binary search with `lo = 0`; if the path fails at day 0, the answer is 0. Time complexity: each feasibility check is O(row·col), and binary search runs O(log(cells.size())) times, so total O(row·col·log(cells.size())). Space complexity is O(row·col) for the grid and BFS queue.
#include <vector>
#include <queue>
#include <cstring>

// Returns the latest day (0-indexed) when a top-to-bottom water path still exists.
// cells[i] = {r, c} (1-indexed) turns to land on day i, and stays land afterward.
int latestDayToCross(int row, int col, const std::vector<std::vector<int>>& cells) {
    auto possible = [&](int lim) -> bool {
        // Build grid: 0 = water, 1 = land
        std::vector<std::vector<int>> grid(row, std::vector<int>(col, 0));
        for (int i = 0; i <= lim; ++i) {
            int r = cells[i][0] - 1;
            int c = cells[i][1] - 1;
            grid[r][c] = 1;
        }

        std::queue<std::pair<int, int>> q;
        // Push all water cells in the top row as BFS sources
        for (int c = 0; c < col; ++c) {
            if (grid[0][c] == 0) {
                q.push({0, c});
                grid[0][c] = 1;  // mark visited
            }
        }

        const int dr[4] = {0, 0, 1, -1};
        const int dc[4] = {1, -1, 0, 0};

        while (!q.empty()) {
            auto [r, c] = q.front();
            q.pop();
            if (r == row - 1) return true;  // reached bottom row
            for (int k = 0; k < 4; ++k) {
                int nr = r + dr[k];
                int nc = c + dc[k];
                if (nr >= 0 && nr < row && nc >= 0 && nc < col && grid[nr][nc] == 0) {
                    grid[nr][nc] = 1;  // mark visited when pushing
                    q.push({nr, nc});
                }
            }
        }
        return false;
    };

    int lo = 0, hi = static_cast<int>(cells.size()) - 1, ans = 0;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (possible(mid)) {
            ans = mid;
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return ans;
}
#include <cassert>
#include <vector>

int main() {
    // Basic case: path blocked on day 2
    std::vector<std::vector<int>> cells1 = {{1,1},{2,1},{1,2}};
    assert(latestDayToCross(2, 2, cells1) == 0);

    // Straight path down the second column, blocked on day 3
    std::vector<std::vector<int>> cells2 = {{1,1},{2,1},{3,1},{2,2}};
    assert(latestDayToCross(3, 2, cells2) == 2);

    // Large open grid, first land appears on day 0 but doesn't block (top row start at col2)
    std::vector<std::vector<int>> cells3 = {{1,1}};
    assert(latestDayToCross(1, 3, cells3) == 0);

    // Column blocked immediately by first cell in top row of a single-column grid
    std::vector<std::vector<int>> cells4 = {{1,1}};
    assert(latestDayToCross(1, 1, cells4) == 0);

    // Path remains until the very last cell turns to land
    std::vector<std::vector<int>> cells5 = {{1,1},{1,2},{1,3},{2,1},{2,2},{2,3}};
    assert(latestDayToCross(2, 3, cells5) == 3);

    // All land on day 0 except top-left start, but top row full of land blocks
    std::vector<std::vector<int>> cells6 = {{1,1},{1,2},{1,3},{2,1},{2,2},{2,3},{1,4}};
    assert(latestDayToCross(2, 4, cells6) == 3);

    // Path exists after some land but breaks later
    std::vector<std::vector<int>> cells7 = {{2,2},{1,2},{2,1},{2,3}};
    assert(latestDayToCross(3, 3, cells7) == 1);

    // Single row: all cells become land gradually, path exists until the first land
    std::vector<std::vector<int>> cells8 = {{1,1},{1,2},{1,3}};
    assert(latestDayToCross(1, 3, cells8) == 0);

    // Random moderate case: check monotonic behavior (day 2 works, day 3 fails)
    std::vector<std::vector<int>> cells9 = {{1,1},{2,1},{3,1},{2,2},{1,3}};
    assert(latestDayToCross(3, 3, cells9) == 1);
}
