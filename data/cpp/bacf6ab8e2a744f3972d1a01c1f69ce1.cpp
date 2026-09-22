Write a standalone C++ function `spiralOrder` that accepts a 2D rectangular matrix of integers (as a `std::vector<std::vector<int>>`) and returns a `std::vector<int>` containing the elements in clockwise spiral order, starting from the top-left corner. The matrix may be empty (return an empty vector), may have a single row or single column, and all rows will have the same length. The function must be `const`-correct (take the matrix by `const` reference) and must not modify the input. Handle matrices of arbitrary non‑negative dimensions robustly, including 1×N, N×1, and square matrices, without out-of-bounds access or duplicate reads.

The solution simulates walking the perimeter of the matrix in four directions: left-to-right across the top row, top-to-bottom down the right column, right-to-left across the bottom row, and bottom-to-top up the left column. After each full layer (or partial layer if the matrix is narrow), we shrink the boundaries inward. The key is to track the current top row index, bottom row index, left column index, and right column index, and to stop immediately when the remaining element count reaches zero. A straightforward approach uses four loops and after each loop checks whether the number of elements still to be placed is zero; if so, break. Edge cases: an empty matrix (rows == 0) returns an empty vector. For a single row, after the first rightward loop the element count becomes zero and we break before moving down. For a single column, the first rightward loop reads one element, then the downward loop reads the rest; the leftward and upward loops are skipped because the count is zero. The time complexity is O(N) where N is the total number of elements, because each element is visited exactly once. The space complexity is O(N) for the output vector; auxiliary space (besides the output) is O(1) because we only use a few integer indices.

#include <vector>

// Returns the elements of a rectangular matrix in clockwise spiral order.
// The input matrix is taken by const reference and is not modified.
std::vector<int> spiralOrder(const std::vector<std::vector<int>>& matrix) {
    std::vector<int> result;
    if (matrix.empty()) {
        return result;
    }

    const int rows = static_cast<int>(matrix.size());
    const int cols = static_cast<int>(matrix[0].size());
    const int total = rows * cols;

    int top = 0;
    int bottom = rows - 1;
    int left = 0;
    int right = cols - 1;
    int count = 0;

    while (count < total) {
        // Traverse from left to right along the current top row.
        for (int col = left; col <= right && count < total; ++col) {
            result.push_back(matrix[top][col]);
            ++count;
        }
        ++top;

        // Traverse from top to bottom along the current right column.
        for (int row = top; row <= bottom && count < total; ++row) {
            result.push_back(matrix[row][right]);
            ++count;
        }
        --right;

        // Traverse from right to left along the current bottom row.
        for (int col = right; col >= left && count < total; --col) {
            result.push_back(matrix[bottom][col]);
            ++count;
        }
        --bottom;

        // Traverse from bottom to top along the current left column.
        for (int row = bottom; row >= top && count < total; --row) {
            result.push_back(matrix[row][left]);
            ++count;
        }
        ++left;
    }

    return result;
}

#include <cassert>
#include <vector>

// The solution function is assumed to be defined above (spiralOrder).
int main() {
    // Empty matrix
    std::vector<std::vector<int>> empty;
    assert(spiralOrder(empty) == std::vector<int>());

    // 1x1 matrix
    std::vector<std::vector<int>> single = {{7}};
    assert(spiralOrder(single) == std::vector<int>({7}));

    // 1xN matrix (single row)
    std::vector<std::vector<int>> row = {{1,2,3,4}};
    assert(spiralOrder(row) == std::vector<int>({1,2,3,4}));

    // Nx1 matrix (single column)
    std::vector<std::vector<int>> col = {{1},{2},{3}};
    assert(spiralOrder(col) == std::vector<int>({1,2,3}));

    // 3x3 matrix
    std::vector<std::vector<int>> m3 = {{1,2,3},{4,5,6},{7,8,9}};
    assert(spiralOrder(m3) == std::vector<int>({1,2,3,6,9,8,7,4,5}));

    // 4x4 matrix
    std::vector<std::vector<int>> m4 = {
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9,10,11,12},
        {13,14,15,16}
    };
    assert(spiralOrder(m4) == std::vector<int>({1,2,3,4,8,12,16,15,14,13,9,5,6,7,11,10}));

    // 3x4 matrix (rectangular, more columns than rows)
    std::vector<std::vector<int>> m34 = {
        {1,2,3,4},
        {5,6,7,8},
        {9,10,11,12}
    };
    assert(spiralOrder(m34) == std::vector<int>({1,2,3,4,8,12,11,10,9,5,6,7}));

    // 4x3 matrix (rectangular, more rows than columns)
    std::vector<std::vector<int>> m43 = {
        {1,2,3},
        {4,5,6},
        {7,8,9},
        {10,11,12}
    };
    assert(spiralOrder(m43) == std::vector<int>({1,2,3,6,9,12,11,10,7,4,5,8}));

    // 2x2 matrix
    std::vector<std::vector<int>> m2 = {{1,2},{3,4}};
    assert(spiralOrder(m2) == std::vector<int>({1,2,4,3}));

    return 0;
}
