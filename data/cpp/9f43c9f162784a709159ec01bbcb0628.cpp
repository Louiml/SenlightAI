/*
Write a C++ function that takes a non-empty matrix of integers (each entry is 0 or 1) represented as a `std::vector<std::vector<int>>` and returns a matrix of the same dimensions where each cell (i, j) contains the difference between (the number of 1s in row i plus the number of 1s in column j) and (the number of 0s in row i plus the number of 0s in column j). The function must compute this efficiently without recalculating row and column sums for every cell individually. The matrix will have at least one row and one column, and all entries will be exactly 0 or 1.
*/
#include <vector>
#include <cstddef>

// Compute a matrix where result[i][j] = (ones in row i + ones in col j) - (zeros in row i + zeros in col j)
// The input matrix is non-empty and all entries are 0 or 1.
std::vector<std::vector<int>> computeDifferenceMatrix(const std::vector<std::vector<int>>& matrix) {
    const std::size_t rows = matrix.size();
    const std::size_t cols = matrix[0].size();
    
    // Precompute the count of 1s in each row and each column.
    std::vector<int> rowOnes(rows, 0);
    std::vector<int> colOnes(cols, 0);
    for (std::size_t i = 0; i < rows; ++i) {
        for (std::size_t j = 0; j < cols; ++j) {
            rowOnes[i] += matrix[i][j];
            colOnes[j] += matrix[i][j];
        }
    }
    
    // Build the result matrix using the formula:
    // difference = 2*rowOnes[i] + 2*colOnes[j] - rows - cols
    std::vector<std::vector<int>> result(rows, std::vector<int>(cols));
    for (std::size_t i = 0; i < rows; ++i) {
        for (std::size_t j = 0; j < cols; ++j) {
            result[i][j] = 2 * rowOnes[i] + 2 * colOnes[j] - static_cast<int>(rows) - static_cast<int>(cols);
        }
    }
    
    return result;
}
#include <cassert>
#include <vector>

// Forward declaration (not needed, but for clarity).
std::vector<std::vector<int>> computeDifferenceMatrix(const std::vector<std::vector<int>>& matrix);

int main() {
    // Test 1: Single 1x1 matrix with a 1.
    {
        std::vector<std::vector<int>> input = {{1}};
        std::vector<std::vector<int>> expected = {{0}}; // 2*1 + 2*1 - 1 - 1 = 0
        assert(computeDifferenceMatrix(input) == expected);
    }
    // Test 2: Single 1x1 matrix with a 0.
    {
        std::vector<std::vector<int>> input = {{0}};
        std::vector<std::vector<int>> expected = {{-2}}; // 0 + 0 - 1 - 1 = -2
        assert(computeDifferenceMatrix(input) == expected);
    }
    // Test 3: 2x2 matrix all zeros.
    {
        std::vector<std::vector<int>> input = {{0, 0}, {0, 0}};
        std::vector<std::vector<int>> expected = {{-4, -4}, {-4, -4}}; // each cell: 2*0+2*0 - 2 - 2 = -4
        assert(computeDifferenceMatrix(input) == expected);
    }
    // Test 4: 2x2 matrix all ones.
    {
        std::vector<std::vector<int>> input = {{1, 1}, {1, 1}};
        std::vector<std::vector<int>> expected = {{2, 2}, {2, 2}}; // each cell: 2*2+2*2 - 2 - 2 = 4? Wait recalc: rowOnes=2, colOnes=2, rows=2, cols=2 => 4+4-4=4
        // Oops, correct expected is 4, not 2. Fix below:
        expected = {{4, 4}, {4, 4}};
        assert(computeDifferenceMatrix(input) == expected);
    }
    // Test 5: Mixed matrix 2x3.
    {
        std::vector<std::vector<int>> input = {{1, 0, 1}, {0, 1, 0}};
        // RowOnes: [2, 1]; ColOnes: [1, 1, 1]; rows=2, cols=3
        // Cell (0,0): 2*2 + 2*1 - 2 - 3 = 4+2-5=1
        // Cell (0,1): 4+2-5=1
        // Cell (0,2): 4+2-5=1
        // Cell (1,0): 2*1+2*1-2-3=2+2-5=-1
        // Cell (1,1): 2+2-5=-1
        // Cell (1,2): 2+2-5=-1
        std::vector<std::vector<int>> expected = {{1, 1, 1}, {-1, -1, -1}};
        assert(computeDifferenceMatrix(input) == expected);
    }
    // Test 6: Single row, multiple columns.
    {
        std::vector<std::vector<int>> input = {{0, 1, 0}};
        // RowOnes: [1]; ColOnes: [0,1,0]; rows=1, cols=3
        // Cell (0,0): 2*1+2*0-1-3 = 2-4 = -2
        // Cell (0,1): 2*1+2*1-4 = 4-4 = 0
        // Cell (0,2): 2*1+2*0-4 = -2
        std::vector<std::vector<int>> expected = {{-2, 0, -2}};
        assert(computeDifferenceMatrix(input) == expected);
    }
    // Test 7: Single column, multiple rows.
    {
        std::vector<std::vector<int>> input = {{1}, {0}, {1}};
        // RowOnes: [1,0,1]; ColOnes: [2]; rows=3, cols=1
        // Cell (0,0): 2*1+2*2-3-1 = 2+4-4=2
        // Cell (1,0): 2*0+4-4=0
        // Cell (2,0): 2*1+4-4=2
        std::vector<std::vector<int>> expected = {{2}, {0}, {2}};
        assert(computeDifferenceMatrix(input) == expected);
    }
    return 0;
}
// The naive approach recomputes row and column counts for each cell, leading to \(O(R \cdot C \cdot (R+C))\) time, which is wasteful. Instead, precompute the number of 1s in each row and each column in a single pass through the matrix: for each row `i`, `rowOnes[i]` accumulates the sum of row i (since entries are 0/1, this equals the count of 1s); similarly, `colOnes[j]` accumulates the sum of column j. For a cell (i, j), the number of 1s in row i plus column j is `rowOnes[i] + colOnes[j]`. The number of 0s in row i is `C - rowOnes[i]` (where C is the number of columns), and the number of 0s in column j is `R - colOnes[j]` (where R is the number of rows). Therefore the total 0s for the cell is `(C - rowOnes[i]) + (R - colOnes[j])`. The difference is thus `rowOnes[i] + colOnes[j] - (C - rowOnes[i]) - (R - colOnes[j]) = 2*rowOnes[i] + 2*colOnes[j] - R - C`. This formula works for any cell, including when the matrix has only one row or column, and there are no edge cases with empty matrices because the input is guaranteed non-empty. Time complexity is \(O(R \cdot C)\) for the precomputation plus \(O(R \cdot C)\) to fill the result, so overall \(O(R \cdot C)\) time; space complexity is \(O(R + C)\) for the row and column arrays, plus the output matrix which is \(O(R \cdot C)\).
