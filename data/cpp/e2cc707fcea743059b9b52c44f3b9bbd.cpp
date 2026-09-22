/*
Write a C++ function `int minPathCost(const std::vector<std::vector<int>>& grid)` that, given a non-empty rectangular 2D grid of non-negative integers, returns the minimum sum of values along any path from the top-left cell `(0,0)` to the bottom-right cell `(rows-1, cols-1)`. You may only move either down or right at each step. The grid is read-only and will contain at least one row and one column. If the grid is empty, the function should return 0. The function must handle all grid sizes efficiently and should not modify the input.
*/

#include <vector>
#include <algorithm>
#include <cstddef>

// Returns the minimum sum along a path from top-left to bottom-right,
// moving only down or right. If grid is empty, returns 0.
int minPathCost(const std::vector<std::vector<int>>& grid) {
    if (grid.empty() || grid[0].empty()) return 0;

    std::size_t rows = grid.size();
    std::size_t cols = grid[0].size();

    // dp[i][j] = minimum cost to reach (i,j) from (0,0)
    std::vector<std::vector<int>> dp(rows, std::vector<int>(cols, 0));

    dp[0][0] = grid[0][0];

    // Fill first column: only possible from above
    for (std::size_t i = 1; i < rows; ++i) {
        dp[i][0] = dp[i-1][0] + grid[i][0];
    }

    // Fill first row: only possible from left
    for (std::size_t j = 1; j < cols; ++j) {
        dp[0][j] = dp[0][j-1] + grid[0][j];
    }

    // Fill rest of the grid
    for (std::size_t i = 1; i < rows; ++i) {
        for (std::size_t j = 1; j < cols; ++j) {
            dp[i][j] = grid[i][j] + std::min(dp[i-1][j], dp[i][j-1]);
        }
    }

    return static_cast<int>(dp[rows-1][cols-1]);
}

#include <cassert>
#include <vector>

int main() {
    // Single cell
    std::vector<std::vector<int>> g1 = {{5}};
    assert(minPathCost(g1) == 5);

    // 1x3 horizontal
    std::vector<std::vector<int>> g2 = {{1, 2, 3}};
    assert(minPathCost(g2) == 6); // 1+2+3

    // 3x1 vertical
    std::vector<std::vector<int>> g3 = {{1}, {2}, {3}};
    assert(minPathCost(g3) == 6);

    // 2x2 with obvious min path
    std::vector<std::vector<int>> g4 = {{1, 2}, {1, 1}};
    assert(minPathCost(g4) == 3); // 1 (down) + 1 (down) + 1 (right) = 3

    // Classic example from problem statement
    std::vector<std::vector<int>> g5 = {{1, 3, 1}, {1, 5, 1}, {4, 2, 1}};
    assert(minPathCost(g5) == 7); // 1→3→1→1→1

    // All zeros
    std::vector<std::vector<int>> g6 = {{0, 0}, {0, 0}};
    assert(minPathCost(g6) == 0);

    // Larger grid with clear min path
    std::vector<std::vector<int>> g7 = {{1, 10, 10}, {1, 10, 10}, {1, 1, 1}};
    assert(minPathCost(g7) == 5); // 1+1+1+1+1

    // Empty grid
    std::vector<std::vector<int>> g8 = {};
    assert(minPathCost(g8) == 0);

    // Single row with large numbers (non-negative)
    std::vector<std::vector<int>> g9 = {{100, 1, 100}};
    assert(minPathCost(g9) == 201); // 100+1+100

    // All same values
    std::vector<std::vector<int>> g10 = {{7, 7}, {7, 7}};
    assert(minPathCost(g10) == 28); // 7+7+7+7

    return 0;
}

// The problem is a classic dynamic programming task on a grid. The optimal substructure is: to reach a cell `(i,j)`, you must come from either `(i-1,j)` (above) or `(i,j-1)` (left). The minimum cost to reach `(i,j)` is the value at that cell plus the minimum of the costs to reach its two possible predecessors. We can solve this bottom-up by filling a DP table of the same size as the grid. For the first row, only leftward moves are possible; for the first column, only upward moves are possible. The base case is the top-left cell, whose cost is its own value. We iterate row by row and column by column. Alternatively, a top-down memoized recursion (as in the snippet) works but uses extra recursion stack; bottom-up is more efficient in practice. Edge cases: a single-cell grid returns that cell’s value; an empty grid returns 0. Time complexity is O(rows × cols), space complexity is O(rows × cols) for the DP table (or O(min(rows, cols)) if using a rolling array, but full table is acceptable). The solution below uses a `const` reference to avoid copying the grid and a 2D vector for DP.
