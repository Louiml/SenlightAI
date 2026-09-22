Write a C++ function that takes a non-empty 2D vector of integers (a matrix) and returns a 1D vector containing all matrix elements in spiral order, starting from the top-left corner and moving clockwise (right, down, left, up, repeating). The matrix may be rectangular (rows and columns need not be equal) but will always have at least one row and one column (i.e., non-empty). The spiral should shrink inward layer by layer until all elements are collected. For example, for the matrix `[[1,2,3],[4,5,6],[7,8,9]]`, the output should be `[1,2,3,6,9,8,7,4,5]`. The function should handle both square and non-square matrices, including single-row and single-column cases.
#include <cassert>
#include <vector>

// declaration of the tested function
std::vector<int> spiralOrder(const std::vector<std::vector<int>>& matrix);

int main() {
    // Standard 3x3
    std::vector<std::vector<int>> m1 = {{1,2,3},{4,5,6},{7,8,9}};
    std::vector<int> r1 = spiralOrder(m1);
    assert(r1 == (std::vector<int>{1,2,3,6,9,8,7,4,5}));

    // 3x4 rectangular
    std::vector<std::vector<int>> m2 = {{1,2,3,4},{5,6,7,8},{9,10,11,12}};
    std::vector<int> r2 = spiralOrder(m2);
    assert(r2 == (std::vector<int>{1,2,3,4,8,12,11,10,9,5,6,7}));

    // Single row
    std::vector<std::vector<int>> m3 = {{1,2,3,4}};
    std::vector<int> r3 = spiralOrder(m3);
    assert(r3 == (std::vector<int>{1,2,3,4}));

    // Single column
    std::vector<std::vector<int>> m4 = {{1},{2},{3}};
    std::vector<int> r4 = spiralOrder(m4);
    assert(r4 == (std::vector<int>{1,2,3}));

    // 1x1
    std::vector<std::vector<int>> m5 = {{7}};
    std::vector<int> r5 = spiralOrder(m5);
    assert(r5 == (std::vector<int>{7}));

    // 2x3
    std::vector<std::vector<int>> m6 = {{1,2,3},{4,5,6}};
    std::vector<int> r6 = spiralOrder(m6);
    assert(r6 == (std::vector<int>{1,2,3,6,5,4}));

    // 3x2
    std::vector<std::vector<int>> m7 = {{1,2},{3,4},{5,6}};
    std::vector<int> r7 = spiralOrder(m7);
    assert(r7 == (std::vector<int>{1,2,4,6,5,3}));

    // 4x4
    std::vector<std::vector<int>> m8 = {{1,2,3,4},{5,6,7,8},{9,10,11,12},{13,14,15,16}};
    std::vector<int> r8 = spiralOrder(m8);
    assert(r8 == (std::vector<int>{1,2,3,4,8,12,16,15,14,13,9,5,6,7,11,10}));

    // Large random test with all identical values to ensure no duplicates or omissions
    std::vector<std::vector<int>> m9(5, std::vector<int>(7, 42));
    std::vector<int> r9 = spiralOrder(m9);
    assert(r9.size() == 35);
    for (int v : r9) assert(v == 42);

    return 0;
}
#include <vector>

// Return all elements of the matrix in spiral order (clockwise, starting top-left).
std::vector<int> spiralOrder(const std::vector<std::vector<int>>& matrix) {
    if (matrix.empty() || matrix[0].empty()) return {};

    const int rows = matrix.size();
    const int cols = matrix[0].size();
    int top = 0;
    int bottom = rows - 1;
    int left = 0;
    int right = cols - 1;

    std::vector<int> result;
    result.reserve(rows * cols);

    while (top <= bottom && left <= right) {
        // Traverse top row from left to right
        for (int i = left; i <= right; ++i) {
            result.push_back(matrix[top][i]);
        }
        ++top;

        // Traverse right column from top to bottom
        for (int i = top; i <= bottom; ++i) {
            result.push_back(matrix[i][right]);
        }
        --right;

        // Traverse bottom row from right to left (if still valid)
        if (top <= bottom) {
            for (int i = right; i >= left; --i) {
                result.push_back(matrix[bottom][i]);
            }
            --bottom;
        }

        // Traverse left column from bottom to top (if still valid)
        if (left <= right) {
            for (int i = bottom; i >= top; --i) {
                result.push_back(matrix[i][left]);
            }
            ++left;
        }
    }

    return result;
}
// The solution uses four boundary indices: `top`, `bottom`, `left`, and `right`, which define the current unvisited layer. The algorithm repeatedly traverses the top row from left to right, the right column from top+1 to bottom, the bottom row from right-1 to left (only if `top <= bottom` remains true after incrementing top), and the left column from bottom-1 to top (only if `left <= right` remains true after decrementing right). After each full layer traversal, the boundaries shrink inward. The key edge cases are: (1) single-row matrix — after moving right, we must skip moving down/left/up to avoid repeating elements; (2) single-column matrix — after moving down, skip right/up/left; (3) matrices with more rows than columns or vice versa — the conditional checks inside the loop prevent out-of-bounds or duplicate traversals when the remaining space becomes a single row or column. Time complexity is O(R*C) because every element is visited exactly once. Space complexity is O(1) auxiliary (excluding the output vector itself, which is O(R*C) and unavoidable).
