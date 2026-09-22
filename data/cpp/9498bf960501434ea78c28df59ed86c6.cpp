Write a C++ function `findShortestPath` that, given a 2D grid represented as a `std::vector<std::vector<int>>` where 0 represents a traversable cell and 1 represents an obstacle, along with a source cell `(startRow, startCol)` and a target cell `(endRow, endCol)` (both valid and traversable), returns a `std::vector<std::pair<int,int>>` containing the shortest path from source to target as a sequence of grid coordinates (inclusive of both endpoints), using only 4-directional moves (up, down, left, right). If no path exists, return an empty vector. The function must be `const`-correct (take the grid by `const` reference), handle edge cases like source equals target (return a single-element path), non-square grids, and obstacles surrounding the start or target.

// The solution uses Breadth-First Search (BFS), which is optimal for unweighted graphs (each move has equal cost). We maintain a queue of cells to explore, a 2D `visited` array (or distance array) initialized to -1, and a parallel 2D array `parent` storing the predecessor coordinates for each visited cell. Starting from the source, we mark it visited with distance 0 and push it into the queue. For each cell popped, we check its four neighbors: if a neighbor is within bounds, not an obstacle (value 0), and not visited, we set its parent to the current cell, mark it visited, and enqueue it. If we reach the target, we stop early and reconstruct the path by tracing back through the `parent` array from target to source, then reverse it. If the queue empties without reaching the target, no path exists, and we return an empty vector. Edge cases: source equals target—immediately return a vector with that single cell; grid with only one row/column—handle bounds correctly; obstacles that block all routes—BFS exhausts without visiting target. Time complexity is \(O(R \times C)\) where \(R\) and \(C\) are grid dimensions, since each cell is visited at most once. Space complexity is also \(O(R \times C)\) for the visited/parent arrays and queue storage.

#include <vector>
#include <queue>
#include <utility>

// Returns the shortest path (as a sequence of grid coordinates) from source to target using BFS.
// Grid: 0 = traversable, 1 = obstacle. Returns empty vector if no path exists.
std::vector<std::pair<int,int>> findShortestPath(
    const std::vector<std::vector<int>>& grid,
    int startRow, int startCol,
    int endRow, int endCol
) {
    const int rows = static_cast<int>(grid.size());
    if (rows == 0) return {};
    const int cols = static_cast<int>(grid[0].size());

    // Validate source and target are within bounds and traversable
    if (startRow < 0 || startRow >= rows || startCol < 0 || startCol >= cols ||
        endRow < 0 || endRow >= rows || endCol < 0 || endCol >= cols ||
        grid[startRow][startCol] != 0 || grid[endRow][endCol] != 0) {
        return {};
    }

    // If source equals target, return single-cell path
    if (startRow == endRow && startCol == endCol) {
        return {{startRow, startCol}};
    }

    // visited array: -1 = unvisited, otherwise distance from source (not strictly needed but useful for debugging)
    std::vector<std::vector<int>> distance(rows, std::vector<int>(cols, -1));
    // parent array: store predecessor for each visited cell
    std::vector<std::vector<std::pair<int,int>>> parent(
        rows, std::vector<std::pair<int,int>>(cols, {-1, -1})
    );

    // BFS queue
    std::queue<std::pair<int,int>> q;
    distance[startRow][startCol] = 0;
    q.push({startRow, startCol});

    // Direction vectors for 4-neighbor moves
    const int dr[4] = {-1, 1, 0, 0};
    const int dc[4] = {0, 0, -1, 1};

    bool found = false;
    while (!q.empty() && !found) {
        auto [r, c] = q.front();
        q.pop();

        for (int k = 0; k < 4; ++k) {
            int nr = r + dr[k];
            int nc = c + dc[k];

            if (nr < 0 || nr >= rows || nc < 0 || nc >= cols) continue;
            if (grid[nr][nc] != 0) continue;           // obstacle
            if (distance[nr][nc] != -1) continue;      // already visited

            distance[nr][nc] = distance[r][c] + 1;
            parent[nr][nc] = {r, c};
            if (nr == endRow && nc == endCol) {
                found = true;
                break;
            }
            q.push({nr, nc});
        }
    }

    if (!found) return {};  // no path

    // Reconstruct path from target back to source
    std::vector<std::pair<int,int>> path;
    int r = endRow, c = endCol;
    while (!(r == startRow && c == startCol)) {
        path.push_back({r, c});
        auto [pr, pc] = parent[r][c];
        r = pr;
        c = pc;
    }
    path.push_back({startRow, startCol});
    std::reverse(path.begin(), path.end());
    return path;
}

