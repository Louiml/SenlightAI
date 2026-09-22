// Write a C++ function `analyzeRowCol` that takes a non-empty 2D vector of integers (representing a matrix), a row index `r`, and a column index `c`, and returns a `std::pair<std::vector<int>, std::vector<int>>` where the first vector contains the elements of the specified row (in order from column 0 to the last column), and the second vector contains the elements of the specified column (in order from row 0 to the last row). The function must compute and include, as the last element of each returned vector, the sum of that row or column (so each returned vector has size = number of elements in that row/column + 1). The input matrix must have at least one row and one column, and the row and column indices are guaranteed to be valid (between 0 and size-1). No output is printed; the function returns data only.
#include <cassert>
#include <vector>
#include <utility>

int main() {
    // Test case 1: 3x3 matrix, row 1, col 2
    std::vector<std::vector<int>> m1 = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    auto res1 = analyzeRowCol(m1, 1, 2);
    assert(res1.first == std::vector<int>({4, 5, 6, 15}));
    assert(res1.second == std::vector<int>({3, 6, 9, 18}));
    
    // Test case 2: 2x4 matrix, row 0, col 3
    std::vector<std::vector<int>> m2 = {{10, 20, 30, 40}, {50, 60, 70, 80}};
    auto res2 = analyzeRowCol(m2, 0, 3);
    assert(res2.first == std::vector<int>({10, 20, 30, 40, 100}));
    assert(res2.second == std::vector<int>({40, 80, 120}));
    
    // Test case 3: single row, single column
    std::vector<std::vector<int>> m3 = {{7}};
    auto res3 = analyzeRowCol(m3, 0, 0);
    assert(res3.first == std::vector<int>({7, 7}));
    assert(res3.second == std::vector<int>({7, 7}));
    
    // Test case 4: single row, multiple columns
    std::vector<std::vector<int>> m4 = {{1, -2, 3}};
    auto res4 = analyzeRowCol(m4, 0, 1);
    assert(res4.first == std::vector<int>({1, -2, 3, 2}));
    assert(res4.second == std::vector<int>({-2, -2}));
    
    // Test case 5: multiple rows, single column
    std::vector<std::vector<int>> m5 = {{5}, {10}, {15}};
    auto res5 = analyzeRowCol(m5, 2, 0);
    assert(res5.first == std::vector<int>({15, 15}));
    assert(res5.second == std::vector<int>({5, 10, 15, 30}));
    
    // Test case 6: zero and negative values
    std::vector<std::vector<int>> m6 = {{0, -1}, {-2, 3}};
    auto res6 = analyzeRowCol(m6, 0, 1);
    assert(res6.first == std::vector<int>({0, -1, -1}));
    assert(res6.second == std::vector<int>({-1, 3, 2}));
    
    return 0;
}
#include <vector>
#include <numeric>
#include <utility>

// Returns a pair of vectors: first contains row r elements plus their sum, second contains column c elements plus their sum.
std::pair<std::vector<int>, std::vector<int>> analyzeRowCol(const std::vector<std::vector<int>>& matrix, int r, int c) {
    const int rows = static_cast<int>(matrix.size());
    const int cols = static_cast<int>(matrix[0].size());
    
    std::vector<int> rowData;
    rowData.reserve(cols + 1);
    int rowSum = 0;
    for (int j = 0; j < cols; ++j) {
        rowData.push_back(matrix[r][j]);
        rowSum += matrix[r][j];
    }
    rowData.push_back(rowSum);
    
    std::vector<int> colData;
    colData.reserve(rows + 1);
    int colSum = 0;
    for (int i = 0; i < rows; ++i) {
        colData.push_back(matrix[i][c]);
        colSum += matrix[i][c];
    }
    colData.push_back(colSum);
    
    return {rowData, colData};
}
// The solution iterates over the given row index `r`, traversing all columns (`j` from 0 to cols-1) and copying each element into the first result vector, while accumulating the sum. After the loop, it appends the sum to that vector. Similarly, for the column index `c`, iterate over all rows (`i` from 0 to rows-1), copying each element into the second result vector and accumulating the sum, then append the sum. Edge cases: the matrix may be single-row or single-column, but indices are valid, so no boundary errors occur. The row and column may overlap, but that does not affect the computation since we compute them independently. Time complexity is O(rows + cols) because we scan the entire row (cols elements) and the entire column (rows elements). Space complexity is O(rows + cols) for the two output vectors.
