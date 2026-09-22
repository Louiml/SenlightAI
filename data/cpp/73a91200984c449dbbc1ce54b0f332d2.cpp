Write a standalone C++ function that takes a 2D vector of characters (`std::vector<std::vector<char>>`) representing a binary matrix (each cell is either `'0'` or `'1'`) and returns the area of the largest square consisting entirely of `'1'`s within the matrix. The matrix may be empty (zero rows), but if it has at least one row, every row must have the same non-zero length (i.e., a rectangular matrix). The function must be named `largestSquareArea` and accept the matrix by const reference. The result should be an integer: the maximum area (side length squared). If the matrix is empty or contains no `'1'` cells, the result must be `0`. The solution must be efficient for large inputs (up to 300×300).
#include <cassert>
#include <vector>

// Function declaration (provided from solution)
int largestSquareArea(const std::vector<std::vector<char>>& matrix);

int main() {
    // Test 1: Empty matrix
    std::vector<std::vector<char>> empty;
    assert(largestSquareArea(empty) == 0);

    // Test 2: Single row with no ones
    std::vector<std::vector<char>> singleRowZero = {{'0','0','0'}};
    assert(largestSquareArea(singleRowZero) == 0);

    // Test 3: Single row with a one
    std::vector<std::vector<char>> singleRowOne = {{'0','1','0'}};
    assert(largestSquareArea(singleRowOne) == 1);

    // Test 4: Single column with multiple ones
    std::vector<std::vector<char>> singleCol = {{'1'}, {'1'}, {'1'}};
    assert(largestSquareArea(singleCol) == 1);

    // Test 5: 2x2 all ones
    std::vector<std::vector<char>> allOnes2x2 = {{'1','1'}, {'1','1'}};
    assert(largestSquareArea(allOnes2x2) == 4);

    // Test 6: 3x3 with a full 2x2 square only
    std::vector<std::vector<char>> mixed = {
        {'1','0','1'},
        {'1','1','0'},
        {'1','1','0'}
    };
    assert(largestSquareArea(mixed) == 4);

    // Test 7: 3x3 all ones gives area 9
    std::vector<std::vector<char>> allOnes3x3 = {
        {'1','1','1'},
        {'1','1','1'},
        {'1','1','1'}
    };
    assert(largestSquareArea(allOnes3x3) == 9);

    // Test 8: Larger rectangle, no square larger than 1
    std::vector<std::vector<char>> noBigSquare = {
        {'1','0','1','0'},
        {'0','1','0','1'},
        {'1','0','1','0'}
    };
    assert(largestSquareArea(noBigSquare) == 1);

    // Test 9: Non-square rectangle with 2x2 square
    std::vector<std::vector<char>> rect = {
        {'1','1','1','0'},
        {'1','1','1','1'},
        {'1','1','0','1'}
    };
    assert(largestSquareArea(rect) == 4);

    // Test 10: Matrix with a 3x3 square inside larger rectangle
    std::vector<std::vector<char>> withBigSquare = {
        {'1','1','1','1'},
        {'1','1','1','1'},
        {'1','1','1','1'},
        {'1','1','1','1'}
    };
    assert(largestSquareArea(withBigSquare) == 16);

    return 0;
}
#include <vector>
#include <algorithm>

// Returns the area of the largest square of '1's in a binary matrix.
int largestSquareArea(const std::vector<std::vector<char>>& matrix) {
    if (matrix.empty()) return 0;
    int rows = static_cast<int>(matrix.size());
    int cols = static_cast<int>(matrix[0].size());
    
    // DP table: dp[i][j] = side length of largest square ending at (i,j)
    std::vector<std::vector<int>> dp(rows, std::vector<int>(cols, 0));
    int maxSide = 0;
    
    // Initialize first row and column
    for (int i = 0; i < rows; ++i) {
        if (matrix[i][0] == '1') {
            dp[i][0] = 1;
            maxSide = std::max(maxSide, 1);
        }
    }
    for (int j = 0; j < cols; ++j) {
        if (matrix[0][j] == '1') {
            dp[0][j] = 1;
            maxSide = std::max(maxSide, 1);
        }
    }
    
    // Fill remaining cells using recurrence
    for (int i = 1; i < rows; ++i) {
        for (int j = 1; j < cols; ++j) {
            if (matrix[i][j] == '1') {
                dp[i][j] = 1 + std::min({dp[i-1][j], dp[i][j-1], dp[i-1][j-1]});
                maxSide = std::max(maxSide, dp[i][j]);
            }
        }
    }
    
    return maxSide * maxSide;
}
// The optimal approach uses dynamic programming. Let `dp[i][j]` represent the side length of the largest square of all ones whose bottom-right corner is at cell `(i, j)`. For any cell containing `'1'`, the recurrence is: `dp[i][j] = 1 + min(dp[i-1][j], dp[i][j-1], dp[i-1][j-1])`, because to form a larger square, the cell must extend a square from the top, left, and top-left neighbors. For cells in the first row or first column, `dp[i][j]` is simply `1` if the cell is `'1'`, else `0`. The answer is the square of the maximum value in `dp`. Important edge cases: an empty matrix (zero rows) – return `0` immediately; a matrix with a single row or single column – the maximum side length is at most 1, so if any `'1'` exists, the area is 1, else 0. The algorithm runs in O(n*m) time and uses O(n*m) auxiliary space for the DP table, which is optimal for this problem. We can also use a rolling array to reduce space to O(m), but for clarity and to match the original snippet we use a full DP table. The code must handle `const` correctness and avoid modifying the input.
