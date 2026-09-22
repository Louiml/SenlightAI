Write a C++ function that takes a single positive integer `n` and returns a vector of vectors of integers representing an `(2n-1) × (2n-1)` grid where each cell contains the value `n - distance_from_center` (with distance measured as the Chebyshev distance, i.e., the maximum of the absolute row and column offsets from the center cell). The grid should have the center cell equal to `1`, and values increase by 1 for each step outward in any of the 8 directions, reaching a maximum value of `n` at the border. For example, with `n = 4`, the output is the 7×7 pattern shown in the snippet. The output grid must be stored row-major (each inner vector is a row). Handle `n = 1` correctly (a single cell with value 1). Do not use any hard-coded arrays; compute the values algorithmically.

The core observation is that for a cell at row `i` and column `j` in a `(2n-1)` square grid, its Chebyshev distance from the center cell (located at `(n-1, n-1)`) is `max(|i - (n-1)|, |j - (n-1)|)`. The desired value in that cell is `n - distance`. Directly computing absolute values is straightforward, but the snippet cleverly computes the distance by considering the four distances to the edges: top, bottom, left, right. For any cell, the Chebyshev distance from the center is the minimum of these four edge distances: `min(top, bottom, left, right)`. This works because the center is equidistant from all four edges, and moving outward reduces the distance to at least one edge. Then the cell value is `n - min(top, bottom, left, right)`. Edge cases: `n = 1` produces a 1×1 grid with value `1`. The algorithm runs in O(n²) time because it processes each of the `(2n-1)²` cells exactly once, and uses O(n²) space to store the output grid (plus O(1) auxiliary space aside from the output). The approach is robust and does not require special case handling for different `n` values, as the formula works universally.

#include <vector>
#include <algorithm>

// Generate a (2n-1) x (2n-1) grid where each cell has value n - Chebyshev distance from center.
std::vector<std::vector<int>> generateConcentricGrid(int n) {
    const int size = 2 * n - 1;
    std::vector<std::vector<int>> grid(size, std::vector<int>(size));

    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            const int top = i;
            const int bottom = size - 1 - i;
            const int left = j;
            const int right = size - 1 - j;
            const int distance = std::min(std::min(top, bottom), std::min(left, right));
            grid[i][j] = n - distance;
        }
    }
    return grid;
}

#include <cassert>
#include <vector>

// (The solution function is assumed to be included above)

int main() {
    // Test n = 1 (single cell)
    auto grid1 = generateConcentricGrid(1);
    assert(grid1.size() == 1 && grid1[0].size() == 1);
    assert(grid1[0][0] == 1);

    // Test n = 2 (3x3 grid)
    auto grid2 = generateConcentricGrid(2);
    std::vector<std::vector<int>> expected2 = {
        {2, 2, 2},
        {2, 1, 2},
        {2, 2, 2}
    };
    assert(grid2 == expected2);

    // Test n = 3 (5x5 grid)
    auto grid3 = generateConcentricGrid(3);
    std::vector<std::vector<int>> expected3 = {
        {3, 3, 3, 3, 3},
        {3, 2, 2, 2, 3},
        {3, 2, 1, 2, 3},
        {3, 2, 2, 2, 3},
        {3, 3, 3, 3, 3}
    };
    assert(grid3 == expected3);

    // Test n = 4 (7x7 grid, as given in snippet)
    auto grid4 = generateConcentricGrid(4);
    std::vector<std::vector<int>> expected4 = {
        {4, 4, 4, 4, 4, 4, 4},
        {4, 3, 3, 3, 3, 3, 4},
        {4, 3, 2, 2, 2, 3, 4},
        {4, 3, 2, 1, 2, 3, 4},
        {4, 3, 2, 2, 2, 3, 4},
        {4, 3, 3, 3, 3, 3, 4},
        {4, 4, 4, 4, 4, 4, 4}
    };
    assert(grid4 == expected4);

    // Test symmetry: for n=5, check that center is 1 and border is 5
    auto grid5 = generateConcentricGrid(5);
    int center = (2*5-1) / 2; // == 4
    assert(grid5[center][center] == 1);
    for (int j = 0; j < 9; ++j) {
        assert(grid5[0][j] == 5);      // top row
        assert(grid5[8][j] == 5);      // bottom row
    }
    for (int i = 0; i < 9; ++i) {
        assert(grid5[i][0] == 5);      // left column
        assert(grid5[i][8] == 5);      // right column
    }

    // Test that all values are in [1, n] and increase away from center
    auto grid6 = generateConcentricGrid(6);
    int size6 = 2*6 - 1;
    for (int i = 0; i < size6; ++i) {
        for (int j = 0; j < size6; ++j) {
            assert(grid6[i][j] >= 1 && grid6[i][j] <= 6);
        }
    }
}
