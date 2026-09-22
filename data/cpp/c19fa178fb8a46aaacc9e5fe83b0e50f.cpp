In C++, write a free function named `upperTriangularMatrixVectorProduct` that accepts a square matrix `A` (represented as a `std::vector<std::vector<double>>` where all rows have equal length equal to the number of columns) and a vector `B` (represented as `std::vector<double>`) of the same size as the matrix dimension. The function must compute and return the product of the **upper triangular part** of matrix `A` (including the main diagonal) with vector `B`, i.e., for each row `i`, the result element `C[i] = Σ_{j=i}^{n-1} A[i][j] * B[j]`. The input matrix and vector must not be modified, and the function must handle edge cases including a 1×1 matrix, an empty matrix/vector (which should return an empty vector), and any square size `n ≥ 0`. Use `const` references for all inputs, and ensure the returned vector has exactly `n` elements. The solution must be self-contained, include only necessary headers, and avoid using any external libraries beyond the standard library.

// The core algorithm is straightforward: since we only need the upper triangular portion (including the diagonal), we iterate over each row `i` from 0 to n-1, and for each such row, iterate over columns `j` from `i` to n-1. For each pair `(i,j)`, we add the product `A[i][j] * B[j]` to `C[i]`. This directly mirrors the mathematical definition. For a 1×1 matrix, the diagonal element is included, so `C[0] = A[0][0] * B[0]`. For an empty matrix (n=0), we simply return an empty vector. Edge cases: (1) Ensure the input matrix is square by checking that every row has size equal to the number of columns (the dimension); if not, we could either throw an exception or, for simplicity, return an empty vector—but the task assumes valid input, so no validation is required, though we can include a guard for safety. (2) The vector `B` must have size `n`; again assumed valid. The complexity is O(n²) time because we sum over approximately half the matrix elements (upper triangle including diagonal), and O(n) space for the result vector (excluding the input storage). We do not modify inputs, so all parameters are `const` references.

#include <vector>

// Compute the product of the upper triangular part (including diagonal) of a square matrix A with vector B.
// Returns a vector C of the same size as B, where C[i] = sum_{j=i}^{n-1} A[i][j] * B[j].
std::vector<double> upperTriangularMatrixVectorProduct(const std::vector<std::vector<double>>& A, const std::vector<double>& B) {
    const size_t n = B.size();
    std::vector<double> C(n, 0.0);

    for (size_t i = 0; i < n; ++i) {
        // Only iterate over columns j >= i (upper triangular part including diagonal)
        for (size_t j = i; j < n; ++j) {
            C[i] += A[i][j] * B[j];
        }
    }

    return C;
}

#include <cassert>
#include <vector>
#include <cmath>

int main() {
    // Test 1: 3x3 matrix with known result
    std::vector<std::vector<double>> A1 = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    std::vector<double> B1 = {1, 1, 1};
    std::vector<double> C1 = upperTriangularMatrixVectorProduct(A1, B1);
    assert(C1.size() == 3);
    assert(std::abs(C1[0] - (1*1 + 2*1 + 3*1)) < 1e-9);
    assert(std::abs(C1[1] - (5*1 + 6*1)) < 1e-9);
    assert(std::abs(C1[2] - (9*1)) < 1e-9);

    // Test 2: 1x1 matrix
    std::vector<std::vector<double>> A2 = {{5}};
    std::vector<double> B2 = {7};
    std::vector<double> C2 = upperTriangularMatrixVectorProduct(A2, B2);
    assert(C2.size() == 1);
    assert(std::abs(C2[0] - 35) < 1e-9);

    // Test 3: Empty matrix/vector
    std::vector<std::vector<double>> A3;
    std::vector<double> B3;
    std::vector<double> C3 = upperTriangularMatrixVectorProduct(A3, B3);
    assert(C3.empty());

    // Test 4: 2x2 matrix where lower triangular elements are ignored
    std::vector<std::vector<double>> A4 = {{1, 2}, {3, 4}};
    std::vector<double> B4 = {10, 20};
    std::vector<double> C4 = upperTriangularMatrixVectorProduct(A4, B4);
    assert(C4.size() == 2);
    // C[0] = 1*10 + 2*20 = 50; C[1] = 4*20 = 80 (note: 3*10 is ignored)
    assert(std::abs(C4[0] - 50) < 1e-9);
    assert(std::abs(C4[1] - 80) < 1e-9);

    // Test 5: Larger matrix with negative values and zeros
    std::vector<std::vector<double>> A5 = {{-1, 0, 2}, {0, -3, 4}, {5, 6, -7}};
    std::vector<double> B5 = {1, -2, 3};
    std::vector<double> C5 = upperTriangularMatrixVectorProduct(A5, B5);
    assert(C5.size() == 3);
    // C[0] = (-1)*1 + 0*(-2) + 2*3 = 5
    // C[1] = (-3)*(-2) + 4*3 = 18
    // C[2] = (-7)*3 = -21
    assert(std::abs(C5[0] - 5) < 1e-9);
    assert(std::abs(C5[1] - 18) < 1e-9);
    assert(std::abs(C5[2] - (-21)) < 1e-9);
}
