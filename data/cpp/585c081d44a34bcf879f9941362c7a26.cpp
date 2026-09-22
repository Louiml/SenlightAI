Write a C++ function named `searchMatrix` that takes a constant reference to a 2D array (fixed size 4×4) of integers and a target integer value. The function should return a `std::vector<std::pair<int, int>>` containing the 1-based row and column indices of every occurrence of the target value in the matrix. If the value is not found, the function should return an empty vector. The search must be performed in row-major order (top-left to bottom-right), and the function must not modify the matrix.

The solution iterates through the entire 4×4 matrix using nested loops (outer over rows, inner over columns). For each element, if it equals the target value, we push a pair `{row + 1, col + 1}` (since indices are 0-based internally, adding 1 converts to 1-based). The function is `const`-correct because it takes the matrix by `const` reference and does not modify any data. Edge cases include: target not present (return empty vector), target present multiple times (return all positions in row-major order), and target equal to values at corners or center. Time complexity is O(rows × cols) = O(16) = O(1) since the size is fixed, but generally O(R×C) for an R×C matrix. Space complexity is O(1) auxiliary plus O(k) for the result vector where k is the number of occurrences.

#include <vector>
#include <utility>

// Returns 1-based positions (row, col) of all occurrences of target in a 4x4 matrix.
std::vector<std::pair<int, int>> searchMatrix(const int matrix[4][4], int target) {
    std::vector<std::pair<int, int>> positions;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (matrix[i][j] == target) {
                positions.push_back({i + 1, j + 1});
            }
        }
    }
    return positions;
}

#include <cassert>
#include <vector>
#include <utility>

// Assume the solution function is declared above.
// (For the test, we re-declare it here.)

std::vector<std::pair<int, int>> searchMatrix(const int matrix[4][4], int target);

int main() {
    int matrix[4][4] = {
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 10, 11, 12},
        {13, 14, 15, 16}
    };

    // Test single occurrence
    auto result = searchMatrix(matrix, 10);
    assert(result.size() == 1);
    assert(result[0] == std::make_pair(3, 2));

    // Test value at top-left
    result = searchMatrix(matrix, 1);
    assert(result.size() == 1);
    assert(result[0] == std::make_pair(1, 1));

    // Test value at bottom-right
    result = searchMatrix(matrix, 16);
    assert(result.size() == 1);
    assert(result[0] == std::make_pair(4, 4));

    // Test value not present
    result = searchMatrix(matrix, 100);
    assert(result.empty());

    // Test duplicate values in a modified matrix
    int dupMatrix[4][4] = {
        {5, 5, 1, 2},
        {3, 5, 5, 4},
        {0, 0, 5, 0},
        {0, 0, 0, 5}
    };
    result = searchMatrix(dupMatrix, 5);
    assert(result.size() == 5);
    assert(result[0] == std::make_pair(1, 1));
    assert(result[1] == std::make_pair(1, 2));
    assert(result[2] == std::make_pair(2, 2));
    assert(result[3] == std::make_pair(2, 3));
    assert(result[4] == std::make_pair(3, 3));

    // Test negative values
    int negMatrix[4][4] = {
        {-1, 0, 2, -1},
        {3, 4, 5, 6},
        {7, 8, -1, 10},
        {11, 12, 13, 14}
    };
    result = searchMatrix(negMatrix, -1);
    assert(result.size() == 3);
    assert(result[0] == std::make_pair(1, 1));
    assert(result[1] == std::make_pair(1, 4));
    assert(result[2] == std::make_pair(3, 3));

    return 0;
}
