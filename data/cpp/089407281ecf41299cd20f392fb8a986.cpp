/*
Write a C++ function `matrixMultiply` that takes two two-dimensional integer vectors `A` and `B` as input, performs standard matrix multiplication, and returns the resulting product matrix as a `vector<vector<int>>`. The function must validate that the number of columns of `A` equals the number of rows of `B`. If the dimensions are incompatible, the function should return an empty `vector<vector<int>>`. The function must handle any non-empty rectangular input matrices (all rows of the same length) and should not modify the inputs.
*/

#include <vector>
#include <cstddef>

// Perform matrix multiplication of two rectangular integer matrices.
// Returns the product matrix. If dimensions are incompatible or matrices are not rectangular, returns an empty matrix.
std::vector<std::vector<int>> matrixMultiply(
    const std::vector<std::vector<int>>& A, 
    const std::vector<std::vector<int>>& B) {
    
    // Validate that both matrices are non-empty and rectangular.
    if (A.empty() || B.empty()) return {};
    const std::size_t m = A.size();
    const std::size_t n = A[0].size();
    const std::size_t p = B.size();
    const std::size_t q = B[0].size();
    
    // Check that all rows in A have the same length.
    for (const auto& row : A) {
        if (row.size() != n) return {};
    }
    // Check that all rows in B have the same length.
    for (const auto& row : B) {
        if (row.size() != q) return {};
    }
    
    // Multiplication is only possible if inner dimensions match.
    if (n != p) return {};
    
    // Initialize result matrix with zeros.
    std::vector<std::vector<int>> result(m, std::vector<int>(q, 0));
    
    // Standard triple-loop matrix multiplication.
    for (std::size_t i = 0; i < m; ++i) {
        for (std::size_t j = 0; j < q; ++j) {
            for (std::size_t k = 0; k < n; ++k) {
                result[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    
    return result;
}

#include <cassert>
#include <vector>

// Declare the solution function (already defined above).
std::vector<std::vector<int>> matrixMultiply(
    const std::vector<std::vector<int>>& A, 
    const std::vector<std::vector<int>>& B);

int main() {
    // Case 1: Given example from snippet (2x2 times 2x2 via extraction from 4x2 and 2x4).
    std::vector<std::vector<int>> A1 = {{1,2},{3,4}};
    std::vector<std::vector<int>> B1 = {{5,6},{7,8}};
    std::vector<std::vector<int>> R1 = matrixMultiply(A1, B1);
    assert(R1 == std::vector<std::vector<int>>({{19,22},{43,50}}));

    // Case 2: Non-square multiplication 3x2 times 2x4.
    std::vector<std::vector<int>> A2 = {{1,2},{3,4},{5,6}};
    std::vector<std::vector<int>> B2 = {{1,2,3,4},{5,6,7,8}};
    std::vector<std::vector<int>> R2 = matrixMultiply(A2, B2);
    assert(R2 == std::vector<std::vector<int>>({{11,14,17,20},{23,30,37,44},{35,46,57,68}}));

    // Case 3: 1x1 matrices.
    std::vector<std::vector<int>> A3 = {{7}};
    std::vector<std::vector<int>> B3 = {{9}};
    assert(matrixMultiply(A3, B3) == std::vector<std::vector<int>>({{63}}));

    // Case 4: Incompatible dimensions (2x3 times 2x2) should return empty.
    std::vector<std::vector<int>> A4 = {{1,2,3},{4,5,6}};
    std::vector<std::vector<int>> B4 = {{1,2},{3,4}};
    assert(matrixMultiply(A4, B4).empty());

    // Case 5: Empty matrix as input should return empty.
    std::vector<std::vector<int>> A5;
    std::vector<std::vector<int>> B5 = {{1,2}};
    assert(matrixMultiply(A5, B5).empty());

    // Case 6: Multiplication with identity matrix (2x2).
    std::vector<std::vector<int>> A6 = {{1,2},{3,4}};
    std::vector<std::vector<int>> I6 = {{1,0},{0,1}};
    assert(matrixMultiply(A6, I6) == A6);
    assert(matrixMultiply(I6, A6) == A6);

    // Case 7: Zero matrix result.
    std::vector<std::vector<int>> A7 = {{1,0},{0,1}};
    std::vector<std::vector<int>> B7 = {{0,0},{0,0}};
    assert(matrixMultiply(A7, B7) == std::vector<std::vector<int>>({{0,0},{0,0}}));

    // Case 8: 2x3 times 3x2 yields 2x2.
    std::vector<std::vector<int>> A8 = {{1,2,3},{4,5,6}};
    std::vector<std::vector<int>> B8 = {{7,8},{9,10},{11,12}};
    std::vector<std::vector<int>> R8 = matrixMultiply(A8, B8);
    assert(R8 == std::vector<std::vector<int>>({{58,64},{139,154}}));

    return 0;
}

// The solution uses the standard triple-nested loop algorithm. Let `A` be `m × n` and `B` be `p × q`. Multiplication is only possible when `n == p`; otherwise, return an empty matrix. For a valid case, allocate the result as an `m × q` matrix initialized to zeros. For each cell `(i, j)`, compute the dot product of row `i` of `A` and column `j` of `B` by iterating over `k` from 0 to `n-1`, summing `A[i][k] * B[k][j]`. Important edge cases include: an input matrix with zero rows (should be treated as invalid or return empty), input matrix with inconsistent row lengths (precondition: assume rectangular but can add a defensive check to return empty if any row has different length from the first row), and dimension mismatch with `n != p`. The time complexity is O(m * n * q) = O(m * p * q) since `n == p`, and the auxiliary space complexity is O(m * q) for the result, excluding input storage.
