/*
Write a standalone C++ function named `findMatrixMinimum` that accepts a two-dimensional integer matrix represented as a `std::vector<std::vector<int>>` (with at least one row and one column, and containing no values beyond the normal int range) and returns the smallest element in that matrix. The function must handle matrices of any rectangular size (all rows have the same number of columns), including cases where all elements are equal or where the minimum appears multiple times. The function should be `const`-correct, taking the matrix by const reference, and must not modify the input. You may assume the matrix is non-empty and rectangular.
*/

#include <vector>
#include <limits>

// Returns the smallest element in a non-empty rectangular matrix.
int findMatrixMinimum(const std::vector<std::vector<int>>& matrix) {
    int min = std::numeric_limits<int>::max();
    for (const auto& row : matrix) {
        for (int value : row) {
            if (value < min) {
                min = value;
            }
        }
    }
    return min;
}

#include <cassert>
#include <vector>

// The solution function is assumed to be declared above main.
// Include the function definition here as well for standalone compilation.

int main() {
    // Single element
    std::vector<std::vector<int>> m1 = {{42}};
    assert(findMatrixMinimum(m1) == 42);

    // Multiple rows and columns with distinct values
    std::vector<std::vector<int>> m2 = {{5, 8, 3}, {9, 1, 7}, {4, 6, 2}};
    assert(findMatrixMinimum(m2) == 1);

    // Negative values and zeros
    std::vector<std::vector<int>> m3 = {{-10, 0, 5}, {-3, -100, 2}, {7, 8, -1}};
    assert(findMatrixMinimum(m3) == -100);

    // All equal values
    std::vector<std::vector<int>> m4 = {{7, 7}, {7, 7}};
    assert(findMatrixMinimum(m4) == 7);

    // Single row with multiple columns
    std::vector<std::vector<int>> m5 = {{3, -5, 8, 0}};
    assert(findMatrixMinimum(m5) == -5);

    // Single column with multiple rows
    std::vector<std::vector<int>> m6 = {{10}, {-2}, {4}};
    assert(findMatrixMinimum(m6) == -2);

    // Large values and duplicates
    std::vector<std::vector<int>> m7 = {{1000, -1, -1}, {-1, 5000, 2}};
    assert(findMatrixMinimum(m7) == -1);

    // Minimum appears multiple times
    std::vector<std::vector<int>> m8 = {{-5, 3}, {-5, 8}, {1, -5}};
    assert(findMatrixMinimum(m8) == -5);

    return 0;
}

// The core algorithm is a straightforward nested loop over all rows and columns. Initialize a `min` variable to the largest possible `int` value (e.g., `std::numeric_limits<int>::max()`), then compare each element against it, updating `min` whenever a smaller value is found. This guarantees correct behavior even if the matrix contains very small negative numbers. Since the matrix is rectangular, indexing with row-major order is safe, and no special handling is needed for uneven rows or empty input. For a matrix with \(r\) rows and \(c\) columns, the time complexity is \(O(r \cdot c)\) because every element is examined exactly once. The space complexity is \(O(1)\), as only a single integer variable is needed for tracking the minimum, plus the space already used by the input matrix.
