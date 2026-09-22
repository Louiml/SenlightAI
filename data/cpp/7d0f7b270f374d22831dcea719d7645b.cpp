/*
Write a C++ function `matrixMultiply` that takes a square matrix `A` of size `n x n`, a square matrix `B` of size `n x n` (where `n` is between 1 and 10 inclusive), and an integer `n`, and returns a square matrix `C` of size `n x n` where `C[i][j]` is the dot product of row `i` of `A` with column `j` of `B`. The matrices are passed as `const` references to `std::array<std::array<int, 10>, 10>` (or a 2D vector if you prefer), but you must ensure the function works for any `n` up to 10. The result matrix must have the same dimensions. The function should not print anything; it should only compute and return the product.
*/
#include <array>
#include <cstddef>

// Compute the product of two n x n matrices (n <= 10).
// A and B are the input matrices; the function returns the product C = A * B.
std::array<std::array<int, 10>, 10> matrixMultiply(
    const std::array<std::array<int, 10>, 10>& A,
    const std::array<std::array<int, 10>, 10>& B,
    int n) {
    std::array<std::array<int, 10>, 10> C{};
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            C[i][j] = 0;
            for (int k = 0; k < n; ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    return C;
}
#include <cassert>

int main() {
    std::array<std::array<int, 10>, 10> A{};
    std::array<std::array<int, 10>, 10> B{};
    std::array<std::array<int, 10>, 10> C{};

    // Test 1: 1x1 identity-like
    A[0][0] = 3;
    B[0][0] = -2;
    C = matrixMultiply(A, B, 1);
    assert(C[0][0] == -6);

    // Test 2: 2x2 simple
    A[0][0] = 1; A[0][1] = 2;
    A[1][0] = 3; A[1][1] = 4;
    B[0][0] = 5; B[0][1] = 6;
    B[1][0] = 7; B[1][1] = 8;
    C = matrixMultiply(A, B, 2);
    assert(C[0][0] == 19);
    assert(C[0][1] == 22);
    assert(C[1][0] == 43);
    assert(C[1][1] == 50);

    // Test 3: 3x3 with zeros
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            A[i][j] = (i == j) ? 1 : 0;
            B[i][j] = (i + j) % 2;
        }
    }
    C = matrixMultiply(A, B, 3);
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            assert(C[i][j] == B[i][j]); // identity matrix

    // Test 4: 2x2 with negative numbers
    A[0][0] = -1; A[0][1] = 0;
    A[1][0] = 2; A[1][1] = -3;
    B[0][0] = 4; B[0][1] = -5;
    B[1][0] = 1; B[1][1] = 6;
    C = matrixMultiply(A, B, 2);
    assert(C[0][0] == -4);
    assert(C[0][1] == 5);
    assert(C[1][0] == 5);
    assert(C[1][1] == -28);

    // Test 5: All zeros
    for (int i = 0; i < 2; ++i)
        for (int j = 0; j < 2; ++j) {
            A[i][j] = 0;
            B[i][j] = 0;
        }
    C = matrixMultiply(A, B, 2);
    for (int i = 0; i < 2; ++i)
        for (int j = 0; j < 2; ++j)
            assert(C[i][j] == 0);

    return 0;
}
// The main algorithm is the standard matrix multiplication: for each row `i` and each column `j` of the result, compute the sum of products `A[i][k] * B[k][j]` for `k` from 0 to `n-1`. This is a triple nested loop. Edge cases: `n` may be 1, which works fine with the loops; matrices may contain zeros or negative integers, but the algorithm handles them normally. The total time complexity is `O(n^3)` because there are three nested loops each iterating up to `n` times. The space complexity is `O(n^2)` for the output matrix, which is required to store the result. No extra auxiliary space beyond the output matrix is needed.
