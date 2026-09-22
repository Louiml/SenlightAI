Write a C++ function `std::vector<std::vector<int>> multiplyMatrices(const std::vector<std::vector<int>>& A, int rA, int cA, const std::vector<std::vector<int>>& B, int rB, int cB)` that takes two matrices A (size rA × cA) and B (size rB × cB), where it is guaranteed that cA == rB, and returns their product as a new matrix of size rA × cB. Each element of the product is the dot product of the corresponding row of A with the corresponding column of B. The matrices contain integers between -100 and 100 inclusive, and all dimensions are between 1 and 100. The function must not modify the input matrices, and the result must be returned by value. Assume the input is always valid, so no error handling for dimension mismatch is needed. The solution should be self-contained, include necessary headers, and be suitable for a beginner-level C++ exercise.
#include <cassert>
#include <vector>

// Function declaration from the solution
std::vector<std::vector<int>> multiplyMatrices(
    const std::vector<std::vector<int>>& A, int rA, int cA,
    const std::vector<std::vector<int>>& B, int rB, int cB);

int main() {
    // Example from the problem statement
    std::vector<std::vector<int>> A1 = {{1, 2}, {2, 1}};
    std::vector<std::vector<int>> B1 = {{3, 4}, {4, 3}};
    std::vector<std::vector<int>> res1 = multiplyMatrices(A1, 2, 2, B1, 2, 2);
    assert(res1.size() == 2 && res1[0].size() == 2);
    assert(res1 == std::vector<std::vector<int>>({{11, 10}, {10, 11}}));

    // 1x1 matrices
    std::vector<std::vector<int>> A2 = {{-5}};
    std::vector<std::vector<int>> B2 = {{3}};
    assert(multiplyMatrices(A2, 1, 1, B2, 1, 1) == std::vector<std::vector<int>>({{-15}}));

    // Rectangular: A (2x3), B (3x2)
    std::vector<std::vector<int>> A3 = {{1, 2, 3}, {4, 5, 6}};
    std::vector<std::vector<int>> B3 = {{7, 8}, {9, 10}, {11, 12}};
    std::vector<std::vector<int>> res3 = multiplyMatrices(A3, 2, 3, B3, 3, 2);
    assert(res3 == std::vector<std::vector<int>>({{58, 64}, {139, 154}}));

    // Zero matrix multiplication
    std::vector<std::vector<int>> A4 = {{0, 0}, {0, 0}};
    std::vector<std::vector<int>> B4 = {{5, -1}, {2, 3}};
    assert(multiplyMatrices(A4, 2, 2, B4, 2, 2) == std::vector<std::vector<int>>({{0, 0}, {0, 0}}));

    // Row vector times matrix (1x2) * (2x1)
    std::vector<std::vector<int>> A5 = {{2, -3}};
    std::vector<std::vector<int>> B5 = {{4}, {7}};
    assert(multiplyMatrices(A5, 1, 2, B5, 2, 1) == std::vector<std::vector<int>>({{-13}}));

    // Large values within range (100 and -100)
    std::vector<std::vector<int>> A6 = {{100, -100}};
    std::vector<std::vector<int>> B6 = {{100}, {100}};
    assert(multiplyMatrices(A6, 1, 2, B6, 2, 1) == std::vector<std::vector<int>>({{0}}));

    // Square 3x3 identity-like multiplication
    std::vector<std::vector<int>> A7 = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};
    std::vector<std::vector<int>> B7 = {{2, 3, 4}, {5, 6, 7}, {8, 9, 10}};
    assert(multiplyMatrices(A7, 3, 3, B7, 3, 3) == B7);

    // Negative values and mixed signs
    std::vector<std::vector<int>> A8 = {{-1, 2}, {3, -4}};
    std::vector<std::vector<int>> B8 = {{5, -6}, {-7, 8}};
    assert(multiplyMatrices(A8, 2, 2, B8, 2, 2) == std::vector<std::vector<int>>({{-19, 22}, {43, -50}}));

    // Dimensions: A (3x1), B (1x3) -> outer product
    std::vector<std::vector<int>> A9 = {{1}, {2}, {3}};
    std::vector<std::vector<int>> B9 = {{4, 5, 6}};
    std::vector<std::vector<int>> res9 = multiplyMatrices(A9, 3, 1, B9, 1, 3);
    assert(res9 == std::vector<std::vector<int>>({{4, 5, 6}, {8, 10, 12}, {12, 15, 18}}));

    return 0;
}
#include <vector>

// Multiply two matrices A (rA x cA) and B (rB x cB), where cA == rB.
// Returns the product as a new matrix of dimensions rA x cB.
std::vector<std::vector<int>> multiplyMatrices(
    const std::vector<std::vector<int>>& A, int rA, int cA,
    const std::vector<std::vector<int>>& B, int rB, int cB)
{
    // Dimensions are guaranteed consistent: cA == rB
    std::vector<std::vector<int>> result(rA, std::vector<int>(cB, 0));

    for (int r = 0; r < rA; ++r) {
        for (int c = 0; c < cB; ++c) {
            int sum = 0;
            for (int i = 0; i < cA; ++i) {
                sum += A[r][i] * B[i][c];
            }
            result[r][c] = sum;
        }
    }

    return result;
}
// The core algorithm is the standard matrix multiplication: for each row index r (0 ≤ r < rA) and each column index c (0 ≤ c < cB), compute the sum of products a[r][i] * b[i][c] for i from 0 to cA-1 (which equals rB-1). Since the matrices are passed as vectors of vectors, we use constant references to avoid copying, and we create the result matrix with size rA × cB initialized to zero. We then fill each element using nested loops. Edge cases: the minimum dimension is 1, so the triple loop always executes at least once; values can be negative, so the accumulator must handle signed integers; the result matrix size is determined by rA and cB, not by the original dimensions of B's rows/columns other than the inner dimension. Time complexity is O(rA × cB × cA), or equivalently O(rA × cB × rB) since cA == rB. Space complexity is O(rA × cB) for the result matrix, plus O(1) auxiliary for the accumulator (ignoring the input vectors' storage). For maximum dimensions (100×100 matrices), the triple loop runs 100^3 = 1,000,000 operations, which is trivial for 1 second.
