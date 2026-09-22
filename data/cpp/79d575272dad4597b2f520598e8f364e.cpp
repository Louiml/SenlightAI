Write a C++ function `int maximalRectangleArea(const std::vector<std::vector<int>>& matrix)` that takes a binary matrix (each entry is 0 or 1) as input and returns the area of the largest rectangle composed entirely of 1s. The input matrix is guaranteed to be non-empty (at least one row and one column) and is passed by const reference so it must not be modified. Handle edge cases such as a single row, a single column, an all-zero matrix, and an all-one matrix, and return 0 if no 1 exists.

// The solution builds on the classic "largest rectangle in a histogram" problem. For each row of the matrix, we treat it as the base of a histogram where each column's height is the number of consecutive 1s ending at that row (cumulative downward). We first construct these heights row by row: for row 0, heights are just the row values. For each subsequent row, we update each column's height by adding 1 if the current cell is 1, otherwise resetting to 0 (because a 0 breaks the consecutive vertical streak). After computing the heights for a given row, we call a helper that computes the largest rectangle area in that histogram using a monotonic stack. The helper finds, for each bar, the previous smaller element (left boundary) and next smaller element (right boundary) by maintaining an increasing stack of indices. The area for a bar at index i is `height[i] * (right[i] - left[i] - 1)`. We update the global maximum across all rows. Time complexity is O(rows × cols) because each bar is pushed and popped from the stack exactly once per row, and the height update is O(1) per cell. Space complexity is O(cols) for the height array and O(cols) for the stack and boundary arrays in the helper.

#include <vector>
#include <stack>
#include <algorithm>

// Helper: largest rectangle area in a histogram given heights.
int largestRectangleInHistogram(const std::vector<int>& heights) {
    int n = static_cast<int>(heights.size());
    if (n == 0) return 0;

    std::vector<int> left(n), right(n);
    std::stack<int> st;

    // Previous smaller element index
    for (int i = 0; i < n; ++i) {
        while (!st.empty() && heights[st.top()] >= heights[i]) {
            st.pop();
        }
        left[i] = st.empty() ? -1 : st.top();
        st.push(i);
    }

    // Clear stack for next pass
    while (!st.empty()) st.pop();

    // Next smaller element index
    for (int i = n - 1; i >= 0; --i) {
        while (!st.empty() && heights[st.top()] >= heights[i]) {
            st.pop();
        }
        right[i] = st.empty() ? n : st.top();
        st.push(i);
    }

    int maxArea = 0;
    for (int i = 0; i < n; ++i) {
        int width = right[i] - left[i] - 1;
        maxArea = std::max(maxArea, heights[i] * width);
    }
    return maxArea;
}

// Main function: compute maximal rectangle of 1s in a binary matrix.
int maximalRectangleArea(const std::vector<std::vector<int>>& matrix) {
    if (matrix.empty() || matrix[0].empty()) return 0;
    int rows = static_cast<int>(matrix.size());
    int cols = static_cast<int>(matrix[0].size());

    std::vector<int> heights(cols, 0);
    int maxArea = 0;

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (matrix[i][j] == 1) {
                heights[j] += 1;
            } else {
                heights[j] = 0;
            }
        }
        maxArea = std::max(maxArea, largestRectangleInHistogram(heights));
    }
    return maxArea;
}

#include <cassert>
#include <vector>

// Assume the solution function is declared above.

int main() {
    // Example from the snippet
    std::vector<std::vector<int>> mat1 = {
        {0, 1, 1, 0},
        {1, 1, 1, 1},
        {1, 1, 1, 1},
        {1, 1, 0, 0}
    };
    assert(maximalRectangleArea(mat1) == 6);

    // Single row all ones
    std::vector<std::vector<int>> mat2 = {{1, 1, 1, 1}};
    assert(maximalRectangleArea(mat2) == 4);

    // Single column all ones
    std::vector<std::vector<int>> mat3 = {{1}, {1}, {1}};
    assert(maximalRectangleArea(mat3) == 3);

    // All zeros
    std::vector<std::vector<int>> mat4 = {{0, 0, 0}, {0, 0, 0}};
    assert(maximalRectangleArea(mat4) == 0);

    // Single 1
    std::vector<std::vector<int>> mat5 = {{0, 0, 0}, {0, 1, 0}, {0, 0, 0}};
    assert(maximalRectangleArea(mat5) == 1);

    // Larger rectangle in the middle
    std::vector<std::vector<int>> mat6 = {
        {1, 0, 1, 0, 0},
        {1, 0, 1, 1, 1},
        {1, 1, 1, 1, 1},
        {1, 0, 0, 1, 0}
    };
    assert(maximalRectangleArea(mat6) == 6);

    // Full 2x2 rectangle
    std::vector<std::vector<int>> mat7 = {{1, 1}, {1, 1}};
    assert(maximalRectangleArea(mat7) == 4);

    // Mixed row heights
    std::vector<std::vector<int>> mat8 = {
        {0, 0, 1, 0, 0},
        {0, 1, 1, 1, 0},
        {1, 1, 1, 1, 0},
        {1, 1, 1, 1, 1}
    };
    assert(maximalRectangleArea(mat8) == 8);

    return 0;
}
