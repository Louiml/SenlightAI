Write a standalone C++ function that, given two square matrices \(A\) and \(B\) of the same size \(n \times n\) (with \(n > 0\)), returns their product matrix \(C = A \times B\). The function must accept the matrices as `const` references to `std::vector<std::vector<int>>`, compute the standard matrix multiplication using three nested loops, and return the resulting matrix as `std::vector<std::vector<int>>`. The input matrices are guaranteed to be square and non‑empty, but you must not assume the entries are non‑negative; they can be any integers. Your function must not modify the input matrices and must handle the general case where \(n\) can be any positive integer. For the implementation, use the classic \(O(n^3)\) algorithm without any optimization (no tiling, no Strassen), and ensure the result matrix is correctly initialized to zeros before accumulating products. Edge cases include \(n = 1\), where the product is simply \(C[0][0] = A[0][0] \times B[0][0]\), and matrices with zeros or negative entries, which must be handled normally.
#include <cassert>
#include <vector>

// Assume the solution function is defined above (or included via header).

int main() {
    // Test 1: 1x1 matrix
    std::vector<std::vector<int>> A1 = {{3}};
    std::vector<std::vector<int>> B1 = {{7}};
    assert(matrixMultiply(A1, B1) == std::vector<std::vector<int>>({{21}}));

    // Test 2: 2x2 with positive numbers
    std::vector<std::vector<int>> A2 = {{1, 2}, {3, 4}};
    std::vector<std::vector<int>> B2 = {{5, 6}, {7, 8}};
    std::vector<std::vector<int>> expected2 = {{19, 22}, {43, 50}};
    assert(matrixMultiply(A2, B2) == expected2);

    // Test 3: 2x2 with negative numbers
    std::vector<std::vector<int>> A3 = {{-1, 2}, {0, -3}};
    std::vector<std::vector<int>> B3 = {{4, -5}, {-2, 6}};
    std::vector<std::vector<int>> expected3 = {{-8, 17}, {6, -18}};
    assert(matrixMultiply(A3, B3) == expected3);

    // Test 4: Identity matrix multiplication (3x3)
    std::vector<std::vector<int>> I = {{1,0,0}, {0,1,0}, {0,0,1}};
    std::vector<std::vector<int>> M = {{2,3,4}, {5,6,7}, {8,9,10}};
    assert(matrixMultiply(I, M) == M);
    assert(matrixMultiply(M, I) == M);

    // Test 5: 3x3 with zeros
    std::vector<std::vector<int>> A5 = {{0,0,1}, {0,1,0}, {1,0,0}};
    std::vector<std::vector<int>> B5 = {{1,2,3}, {4,5,6}, {7,8,9}};
    std::vector<std::vector<int>> expected5 = {{7,8,9}, {4,5,6}, {1,2,3}};
    assert(matrixMultiply(A5, B5) == expected5);

    // Test 6: All zeros result
    std::vector<std::vector<int>> A6 = {{0,0}, {0,0}};
    std::vector<std::vector<int>> B6 = {{1,2}, {3,4}};
    std::vector<std::vector<int>> expected6 = {{0,0}, {0,0}};
    assert(matrixMultiply(A6, B6) == expected6);

    // Test 7: Larger dimensions (4x4) random check with simple known formula
    std::vector<std::vector<int>> A7 = {{1,0,0,0}, {0,1,0,0}, {0,0,1,0}, {0,0,0,1}};
    std::vector<std::vector<int>> B7 = {{9,8,7,6}, {5,4,3,2}, {1,0,-1,-2}, {-3,-4,-5,-6}};
    assert(matrixMultiply(A7, B7) == B7);
    assert(matrixMultiply(B7, A7) == B7);

    return 0;
}
#include <vector>

// Multiplies two square matrices A and B of the same size n x n.
// Returns the resulting n x n matrix.
std::vector<std::vector<int>> matrixMultiply(const std::vector<std::vector<int>>& A,
                                             const std::vector<std::vector<int>>& B) {
    int n = static_cast<int>(A.size());
    // Initialize result matrix with zeros.
    std::vector<std::vector<int>> C(n, std::vector<int>(n, 0));

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            for (int k = 0; k < n; ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    return C;
}
// The solution is straightforward: define a function `matrixMultiply(const std::vector<std::vector<int>>& A, const std::vector<std::vector<int>>& B)` that first determines the dimension \(n\) from `A.size()`. Since both matrices are square and same size, we can assume `B.size() == n` and that each inner vector also has size `n`. The result matrix is initialized as an \(n \times n\) vector filled with zeros. Then three nested loops iterate: for each row `i` of `A`, each column `j` of `B`, and each inner index `k` (from 0 to \(n-1\)), we accumulate `C[i][j] += A[i][k] * B[k][j]`. This directly implements matrix multiplication. Edge cases: when `n == 1`, the loops still work correctly: for `i=0`, `j=0`, `k=0`, we get `C[0][0] = A[0][0]*B[0][0]`. Negative and zero values are handled naturally by integer arithmetic. Time complexity is \(O(n^3)\) due to three nested loops; space complexity is \(O(n^2)\) for the result matrix, plus constant extra space.
