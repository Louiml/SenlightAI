Given an `n x n` grid (`n` between 1 and 30) of `0`s and `1`s, where the `1`s form at least one simple closed boundary (a loop that does not touch itself or the grid edge), write a C++ function `std::vector<std::vector<int>> fillEnclosedZeros(const std::vector<std::vector<int>>& grid)` that returns a copy of the grid in which every `0` that lies strictly inside *any* closed boundary is changed to `2`. The function must handle the possibility that the closed loop may be anywhere in the grid (not necessarily touching the border), and there may be other `0`s outside the boundary that remain unchanged. The input grid is guaranteed to contain at least one closed loop of `1`s; the loop may have holes (empty spaces) inside, but all such interior `0`s must be filled. The boundary of the grid is considered “outside”, so any `0` connected (via 4-directional movement) to the grid edge is outside and must remain `0`. All `1`s stay as `1`.

The core idea is to distinguish interior `0`s from exterior `0`s using flood‑fill from the border. Since the grid boundary is always outside, we can mark all `0` cells reachable from any border cell (moving in 4 directions) as “exterior”. Any remaining `0` that was not visited by this flood‑fill is necessarily trapped entirely by `1`s and is therefore inside the closed loop. We then convert all unvisited `0`s to `2` and return the modified grid. Edge cases: if the grid is all `1`s or has no exterior `0`s (the loop touches the border), the flood‑fill from the border will simply do nothing, and all `0`s (if any) are interior. The input is guaranteed to have at least one closed loop, so there will be at least one interior `0` unless the loop encloses no `0` (which is not possible because the loop is made of `1`s and must enclose some region, but we handle it gracefully by returning the grid unchanged if there are no interior zeros). The algorithm uses two passes: first, a BFS/DFS from all border `0`s marking visited; second, iterate all cells and set unvisited `0`s to `2`. Time complexity: each cell is processed a constant number of times, so \(O(n^2)\). Space complexity: \(O(n^2)\) for the visited array and the recursion stack (if using DFS) or queue (BFS); we use an iterative BFS to avoid stack overflow for \(n=30\), though recursion is safe. We use a copy of the grid to avoid mutating the input.

#include <vector>
#include <queue>
#include <utility>

// Return a copy of the grid where every 0 that is completely enclosed by 1s
// (not connected to the grid border by 4-directional moves) is changed to 2.
std::vector<std::vector<int>> fillEnclosedZeros(const std::vector<std::vector<int>>& grid) {
    const int n = static_cast<int>(grid.size());
    if (n == 0) return {};

    std::vector<std::vector<int>> result = grid;
    std::vector<std::vector<bool>> visited(n, std::vector<bool>(n, false));

    // Directions: down, up, right, left (or any order)
    const int dx[4] = {1, -1, 0, 0};
    const int dy[4] = {0, 0, 1, -1};

    // Perform BFS from every border cell that is 0
    std::queue<std::pair<int, int>> q;
    for (int i = 0; i < n; ++i) {
        // Left and right borders
        if (grid[i][0] == 0 && !visited[i][0]) {
            visited[i][0] = true;
            q.push({i, 0});
        }
        if (grid[i][n - 1] == 0 && !visited[i][n - 1]) {
            visited[i][n - 1] = true;
            q.push({i, n - 1});
        }
        // Top and bottom borders (but avoid double-checking corners; they are included above)
    }
    for (int j = 0; j < n; ++j) {
        if (grid[0][j] == 0 && !visited[0][j]) {
            visited[0][j] = true;
            q.push({0, j});
        }
        if (grid[n - 1][j] == 0 && !visited[n - 1][j]) {
            visited[n - 1][j] = true;
            q.push({n - 1, j});
        }
    }

    while (!q.empty()) {
        auto [x, y] = q.front();
        q.pop();
        for (int d = 0; d < 4; ++d) {
            int nx = x + dx[d];
            int ny = y + dy[d];
            if (nx >= 0 && nx < n && ny >= 0 && ny < n && !visited[nx][ny] && grid[nx][ny] == 0) {
                visited[nx][ny] = true;
                q.push({nx, ny});
            }
        }
    }

    // All unvisited zeros are interior
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (grid[i][j] == 0 && !visited[i][j]) {
                result[i][j] = 2;
            }
        }
    }

    return result;
}

#include <cassert>
#include <vector>

// Assume the solution function is declared above (e.g., in the same file).
int main() {
    // Test 1: Simple loop enclosing one zero
    std::vector<std::vector<int>> g1 = {
        {1, 1, 1, 1},
        {1, 0, 0, 1},
        {1, 0, 1, 1},
        {1, 1, 1, 1}
    };
    std::vector<std::vector<int>> expected1 = {
        {1, 1, 1, 1},
        {1, 2, 2, 1},
        {1, 2, 1, 1},
        {1, 1, 1, 1}
    };
    assert(fillEnclosedZeros(g1) == expected1);

    // Test 2: Loop does not touch border, exterior zeros remain
    std::vector<std::vector<int>> g2 = {
        {0, 0, 0, 0, 0},
        {0, 1, 1, 1, 0},
        {0, 1, 0, 1, 0},
        {0, 1, 1, 1, 0},
        {0, 0, 0, 0, 0}
    };
    std::vector<std::vector<int>> expected2 = {
        {0, 0, 0, 0, 0},
        {0, 1, 1, 1, 0},
        {0, 1, 2, 1, 0},
        {0, 1, 1, 1, 0},
        {0, 0, 0, 0, 0}
    };
    assert(fillEnclosedZeros(g2) == expected2);

    // Test 3: Loop touches border, still fill interior zeros
    std::vector<std::vector<int>> g3 = {
        {1, 1, 1},
        {1, 0, 1},
        {1, 1, 1}
    };
    std::vector<std::vector<int>> expected3 = {
        {1, 1, 1},
        {1, 2, 1},
        {1, 1, 1}
    };
    assert(fillEnclosedZeros(g3) == expected3);

    // Test 4: All ones, no zeros
    std::vector<std::vector<int>> g4 = {{1, 1}, {1, 1}};
    std::vector<std::vector<int>> expected4 = {{1, 1}, {1, 1}};
    assert(fillEnclosedZeros(g4) == expected4);

    // Test 5: Larger loop with two separate interior regions (both filled)
    std::vector<std::vector<int>> g5 = {
        {0, 0, 0, 0, 0, 0},
        {0, 1, 1, 1, 1, 0},
        {0, 1, 0, 0, 1, 0},
        {0, 1, 0, 0, 1, 0},
        {0, 1, 1, 1, 1, 0},
        {0, 0, 0, 0, 0, 0}
    };
    std::vector<std::vector<int>> expected5 = {
        {0, 0, 0, 0, 0, 0},
        {0, 1, 1, 1, 1, 0},
        {0, 1, 2, 2, 1, 0},
        {0, 1, 2, 2, 1, 0},
        {0, 1, 1, 1, 1, 0},
        {0, 0, 0, 0, 0, 0}
    };
    assert(fillEnclosedZeros(g5) == expected5);

    return 0;
}
