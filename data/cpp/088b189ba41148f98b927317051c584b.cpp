// Write a C++ function named `findInMatrix` that takes a constant 2D integer array (with exactly 4 columns) and a target integer, and returns a `std::string` describing the search result. If the target is found, return a string in the format `"Element found at row X and column Y!"` where X and Y are 1-based row and column numbers (e.g., row 2, column 3 means the second row and third column). If the target is not found, return `"The element does not exist in the matrix!"`. The matrix is fixed-size with 2 rows and 4 columns, and all values are positive integers. The function must be `const`-correct and not modify the input matrix.
#include <cassert>
#include <string>

// Assume the solution function is declared above.
int main() {
    const int matrix[2][4] = {
        {1, 2, 3, 4},
        {5, 6, 7, 8}
    };

    assert(findInMatrix(matrix, 3) == "Element found at row 1 and column 3!");
    assert(findInMatrix(matrix, 7) == "Element found at row 2 and column 3!");
    assert(findInMatrix(matrix, 1) == "Element found at row 1 and column 1!");
    assert(findInMatrix(matrix, 8) == "Element found at row 2 and column 4!");
    assert(findInMatrix(matrix, 10) == "The element does not exist in the matrix!");
    assert(findInMatrix(matrix, 0) == "The element does not exist in the matrix!");
    assert(findInMatrix(matrix, 9) == "The element does not exist in the matrix!");
    assert(findInMatrix(matrix, 6) == "Element found at row 2 and column 2!");
    // Test duplicate by using a different matrix
    const int dupMatrix[2][4] = {{5, 5, 5, 5}, {5, 5, 5, 5}};
    assert(findInMatrix(dupMatrix, 5) == "Element found at row 1 and column 1!");
}
#include <string>

// Search for target in a fixed 2x4 matrix. Returns a descriptive string.
std::string findInMatrix(const int matrix[2][4], int target) {
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (matrix[i][j] == target) {
                return "Element found at row " + std::to_string(i + 1) +
                       " and column " + std::to_string(j + 1) + "!";
            }
        }
    }
    return "The element does not exist in the matrix!";
}
// The solution uses a nested loop to iterate through all 2 rows and 4 columns of the matrix. For each cell, compare the stored value to the target. If a match is found, immediately construct and return the success message using the current 1-based indices (row index + 1 and column index + 1). Since the problem asks for only the first occurrence (matching the original snippet's behavior), return on the first match—this also avoids unnecessary further scanning. If the loops complete without any match, return the failure message. Edge case: if the target appears multiple times, only the first occurrence (row-major order) is reported. Time complexity is O(8) constant since the matrix size is fixed, but generally O(R*C) for a matrix of R rows and C columns; space complexity is O(1) auxiliary, plus O(1) for the returned string size (constant length messages). No special handling for negative numbers is needed because the problem states all values are positive.
