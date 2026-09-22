// Write a C++ function that takes a non-empty square matrix of integers (same number of rows and columns) and sorts each diagonal individually in ascending order, from top-left to bottom-right. A diagonal is defined by all cells where the difference between the row index and column index is constant (i.e., all `mat[i][j]` with the same `i - j` value). The function must modify and return the matrix with each diagonal sorted independently, without changing the relative positions of diagonals. The input matrix will have at least 1 row and 1 column, and all rows will have equal length. The matrix should not be modified directly; instead, return a new matrix with sorted diagonals. The function signature must be `std::vector<std::vector<int>> sortDiagonals(const std::vector<std::vector<int>>& mat)`. Ensure the solution is efficient for large matrices and handles edge cases like 1x1 matrices, matrices with duplicate values, and negative integers.
#include <cassert>
#include <vector>

// Assume sortDiagonals is defined as above.

int main() {
    // Test 1: Simple 3x3 matrix with mixed values.
    std::vector<std::vector<int>> mat1 = {{3, 2, 1}, {4, 5, 6}, {9, 8, 7}};
    std::vector<std::vector<int>> result1 = sortDiagonals(mat1);
    std::vector<std::vector<int>> expected1 = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    assert(result1 == expected1);

    // Test 2: 1x1 matrix – unchanged.
    std::vector<std::vector<int>> mat2 = {{42}};
    std::vector<std::vector<int>> result2 = sortDiagonals(mat2);
    assert(result2 == std::vector<std::vector<int>>{{42}});

    // Test 3: 2x2 matrix with negative numbers and duplicates.
    std::vector<std::vector<int>> mat3 = {{-1, 5}, {-3, -1}};
    std::vector<std::vector<int>> result3 = sortDiagonals(mat3);
    std::vector<std::vector<int>> expected3 = {{-3, 5}, {-1, -1}};
    assert(result3 == expected3);

    // Test 4: 4x4 matrix with larger random values, diagonal lengths vary.
    std::vector<std::vector<int>> mat4 = {
        {9, 8, 7, 6},
        {5, 4, 3, 2},
        {10, 12, 14, 16},
        {1, 3, 5, 7}
    };
    std::vector<std::vector<int>> result4 = sortDiagonals(mat4);
    // Manually verify: diagonal (0,0)={9,4,14,7} -> sorted {4,7,9,14}
    //       (0,1)={8,3,16} -> {3,8,16}
    //       (0,2)={7,2} -> {2,7}
    //       (0,3)={6} -> {6}
    //       (1,0)={5,12,5} -> {5,5,12}
    //       (2,0)={10,3} -> {3,10}
    //       (3,0)={1} -> {1}
    std::vector<std::vector<int>> expected4 = {
        {4, 3, 2, 6},
        {5, 7, 8, 16},
        {5, 10, 9, 3},
        {1, 12, 14, 7}
    };
    // Wait, need precise computation: Let's write out the matrix and diagonals:
    // Matrix:
    // Row0: 9 8 7 6
    // Row1: 5 4 3 2
    // Row2: 10 12 14 16
    // Row3: 1 3 5 7
    // Diagonal key 0: (0,0)=9, (1,1)=4, (2,2)=14, (3,3)=7 -> sorted = {4,7,9,14}
    // So result[0][0]=4, result[1][1]=7, result[2][2]=9, result[3][3]=14
    // Key 1: (0,1)=8, (1,2)=3, (2,3)=16 -> sorted={3,8,16}
    // result[0][1]=3, result[1][2]=8, result[2][3]=16
    // Key 2: (0,2)=7, (1,3)=2 -> sorted={2,7}
    // result[0][2]=2, result[1][3]=7
    // Key 3: (0,3)=6 -> sorted={6}
    // result[0][3]=6
    // Key -1: (1,0)=5, (2,1)=12, (3,2)=5 -> sorted={5,5,12}
    // result[1][0]=5, result[2][1]=5, result[3][2]=12
    // Key -2: (2,0)=10, (3,1)=3 -> sorted={3,10}
    // result[2][0]=3, result[3][1]=10
    // Key -3: (3,0)=1 -> sorted={1}
    // result[3][0]=1
    // Thus expected matrix:
    // Row0: 4 3 2 6
    // Row1: 5 7 8 7  (but careful, row1 col3 is 7, row1 col0=5, row1 col1=7, row1 col2=8, row1 col3=7)
    // Let's build:
    // result[0] = {4,3,2,6}
    // result[1] = {5,7,8,7}
    // result[2] = {3,5,9,16}
    // result[3] = {1,10,12,14}
    std::vector<std::vector<int>> expected4 = {
        {4, 3, 2, 6},
        {5, 7, 8, 7},
        {3, 5, 9, 16},
        {1, 10, 12, 14}
    };
    assert(result4 == expected4);

    // Test 5: 3x3 with all same values.
    std::vector<std::vector<int>> mat5 = {{5,5,5},{5,5,5},{5,5,5}};
    std::vector<std::vector<int>> result5 = sortDiagonals(mat5);
    std::vector<std::vector<int>> expected5 = mat5; // unchanged
    assert(result5 == expected5);

    // Test 6: 2x2 with already sorted diagonals.
    std::vector<std::vector<int>> mat6 = {{1,2},{3,4}};
    std::vector<std::vector<int>> result6 = sortDiagonals(mat6);
    std::vector<std::vector<int>> expected6 = {{1,2},{3,4}};
    assert(result6 == expected6);

    return 0;
}
#include <vector>
#include <queue>
#include <unordered_map>

