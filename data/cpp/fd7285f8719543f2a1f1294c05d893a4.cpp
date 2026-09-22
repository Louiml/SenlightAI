Write a C++ function that, given a square matrix represented as a `std::vector<std::vector<double>>`, sets the strictly upper triangular part (excluding the diagonal) to zero, sets the diagonal to 1, and leaves the strictly lower triangular part unchanged. Then, using the fact that the resulting matrix is lower-triangular, solve the linear system `L * x = b` for a given right-hand side vector `b` (also a `std::vector<double>`), where `L` is the lower-triangular matrix produced from the input. The function should return the solution vector `x`. The input matrix is guaranteed to be square and non-empty, and the diagonal elements after modification are all 1, so the system is always solvable (invertible). Handle edge cases such as a 1×1 matrix and ensure no division by zero.
// The task has two main parts: constructing a lower-triangular matrix with unit diagonal from an arbitrary square input, and then solving a lower-triangular system via forward substitution. First, iterate over all rows `i` and columns `j`. For `j > i`, set the element to 0. For `j == i`, set to 1. For `j < i`, keep the original value. This transformation runs in O(n²) time and uses O(1) extra space if we modify the input in place, but since we pass by const we create a copy. The forward substitution algorithm solves `L * x = b` row by row: for each row `i`, `x[i] = (b[i] - sum_{j=0}^{i-1} L[i][j] * x[j]) / L[i][i]`. Since `L[i][i] = 1`, the division is safe and can be omitted for speed but kept for generality. Time complexity is O(n²) for forward substitution, dominated by the sum over previous columns. Space complexity is O(n) for the output vector plus O(n²) if we copy the input matrix. Edge cases: 1×1 matrix yields `x[0] = b[0]`; empty matrix is not allowed by specification. The solution is numerically stable for well‑conditioned inputs, but no pivoting is needed because diagonal is all ones.
#include <vector>

// Given a square matrix, set upper-triangular part (above diagonal) to zero,
// diagonal to 1, keep lower-triangular part unchanged, then solve L*x = b.
std::vector<double> solveWithLowerTriangularFromUpper(
    const std::vector<std::vector<double>>& inputMatrix,
    const std::vector<double>& rhs) 
{
    const std::size_t n = inputMatrix.size();
    
    // Copy input and transform into lower-triangular with unit diagonal
    std::vector<std::vector<double>> L = inputMatrix;
    for (std::size_t i = 0; i < n; ++i) {
        for (std::size_t j = 0; j < n; ++j) {
            if (j > i) {
                L[i][j] = 0.0;
            } else if (j == i) {
                L[i][j] = 1.0;
            }
        }
    }
    
    // Forward substitution: L * x = rhs
    std::vector<double> x(n, 0.0);
    for (std::size_t i = 0; i < n; ++i) {
        double sum = rhs[i];
        for (std::size_t j = 0; j < i; ++j) {
            sum -= L[i][j] * x[j];
        }
        // L[i][i] is always 1, so no division needed
        x[i] = sum;
    }
    return x;
}
#include <cassert>
#include <cmath>
#include <vector>

// Function under test
std::vector<double> solveWithLowerTriangularFromUpper(
    const std::vector<std::vector<double>>& inputMatrix,
    const std::vector<double>& rhs);

int main() {
    // 1×1 matrix
    {
        std::vector<std::vector<double>> m = {{5.0}};
        std::vector<double> b = {3.0};
        auto x = solveWithLowerTriangularFromUpper(m, b);
        assert(x.size() == 1);
        assert(std::fabs(x[0] - 3.0) < 1e-9);
    }

    // 2×2 with upper part cleared
    {
        std::vector<std::vector<double>> m = {{4.0, 9.0}, {2.0, 7.0}};
        std::vector<double> b = {1.0, 3.0};
        // L = [[1,0],[2,1]], solve: x0=1, x1=3-2*1=1
        auto x = solveWithLowerTriangularFromUpper(m, b);
        assert(x.size() == 2);
        assert(std::fabs(x[0] - 1.0) < 1e-9);
        assert(std::fabs(x[1] - 1.0) < 1e-9);
    }

    // 3×3 with zeros already, verify exact solution
    {
        std::vector<std::vector<double>> m = {{0.0, 5.0, 5.0}, {0.0, 0.0, 3.0}, {0.0, 0.0, 0.0}};
        std::vector<double> b = {2.0, 4.0, 6.0};
        // L = I (identity) because all lower and diagonal become 0/1
        auto x = solveWithLowerTriangularFromUpper(m, b);
        assert(x.size() == 3);
        assert(std::fabs(x[0] - 2.0) < 1e-9);
        assert(std::fabs(x[1] - 4.0) < 1e-9);
        assert(std::fabs(x[2] - 6.0) < 1e-9);
    }

    // 3×3 with mixed values, hand-checked
    {
        std::vector<std::vector<double>> m = {{1.0, 2.0, 3.0}, {4.0, 5.0, 6.0}, {7.0, 8.0, 9.0}};
        std::vector<double> b = {10.0, 20.0, 30.0};
        // L = [[1,0,0],[4,1,0],[7,8,1]]
        // x0 = 10
        // x1 = 20 - 4*10 = -20
        // x2 = 30 - 7*10 - 8*(-20) = 30 -70 +160 = 120
        auto x = solveWithLowerTriangularFromUpper(m, b);
        assert(x.size() == 3);
        assert(std::fabs(x[0] - 10.0) < 1e-9);
        assert(std::fabs(x[1] - (-20.0)) < 1e-9);
        assert(std::fabs(x[2] - 120.0) < 1e-9);
    }

    // Negative values
    {
        std::vector<std::vector<double>> m = {{-1.0, 3.0}, {-2.0, 4.0}};
        std::vector<double> b = {-5.0, 7.0};
        // L = [[1,0],[-2,1]] => x0=-5, x1=7-(-2)*(-5)=7-10=-3
        auto x = solveWithLowerTriangularFromUpper(m, b);
        assert(std::fabs(x[0] - (-5.0)) < 1e-9);
        assert(std::fabs(x[1] - (-3.0)) < 1e-9);
    }
    return 0;
}
