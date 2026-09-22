Write a C++ function `int maxAreaInBinaryMatrix(const std::vector<std::vector<int>>& matrix)` that takes a binary matrix (each cell is either 0 or 1) and returns the area of the largest axis-aligned rectangle composed entirely of zeros. The input matrix dimensions are between 1 and 1000 for rows and columns, and the matrix is guaranteed to be non-empty. The function should treat 0 as an empty cell and 1 as an obstacle. For each row, compute the consecutive count of zeros ending at that cell (i.e., if the cell is 0, add 1 to the count from the cell above, otherwise reset to 0). Then for each row histogram, find the largest rectangle in a histogram using a monotonic stack. Return the maximum area across all rows.

// The problem is a classic extension of the "largest rectangle in histogram" problem to a 2D binary matrix. The key observation is that for each row `i`, we can build a histogram `hist[j]` representing the number of consecutive zeros ending at cell `(i,j)` (i.e., the height of zeros above and including this cell). Then the largest rectangle that ends on this row is exactly the largest rectangle in that histogram. So we process rows from top to bottom, updating the histogram incrementally: for each cell, if the matrix value is 0, increase the histogram value by 1; if it's 1, reset it to 0. For each row, we run the largest rectangle in histogram algorithm (using a monotonic stack of pairs `(height, start_index)`). The algorithm pushes a sentinel 0 at the end to flush the stack. We maintain the maximum width by storing the start index of a bar when we pop. Edge cases include all zeros (max area is full matrix), all ones (max area 0), and a single row or column. Time complexity is O(rows * cols) because each histogram cell is pushed and popped at most once per row. Space complexity is O(cols) for the histogram vector and O(cols) for the stack.

#include <vector>
#include <stack>
#include <algorithm>

// Helper: largest rectangle area in a histogram (heights vector)
int largestRectangleInHistogram(const std::vector<int>& heights) {
    std::vector<int> hist = heights;
    hist.push_back(0); // sentinel to flush stack
    std::stack<std::pair<int, int>> st; // pair (height, start_index)
    int maxArea = 0;
    for (int i = 0; i < static_cast<int>(hist.size()); ++i) {
        int start = i;
        while (!st.empty() && st.top().first >= hist[i]) {
            int height = st.top().first;
            int left = st.top().second;
            st.pop();
            int width = i - left;
            maxArea = std::max(maxArea, height * width);
            start = left; // extend current bar backwards
        }
        st.emplace(hist[i], start);
    }
    return maxArea;
}

// Returns the area of the largest rectangle of zeros in the binary matrix
int maxAreaInBinaryMatrix(const std::vector<std::vector<int>>& matrix) {
    if (matrix.empty() || matrix[0].empty()) return 0;
    int rows = static_cast<int>(matrix.size());
    int cols = static_cast<int>(matrix[0].size());
    std::vector<int> hist(cols, 0);
    int globalMax = 0;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (matrix[i][j] == 0) {
                hist[j] += 1;
            } else {
                hist[j] = 0;
            }
        }
        globalMax = std::max(globalMax, largestRectangleInHistogram(hist));
    }
    return globalMax;
}

#include <cassert>
#include <vector>

// Assume the solution function is declared above.
// Add the solution code here (or include it).

int main() {
    // Test 1: all zeros 2x2 -> area 4
    std::vector<std::vector<int>> m1 = {{0,0},{0,0}};
    assert(maxAreaInBinaryMatrix(m1) == 4);

    // Test 2: all ones -> 0
    std::vector<std::vector<int>> m2 = {{1,1},{1,1}};
    assert(maxAreaInBinaryMatrix(m2) == 0);

    // Test 3: single row with zeros and ones
    std::vector<std::vector<int>> m3 = {{0,1,0,0,0}};
    assert(maxAreaInBinaryMatrix(m3) == 3);

    // Test 4: single column
    std::vector<std::vector<int>> m4 = {{0},{1},{0},{0}};
    assert(maxAreaInBinaryMatrix(m4) == 2);

    // Test 5: L-shaped zeros
    std::vector<std::vector<int>> m5 = {{0,0,1},{0,0,1},{1,0,0}};
    // Largest zero rectangle is a 2x2 at top-left -> area 4
    assert(maxAreaInBinaryMatrix(m5) == 4);

    // Test 6: full 3x3 zeros
    std::vector<std::vector<int>> m6 = {{0,0,0},{0,0,0},{0,0,0}};
    assert(maxAreaInBinaryMatrix(m6) == 9);

    // Test 7: diagonal zeros
    std::vector<std::vector<int>> m7 = {{0,1,1},{1,0,1},{1,1,0}};
    // Each 1x1 zero cell, max area 1
    assert(maxAreaInBinaryMatrix(m7) == 1);

    // Test 8: larger matrix with a 2x3 zero block
    std::vector<std::vector<int>> m8 = {
        {1,0,0,0,1},
        {1,0,0,0,1},
        {1,1,1,1,1}
    };
    // Zero rectangle is 2x3 at columns 1-3 -> area 6
    assert(maxAreaInBinaryMatrix(m8) == 6);

    // Test 9: tall rectangle
    std::vector<std::vector<int>> m9 = {
        {0,1},
        {0,1},
        {0,0}
    };
    // The zero rectangle of height 2 and width 1 on left column -> area 2
    // Also bottom row has two zeros, but with height 1 width 2 -> area 2
    // Max is 2
    assert(maxAreaInBinaryMatrix(m9) == 2);

    // Test 10: single cell zero
    std::vector<std::vector<int>> m10 = {{0}};
    assert(maxAreaInBinaryMatrix(m10) == 1);

    return 0;
}
