Write a C++ function that takes a non-empty square matrix (represented as a vector of vectors of integers) and returns a vector of integers containing all matrix elements in "snake pattern" order: starting from the top-left corner, traverse the first row left-to-right, then the second row right-to-left, the third row left-to-right, and so on, alternating direction for each row. The input matrix is guaranteed to have at least one row and one column, and all rows have the same length. The function must be const-correct (accept the matrix by const reference) and return a new vector without modifying the input.

#include <cassert>
#include <vector>

// The solution function is declared above; include its definition before this main.

int main() {
    // 1x1 matrix
    std::vector<std::vector<int>> m1 = {{7}};
    assert(snakePattern(m1) == std::vector<int>({7}));

    // 1x4 matrix (single row)
    std::vector<std::vector<int>> m2 = {{1, 2, 3, 4}};
    assert(snakePattern(m2) == std::vector<int>({1, 2, 3, 4}));

    // 2x3 matrix
    std::vector<std::vector<int>> m3 = {{1, 2, 3}, {4, 5, 6}};
    assert(snakePattern(m3) == std::vector<int>({1, 2, 3, 6, 5, 4}));

    // 3x3 matrix
    std::vector<std::vector<int>> m4 = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    assert(snakePattern(m4) == std::vector<int>({1, 2, 3, 6, 5, 4, 7, 8, 9}));

    // 4x2 matrix with negative numbers
    std::vector<std::vector<int>> m5 = {{1, -2}, {3, 4}, {-5, 6}, {7, 8}};
    assert(snakePattern(m5) == std::vector<int>({1, -2, 4, 3, -5, 6, 8, 7}));

    // 3x1 matrix (single column)
    std::vector<std::vector<int>> m6 = {{1}, {2}, {3}};
    assert(snakePattern(m6) == std::vector<int>({1, 2, 3}));

    // Matrix with equal elements
    std::vector<std::vector<int>> m7 = {{5, 5}, {5, 5}};
    assert(snakePattern(m7) == std::vector<int>({5, 5, 5, 5}));

    return 0;
}

#include <vector>

// Return a vector containing all elements of the matrix in snake pattern order.
std::vector<int> snakePattern(const std::vector<std::vector<int>>& matrix) {
    std::vector<int> result;
    if (matrix.empty() || matrix[0].empty()) {
        return result;
    }

    const int rows = static_cast<int>(matrix.size());
    const int cols = static_cast<int>(matrix[0].size());

    bool leftToRight = true;

    for (int row = 0; row < rows; ++row) {
        if (leftToRight) {
            for (int col = 0; col < cols; ++col) {
                result.push_back(matrix[row][col]);
            }
        } else {
            for (int col = cols - 1; col >= 0; --col) {
                result.push_back(matrix[row][col]);
            }
        }
        leftToRight = !leftToRight;
    }

    return result;
}

// The solution iterates over each row in order from top to bottom. A boolean flag tracks whether the current row should be traversed left-to-right (true) or right-to-left (false). For each row, push elements into the result vector using either forward or reverse iteration over the column indices. After processing each row, flip the flag. Edge cases: a 1x1 matrix (single element) produces a single-element result; a matrix with only one row is traversed left-to-right; a matrix with only one column alternates between pushing that single column's element in forward order on odd rows and reverse order (which is identical) on even rows — no special handling needed since the loops work correctly for any column count. Time complexity is O(n) where n is the total number of elements, since each element is visited exactly once. Space complexity is O(n) for the result vector, plus O(1) auxiliary space for the flag and loop indices.