#include <cassert>
#include <vector>
#include <utility>

// The solution function is assumed to be declared above here.

int main() {
    // Simple 3x3 open grid
    std::vector<std::vector<int>> grid1 = {
        {0,0,0},
        {0,0,0},
        {0,0,0}
    };
    auto path1 = findShortestPath(grid1, 0, 0, 2, 2);
    assert(!path1.empty());
    assert(path1.front() == std::make_pair(0,0));
    assert(path1.back() == std::make_pair(2,2));
    assert(path1.size() == 5); // length = Manhattan distance + 1

    // Grid with obstacles requiring a detour
    std::vector<std::vector<int>> grid2 = {
        {0,1,0},
        {0,1,0},
        {0,0,0}
    };
    auto path2 = findShortestPath(grid2, 0, 0, 0, 2);
    assert(!path2.empty());
    assert(path2.front() == std::make_pair(0,0));
    assert(path2.back() == std::make_pair(0,2));
    assert(path2.size() == 5); // goes around the obstacle

    // No path exists (target surrounded by obstacles)
    std::vector<std::vector<int>> grid3 = {
        {0,0,0},
        {0,1,1},
        {0,1,1}
    };
    auto path3 = findShortestPath(grid3, 0, 0, 0, 2); // target (0,2) is fine, but blocking? Actually target is 0,0? Let's use a clear impossible case.
    // Better: target at (2,2) is blocked on all sides except maybe from left, but let's make it fully enclosed.
    std::vector<std::vector<int>> grid3b = {
        {0,1,0},
        {0,1,0},
        {0,1,0}
    };
    // target (0,2) has neighbors: (0,1)=1, (1,2) out of bounds? Actually (1,2) is 0, so it's reachable. Let's create fully enclosed:
    std::vector<std::vector<int>> grid4 = {
        {0,1,0},
        {0,1,0},
        {0,1,0}
    };
    // source (0,0) and target (2,0) – both in same column, but obstacles block? Actually column 0 is open. Let's use a real blockage.
    std::vector<std::vector<int>> grid5 = {
        {0,0,0},
        {0,0,0},
        {0,0,0}
    };
    // No problem. Let's design a fully blocked target:
    std::vector<std::vector<int>> grid6 = {
        {0,0,0},
        {0,1,0},
        {0,0,0}
    };
    // target at (1,1) is obstacle, so invalid input – but our function should return {} because grid[1][1]!=0.
    auto invalidTarget = findShortestPath(grid6, 0, 0, 1, 1);
    assert(invalidTarget.empty());

    // Source equals target
    auto singlePath = findShortestPath(grid1, 1, 1, 1, 1);
    assert(singlePath.size() == 1);
    assert(singlePath[0] == std::make_pair(1,1));

    // Non-square grid (3 rows, 2 columns)
    std::vector<std::vector<int>> grid7 = {
        {0,0},
        {0,1},
        {0,0}
    };
    auto path7 = findShortestPath(grid7, 0, 0, 2, 0);
    assert(!path7.empty());
    assert(path7.size() == 3); // straight down

    // Obstacle blocking source? Should return {} because source is obstacle
    std::vector<std::vector<int>> grid8 = {
        {1,0},
        {0,0}
    };
    auto invalidSource = findShortestPath(grid8, 0, 0, 1, 1);
    assert(invalidSource.empty());

    // Out of bounds target
    auto outOfBounds = findShortestPath(grid1, 0, 0, 3, 3);
    assert(outOfBounds.empty());
}
