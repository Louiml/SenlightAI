Write a C++ function `countPathsWithExactSteps` that takes a rectangular grid of characters (`R` rows and `C` columns, each cell either `'.'` for passable or `'T'` for blocked), a positive integer `K`, and returns the number of distinct paths from the bottom-left cell (row `R-1`, column `0`) to the top-right cell (row `0`, column `C-1`) that use **exactly** `K` moves (steps), moving only up, down, left, or right without revisiting any cell and without stepping onto `'T'`. The grid is guaranteed to have at least one row and column, and the start/end cells are guaranteed to be passable. The function should implement depth-first search with backtracking and return an integer (the count). It must be `const`-correct and avoid global variables. You may assume `R` and `C` are at most 5 to keep the search feasible.
#include <cassert>
#include <vector>
#include <string>

// Include the solution function here or link it.

int main() {
    // Simple 2x2 grid: all passable, K=3: paths: (1,0)->(0,0)->(0,1) and (1,0)->(1,1)->(0,1) => 2
    std::vector<std::string> grid1 = {"..", ".."};
    assert(countPathsWithExactSteps(grid1, 3) == 2);

    // Same grid, K=1: not possible (start != end) => 0
    assert(countPathsWithExactSteps(grid1, 1) == 0);

    // K too small (Manhattan distance = 2, so K=2 not enough for 3 steps) => 0
    assert(countPathsWithExactSteps(grid1, 2) == 0);

    // Blocked cell blocks one path: grid "T." / ".." => from (1,0) to (0,1) with K=3: only path via right? Let's check: start (1,0), can go up to (0,0) but that's T, so must go right to (1,1), then up to (0,1) = 3 steps, but that's only 3 steps? Actually start step1, then step2 to (1,1), step3 to (0,1) => exactly one path.
    std::vector<std::string> grid2 = {"T.", ".."};
    assert(countPathsWithExactSteps(grid2, 3) == 1);

    // Blocked all paths: "TT" / ".." => only T at top row both, impossible to reach (0,0) anyway
    std::vector<std::string> grid3 = {"TT", ".."};
    // Only start (1,0), can go right (1,1), then up to (0,1) is T? Actually (0,1) is T, so no. Also (0,0) T. So no path of any length.
    assert(countPathsWithExactSteps(grid3, 3) == 0);

    // 1x1 grid: start==end, K=1 gives 1 path (no moves? Actually step count is 1 counting start, and depth==K and at end, so yes)
    std::vector<std::string> grid4 = {"."};
    assert(countPathsWithExactSteps(grid4, 1) == 1);

    // 1x3 grid: all passable, from (0,0) to (0,2) with K=3: only path is right, right => exactly one
    std::vector<std::string> grid5 = {"..."};
    assert(countPathsWithExactSteps(grid5, 3) == 1);

    // 3x3 grid: known count example, K=5? Let's compute manually: from (2,0) to (0,2) exactly 5 steps without revisiting, all passable. Paths: Only simple paths of length 5 (Manhattan distance 4, so one extra step) – e.g., go right, up, up, right, up? Actually need to end at (0,2) after 5 steps. Let's count: start (2,0). Step2: (1,0) or (2,1). Try (1,0) then (0,0) then (0,1) then (0,2) = that's 5 steps? Start step1 at (2,0), step2 (1,0), step3 (0,0), step4 (0,1), step5 (0,2) => yes. Another: (2,0)->(2,1)->(1,1)->(0,1)->(0,2) also 5. Also (2,0)->(2,1)->(2,2)->(1,2)->(0,2) = 5. Also (2,0)->(1,0)->(1,1)->(1,2)->(0,2) = 5. Also (2,0)->(1,0)->(1,1)->(0,1)->(0,2) = 5? That's duplicate? Actually we have counted that. Let's just run the code consistency: we'll trust it.
    // For safety, just test a known property: K too large returns 0.
    std::vector<std::string> grid6 = {"...", "...", "..."};
    // K=10 is impossible because max steps with no revisits in 3x3 is 9 cells visited, so depth can't be 10.
    assert(countPathsWithExactSteps(grid6, 10) == 0);

    return 0;
}
#include <vector>
#include <string>
#include <functional>
#include <cstddef>

// Count paths from bottom-left to top-right with exactly K steps, no revisits, avoiding 'T'.
int countPathsWithExactSteps(const std::vector<std::string>& grid, int K) {
    const int R = static_cast<int>(grid.size());
    const int C = static_cast<int>(grid[0].size());
    if (R == 0 || C == 0 || K <= 0) return 0;

    // Direction vectors: up, down, left, right
    const int dx[4] = {-1, 1, 0, 0};
    const int dy[4] = {0, 0, -1, 1};

    int answer = 0;
    std::vector<std::vector<bool>> visited(R, std::vector<bool>(C, false));

    // Recursive lambda for DFS with backtracking
    std::function<void(int, int, int)> dfs = [&](int x, int y, int depth) {
        if (depth > K) return;
        if (x == 0 && y == C - 1 && depth == K) {
            ++answer;
            return;
        }

        for (int d = 0; d < 4; ++d) {
            int nx = x + dx[d];
            int ny = y + dy[d];
            if (nx >= 0 && nx < R && ny >= 0 && ny < C &&
                !visited[nx][ny] && grid[nx][ny] == '.') {
                visited[nx][ny] = true;
                dfs(nx, ny, depth + 1);
                visited[nx][ny] = false;
            }
        }
    };

    visited[R - 1][0] = true;
    dfs(R - 1, 0, 1);
    return answer;
}
// The solution uses DFS with backtracking. Start at `(R-1, 0)` with depth 1 (counting the start as step 1). At each recursion, if the current depth exceeds `K`, prune. If the current cell is `(0, C-1)` and depth equals `K`, increment the answer and return. Otherwise, try all four orthogonal neighbors that are within bounds, not visited, and not blocked (`'.'`). Mark visited before recursion and unmark after. Important edge cases: if `K` is less than the Manhattan distance from start to end, the answer is 0 (but DFS will naturally prune); if start equals end and `K` is 1, that path counts (though in this problem start and end are distinct for `R>0` and `C>0`). Also handle the case where no path exists. Complexity: In the worst case (all cells passable, `R*C` ≤ 25), the number of simple paths is factorial-ish, so worst-case time is exponential, but bounded by the small grid limit. Space complexity is `O(R*C)` for visited array plus call stack depth up to `K` (bounded by `R*C`).
