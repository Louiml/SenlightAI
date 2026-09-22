Write a C++ function that takes a 2D rectangular matrix of integers (with all rows having the same length) and returns a vector of integers containing the elements of the matrix traversed in a counterclockwise spiral order starting from the bottom-left corner and moving inward. The spiral should follow the standard spiral pattern: start at the bottom-left, move upward along the leftmost column, then right along the top row, then down along the rightmost column, then left along the bottom row, and continue inward until all elements are visited. The function should accept the matrix as a `std::vector<std::vector<int>>` and return the traversal as a `std::vector<int>`. The matrix will have at least one row and one column, and all rows will be non-empty and of equal length. The function must handle matrices of any size, including single-row, single-column, and rectangular shapes, and must not modify the input.
The solution simulates the spiral traversal by maintaining four boundary indices: `top` (current top row index), `bottom` (current bottom row index), `left` (current left column index), and `right` (current right column index). The traversal proceeds in four steps per layer: first, move from bottom to top along the leftmost column (from `bottom` to `top`) adding those elements; then increment `left`. Next, if still within bounds, move from left to right along the top row (from `left` to `right`) and then increment `top`. Then, if still within bounds, move from top to bottom along the rightmost column (from `top` to `bottom`) and decrement `right`. Finally, if still within bounds, move from right to left along the bottom row (from `right` to `left`) and decrement `bottom`. The loop continues while `top <= bottom` and `left <= right`. After each move, boundary checks are performed to avoid adding duplicate elements, especially for single-row or single-column matrices. The initial step for the first layer must correctly start at the bottom-left corner: after the first upward move along the left column, the next move to the right along the top row must start from the correct cell. The algorithm runs in O(n*m) time, visiting each cell exactly once, and uses O(n*m) auxiliary space for the result vector (which is required to store the output). The space complexity could be considered O(n*m) for the result, but the algorithm itself uses only O(1) extra space for boundary indices. Edge cases include single-element matrices, single-row matrices (which produce a vertical traversal first then a horizontal, but careful handling), and single-column matrices.
#include <vector>

// Returns the elements of a matrix in counterclockwise spiral order starting from bottom-left.
std::vector<int> counterClockwiseSpiral(const std::vector<std::vector<int>>& matrix) {
    std::vector<int> result;
    if (matrix.empty() || matrix[0].empty()) return result;

    int top = 0;
    int bottom = static_cast<int>(matrix.size()) - 1;
    int left = 0;
    int right = static_cast<int>(matrix[0].size()) - 1;

    while (top <= bottom && left <= right) {
        // Move upward along the left column (from bottom to top)
        for (int i = bottom; i >= top; --i) {
            result.push_back(matrix[i][left]);
        }
        left++;

        // Move right along the top row (from left to right)
        if (left > right) break;
        for (int i = left; i <= right; ++i) {
            result.push_back(matrix[top][i]);
        }
        top++;

        // Move downward along the right column (from top to bottom)
        if (top > bottom) break;
        for (int i = top; i <= bottom; ++i) {
            result.push_back(matrix[i][right]);
        }
        right--;

        // Move left along the bottom row (from right to left)
        if (left > right) break;
        for (int i = right; i >= left; --i) {
            result.push_back(matrix[bottom][i]);
        }
        bottom--;
    }

    return result;
}
#include <cassert>
#include <vector>

// Function to test (repeated here to make test self-contained)
std::vector<int> counterClockwiseSpiral(const std::vector<std::vector<int>>& matrix) {
    std::vector<int> result;
    if (matrix.empty() || matrix[0].empty()) return result;

    int top = 0;
    int bottom = static_cast<int>(matrix.size()) - 1;
    int left = 0;
    int right = static_cast<int>(matrix[0].size()) - 1;

    while (top <= bottom && left <= right) {
        for (int i = bottom; i >= top; --i) {
            result.push_back(matrix[i][left]);
        }
        left++;

        if (left > right) break;
        for (int i = left; i <= right; ++i) {
            result.push_back(matrix[top][i]);
        }
        top++;

        if (top > bottom) break;
        for (int i = top; i <= bottom; ++i) {
            result.push_back(matrix[i][right]);
        }
        right--;

        if (left > right) break;
        for (int i = right; i >= left; --i) {
            result.push_back(matrix[bottom][i]);
        }
        bottom--;
    }

    return result;
}

int main() {
    // 3x3 matrix
    std::vector<std::vector<int>> m1 = {{1,2,3},{4,5,6},{7,8,9}};
    std::vector<int> r1 = counterClockwiseSpiral(m1);
    assert(r1 == std::vector<int>({7,4,1,2,3,6,9,8,5}));

    // 2x3 matrix
    std::vector<std::vector<int>> m2 = {{1,2,3},{4,5,6}};
    std::vector<int> r2 = counterClockwiseSpiral(m2);
    assert(r2 == std::vector<int>({4,1,2,3,6,5}));

    // 3x2 matrix
    std::vector<std::vector<int>> m3 = {{1,2},{3,4},{5,6}};
    std::vector<int> r3 = counterClockwiseSpiral(m3);
    assert(r3 == std::vector<int>({5,3,1,2,4,6}));

    // Single row
    std::vector<std::vector<int>> m4 = {{1,2,3,4}};
    std::vector<int> r4 = counterClockwiseSpiral(m4);
    assert(r4 == std::vector<int>({1,2,3,4}));

    // Single column
    std::vector<std::vector<int>> m5 = {{1},{2},{3}};
    std::vector<int> r5 = counterClockwiseSpiral(m5);
    assert(r5 == std::vector<int>({3,2,1}));

    // Single element
    std::vector<std::vector<int>> m6 = {{5}};
    std::vector<int> r6 = counterClockwiseSpiral(m6);
    assert(r6 == std::vector<int>({5}));

    // 4x4 matrix
    std::vector<std::vector<int>> m7 = {{1,2,3,4},{5,6,7,8},{9,10,11,12},{13,14,15,16}};
    std::vector<int> r7 = counterClockwiseSpiral(m7);
    assert(r7 == std::vector<int>({13,9,5,1,2,3,4,8,12,16,15,14,10,6,7,11}));

    return 0;
}
