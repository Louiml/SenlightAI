Given a non-empty matrix of integers whose rows and columns are both sorted in strictly increasing order, write a C++ function `bool searchSortedMatrix(const vector<vector<int>>& mat, int x)` that returns `true` if the integer `x` exists in the matrix and `false` otherwise. The matrix dimensions `n` (rows) and `m` (columns) are both at least 1. Your function must traverse the matrix using the "staircase search" method, starting from the top-right corner, and must not use binary search, sorting, or any extra container. Important edge cases include a matrix with a single row or column, and the value `x` being the smallest or largest element in the matrix.

// The algorithm starts at the top-right corner (`i = 0`, `j = m-1`) because at that position, all elements to the left are smaller (since the row is increasing left-to-right) and all elements below are larger (since the column is increasing top-to-bottom). While the indices remain valid, compare the current element with `x`. If equal, return `true`. If the current element is greater than `x`, then `x` cannot be in the current column below or at the current element, so move left (`j--`). If the current element is less than `x`, then `x` cannot be in the current row to the left or at the current element, so move down (`i++`). The loop terminates when index boundaries are exceeded, meaning `x` is not present. Edge cases: a 1×1 matrix works trivially; a single row behaves like a linear scan from right to left; a single column behaves like a linear scan from top to bottom. Time complexity is `O(n + m)` because in the worst case each step moves either left or down, and the total number of moves is at most `n + m`. Space complexity is `O(1)` because only indices are used.

#include <vector>

// Returns true if integer x exists in the matrix, where both rows and columns are strictly increasing.
bool searchSortedMatrix(const std::vector<std::vector<int>>& mat, int x) {
    int rows = mat.size();
    int cols = mat[0].size();
    int i = 0;          // row index starting at top
    int j = cols - 1;   // column index starting at rightmost

    while (i < rows && j >= 0) {
        if (mat[i][j] == x) {
            return true;
        }
        if (mat[i][j] > x) {
            // Current element is too large, so x cannot be in this column below.
            --j;
        } else {
            // Current element is too small, so x cannot be in this row to the left.
            ++i;
        }
    }
    return false;
}

#include <cassert>
#include <vector>

int main() {
    // Standard 3x3 sorted matrix from the prompt.
    std::vector<std::vector<int>> mat1 = {
        {1, 4, 8},
        {5, 7, 10},
        {9, 12, 15}
    };
    assert(searchSortedMatrix(mat1, 8) == true);
    assert(searchSortedMatrix(mat1, 7) == true);
    assert(searchSortedMatrix(mat1, 15) == true);
    assert(searchSortedMatrix(mat1, 1) == true);
    assert(searchSortedMatrix(mat1, 6) == false);
    assert(searchSortedMatrix(mat1, 0) == false);
    assert(searchSortedMatrix(mat1, 16) == false);

    // Single row matrix.
    std::vector<std::vector<int>> mat2 = {{2, 5, 9, 11}};
    assert(searchSortedMatrix(mat2, 2) == true);
    assert(searchSortedMatrix(mat2, 11) == true);
    assert(searchSortedMatrix(mat2, 7) == false);

    // Single column matrix.
    std::vector<std::vector<int>> mat3 = {{-3}, {0}, {4}, {8}};
    assert(searchSortedMatrix(mat3, -3) == true);
    assert(searchSortedMatrix(mat3, 4) == true);
    assert(searchSortedMatrix(mat3, 5) == false);

    // 1x1 matrix.
    std::vector<std::vector<int>> mat4 = {{42}};
    assert(searchSortedMatrix(mat4, 42) == true);
    assert(searchSortedMatrix(mat4, 43) == false);

    // Negative numbers and edge values.
    std::vector<std::vector<int>> mat5 = {{-10, -5, 0}, {-4, -1, 3}, {2, 6, 9}};
    assert(searchSortedMatrix(mat5, -10) == true);
    assert(searchSortedMatrix(mat5, 9) == true);
    assert(searchSortedMatrix(mat5, -6) == false);
    assert(searchSortedMatrix(mat5, 4) == false);

    return 0;
}
