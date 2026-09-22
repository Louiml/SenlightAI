// Write a C++ function named `triangularMatrixVectorProduct` that takes three parameters: a constant reference to a square matrix `A` (represented as a `std::vector<std::vector<double>>`), a constant reference to a vector `B` (represented as a `std::vector<double>`), and a reference to an output vector `C` (also `std::vector<double>`). The function must compute `C += L * B`, where `L` is the lower-triangular part of `A` (including its diagonal). That is, for each row `i` and column `j` with `j <= i`, multiply `A[i][j]` by `B[j]` and add the product to `C[i]`. Elements above the diagonal are ignored. The input matrix is guaranteed to be square and the vector lengths match the matrix dimension. The function must not return anything and must not allocate extra containers for the result (compute in-place on `C`). Apply `const` correctness where applicable, and use size type `std::size_t` for indices.

#include <cassert>
#include <vector>
#include <cmath>

int main() {
    // Case 1: 3x3 matrix, standard lower triangle
    {
        std::vector<std::vector<double>> A = {
            {1.0, 0.0, 0.0},
            {2.0, 3.0, 0.0},
            {4.0, 5.0, 6.0}
        };
        std::vector<double> B = {1.0, 2.0, 3.0};
        std::vector<double> C = {0.0, 0.0, 0.0};
        triangularMatrixVectorProduct(A, B, C);
        std::vector<double> expected = {1.0, 8.0, 32.0}; // 1*1=1; 2*1+3*2=8; 4*1+5*2+6*3=32
        assert(C == expected);
    }

    // Case 2: Existing C values are preserved and added
    {
        std::vector<std::vector<double>> A = {
            {2.0, 0.0},
            {1.0, 4.0}
        };
        std::vector<double> B = {3.0, 2.0};
        std::vector<double> C = {10.0, 20.0};
        triangularMatrixVectorProduct(A, B, C);
        std::vector<double> expected = {16.0, 31.0}; // 10 + 2*3=16; 20 + 1*3+4*2=31
        assert(C == expected);
    }

    // Case 3: 1x1 matrix
    {
        std::vector<std::vector<double>> A = {{5.0}};
        std::vector<double> B = {7.0};
        std::vector<double> C = {1.0};
        triangularMatrixVectorProduct(A, B, C);
        assert(std::fabs(C[0] - 36.0) < 1e-12);
    }

    // Case 4: Empty matrix and vectors
    {
        std::vector<std::vector<double>> A;
        std::vector<double> B;
        std::vector<double> C;
        triangularMatrixVectorProduct(A, B, C);
        assert(C.empty());
    }

    // Case 5: Diagonal matrix (off-diagonal are zero but still in lower triangle)
    {
        std::vector<std::vector<double>> A = {
            {3.0, 0.0, 0.0},
            {0.0, 4.0, 0.0},
            {0.0, 0.0, 5.0}
        };
        std::vector<double> B = {2.0, 3.0, 4.0};
        std::vector<double> C = {0.0, 0.0, 0.0};
        triangularMatrixVectorProduct(A, B, C);
        std::vector<double> expected = {6.0, 12.0, 20.0};
        assert(C == expected);
    }

    // Case 6: Negative values
    {
        std::vector<std::vector<double>> A = {
            {-1.0, 0.0},
            {2.0, -3.0}
        };
        std::vector<double> B = {4.0, -2.0};
        std::vector<double> C = {0.0, 0.0};
        triangularMatrixVectorProduct(A, B, C);
        std::vector<double> expected = {-4.0, 14.0}; // -1*4 = -4; 2*4 + (-3)*(-2) = 8+6=14
        assert(C == expected);
    }

    // Case 7: Large dimension (5x5) sanity check
    {
        std::vector<std::vector<double>> A(5, std::vector<double>(5, 1.0)); // all entries 1, but we use lower triangle
        std::vector<double> B(5, 2.0);
        std::vector<double> C(5, 0.0);
        triangularMatrixVectorProduct(A, B, C);
        for (std::size_t i = 0; i < 5; ++i) {
            // For row i, columns 0..i each contribute 1*2 = 2, so sum = 2*(i+1)
            assert(std::fabs(C[i] - 2.0 * (i + 1)) < 1e-12);
        }
    }

    return 0;
}

#include <vector>
#include <cstddef>

// Compute C += L * B, where L is the lower-triangular part (including diagonal) of A.
// A is a square matrix, B and C are vectors of the same length as A's dimension.
void triangularMatrixVectorProduct(
    const std::vector<std::vector<double>>& A,
    const std::vector<double>& B,
    std::vector<double>& C)
{
    const std::size_t n = A.size();
    for (std::size_t i = 0; i < n; ++i) {
        double sum = C[i];  // preserve existing value of C[i]
        for (std::size_t j = 0; j <= i && j < B.size(); ++j) {
            sum += A[i][j] * B[j];
        }
        C[i] = sum;
    }
}

// The problem is a straightforward matrix-vector multiplication restricted to the lower triangle of the matrix. For each row `i` from `0` to `n-1`, you iterate columns `j` from `0` to `i` inclusive, and accumulate `A[i][j] * B[j]` into `C[i]`. This naturally uses the fact that `C` is updated in place, so the initial contents of `C` are preserved and added to. Edge cases: an empty matrix (dimension 0) should do nothing; a 1×1 matrix works fine. No special handling for negative numbers or zero entries is required—just standard multiplication and addition. Time complexity is O(n²) because it sums over roughly half the matrix entries (the lower triangle has n(n+1)/2 elements). Auxiliary space is O(1) beyond the input/output vectors themselves.
