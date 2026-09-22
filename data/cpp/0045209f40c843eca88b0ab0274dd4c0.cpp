/*
Write a C++ function `matrixMultiply` that takes two square matrices of the same size (both dimensions equal to `n`) as vectors of vectors of integers, returns their product as a vector of vectors of integers. The function should perform standard row-by-column matrix multiplication, where each element `result[i][j]` is the sum of `A[i][k] * B[k][j]` for all `k` from 0 to `n-1`. Assume the input matrices are non-empty and square (i.e., number of rows equals number of columns, and each row has exactly that many elements). The function must not modify the input matrices.
*/

#include <vector>

// Multiply two square matrices A and B of size n x n.
// Returns the product matrix C = A * B.
std::vector<std::vector<int>> matrixMultiply(
    const std::vector<std::vector<int>>& A,
    const std::vector<std::vector<int>>& B) {
    
    int n = A.size(); // assumes A is non-empty and square
    std::vector<std::vector<int>> result(n, std::vector<int>(n, 0));
    
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            int sum = 0;
            for (int k = 0; k < n; ++k) {
                sum += A[i][k] * B[k][j];
            }
            result[i][j] = sum;
        }
    }
    
    return result;
}

#include <cassert>
#include <vector>

// Assume the solution function is defined above.
std::vector<std::vector<int>> matrixMultiply(
    const std::vector<std::vector<int>>& A,
    const std::vector<std::vector<int>>& B);

int main() {
    // Test 1: 1x1 matrix
    std::vector<std::vector<int>> A1 = {{3}};
    std::vector<std::vector<int>> B1 = {{4}};
    assert(matrixMultiply(A1, B1) == std::vector<std::vector<int>>({{12}}));

    // Test 2: 2x2 identity multiplication
    std::vector<std::vector<int>> A2 = {{1, 0}, {0, 1}};
    std::vector<std::vector<int>> B2 = {{5, 6}, {7, 8}};
    assert(matrixMultiply(A2, B2) == B2);

    // Test 3: 2x2 known product
    std::vector<std::vector<int>> A3 = {{1, 2}, {3, 4}};
    std::vector<std::vector<int>> B3 = {{5, 6}, {7, 8}};
    std::vector<std::vector<int>> R3 = {{19, 22}, {43, 50}};
    assert(matrixMultiply(A3, B3) == R3);

    // Test 4: 3x3 with zeros
    std::vector<std::vector<int>> A4 = {{0, 0, 0}, {1, 2, 3}, {0, 0, 0}};
    std::vector<std::vector<int>> B4 = {{9, 8, 7}, {6, 5, 4}, {3, 2, 1}};
    std::vector<std::vector<int>> R4 = {{0, 0, 0}, {30, 24, 18}, {0, 0, 0}};
    assert(matrixMultiply(A4, B4) == R4);

    // Test 5: 3x3 general case
    std::vector<std::vector<int>> A5 = {{2, -1, 0}, {0, 1, -3}, {4, 0, 5}};
    std::vector<std::vector<int>> B5 = {{1, 2, 3}, {0, -2, 1}, {2, 0, 4}};
    std::vector<std::vector<int>> R5 = {{2, 6, 5}, {-6, -2, -11}, {14, 8, 32}};
    assert(matrixMultiply(A5, B5) == R5);

    // Test 6: Matrix with negative numbers
    std::vector<std::vector<int>> A6 = {{-1, -2}, {3, 4}};
    std::vector<std::vector<int>> B6 = {{0, 1}, {2, -3}};
    std::vector<std::vector<int>> R6 = {{-4, 5}, {8, -9}};
    assert(matrixMultiply(A6, B6) == R6);

    // Test 7: 1x1 with zero
    std::vector<std::vector<int>> A7 = {{0}};
    std::vector<std::vector<int>> B7 = {{100}};
    assert(matrixMultiply(A7, B7) == std::vector<std::vector<int>>({{0}}));

    return 0;
}

// The core algorithm is the standard matrix multiplication algorithm for square matrices. For each output cell `(i, j)`, iterate over the shared dimension `k` (size `n`), accumulating the product of `A[i][k]` and `B[k][j]`. The result matrix is initialized with zeros of size `n x n`. Edge cases include `n = 1`, where the result is simply the single element `A[0][0] * B[0][0]`; the algorithm naturally handles this because the inner loop runs once. Since only square matrices are allowed, no dimension mismatch errors occur. Using `std::vector` ensures memory safety and automatic cleanup. The time complexity is `O(n^3)` because there are `n^2` output cells, each requiring `n` multiplications and additions. The auxiliary space complexity is `O(n^2)` for storing the result matrix; no extra space beyond the result is used other than loop variables. The inputs are passed by `const` reference to avoid copying and to guarantee they remain unchanged.
