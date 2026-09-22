// Write a C++ function that takes a 2D integer vector (matrix) with at least one element, and returns a `std::pair<int, std::pair<int, int>>` representing the minimum element and its (row, column) position. The function must handle negative numbers, duplicate minima (return the first occurrence in row-major order), and matrices of any positive size. The input matrix is passed as `const std::vector<std::vector<int>>&` for read-only access. The function should not use any global state or random generation; it should be purely deterministic. The returned pair must contain the minimum value as the first member, and the position as a nested pair (row, column) zero-indexed.

The solution iterates through the matrix in row-major order (row by row, then column by column), tracking the current minimum and its position. Initialize the first element as the minimum, and then for every subsequent element, if it is strictly less than the current minimum, update the minimum and its row/column indices. Using strict `<` ensures that the first occurrence of a duplicate minimum is retained, as required. Edge cases include matrices with a single row or a single column (still handled by the generic loop), negative numbers (comparison works naturally), and duplicate values. Time complexity is O(R×C) where R is the number of rows and C is the number of columns, because every element is visited exactly once. Space complexity is O(1) auxiliary, since only a few scalar variables are used; the input matrix itself is not copied.

#include <vector>
#include <utility>

// Return the minimum element and its (row, col) position in the matrix.
// The matrix is assumed to be non-empty and rectangular.
std::pair<int, std::pair<int, int>> minElementWithPosition(const std::vector<std::vector<int>>& matrix) {
    // Initialize with the first element
    int minVal = matrix[0][0];
    int minRow = 0;
    int minCol = 0;

    // Iterate row-major order
    for (int i = 0; i < matrix.size(); ++i) {
        for (int j = 0; j < matrix[i].size(); ++j) {
            if (matrix[i][j] < minVal) {
                minVal = matrix[i][j];
                minRow = i;
                minCol = j;
            }
        }
    }

    return {minVal, {minRow, minCol}};
}

#include <cassert>
#include <vector>
#include <utility>

// Include the solution function here (or in a header)

int main() {
    // Test 1: Basic 2x2 matrix
    std::vector<std::vector<int>> m1 = {{5, 3}, {8, 1}};
    assert(minElementWithPosition(m1) == std::make_pair(1, std::make_pair(1, 1)));

    // Test 2: Negative numbers and duplicates (first occurrence should be returned)
    std::vector<std::vector<int>> m2 = {{-2, -2, 0}, {3, -5, -5}};
    assert(minElementWithPosition(m2) == std::make_pair(-5, std::make_pair(1, 1)));

    // Test 3: Single element matrix
    std::vector<std::vector<int>> m3 = {{42}};
    assert(minElementWithPosition(m3) == std::make_pair(42, std::make_pair(0, 0)));

    // Test 4: Single row matrix
    std::vector<std::vector<int>> m4 = {{7, 2, 9}};
    assert(minElementWithPosition(m4) == std::make_pair(2, std::make_pair(0, 1)));

    // Test 5: Single column matrix
    std::vector<std::vector<int>> m5 = {{4}, {0}, {-1}};
    assert(minElementWithPosition(m5) == std::make_pair(-1, std::make_pair(2, 0)));

    // Test 6: All identical values -> first element (0,0)
    std::vector<std::vector<int>> m6 = {{3, 3}, {3, 3}};
    assert(minElementWithPosition(m6) == std::make_pair(3, std::make_pair(0, 0)));

    // Test 7: Larger irregular? Actually assume rectangular; test with 3x4
    std::vector<std::vector<int>> m7 = {{10, 20, 30, 40}, {5, 6, 7, 8}, {9, -1, 2, 3}};
    assert(minElementWithPosition(m7) == std::make_pair(-1, std::make_pair(2, 1)));

    // Test 8: Minimum in top-right corner
    std::vector<std::vector<int>> m8 = {{9, 8, 0}, {1, 2, 3}};
    assert(minElementWithPosition(m8) == std::make_pair(0, std::make_pair(0, 2)));

    return 0;
}
