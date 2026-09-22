/*
Write a C++ function `longestPathInTriangle` that takes a square 2D vector of integers (where all entries below the main diagonal are ignored and may contain arbitrary values) and an integer `n` representing the number of rows in a triangle, and returns the maximum possible sum of numbers from the top of the triangle (row 0, column 0) to any cell in the bottom row (row `n-1`), moving only from a cell `(i, j)` to either `(i+1, j)` or `(i+1, j+1)` (adjacent below-left or below-right). The input triangle is embedded in the first `n` rows and first `j <= i` columns of the 2D vector; entries at `(i, j)` with `j > i` are not part of the triangle and must be ignored. The function must handle `n >= 1`, possibly negative values, and must not modify the input vector.
*/

#include <vector>
#include <algorithm>

// Returns the maximum sum from the top of a triangle to its bottom.
// The triangle is stored in the first n rows and first (i+1) columns of a 2D vector.
// The function does not modify the input and works for n >= 1.
int longestPathInTriangle(const std::vector<std::vector<int>>& triangle, int n) {
    if (n <= 0) return 0;  // defensive, though the problem guarantees n >= 1
    
    // Copy the bottom row's values (only the first n entries are valid)
    std::vector<int> dp(triangle[n-1].begin(), triangle[n-1].begin() + n);
    
    // Process rows from the second-last to the top
    for (int i = n - 2; i >= 0; --i) {
        for (int j = 0; j <= i; ++j) {
            dp[j] = triangle[i][j] + std::max(dp[j], dp[j+1]);
        }
    }
    return dp[0];
}

#include <cassert>
#include <vector>
#include <iostream>

// Declaration of the tested function
int longestPathInTriangle(const std::vector<std::vector<int>>& triangle, int n);

int main() {
    // Basic single element
    {
        std::vector<std::vector<int>> tri = {{5}};
        assert(longestPathInTriangle(tri, 1) == 5);
    }
    // Simple 3-row triangle: 
    //    3
    //   2 4
    //  6 5 7
    // Paths: 3->2->6=11, 3->2->5=10, 3->4->5=12, 3->4->7=14 => max 14
    {
        std::vector<std::vector<int>> tri = {{3}, {2,4}, {6,5,7}};
        assert(longestPathInTriangle(tri, 3) == 14);
    }
    // Negative values: 
    //   -1
    //  -5 -2
    //   -1 -1 -1
    // Paths: -1-5-1=-7, -1-5-1=-7, -1-2-1=-4, -1-2-1=-4 => max -4
    {
        std::vector<std::vector<int>> tri = {{-1}, {-5,-2}, {-1,-1,-1}};
        assert(longestPathInTriangle(tri, 3) == -4);
    }
    // Larger triangle with mixed values:
    //     7
    //    3 8
    //   8 1 0
    //  2 7 4 4
    // Manual check: best path 7->8->0->4 = 19? Actually attempt all:
    // 7-3-8-2=20, 7-3-8-7=25, 7-3-1-7=18, 7-3-1-4=15
    // 7-8-1-7=23, 7-8-1-4=20, 7-8-0-4=19, 7-8-0-4=19
    // Max = 25 from 7-3-8-7
    {
        std::vector<std::vector<int>> tri = {{7}, {3,8}, {8,1,0}, {2,7,4,4}};
        assert(longestPathInTriangle(tri, 4) == 25);
    }
    // Ensure input is not modified
    {
        std::vector<std::vector<int>> tri = {{1}, {2,3}};
        std::vector<std::vector<int>> original = tri;
        longestPathInTriangle(tri, 2);
        assert(tri == original);
    }
    // Triangle with extra ignored entries (j > i) in the 2D vector
    {
        std::vector<std::vector<int>> tri = {{1, 99}, {2, 3, 98}, {4, 5, 6, 97}};
        // Actual triangle rows: row0: [1], row1: [2,3], row2: [4,5,6]
        // Paths: 1-2-4=7, 1-2-5=8, 1-3-5=9, 1-3-6=10 => max 10
        assert(longestPathInTriangle(tri, 3) == 10);
    }
    // Single row with many columns but only first is used
    {
        std::vector<std::vector<int>> tri = {{-10, 1000, -5}};
        assert(longestPathInTriangle(tri, 1) == -10);
    }
    std::cout << "All tests passed!" << std::endl;
    return 0;
}

// This problem is a classic dynamic programming "triangle path maximum sum" problem. The key insight is that for each cell `(i, j)`, the maximum sum from that cell to the bottom is the value at that cell plus the maximum of the maximum sums from its two possible next cells (`(i+1, j)` and `(i+1, j+1)`). We compute bottom-up: starting from the second-last row (`i = n-2`) down to row 0, for each `j` from 0 to `i`, update `dp[i][j] = value[i][j] + max(dp[i+1][j], dp[i+1][j+1])`. The answer is `dp[0][0]`. Base case: the last row's `dp` values are just the triangle values themselves. Edge cases: `n = 1` (returns the single top value), negative numbers (handled naturally by `max` when choosing the better path; note that we must not skip negative paths because the sum includes all cells visited). The bottom row's ignored entries with `j > i` are never accessed in the recurrence because we only iterate `j <= i`. Time complexity is `O(n^2)` because we visit each triangle cell once. Space complexity is `O(1)` auxiliary if we modify a copy, but since we must not modify the input, we can either use a DP table of size `n x n` (extra `O(n^2)` space) or reuse a single row (or copy the input). A clean approach is to create a copy of the triangle values in a local vector and modify that copy, giving `O(n^2)` space. Alternatively, we can keep a single DP row of size `n` and update in place from bottom to top, using `O(n)` space. The reference solution uses an `O(n)` DP array for elegance.
