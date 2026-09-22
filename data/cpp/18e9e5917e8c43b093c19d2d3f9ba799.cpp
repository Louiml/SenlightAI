Write a C++ function `float farthestReachableInGrid(const std::vector<std::vector<int>>& grid, int startRow, int startCol, const std::vector<std::pair<int,int>>& directions)` that, given a rectangular grid of integers where positive values represent passable cells and non-positive values represent blocked cells, starts at a given cell and repeatedly moves to an adjacent passable cell using only the provided movement directions (each direction is a `{dr, dc}` offset). The function must return the maximum Manhattan distance (sum of absolute row and column differences) from the start cell that can be reached by walking along passable cells, moving only through cells that are strictly closer to the start in terms of Manhattan distance than the previously visited cell (i.e., each move must strictly decrease the Manhattan distance to the start). If the start cell itself is blocked or out of bounds, return `-1.0f`. When multiple adjacent passable cells are available, choose any that satisfies the strict distance decrease. The search terminates when no neighboring passable cell is closer to the start than the current cell. The function must not modify the input grid and must be efficient for grids up to 100×100.

#include <cassert>
#include <vector>
#include <utility>

// Declaration of the function under test
float farthestReachableInGrid(const std::vector<std::vector<int>>& grid,
                              int startRow, int startCol,
                              const std::vector<std::pair<int,int>>& directions);

int main() {
    // 1. Simple 3x3 grid, start at center, move left/up/down/right, all passable
    std::vector<std::vector<int>> g1 = {
        {1,1,1},
        {1,1,1},
        {1,1,1}
    };
    std::vector<std::pair<int,int>> dirs1 = {{-1,0},{1,0},{0,-1},{0,1}};
    assert(farthestReachableInGrid(g1, 1, 1, dirs1) == 2.0f);

    // 2. Start at corner, only one direction moves closer (up/left not available)
    assert(farthestReachableInGrid(g1, 0, 0, dirs1) == 0.0f); // no neighbor is strictly closer

    // 3. Blocked start cell
    std::vector<std::vector<int>> g2 = {
        {0,1},
        {1,1}
    };
    assert(farthestReachableInGrid(g2, 0, 0, dirs1) == -1.0f);

    // 4. Out-of-bounds start
    assert(farthestReachableInGrid(g1, 3, 0, dirs1) == -1.0f);

    // 5. Only diagonal moves allowed; but Manhattan distance never strictly decreases with diagonal? 
    // Test with a path that requires moving both row and col to get closer
    std::vector<std::pair<int,int>> dirs_diag = {{-1,-1},{1,1},{-1,1},{1,-1}};
    // Start at (1,1), all passable, but diagonal does not strictly decrease manhattan distance
    assert(farthestReachableInGrid(g1, 1, 1, dirs_diag) == 0.0f);

    // 6. A path with multiple moves: start at (2,2), can move up to (1,2) then (0,2) etc.
    std::vector<std::vector<int>> g3 = {
        {1,1,1,1},
        {1,0,1,1},
        {1,1,1,1}
    };
    // directions: left/right/up/down, start at (2,0), path: (1,0) dist1, (0,0) dist2 -> stops
    assert(farthestReachableInGrid(g3, 2, 0, dirs1) == 2.0f);

    // 7. Blocked cell blocks a path to a farther cell
    std::vector<std::vector<int>> g4 = {
        {1,1,1},
        {1,0,1},
        {1,1,1}
    };
    // Start at (1,0), can move to (0,0) dist1, then (0,1) dist2, then (0,2) dist3, then (1,2) dist2? 
    // Actually from (0,2) the only closer is (0,1) visited, so stops at dist3.
    assert(farthestReachableInGrid(g4, 1, 0, dirs1) == 3.0f);

    // 8. Single-cell grid
    std::vector<std::vector<int>> g5 = {{1}};
    assert(farthestReachableInGrid(g5, 0, 0, dirs1) == 0.0f);

    // 9. Start not at minimum distance cell but no moves possible
    std::vector<std::vector<int>> g6 = {
        {1,0},
        {0,1}
    };
    assert(farthestReachableInGrid(g6, 0, 0, dirs1) == 0.0f);

    // 10. Larger grid with a path that zigzags
    std::vector<std::vector<int>> g7 = {
        {1,1,1,1,1},
        {1,0,1,0,1},
        {1,1,1,1,1}
    };
    // start at (0,0), can go to (0,1) dist1, (0,2) dist2, (0,3) dist3, (0,4) dist4, 
    // then down to (1,4) dist5 but that is not closer (Manhattan of (1,4) is 5, which is >4). 
    // So stops at dist4.
    assert(farthestReachableInGrid(g7, 0, 0, dirs1) == 4.0f);

    return 0;
}

