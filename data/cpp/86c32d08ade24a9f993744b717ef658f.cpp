// Write a C++ function `int maximalRectangleArea(const std::vector<std::vector<char>>& matrix)` that, given a binary matrix (where each cell is either `'0'` or `'1'`), returns the area of the largest rectangle containing only `'1'`s. The matrix may be empty (with zero rows or zero columns), in which case the function must return 0. The matrix is non-modifiable (const). The function must handle arbitrary dimensions, including rows of different lengths? No — assume it is a valid rectangular matrix (all rows have the same length). The solution must be efficient for large matrices (e.g., up to 200×200). The result is an integer, so areas must fit within `int` (but you may assume the input is such that no overflow occurs).

The problem is a classic "maximal rectangle in a binary matrix" problem, which can be reduced to the "largest rectangle in a histogram" problem. For each row of the matrix, we maintain a `heights` array where `heights[j]` equals the number of consecutive `'1'`s ending at the current row in column `j`. Then, for that row, we compute the largest rectangle area in this histogram. The histogram problem itself can be solved in O(cols) using a stack-based approach (monotonic stack) for each row, leading to an overall O(rows * cols) time complexity. Since we only keep one `heights` array of size `cols+1`, the auxiliary space is O(cols). We add an extra element at the end (height 0) to simplify the stack algorithm by ensuring all bars are popped at the end. Edge cases include an empty matrix (return 0), a matrix with only zeros (heights all zero, max area 0), and a single cell. The stack algorithm: for each index `i` from 0 to `cols` (inclusive), we maintain a stack of indices with increasing heights. When the current height is less than the height at the stack top, we pop and compute the area with the popped height as the shortest bar, and the width determined by the current index minus the new top index minus 1. This ensures each bar is pushed and popped once, so O(cols) per row. Time complexity: O(rows * cols), space: O(cols).

#include <vector>
#include <algorithm>
#include <stack>

// Returns the area of the largest rectangle consisting only of '1's in a binary matrix.
// Uses histogram method with a monotonic stack for each row.
int maximalRectangleArea(const std::vector<std::vector<char>>& matrix) {
    if (matrix.empty() || matrix[0].empty()) {
        return 0;
    }
    
    const int rows = static_cast<int>(matrix.size());
    const int cols = static_cast<int>(matrix[0].size());
    
    // heights[i] = consecutive '1's ending at the current row in column i.
    std::vector<int> heights(cols + 1, 0); // extra 0 at the end for easier stack handling
    int maxArea = 0;
    
    for (const auto& row : matrix) {
        // Update heights for this row.
        for (int j = 0; j < cols; ++j) {
            if (row[j] == '1') {
                heights[j] += 1;
            } else {
                heights[j] = 0;
            }
        }
        // Reset the sentinel (last element) to 0.
        heights[cols] = 0;
        
        // Monotonic stack to find largest rectangle in current histogram.
        std::stack<int> st; // stores indices with non-decreasing heights
        for (int i = 0; i <= cols; ++i) {
            while (!st.empty() && heights[i] < heights[st.top()]) {
                int height = heights[st.top()];
                st.pop();
                int left = st.empty() ? -1 : st.top();
                int width = i - left - 1;
                maxArea = std::max(maxArea, height * width);
            }
            st.push(i);
        }
    }
    
    return maxArea;
}

#include <cassert>
#include <vector>

// Assume maximalRectangleArea is defined as above.

int main() {
    // Empty matrix
    std::vector<std::vector<char>> empty;
    assert(maximalRectangleArea(empty) == 0);
    
    // Empty rows
    std::vector<std::vector<char>> emptyRows = {{}};
    assert(maximalRectangleArea(emptyRows) == 0);
    
    // Single '1'
    std::vector<std::vector<char>> single1 = {{'1'}};
    assert(maximalRectangleArea(single1) == 1);
    
    // Single '0'
    std::vector<std::vector<char>> single0 = {{'0'}};
    assert(maximalRectangleArea(single0) == 0);
    
    // All zeros 2x2
    std::vector<std::vector<char>> zeros2x2 = {{'0','0'}, {'0','0'}};
    assert(maximalRectangleArea(zeros2x2) == 0);
    
    // All ones 2x3 -> area = 6
    std::vector<std::vector<char>> allOnes2x3 = {{'1','1','1'}, {'1','1','1'}};
    assert(maximalRectangleArea(allOnes2x3) == 6);
    
    // 4x4 example from LeetCode, expected 6
    std::vector<std::vector<char>> leetCode = {
        {'1','0','1','0','0'},
        {'1','0','1','1','1'},
        {'1','1','1','1','1'},
        {'1','0','0','1','0'}
    };
    assert(maximalRectangleArea(leetCode) == 6);
    
    // Larger rectangle hidden: 3x3 with a 2x2 block
    std::vector<std::vector<char>> block2x2 = {
        {'1','1','0'},
        {'1','1','0'},
        {'0','0','1'}
    };
    assert(maximalRectangleArea(block2x2) == 4);
    
    // Single column with height 5
    std::vector<std::vector<char>> column = {{'1'}, {'1'}, {'1'}, {'1'}, {'1'}};
    assert(maximalRectangleArea(column) == 5);
    
    // Single row with all ones
    std::vector<std::vector<char>> row = {{'1','1','1','1'}};
    assert(maximalRectangleArea(row) == 4);
}
