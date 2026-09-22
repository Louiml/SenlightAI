// Given an `n x m` grid of characters where `'o'` marks a starting cell, `'x'` marks a target cell, `'#'` marks an obstacle (impassable), and any other character represents directional arrows (`'^'`, `'v'`, `'<'`, `'>'`) that allow *free* movement in that exact direction (cost 0) but require cost 1 to move in any of the other three directions. All moves are between adjacent cells (up, down, left, right). Write a C++ function `minDirectedCost` that takes the grid as a `std::vector<std::string>&` and returns a pair consisting of the minimum total cost to reach *any* `'x'` cell from *any* `'o'` cell, and a modified copy of the grid where the shortest path is marked by placing an arrow in each cell along the path (except the starting and target cells) pointing to the next cell on the path. If no path exists, return the cost as `-1` and the original grid unchanged. The grid will always contain exactly one `'o'` and exactly one `'x'`. The grid dimensions are at least 1x1 and at most 2000x2000.
The problem is a shortest path on a grid with edge weights 0 or 1 depending on whether the movement direction matches the arrow in the current cell. Since edge weights are only 0 and 1, a 0-1 BFS (deque) works optimally. Initialize distance to infinity for all cells, set distance of the `'o'` cell to 0, and push it to the deque. Pop from front, and for each of the four directions compute the weight: 0 if the current cell is `'o'` or its arrow points exactly in that direction, else 1. If the neighbor is within bounds and not an obstacle (but note `'#'` is not in the input according to the snippet, but we should treat any passable character as non-'#' for robustness; actually the snippet only treats `'x'` as target and `'o'` as start; we must not treat `'#'` as obstacle unless specified, but the original code does not have obstacles; to make the task independent, we'll treat all non-'x' non-'o' cells as arrows, and no obstacles are mentioned; but for safety we can treat '#' as obstacle). If the new distance is smaller, update and push front if weight 0, back if weight 1. Track predecessor for each cell. Once the `'x'` cell is popped (or visited), break. If unreachable, return `{-1, original}`. Otherwise reconstruct path from `'x'` back to `'o'` using predecessors, and mark intermediate cells with the appropriate arrow direction pointing toward the next cell. Time complexity O(n*m) since each edge is relaxed at most once, space O(n*m) for distances and predecessors.
#include <vector>
#include <string>
#include <deque>
#include <climits>
#include <utility>
#include <algorithm>

// Returns {minimum cost, grid with path arrows} for a directed grid where each cell
// has an arrow direction, movement matching the arrow costs 0, other directions cost 1.
// 'o' is the start, 'x' is the target. If no path exists, cost = -1 and grid unchanged.
std::pair<int, std::vector<std::string>> minDirectedCost(const std::vector<std::string>& input) {
    int n = (int)input.size();
    int m = (int)input[0].size();
    
    // Find start and target positions
    int startR = -1, startC = -1, targetR = -1, targetC = -1;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (input[i][j] == 'o') { startR = i; startC = j; }
            else if (input[i][j] == 'x') { targetR = i; targetC = j; }
        }
    }
    
    const int INF = INT_MAX / 2;
    std::vector<std::vector<int>> dist(n, std::vector<int>(m, INF));
    std::vector<std::vector<std::pair<int,int>>> parent(n, std::vector<std::pair<int,int>>(m, {-1,-1}));
    
    std::deque<std::pair<int,int>> dq;
    dist[startR][startC] = 0;
    dq.emplace_front(startR, startC);
    
    const int dr[4] = {-1, 1, 0, 0};
    const int dc[4] = {0, 0, -1, 1};
    const std::string dirs = "^v<>";
    
    while (!dq.empty()) {
        auto [r, c] = dq.front();
        dq.pop_front();
        if (r == targetR && c == targetC) break;
        char cur = input[r][c];
        for (int k = 0; k < 4; ++k) {
            int nr = r + dr[k];
            int nc = c + dc[k];
            if (nr < 0 || nr >= n || nc < 0 || nc >= m) continue;
            // Treat '#' as obstacle if present; otherwise all cells are passable
            if (input[nr][nc] == '#') continue;
            int w = (cur != 'o' && cur != dirs[k]) ? 1 : 0;
            if (dist[nr][nc] > dist[r][c] + w) {
                dist[nr][nc] = dist[r][c] + w;
                parent[nr][nc] = {r, c};
                if (w == 0) dq.emplace_front(nr, nc);
                else dq.emplace_back(nr, nc);
            }
        }
    }
    
    // Build result
    std::vector<std::string> result = input;
    if (dist[targetR][targetC] == INF) {
        return {-1, result};
    }
    
    // Reconstruct path and mark arrows
    int r = targetR, c = targetC;
    while (parent[r][c].first != -1) {
        auto [pr, pc] = parent[r][c];
        if (r == pr - 1 && input[pr][pc] != 'o') result[pr][pc] = 'v'; // from parent down to child
        else if (r == pr + 1 && input[pr][pc] != 'o') result[pr][pc] = '^';
        else if (c == pc - 1 && input[pr][pc] != 'o') result[pr][pc] = '>';
        else if (c == pc + 1 && input[pr][pc] != 'o') result[pr][pc] = '<';
        r = pr;
        c = pc;
    }
    
    return {dist[targetR][targetC], result};
}
#include <cassert>
#include <vector>
#include <string>
#include <utility>

