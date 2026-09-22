/*
Write a C++ function `bool search2DMatrix(const std::vector<std::vector<int>>& matrix, int target)` that determines whether an integer `target` exists in a 2D matrix where each row is sorted in ascending order from left to right, and the first integer of each row is strictly greater than the last integer of the previous row (i.e., the entire matrix can be viewed as a single sorted sequence when read row by row). The matrix may have zero rows or zero columns; in such cases, the function must return `false`. The function should leverage the sorted property to achieve logarithmic time complexity without flattening the matrix into a separate 1D vector. Ensure the function is `const`-correct and uses appropriate bounds checking.
*/

#include <vector>

// Determines if 'target' exists in a row-wise sorted matrix where each row is
// sorted and the first element of each row is greater than the last element of
// the previous row. The matrix may be empty or have zero columns.
bool search2DMatrix(const std::vector<std::vector<int>>& matrix, int target) {
    const int rows = static_cast<int>(matrix.size());
    if (rows == 0) {
        return false;
    }

    const int cols = static_cast<int>(matrix[0].size());
    if (cols == 0) {
        return false;
    }

    int left = 0;
    int right = rows * cols - 1;

    while (left <= right) {
        const int mid = left + (right - left) / 2;  // avoid potential overflow
        const int midValue = matrix[mid / cols][mid % cols];

        if (midValue == target) {
            return true;
        } else if (midValue < target) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }

    return false;
}

#include <cassert>
#include <vector>

int main() {
    // Standard 3x4 matrix sorted row-wise with strict row-first ordering.
    std::vector<std::vector<int>> matrix1 = {
        {1, 3, 5, 7},
        {10, 11, 16, 20},
        {23, 30, 34, 60}
    };
    assert(search2DMatrix(matrix1, 3) == true);
    assert(search2DMatrix(matrix1, 13) == false);
    assert(search2DMatrix(matrix1, 60) == true);
    assert(search2DMatrix(matrix1, 1) == true);
    assert(search2DMatrix(matrix1, 0) == false);
    assert(search2DMatrix(matrix1, 100) == false);

    // Single element matrix.
    std::vector<std::vector<int>> matrix2 = {{5}};
    assert(search2DMatrix(matrix2, 5) == true);
    assert(search2DMatrix(matrix2, 4) == false);

    // Single row matrix.
    std::vector<std::vector<int>> matrix3 = {{1, 2, 3}};
    assert(search2DMatrix(matrix3, 2) == true);
    assert(search2DMatrix(matrix3, 4) == false);

    // Single column matrix.
    std::vector<std::vector<int>> matrix4 = {{1}, {4}, {7}};
    assert(search2DMatrix(matrix4, 4) == true);
    assert(search2DMatrix(matrix4, 3) == false);

    // Empty matrix (no rows).
    std::vector<std::vector<int>> emptyRows;
    assert(search2DMatrix(emptyRows, 1) == false);

    // Matrix with zero columns.
    std::vector<std::vector<int>> zeroCols = {{}, {}, {}};
    assert(search2DMatrix(zeroCols, 0) == false);

    // Duplicate values? In this problem, duplicates are allowed but monotonicity holds.
    std::vector<std::vector<int>> matrix5 = {{1, 1, 2}, {3, 3, 4}};
    assert(search2DMatrix(matrix5, 1) == true);
    assert(search2DMatrix(matrix5, 3) == true);
    assert(search2DMatrix(matrix5, 5) == false);

    return 0;
}

// The key observation is that because each row is sorted and the first element of every row is larger than the last element of the previous row, the matrix is effectively a sorted 1D array of length `rows * cols`. We can perform a standard binary search over the index space `[0, rows*cols - 1]` and map each 1D index `mid` to its 2D coordinates using `row = mid / cols` and `col = mid % cols`. Since division and modulo are integer operations, this mapping is valid for any `mid` in range.  
//
// Edge cases:  
// - If `matrix` is empty (`rows == 0`) or the first row is empty (`cols == 0`), return `false` immediately to avoid division by zero.  
// - The binary search loop uses `start <= end` and updates `start = mid + 1` or `end = mid - 1` to avoid infinite loops; this works correctly because the search space strictly shrinks.  
//
// Time complexity: O(log(rows * cols)) because each iteration halves the search range.  
// Space complexity: O(1) auxiliary space, as we only store a few integers.
