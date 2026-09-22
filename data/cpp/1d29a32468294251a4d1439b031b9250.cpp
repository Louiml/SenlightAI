/*
Write a C++ function named `matrixSingularValues` that accepts a square matrix of real numbers (represented as a `std::vector<std::vector<double>>`) and returns a `std::vector<double>` containing the singular values of the matrix in non-increasing order. The function must handle matrices of size 0×0 (returning an empty vector), 1×1, and larger square matrices. For the computation, use the Eigen library's `JacobiSVD` with the `ComputeFullU` and `ComputeFullV` options. The returned vector should contain exactly the singular values (no vectors). Your function should be `const`-correct, take the input by `const&`, and be self-contained with all necessary Eigen includes.
*/

#include <vector>
#include <Eigen/SVD>

/**
 * Computes the singular values of a square matrix in non-increasing order.
 * @param matrix A square matrix represented as a vector of rows.
 * @return A vector of singular values, empty if the input is empty.
 */
std::vector<double> matrixSingularValues(const std::vector<std::vector<double>>& matrix) {
    const std::size_t n = matrix.size();
    if (n == 0) {
        return {};
    }

    // Convert to Eigen matrix
    Eigen::MatrixXd eigMat(n, n);
    for (std::size_t i = 0; i < n; ++i) {
        for (std::size_t j = 0; j < n; ++j) {
            eigMat(i, j) = matrix[i][j];
        }
    }

    // Compute SVD with full U and V (required for square matrices)
    Eigen::JacobiSVD<Eigen::MatrixXd> svd(eigMat, Eigen::ComputeFullU | Eigen::ComputeFullV);

    // Extract singular values
    const auto& sv = svd.singularValues();
    std::vector<double> result(sv.data(), sv.data() + sv.size());
    return result;
}

#include <cassert>
#include <cmath>
#include <vector>

// Function under test (declared here for completeness, but in practice included from solution)
std::vector<double> matrixSingularValues(const std::vector<std::vector<double>>& matrix);

int main() {
    // Test 1: 0x0 matrix
    {
        std::vector<std::vector<double>> m;
        auto sv = matrixSingularValues(m);
        assert(sv.empty());
    }

    // Test 2: 1x1 matrix
    {
        std::vector<std::vector<double>> m = {{-5.0}};
        auto sv = matrixSingularValues(m);
        assert(sv.size() == 1);
        assert(std::fabs(sv[0] - 5.0) < 1e-12);
    }

    // Test 3: 2x2 diagonal matrix
    {
        std::vector<std::vector<double>> m = {{3.0, 0.0}, {0.0, 4.0}};
        auto sv = matrixSingularValues(m);
        assert(sv.size() == 2);
        assert(std::fabs(sv[0] - 4.0) < 1e-12);
        assert(std::fabs(sv[1] - 3.0) < 1e-12);
    }

    // Test 4: 2x2 non-diagonal matrix with known SVD
    {
        std::vector<std::vector<double>> m = {{1.0, 2.0}, {3.0, 4.0}};
        auto sv = matrixSingularValues(m);
        // Singular values: sqrt( (25 ± sqrt(221))/2 ) ≈ 5.46499 and 0.36597
        assert(sv.size() == 2);
        assert(std::fabs(sv[0] - 5.4649857) < 1e-4);
        assert(std::fabs(sv[1] - 0.3659662) < 1e-4);
    }

    // Test 5: 3x3 zero matrix
    {
        std::vector<std::vector<double>> m = {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}};
        auto sv = matrixSingularValues(m);
        assert(sv.size() == 3);
        for (double v : sv) {
            assert(std::fabs(v) < 1e-12);
        }
    }

    // Test 6: 3x3 identity matrix
    {
        std::vector<std::vector<double>> m = {{1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}};
        auto sv = matrixSingularValues(m);
        assert(sv.size() == 3);
        for (double v : sv) {
            assert(std::fabs(v - 1.0) < 1e-12);
        }
    }

    return 0;
}

// The solution computes the singular value decomposition (SVD) of the input square matrix using Eigen's `JacobiSVD` solver. Since the input is square, the number of singular values equals the matrix dimension `n`. The singular values are obtained via `svd.singularValues()`, which returns a vector sorted in non-increasing order by default. Edge cases: for an empty matrix (0×0), the function returns an empty vector. For a 1×1 matrix, the singular value is the absolute value of the single element. The algorithm runs in \(O(n^3)\) time for an \(n \times n\) matrix due to the dense SVD computation, and uses \(O(n^2)\) auxiliary space for the matrices U and V (though we discard them). We initialize the result vector by copying the singular values from Eigen's vector and return it.
