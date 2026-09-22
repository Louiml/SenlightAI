Write a C++ function `int findRowWithMaxOnes(int rows, int cols, const std::vector<std::vector<int>>& grid)` that processes a binary matrix (containing only 0s and 1s) and returns the 1-based row index (i.e., the first row is row 1, not 0) that has the **highest total number of 1s**. If multiple rows have the same maximum count, return the **smallest row index** among them (the one that appears first in the input order). You may assume the matrix has at least one row and one column, and that every cell is either 0 or 1. The function should be const-correct (take the grid by const reference) and should not modify the input. You must not use any external global state or I/O inside the function.
#include <cassert>
#include <vector>

int findRowWithMaxOnes(int rows, int cols, const std::vector<std::vector<int>>& grid);

int main() {
    // Test 1: Simple matrix with clear max
    std::vector<std::vector<int>> grid1 = {{0,1,0}, {1,1,1}, {0,0,1}};
    assert(findRowWithMaxOnes(3, 3, grid1) == 2);

    // Test 2: Tie between rows 1 and 3, should return 1
    std::vector<std::vector<int>> grid2 = {{1,0}, {0,0}, {1,0}};
    assert(findRowWithMaxOnes(3, 2, grid2) == 1);

    // Test 3: All zeros, should return 1
    std::vector<std::vector<int>> grid3 = {{0,0,0}, {0,0,0}, {0,0,0}};
    assert(findRowWithMaxOnes(3, 3, grid3) == 1);

    // Test 4: Single row, single column with 1
    std::vector<std::vector<int>> grid4 = {{1}};
    assert(findRowWithMaxOnes(1, 1, grid4) == 1);

    // Test 5: Single row with all ones
    std::vector<std::vector<int>> grid5 = {{1,1,1,1}};
    assert(findRowWithMaxOnes(1, 4, grid5) == 1);

    // Test 6: Multiple rows, max in the last row
    std::vector<std::vector<int>> grid6 = {{0,0}, {0,1}, {1,1}};
    assert(findRowWithMaxOnes(3, 2, grid6) == 3);

    // Test 7: Row with max count appears first, later rows have same count
    std::vector<std::vector<int>> grid7 = {{1,1}, {0,0}, {1,1}};
    assert(findRowWithMaxOnes(3, 2, grid7) == 1);

    // Test 8: Larger matrix with varying counts
    std::vector<std::vector<int>> grid8 = {
        {1,0,0,0},
        {0,1,1,0},
        {1,1,1,0},
        {0,0,0,1}
    };
    assert(findRowWithMaxOnes(4, 4, grid8) == 3);

    // Test 9: Only one row with some ones
    std::vector<std::vector<int>> grid9 = {{0,1,0,1,0}};
    assert(findRowWithMaxOnes(1, 5, grid9) == 1);

    // Test 10: All ones matrix
    std::vector<std::vector<int>> grid10 = {{1,1}, {1,1}, {1,1}};
    assert(findRowWithMaxOnes(3, 2, grid10) == 1);
}
#include <vector>
#include <algorithm> // for std::max

// Returns the 1-based row index (1..rows) with the highest count of 1s.
// If ties occur, returns the smallest such row index.
int findRowWithMaxOnes(int rows, int cols, const std::vector<std::vector<int>>& grid) {
    int bestRow = 1;          // smallest possible row index
    int maxOnes = -1;         // start below any possible count (including 0)
    
    for (int i = 0; i < rows; ++i) {
        int currentOnes = 0;
        for (int j = 0; j < cols; ++j) {
            currentOnes += grid[i][j];  // since values are only 0 or 1
        }
        if (currentOnes > maxOnes) {    // strictly greater ensures smallest index on ties
            maxOnes = currentOnes;
            bestRow = i + 1;            // convert 0-based to 1-based
        }
    }
    return bestRow;
}
// The problem is straightforward: iterate over each row, count the number of 1s in that row by summing its elements (since the grid is binary, the sum equals the count of 1s). Track the maximum count seen so far and the row index (1-based) where that maximum first occurs. Because we iterate rows in order, when we encounter a count strictly greater than the current maximum, we update both the maximum and the row index. If a later row has the same count, we simply ignore it, preserving the earliest row index. Edge case: if all rows have zero 1s, the maximum count is 0, and the first row (row 1) should be returned because that is the smallest index with maximum count (0). Time complexity is \(O(rows \times cols)\) since we must examine each cell once. Space complexity is \(O(1)\) extra space, excluding the input vector storage.
