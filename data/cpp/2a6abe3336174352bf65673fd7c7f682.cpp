// Write a standalone C++ function that takes two square matrices of equal size (represented as `std::vector<std::vector<double>>`) containing numeric values, and returns a new matrix (also `std::vector<std::vector<double>>`) that represents their Hadamard product (element-wise multiplication). The function should handle matrices with zero rows or columns gracefully (returning an empty matrix), and must not modify the input matrices. Assume all input matrices are properly rectangular (all rows have the same number of columns). The output matrix must have the same dimensions as the inputs, and each element at position (i, j) should be the product of the corresponding input elements: `out[i][j] = A[i][j] * B[i][j]`. Use `const` references for inputs and ensure the function is named `hadamardProduct`.
// The solution iterates over each row and column index of the input matrices. Since both matrices are guaranteed to have identical dimensions, we can safely index them with the same loop counters. For each cell, compute the product of the two corresponding elements and store it in a result matrix of the same size. Edge cases: (1) If either matrix is empty (size 0), return an empty matrix. (2) If either matrix has rows but zero columns (e.g., a 3x0 matrix), the result should also be an empty matrix or a matrix with zero columns—here, we simply return an empty `std::vector<std::vector<double>>` for any case where either dimension is zero. (3) Since inputs are `const`, we only read from them. Time complexity is \(O(n \times m)\) where \(n\) is the number of rows and \(m\) the number of columns, because we visit each cell exactly once. Space complexity is \(O(n \times m)\) for the result matrix, which is required to store the output.
#include <vector>

// Compute the Hadamard (element-wise) product of two square matrices.
// Returns an empty matrix if either input has zero rows or zero columns.
std::vector<std::vector<double>> hadamardProduct(
    const std::vector<std::vector<double>>& A,
    const std::vector<std::vector<double>>& B) {
    
    // If either matrix is empty or has rows with no columns, return empty result.
    if (A.empty() || B.empty() || A[0].empty() || B[0].empty()) {
        return {};
    }
    
    const std::size_t rows = A.size();
    const std::size_t cols = A[0].size();
    
    // Initialize result with the same dimensions.
    std::vector<std::vector<double>> result(rows, std::vector<double>(cols, 0.0));
    
    for (std::size_t i = 0; i < rows; ++i) {
        for (std::size_t j = 0; j < cols; ++j) {
            result[i][j] = A[i][j] * B[i][j];
        }
    }
    
    return result;
}
#include <cassert>
#include <vector>

// Declaration of the solution function (included here for self-contained test).
std::vector<std::vector<double>> hadamardProduct(
    const std::vector<std::vector<double>>& A,
    const std::vector<std::vector<double>>& B);

int main() {
    // Test 1: Basic 3x3 example (similar to uBLAS snippet values).
    std::vector<std::vector<double>> A = {
        {0.0, 1.0, 2.0},
        {3.0, 4.0, 5.0},
        {6.0, 7.0, 8.0}
    };
    std::vector<std::vector<double>> B = {
        {2.0, 2.0, 2.0},
        {2.0, 2.0, 2.0},
        {2.0, 2.0, 2.0}
    };
    auto result = hadamardProduct(A, B);
    assert(result.size() == 3);
    assert(result[0][0] == 0.0);
    assert(result[0][1] == 2.0);
    assert(result[2][2] == 16.0);
    
    // Test 2: Identity matrix product.
    std::vector<std::vector<double>> I = {
        {1.0, 0.0},
        {0.0, 1.0}
    };
    auto squared = hadamardProduct(I, I);
    assert(squared[0][0] == 1.0);
    assert(squared[1][1] == 1.0);
    assert(squared[0][1] == 0.0);
    assert(squared[1][0] == 0.0);
    
    // Test 3: Empty matrix.
    std::vector<std::vector<double>> empty;
    auto emptyResult = hadamardProduct(empty, empty);
    assert(emptyResult.empty());
    
    // Test 4: Matrix with zero columns.
    std::vector<std::vector<double>> zeroCols(2, std::vector<double>());
    auto zeroColsResult = hadamardProduct(zeroCols, zeroCols);
    assert(zeroColsResult.empty());
    
    // Test 5: Non-square but rectangular (2x3) with exact values.
    std::vector<std::vector<double>> C = {
        {1.0, 2.0, 3.0},
        {4.0, 5.0, 6.0}
    };
    std::vector<std::vector<double>> D = {
        {0.5, 0.5, 0.5},
        {0.5, 0.5, 0.5}
    };
    auto rectResult = hadamardProduct(C, D);
    assert(rectResult[0][0] == 0.5);
    assert(rectResult[1][2] == 3.0);
    
    // Test 6: Negative numbers.
    std::vector<std::vector<double>> NegA = {{-2.0, 3.0}};
    std::vector<std::vector<double>> NegB = {{4.0, -5.0}};
    auto negResult = hadamardProduct(NegA, NegB);
    assert(negResult[0][0] == -8.0);
    assert(negResult[0][1] == -15.0);
    
    return 0;
}
