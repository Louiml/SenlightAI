/*
Write a standalone C++ function `hasRatPath` that takes a square maze represented as a 2D vector of integers (where `1` indicates a traversable cell and `0` indicates a blocked cell) and returns a boolean indicating whether a rat can travel from the top-left corner `(0,0)` to the bottom-right corner `(size-1, size-1)`. The rat may only move **right** (increase column) or **down** (increase row). The function should handle any square size (including 1x1), and must return `false` if the start or destination cell is blocked. The function should not modify the input maze; it should pass the maze by `const` reference. Implement the solution using backtracking (DFS) and ensure correct handling of edge cases like a single-cell maze or a maze with no possible path.
*/
#include <vector>

// Returns true if a rat can reach (size-1, size-1) from (0,0) using only right and down moves.
bool hasRatPath(const std::vector<std::vector<int>>& maze) {
    if (maze.empty() || maze[0].empty()) return false;
    int n = static_cast<int>(maze.size());
    // Check if start or end is blocked
    if (maze[0][0] == 0 || maze[n-1][n-1] == 0) return false;

    // DFS helper
    std::vector<std::vector<bool>> visited(n, std::vector<bool>(n, false));
    // Use lambda for internal recursion
    std::function<bool(int,int)> dfs = [&](int r, int c) -> bool {
        // Reached destination
        if (r == n-1 && c == n-1) return true;
        visited[r][c] = true;

        // Try moving right
        if (c+1 < n && maze[r][c+1] == 1 && !visited[r][c+1]) {
            if (dfs(r, c+1)) return true;
        }
        // Try moving down
        if (r+1 < n && maze[r+1][c] == 1 && !visited[r+1][c]) {
            if (dfs(r+1, c)) return true;
        }

        // Backtrack
        visited[r][c] = false;
        return false;
    };

    return dfs(0, 0);
}
#include <cassert>
#include <vector>

// (Include the above solution here)

int main() {
    // Single cell maze, traversable
    std::vector<std::vector<int>> maze1 = {{1}};
    assert(hasRatPath(maze1) == true);

    // Single cell maze, blocked
    std::vector<std::vector<int>> maze2 = {{0}};
    assert(hasRatPath(maze2) == false);

    // Example from the snippet: should have a path
    std::vector<std::vector<int>> maze3 = {
        {1,0,1,0},
        {1,0,1,1},
        {1,0,0,1},
        {1,1,1,1}
    };
    assert(hasRatPath(maze3) == true);

    // No path because start is blocked
    std::vector<std::vector<int>> maze4 = {
        {0,1},
        {1,1}
    };
    assert(hasRatPath(maze4) == false);

    // No path because destination is blocked
    std::vector<std::vector<int>> maze5 = {
        {1,1},
        {1,0}
    };
    assert(hasRatPath(maze5) == false);

    // Simple 2x2 with clear path
    std::vector<std::vector<int>> maze6 = {
        {1,1},
        {1,1}
    };
    assert(hasRatPath(maze6) == true);

    // 2x2 with only down-right path blocked, no alternative
    std::vector<std::vector<int>> maze7 = {
        {1,0},
        {0,1}
    };
    assert(hasRatPath(maze7) == false);

    // 3x3 with a snake-like path
    std::vector<std::vector<int>> maze8 = {
        {1,1,1},
        {0,0,1},
        {1,1,1}
    };
    assert(hasRatPath(maze8) == true);

    // 4x4 with no path due to a wall
    std::vector<std::vector<int>> maze9 = {
        {1,1,0,1},
        {1,1,0,1},
        {1,1,0,1},
        {1,1,0,1}
    };
    assert(hasRatPath(maze9) == false);

    // 3x3 all blocked except a single valid corner
    std::vector<std::vector<int>> maze10 = {
        {1,0,0},
        {0,0,0},
        {0,0,1}
    };
    assert(hasRatPath(maze10) == false);

    return 0;
}
// The problem is a classic rat-in-a-maze variant solvable with depth-first search (DFS) with backtracking. Starting at `(0,0)`, we mark the current cell as visited on a separate path-tracking grid (or use a temporary visited set) and attempt to move right or down. If moving right is valid (within bounds, cell is `1`, and not already part of the current path), we recursively attempt from the new cell. If that fails, we try moving down similarly. If neither works, we backtrack by unmarking the current cell and return `false`. The base case is reaching `(size-1, size-1)`; we return `true` if we reach it, but we must also verify the start and end cells are `1`. Edge cases: 
// - A 1x1 maze: if the single cell is `1`, return `true`; if `0`, return `false`.
// - Blocked start or destination: directly return `false`.
// - Dead ends and cycles: backtracking handles them by unmarking.
// Time complexity: In the worst case, we explore all possible paths; with only two directions and no revisiting, the number of states is at most `N^2`, but since we backtrack, the worst-case number of recursive calls is `O(2^(2N))`? Actually, with memoization not used, it is exponential in the worst case (each cell can be on multiple paths), but typical bound is `O(2^(N^2))`? For clarity: Since each cell can be visited multiple times through different paths, the worst-case time is exponential in the number of cells, but in practice for small N it is fine. Space complexity: `O(N^2)` for the recursion stack in the worst case (the recursion depth can be up to `N^2` in a snake-like path) plus `O(N^2)` for the visited matrix if used.
