// Write a C++ function `int maxSubmatrixSum(const std::vector<std::vector<int>>& matrix)` that takes a square matrix of integers (dimension \(n \times n\), where \(1 \le n \le 100\)) and returns the maximum sum of any contiguous rectangular submatrix (i.e., a submatrix defined by a contiguous range of rows and contiguous range of columns). The matrix may contain negative numbers, and the answer must be the maximum over all possible non-empty submatrices. For example, if the matrix is `{{1,2,3},{4,5,6},{7,8,9}}`, the maximum sum is the entire matrix sum `45`; if the matrix is `{{-1,-2},{-3,-4}}`, the maximum is `-1` (the largest single element). The function should handle all edge cases, including all-negative matrices, single-row or single-column matrices, and varying matrix sizes.
The problem is a classic 2D maximum subarray sum (Kadane’s algorithm extended). The key observation is to reduce the 2D problem to 1D by fixing a pair of rows (top and bottom) and collapsing the columns between them into a 1D array where each element is the sum of the column values in that row range. Then, apply Kadane’s algorithm to this 1D array to find the maximum sum of a contiguous subarray, which corresponds to the best submatrix with those row bounds. Iterate over all possible top and bottom row indices (including the case where the submatrix is a single row, i.e., top==bottom), and for each pair, compute the column sums efficiently using a prefix sum over rows: precompute `colPrefix[row][col]` so that the sum of column `col` from row `top` to `bottom` is `colPrefix[bottom+1][col] - colPrefix[top][col]`. Then run Kadane’s on that 1D array, updating the global maximum. Edge cases: if the matrix contains all negative numbers, Kadane’s will naturally pick the largest single element, because we initialize the current sum to `0`? Actually, we need to be careful: the classic Kadane’s for at least one element uses `currentSum = max(element, currentSum + element)`, which works for all-negative arrays and returns the maximum element. So the algorithm is correct for all-negative matrices. Complexity: There are \(O(n^2)\) row pairs, and for each pair we run Kadane in \(O(n)\), so total time is \(O(n^3)\), which is fine for \(n \le 100\). Space complexity is \(O(n^2)\) for the prefix sum matrix, or we can compute column sums on the fly in \(O(n^2)\) preprocessing and \(O(n)\) extra array per row pair.
#include <vector>
#include <algorithm>
#include <climits>

// Returns the maximum sum of any contiguous rectangular submatrix.
int maxSubmatrixSum(const std::vector<std::vector<int>>& matrix) {
    int n = (int)matrix.size();
    if (n == 0) return 0;
    int m = (int)matrix[0].size();
    if (m == 0) return 0;

    // Prefix sums per column: prefix[i][j] = sum of matrix[0..i-1][j]
    std::vector<std::vector<int>> prefix(n + 1, std::vector<int>(m, 0));
    for (int i = 1; i <= n; ++i) {
        for (int j = 0; j < m; ++j) {
            prefix[i][j] = prefix[i - 1][j] + matrix[i - 1][j];
        }
    }

    int best = INT_MIN;
    // Iterate over all pairs of rows (top, bottom)
    for (int top = 0; top < n; ++top) {
        for (int bottom = top; bottom < n; ++bottom) {
            // Build 1D array: colSum[j] = sum of matrix[top..bottom][j]
            std::vector<int> colSum(m);
            for (int j = 0; j < m; ++j) {
                colSum[j] = prefix[bottom + 1][j] - prefix[top][j];
            }
            // Kadane's algorithm on colSum
            int current = colSum[0];
            int localBest = colSum[0];
            for (int j = 1; j < m; ++j) {
                current = std::max(colSum[j], current + colSum[j]);
                localBest = std::max(localBest, current);
            }
            best = std::max(best, localBest);
        }
    }
    return best;
}
#include <cassert>
#include <vector>

// The function under test is declared above (maxSubmatrixSum).

int main() {
    // Single element
    assert(maxSubmatrixSum({{5}}) == 5);
    assert(maxSubmatrixSum({{-7}}) == -7);

    // Single row
    assert(maxSubmatrixSum({{1, -2, 3, 4}}) == 7); // subarray [3,4] or [1,-2,3,4]? actually max is 7 from 3+4
    assert(maxSubmatrixSum({{-1, -2, -3}}) == -1);

    // Single column
    assert(maxSubmatrixSum({{1}, {-2}, {3}}) == 3);
    assert(maxSubmatrixSum({{-1}, {-2}, {-3}}) == -1);

    // 2x2 all positive
    std::vector<std::vector<int>> m1 = {{1, 2}, {3, 4}};
    assert(maxSubmatrixSum(m1) == 10);

    // 2x2 all negative
    std::vector<std::vector<int>> m2 = {{-1, -2}, {-3, -4}};
    assert(maxSubmatrixSum(m2) == -1);

    // 3x3 mixed
    std::vector<std::vector<int>> m3 = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };
    assert(maxSubmatrixSum(m3) == 45);

    // 3x3 with negative numbers
    std::vector<std::vector<int>> m4 = {
        { -1, -2, -3},
        { -4,  5, -6},
        { -7, -8,  9}
    };
    // Max submatrix? The single 9 is 9, but 5+9+(-8) = 6, etc. Actually entire matrix sum = -17? Wait let's compute: -1-2-3=-6, -4+5-6=-5, -7-8+9=-6, total=-17. Best single is 9, but maybe 5+9? No, rectangles: row2 col2 = 5, row3 col3=9, row2-col2 to row3-col3 = 5+(-8)+9=6, row2-col2 alone=5, max is 9? Actually 5+9=14? No they are not adjacent in a rectangle. So max is 9. Let's test.
    assert(maxSubmatrixSum(m4) == 9);

    // Submatrix that is a single row and multi columns
    std::vector<std::vector<int>> m5 = {{2, -1, 3, -2, 4}};
    assert(maxSubmatrixSum(m5) == 6); // 2-1+3-2+4 = 6, or 3-2+4=5, so max=6

    return 0;
}
