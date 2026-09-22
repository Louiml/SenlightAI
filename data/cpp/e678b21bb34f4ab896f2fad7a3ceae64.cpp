// Write a C++ function that takes a non-empty rectangular matrix of integers and returns a vector containing all "lucky numbers" in the matrix. A lucky number is defined as an element that is the minimum value in its row and simultaneously the maximum value in its column. The returned vector may contain multiple elements (in any order) if more than one such number exists; if none exist, return an empty vector. The matrix dimensions are at least 1×1.

#include <cassert>
#include <vector>

// (The solution function is assumed to be defined above; including it here for completeness.)
int main() {
    // Example from LeetCode-style cases.
    std::vector<std::vector<int>> mat1 = {{3, 7, 8}, {9, 11, 13}, {15, 16, 17}};
    std::vector<int> res1 = luckyNumbers(mat1);
    assert(res1.size() == 1 && res1[0] == 15);
    
    // 1x1 matrix.
    std::vector<std::vector<int>> mat2 = {{5}};
    std::vector<int> res2 = luckyNumbers(mat2);
    assert(res2.size() == 1 && res2[0] == 5);
    
    // No lucky numbers.
    std::vector<std::vector<int>> mat3 = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    std::vector<int> res3 = luckyNumbers(mat3);
    assert(res3.empty());
    
    // Multiple lucky numbers (diagonal pattern).
    std::vector<std::vector<int>> mat4 = {{1, 10, 4}, {2, 3, 20}};
    std::vector<int> res4 = luckyNumbers(mat4);
    // Expected: 2 (row min of row 0? No—row0 min=1 but col0 max=7? Let's compute manually:
    // Row0 min=1, Row1 min=2. Col0 max=7, Col1 max=10, Col2 max=20.
    // Element 2 is row1 min and also col0 max (since col0 max is 2? Actually col0 = {1,2} max=2). So 2 is lucky.
    // Element 10 is col1 max, but row0 min=1 not 10. Element 20 is col2 max, but row1 min=2. So only 2 is lucky.
    assert(res4.size() == 1 && res4[0] == 2);
    
    // Case with negative numbers and duplicate extremes.
    std::vector<std::vector<int>> mat5 = {{-3, -1}, {-2, -4}};
    // Row0 min=-3, Row1 min=-4. Col0 max=-2, Col1 max=-1.
    // Element -3? Not col max. -1? Row0 min not. -2? Row1 min? no, row1 min=-4. -4? col? col1 max=-1. So none.
    std::vector<int> res5 = luckyNumbers(mat5);
    assert(res5.empty());
    
    // Rectangular matrix where multiple lucky numbers exist.
    std::vector<std::vector<int>> mat6 = {{2, 2}, {2, 2}};
    // All elements are row min=2 and col max=2 → all four are lucky.
    std::vector<int> res6 = luckyNumbers(mat6);
    assert(res6.size() == 4);
    
    // Single row.
    std::vector<std::vector<int>> mat7 = {{1, 2, 3}};
    // Row min=1, col max[0]=1, col max[1]=2, col max[2]=3. Only 1 qualifies (col0 max=1) because 2 and 3 are not row mins.
    std::vector<int> res7 = luckyNumbers(mat7);
    assert(res7.size() == 1 && res7[0] == 1);
    
    return 0;
}

#include <vector>
#include <algorithm>
#include <climits>

// Return all elements that are the minimum in their row and maximum in their column.
std::vector<int> luckyNumbers(const std::vector<std::vector<int>>& matrix) {
    if (matrix.empty() || matrix[0].empty()) return {};
    
    const int rows = static_cast<int>(matrix.size());
    const int cols = static_cast<int>(matrix[0].size());
    
    std::vector<int> rowMin(rows, INT_MAX);
    std::vector<int> colMax(cols, INT_MIN);
    
    // First pass: compute row minima and column maxima.
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            rowMin[i] = std::min(rowMin[i], matrix[i][j]);
            colMax[j] = std::max(colMax[j], matrix[i][j]);
        }
    }
    
    // Second pass: collect lucky numbers.
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

// The core idea is to precompute for each row its minimum value and for each column its maximum value in two separate arrays. First, iterate through the entire matrix once, updating `rowMin[i] = min(rowMin[i], matrix[i][j])` and `colMax[j] = max(colMax[j], matrix[i][j])`. After that, perform a second pass over all elements. For each element, check whether it equals its row's minimum and its column's maximum; if so, append it to the result. Since the conditions are strict (equality), duplicates are naturally handled—if a row has two equal minima, neither will typically be a column maximum unless the column maxima coincide. Edge cases include a 1×1 matrix (the single element is lucky), and matrices where no element satisfies both conditions (returns empty). Time complexity is O(n·m) for the two passes (constant factor), space complexity is O(n + m) for the auxiliary arrays, excluding the output vector.
