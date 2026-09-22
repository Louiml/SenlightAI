/*
Given an `n × m` grid of integers and `q` rectangular queries, write a C++ function `long long rectangleSum(const std::vector<std::vector<int>>& grid, int x1, int y1, int x2, int y2)` that returns the sum of all elements in the axis-aligned rectangle whose top-left corner is at 1-based row `x1` and column `y1`, and bottom-right corner is at row `x2` and column `y2`, where `1 ≤ x1 ≤ x2 ≤ n` and `1 ≤ y1 ≤ y2 ≤ m`. The function must handle grids up to 1000×1000 and up to 100,000 queries efficiently by precomputing a 2D prefix sum matrix once per grid. You may assume the inputs are valid and the grid contains non-negative integers. Do not include the precomputation inside the function; instead, design the solution so that the caller builds the prefix sum structure externally, or have the function accept the prefix sum as an additional parameter. To make it self-contained, the task should provide a helper free function `buildPrefixSum` that takes the grid and returns a `std::vector<std::vector<long long>>` of size `(n+1)×(m+1)` with `prefix[i][j]` equal to the sum of the subgrid from (1,1) to (i,j), and the main query function `rectangleSum` that takes the prefix sum and coordinates. Wrap the query function to be called repeatedly for each query in the test.
*/

#include <vector>
#include <cstddef>

// Build a (n+1) x (m+1) 2D prefix sum table from a 1-based grid.
// grid has dimensions n rows, m columns (indices 0..n-1, 0..m-1).
// The returned table pref has dimensions (n+1) x (m+1), where pref[i][j]
// is the sum of grid[0..i-1][0..j-1].
std::vector<std::vector<long long>> buildPrefixSum(const std::vector<std::vector<int>>& grid) {
    std::size_t n = grid.size();
    std::size_t m = grid[0].size();
    std::vector<std::vector<long long>> pref(n + 1, std::vector<long long>(m + 1, 0));
    for (std::size_t i = 1; i <= n; ++i) {
        for (std::size_t j = 1; j <= m; ++j) {
            pref[i][j] = pref[i-1][j] + pref[i][j-1] - pref[i-1][j-1] + grid[i-1][j-1];
        }
    }
    return pref;
}

// Return the sum of the rectangle with top-left corner (x1, y1) and
// bottom-right corner (x2, y2) using 1-based coordinates, given the
// prefix sum table built by buildPrefixSum.
long long rectangleSum(const std::vector<std::vector<long long>>& pref, int x1, int y1, int x2, int y2) {
    return pref[x2][y2] - pref[x1-1][y2] - pref[x2][y1-1] + pref[x1-1][y1-1];
}

#include <cassert>
#include <vector>

int main() {
    // Test 1: 3x3 grid
    std::vector<std::vector<int>> grid1 = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };
    auto pref1 = buildPrefixSum(grid1);
    assert(rectangleSum(pref1, 1, 1, 3, 3) == 45);  // entire grid
    assert(rectangleSum(pref1, 2, 2, 3, 3) == 28);  // 5+6+8+9
    assert(rectangleSum(pref1, 1, 2, 2, 3) == 16); // 2+3+5+6
    assert(rectangleSum(pref1, 1, 1, 1, 1) == 1);  // single element
    assert(rectangleSum(pref1, 3, 3, 3, 3) == 9);  // bottom-right corner

    // Test 2: Single row
    std::vector<std::vector<int>> grid2 = {{10, 20, 30, 40}};
    auto pref2 = buildPrefixSum(grid2);
    assert(rectangleSum(pref2, 1, 1, 1, 4) == 100);
    assert(rectangleSum(pref2, 1, 2, 1, 3) == 50);

    // Test 3: Single column
    std::vector<std::vector<int>> grid3 = {{5}, {15}, {25}};
    auto pref3 = buildPrefixSum(grid3);
    assert(rectangleSum(pref3, 1, 1, 3, 1) == 45);
    assert(rectangleSum(pref3, 2, 1, 2, 1) == 15);

    // Test 4: All zeros
    std::vector<std::vector<int>> grid4 = {{0, 0}, {0, 0}};
    auto pref4 = buildPrefixSum(grid4);
    assert(rectangleSum(pref4, 1, 1, 2, 2) == 0);
    assert(rectangleSum(pref4, 1, 1, 1, 1) == 0);

    // Test 5: Larger values
    std::vector<std::vector<int>> grid5 = {
        {100, 200},
        {300, 400}
    };
    auto pref5 = buildPrefixSum(grid5);
    assert(rectangleSum(pref5, 1, 1, 2, 2) == 1000);
    assert(rectangleSum(pref5, 2, 2, 2, 2) == 400);
    assert(rectangleSum(pref5, 1, 2, 2, 2) == 600);

    return 0;
}

// The core idea is to use a 2D prefix sum (also called integral image). Build a `(n+1)×(m+1)` table `pref` where `pref[i][j] = pref[i-1][j] + pref[i][j-1] - pref[i-1][j-1] + grid[i-1][j-1]` for 1-based indexing (storing grid at `i-1, j-1`). Then a rectangle sum from `(x1,y1)` to `(x2,y2)` is `pref[x2][y2] - pref[x1-1][y2] - pref[x2][y1-1] + pref[x1-1][y1-1]`. This works by inclusion–exclusion: subtract the overlapping top and left parts, then add back the double-subtracted top-left corner. Edge cases include queries starting at row/column 1, which are handled by using index 0 in the prefix table (initialized to 0) so `pref[x1-1][*]` is valid. All values are non-negative so no overflow concerns for the given constraints, but use `long long` to be safe. Time complexity: building the prefix table is `O(n*m)`, each query is `O(1)`. Space is `O(n*m)` for the prefix table.
