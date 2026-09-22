Write a C++ function `markValidCells` that takes a 2D grid of booleans (represented as `std::vector<std::vector<bool>>`), a list of query points (each point is a pair of integer coordinates `(x, y)`), and a rectangular axis-aligned bounding box defined by its lower-left corner `(min_x, min_y)` and upper-right corner `(max_x, max_y)`. The function must mark every grid cell `(i, j)` as `true` only if there exists at least one query point whose coordinates fall exactly on that grid cell AND that point lies strictly inside or on the boundary of the bounding box. Points outside the bounding box must be ignored. The grid's rows correspond to y-coordinates (index 0 = min_y) and columns to x-coordinates (index 0 = min_x). The function returns `void` and modifies the grid in place. Assume all coordinates are non-negative integers. If a query point has coordinates that are outside the grid's dimensions, it must be ignored silently.

The solution iterates over each query point once. For each point, it first checks whether the x-coordinate is within the inclusive range `[min_x, max_x]` and the y-coordinate within `[min_y, max_y]`. If not, the point is skipped. Then it verifies that the point lies inside the grid boundaries: `x - min_x` must be a valid column index (>= 0 and < number of columns), and `y - min_y` must be a valid row index (>= 0 and < number of rows). If valid, it sets that grid cell to `true`. A key edge case is when the bounding box extends beyond the grid; those points are ignored even if inside the box. Another edge case is duplicate query points — setting the cell to `true` multiple times has no adverse effect. Time complexity is O(P) where P is the number of query points, and space complexity is O(1) extra apart from the grid modification.

#include <vector>
#include <utility>

// Mark grid cells as true if a query point lies exactly on that cell and within the bounding box.
void markValidCells(std::vector<std::vector<bool>>& grid,
                    const std::vector<std::pair<int, int>>& points,
                    int min_x, int min_y, int max_x, int max_y) {
    if (grid.empty() || grid[0].empty()) return;

    const int rows = static_cast<int>(grid.size());
    const int cols = static_cast<int>(grid[0].size());

    for (const auto& p : points) {
        const int x = p.first;
        const int y = p.second;

        // Ignore points outside the bounding box.
        if (x < min_x || x > max_x || y < min_y || y > max_y) {
            continue;
        }

        // Convert world coordinates to grid indices.
        const int col = x - min_x;
        const int row = y - min_y;

        // Check that indices are within the grid's dimensions.
        if (col >= 0 && col < cols && row >= 0 && row < rows) {
            grid[row][col] = true;
        }
    }
}

#include <cassert>
#include <vector>
#include <utility>

int main() {
    // Test 1: Basic marking within grid and box.
    {
        std::vector<std::vector<bool>> grid(3, std::vector<bool>(3, false));
        std::vector<std::pair<int, int>> points = {{0,0}, {2,2}, {1,1}};
        markValidCells(grid, points, 0, 0, 2, 2);
        assert(grid[0][0] == true);
        assert(grid[2][2] == true);
        assert(grid[1][1] == true);
        assert(grid[0][1] == false);
    }

    // Test 2: Points outside bounding box are ignored.
    {
        std::vector<std::vector<bool>> grid(2, std::vector<bool>(2, false));
        std::vector<std::pair<int, int>> points = {{0,0}, {3,3}, {0,5}};
        markValidCells(grid, points, 1, 1, 2, 2);
        assert(grid[0][0] == false); // (0,0) outside box
        assert(grid[0][1] == false); // (0,5) outside box
        // (3,3) outside box
    }

    // Test 3: Points inside box but outside grid are ignored.
    {
        std::vector<std::vector<bool>> grid(1, std::vector<bool>(1, false));
        std::vector<std::pair<int, int>> points = {{5,5}, {6,5}};
        markValidCells(grid, points, 0, 0, 10, 10);
        assert(grid[0][0] == false); // Both points outside grid dims
    }

    // Test 4: Duplicate points produce same result.
    {
        std::vector<std::vector<bool>> grid(2, std::vector<bool>(2, false));
        std::vector<std::pair<int, int>> points = {{1,1}, {1,1}, {1,1}};
        markValidCells(grid, points, 0, 0, 1, 1);
        assert(grid[1][1] == true);
    }

    // Test 5: Empty grid does not crash.
    {
        std::vector<std::vector<bool>> grid;
        std::vector<std::pair<int, int>> points = {{0,0}};
        markValidCells(grid, points, 0, 0, 1, 1);
        // No assertion needed; just ensure no crash.
    }

    // Test 6: Grid with multiple rows/columns and box smaller than grid.
    {
        std::vector<std::vector<bool>> grid(4, std::vector<bool>(4, false));
        std::vector<std::pair<int, int>> points = {{2,2}, {3,3}, {4,4}};
        markValidCells(grid, points, 1, 1, 3, 3);
        // (2,2) -> row=1, col=1
        assert(grid[1][1] == true);
        // (3,3) -> row=2, col=2
        assert(grid[2][2] == true);
        // (4,4) outside box -> ignored
        assert(grid[3][3] == false);
    }

    // Test 7: Points exactly on box boundary are accepted.
    {
        std::vector<std::vector<bool>> grid(3, std::vector<bool>(3, false));
        std::vector<std::pair<int, int>> points = {{0,0}, {0,2}, {2,0}, {2,2}};
        markValidCells(grid, points, 0, 0, 2, 2);
        assert(grid[0][0] == true);
        assert(grid[0][2] == true);
        assert(grid[2][0] == true);
        assert(grid[2][2] == true);
    }

    // Test 8: Negative coordinates in grid indices? Not allowed per spec, but test with small grid.
    // Ensure function does not access out of bounds when min_x > 0 and points start from 0.
    {
        std::vector<std::vector<bool>> grid(2, std::vector<bool>(2, false));
        std::vector<std::pair<int, int>> points = {{0,0}, {1,1}};
        markValidCells(grid, points, 1, 1, 2, 2);
        // (0,0) outside box; (1,1) inside box -> row=0, col=0
        assert(grid[0][0] == true);
        assert(grid[0][1] == false);
        assert(grid[1][0] == false);
        assert(grid[1][1] == false);
    }

    return 0;
}