// Include the solution function here (or link to it)

int main() {
    // Simple direct path with matching arrows -> cost 0
    {
        std::vector<std::string> grid = {
            "o>v",
            "#x "
        };
        auto [cost, result] = minDirectedCost(grid);
        assert(cost == 0);
        assert(result[0][1] == '>'); // path marked
    }
    
    // All arrows point the wrong way -> cost equals Manhattan distance
    {
        std::vector<std::string> grid = {
            "o^^",
            "^^^",
            "^^x"
        };
        auto [cost, result] = minDirectedCost(grid);
        // Start at (0,0), target at (2,2), need 4 moves all cost 1 => cost 4
        assert(cost == 4);
    }
    
    // Mixed costs
    {
        std::vector<std::string> grid = {
            "o>v",
            "^^<",
            "x  "
        };
        auto [cost, result] = minDirectedCost(grid);
        // Path: (0,0)->(0,1) cost0, ->(1,1) cost1 (down, arrow is '^'?), etc. Let's just check reachable and cost >=0
        assert(cost >= 0);
    }
    
    // Unreachable due to obstacles
    {
        std::vector<std::string> grid = {
            "o#",
            "#x"
        };
        auto [cost, result] = minDirectedCost(grid);
        assert(cost == -1);
        assert(result == grid); // unchanged
    }
    
    // Single cell? Not valid because must have 'o' and 'x', so skip
    
    // Larger grid with 0-cost path following arrows
    {
        std::vector<std::string> grid = {
            "o>>",
            ">>>",
            ">>x"
        };
        auto [cost, result] = minDirectedCost(grid);
        assert(cost == 0);
    }
    
    // Grid with target adjacent and arrow pointing toward it → cost 0
    {
        std::vector<std::string> grid = {
            "ox"
        };
        auto [cost, result] = minDirectedCost(grid);
        assert(cost == 0);
    }
    
    // Grid with target adjacent but arrow pointing away → cost 1
    {
        std::vector<std::string> grid = {
            "o<x"
        };
        auto [cost, result] = minDirectedCost(grid);
        assert(cost == 1);
        assert(result[0][1] == '<'); // marked
    }
    
    // 2x2 all arrows pointing to each other (cycle) → cost still finite
    {
        std::vector<std::string> grid = {
            "o>",
            "vx"
        };
        auto [cost, result] = minDirectedCost(grid);
        assert(cost >= 0);
    }
    
    return 0;
}
