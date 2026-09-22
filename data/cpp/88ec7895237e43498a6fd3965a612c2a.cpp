// Write a C++ function `std::vector<int> spiralOrder(const std::vector<std::vector<int>>& matrix)` that returns the elements of a 2D matrix in clockwise spiral order, starting from the top-left corner and moving right, then down, then left, then up, continuing inward until all elements are collected. The input matrix is non-empty (has at least one row and one column, but may be rectangular with rows different from columns). The function must work for any valid rectangular matrix, including single-row, single-column, and square matrices. Do not modify the input matrix. The returned vector should contain exactly `rows * columns` integers in spiral order. Handle edge cases like a 1x1 matrix and a matrix with one row or one column correctly.
// The spiral traversal can be implemented using a boundary-based approach that shrinks the traversal limits after each full side is processed. Maintain four integer boundaries: `top` (initial 0), `bottom` (initial rows-1), `left` (initial 0), and `right` (initial columns-1). Repeatedly traverse the top row from left to right, increment `top`; then the right column from top to bottom, decrement `right`; then the bottom row from right to left, decrement `bottom`; then the left column from bottom to top, increment `left`. Continue until the collected count equals the total number of elements, being careful to avoid duplicate elements when the matrix has an odd number of rows or columns. The key edge cases are: a single row (no need to go down/up), a single column (no need to go right/left), and an empty matrix (return empty vector). Time complexity is O(rows × columns) since each element is visited exactly once. Space complexity is O(rows × columns) for the output vector, ignoring input storage.
#include <vector>

std::vector<int> spiralOrder(const std::vector<std::vector<int>>& matrix) {
    if (matrix.empty() || matrix[0].empty()) {
        return {};
    }
    
    int top = 0;
    int bottom = static_cast<int>(matrix.size()) - 1;
    int left = 0;
    int right = static_cast<int>(matrix[0].size()) - 1;
    int total = matrix.size() * matrix[0].size();
    std::vector<int> result;
    result.reserve(total);
    
    while (result.size() < total) {
        // Traverse top row from left to right
        for (int col = left; col <= right && result.size() < total; ++col) {
            result.push_back(matrix[top][col]);
        }
        ++top;
        
        // Traverse right column from top to bottom
        for (int row = top; row <= bottom && result.size() < total; ++row) {
            result.push_back(matrix[row][right]);
        }
        --right;
        
        // Traverse bottom row from right to left
        for (int col = right; col >= left && result.size() < total; --col) {
            result.push_back(matrix[bottom][col]);
        }
        --bottom;
        
        // Traverse left column from bottom to top
        for (int row = bottom; row >= top && result.size() < total; --row) {
            result.push_back(matrix[row][left]);
        }
        ++left;
    }
    
    return result;
}
#include <cassert>
#include <vector>

// Include the solution function here (or link it)

int main() {
    // Test 1: Square 3x3
    std::vector<std::vector<int>> m1 = {{1,2,3},{4,5,6},{7,8,9}};
    std::vector<int> r1 = spiralOrder(m1);
    assert(r1 == std::vector<int>({1,2,3,6,9,8,7,4,5}));
    
    // Test 2: Single row
    std::vector<std::vector<int>> m2 = {{1,2,3,4}};
    assert(spiralOrder(m2) == std::vector<int>({1,2,3,4}));
    
    // Test 3: Single column
    std::vector<std::vector<int>> m3 = {{1},{2},{3}};
    assert(spiralOrder(m3) == std::vector<int>({1,2,3}));
    
    // Test 4: 1x1
    std::vector<std::vector<int>> m4 = {{42}};
    assert(spiralOrder(m4) == std::vector<int>({42}));
    
    // Test 5: Rectangular 2x3
    std::vector<std::vector<int>> m5 = {{1,2,3},{4,5,6}};
    assert(spiralOrder(m5) == std::vector<int>({1,2,3,6,5,4}));
    
    // Test 6: Rectangular 3x2
    std::vector<std::vector<int>> m6 = {{1,2},{3,4},{5,6}};
    assert(spiralOrder(m6) == std::vector<int>({1,2,4,6,5,3}));
    
    // Test 7: 4x4
    std::vector<std::vector<int>> m7 = {{1,2,3,4},{5,6,7,8},{9,10,11,12},{13,14,15,16}};
    std::vector<int> r7 = spiralOrder(m7);
    assert(r7 == std::vector<int>({1,2,3,4,8,12,16,15,14,13,9,5,6,7,11,10}));
    
    // Test 8: Matrix with negative numbers
    std::vector<std::vector<int>> m8 = {{-1,-2},{-3,-4}};
    assert(spiralOrder(m8) == std::vector<int>({-1,-2,-4,-3}));
    
    // Test 9: Large single row with many elements
    std::vector<std::vector<int>> m9 = {{1,2,3,4,5,6,7}};
    assert(spiralOrder(m9) == std::vector<int>({1,2,3,4,5,6,7}));
    
    // Test 10: Empty matrix (zero rows)
    std::vector<std::vector<int>> m10 = {};
    assert(spiralOrder(m10) == std::vector<int>());
    
    return 0;
}
