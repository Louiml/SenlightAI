/*
Write a C++ function that takes two square matrices of equal size \(n \times n\), represented as `std::vector<std::vector<int>>`, and returns their product using the classic triple‑loop algorithm. The matrices contain integers in the range \(1\) to \(100\) (inclusive). The function must be const‑correct (accept `const` references), return the result matrix by value, and handle the general case where the input matrices are non‑empty. The function should not modify the input matrices. You may assume that all rows have the same length and both matrices are square. The function signature should be: `std::vector<std::vector<int>> multiplyMatrices(const std::vector<std::vector<int>>& A, const std::vector<std::vector<int>>& B);`
*/

#include <vector>

// Multiply two square matrices A and B of equal size n x n using the classic triple-loop algorithm.
std::vector<std::vector<int>> multiplyMatrices(const std::vector<std::vector<int>>& A, const std::vector<std::vector<int>>& B) {
    // If either matrix is empty, return an empty result.
    if (A.empty() || B.empty()) {
        return {};
    }

    const int n = static_cast<int>(A.size());      // Number of rows of A (also columns of B)
    const int m = static_cast<int>(B[0].size());   // Number of columns of B (same as rows of A)

    // Pre-allocate result matrix with n rows, each having m columns initialized to zero.
    std::vector<std::vector<int>> C(n, std::vector<int>(m, 0));

    // Classic matrix multiplication: C[i][j] = sum_k A[i][k] * B[k][j]
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            int sum = 0;
            for (int k = 0; k < n; ++k) { // k goes over the common dimension (n = number of rows of B)
                sum += A[i][k] * B[k][j];
            }
            C[i][j] = sum;
        }
    }

    return C;
}

#include <cassert>
#include <vector>

// The solution function is assumed to be declared above (in the same translation unit).
std::vector<std::vector<int>> multiplyMatrices(const std::vector<std::vector<int>>& A, const std::vector<std::vector<int>>& B);

int main() {
    // Test 1: 2x2 matrices
    std::vector<std::vector<int>> A1 = {{1, 2}, {3, 4}};
    std::vector<std::vector<int>> B1 = {{5, 6}, {7, 8}};
    std::vector<std::vector<int>> C1 = multiplyMatrices(A1, B1);
    assert(C1 == std::vector<std::vector<int>>({{19, 22}, {43, 50}}));

    // Test 2: 1x1 matrix
    std::vector<std::vector<int>> A2 = {{7}};
    std::vector<std::vector<int>> B2 = {{3}};
    std::vector<std::vector<int>> C2 = multiplyMatrices(A2, B2);
    assert(C2 == std::vector<std::vector<int>>({{21}}));

    // Test 3: Identity matrix multiplication
    std::vector<std::vector<int>> I = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};
    std::vector<std::vector<int>> A3 = {{2, -1, 4}, {0, 5, -3}, {6, 7, 8}};
    std::vector<std::vector<int>> C3 = multiplyMatrices(A3, I);
    assert(C3 == A3);

    // Test 4: Zero matrix multiplication
    std::vector<std::vector<int>> Z = {{0, 0}, {0, 0}};
    std::vector<std::vector<int>> A4 = {{1, 2}, {3, 4}};
    std::vector<std::vector<int>> C4 = multiplyMatrices(A4, Z);
    assert(C4 == Z);

    // Test 5: Empty input returns empty
    std::vector<std::vector<int>> E;
    std::vector<std::vector<int>> C5 = multiplyMatrices(E, E);
    assert(C5.empty());

    // Test 6: Larger 3x3 with random-like values
    std::vector<std::vector<int>> A6 = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    std::vector<std::vector<int>> B6 = {{9, 8, 7}, {6, 5, 4}, {3, 2, 1}};
    std::vector<std::vector<int>> C6 = multiplyMatrices(A6, B6);
    assert(C6 == std::vector<std::vector<int>>({{30, 24, 18}, {84, 69, 54}, {138, 114, 90}}));

    return 0;
}

// The solution follows the standard definition of matrix multiplication: for each row \(i\) of the first matrix and each column \(j\) of the second matrix, compute the dot product of row \(i\) of A with column \(j\) of B. This requires three nested loops: the outer loop iterates over rows of A, the middle loop over columns of B, and the inner loop over the common dimension (which is the number of rows of B, equal to the size of the matrices). Because the input is square, we can pre‑allocate the result matrix as a vector of vectors with the correct number of rows and fill each row with the appropriate number of zeros. Edge cases: if the matrices are empty (size 0), the result is an empty vector of vectors; the algorithm naturally handles this because the loops do not execute. The time complexity is \(O(n^3)\) for an \(n \times n\) matrix, and the space complexity is \(O(n^2)\) for the result matrix, plus \(O(1)\) auxiliary space for loop counters and temporary sums. Since inputs are const references, no copies are made, ensuring efficiency.
