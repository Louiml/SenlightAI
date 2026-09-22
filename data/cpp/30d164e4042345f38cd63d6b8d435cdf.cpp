Write a C++ function that, given the number of rows `R`, number of columns `C`, and a starting cell coordinate `(r0, c0)` within a grid of size `R x C` (with valid row indices `0 <= r < R` and column indices `0 <= c < C`), returns a `std::vector<std::vector<int>>` containing the coordinates of all cells in the grid as `{row, column}` pairs, sorted by their Manhattan distance from `(r0, c0)` in ascending order. For cells with equal distance, any relative order among them is acceptable. The function must handle edge cases such as a grid with only one cell, a starting cell at a corner or edge, and grids with large dimensions (up to 100x100) efficiently. The function signature should be `std::vector<std::vector<int>> allCellsDistOrder(int R, int C, int r0, int c0)`. You may assume inputs are valid (0 ≤ r0 < R, 0 ≤ c0 < C) and `R > 0`, `C > 0`.
#include <cassert>
#include <vector>

int main() {
    // Test case 1: Single cell grid
    auto res1 = allCellsDistOrder(1, 1, 0, 0);
    assert(res1.size() == 1 && res1[0] == std::vector<int>{0, 0});

    // Test case 2: Example from problem (R=1, C=2)
    auto res2 = allCellsDistOrder(1, 2, 0, 0);
    assert(res2 == std::vector<std::vector<int>>{{0, 0}, {0, 1}});

    // Test case 3: Example 2 (R=2, C=2, start at (0,1))
    auto res3 = allCellsDistOrder(2, 2, 0, 1);
    // Both distance-1 cells can be in any order; we check set of all coordinates.
    assert(res3.size() == 4);
    // Check that distances are non-decreasing manually
    for (size_t i = 1; i < res3.size(); ++i) {
        int d_prev = std::abs(res3[i-1][0] - 0) + std::abs(res3[i-1][1] - 1);
        int d_cur = std::abs(res3[i][0] - 0) + std::abs(res3[i][1] - 1);
        assert(d_prev <= d_cur);
    }

    // Test case 4: Example 3 (R=2, C=3, start at (1,2)) – check size and distances
    auto res4 = allCellsDistOrder(2, 3, 1, 2);
    assert(res4.size() == 6);
    // Verify all coordinates are present
    std::vector<std::vector<int>> expected_coords = {{0,0},{0,1},{0,2},{1,0},{1,1},{1,2}};
    for (const auto& coord : expected_coords) {
        bool found = false;
        for (const auto& cell : res4) {
            if (cell == coord) { found = true; break; }
        }
        assert(found);
    }

    // Test case 5: Larger grid (R=3, C=3, start at center) – distances from 0 to 4
    auto res5 = allCellsDistOrder(3, 3, 1, 1);
    assert(res5.size() == 9);
    // Check that the first cell is the start, last are distance 4
    assert(res5[0] == std::vector<int>{1, 1});
    assert(std::abs(res5.back()[0] - 1) + std::abs(res5.back()[1] - 1) == 4);

    // Test case 6: Start at corner of 2x3 – no crash, size correct
    auto res6 = allCellsDistOrder(2, 3, 0, 0);
    assert(res6.size() == 6);

    // Test case 7: Grid 3x3, start at (0,2) – check non-decreasing distance
    auto res7 = allCellsDistOrder(3, 3, 0, 2);
    for (size_t i = 1; i < res7.size(); ++i) {
        int d_prev = std::abs(res7[i-1][0] - 0) + std::abs(res7[i-1][1] - 2);
        int d_cur = std::abs(res7[i][0] - 0) + std::abs(res7[i][1] - 2);
        assert(d_prev <= d_cur);
    }

    // Test case 8: Grid 100x100, start at (50,50) – just check size
    auto res8 = allCellsDistOrder(100, 100, 50, 50);
    assert(res8.size() == 10000);

    return 0;
}
#include <vector>
#include <algorithm>
#include <cmath>

// Return all cell coordinates in a R x C grid sorted by Manhattan distance from (r0, c0).
std::vector<std::vector<int>> allCellsDistOrder(int R, int C, int r0, int c0) {
    std::vector<std::vector<int>> cells;
    cells.reserve(R * C);

    // Generate all coordinates with their distance.
    for (int r = 0; r < R; ++r) {
        for (int c = 0; c < C; ++c) {
            int distance = std::abs(r - r0) + std::abs(c - c0);
            cells.push_back({r, c, distance});
        }
    }

    // Sort by distance (third element). Stable sort keeps original order for ties.
    std::stable_sort(cells.begin(), cells.end(),
        [](const std::vector<int>& a, const std::vector<int>& b) {
            return a[2] < b[2];
        });

    // Remove the distance element to return only coordinates.
    for (auto& cell : cells) {
        cell.pop_back();
    }

    return cells;
}
// The standard approach is to generate all cell coordinates, compute their Manhattan distance from the starting cell, and then sort them by that distance. Since the distance formula is `|r - r0| + |c - c0|`, we can iterate over all rows and columns, store each coordinate paired with its distance, and use `std::stable_sort` (or `std::sort` with a custom comparator) to order by distance. Alternatively, a more efficient BFS approach exists that generates cells in increasing distance order without explicit sorting, but for simplicity and given constraints (up to 10,000 cells), sorting is perfectly fine and easy to implement correctly.
//
// Edge cases: 
// - When `R == 1 && C == 1`, the result is just the starting cell.
// - When the starting cell is at a corner, the distances range from 0 to `(R-1) + (C-1)`.
// - Duplicate distances are common; any order among them is valid and the comparator only needs to compare distances.
//
// Time complexity: O(R*C log(R*C)) due to sorting. Space complexity: O(R*C) to store the coordinates. No extra large memory is needed beyond the result vector.
