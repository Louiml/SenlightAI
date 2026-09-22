Given a rectangular grid of positive integers, write a C++ function that finds the maximum sum of a \(2 \times 2\) subgrid (contiguous block of exactly two rows and two columns). The function should take a `std::vector<std::vector<int>>` as input and return the maximum sum as an `int`. The grid will have at least one row and one column, but may have fewer than 2 rows or fewer than 2 columns; in such cases, the function should return `0`. The input grid is not necessarily square, and all values are positive integers. The function must be named `maxSubgridSum` and must not modify the input grid.

// The main algorithm is a direct traversal of all possible \(2 \times 2\) subgrids. For a grid with \(R\) rows and \(C\) columns, there are \((R-1) \times (C-1)\) such subgrids. For each top-left corner \((i, j)\) where \(0 \le i \le R-2\) and \(0 \le j \le C-2\), compute the sum of the four cells: \(grid[i][j] + grid[i][j+1] + grid[i+1][j] + grid[i+1][j+1]\). Track the maximum among all these sums. The edge cases: if the grid has fewer than 2 rows or fewer than 2 columns, no \(2 \times 2\) subgrid exists, so return `0`. Since all numbers are positive, no negative-handling is needed. Time complexity is \(O(R \cdot C)\) in the worst case because we iterate over every cell once (as part of a subgrid). Auxiliary space is \(O(1)\) because we only store a few integers. The input vector is passed by const reference to guarantee no modification, and we use `std::size_t` for indices to avoid signed/unsigned warnings.

#include <vector>
#include <algorithm>

// Returns the maximum sum of any 2x2 subgrid in the given grid.
// If the grid has fewer than 2 rows or fewer than 2 columns, returns 0.
int maxSubgridSum(const std::vector<std::vector<int>>& grid) {
    const std::size_t rows = grid.size();
    if (rows < 2) {
        return 0;
    }
    
    const std::size_t cols = grid[0].size();
    if (cols < 2) {
        return 0;
    }
    
    int maxSum = 0;  // All numbers are positive, so 0 is a safe initializer.
    for (std::size_t i = 0; i + 1 < rows; ++i) {
        for (std::size_t j = 0; j + 1 < cols; ++j) {
            const int currentSum = grid[i][j] + grid[i][j + 1] +
                                   grid[i + 1][j] + grid[i + 1][j + 1];
            maxSum = std::max(maxSum, currentSum);
        }
    }
    return maxSum;
}

#include <cassert>
#include <vector>

int maxSubgridSum(const std::vector<std::vector<int>>& grid); // declaration for linking

int main() {
    // Test 1: Basic 2x2 grid
    std::vector<std::vector<int>> grid1 = {{1, 2}, {3, 4}};
    assert(maxSubgridSum(grid1) == 10);

    // Test 2: Larger grid with known maximum
    std::vector<std::vector<int>> grid2 = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };
    // Subgrid at (1,1) gives 5+6+8+9 = 28
    assert(maxSubgridSum(grid2) == 28);

    // Test 3: Single row -> returns 0
    std::vector<std::vector<int>> grid3 = {{1, 2, 3}};
    assert(maxSubgridSum(grid3) == 0);

    // Test 4: Single column -> returns 0
    std::vector<std::vector<int>> grid4 = {{1}, {2}, {3}};
    assert(maxSubgridSum(grid4) == 0);

    // Test 5: Rectangular grid, maximum is not at bottom-right
    std::vector<std::vector<int>> grid5 = {
        {1, 1, 1, 1},
        {1, 100, 100, 1},
        {1, 100, 100, 1},
        {1, 1, 1, 1}
    };
    // Center 2x2 subgrid sums to 400
    assert(maxSubgridSum(grid5) == 400);

    // Test 6: All same values
    std::vector<std::vector<int>> grid6 = {
        {5, 5, 5},
        {5, 5, 5},
        {5, 5, 5}
    };
    assert(maxSubgridSum(grid6) == 20);

    // Test 7: 2x3 grid
    std::vector<std::vector<int>> grid7 = {
        {1, 2, 3},
        {4, 5, 6}
    };
    // Subgrids: (0,0):1+2+4+5=12, (0,1):2+3+5+6=16
    assert(maxSubgridSum(grid7) == 16);
}
