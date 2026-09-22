Write a C++ function `std::pair<int,int> findPeakGrid(const std::vector<std::vector<int>>& mat)` that, given a non-empty matrix where all rows and columns are strictly increasing (i.e., each element is strictly greater than the element directly above, below, left, and right, if those exist), returns the 0-based indices `{row, col}` of a peak element. A peak element is defined as an element that is strictly greater than all its orthogonal neighbors (up, down, left, right). The matrix is guaranteed to have exactly one such peak, and it is guaranteed that the matrix has at least one row and one column. If the matrix is empty or has no valid peak (though not expected per constraints), return `{-1, -1}`. The function must be efficient and avoid checking every element. Note: The input is `const`-qualified, so your function must not modify it.

The standard approach is a binary search on columns. Since every column is strictly increasing from top to bottom, we can reduce the problem by picking a middle column, finding the row index of the maximum element in that column (call it `maxRow`). Now compare that maximum with its left and right neighbors (within the same row). If the left and right neighbors are both smaller, then that element is a peak (because it is also greater than its vertical neighbors due to being the column maximum). If the right neighbor is larger, then any peak in the right half of the matrix must exist, because moving to the right and then down/up from the column maximum guarantees a local maximum in that half; similarly, if left neighbor is larger, search the left half. This works because the matrix is strictly increasing along both axes, ensuring a single peak and that the "descent" direction leads to it. Edge cases: handle boundaries (mid=0 or mid=n-1) by treating out-of-bounds as `-infinity`. If the matrix has only one column, the maximum of that column is the peak. The time complexity is O(m log n) where m is number of rows and n is number of columns (each column search takes O(m)). Space complexity is O(1) auxiliary.

#include <vector>
#include <utility>

// Find the row index of the maximum element in a given column.
int findMaxRowIndex(const std::vector<std::vector<int>>& mat, int col) {
    int maxRow = 0;
    for (int i = 1; i < (int)mat.size(); ++i) {
        if (mat[i][col] > mat[maxRow][col]) {
            maxRow = i;
        }
    }
    return maxRow;
}

// Return the indices {row, col} of the unique peak in a strictly increasing matrix.
std::pair<int, int> findPeakGrid(const std::vector<std::vector<int>>& mat) {
    if (mat.empty() || mat[0].empty()) {
        return {-1, -1};
    }

    const int rows = (int)mat.size();
    const int cols = (int)mat[0].size();
    int left = 0, right = cols - 1;

    while (left <= right) {
        const int mid = left + (right - left) / 2;
        const int maxRow = findMaxRowIndex(mat, mid);

        // Check left neighbor (if it exists)
        const bool isLeftSmaller = (mid == 0) || (mat[maxRow][mid - 1] < mat[maxRow][mid]);
        // Check right neighbor (if it exists)
        const bool isRightSmaller = (mid == cols - 1) || (mat[maxRow][mid] > mat[maxRow][mid + 1]);

        if (isLeftSmaller && isRightSmaller) {
            return {maxRow, mid};
        } else if (isLeftSmaller && !isRightSmaller) {
            left = mid + 1; // move right
        } else {
            right = mid - 1; // move left
        }
    }

    return {-1, -1}; // Should never reach here for valid inputs
}

#include <cassert>
#include <vector>
#include <utility>

// Include the solution function here (or link against it)
// For assert testing, we assume the solution function is available.

int main() {
    // Single element
    std::vector<std::vector<int>> m1 = {{5}};
    assert(findPeakGrid(m1) == std::make_pair(0, 0));

    // Single row, multiple columns
    std::vector<std::vector<int>> m2 = {{1, 3, 5, 4, 2}};
    // Peak is 5 at index (0,2)
    assert(findPeakGrid(m2) == std::make_pair(0, 2));

    // Single column
    std::vector<std::vector<int>> m3 = {{1}, {4}, {7}, {6}};
    // Peak is 7 at (2,0)
    assert(findPeakGrid(m3) == std::make_pair(2, 0));

    // 2x2 matrix
    std::vector<std::vector<int>> m4 = {{1, 2}, {3, 4}};
    // Peak is 4 at (1,1)
    assert(findPeakGrid(m4) == std::make_pair(1, 1));

    // 3x3 matrix, peak in middle
    std::vector<std::vector<int>> m5 = {{1, 2, 3}, {2, 5, 4}, {3, 4, 6}};
    // Peak is 6 at (2,2) actually, but also 5 at (1,1) and 3 at (0,2) – but strictly increasing rows/cols ensures unique? Let's test with a valid matrix.
    // For strictly increasing both rows and cols, we need e.g.:
    std::vector<std::vector<int>> m6 = {{1, 2, 3}, {2, 4, 5}, {3, 5, 6}};
    // Actually this has multiple equal? Let's make strictly increasing:
    std::vector<std::vector<int>> m7 = {{1, 2, 3}, {2, 4, 6}, {3, 5, 7}};
    // Here 7 is peak at (2,2)
    assert(findPeakGrid(m7) == std::make_pair(2, 2));

    // Peak near left edge
    std::vector<std::vector<int>> m8 = {{4, 3, 2}, {5, 4, 3}, {6, 5, 4}};
    // Peak is 6 at (2,0)
    assert(findPeakGrid(m8) == std::make_pair(2, 0));

    // Peak near top-right edge
    std::vector<std::vector<int>> m9 = {{1, 2, 9}, {2, 3, 10}, {3, 4, 11}};
    // Peak is 11 at (2,2)
    assert(findPeakGrid(m9) == std::make_pair(2, 2));

    // Larger example
    std::vector<std::vector<int>> m10 = {
        {1,  2,  3,  4},
        {2,  3,  4,  5},
        {3,  4,  5,  6},
        {4,  5,  6,  7}
    };
    // Peak is 7 at (3,3)
    assert(findPeakGrid(m10) == std::make_pair(3, 3));

    return 0;
}
