Write a C++ function `matrixProduct` that takes two 2D vectors of integers representing matrices A (dimensions a1 x a2) and B (dimensions b1 x b2), and returns the product matrix as a 2D vector of integers. If the matrices cannot be multiplied because the number of columns of A (a2) does not equal the number of rows of B (b1), the function should return an empty 2D vector. Assume the input vectors are non-empty and all rows have the same length (i.e., valid rectangular matrices). The function must compute the standard matrix product, where each element at position (i,j) is the sum of products of elements from row i of A and column j of B.
// The solution follows the standard matrix multiplication algorithm. We first validate the dimensions: if `A[0].size() != B.size()`, multiplication is impossible, so return `{}` (an empty vector). Otherwise, we create a result matrix of size `A.size() x B[0].size()`, initialized to zeros. Then triple-nested loops: for each row `i` of A, for each column `j` of B, compute the dot product over the shared dimension `k` (which must equal A's column count and B's row count). For each `(i,j)`, accumulate `A[i][k] * B[k][j]`. Complexity: Let `m = A.rows()`, `n = A.cols()` (same as B.rows()), `p = B.cols()`. Time is O(m * n * p) because of the triple loop; space is O(m * p) for the result. Edge cases include: a 1x1 matrix multiply (trivial), multiplying by an identity-like matrix (works fine), and ensuring we don’t access out-of-bounds by using the validated dimensions. We also need to handle potential overflow, but for typical integer ranges and reasonable dimensions, standard `int` is fine; if larger values expected, use `long long`. The function should be `const`-correct by taking the input vectors as `const` references.
#include <vector>
#include <cstddef>

// Compute the product of two matrices. Returns an empty vector if dimensions are incompatible.
std::vector<std::vector<int>> matrixProduct(
    const std::vector<std::vector<int>>& A,
    const std::vector<std::vector<int>>& B
) {
    // Validate dimensions: number of columns of A must equal number of rows of B
    const std::size_t rowsA = A.size();
    const std::size_t colsA = A[0].size();
    const std::size_t rowsB = B.size();
    const std::size_t colsB = B[0].size();

    if (colsA != rowsB) {
        return {};
    }

    // Initialize result matrix with zeros
    std::vector<std::vector<int>> result(rowsA, std::vector<int>(colsB, 0));

    // Standard triple-loop matrix multiplication
    for (std::size_t i = 0; i < rowsA; ++i) {
        for (std::size_t j = 0; j < colsB; ++j) {
            for (std::size_t k = 0; k < colsA; ++k) {
                result[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    return result;
}
#include <cassert>
#include <vector>

// Include the solution here or link to it (declaration above)

int main() {
    // Test 1: 2x3 * 3x2 = 2x2
    std::vector<std::vector<int>> A1 = {{1,2,3},{4,5,6}};
    std::vector<std::vector<int>> B1 = {{7,8},{9,10},{11,12}};
    std::vector<std::vector<int>> R1 = matrixProduct(A1, B1);
    std::vector<std::vector<int>> expected1 = {{58,64},{139,154}};
    assert(R1 == expected1);

    // Test 2: 1x1 * 1x1
    std::vector<std::vector<int>> A2 = {{3}};
    std::vector<std::vector<int>> B2 = {{4}};
    assert(matrixProduct(A2, B2) == std::vector<std::vector<int>>{{12}});

    // Test 3: Incompatible dimensions (2x2 * 3x2) -> empty
    std::vector<std::vector<int>> A3 = {{1,2},{3,4}};
    std::vector<std::vector<int>> B3 = {{1,2},{3,4},{5,6}};
    assert(matrixProduct(A3, B3).empty());

    // Test 4: Multiply by identity (2x2 identity) -> same matrix
    std::vector<std::vector<int>> A4 = {{5,6},{7,8}};
    std::vector<std::vector<int>> I4 = {{1,0},{0,1}};
    assert(matrixProduct(A4, I4) == A4);

    // Test 5: Single row * single column -> scalar result as 1x1
    std::vector<std::vector<int>> A5 = {{1,2,3}};
    std::vector<std::vector<int>> B5 = {{4},{5},{6}};
    std::vector<std::vector<int>> R5 = matrixProduct(A5, B5);
    assert(R5 == std::vector<std::vector<int>>{{32}}); // 1*4 + 2*5 + 3*6 = 32

    // Test 6: Zero matrix times any compatible matrix -> zero matrix
    std::vector<std::vector<int>> Z = {{0,0,0},{0,0,0}}; // 2x3
    std::vector<std::vector<int>> C = {{1,2},{3,4},{5,6}}; // 3x2
    std::vector<std::vector<int>> R6 = matrixProduct(Z, C);
    std::vector<std::vector<int>> expected6 = {{0,0},{0,0}};
    assert(R6 == expected6);

    return 0;
}
