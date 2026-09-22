// Write a C++ function `spiralOrder` that takes a non-empty 2D vector of integers (matrix) and returns a vector containing all elements in spiral order, starting from the top-left corner and moving right, then down, then left, then up, repeating this pattern inward until all elements are collected. The matrix may be rectangular (not necessarily square), and may have one or more rows and columns. The function must handle cases where the input matrix is a single row, a single column, or a 1×1 matrix correctly, and must not modify the original matrix.
#include <cassert>
#include <vector>

int main() {
    // 3x3 matrix
    std::vector<std::vector<int>> m1 = {{1,2,3},{4,5,6},{7,8,9}};
    std::vector<int> r1 = spiralOrder(m1);
    assert(r1 == std::vector<int>({1,2,3,6,9,8,7,4,5}));
    
    // 3x4 matrix
    std::vector<std::vector<int>> m2 = {{1,2,3,4},{5,6,7,8},{9,10,11,12}};
    std::vector<int> r2 = spiralOrder(m2);
    assert(r2 == std::vector<int>({1,2,3,4,8,12,11,10,9,5,6,7}));
    
    // 1x1 matrix
    std::vector<std::vector<int>> m3 = {{42}};
    std::vector<int> r3 = spiralOrder(m3);
    assert(r3 == std::vector<int>({42}));
    
    // Single row
    std::vector<std::vector<int>> m4 = {{1,2,3,4}};
    std::vector<int> r4 = spiralOrder(m4);
    assert(r4 == std::vector<int>({1,2,3,4}));
    
    // Single column
    std::vector<std::vector<int>> m5 = {{1},{2},{3}};
    std::vector<int> r5 = spiralOrder(m5);
    assert(r5 == std::vector<int>({1,2,3}));
    
    // 2x2 matrix
    std::vector<std::vector<int>> m6 = {{1,2},{3,4}};
    std::vector<int> r6 = spiralOrder(m6);
    assert(r6 == std::vector<int>({1,2,4,3}));
    
    // 2x3 matrix
    std::vector<std::vector<int>> m7 = {{1,2,3},{4,5,6}};
    std::vector<int> r7 = spiralOrder(m7);
    assert(r7 == std::vector<int>({1,2,3,6,5,4}));
    
    // 4x1 matrix
    std::vector<std::vector<int>> m8 = {{1},{2},{3},{4}};
    std::vector<int> r8 = spiralOrder(m8);
    assert(r8 == std::vector<int>({1,2,3,4}));
    
    return 0;
}
#include <vector>

// Return all elements of a non-empty matrix in spiral order starting from top-left.
std::vector<int> spiralOrder(const std::vector<std::vector<int>>& matrix) {
    int rows = matrix.size();
    int cols = matrix[0].size();
    int total = rows * cols;
    int count = 0;
    
    int left = 0;
    int right = cols - 1;
    int top = 0;
    int bottom = rows - 1;
    
    std::vector<int> result;
    
    while (count < total) {
        // Traverse top row from left to right
        for (int i = left; i <= right && count < total; ++i) {
            result.push_back(matrix[top][i]);
            ++count;
        }
        ++top;
        
        // Traverse right column from top to bottom
        for (int i = top; i <= bottom && count < total; ++i) {
            result.push_back(matrix[i][right]);
            ++count;
        }
        --right;
        
        // Traverse bottom row from right to left
        for (int i = right; i >= left && count < total; ++i) {
            result.push_back(matrix[bottom][i]);
            ++count;
        }
        --bottom;
        
        // Traverse left column from bottom to top
        for (int i = bottom; i >= top && count < total; ++i) {
            result.push_back(matrix[i][left]);
            ++count;
        }
        ++left;
    }
    
    return result;
}
// The solution uses boundary tracking with four indices: `left`, `right`, `top`, `bottom` representing the current unvisited rectangle. A loop runs until the total number of elements collected equals `m*n`, where `m` is rows and `n` is columns. In each iteration, traverse the top row from left to right, then decrement `top`; traverse the right column from top to bottom, then decrement `right`; traverse the bottom row from right to left, then increment `bottom`; traverse the left column from bottom to top, then increment `left`. After each traversal, check that the count is still less than total to avoid re-reading elements when the matrix is a single row or column. Edge cases include a single row (after first traversal, `top` moves past `bottom`, but subsequent loops are guarded by count check), a single column (similarly), and a 1×1 matrix (just one element). Time complexity is O(m*n) because each element is visited once. Space complexity is O(m*n) for the output vector (excluding input storage), plus O(1) for auxiliary variables.
