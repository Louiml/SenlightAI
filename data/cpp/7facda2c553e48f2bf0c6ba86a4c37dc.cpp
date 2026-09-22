// Write a C++ function `bool findPath(const std::vector<std::vector<bool>>& grid, std::vector<std::pair<int,int>>& path)` that determines whether a robot can travel from the top-left cell `(0,0)` to the bottom-right cell `(rows-1, cols-1)` in a rectangular boolean grid. The robot may only move one cell to the right or one cell down. A cell with value `true` is passable, `false` is blocked. The function must return `true` if a path exists and fill `path` (in order from start to end) with the coordinates of the cells visited. Implement the solution using dynamic programming with memoization of failed cells to avoid exponential recomputation, and ensure the function handles empty grids, single‑cell grids, grids where the start or end is blocked, and rectangular grids with varying row/column lengths. The path vector should be empty if no path exists.
The problem asks for a path from the top‑left to bottom‑right in a grid with only right/down moves. A natural recursive approach tries from the target cell (`rows-1`, `cols-1`) and explores going up or left, but the provided snippet works from the bottom‑right backwards. For a forward solution (start to end), we could also use recursion with memoization. Here, we adopt the backwards recursion with a `std::set` (or `std::unordered_set`) to cache failed coordinates, as in the snippet. Key points:
- Base cases: If a coordinate is out of bounds or the cell is blocked (`false`), return `false`.
- If we reach `(0,0)`, success. Otherwise, try moving left (`col-1`) or up (`row-1`) first; if either yields a path, add the current coordinate to the path and return `true`.
- To avoid revisiting failed states, store coordinates that have already been determined to lead to failure in a set. When a coordinate is encountered again, return `false` immediately.
- Edge cases: Empty grid (0 rows or 0 columns) → return `false`. Single‑cell grid: if the cell is `true`, path contains just that cell; else `false`. Start or end blocked → `false` immediately.
- Complexity: With memoization, each cell is processed at most once, leading to O(rows × cols) time and O(rows × cols) space for the memo set plus the recursion stack depth O(rows+cols).
- The solution uses `const` references for the grid to avoid copies and passes the path vector by reference to fill it.
#include <vector>
#include <set>
#include <utility> // for std::pair

// Recursive helper that tries to find a path from (row,col) back to (0,0)
// Returns true if a path exists, and appends the coordinates (in reverse order) to 'path'.
bool findPathHelper(const std::vector<std::vector<bool>>& grid,
                    int row, int col,
                    std::vector<std::pair<int,int>>& path,
                    std::set<std::pair<int,int>>& failedPoints) {
    // Out of bounds or blocked cell
    if (row < 0 || col < 0 || !grid[row][col]) {
        return false;
    }

    std::pair<int,int> current = {row, col};

    // If we already know this cell leads to failure, skip
    if (failedPoints.count(current)) {
        return false;
    }

    // If we've reached the origin, success
    if (row == 0 && col == 0) {
        path.push_back(current);
        return true;
    }

    // Try moving left (col-1) first, then up (row-1)
    if (findPathHelper(grid, row, col - 1, path, failedPoints) ||
        findPathHelper(grid, row - 1, col, path, failedPoints)) {
        path.push_back(current); // append current cell after recursion returns
        return true;
    }

    // No path from here; cache this failure
    failedPoints.insert(current);
    return false;
}

// Public function: returns true if a path from (0,0) to (rows-1,cols-1) exists.
// If true, 'path' is filled with coordinates in order from start to end.
bool findPath(const std::vector<std::vector<bool>>& grid,
              std::vector<std::pair<int,int>>& path) {
    path.clear();
    if (grid.empty() || grid[0].empty()) return false;

    int rows = static_cast<int>(grid.size());
    int cols = static_cast<int>(grid[0].size());

    // Start or end blocked
    if (!grid[0][0] || !grid[rows-1][cols-1]) return false;

    std::set<std::pair<int,int>> failedPoints;
    if (!findPathHelper(grid, rows - 1, cols - 1, path, failedPoints)) {
        return false;
    }

    // The helper builds the path in reverse order (from end to start)
    std::reverse(path.begin(), path.end());
    return true;
}
#include <cassert>
#include <vector>
#include <utility>

int main() {
    // Test 1: Simple 2x2 grid, path exists
    {
        std::vector<std::vector<bool>> grid = {{true, true}, {true, true}};
        std::vector<std::pair<int,int>> path;
        assert(findPath(grid, path) == true);
        assert(path.size() == 3);
        assert(path[0] == std::make_pair(0,0));
        assert(path[path.size()-1] == std::make_pair(1,1));
    }

    // Test 2: Blocked end
    {
        std::vector<std::vector<bool>> grid = {{true, true}, {true, false}};
        std::vector<std::pair<int,int>> path;
        assert(findPath(grid, path) == false);
        assert(path.empty());
    }

    // Test 3: Blocked start
    {
        std::vector<std::vector<bool>> grid = {{false, true}, {true, true}};
        std::vector<std::pair<int,int>> path;
        assert(findPath(grid, path) == false);
        assert(path.empty());
    }

    // Test 4: Single cell passable
    {
        std::vector<std::vector<bool>> grid = {{true}};
        std::vector<std::pair<int,int>> path;
        assert(findPath(grid, path) == true);
        assert(path.size() == 1);
        assert(path[0] == std::make_pair(0,0));
    }

    // Test 5: Single cell blocked
    {
        std::vector<std::vector<bool>> grid = {{false}};
        std::vector<std::pair<int,int>> path;
        assert(findPath(grid, path) == false);
        assert(path.empty());
    }

    // Test 6: Empty grid
    {
        std::vector<std::vector<bool>> grid;
        std::vector<std::pair<int,int>> path;
        assert(findPath(grid, path) == false);
        assert(path.empty());
    }

    // Test 7: Path must go around obstacles (3x3)
    {
        std::vector<std::vector<bool>> grid = {
            {true, false, true},
            {true, false, true},
            {true, true, true}
        };
        std::vector<std::pair<int,int>> path;
        assert(findPath(grid, path) == true);
        // Expected path: (0,0)->(0,1?) actually (0,1) is blocked, so must go down, down, right, right, up? Let's verify:
        // Valid moves only right/down. From (0,0) go down to (1,0), down to (2,0), right to (2,1), right to (2,2). So path length 5.
        assert(path.size() == 5);
        assert(path[0] == std::make_pair(0,0));
        assert(path[4] == std::make_pair(2,2));
    }

    // Test 8: More complex grid with dead end (forces memoization)
    {
        std::vector<std::vector<bool>> grid = {
            {true, true, false, true},
            {true, true, false, true},
            {true, true, true, true},
            {true, false, true, true}
        };
        std::vector<std::pair<int,int>> path;
        assert(findPath(grid, path) == true);
        assert(path.size() >= 7); // minimum steps = 3+3 = 6 cells? Actually rows=4, cols=4, min steps = 6 moves, 7 cells.
        assert(path.front() == std::make_pair(0,0));
        assert(path.back() == std::make_pair(3,3));
    }

    // Test 9: No path due to full blockage
    {
        std::vector<std::vector<bool>> grid = {
            {true, true},
            {false, true}
        };
        std::vector<std::pair<int,int>> path;
        assert(findPath(grid, path) == false);
        assert(path.empty());
    }

    return 0;
}
