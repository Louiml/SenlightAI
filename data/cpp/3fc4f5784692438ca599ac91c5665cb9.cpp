// Write a C++ function that takes a square matrix represented as a 2D vector of integers along with its number of rows and columns, and returns a vector of integers containing the elements along the anti-diagonal that runs from the bottom-left corner of the matrix to the top-right corner, but only for the minimum dimension of the matrix. Specifically, if the matrix has dimensions n x m, the anti-diagonal starts at the bottom-left element (position n-1,0) and moves diagonally up-right (decreasing row index, increasing column index) for p = min(n,m) elements. The function should return these p elements in the order they are traversed (from bottom-left to top-right). The matrix may be non-square, and you must handle cases where n or m is zero (return an empty vector).
#include <cassert>
#include <vector>

int main() {
    // Test 1: 3x3 square matrix
    std::vector<std::vector<int>> mat1 = {{1,2,3},{4,5,6},{7,8,9}};
    std::vector<int> res1 = antiDiagonal(mat1, 3, 3);
    assert(res1 == std::vector<int>({7,5,3}));

    // Test 2: Rectangular 2x4 matrix (min=2)
    std::vector<std::vector<int>> mat2 = {{1,2,3,4},{5,6,7,8}};
    std::vector<int> res2 = antiDiagonal(mat2, 2, 4);
    assert(res2 == std::vector<int>({5,2}));

    // Test 3: Rectangular 4x2 matrix (min=2)
    std::vector<std::vector<int>> mat3 = {{1,2},{3,4},{5,6},{7,8}};
    std::vector<int> res3 = antiDiagonal(mat3, 4, 2);
    assert(res3 == std::vector<int>({3,2}));

    // Test 4: Single element matrix
    std::vector<std::vector<int>> mat4 = {{42}};
    std::vector<int> res4 = antiDiagonal(mat4, 1, 1);
    assert(res4 == std::vector<int>({42}));

    // Test 5: Zero rows (empty matrix)
    std::vector<std::vector<int>> mat5;
    std::vector<int> res5 = antiDiagonal(mat5, 0, 3);
    assert(res5.empty());

    // Test 6: Zero columns
    std::vector<std::vector<int>> mat6 = {{}, {}, {}};
    std::vector<int> res6 = antiDiagonal(mat6, 3, 0);
    assert(res6.empty());

    // Test 7: Matrix with negative numbers, 2x2
    std::vector<std::vector<int>> mat7 = {{-1,2},{-3,4}};
    std::vector<int> res7 = antiDiagonal(mat7, 2, 2);
    assert(res7 == std::vector<int>({-3,2}));

    return 0;
}
#include <vector>
#include <algorithm>

// Return the anti-diagonal from bottom-left to top-right, limited to min(rows, cols) elements.
std::vector<int> antiDiagonal(const std::vector<std::vector<int>>& matrix, int rows, int cols) {
    std::vector<int> result;
    int p = std::min(rows, cols);
    result.reserve(p);
    for (int i = 0; i < p; ++i) {
        result.push_back(matrix[p - 1 - i][i]);
    }
    return result;
}
// The main algorithm is straightforward: identify the number of elements to collect as the smaller of the two dimensions, call it `p`. Then iterate i from 0 to p-1, and for each i, access the element at row `(p-1-i)` and column `i`. This is because the starting point is the bottom-left corner of the submatrix defined by the first p rows and p columns, which is row p-1, column 0. As i increases by 1, the row decreases by 1 and the column increases by 1, so each step moves up-right along the anti-diagonal. Edge cases include a zero dimension (return empty vector), a single element matrix (return that element), and non-square matrices where the anti-diagonal is limited by the smaller dimension. The time complexity is O(p) where p = min(n,m), and the space complexity is O(p) for the result vector, plus O(1) auxiliary space.
