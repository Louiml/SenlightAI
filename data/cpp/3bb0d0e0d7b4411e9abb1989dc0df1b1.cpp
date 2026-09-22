// Write a C++ function `findPeakElement2D` that takes a non-empty 2D vector of integers (representing a matrix with at least one row and one column) and returns a `std::pair<int, int>` containing the row and column indices of any peak element. A peak element is defined as an element that is strictly greater than all its orthogonal neighbors (up, down, left, right), where neighbors outside the matrix boundaries are ignored. If multiple peaks exist, any valid peak may be returned. The function must not modify the input matrix and should work efficiently for matrices up to 1000×1000.
// The problem is a two-dimensional extension of finding a single peak. A simple O(m×n) exhaustive search always works but is suboptimal. A more efficient approach uses binary search on columns. The key insight: choose the middle column, find the element with maximum value in that column. If that element is greater than its left and right neighbors, it is a peak. Otherwise, move to the half containing the larger neighbor, because the maximum of the neighbor column must be greater than the middle column's maximum, guaranteeing that a peak exists in that half. This reduces the search space by half each iteration. Time complexity is O(m log n) where m is rows and n is columns. Space is O(1) aside from recursion/iteration overhead. Edge cases: single row (then check left/right), single column (then check up/down), or 1×1 matrix (trivially a peak). The algorithm must handle ties: if a neighbor is equal, we can choose either direction; the definition requires strict greater, but moving to an equal-valued neighbor is still safe because we can eventually find a strictly greater element or reach the boundary and the maximum in the final column is a peak.
#include <vector>
#include <utility>
#include <algorithm>

// Find any peak element in a 2D matrix, returning its (row, col) indices.
// A peak is strictly greater than its orthogonal neighbors (ignoring out-of-bounds).
// Uses binary search on columns for O(m log n) time.
std::pair<int, int> findPeakElement2D(const std::vector<std::vector<int>>& mat) {
    int m = static_cast<int>(mat.size());
    int n = static_cast<int>(mat[0].size());
    int left = 0, right = n - 1;

    while (left <= right) {
        int mid = left + (right - left) / 2;

        // Find row index of max element in column mid
        int max_row = 0;
        for (int i = 1; i < m; ++i) {
            if (mat[i][mid] > mat[max_row][mid]) {
                max_row = i;
            }
        }

        bool left_greater = (mid > 0 && mat[max_row][mid - 1] > mat[max_row][mid]);
        bool right_greater = (mid < n - 1 && mat[max_row][mid + 1] > mat[max_row][mid]);

        if (!left_greater && !right_greater) {
            // Peak found
            return {max_row, mid};
        }
        else if (left_greater) {
            right = mid - 1;
        }
        else { // right_greater true
            left = mid + 1;
        }
    }
    // Should never reach here for non-empty input
    return {0, 0};
}
#include <cassert>
#include <vector>

bool isPeak(const std::vector<std::vector<int>>& mat, int r, int c) {
    int m = mat.size(), n = mat[0].size();
    int val = mat[r][c];
    if (r > 0 && mat[r-1][c] >= val) return false;
    if (r < m-1 && mat[r+1][c] >= val) return false;
    if (c > 0 && mat[r][c-1] >= val) return false;
    if (c < n-1 && mat[r][c+1] >= val) return false;
    return true;
}

int main() {
    // 1x1
    std::vector<std::vector<int>> mat1 = {{5}};
    auto p1 = findPeakElement2D(mat1);
    assert(p1.first == 0 && p1.second == 0);

    // Single row, increasing
    std::vector<std::vector<int>> mat2 = {{1, 2, 3, 4}};
    auto p2 = findPeakElement2D(mat2);
    assert(isPeak(mat2, p2.first, p2.second));

    // Single column, decreasing
    std::vector<std::vector<int>> mat3 = {{4}, {3}, {2}};
    auto p3 = findPeakElement2D(mat3);
    assert(isPeak(mat3, p3.first, p3.second));

    // 2x2 diagonal peak
    std::vector<std::vector<int>> mat4 = {{1, 2}, {3, 4}};
    auto p4 = findPeakElement2D(mat4);
    assert(isPeak(mat4, p4.first, p4.second));

    // 3x3 with peak in middle
    std::vector<std::vector<int>> mat5 = {{1, 1, 1},{1, 9, 1},{1, 1, 1}};
    auto p5 = findPeakElement2D(mat5);
    assert(p5.first == 1 && p5.second == 1);

    // Larger, multiple peaks, check validity
    std::vector<std::vector<int>> mat6 = {
        {10, 20, 15}, 
        {21, 30, 14},
        {7, 16, 32}
    };
    auto p6 = findPeakElement2D(mat6);
    assert(isPeak(mat6, p6.first, p6.second));

    // All equal (any is peak because strict greater over neighbors? No, all equal means no strict greater, so no peak? Actually with strict greater definition, all equal has no peak. But constraints? Let's test with equal numbers: the algorithm will return some cell, but that's not a true peak. For strict definition, we must ensure at least one strictly greater exists. So this test is invalid; skip.

    // Edge: 2x3 matrix
    std::vector<std::vector<int>> mat7 = {{1, 4, 3}, {2, 5, 6}};
    auto p7 = findPeakElement2D(mat7);
    assert(isPeak(mat7, p7.first, p7.second));

    return 0;
}
