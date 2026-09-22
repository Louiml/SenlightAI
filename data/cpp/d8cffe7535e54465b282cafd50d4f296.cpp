/*
Given a square matrix of integers with at least one row, write a C++ function `int minFallingPathSum(const std::vector<std::vector<int>>& matrix)` that returns the minimum sum of a "falling path" from the top row to the bottom row. A falling path starts at any element in the first row, and each step moves down to the next row, but you cannot stay in the same column; choose from any of the other columns in that row. The path must end at the bottom row. The input matrix may contain negative numbers, zeroes, and duplicates. The function should compute this without modifying the input matrix (i.e., treat it as const). For a matrix with one row, the answer is simply the minimum element in that row.
*/
#include <vector>
#include <algorithm>
#include <climits>

// Returns the minimum falling path sum from top to bottom.
// A path moves down one row at a time, never staying in the same column.
int minFallingPathSum(const std::vector<std::vector<int>>& matrix) {
    int rows = matrix.size();
    int cols = matrix[0].size();
    
    if (rows == 1) {
        return *std::min_element(matrix[0].begin(), matrix[0].end());
    }
    
    // For the previous row, track the smallest and second smallest values
    // along with their column indices.
    int prevMin1 = INT_MAX, prevMin2 = INT_MAX;
    int prevCol1 = -1, prevCol2 = -1;
    
    // Initialize from the first row
    for (int j = 0; j < cols; ++j) {
        int val = matrix[0][j];
        if (val < prevMin1) {
            prevMin2 = prevMin1;
            prevCol2 = prevCol1;
            prevMin1 = val;
            prevCol1 = j;
        } else if (val < prevMin2) {
            prevMin2 = val;
            prevCol2 = j;
        }
    }
    
    // Process remaining rows
    for (int i = 1; i < rows; ++i) {
        int currMin1 = INT_MAX, currMin2 = INT_MAX;
        int currCol1 = -1, currCol2 = -1;
        
        for (int j = 0; j < cols; ++j) {
            int bestPrev;
            if (j != prevCol1) {
                bestPrev = prevMin1;
            } else {
                bestPrev = prevMin2;
            }
            int curVal = matrix[i][j] + bestPrev;
            
            if (curVal < currMin1) {
                currMin2 = currMin1;
                currCol2 = currCol1;
                currMin1 = curVal;
                currCol1 = j;
            } else if (curVal < currMin2) {
                currMin2 = curVal;
                currCol2 = j;
            }
        }
        prevMin1 = currMin1;
        prevMin2 = currMin2;
        prevCol1 = currCol1;
        prevCol2 = currCol2;
    }
    
    return prevMin1;
}
#include <cassert>
#include <vector>

int main() {
    // Single row: just the minimum
    std::vector<std::vector<int>> m1 = {{5, -1, 3}};
    assert(minFallingPathSum(m1) == -1);
    
    // 2x2 matrix
    std::vector<std::vector<int>> m2 = {{1, 2}, {3, 4}};
    // Paths: (0,0)->(1,1)=1+4=5; (0,1)->(1,0)=2+3=5
    assert(minFallingPathSum(m2) == 5);
    
    // 3x3 classic example
    std::vector<std::vector<int>> m3 = {{2,1,3}, {6,5,4}, {7,8,9}};
    // Paths: 1+4+7=12, 1+4+8=13, 1+4+9=14, 1+5+7=13, 1+5+8=14, 1+5+9=15,
    // 1+6+7=14, 1+6+8=15, 1+6+9=16, 3+4+7=14, 3+4+8=15, 3+4+9=16, etc.
    // Minimum is 12
    assert(minFallingPathSum(m3) == 12);
    
    // All negatives
    std::vector<std::vector<int>> m4 = {{-1, -2}, {-3, -4}};
    // Paths: -1 + -4 = -5; -2 + -3 = -5
    assert(minFallingPathSum(m4) == -5);
    
    // Larger test with duplicates
    std::vector<std::vector<int>> m5 = {{1, 1, 1}, {1, 1, 1}, {1, 1, 1}};
    // Any path: 1+1+1 = 3
    assert(minFallingPathSum(m5) == 3);
    
    // Single column, single row (valid)
    std::vector<std::vector<int>> m6 = {{7}};
    assert(minFallingPathSum(m6) == 7);
    
    // Two rows, three columns
    std::vector<std::vector<int>> m7 = {{10, -5, 20}, {30, 5, -10}};
    // Best: -5 + -10 = -15
    assert(minFallingPathSum(m7) == -15);
    
    return 0;
}
// This is a dynamic programming problem where we build the answer row by row. For each cell in the current row, the minimum falling path sum ending at that cell is the cell's value plus the minimum falling path sum from the previous row **excluding** the cell directly above it (same column). A naive approach would, for each cell, scan all columns of the previous row and take the minimum among those not equal to the current column, leading to O(n^3) time. However, we can optimize: for each previous row, we can precompute the smallest and second-smallest values along with their column indices. When processing a current cell, if its column differs from the column of the smallest previous value, add the smallest; otherwise add the second-smallest. This reduces the per-cell decision to O(1), giving total O(n^2) time. Edge cases: a single row (answer is min of that row), a single column (since you cannot stay in the same column, you must always switch; but with only one column, there is no valid path beyond the first row—so this problem implicitly assumes the matrix has at least two columns if more than one row; if not, the path would be impossible, but we can still process it and the algorithm would yield INT_MAX for later rows; however, typical constraints guarantee at least one column, and for single column with multiple rows, it's undefined; we can handle by returning INT_MAX or simply letting it compute). In practice, assume the matrix has at least one column, and for multiple rows, at least two columns. The algorithm uses O(1) extra space beyond the input (we avoid modifying the matrix by reading it as const and using temporary variables). Time: O(rows * cols), space: O(1) auxiliary.
