// Write a C++ function `int minimumReplacementMoves(int n, int m, const std::vector<std::vector<int>>& grid)` that takes the dimensions of an `n` by `m` grid (both `n` and `m` at least 1) and a 2D vector of integers (each cell containing either 0 or 1). The function must return the minimum number of moves required to make all cells equal to 0, where in one move you can select any connected component (connected via 4-directional adjacency) of cells containing 1 and flip all those cells to 0. If the grid already has no 1s, return 0. However, there is a special rule: if any 1 is located on the border of the grid (first row, last row, first column, or last column), then the required number of moves is exactly 2 (regardless of how many border 1s exist); otherwise, the required number is exactly 4. The function should simply return 2 or 4, not simulate the actual flipping process.

// The problem is intentionally simplified: instead of computing the actual minimum number of moves via graph algorithms (which could be complex), the answer depends only on whether any cell with value 1 lies on the border. If at least one 1 is on the border, the answer is 2. If no 1 is on the border but there is at least one 1 somewhere interior, the answer is 4. If there are no 1s at all, the answer is 0. The logic is derived from the original snippet: that code scans the entire grid and immediately prints "2" if it finds a non‑zero element on the border, otherwise prints "4". However, the original snippet did not handle the case of an all‑zero grid (it would print 4 incorrectly). Our task corrects that: return 0 for an all‑zero grid. Edge cases: empty grid? We assume `n` and `m` are at least 1, but the vector could be empty. If `grid` is empty, treat it as all zeros and return 0. Also handle `n` or `m` being 1 (then every cell is on the border, so any 1 yields 2). Time complexity is O(n*m) because we scan all cells once. Space complexity is O(1) extra (excluding input storage).

#include <vector>

// Return the minimum number of moves to clear all 1s, given the border rule.
// Returns 0 if grid has no 1s, 2 if any 1 is on the border, otherwise 4.
int minimumReplacementMoves(int n, int m, const std::vector<std::vector<int>>& grid) {
    // Handle empty grid case: treat as no 1s.
    if (grid.empty() || n == 0 || m == 0) {
        return 0;
    }

    bool hasOne = false;
    bool hasBorderOne = false;

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (grid[i][j] == 1) {
                hasOne = true;
                if (i == 0 || i == n - 1 || j == 0 || j == m - 1) {
                    hasBorderOne = true;
                }
            }
        }
    }

    if (!hasOne) {
        return 0;
    }
    if (hasBorderOne) {
        return 2;
    }
    return 4;
}

#include <cassert>
#include <vector>

// Declare the function to test (provided in solution).
int minimumReplacementMoves(int n, int m, const std::vector<std::vector<int>>& grid);

int main() {
    // All zeros -> 0
    assert(minimumReplacementMoves(2, 2, {{0,0},{0,0}}) == 0);
    // Empty grid -> 0
    assert(minimumReplacementMoves(0, 0, {}) == 0);

    // Border 1 in corner -> 2
    assert(minimumReplacementMoves(3, 3, {{1,0,0},{0,0,0},{0,0,0}}) == 2);
    // Border 1 on edge -> 2
    assert(minimumReplacementMoves(3, 3, {{0,0,0},{0,0,0},{0,1,0}}) == 2);
    // Only interior 1s -> 4
    assert(minimumReplacementMoves(3, 3, {{0,0,0},{0,1,0},{0,0,0}}) == 4);
    // Multiple interior 1s -> 4
    assert(minimumReplacementMoves(4, 4, {{0,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,0}}) == 4);

    // Single row grid: any 1 is border -> 2
    assert(minimumReplacementMoves(1, 5, {{0,1,0,0,0}}) == 2);
    // Single column grid: any 1 is border -> 2
    assert(minimumReplacementMoves(5, 1, {{0},{1},{0},{0},{0}}) == 2);

    // Mixed border and interior -> 2
    assert(minimumReplacementMoves(3, 3, {{0,0,0},{0,1,1},{0,0,0}}) == 4); // both interior
    assert(minimumReplacementMoves(3, 3, {{0,0,1},{0,1,0},{0,0,0}}) == 2); // has border 1

    // Large grid with only interior 1s far from border -> 4
    assert(minimumReplacementMoves(5, 5, {{0,0,0,0,0},{0,1,0,0,0},{0,0,0,1,0},{0,0,0,0,0},{0,0,0,0,0}}) == 4);

    return 0;
}