// Sort each diagonal (top-left to bottom-right) of a square matrix in ascending order.
// Returns a new matrix with sorted diagonals without modifying the input.
std::vector<std::vector<int>> sortDiagonals(const std::vector<std::vector<int>>& mat) {
    int rows = static_cast<int>(mat.size());
    int cols = static_cast<int>(mat[0].size());
    
    // Map diagonal key (row - col) to a min-heap of values on that diagonal.
    std::unordered_map<int, std::priority_queue<int, std::vector<int>, std::greater<int>>> diagonalHeaps;
    
    // First pass: push all elements into the corresponding diagonal heap.
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            diagonalHeaps[i - j].push(mat[i][j]);
        }
    }
    
    // Create result matrix with the same dimensions.
    std::vector<std::vector<int>> result(rows, std::vector<int>(cols));
    
    // Second pass: fill the result matrix by popping smallest values from each heap.
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            int key = i - j;
            result[i][j] = diagonalHeaps[key].top();
            diagonalHeaps[key].pop();
        }
    }
    
    return result;
}
// The core observation is that all cells sharing the same value of `i - j` belong to the same diagonal (where `i` is the row index and `j` is the column index). The indices `i - j` range from `-(n-1)` to `(m-1)` for an `m x n` matrix, but here since it's square, `m == n = k`, so the range is `-(k-1)` to `(k-1)`. A straightforward approach is to use a hash map that maps each diagonal key (`i - j`) to a min-heap (priority queue) of all the elements on that diagonal. First, iterate through every cell and push its value into the heap corresponding to `i - j`. After collecting all values, iterate again in row-major order and for each cell, pop the smallest value from the heap for that diagonal and assign it back to the cell. Since the positions are filled in row-major order, and each diagonal is traversed from top-left to bottom-right during this second pass, the smallest values are placed correctly first, which automatically sorts each diagonal in ascending order. This works because the order in which cells on a diagonal are visited in the second pass is the exact order of the diagonal from top-left to bottom-right; by always popping the smallest remaining value and placing it in that sequence, the diagonal becomes sorted. Edge cases: a 1x1 matrix has only one diagonal, so the value remains unchanged. Duplicate values are handled naturally by the priority queue. Negative integers are fine because the comparison is based on integer value. Time complexity is O(m*n*log(maxDiagonalLength)) because each element is pushed and popped once from a heap, and each heap operation takes O(log L) where L is the length of the longest diagonal (which is O(min(m,n)) = O(k) for a square matrix). Space complexity is O(m*n) to store all elements in the heaps (since the total number of elements across all heaps equals the matrix size). An alternative approach without heaps could extract each diagonal into a vector, sort it, and write it back, which would be O(m*n*log(diagonalLength)) time but with O(k^2) space for the vectors if done naively; the heap approach is clean and intuitive.
