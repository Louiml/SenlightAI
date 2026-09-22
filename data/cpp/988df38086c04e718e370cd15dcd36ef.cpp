// Write a C++ function named `matrixMultiply` that takes two 2D integer vectors, `A` (with dimensions rowsA × colsA) and `B` (with dimensions rowsB × colsB), and returns a 2D integer vector representing their product. The function must first validate that `colsA == rowsB`; if this condition is not met, it should return an empty vector `{}` to indicate an invalid multiplication. Assume all inputs contain at least one element, and the multiplication must follow standard matrix multiplication rules: the element at position (i, j) of the result is computed as the sum of `A[i][k] * B[k][j]` for k from 0 to colsA-1. The function should be `const`-correct (take const references) and use no global variables.
#include <cassert>
#include <vector>

// The solution function is declared above; assume it is included.

int main() {
    // Test 1: Simple 2x3 * 3x2
    std::vector<std::vector<int>> A1 = {{1,2,3},{4,5,6}};
    std::vector<std::vector<int>> B1 = {{7,8},{9,10},{11,12}};
    std::vector<std::vector<int>> R1 = matrixMultiply(A1, B1);
    std::vector<std::vector<int>> E1 = {{58,64},{139,154}};
    assert(R1 == E1);

    // Test 2: Identity multiplication (3x3 identity * same matrix)
    std::vector<std::vector<int>> I = {{1,0,0},{0,1,0},{0,0,1}};
    std::vector<std::vector<int>> M = {{2,3,4},{5,6,7},{8,9,10}};
    std::vector<std::vector<int>> R2 = matrixMultiply(I, M);
    assert(R2 == M);

    // Test 3: 1x1 * 1x1
    std::vector<std::vector<int>> A3 = {{5}};
    std::vector<std::vector<int>> B3 = {{7}};
    std::vector<std::vector<int>> R3 = matrixMultiply(A3, B3);
    std::vector<std::vector<int>> E3 = {{35}};
    assert(R3 == E3);

    // Test 4: Invalid dimensions (2x2 * 3x2) returns empty
    std::vector<std::vector<int>> A4 = {{1,2},{3,4}};
    std::vector<std::vector<int>> B4 = {{1,2},{3,4},{5,6}};
    std::vector<std::vector<int>> R4 = matrixMultiply(A4, B4);
    assert(R4.empty());

    // Test 5: Row vector * column vector (1x3 * 3x1) -> 1x1
    std::vector<std::vector<int>> A5 = {{1,2,3}};
    std::vector<std::vector<int>> B5 = {{4},{5},{6}};
    std::vector<std::vector<int>> R5 = matrixMultiply(A5, B5);
    std::vector<std::vector<int>> E5 = {{32}}; // 1*4 + 2*5 + 3*6 = 32
    assert(R5 == E5);

    // Test 6: Column vector * row vector (3x1 * 1x3) -> 3x3
    std::vector<std::vector<int>> A6 = {{1},{2},{3}};
    std::vector<std::vector<int>> B6 = {{4,5,6}};
    std::vector<std::vector<int>> R6 = matrixMultiply(A6, B6);
    std::vector<std::vector<int>> E6 = {{4,5,6},{8,10,12},{12,15,18}};
    assert(R6 == E6);

    // Test 7: Empty input (one matrix empty) returns empty
    std::vector<std::vector<int>> A7 = {};
    std::vector<std::vector<int>> B7 = {{1,2}};
    assert(matrixMultiply(A7, B7).empty());

    // Test 8: Zero matrix * non-zero (2x2 zeros * 2x2)
    std::vector<std::vector<int>> A8 = {{0,0},{0,0}};
    std::vector<std::vector<int>> B8 = {{1,2},{3,4}};
    std::vector<std::vector<int>> R8 = matrixMultiply(A8, B8);
    std::vector<std::vector<int>> E8 = {{0,0},{0,0}};
    assert(R8 == E8);
}
#include <vector>

// Multiplies two matrices A (rowsA x colsA) and B (rowsB x colsB).
// Returns an empty vector if colsA != rowsB (invalid multiplication).
std::vector<std::vector<int>> matrixMultiply(
    const std::vector<std::vector<int>>& A,
    const std::vector<std::vector<int>>& B
) {
    // Validate dimensions: cols of A must equal rows of B.
    if (A.empty() || B.empty() || A[0].size() != B.size()) {
        return {};
    }

    size_t rowsA = A.size();
    size_t colsA = A[0].size();
    size_t colsB = B[0].size();

    // Initialize result matrix with zeros.
    std::vector<std::vector<int>> result(rowsA, std::vector<int>(colsB, 0));

    // Standard matrix multiplication.
    for (size_t i = 0; i < rowsA; ++i) {
        for (size_t j = 0; j < colsB; ++j) {
            for (size_t k = 0; k < colsA; ++k) {
                result[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    return result;
}
// The solution involves nested loops to iterate over the rows of the first matrix and columns of the second matrix. For each result cell, we accumulate the dot product of the corresponding row of A and column of B. First, check dimension compatibility: if `A[0].size() != B.size()` (i.e., number of columns of A equals number of rows of B), return an empty vector. Otherwise, initialize the result matrix with dimensions `A.size()` rows and `B[0].size()` columns, all set to 0. Then use three nested loops: outer over rows of A, middle over columns of B, inner over the shared dimension. Time complexity is O(rowsA * colsB * sharedDim), where sharedDim = colsA = rowsB. Space complexity is O(rowsA * colsB) for the result matrix (ignoring input storage). Edge cases include single-element matrices (1×1 dot 1×1) and cases where the shared dimension is large; also handle non-square matrices normally.
