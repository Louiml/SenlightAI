Write a standalone C++ function named `addMatricesElementwise` that takes two square matrices of equal dimensions, represented as `std::vector<std::vector<int>>` (row-major order), and returns a new matrix of the same dimensions where each element `result[i][j]` equals `A[i][j] + B[i][j]`. The function must handle empty matrices gracefully (return an empty matrix if either input is empty), and assume both matrices are square and of equal size. The function must be `const`-correct: it should accept the input matrices by `const` reference and not modify them. Additionally, the function must work for any size `n x n` where `n` is a non-negative integer. Provide the implementation with appropriate includes and comments.
// The solution is straightforward: verify that both input matrices are non-empty and square (we can assume the task guarantees this, but we still check for empty inputs as an edge case). If either matrix is empty, return an empty `std::vector<std::vector<int>>`. Otherwise, determine the dimension `n` from the first matrix's size (which equals `A.size()` and `B.size()` since they are square and equal). Allocate a result matrix of size `n x n` initialized with zeros. Then iterate over all rows `i` from 0 to `n-1` and all columns `j` from 0 to `n-1`, computing `result[i][j] = A[i][j] + B[i][j]`. This is an element-wise addition, so no special cases for negative numbers or overflow are required (though we could note that overflow is possible with large `int` values, we do not handle it). Time complexity is O(n^2) because we process each of the `n^2` elements once. Space complexity is also O(n^2) for the result matrix, plus O(1) auxiliary space. Edge case: empty matrix triggers an early return to avoid accessing invalid indices.
#include <vector>

// Performs element-wise addition of two square matrices of equal size.
// Returns a new matrix where result[i][j] = A[i][j] + B[i][j].
// If either input matrix is empty, returns an empty matrix.
std::vector<std::vector<int>> addMatricesElementwise(
    const std::vector<std::vector<int>>& A,
    const std::vector<std::vector<int>>& B) 
{
    // Handle empty input gracefully.
    if (A.empty() || B.empty()) {
        return {};
    }

    // Assume both matrices are square and equal size (as per task).
    const std::size_t n = A.size();
    
    // Allocate result matrix initialized with zeros.
    std::vector<std::vector<int>> result(n, std::vector<int>(n, 0));

    // Perform element-wise addition.
    for (std::size_t i = 0; i < n; ++i) {
        for (std::size_t j = 0; j < n; ++j) {
            result[i][j] = A[i][j] + B[i][j];
        }
    }

    return result;
}
#include <cassert>
#include <vector>

// Declaration of the function under test.
std::vector<std::vector<int>> addMatricesElementwise(
    const std::vector<std::vector<int>>& A,
    const std::vector<std::vector<int>>& B);

int main() {
    // Test 1: Basic 2x2 addition.
    std::vector<std::vector<int>> a1 = {{1, 2}, {3, 4}};
    std::vector<std::vector<int>> b1 = {{5, 6}, {7, 8}};
    std::vector<std::vector<int>> r1 = addMatricesElementwise(a1, b1);
    assert(r1 == std::vector<std::vector<int>>({{6, 8}, {10, 12}}));
    
    // Test 2: 1x1 matrix.
    std::vector<std::vector<int>> a2 = {{42}};
    std::vector<std::vector<int>> b2 = {{-8}};
    assert(addMatricesElementwise(a2, b2) == std::vector<std::vector<int>>({{34}}));
    
    // Test 3: 3x3 matrix with zeros.
    std::vector<std::vector<int>> a3 = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}};
    std::vector<std::vector<int>> b3 = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    assert(addMatricesElementwise(a3, b3) == b3);
    
    // Test 4: Empty matrix handling.
    std::vector<std::vector<int>> empty;
    assert(addMatricesElementwise(empty, empty) == empty);
    
    // Test 5: Matrix with negative numbers.
    std::vector<std::vector<int>> a5 = {{-1, -2}, {3, 4}};
    std::vector<std::vector<int>> b5 = {{1, 2}, {-3, -4}};
    assert(addMatricesElementwise(a5, b5) == std::vector<std::vector<int>>({{0, 0}, {0, 0}}));
    
    // Test 6: Large single row (1x4) – squareness not enforced here, but input square by task assumption.
    std::vector<std::vector<int>> a6 = {{1, 2, 3, 4}};
    std::vector<std::vector<int>> b6 = {{4, 3, 2, 1}};
    assert(addMatricesElementwise(a6, b6) == std::vector<std::vector<int>>({{5, 5, 5, 5}}));
    
    // Test 7: Mixed values and larger dimension (4x4).
    std::vector<std::vector<int>> a7 = {{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}, {13, 14, 15, 16}};
    std::vector<std::vector<int>> b7 = {{16, 15, 14, 13}, {12, 11, 10, 9}, {8, 7, 6, 5}, {4, 3, 2, 1}};
    std::vector<std::vector<int>> r7 = {{17, 17, 17, 17}, {17, 17, 17, 17}, {17, 17, 17, 17}, {17, 17, 17, 17}};
    assert(addMatricesElementwise(a7, b7) == r7);
    
    return 0;
}
