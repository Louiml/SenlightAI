// Write a C++ function `vector<int> luckyNumbers(const vector<vector<int>>& matrix)` that returns all lucky numbers in a given rectangular matrix. A lucky number is an element of the matrix such that it is the minimum element in its row and the maximum element in its column. The matrix has at least one row and one column, and all elements are distinct positive integers. The function should return the lucky numbers in any order, but if there are none, return an empty vector. The matrix is guaranteed to have at most one lucky number, but your solution should still handle the general case gracefully.
The key observation is that a lucky number must be the minimum of its row and simultaneously the maximum of its column. A straightforward approach is to precompute two arrays: `rowMin[i]` for the minimum value in row `i`, and `colMax[j]` for the maximum value in column `j`. Then, iterate over every cell `(i, j)` and check if `matrix[i][j] == rowMin[i] && matrix[i][j] == colMax[j]`. If so, push it into the result. Since all values are distinct, at most one lucky number can exist, but this general check is still correct. Edge cases include a single-row or single-column matrix, where the condition still works because each row’s minimum and each column’s maximum are well-defined. The algorithm runs in O(m * n) time and uses O(m + n) extra space for the precomputed arrays. No sorting or advanced data structures are needed.
#include <vector>
#include <algorithm>

// Return all lucky numbers: minimum in row and maximum in column.
std::vector<int> luckyNumbers(const std::vector<std::vector<int>>& matrix) {
    if (matrix.empty() || matrix[0].empty()) return {};
    
    const int rows = matrix.size();
    const int cols = matrix[0].size();
    
    std::vector<int> rowMin(rows, INT_MAX);
    std::vector<int> colMax(cols, INT_MIN);
    
    // Precompute row minimums and column maximums.
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            rowMin[i] = std::min(rowMin[i], matrix[i][j]);
            colMax[j] = std::max(colMax[j], matrix[i][j]);
        }
    }
    
    std::vector<int> result;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (matrix[i][j] == rowMin[i] && matrix[i][j] == colMax[j]) {
                result.push_back(matrix[i][j]);
            }
        }
    }
    return result;
}
#include <cassert>
#include <vector>

int main() {
    // Basic case with one lucky number.
    std::vector<std::vector<int>> m1 = {{3,7,8},{9,11,13},{15,16,17}};
    assert(luckyNumbers(m1) == std::vector<int>({15}));
    
    // No lucky numbers.
    std::vector<std::vector<int>> m2 = {{1,2,3},{4,5,6},{7,8,9}};
    assert(luckyNumbers(m2).empty());
    
    // Single element matrix.
    std::vector<std::vector<int>> m3 = {{5}};
    assert(luckyNumbers(m3) == std::vector<int>({5}));
    
    // Single row: each column's max, only the row minimum qualifies.
    std::vector<std::vector<int>> m4 = {{1,2,3}};
    assert(luckyNumbers(m4) == std::vector<int>({1})); // 1 is min of row and max of its column.
    
    // Single column: each row's min, only the column maximum qualifies.
    std::vector<std::vector<int>> m5 = {{10},{1},{7}};
    assert(luckyNumbers(m5) == std::vector<int>({10})); // 10 is max of column and min of its row.
    
    // Matrix with distinct values and no lucky number.
    std::vector<std::vector<int>> m6 = {{2,5},{3,4}};
    assert(luckyNumbers(m6).empty());
    
    // Larger matrix where lucky number is not at a corner.
    std::vector<std::vector<int>> m7 = {{1,10,4},{2,3,5},{7,8,9}};
    assert(luckyNumbers(m7) == std::vector<int>({7})); // 7 is min in row 2, max in column 0.
    
    // Duplicate values? Task says distinct, but test with distinct to be safe.
    std::vector<std::vector<int>> m8 = {{35,30,20},{36,5,10},{40,8,15}};
    assert(luckyNumbers(m8) == std::vector<int>({35})); // 35: min row0, max col0.
    
    // All rows same minimum and all columns same max? Not possible with distinct, but just check no crash.
    std::vector<std::vector<int>> m9 = {{100,101,102},{103,104,105}};
    assert(luckyNumbers(m9).empty());
    
    return 0;
}
