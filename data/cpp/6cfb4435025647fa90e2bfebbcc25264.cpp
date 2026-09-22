// Write a C++ function `spiralOrder` that takes a non-empty 2D vector of integers (a matrix with at least one row and one column) and returns a 1D vector containing the matrix elements in spiral order: starting from the top-left corner, moving right, then down, then left, then up, and repeating this pattern inward until all elements have been visited. The input matrix may be rectangular (rows and columns can differ), and you must handle cases where the matrix has only one row or only one column without causing out-of-bounds access or repeating elements. Your solution must not modify the input matrix.
#include <cassert>
#include <vector>

// The function declaration is provided here for completeness; 
// in practice, include the solution file.
std::vector<int> spiralOrder(const std::vector<std::vector<int>>& matrix);

int main() {
    // Test 1: 3x3 matrix
    std::vector<std::vector<int>> m1 = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };
    assert(spiralOrder(m1) == std::vector<int>({1, 2, 3, 6, 9, 8, 7, 4, 5}));

    // Test 2: 3x4 matrix (rectangular)
    std::vector<std::vector<int>> m2 = {
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 10, 11, 12}
    };
    assert(spiralOrder(m2) == std::vector<int>({1, 2, 3, 4, 8, 12, 11, 10, 9, 5, 6, 7}));

    // Test 3: Single row (1x4)
    std::vector<std::vector<int>> m3 = {{1, 2, 3, 4}};
    assert(spiralOrder(m3) == std::vector<int>({1, 2, 3, 4}));

    // Test 4: Single column (4x1)
    std::vector<std::vector<int>> m4 = {{1}, {2}, {3}, {4}};
    assert(spiralOrder(m4) == std::vector<int>({1, 2, 3, 4}));

    // Test 5: 1x1 matrix
    std::vector<std::vector<int>> m5 = {{42}};
    assert(spiralOrder(m5) == std::vector<int>({42}));

    // Test 6: 2x3 matrix
    std::vector<std::vector<int>> m6 = {
        {1, 2, 3},
        {4, 5, 6}
    };
    assert(spiralOrder(m6) == std::vector<int>({1, 2, 3, 6, 5, 4}));

    // Test 7: 3x2 matrix (tall rectangle)
    std::vector<std::vector<int>> m7 = {
        {1, 2},
        {3, 4},
        {5, 6}
    };
    assert(spiralOrder(m7) == std::vector<int>({1, 2, 4, 6, 5, 3}));

    // Test 8: Empty matrix (should return empty vector)
    std::vector<std::vector<int>> m8;
    assert(spiralOrder(m8).empty());

    return 0;
}
#include <vector>

// Return the elements of a matrix in spiral order without modifying the input.
std::vector<int> spiralOrder(const std::vector<std::vector<int>>& matrix) {
    std::vector<int> result;
    if (matrix.empty() || matrix[0].empty()) {
        return result;
    }

    int top = 0;
    int bottom = static_cast<int>(matrix.size()) - 1;
    int left = 0;
    int right = static_cast<int>(matrix[0].size()) - 1;

    while (top <= bottom && left <= right) {
        // Traverse from left to right along the top row.
        for (int col = left; col <= right; ++col) {
            result.push_back(matrix[top][col]);
        }
        ++top;

        // Traverse from top to bottom along the right column.
        for (int row = top; row <= bottom; ++row) {
            result.push_back(matrix[row][right]);
        }
        --right;

        // Traverse from right to left along the bottom row (if still valid).
        if (top <= bottom) {
            for (int col = right; col >= left; --col) {
                result.push_back(matrix[bottom][col]);
            }
            --bottom;
        }

        // Traverse from bottom to top along the left column (if still valid).
        if (left <= right) {
            for (int row = bottom; row >= top; --row) {
                result.push_back(matrix[row][left]);
            }
            ++left;
        }
    }

    return result;
}
// The standard approach is to simulate the spiral traversal by maintaining four boundaries: top row index (`top`), bottom row index (`bottom`), left column index (`left`), and right column index (`right`). At each step, traverse the current boundary in the correct direction, append elements to the result, and then shrink the boundary inward. The traversal continues while the result size is less than the total number of elements (`rows * columns`). The code snippet provided uses a direction-based state machine, but an even cleaner and more robust method is to process each of the four sides in a loop, checking after each side whether the boundaries have crossed to prevent duplicates, especially for single-row or single-column matrices. Time complexity is O(N) where N is the total number of elements, since each element is visited exactly once. Space complexity is O(N) for the output vector, and O(1) auxiliary space beyond that (ignoring the output storage). Edge cases include an empty matrix (though the problem statement guarantees non-empty, it is safe to return an empty vector if rows are zero), a single row (which only needs the left-to-right pass), a single column (which only needs the top-to-bottom pass), and matrices with odd dimensions where the last element is in the center.
