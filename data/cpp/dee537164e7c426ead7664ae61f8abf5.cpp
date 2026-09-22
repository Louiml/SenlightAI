Write a C++ function that takes a 2D integer matrix represented by a `std::vector<std::vector<int>>` and returns a `std::vector<int>` containing the elements of the matrix in spiral order, starting from the top-left corner and moving clockwise (right, down, left, up, then repeat inward). The matrix is guaranteed to be non-empty and rectangular (all rows have the same number of columns), but it may have any number of rows and columns, including cases where one dimension is 1 (e.g., a single row or single column). The function must handle all such shapes correctly without printing anything to the console.

// The spiral traversal is performed by maintaining four boundary indices: `top`, `bottom`, `left`, and `right`, which define the current unvisited rectangle. Initially, `top = 0`, `bottom = rows-1`, `left = 0`, and `right = cols-1`. The algorithm repeatedly:
// 1. Traverse from `left` to `right` along the top row, then increment `top`.
// 2. Traverse from `top` to `bottom` along the right column, then decrement `right`.
// 3. If `top <= bottom`, traverse from `right` to `left` along the bottom row, then decrement `bottom`.
// 4. If `left <= right`, traverse from `bottom` to `top` along the left column, then increment `left`.
// The loop continues while `top <= bottom` and `left <= right`. For each step, push the current matrix element into the result vector. Important edge cases: a single-row matrix (only step 1 applies, steps 3 and 4 are skipped because `top` exceeds `bottom`), a single-column matrix (only step 1 and step 2 apply, later steps are skipped), and a 1x1 matrix where only one element is visited. Time complexity is \(O(m \times n)\) because every element is visited exactly once, and space complexity is \(O(m \times n)\) for the output vector (excluding the input matrix itself).

#include <vector>

// Return the elements of a non-empty rectangular matrix in clockwise spiral order.
std::vector<int> spiralOrder(const std::vector<std::vector<int>>& matrix) {
    std::vector<int> result;
    int top = 0;
    int bottom = static_cast<int>(matrix.size()) - 1;
    int left = 0;
    int right = static_cast<int>(matrix[0].size()) - 1;

    while (top <= bottom && left <= right) {
        // Traverse right along the top row.
        for (int col = left; col <= right; ++col) {
            result.push_back(matrix[top][col]);
        }
        ++top;

        // Traverse down along the right column.
        for (int row = top; row <= bottom; ++row) {
            result.push_back(matrix[row][right]);
        }
        --right;

        // If there is still a bottom row to traverse, go left.
        if (top <= bottom) {
            for (int col = right; col >= left; --col) {
                result.push_back(matrix[bottom][col]);
            }
            --bottom;
        }

        // If there is still a left column to traverse, go up.
        if (left <= right) {
            for (int row = bottom; row >= top; --row) {
                result.push_back(matrix[row][left]);
            }
            ++left;
        }
    }

    return result;
}

#include <cassert>
#include <vector>

// Include the solution function here (or link it).

int main() {
    // 3x3 matrix
    std::vector<std::vector<int>> m1 = {{1,2,3},{4,5,6},{7,8,9}};
    std::vector<int> r1 = {1,2,3,6,9,8,7,4,5};
    assert(spiralOrder(m1) == r1);

    // 3x4 matrix
    std::vector<std::vector<int>> m2 = {{1,2,3,4},{5,6,7,8},{9,10,11,12}};
    std::vector<int> r2 = {1,2,3,4,8,12,11,10,9,5,6,7};
    assert(spiralOrder(m2) == r2);

    // Single row
    std::vector<std::vector<int>> m3 = {{1,2,3,4,5}};
    std::vector<int> r3 = {1,2,3,4,5};
    assert(spiralOrder(m3) == r3);

    // Single column
    std::vector<std::vector<int>> m4 = {{1},{2},{3},{4}};
    std::vector<int> r4 = {1,2,3,4};
    assert(spiralOrder(m4) == r4);

    // 1x1 matrix
    std::vector<std::vector<int>> m5 = {{7}};
    std::vector<int> r5 = {7};
    assert(spiralOrder(m5) == r5);

    // 2x2 matrix
    std::vector<std::vector<int>> m6 = {{1,2},{3,4}};
    std::vector<int> r6 = {1,2,4,3};
    assert(spiralOrder(m6) == r6);

    // 2x3 matrix
    std::vector<std::vector<int>> m7 = {{1,2,3},{4,5,6}};
    std::vector<int> r7 = {1,2,3,6,5,4};
    assert(spiralOrder(m7) == r7);

    // 3x2 matrix
    std::vector<std::vector<int>> m8 = {{1,2},{3,4},{5,6}};
    std::vector<int> r8 = {1,2,4,6,5,3};
    assert(spiralOrder(m8) == r8);

    // 4x4 matrix
    std::vector<std::vector<int>> m9 = {{1,2,3,4},{5,6,7,8},{9,10,11,12},{13,14,15,16}};
    std::vector<int> r9 = {1,2,3,4,8,12,16,15,14,13,9,5,6,7,11,10};
    assert(spiralOrder(m9) == r9);

    // Large matrix with negative numbers
    std::vector<std::vector<int>> m10 = {{-1,-2},{-3,-4}};
    std::vector<int> r10 = {-1,-2,-4,-3};
    assert(spiralOrder(m10) == r10);

    return 0;
}