#include <vector>
#include <utility>
#include <cmath>
#include <cstdlib>

// Returns the maximum Manhattan distance reachable from startCell by moving only
// to adjacent passable cells that are strictly closer to start in Manhattan distance.
// Directions are given as {dr, dc} offsets. Returns -1.0f if the start is invalid.
float farthestReachableInGrid(const std::vector<std::vector<int>>& grid,
                              int startRow, int startCol,
                              const std::vector<std::pair<int,int>>& directions) {
    int rows = static_cast<int>(grid.size());
    if (rows == 0) return -1.0f;
    int cols = static_cast<int>(grid[0].size());

    // Validate start cell
    if (startRow < 0 || startRow >= rows || startCol < 0 || startCol >= cols)
        return -1.0f;
    if (grid[startRow][startCol] <= 0)
        return -1.0f;

    std::vector<std::vector<bool>> visited(rows, std::vector<bool>(cols, false));
    visited[startRow][startCol] = true;

    int curRow = startRow;
    int curCol = startCol;
    int maxDistance = 0; // Manhattan distance from start to current cell

    bool moved = true;
    while (moved) {
        moved = false;
        for (const auto& dir : directions) {
            int nr = curRow + dir.first;
            int nc = curCol + dir.second;
            // Check bounds, passability, strict distance decrease, and not visited
            if (nr >= 0 && nr < rows && nc >= 0 && nc < cols &&
                grid[nr][nc] > 0 && !visited[nr][nc]) {
                int dist = std::abs(nr - startRow) + std::abs(nc - startCol);
                int curDist = std::abs(curRow - startRow) + std::abs(curCol - startCol);
                if (dist < curDist) {
                    // Move to this neighbor
                    visited[nr][nc] = true;
                    curRow = nr;
                    curCol = nc;
                    maxDistance = dist;
                    moved = true;
                    break; // restart neighbor scan from new current cell
                }
            }
        }
    }

    return static_cast<float>(maxDistance);
}

// The problem requires simulating a greedy descent along a Manhattan-distance gradient from a start cell, using a restricted set of movements. We begin by validating the start cell: if it is out of bounds or the grid value at that cell is non-positive, we return `-1.0f`. Otherwise, we initialize a current cell to the start, a `visited` boolean matrix of the same size to prevent revisiting cells (since cycles could occur even with strict distance decrease, we still guard against revisits to avoid infinite loops), and a current maximum distance variable. At each step, we iterate over all given directions, compute the neighbor row and column, and check: (1) the neighbor is within bounds, (2) the grid at the neighbor is positive (passable), (3) the neighbor is strictly closer to the start in Manhattan distance than the current cell, and (4) the neighbor has not been visited. Among all valid neighbors, we select the first one that satisfies these conditions (order of selection does not affect the maximum distance because any valid move still leads to a strictly decreasing distance, and the algorithm terminates when no move is possible; the maximum distance reached is simply the Manhattan distance of the last cell visited along that chosen path). Since we always move strictly closer, the path length is at most the Manhattan distance between start and any reachable cell, which is bounded by `2*(rows+cols)`. After moving to a neighbor, we mark it visited, update the current cell, and update the maximum distance as the Manhattan distance from the start to the new current cell. If no valid neighbor exists, we break and return the current maximum distance (which is at least 0 for a valid start). Because we never revisit and the distance strictly decreases, the number of steps is at most `O(rows+cols)`, and each step scans up to `directions.size()` neighbors, giving overall time complexity `O((rows+cols) * directions.size())`. Space complexity is `O(rows*cols)` for the visited matrix. Edge cases include: start cell being blocked, start out of bounds, no valid directions, or a grid that is entirely blocked except the start (then return 0). Also note that the Manhattan distance is computed as `abs(r - startRow) + abs(c - startCol)`, and we use floating-point return as specified, but the result is naturally an integer distance, so we cast it to `float`.
