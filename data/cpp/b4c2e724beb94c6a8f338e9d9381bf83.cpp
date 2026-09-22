// Write a C++ function `double computeMatrixProductNorm(const std::vector<float>& A, const std::vector<float>& B, int n)` that computes the Frobenius norm of the matrix product \( C = A \times B \), where both \( A \) and \( B \) are \( n \times n \) matrices stored in row-major order (i.e., element \((i,j)\) is at index `i*n + j`). The function must return the square root of the sum of squares of all elements of \( C \). The inputs are guaranteed to be non-empty, with sizes exactly `n*n` each, and `n > 0`. The computation must be done without modifying the input vectors and using `double` precision for accumulation to reduce floating-point error. No parallelization is needed; a straightforward triple loop is expected.

// The solution computes each element \( C[i][j] = \sum_{k=0}^{n-1} A[i][k] \cdot B[k][j] \). For each row `i`, for each column `j`, we accumulate the sum of products over `k`. Since we need the Frobenius norm, we add `sum * sum` (the square of the element) to an accumulator `sumSquares`. To avoid reading the whole row of `B` repeatedly in an inefficient cache-unfriendly manner, we could use a temporary column slice, but for a clear and correct implementation, a direct triple loop suffices. Edge cases: `n=1` works trivially; empty inputs are excluded by the task; all values can be zero, giving norm 0. The algorithm runs in \( O(n^3) \) time and uses \( O(1) \) extra space (excluding the input storage). Use `std::inner_product` or manual loops; manual loops with `const` references to vectors are fine. Return `std::sqrt(sumSquares)`. Ensure the accumulator is `double` to avoid precision loss.

#include <vector>
#include <cmath>

// Compute the Frobenius norm of the matrix product C = A * B, where A and B are n x n matrices in row-major order.
// Precondition: A.size() == n*n and B.size() == n*n, n > 0.
double computeMatrixProductNorm(const std::vector<float>& A, const std::vector<float>& B, int n) {
    double sumSquares = 0.0;
    
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            double sum = 0.0;
            // Compute C[i][j] = sum_{k=0}^{n-1} A[i][k] * B[k][j]
            for (int k = 0; k < n; ++k) {
                sum += static_cast<double>(A[i * n + k]) * static_cast<double>(B[k * n + j]);
            }
            sumSquares += sum * sum;
        }
    }
    
    return std::sqrt(sumSquares);
}

#include <cassert>
#include <cmath>
#include <vector>

// The solution function (copy from above)
double computeMatrixProductNorm(const std::vector<float>& A, const std::vector<float>& B, int n);

int main() {
    // Test 1: n=1, A=[2], B=[3] -> C=[6], norm = sqrt(36) = 6
    {
        std::vector<float> A = {2.0f};
        std::vector<float> B = {3.0f};
        assert(std::fabs(computeMatrixProductNorm(A, B, 1) - 6.0) < 1e-6);
    }

    // Test 2: n=2, A = [[1,0],[0,0]], B = [[4,0],[0,0]] -> C = [[4,0],[0,0]], norm = 4
    {
        std::vector<float> A = {1.0f, 0.0f, 0.0f, 0.0f};
        std::vector<float> B = {4.0f, 0.0f, 0.0f, 0.0f};
        assert(std::fabs(computeMatrixProductNorm(A, B, 2) - 4.0) < 1e-6);
    }

    // Test 3: n=2, identity matrices -> C = identity, norm = sqrt(2)
    {
        std::vector<float> A = {1.0f, 0.0f, 0.0f, 1.0f};
        std::vector<float> B = {1.0f, 0.0f, 0.0f, 1.0f};
        assert(std::fabs(computeMatrixProductNorm(A, B, 2) - std::sqrt(2.0)) < 1e-6);
    }

    // Test 4: n=2, all zeros -> norm = 0
    {
        std::vector<float> A(4, 0.0f);
        std::vector<float> B(4, 0.0f);
        assert(computeMatrixProductNorm(A, B, 2) == 0.0);
    }

    // Test 5: n=3, A and B such that C has known values.
    // A = [[1,2,3],[0,0,0],[0,0,0]], B = [[1,0,0],[0,1,0],[0,0,1]] -> C = [[1,2,3],[0,0,0],[0,0,0]]
    // Norm = sqrt(1^2+2^2+3^2) = sqrt(14)
    {
        std::vector<float> A = {1.0f, 2.0f, 3.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
        std::vector<float> B = {1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f};
        assert(std::fabs(computeMatrixProductNorm(A, B, 3) - std::sqrt(14.0)) < 1e-6);
    }

    // Test 6: n=2, B is not identity, simple product.
    // A = [[1,2],[0,1]], B = [[1,0],[0,1]] -> C = A, norm = sqrt(1+4+0+1) = sqrt(6)
    {
        std::vector<float> A = {1.0f, 2.0f, 0.0f, 1.0f};
        std::vector<float> B = {1.0f, 0.0f, 0.0f, 1.0f};
        assert(std::fabs(computeMatrixProductNorm(A, B, 2) - std::sqrt(6.0)) < 1e-6);
    }

    return 0;
}
