/*
Write a C++ function `int maxPathSum(const std::vector<std::vector<int>>& triangle)` that takes a triangular grid of non-negative integers (where row `i` contains `i+1` integers, for `i` from 0 to n-1), and returns the maximum possible sum of a path from the top of the triangle (element at row 0, column 0) to any element in the bottom row. At each step, from position `(i, j)` you may move down to `(i+1, j)` (directly below) or `(i+1, j+1)` (diagonally down-right). The input triangle is guaranteed to have at least one row. The function must not modify the input and must handle empty input gracefully (return 0 if the triangle is empty).
*/

#include <vector>
#include <algorithm>

// Returns the maximum sum of a path from the top to the bottom of a triangular grid.
// The triangle is a vector of rows, where row i has i+1 elements.
// The function does not modify the input and returns 0 for an empty triangle.
int maxPathSum(const std::vector<std::vector<int>>& triangle) {
    if (triangle.empty()) {
        return 0;
    }

    // Work on a mutable copy to avoid modifying the input.
    std::vector<std::vector<int>> dp = triangle;

    // Process rows from top to bottom.
    for (size_t i = 1; i < dp.size(); ++i) {
        for (size_t j = 0; j <= i; ++j) {
            if (j == 0) {
                // Only reachable from directly above.
                dp[i][j] += dp[i-1][j];
            } else if (j == i) {
                // Only reachable from above-left.
                dp[i][j] += dp[i-1][j-1];
            } else {
                // Choose the larger of the two possible predecessors.
                dp[i][j] += std::max(dp[i-1][j], dp[i-1][j-1]);
            }
        }
    }

    // The answer is the maximum value in the last row.
    return *std::max_element(dp.back().begin(), dp.back().end());
}

#include <cassert>
#include <vector>

// The solution function is assumed to be defined above.
int maxPathSum(const std::vector<std::vector<int>>& triangle);

int main() {
    // Single row.
    std::vector<std::vector<int>> t1 = {{5}};
    assert(maxPathSum(t1) == 5);

    // Two rows.
    std::vector<std::vector<int>> t2 = {{3}, {7, 4}};
    assert(maxPathSum(t2) == 10); // 3+7

    // Three rows classic example.
    std::vector<std::vector<int>> t3 = {{3}, {7, 4}, {2, 4, 6}};
    // Paths: 3+7+2=12, 3+7+4=14, 3+4+4=11, 3+4+6=13 -> max 14.
    assert(maxPathSum(t3) == 14);

    // Four rows with larger values.
    std::vector<std::vector<int>> t4 = {{1}, {2, 3}, {1, 5, 1}, {4, 1, 2, 1}};
    // Best path: 1+3+5+2=11 (or 1+3+1+4=9, 1+2+5+1=9, etc.)
    assert(maxPathSum(t4) == 11);

    // All zeros.
    std::vector<std::vector<int>> t5 = {{0}, {0, 0}, {0, 0, 0}};
    assert(maxPathSum(t5) == 0);

    // Empty triangle.
    std::vector<std::vector<int>> t6;
    assert(maxPathSum(t6) == 0);

    // Single column (only left edge).
    std::vector<std::vector<int>> t7 = {{2}, {5}, {1}};
    assert(maxPathSum(t7) == 8); // 2+5+1

    // Larger random-like: triangle with increasing numbers.
    std::vector<std::vector<int>> t8 = {{9}, {3, 8}, {2, 7, 4}, {4, 5, 6, 1}};
    // Best: 9+8+7+6=30
    assert(maxPathSum(t8) == 30);

    // Check input not modified.
    std::vector<std::vector<int>> t9 = {{4}, {1, 2}};
    auto original = t9;
    maxPathSum(t9);
    assert(t9 == original);

    return 0;
}

// The problem is a classic dynamic programming (DP) task on a triangular structure. The key observation is that the optimal path to a cell depends only on the optimal paths to the two cells above it. We process rows top-to-bottom, and for each cell we take the maximum of the two possible predecessor sums. We can do this in-place on a copy of the triangle (to preserve input) or use a 1D rolling array. Simpler: copy the triangle into a mutable 2D vector, then for each row `i` from 1 to n-1, for each column `j` from 0 to i, compute:  
// - If `j==0`: only the direct above `(i-1,0)` is reachable, so `dp[i][j] = original[i][j] + dp[i-1][j]`.  
// - If `j==i`: only the above-left `(i-1, j-1)` is reachable, so `dp[i][j] = original[i][j] + dp[i-1][j-1]`.  
// - Otherwise: `dp[i][j] = original[i][j] + max(dp[i-1][j], dp[i-1][j-1])`.  
// After processing all rows, the answer is the maximum value in the last row. Edge cases: an empty triangle (return 0), a single row (the max is that single element). Time complexity is O(n²) where n is the number of rows, since we visit each element once. Space complexity is O(n²) if we copy the whole triangle, or O(n) if we use a 1D array; the simplest in-place copy uses O(n²).
